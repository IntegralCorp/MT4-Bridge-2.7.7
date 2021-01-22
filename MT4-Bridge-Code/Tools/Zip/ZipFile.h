//+------------------------------------------------------------------+
//|                                         MetaTrader 5 NFA.Reports |
//|                   Copyright 2000-2019, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "Zlib.h"
//+------------------------------------------------------------------+
//| ZIP file                                                         |
//+------------------------------------------------------------------+
class CZipFile
  {
private:
   enum constants
     {
      ZIP_READ_CHUNK       =4*1024*1024,
      SIZEDATA_INDATABLOCK =4080,
      //--- magic numbers
      CENTRALHEADERMAGIC   =0x02014b50,
      LOCALHEADERMAGIC     =0x04034b50,
      ENDHEADERMAGIC       =0x06054b50,
      //--- versions
      VERSIONMADEBY        =0x0,
      EXTRACTVERSION       =20,
      EXTRACTVERSION_ZIP64 =45,
      //--- size of headers
      SIZECENTRALHEADER    =0x2e,
      SIZEEXTRAFIELD_CD    =20,
      SIZEZIP64ENDOFCENTDIR=44,
     };
   //---
   struct ZipFileInfo
     {
      SYSTEMTIME        tmzDate;
      UINT              dosDate;                // if dos_date==0, tmu_date is used
      UINT              internal_fa;            // internal file attributes
      UINT              external_fa;            // external file attributes
      wchar_t           file_path[512];
      UINT64            file_size;              // size of current file for init zip64
     };
   //---
   struct ZipFileDataBlock
     {
      UINT              avail_in_this_block;
      UINT              filled_in_this_block;
      UINT              unused;                 // for future use and alignement
      BYTE             *data;
      ZipFileDataBlock *next;
     };
   //---
   struct ZipCurFileInfo
     {
      SZlib::z_stream   stream;                 // zLib stream structure for inflate
      int               stream_initialised;     // 1 is stream is initialised
      UINT              pos_in_buffered_data;   // last written byte in buffered_data
      UINT              pos_local_header;       // offset of the local header of the file currenty writing
      char             *central_header;         // central header data for the current file
      UINT              size_centralheader;     // size of the central header for cur file
      UINT              flag;                   // flag of the file currently writing
      int               method;                 // compression method of file currenty wr.
      BYTE             *buffered_data;          // buffer contain compressed data to be write
      UINT              dosDate;
      UINT              crc32;
      UINT64            file_size;              //
      UINT              filename_len;           //
      bool              using_zip64;            //
     };
   //---
   struct ZipFileInfoInt
     {
      ZipFileDataBlock *first;
      ZipFileDataBlock *last;
      bool              in_opened_file_in_zip;  // 1 if a file in the metazip is currently write
      ZipCurFileInfo    ci;                     // info on the file curretly writing
      UINT              begin_pos;              // position of the beginning of the metazipfile
      UINT              number_entry;
     };

private:
   CMTStrPath        m_path;     // path
   CMTFile           m_file;     // file
   ZipFileInfoInt    m_info;     // information
   char*             m_buffer;   // buffer

public:
                     CZipFile(void);
                    ~CZipFile(void);
   //--- open/close
   bool              Open(LPCWSTR path);
   bool              Close(void);
   LPCWSTR           Path(void) const { return(m_path.Str()); }
   //--- adding
   bool              AddFile(LPCWSTR path,LPCWSTR zip_path);
   bool              AddDirectory(LPCWSTR zip_path);
   bool              AddDirectory(LPCWSTR zip_path,LPCWSTR full_path,LPCWSTR mask);
   UINT64            GetLastOriginalFileLength(void) { return(m_info.ci.stream.total_in);  }
   UINT64            GetLastZippedFileLength(void)   { return(m_info.ci.stream.total_out); }

private:
   //---
   void              PutValueInMemory(BYTE dest,UINT x,int size);
   bool              PutValueInMemory(char *buff,const UINT buff_size,UINT64 x,const UINT size);
   bool              PutValue(UINT x,int size);
   bool              PutValue64(UINT64 x,const int size);
   void              UpdateZipInfo(ZipFileInfo& info);
   //--- write zip header
   bool              WriteEndOfCentralDirectory(UINT64& pos_eocd_inzip,const UINT64 pos_centdir_inzip);
   bool              WriteEndOfCentralDirectoryLocator(const UINT64 pos_eocd_inzip);
   //---
   int               BlockWrite(void);
   void              BlockAddData(char *buffer,UINT size);
   void              BlockFreeAll(void);
   ZipFileDataBlock *BlockAlloc(void);
   void              BlockFree(ZipFileDataBlock *block);
   //---
   UINT              GetDosDate(ZipFileInfo& info);
   //---
   bool              FileInZipOpenNew(ZipFileInfo& info);
   bool              FileInZipWrite(const void *buffer,UINT size);
   bool              FileInZipClose(void);
  };
//+------------------------------------------------------------------+
  