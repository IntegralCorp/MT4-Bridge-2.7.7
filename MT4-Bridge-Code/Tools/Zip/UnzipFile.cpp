//+------------------------------------------------------------------+
//|                                         MetaTrader 5 NFA.Reports |
//|                   Copyright 2000-2019, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "UnzipFile.h"
#include "ZLib.h"
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CUnzipFile::CUnzipFile(void)
  {
   ZeroMemory(&m_info,sizeof(m_info));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CUnzipFile::~CUnzipFile(void)
  {
   Close();
  }
//+------------------------------------------------------------------+
//| ZIP file open                                                    |
//+------------------------------------------------------------------+
bool CUnzipFile::Open(LPCWSTR path)
  {
//--- check
   if(!path || *path==0)
      return(false);
   Close();
//--- open file
   if(!m_file.Open(path,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,OPEN_EXISTING))
      return(false);
//--- find central catalogue
   UINT64 central_pos=0;
   UINT   uL         =0;
   UINT64 u64_data   =0;
   if((central_pos=FindByMagic(SZlib::ENDOFCENTRALDIRMAGIC))==0)
     {
      m_file.Close();
      return(false);
     }
//--- seek
   if(m_file.Seek(central_pos,FILE_BEGIN)!=central_pos)
     {
      m_file.Close();
      return(false);
     }
//--- the signature, already checked but check it
   GetLong(uL);
   if(uL!=SZlib::ENDOFCENTRALDIRMAGIC)
     {
      m_file.Close();
      return(false);
     }
//--- 
   UINT number_disk=0;
   UINT number_disk_with_CD=0;
   UINT number_entry_CD=0;

   GetShort(number_disk);               // number of the current disk, used for spaning ZIP, unsupported, always 0
   GetShort(number_disk_with_CD);       // number the the disk with central dir, used for spaning ZIP, unsupported, always 0
   GetShort(m_info.gi.entries);         // number of the disk with the start of the central directory
   GetShort(number_entry_CD);           // total number of entries in the central dir (same than number_entry on nospan)
//--- check disks
   if(number_entry_CD!=m_info.gi.entries || number_disk_with_CD!=0 || number_disk!=0)
     {
      m_file.Close();
      return(false);
     }
//---
   GetLong(m_info.size_cdir);           // size of the central directory
   GetLong((UINT&)m_info.offset_cdir);  // offset of start of central directory with respect to the starting disk number
   GetShort(m_info.gi.size_comment);    // zipfile comment length
//--- check offset_cdir
   if(m_info.offset_cdir==UINT_MAX)
     {
      central_pos=0; uL=0;
      //--- find real size in zip64 end of central directory
      if((central_pos=FindByMagic(SZlib::ZIP64EOCD_LOCATORMAGIC))==0)
        {
         m_file.Close();
         return(false);
        }
      //--- seek
      if(m_file.Seek(central_pos,FILE_BEGIN)!=central_pos)
        {
         m_file.Close();
         return(false);
        }
      //--- get and check zip64 locator
      GetLong(uL);
      if(uL!=SZlib::ZIP64EOCD_LOCATORMAGIC)
        {
         m_file.Close();
         return(false);
        }
      //--- number of the disk with the start of the zip64 end of central directory  
      GetLong(number_disk_with_CD);
      //--- relative offset of the zip64 end of central directory record
      GetUInt64(u64_data);
      if(u64_data==0 || (central_pos=m_file.Seek(u64_data,FILE_BEGIN))!=u64_data)
        {
         m_file.Close();
         return(false);
        }
      //--- check signature
      GetLong(uL);
      if(uL!=SZlib::ZIP64ENDOFCENTDIRMAGIC)
        {
         m_file.Close();
         return(false);
        }
      //--- read offset from zip64 end of central directory
      if(m_file.Seek(central_pos+SZlib::OFFSET_ZIP64EOCD_CDOFF,FILE_BEGIN)!=central_pos+SZlib::OFFSET_ZIP64EOCD_CDOFF)
        {
         m_file.Close();
         return(false);
        }
      GetUInt64(m_info.offset_cdir);
     }
//---
   if(central_pos<m_info.offset_cdir+m_info.size_cdir)
     {
      m_file.Close();
      return(false);
     }
//---
   m_info.byte_before=central_pos-(m_info.offset_cdir+m_info.size_cdir);
   m_info.central_pos=central_pos;
//--- seek to first file
   return(FileFirst());
  }
//+------------------------------------------------------------------+
//| ZIP file close                                                   |
//+------------------------------------------------------------------+
void CUnzipFile::Close(void)
  {
   FileClose();
   m_file.Close();
  }
//+------------------------------------------------------------------+
//| ZIP file info                                                    |
//+------------------------------------------------------------------+
bool CUnzipFile::Info(UnzipInfo& info)
  {
   memcpy(&info,&m_info.gi,sizeof(UnzipInfo));
   return(true);
  }
//+------------------------------------------------------------------+
//| Seek to first file                                               |
//+------------------------------------------------------------------+
bool CUnzipFile::FileFirst(void)
  {
   m_info.pos_in_cdir=m_info.offset_cdir;
   m_info.num_file   =0;
//--- get first file info
   if(!CurFileInfoInt(m_info.info,m_info.offset_curfile,NULL,0))
      return(false);
//---
   m_info.file_ok=1;
   return(true);
  }
//+------------------------------------------------------------------+
//| Seek to next file                                                |
//+------------------------------------------------------------------+
bool CUnzipFile::FileNext(void)
  {
//--- checks
   if(!m_info.file_ok || m_info.num_file+1==m_info.gi.entries)
      return(false);
//--- seek
   m_info.pos_in_cdir+=SIZECENTRALDIRITEM+m_info.info.size_filename+m_info.info.size_extra+m_info.info.size_comment;
   m_info.num_file++;
//--- get file info
   return(m_info.file_ok=CurFileInfoInt(m_info.info,m_info.offset_curfile,NULL,0));
  }
//+------------------------------------------------------------------+
//| Open file in zip                                                 |
//+------------------------------------------------------------------+
bool CUnzipFile::FileOpen(void)
  {
   UINT  size_var;
   UINT  extra_offset;  // offset of the local extra field
   UINT  extra_size;    // size of the local extra field
   int   err;
//--- checks
   if(!m_info.file_ok)
      return(false);
//--- 
   FileClose();
   if(!CheckHeader(size_var,extra_offset,extra_size))
      return(false);
   if(!FileInZipAlloc())
      return(false);

   m_info.pzfile->loc_extra_offset=extra_offset;
   m_info.pzfile->loc_extra_size=extra_size;
   if((m_info.info.method!=0) && (m_info.info.method!=SZlib::Z_DEFLATED))
      return(false);
//---
   m_info.pzfile->crc32_wait =m_info.info.crc;
   m_info.pzfile->method     =m_info.info.method;
   m_info.pzfile->byte_before=m_info.byte_before;
//---
   if(m_info.info.method!=0)
     {
      if((err=SZlib::inflateInit2(&m_info.pzfile->stream,-SZlib::MAX_WBITS))==SZlib::Z_OK)
         m_info.pzfile->initialised=1;
      else
         if((err!=SZlib::Z_OK) && (err!=SZlib::Z_NEED_DICT))
            return(FALSE);
     }
//---
   m_info.pzfile->read_comp      =m_info.info.comp_size;
   m_info.pzfile->read_uncomp    =m_info.info.uncomp_size;
   m_info.pzfile->position       =m_info.offset_curfile+SIZEZIPLOCALHEADER+size_var;
   m_info.pzfile->stream.avail_in=0;
   return(true);
  }
//+------------------------------------------------------------------+
//| Close file in ZIP                                                |
//+------------------------------------------------------------------+
bool CUnzipFile::FileClose(void)
  {
//--- checks
   if(!m_info.pzfile)
      return(false);
//--- check crc
   if(m_info.pzfile->read_uncomp==0)
      if(m_info.pzfile->crc32!=m_info.pzfile->crc32_wait)
         return(false);
//--- finish deflate
   if(m_info.pzfile->initialised) SZlib::inflateEnd(&m_info.pzfile->stream);
//--- free data
   FileInZipFree();
   return(true);
  }
//+------------------------------------------------------------------+
//| Read file from ZIP                                               |
//+------------------------------------------------------------------+
UINT CUnzipFile::FileRead(void *buffer,UINT size)
  {
//--- 
   if(buffer==NULL || size<1 || m_info.pzfile==NULL)
      return(0);
//---
   m_info.pzfile->stream.next_out =(char*)buffer;
   m_info.pzfile->stream.avail_out=(UINT)size;

   if(size>m_info.pzfile->read_uncomp)
      m_info.pzfile->stream.avail_out=(UINT)m_info.pzfile->read_uncomp;
//---
   UINT iRead=0;

   while(m_info.pzfile->stream.avail_out>0)
     {
      if((m_info.pzfile->stream.avail_in==0) && (m_info.pzfile->read_comp>0))
        {
         UINT uReadThis=32768;
         //---
         if(m_info.pzfile->read_comp<uReadThis) uReadThis=(UINT)m_info.pzfile->read_comp;
         if(uReadThis==0)
            return(0);
         //---
         if(m_file.Seek(m_info.pzfile->position+m_info.pzfile->byte_before,FILE_BEGIN)!=m_info.pzfile->position+m_info.pzfile->byte_before)
            return(0);
         //---
         if(m_file.Read(m_info.pzfile->buffer,uReadThis)!=uReadThis)
            return(0);
         //---
         m_info.pzfile->position       +=uReadThis;
         m_info.pzfile->read_comp     -=uReadThis;
         m_info.pzfile->stream.next_in  =(char*)m_info.pzfile->buffer;
         m_info.pzfile->stream.avail_in =(UINT)uReadThis;
        }
      if(m_info.pzfile->method==0)
        {
         UINT uDoCopy;
         if(m_info.pzfile->stream.avail_out<m_info.pzfile->stream.avail_in)
            uDoCopy=m_info.pzfile->stream.avail_out;
         else
            uDoCopy=m_info.pzfile->stream.avail_in;
         //---
         memcpy(m_info.pzfile->stream.next_out,m_info.pzfile->stream.next_in,uDoCopy);
         m_info.pzfile->crc32=SZlib::crc32(m_info.pzfile->crc32,(unsigned char*)(m_info.pzfile->stream.next_out),uDoCopy);
         m_info.pzfile->read_uncomp     -=uDoCopy;
         m_info.pzfile->stream.avail_in -=uDoCopy;
         m_info.pzfile->stream.avail_out-=uDoCopy;
         m_info.pzfile->stream.next_out +=uDoCopy;
         m_info.pzfile->stream.next_in  +=uDoCopy;
         m_info.pzfile->stream.total_out+=uDoCopy;
         iRead+=uDoCopy;
        }
      else
        {
         UINT64     out_before  =m_info.pzfile->stream.total_out;
         const BYTE *buf_before =(BYTE*)m_info.pzfile->stream.next_out;
         int         err        =SZlib::inflate(&m_info.pzfile->stream,SZlib::Z_SYNC_FLUSH);
         UINT64      out_after  =m_info.pzfile->stream.total_out;
         UINT        out_this   =(UINT)(out_after-out_before);
         //---
         m_info.pzfile->crc32=SZlib::crc32(m_info.pzfile->crc32,buf_before,(UINT)(out_this));
         //---
         m_info.pzfile->read_uncomp-=out_this;
         iRead+=(UINT)(out_after-out_before);

         if(err==SZlib::Z_STREAM_END)
            return iRead;
         if((err!=SZlib::Z_OK) && (err!=SZlib::Z_NEED_DICT))
            return(0);
        }
     }
//---
   return(iRead);
  }
//+------------------------------------------------------------------+
//| Get ZIP file info                                                |
//+------------------------------------------------------------------+
bool CUnzipFile::FileInfo(UnzipFileInfo& info,LPWSTR name,UINT name_size)
  {
   UINT temp;
   return(CurFileInfoInt(info,temp,name,name_size));
  }
//+------------------------------------------------------------------+
//| Find data start                                                  |
//+------------------------------------------------------------------+
UINT64 CUnzipFile::FindByMagic(const UINT magic_number)
  {
   UINT64 uMaxBack =0xffff;
   UINT64 uSizeFile=m_file.Size();
   UINT64 uBackRead=4;
   UINT64 uPosFound=0;
   BYTE   buf[BUFREADCOMMENT+4];
   BYTE   nums[4]={0};
//--- 
   nums[0]=magic_number&0xff;
   nums[1]=(magic_number>>8)&0xff;
   nums[2]=(magic_number>>16)&0xff;
   nums[3]=(magic_number>>24)&0xff;
//---
   if(uMaxBack>uSizeFile)
      uMaxBack=uSizeFile;
//---
   while((uBackRead<uMaxBack) && !uPosFound)
     {
      UINT64 uReadSize,uReadPos;
      //---
      if(uBackRead+BUFREADCOMMENT>uMaxBack) uBackRead =uMaxBack;
      else                                  uBackRead+=BUFREADCOMMENT;
      //---
      uReadPos =uSizeFile-uBackRead;
      uReadSize=((BUFREADCOMMENT+4)<(uSizeFile-uReadPos))?(BUFREADCOMMENT+4):(UINT)(uSizeFile-uReadPos);
      //--- 
      if(m_file.Seek(uReadPos,FILE_BEGIN)!=uReadPos)
         return(0);
      //---
      if(m_file.Read(buf,(DWORD)uReadSize)!=uReadSize)
         return(0);
      //---
      for(int i=(int)uReadSize-3;(i--)>0;)
        {
         if(((*(buf+i))==nums[0]) && ((*(buf+i+1))==nums[1]) && ((*(buf+i+2))==nums[2]) && ((*(buf+i+3))==nums[3]))
           {
            uPosFound=uReadPos+i;
            break;
           }
        }
     }
//---
   return(uPosFound);
  }
//+------------------------------------------------------------------+
//| Get file info                                                    |
//+------------------------------------------------------------------+
bool CUnzipFile::CurFileInfoInt(UnzipFileInfo& info,UINT& offset,LPWSTR name,UINT name_size)
  {
   char   buffer[256]={0};
   UINT64 pos=m_info.pos_in_cdir+m_info.byte_before;
//--- 
   ZeroMemory(&info,sizeof(info));
//--- 
   if(m_file.Seek(pos,FILE_BEGIN)!=pos)
      return(false);
//--- we check the magic
   UINT uMagic=0;

   GetLong(uMagic);
   if(uMagic!=SZlib::CENTRALHEADERMAGIC)
      return(false);
//---
   GetShort(info.version);
   GetShort(info.version_needed);
   GetShort(info.flag);
   GetShort(info.method);
   GetLong (info.dosDate);
//---
   FILETIME ft;

   DosDateTimeToFileTime(HIWORD(info.dosDate),LOWORD(info.dosDate),&ft);
   FileTimeToSystemTime(&ft,&(info.tmu_date));
//---
   GetLong (info.crc);
   GetLong ((UINT&)info.comp_size);
   GetLong ((UINT&)info.uncomp_size);
   GetShort(info.size_filename);
   GetShort(info.size_extra);
   GetShort(info.size_comment);
   GetShort(info.disk_start);
   GetShort(info.int_fa);
   GetLong (info.ext_fa);
   GetLong (offset);
//--- 
   if(name)
     {
      //--- 
      UINT uSizeRead=_countof(buffer)-1;
      //--- 
      uSizeRead=min(uSizeRead,info.size_filename);
      //--- 
      if(info.size_filename>0 && name_size>0)
        {
         //--- 
         if(m_file.Read(buffer,uSizeRead)!=uSizeRead)
            return(false);
         //--- 
         buffer[uSizeRead]=0;
         //--- Bit 11: Language encoding flag (EFS). If this bit is set, the filename and comment fields for this file must be encoded using UTF-8.
         if(info.flag&0x800)
           {
            //--- check size
            UINT len=(UINT)::MultiByteToWideChar(CP_UTF8,0,buffer,uSizeRead,NULL,0);
            //--- convert from utf8
            ::MultiByteToWideChar(CP_UTF8,0,buffer,uSizeRead,name,name_size);
            //--- terminate
            if(len<name_size)
               name[len]=0;
           }
         else
           {
            CMTStr256 temp;
            temp.Assign(buffer);
            //--- replace file system delimiters to Windows compatible
            temp.ReplaceChar(L'/',L'\\');
            temp.Copy(name,name_size);
           }
        }
     }
//--- check file size
   if((info.uncomp_size==UINT_MAX) || (info.comp_size==UINT_MAX))
     {
      UINT header_id=0,data_size=0;
      //--- 
      pos+=SZlib::OFFSET_CENTDIR_FNAME+info.size_filename;
      //--- find extend size sign
      for(UINT64 i=pos;i<pos+info.size_extra;i+=(data_size+4))
        {
         if(m_file.Seek(i,FILE_BEGIN)!=i)
            return(false);
         //---
         GetShort(header_id);
         GetShort(data_size);
         if(header_id==SZlib::EXTRAHEADERID_ZIP64)
            break;
        }
      if(header_id!=SZlib::EXTRAHEADERID_ZIP64)
         return(false);
      //--- read from extend size part?
      if(info.uncomp_size==UINT_MAX)
        {
         if(data_size<8)
            return(false);
         data_size-=8;
         GetUInt64(info.uncomp_size);
        }
      //--- read from extend size part?
      if(info.comp_size==UINT_MAX)
        {
         if(data_size<8)
            return(false);
         data_size-=8;
         GetUInt64(info.comp_size);
        }
     }
//--- все хорошо
   return(true);
  }
//+------------------------------------------------------------------+
//| Header check                                                     |
//+------------------------------------------------------------------+
bool CUnzipFile::CheckHeader(UINT& size_var,UINT& extra_offset,UINT& extra_size)
  {
   UINT64 pos=m_info.offset_curfile+m_info.byte_before;
   UINT   uData=0,uMagic=0,uFlags=0;
//--- zero all
   size_var=extra_offset=extra_size=0;
//--- 
   if(m_file.Seek(pos,FILE_BEGIN)!=pos)
      return(false);
//--- 
   GetLong(uMagic);
   if(uMagic!=SZlib::LOCALHEADERMAGIC)
      return(false);
//--- 
   GetShort(uData);
//--- 
   GetShort(uFlags);
//--- 
   GetShort(uData);
   if(uData!=m_info.info.method)
      return(false);
//--- 
   if((m_info.info.method!=0) && (m_info.info.method!=SZlib::Z_DEFLATED))
      return(false);
//--- 
   GetLong(uData);
//--- crc
   GetLong(uData);
   if((uData!=m_info.info.crc) && ((uFlags&8)==0))
      return(false);
//--- size compr
   UINT64 compr_size=0,uncompr_size=0;
   GetLong((UINT&)compr_size);
//--- size uncompr
   GetLong ((UINT&)uncompr_size);
//--- 
   UINT size_filename; GetShort(size_filename);
   if((size_filename!=m_info.info.size_filename))
      return(false);
   size_var=(UINT)size_filename;
//--- extra field
   UINT size_extra_field=0;
   GetShort(size_extra_field);
//---
   extra_offset=m_info.offset_curfile+SIZEZIPLOCALHEADER+size_filename;
   extra_size  =(UINT)size_extra_field;
   size_var   +=(UINT)size_extra_field;
//--- get real file size
   if((compr_size==UINT_MAX) || (uncompr_size==UINT_MAX))
     {
      UINT data_size=0, header_id=0;
      //--- find real sizes
      for(UINT64 i=extra_offset;i<extra_offset+size_extra_field;i+=(data_size+4))
        {
         if(m_file.Seek(i,FILE_BEGIN)!=i)
            return(false);
         //---
         GetShort(header_id);
         GetShort(data_size);
         if(header_id==SZlib::EXTRAHEADERID_ZIP64)
            break;
        }
      //--- check zip64 field
      if(header_id!=SZlib::EXTRAHEADERID_ZIP64)
         return(false);
      //--- read from extend size part?
      if(uncompr_size==UINT_MAX)
        {
         if(data_size<8)
            return(false);
         GetUInt64(uncompr_size);
         data_size-=8;
        }
      //--- read from extend size part?
      if(compr_size==UINT_MAX)
        {
         if(data_size<8)
            return(false);
         GetUInt64(compr_size);
         data_size-=8;
        }
     }
//--- check sizes
   if((compr_size  !=m_info.info.comp_size) && ((uFlags&8)==0))
      return(false);
   if((uncompr_size!=m_info.info.uncomp_size) && ((uFlags&8)==0))
      return(false);
//---
   return(true);
  }
//+------------------------------------------------------------------+
//| Read byte                                                        |
//+------------------------------------------------------------------+
bool CUnzipFile::GetByte(UINT& value)
  {
   UCHAR x;
   if(m_file.Read(&x,sizeof(x))!=sizeof(x))
     {
      value=0;
      return(false);
     }
   value=x;
   return(true);
  }
//+------------------------------------------------------------------+
//| Read short                                                       |
//+------------------------------------------------------------------+
bool CUnzipFile::GetShort(UINT& value)
  {
   UINT a,b;
   if(GetByte(a) && GetByte(b))
     {
      value=a+(b<<8);
      return(true);
     }
   value=0;
   return(false);
  }
//+------------------------------------------------------------------+
//| Read long                                                        |
//+------------------------------------------------------------------+
bool CUnzipFile::GetLong(UINT& value)
  {
   UINT a,b;
   if(GetShort(a) && GetShort(b))
     {
      value=a+(b<<16);
      return(true);
     }
   value=0;
   return(false);
  }
//+------------------------------------------------------------------+
//| Read long-long                                                   |
//+------------------------------------------------------------------+
bool CUnzipFile::GetUInt64(UINT64& value)
  {
   UINT   a=0;
   UINT64 b=0;
   if(GetLong(a) && GetLong((UINT&)b))
     {
      value=a+(b<<32);
      return(true);
     }
   value=0;
   return(false);
  }
//+------------------------------------------------------------------+
//| ZIP file info allocation                                         |
//+------------------------------------------------------------------+
bool CUnzipFile::FileInZipAlloc(void)
  {
   FileInZipFree();
//---
   if((m_info.pzfile=new(std::nothrow) FileInZip)==NULL)
      return(false);
   ZeroMemory(m_info.pzfile,sizeof(FileInZip));
//--- 
   if((m_info.pzfile->buffer=new(std::nothrow) char[33000])==NULL)
      return(false);
   return(true);
  }
//+------------------------------------------------------------------+
//| ZIP file info free                                               |
//+------------------------------------------------------------------+
void CUnzipFile::FileInZipFree(void)
  {
   if(m_info.pzfile)
     {
      if(m_info.pzfile->buffer)
        {
         delete[] m_info.pzfile->buffer;
         m_info.pzfile->buffer=NULL;
        }
      delete m_info.pzfile;
      m_info.pzfile=NULL;
     }
  }
//+------------------------------------------------------------------+