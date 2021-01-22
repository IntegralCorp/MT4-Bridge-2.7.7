//+------------------------------------------------------------------+
//|                                         MetaTrader 5 NFA.Reports |
//|                   Copyright 2000-2019, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "Zlib.h"
//+------------------------------------------------------------------+
//| ZIP file extraction                                              |
//+------------------------------------------------------------------+
class CUnzipFile
  {
public:
   enum constants
     {
      BUFREADCOMMENT    =0x400,
      SIZECENTRALDIRITEM=0x2e,
      SIZEZIPLOCALHEADER=0x1e
     };
   //---
   struct UnzipInfo
     {
      UINT              entries;             // total number of entries in the central dir on this disk
      UINT              size_comment;        // size of the global comment of the zipfile
     };
   //---
   struct UnzipFileInfo
     {
      UINT              version;             // version made by                 2 bytes
      UINT              version_needed;      // version needed to extract       2 bytes
      UINT              flag;                // general purpose bit flag        2 bytes
      UINT              method;              // compression method              2 bytes
      UINT              dosDate;             // last mod file date in Dos fmt   4 bytes
      UINT              crc;                 // crc-32                          4 bytes
      UINT64            comp_size;           // compressed size, zip64 - 8b     4 bytes
      UINT64            uncomp_size;         // uncompressed size, zip64 - 8b   4 bytes
      UINT              size_filename;       // filename length                 2 bytes
      UINT              size_extra;          // extra field length              2 bytes
      UINT              size_comment;        // file comment length             2 bytes
      UINT              disk_start;          // disk number start               2 bytes
      UINT              int_fa;              // internal file attributes        2 bytes
      UINT              ext_fa;              // external file attributes        4 bytes
      SYSTEMTIME        tmu_date;
     };
private:
   //---
   struct FileInZip
     {
      char             *buffer;              // internal buffer for compressed data
      SZlib::z_stream   stream;              // stream structure for inflate
      UINT64            position;            // position in byte on the zipfile, for fseek
      UINT              initialised;         // flag set if stream structure is initialised
      UINT              loc_extra_offset;    // offset of the local extra field
      UINT              loc_extra_size;      // size of the local extra field
      UINT              loc_extra_pos;       // position in the local extra field in read
      UINT              crc32;               // crc32 of all data uncompressed
      UINT              crc32_wait;          // crc32 we must obtain after decompress all
      UINT64            read_comp;           // number of byte to be decompressed
      UINT64            read_uncomp;         // number of byte to be obtained after decomp
      UINT              method;              // compression method (0==store)
      UINT64            byte_before;         // byte before the zipfile, (>0 for sfx)
     };
   //---
   struct UnzipFileInfoInt
     {
      UnzipInfo         gi;                  // public global information
      UINT64            byte_before;         // byte before the zipfile, (>0 for sfx)
      UINT              num_file;            // number of the current file in the zipfile
      UINT64            pos_in_cdir;         // pos of the current file in the central dir
      bool              file_ok;             // flag about the usability of the current file
      UINT64            central_pos;         // position of the beginning of the central dir
      UINT              size_cdir;           // size of the central directory
      UINT64            offset_cdir;         // offset of start of central directory with respect to the starting disk number
      UnzipFileInfo     info;                // public info about the current file in zip
      UINT              offset_curfile;      // relative offset of local header 4 bytes
      FileInZip*        pzfile;              // structure about the current file if we are decompressing it
     };
private:
   CMTFile           m_file;
   UnzipFileInfoInt  m_info;

public:
                     CUnzipFile(void);
                    ~CUnzipFile(void);
   //--- file open\close
   bool              Open(LPCWSTR path);
   void              Close(void);
   bool              IsOpen(void) const { return(m_info.central_pos!=0); }
   //--- information
   bool              Info(UnzipInfo& info);
   //--- file navigation
   bool              FileFirst(void);
   bool              FileNext(void);
   bool              FileOpen(void);
   bool              FileClose(void);
   UINT              FileRead(void *buffer,UINT size);
   bool              FileInfo(UnzipFileInfo& info,LPWSTR name,UINT name_size);

private:
   //---
   UINT64            FindByMagic(const UINT magic_number);
   bool              CurFileInfoInt(UnzipFileInfo& info,UINT& offset,LPWSTR name,UINT name_size);
   bool              CheckHeader(UINT& size_var,UINT& extra_offset,UINT& extra_size);
   //---
   bool              GetByte(UINT& value);
   bool              GetShort(UINT& value);
   bool              GetLong(UINT& value);
   bool              GetUInt64(UINT64& value);
   //---
   bool              FileInZipAlloc(void);
   void              FileInZipFree(void);
  };
//+------------------------------------------------------------------+
