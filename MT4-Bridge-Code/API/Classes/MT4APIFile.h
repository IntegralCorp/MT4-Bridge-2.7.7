//+------------------------------------------------------------------+
//|                                                 MetaTrader 4 API |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include <handleapi.h>
#include <winbase.h>
#include <fileapi.h>
#include "MT4APIStr.h"
//+------------------------------------------------------------------+
//| File operations wrapper class                                    |
//+------------------------------------------------------------------+
class CMTFile
  {
public:
   static const UINT64 INVALID_POSITION;
private:
   HANDLE            m_file;

public:
                     CMTFile():m_file(INVALID_HANDLE_VALUE) {}
   virtual          ~CMTFile()                              { Close(); }
   //--- file open
   bool              Open(LPCSTR lpFileName,const DWORD dwAccess,const DWORD dwShare,const DWORD dwCreationFlags,const DWORD dwAttributes=FILE_ATTRIBUTE_NORMAL);
   bool              OpenRead(LPCSTR lpFileName);
   bool              OpenWrite(LPCSTR lpFileName);
   //--- file close
   void              Close(void);
   //--- file properties
   HANDLE            Handle(void);
   bool              IsOpen() const;
   UINT64            Size(void) const;
   static UINT64     Size(LPCSTR path);
   FILETIME          TimeCreate(void) const;
   FILETIME          TimeLastAccess(void) const;
   FILETIME          TimeLastModify(void) const;
   UINT64            CurrPos(void);
   //--- file operations
   DWORD             Read(void  *buffer,const DWORD length);
   DWORD             Write(const void *buffer,const DWORD length);
   UINT64            Seek(const INT64 distance,const DWORD method);
   bool              ChangeSize(const UINT64 size);
   bool              Flush();
   //--- files group operations
   static int        FilesCopy(const StringType& path,const StringType& newpath,const StringType& mask,const bool subdir);
   //--- directory operations
   static bool       DirectoryCreate(const StringType& path);
   static bool       DirectoryRemove(const StringType& path);
   static bool       DirectoryClean(const StringType& path,const StringType& mask);
  };
//+------------------------------------------------------------------+
//| Constant declaration                                             |
//+------------------------------------------------------------------+
const __declspec(selectany) UINT64 CMTFile::INVALID_POSITION=_UI64_MAX;
//+------------------------------------------------------------------+
//| File open                                                        |
//+------------------------------------------------------------------+
inline bool CMTFile::Open(LPCSTR lpFileName,const DWORD dwAccess,const DWORD dwShare,const DWORD dwCreationFlags,const DWORD dwAttributes)
  {
//--- close previous
   Close();
//--- check name and open
   if(lpFileName)
      m_file=CreateFileA(lpFileName,dwAccess,dwShare,NULL,dwCreationFlags,dwAttributes,NULL);
//--- return
   return(m_file!=INVALID_HANDLE_VALUE);
  }
//+------------------------------------------------------------------+
//| File open for read                                               |
//+------------------------------------------------------------------+
inline bool CMTFile::OpenRead(LPCSTR lpFileName)  
  { 
   return Open(lpFileName,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,OPEN_EXISTING);
  }
//+------------------------------------------------------------------+
//| File open for write                                              |
//+------------------------------------------------------------------+
inline bool CMTFile::OpenWrite(LPCSTR lpFileName)
 { 
  return Open(lpFileName,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,CREATE_ALWAYS); 
 }
//+------------------------------------------------------------------+
//| File close                                                       |
//+------------------------------------------------------------------+
inline void CMTFile::Close(void)
  {
   if(m_file!=INVALID_HANDLE_VALUE)
     {
      CloseHandle(m_file);
      m_file=INVALID_HANDLE_VALUE;
     }
  }
//+------------------------------------------------------------------+
//| File handle                                                      |
//+------------------------------------------------------------------+
inline HANDLE CMTFile::Handle(void)
  {
   return(m_file);
  }
//+------------------------------------------------------------------+
//| Check file state                                                 |
//+------------------------------------------------------------------+
inline bool CMTFile::IsOpen(void) const
  { 
   return(m_file!=INVALID_HANDLE_VALUE); 
  }
//+------------------------------------------------------------------+
//| File size                                                        |
//+------------------------------------------------------------------+
inline UINT64 CMTFile::Size() const
  {
   LARGE_INTEGER li={};
//--- check and receive
   if(m_file==INVALID_HANDLE_VALUE || !::GetFileSizeEx(m_file,&li)) 
      return(0);
//--- return result
   return((UINT64)li.QuadPart);
  }
//+------------------------------------------------------------------+
//| File size                                                        |
//+------------------------------------------------------------------+
inline UINT64 CMTFile::Size(LPCSTR path)
  {
   LARGE_INTEGER             li={};
   WIN32_FILE_ATTRIBUTE_DATA fad;
//--- receive file 
   if(GetFileAttributesExA(path,GetFileExInfoStandard,&fad))
     {
      li.LowPart =fad.nFileSizeLow;
      li.HighPart=(LONG)fad.nFileSizeHigh;
     }
//--- return result
   return((UINT64)li.QuadPart);
  }
//+------------------------------------------------------------------+
//| File creation time                                               |
//+------------------------------------------------------------------+
inline FILETIME CMTFile::TimeCreate() const
  {
   FILETIME ft={};
//--- check and receive
   if(m_file!=INVALID_HANDLE_VALUE) 
      GetFileTime(m_file,&ft,NULL,NULL);
//--- return result
   return(ft);
  }
//+------------------------------------------------------------------+
//| File last access time                                            |
//+------------------------------------------------------------------+
inline FILETIME CMTFile::TimeLastAccess() const
  {
   FILETIME ft={};
//--- check and receive
   if(m_file!=INVALID_HANDLE_VALUE) 
      GetFileTime(m_file,NULL,&ft,NULL);
//--- return result
   return(ft);
  }
//+------------------------------------------------------------------+
//| File last modify time                                            |
//+------------------------------------------------------------------+
inline FILETIME CMTFile::TimeLastModify() const
  {
   FILETIME ft={};
//--- check and receive
   if(m_file!=INVALID_HANDLE_VALUE) 
      GetFileTime(m_file,NULL,NULL,&ft);
//--- return result
   return(ft);
  }
//+------------------------------------------------------------------+
//| File pointer position                                            |
//+------------------------------------------------------------------+
inline UINT64 CMTFile::CurrPos()
  {
   return(CMTFile::Seek(INT64(0),FILE_CURRENT));
  }
//+------------------------------------------------------------------+
//| File read                                                        |
//+------------------------------------------------------------------+
inline DWORD CMTFile::Read(void *buffer,const DWORD length)
  {
//--- check
   if(m_file==INVALID_HANDLE_VALUE || buffer==NULL || length<1)
      return(0);
//--- read
   DWORD readed=0;

   if(ReadFile(m_file,buffer,length,&readed,NULL)==0) 
      readed=0;
//--- return
   return(readed);
  }
//+------------------------------------------------------------------+
//| Write to file                                                    |
//+------------------------------------------------------------------+
inline DWORD CMTFile::Write(const void *buffer,const DWORD length)
  {
//--- check
   if(m_file==INVALID_HANDLE_VALUE || buffer==NULL || length<1) 
      return(0);
//--- write
   DWORD written=0;
   
   if(WriteFile(m_file,buffer,length,&written,NULL)==0) 
      written=0;
//--- return
   return(written);
  }
//+------------------------------------------------------------------+
//| Seek file pointer                                                |
//+------------------------------------------------------------------+
inline UINT64 CMTFile::Seek(const INT64 distance,const DWORD method)
  {
//--- check
   if(m_file==INVALID_HANDLE_VALUE) 
      return(INVALID_POSITION);
//--- fill and seek
   LARGE_INTEGER li={};

   li.QuadPart=distance;
   li.LowPart=SetFilePointer(m_file,(LONG)li.LowPart,&li.HighPart,method);
//--- check result
   if(li.LowPart==INVALID_SET_FILE_POINTER && GetLastError()!=NO_ERROR)
      return(INVALID_POSITION);
//--- return new file pointer position
   return((UINT64)li.QuadPart);
  }
//+------------------------------------------------------------------+
//| Change file size                                                 |
//+------------------------------------------------------------------+
inline bool CMTFile::ChangeSize(const UINT64 size)
  {
   return(CMTFile::Seek((INT64)size,FILE_BEGIN)==size && SetEndOfFile(m_file));
  }
//+------------------------------------------------------------------+
//| Flush file buffer                                                |
//+------------------------------------------------------------------+
inline bool CMTFile::Flush()
  {
   if(m_file!=INVALID_HANDLE_VALUE) 
      return(::FlushFileBuffers(m_file)!=0);
   return(false);
  }
//+------------------------------------------------------------------+
//| Copy files by mask                                               |
//+------------------------------------------------------------------+
inline int CMTFile::FilesCopy(const StringType& path,const StringType& newpath,const StringType& mask,const bool subdir)
  {
   StringType512    src,dst,name;
   HANDLE           hSearch;
   WIN32_FIND_DATA  fnd;
   int              count=0;
//--- find files
   src.Format("%s\\%s",path.Str(),mask.Str());
   if((hSearch=FindFirstFileA(src.Str(), &fnd))!=INVALID_HANDLE_VALUE)
     {
      do
        {
         //--- skip root directories
         if(StringType::Compare(fnd.cFileName,".")==0 || StringType::Compare(fnd.cFileName,"..")==0)
            continue;
         //--- directory?
         if(!(fnd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
           {
            //--- create source path
            src.Format("%s\\%s",path.Str(),fnd.cFileName);
            //--- create dst path
            dst.Format("%s\\%s",newpath.Str(),fnd.cFileName);
            //--- copy file
            if(::CopyFileA(src.Str(),dst.Str(),FALSE)!=FALSE) count++;
           }
         else
            if(subdir)
              {
               //--- create path
               src.Format("%s\\%s",path.Str(),fnd.cFileName);
               dst.Format("%s\\%s",newpath.Str(),fnd.cFileName);
               //--- create dst path
               DirectoryCreate(dst);
               //--- copy files
               count+=FilesCopy(src,dst,mask,subdir);
              }
        } while(FindNextFileA(hSearch,&fnd)!=0);
      //--- close handle
      FindClose(hSearch);
     }
//--- return copied count
   return(count);
  }
//+------------------------------------------------------------------+
//| Create directory with subdirectories                             |
//+------------------------------------------------------------------+
inline bool CMTFile::DirectoryCreate(const StringType& path)
  {
   char *cp,temp[MAX_PATH];
//--- check directory
   if(GetFileAttributesA(path.Str())!=INVALID_FILE_ATTRIBUTES) 
      return(true);
//--- copy
   StringType::Copy(temp,path.Str());
//--- parse path and create directories
   for(cp=temp;*cp!='\0';cp++)
      if(*cp=='\\')
        {
         *cp='\0';
         if(GetFileAttributesA(temp)==INVALID_FILE_ATTRIBUTES)
            if(!::CreateDirectoryA(temp,NULL)) 
               return(false);
         *cp='\\';
        }
//--- copy remainder
   if(GetFileAttributesA(temp)==INVALID_FILE_ATTRIBUTES)
      if(!::CreateDirectoryA(temp,NULL))
         return(false);
//--- ok
   return(true);
  }
//+------------------------------------------------------------------+
//| Directory clean by mask                                          |
//+------------------------------------------------------------------+
inline bool CMTFile::DirectoryClean(const StringType& path,const StringType& mask)
  {
   HANDLE           hSearch;
   WIN32_FIND_DATAA fnd;
   StringTypePath   tmp;
   bool             res=true;
//--- check
   if(path.Empty() || path.Len()<=3) 
      return(false);
//--- create path
   tmp.Format("%s\\%s",path.Str(),mask.Str());
//--- search
   if((hSearch=FindFirstFileA(tmp.Str(),&fnd))!=INVALID_HANDLE_VALUE)
     {
      do
        {
         //--- directory?
         if(fnd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
           {
            //--- skip root directories
            if(StringType::Compare(fnd.cFileName,".")==0 || StringType::Compare(fnd.cFileName,"..")==0)
               continue;
            //--- create directory path
            tmp.Format("%s\\%s",path.Str(),fnd.cFileName);
            //--- clean directory
            DirectoryClean(tmp,mask);
            //--- delete directory
            res=::RemoveDirectoryA(tmp.Str()) && res;
           }
         else
           {
            //--- create file path
            tmp.Format("%s\\%s",path.Str(),fnd.cFileName);
            //--- delete file
            res=::DeleteFileA(tmp.Str()) && res;
           }
        }
      while(FindNextFileA(hSearch,&fnd));
      //--- close handle
      FindClose(hSearch);
     }
//--- result
   return(res);
  }
//+------------------------------------------------------------------+
//| Full directory remove                                            |
//+------------------------------------------------------------------+
inline bool CMTFile::DirectoryRemove(const StringType& path)
  {
   return(DirectoryClean(path,StringType16("*")) && ::RemoveDirectoryA(path.Str()));
  }
//+------------------------------------------------------------------+
