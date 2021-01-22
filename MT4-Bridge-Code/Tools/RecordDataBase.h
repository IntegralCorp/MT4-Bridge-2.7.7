//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include <minwindef.h>

#include "..\API\MT4APITools.h"
#include "..\Common\Const.h"
//+------------------------------------------------------------------+
//| Header for record database                                       |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct RecordDataBaseHeader
  {
   UINT              version;         // version number
   char              copyright[64];   // copyright string
   char              name[16];        // database type description
   INT64             timestamp;       // 
   UINT64            lastid;          // last record id
   UINT              total;           // records count    
   char              reserved[84];    // keep extra space 
   //--- constructor/destructor
                     RecordDataBaseHeader()                                    { Clear();                      }
                     RecordDataBaseHeader(const UINT tversion,LPCSTR tname)    { Initialize(tversion,tname);   }
                    ~RecordDataBaseHeader()                                    {                               }
   //---
   void              Initialize(const UINT tversion,LPCSTR tname)
     {
      Clear();
      version=tversion;
      StringType::Copy(copyright,Copyright);
      StringType::Copy(name,tname);
     }
   void              Clear()
     {
      ZeroMemory(this,sizeof(*this));
     }
   //--- read self from file   
   template<class T> bool Read(T& file)
     {
      if(file.Read(this,sizeof(*this))==sizeof(*this))
        {
         StringType::Terminate(copyright);
         StringType::Terminate(name);
         return(true);
        }
      return(false);
     }
   //--- write self to file
   template<class T> bool Write(T& file) const
     {
      return(file.Write(this,sizeof(*this))==sizeof(*this));
     }
   //--- checking version and name
   bool              Check(const UINT tversion,LPCSTR tname)
     {
      //--- ensure strings terminated
      StringType::Terminate(copyright);
      StringType::Terminate(name);
      //--- check
      return(version==tversion && tname && StringType::Compare(tname,name)==0);
     }
   //--- check database type
   bool              Check(LPCSTR tname)
     {
      //--- ensure strings terminated
      StringType::Terminate(copyright);
      StringType::Terminate(name);
      //--- check                    
      return(StringType::Compare(tname,name)==0);
     }
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Template class for storing POD structures on the drive           |
//| Requirements for T type:                                         |
//| 1. fields 'id' (UINT64) and 'timestamp' (INT64)                  |
//| 2. methods Read/Write with CMTFile as argument                   |
//| 3. method for version-dependent Read from CMTFile                |
//+------------------------------------------------------------------+
template <class T>
class TRecordDataBase
  {
private:
   enum constants
     {
      DEFAULT_FILE_GROW_STEP=256*KB,        // step for file allocation
     };

private:
   //--- Record descriptor. In the memory places this descriptor struct first, than T struct
   //--- Uses for backward validation T*
   struct DBRecordNode
     {
      UINT64            offset;             // on the disk
      DBRecordNode     *next;               // element
      DBRecordNode     *prev;               // element
      T                *record;             // record pointer
     };

protected:
   StringTypePath    m_data_path;           // database file path
   StringTypePath    m_data_name;           // database name
   UINT              m_data_version;        // 
   RecordDataBaseHeader m_data_header;      // 
   CMTFile           m_data_file;           // 
   UINT64            m_data_file_next;      // file position to the next record
   UINT              m_data_file_grow;      // file grownup step
   UINT64            m_data_file_max;       // current file size
   //--- indexes                            
   DBRecordNode     *m_index;
   DBRecordNode     *m_index_tail;
   DBRecordNode     *m_index_deleted;
   UINT              m_index_total;

public:
                     TRecordDataBase(void);
                    ~TRecordDataBase(void);
   //--- database processing
   bool              BaseInitialize(LPCSTR path,LPCSTR name,const UINT version,const UINT file_grow=0);
   void              BaseShutdown(void);
   bool              BaseClear(void);
   bool              BaseCompact(void);
   //--- records processing
   bool              Add(const T* record,T** added=NULL);
   T*                Add(void);
   bool              Update(T* record);
   bool              Delete(T* record);
   //--- records access
   UINT              Total(void) const;
   T*                First(void);
   const T*          First(void) const;
   T*                Tail(void);
   const T*          Tail(void) const;
   T*                Next(T *record);
   const T*          Next(const T *record) const;
   T*                Prev(T *record);
   const T*          Prev(const T *record) const;

protected:
   //--- return true if database must be removed
   virtual bool      BaseCheckDelete(RecordDataBaseHeader& header) { return(false); }
   //--- override for putting messages into log
   virtual void      Out(const UINT code,LPCSTR msg,...)=0;

private:
   //--- database processing
   bool              BaseCreate(void);
   bool              BaseLoad(const bool allowconvert=true);
   //--- index processing
   void              IndexShutdown(void);
   void              IndexAttach(DBRecordNode* node);
   void              IndexDetach(DBRecordNode* node);
   //--- records processing
   T*                RecordAllocate(DBRecordNode*& index);
   void              RecordFree(T* record);
   DBRecordNode*     RecordNode(T* record);
   const DBRecordNode* RecordNode(const T* record);
   INT64             RecordTimestamp(void);
  };
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
template <class T>
inline TRecordDataBase<T>::TRecordDataBase(void) : m_data_version(0),m_data_file_next(0),m_data_file_grow(0),m_data_file_max(0),m_index(0),m_index_tail(0),m_index_deleted(0),m_index_total(0)
  {
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
template <class T>
inline TRecordDataBase<T>::~TRecordDataBase(void)
  {
   BaseShutdown();
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
template <class T>
inline bool TRecordDataBase<T>::BaseInitialize(LPCSTR path,LPCSTR name,const UINT version,const UINT file_grow/*=0*/)
  {
   bool res=false;
//--- first - de-initialize
   BaseShutdown();
//--- checks
   if(path && path[0] && name && name[0] && version)
     {
      //--- store arguments
      m_data_path.Assign(path);
      m_data_name.Assign(name);
      m_data_version=version;
      m_data_file_grow=file_grow ? file_grow : DEFAULT_FILE_GROW_STEP;
      //--- check file exists
      if(GetFileAttributes(m_data_path.Str())==INVALID_FILE_ATTRIBUTES)
         res=BaseCreate();
      else
         res=BaseLoad();
     }
//--- 
   return(res);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
template <class T>
inline void TRecordDataBase<T>::BaseShutdown(void)
  {
//--- clear index
   IndexShutdown();
//--- clear fields
   m_data_path.Clear();
   m_data_name.Clear();
   m_data_version  =0;
   m_data_header.Clear();
   m_data_file_next=0;
   m_data_file_grow=0;
   m_data_file_max =0;
   m_data_file.Close();
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
template <class T>
inline bool TRecordDataBase<T>::BaseClear(void)
  {
//--- just create new database
   if(BaseCreate())
     {
      IndexShutdown();
      return(true);
     }
//--- fail
   return(false);
  }
//+------------------------------------------------------------------+
//| compact database for less disk usage                             |
//+------------------------------------------------------------------+
template <class T>
inline bool TRecordDataBase<T>::BaseCompact(void)
  {
//--- check file
   if(!m_data_file.IsOpen())
      return(false);
//--- process
   StringTypePath  new_path;
   CMTFile         new_file;
   RecordDataBaseHeader new_header=m_data_header;
   UINT64          new_size=sizeof(m_data_header)+m_index_total*sizeof(T);
//--- renew header
   new_header.total  =m_index_total;
   new_header.version=m_data_version;
//--- new file name
   new_path.Format("%s.new",m_data_path.Str());
//--- create new file
   if(!new_file.Open(new_path.Str(),GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,CREATE_ALWAYS) ||
      !new_header.Write(new_file) ||
      !new_file.ChangeSize(sizeof(m_data_header)+new_size+m_data_file_grow) ||
      new_file.Seek(sizeof(m_data_header),FILE_BEGIN)==CMTFile::INVALID_POSITION)
     {
      Out(CmdErr,"compact create database error [%u]",GetLastError());
      new_file.Close();
      DeleteFile(new_path.Str());
      return(false);
     }
//--- write into new file 
   for(DBRecordNode *node=m_index;node;node=node->next)
      if(!node->record->Write(new_file))
        {
         Out(CmdErr,"database compact write error [%u]",GetLastError());
         new_file.Close();
         DeleteFile(new_path.Str());
         return(false);
        }
//--- close new
   new_file.Close();
//--- close old
   m_data_file.Close();
//--- replace files
   if(MoveFileEx(new_path.Str(),m_data_path.Str(),MOVEFILE_WRITE_THROUGH|MOVEFILE_REPLACE_EXISTING)==FALSE)
     {
      Out(CmdErr,"database replace error [%u]",GetLastError());
      DeleteFile(new_path.Str());
     }
//--- reload database
   return(BaseLoad(false));
  }
//+------------------------------------------------------------------+
//| Adding record into database                                      |
//+------------------------------------------------------------------+
template <class T>
inline bool TRecordDataBase<T>::Add(const T* record,T** added/*=NULL*/)
  {
//--- checks
   if(!record || !m_data_file.IsOpen())
      return(false);
//--- create index
   DBRecordNode *node=NULL;
//--- check for removed
   if(m_index_deleted)
     {
      //--- take place of removed record
      node=m_index_deleted;
      m_index_deleted=m_index_deleted->next;
      node->next=node->prev=NULL;
     }
   else
     {
      //--- allocate place for new record
      if(RecordAllocate(node)==NULL)
        {
         Out(CmdErr,"not enough memory for add record");
         return(false);
        }
     }
//--- copy record data
   *node->record=*record;
//--- check position in file
   if(!node->offset)
      node->offset=m_data_file_next;
//--- take record id
   node->record->id=m_data_header.lastid+1;
//--- setup timestamp
   node->record->timestamp=RecordTimestamp();
//--- write to the drive
   if(node->offset>=m_data_file_max)
      if(m_data_file.ChangeSize(node->offset+m_data_file_grow))
         m_data_file_max=node->offset+m_data_file_grow;
//--- seek to the position and write
   if(m_data_file.Seek(node->offset,FILE_BEGIN)==CMTFile::INVALID_POSITION || !node->record->Write(m_data_file))
     {
      Out(CmdErr,"seek/write error [%u]",GetLastError());
      RecordFree(node->record);
      return(false);
     }
//--- append index
   IndexAttach(node);
//--- update records count
   m_index_total++;
//--- renew header
   m_data_header.lastid   =max(m_data_header.lastid   ,node->record->id);
   m_data_header.timestamp=max(m_data_header.timestamp,node->record->timestamp);
   m_data_file_next       =max(m_data_file_next       ,node->offset+sizeof(T));
//--- return result
   if(added)
      *added=node->record;
//--- 
   return(true);
  }
//+------------------------------------------------------------------+
//| Add new blank record into database                               |
//+------------------------------------------------------------------+
template <class T>
inline T* TRecordDataBase<T>::Add(void)
  {
   T record={},*ptr=NULL;
//--- add and return
   return(Add(&record,&ptr) ? ptr : NULL);
  }
//+------------------------------------------------------------------+
//| Update record                                                    |
//+------------------------------------------------------------------+
template <class T>
inline bool TRecordDataBase<T>::Update(T* record)
  {
//--- checks
   if(record && m_data_file.IsOpen())
     {
      //--- get and verify record descriptor
      DBRecordNode *node=RecordNode(record);
      if(node && node->record==record)
        {
         //--- save current timestamp
         UINT64 prevstamp=node->record->timestamp;
         //--- renew timestamp
         node->record->timestamp=RecordTimestamp();
         //--- seek to the position and write file
         if(m_data_file.Seek(node->offset,FILE_BEGIN)!=CMTFile::INVALID_POSITION && node->record->Write(m_data_file))
           {
            //--- renew timestamp in header
            m_data_header.timestamp=max(m_data_header.timestamp,node->record->timestamp);
            return(true);
           }
         //--- log error
         Out(CmdErr,"seek/write error [%u]",GetLastError());
         //--- repair old timestamp
         node->record->timestamp=prevstamp;
        }
      else
         Out(CmdErr,"invalid pointer for update");
     }
//--- fail
   return(false);
  }
//+------------------------------------------------------------------+
//| Удаление записи                                                  |
//+------------------------------------------------------------------+
template <class T>
inline bool TRecordDataBase<T>::Delete(T* record)
  {
//--- checks (if timestamp is zero than record already deleted)
   if(record && record->timestamp && m_data_file.IsOpen())
     {
      //--- get descriptor
      DBRecordNode *node=RecordNode(record);
      if(node && node->record==record)
        {
         //--- store current timestamp
         UINT64 prevstamp=node->record->timestamp;
         //--- reset timestamp - mark record as 'deleted'
         node->record->timestamp=0;
         //--- seek and write file
         if(m_data_file.Seek(node->offset,FILE_BEGIN)!=CMTFile::INVALID_POSITION && node->record->Write(m_data_file))
           {
            //--- remove from index for future using
            IndexDetach(node);
            //--- add into removed list
            node->next=m_index_deleted;
            node->prev=NULL;
            m_index_deleted=node;
            //--- decrement records count
            if(m_index_total)
               m_index_total--;
            //--- success
            return(true);
           }
         //--- log error
         Out(CmdErr,"seek/write error [%u]",GetLastError());
         //--- repair old timestamp
         node->record->timestamp=prevstamp;
        }
      else
         Out(CmdErr,"invalid pointer for delete");
     }
//--- fail
   return(false);
  }
//+------------------------------------------------------------------+
//| Return records count                                             |
//+------------------------------------------------------------------+
template <class T>
inline UINT TRecordDataBase<T>::Total(void) const
  {
   return(m_index_total);
  }
//+------------------------------------------------------------------+
//| Return pointer to the first record                               |
//+------------------------------------------------------------------+
template <class T>
inline T* TRecordDataBase<T>::First(void)
  {
   return(m_index ? m_index->record : NULL);
  }
//+------------------------------------------------------------------+
//| Return pointer to the first record                               |
//+------------------------------------------------------------------+
template <class T>
inline const T* TRecordDataBase<T>::First(void) const
  {
   return(m_index ? m_index->record : NULL);
  }
//+------------------------------------------------------------------+
//| Return pointer to the tail record                                |
//+------------------------------------------------------------------+
template <class T>
inline T* TRecordDataBase<T>::Tail(void)
  {
   return(m_index_tail ? m_index_tail->record : NULL);
  }
//+------------------------------------------------------------------+
//| Return pointer to the tail record                                |
//+------------------------------------------------------------------+
template <class T>
inline const T* TRecordDataBase<T>::Tail(void) const
  {
   return(m_index_tail ? m_index_tail->record : NULL);
  }
//+------------------------------------------------------------------+
//| Return pointer to the next record                                |
//+------------------------------------------------------------------+
template <class T>
inline T* TRecordDataBase<T>::Next(T *record)
  {
   DBRecordNode *node=RecordNode(record);
   if(node && node->next)
      return(node->next->record);
//--- not found 
   return(NULL);
  }
//+------------------------------------------------------------------+
//| Return pointer to the next record                                |
//+------------------------------------------------------------------+
template <class T>
inline const T* TRecordDataBase<T>::Next(const T *record) const
  {
   const DBRecordNode *node=RecordNode(record);
   if(node && node->next)
      return(node->next->record);
//--- not found 
   return(NULL);
  }
//+------------------------------------------------------------------+
//| Return pointer to the previous record                            |
//+------------------------------------------------------------------+
template <class T>
inline T* TRecordDataBase<T>::Prev(T *record)
  {
   DBRecordNode *node=RecordNode(record);
   if(node && node->prev)
      return(node->prev->record);
//--- not found 
   return(NULL);
  }
//+------------------------------------------------------------------+
//| Return pointer to the previous record                            |
//+------------------------------------------------------------------+
template <class T>
inline const T* TRecordDataBase<T>::Prev(const T *record) const
  {
   const DBRecordNode *node=RecordNode(record);
   if(node && node->prev)
      return(node->prev->record);
//--- not found 
   return(NULL);
  }
//+------------------------------------------------------------------+
//| Database creation                                                |
//+------------------------------------------------------------------+
template <class T>
inline bool TRecordDataBase<T>::BaseCreate(void)
  {
//--- clear header    
   m_data_header.Clear();
   m_data_header.Initialize(m_data_version,m_data_name.Str());
//--- create file         
   if(!m_data_file.Open(m_data_path.Str(),GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,CREATE_ALWAYS) || !m_data_header.Write(m_data_file))
     {
      Out(CmdErr,"file creation error [%s][%u]",m_data_path.Str(),GetLastError());
      m_data_file.Close();
      DeleteFile(m_data_path.Str());
      return(false);
     }
//--- point to the next record
   m_data_file_next=sizeof(m_data_header);
//--- renew file size              
   m_data_file_max=m_data_file.Size();
//--- 
   return(true);
  }
//+------------------------------------------------------------------+
//| Load database from file                                          |
//+------------------------------------------------------------------+
template <class T>
inline bool TRecordDataBase<T>::BaseLoad(const bool allowconvert/*=true*/)
  {
   T             *record=NULL;
   DBRecordNode *node  =NULL;
   bool           invalid_version=false;
//--- clear index  
   IndexShutdown();
//--- open file
   if(!m_data_file.Open(m_data_path.Str(),GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,OPEN_EXISTING))
     {
      Out(CmdErr,"base open error [%s][%u]",m_data_path.Str(),GetLastError());
      return(false);
     }
//--- read and check header
   if(!m_data_header.Read(m_data_file) || !m_data_header.Check(m_data_name.Str()))
     {
      Out(CmdErr,"file header read/check error [%s][%u]",m_data_path.Str(),GetLastError());
      m_data_file.Close();
      return(false);
     }
//--- check version
   if(m_data_header.version!=m_data_version)
     {
      //--- may be remove?
      if(BaseCheckDelete(m_data_header))
         return(BaseCreate());
      //--- can we convert database?
      if(!allowconvert)
         return(false);
     }
//--- stay to the begin of records part
   m_data_file_next=sizeof(m_data_header);
//--- read records
   while(true)
     {
      //--- Allocate memory for new record
      if((record=RecordAllocate(node))==NULL)
        {
         Out(CmdErr,"not enough memory for load record");
         m_data_file.Close();
         return(false);
        }
      //--- create index
      node->offset=m_data_file.CurrPos();
      //--- check version?
      if(m_data_header.version==m_data_version)
        {
         //--- read record from file
         if(!record->Read(m_data_file))
            break;
        }
      else
        {
         //--- read record version-dependent
         if(!record->Read(m_data_file,m_data_header.version,invalid_version))
            break;
        }
      //--- is record empty?
      if(record->id || record->timestamp)
        {
         //--- renew header
         m_data_header.lastid   =max(m_data_header.lastid   ,record->id);
         m_data_header.timestamp=max(m_data_header.timestamp,record->timestamp);
         //--- end of file
         m_data_file_next=max(m_data_file_next,node->offset+sizeof(T));
         //--- is record removed?
         if(record->timestamp)
           {
            //--- append to the index
            IndexAttach(node);
            //--- update total counter
            m_index_total++;
           }
         else
           {
            //--- append to the list of removed records
            node->next=m_index_deleted;
            node->prev=NULL;
            m_index_deleted=node;
           }
        }
      else
         break;
     }
//--- release memory
   if(record)
      RecordFree(record);
//--- another version?
   if(m_data_header.version!=m_data_version)
     {
      //--- bad version
      if(invalid_version)
        {
         Out(CmdErr,"base version error [%u]",m_data_header.version);
         m_data_file.Close();
         return(false);
        }
      //--- renew timestamps
      for(node=m_index;node;node=node->next)
        {
         //--- get current timestamp
         node->record->timestamp=RecordTimestamp();
         //--- update header timestamp
         m_data_header.timestamp=node->record->timestamp;
        }
      //--- compact 
      return(BaseCompact());
     }
//--- renew file size
   m_data_file_max=m_data_file.Size();
//--- 
   return(true);
  }
//+------------------------------------------------------------------+
//| Clear memory indexes                                             |
//+------------------------------------------------------------------+
template <class T>
inline void TRecordDataBase<T>::IndexShutdown(void)
  {
//--- cycle on the main index list
   while(m_index)
     {
      DBRecordNode *died=m_index;
      m_index=m_index->next;
      RecordFree(died->record);
     }
//--- cycle on the removed index list
   while(m_index_deleted)
     {
      DBRecordNode *died=m_index_deleted;
      m_index_deleted=m_index_deleted->next;
      RecordFree(died->record);
     }
//--- reset tail and count of indexes
   m_index_tail=NULL;
   m_index_total=0;
  }
//+------------------------------------------------------------------+
//| Append to the main index                                         |
//+------------------------------------------------------------------+
template <class T>
inline void TRecordDataBase<T>::IndexAttach(DBRecordNode* node)
  {
//--- checks
   if(node)
     {
      //--- have a tail?
      if(m_index_tail)
        {
         m_index_tail->next=node;
         node->prev=m_index_tail;
         node->next=NULL;
         m_index_tail=node;
        }
      else
        {
         //--- at start
         m_index=m_index_tail=node;
         node->prev=node->next=NULL;
        }
     }
  }
//+------------------------------------------------------------------+
//| Remove from main index                                           |
//+------------------------------------------------------------------+
template <class T>
inline void TRecordDataBase<T>::IndexDetach(DBRecordNode* node)
  {
//--- checks
   if(node)
     {
      if(node->next)
         node->next->prev=node->prev;
      else
         m_index_tail=node->prev;
      if(node->prev)
         node->prev->next=node->next;
      else
        {
         m_index=m_index->next;
         if(m_index)
            m_index->prev=NULL;
        }
     }
  }
//+------------------------------------------------------------------+
//| Allocate memory piece for record descriptor and record           |
//+------------------------------------------------------------------+
template <class T>
inline T* TRecordDataBase<T>::RecordAllocate(DBRecordNode*& node)
  {
   node=NULL;
//--- allocate
   char *data=::new char[sizeof(DBRecordNode)+sizeof(T)];
//--- check allocated
   if(data)
     {
      //--- call constructor
      ::new (data+sizeof(DBRecordNode)) T();
      //--- return descriptor address for index
      node=(DBRecordNode*)data;
      ZeroMemory(node,sizeof(DBRecordNode));
      node->record=(T*)(data+sizeof(DBRecordNode));
      //--- return record address
      return(node->record);
     }
   return(NULL);
  }
//+------------------------------------------------------------------+
//| Free memory of descriptor + record                               |
//+------------------------------------------------------------------+
template <class T>
inline void TRecordDataBase<T>::RecordFree(T* record)
  {
//--- check
   if(record)
     {
      record->~T();
      ::delete [] ((char*)record-sizeof(DBRecordNode));
     }
  }
//+------------------------------------------------------------------+
//| Find descriptor for record                                       |
//+------------------------------------------------------------------+
template <class T>
inline typename TRecordDataBase<T>::DBRecordNode* TRecordDataBase<T>::RecordNode(T* record)
  {
//--- check
   if(record)
     {
      //--- calculate address of leading record
      char *node_ptr=((char*)record)-sizeof(DBRecordNode);
      //--- 
      return((DBRecordNode*)node_ptr);
     }
//--- no record - no descriptor
   return(NULL);
  }
//+------------------------------------------------------------------+
//| Find descriptor for record                                       |
//+------------------------------------------------------------------+
template <class T>
inline const typename TRecordDataBase<T>::DBRecordNode* TRecordDataBase<T>::RecordNode(const T* record)
  {
//--- check
   if(record)
     {
      //--- calculate address of leading record
      const char *node_ptr=((char*)record)-sizeof(DBRecordNode);
      //--- 
      return((const DBRecordNode*)node_ptr);
     }
//--- 
   return(NULL);
  }
//+------------------------------------------------------------------+
//| Get next timestamp for record                                    |
//+------------------------------------------------------------------+
template <class T>
inline INT64 TRecordDataBase<T>::RecordTimestamp(void)
  {
   INT64 timestamp=0;
   GetSystemTimeAsFileTime((FILETIME*)&timestamp);
   return(max(timestamp,m_data_header.timestamp+1));
  }
//+------------------------------------------------------------------+
