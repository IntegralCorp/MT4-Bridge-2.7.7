//+------------------------------------------------------------------+
//|                                         MetaTrader 5 NFA.Reports |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "TradeRequestBase.h"
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CTradeRequestBase::CTradeRequestBase()
  {
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CTradeRequestBase::~CTradeRequestBase()
  {
  Shutdown();
  }
//+------------------------------------------------------------------+
//| Reopen database file and init in-memory cache                    |
//+------------------------------------------------------------------+
bool CTradeRequestBase::Initialize(void)
  {
  Shutdown();
  //--- check directory
  StringTypePath path;
  path.Format("%s\\%s",ExtProgramPath,"WatchTower");
  if(!CMTFile::DirectoryCreate(path))
    {
    Out(CmdErr,"create directory error [%s][%u]",path.Str(),GetLastError());
    return(false);
    }
  //--- make database path
  path.Append(TradeRequestBaseFile);
  if(!BaseInitialize(path.Str(),TradeRequestBaseHeaderName,TradeRequestBaseHeaderVersion))
    return(false);
  //--- build index
  return(IndexRebuild());
  }
//+------------------------------------------------------------------+
//| Index rebuild                                                    |
//+------------------------------------------------------------------+
bool CTradeRequestBase::IndexRebuild(void)
  {
  m_index_position.Clear();
  if(!m_index_position.Reserve(m_index_total))
    return(false);
  TradeRequest *rec=First();
  while(rec)
    {
    m_index_position.Add(&rec);
    rec=Next(rec);
    }
  m_index_position.Sort(SortByUniqueId);
  return(true);
  }
//+------------------------------------------------------------------+
//| Logger for RecordDatabase                                        |
//+------------------------------------------------------------------+
void CTradeRequestBase::Out(UINT code,LPCSTR msg,...)
  {
   if(!ExtServer)
      return;
//---
   char buffer[1024];
   va_list args;
   va_start(args,msg);
   _vsnprintf_s(buffer,sizeof(buffer)/sizeof(char),_TRUNCATE,msg,args);
   va_end(args);
//--- out
   ExtServer->LogsOut(code,"",buffer);
  }
//+------------------------------------------------------------------+
//| Release database file and clear in-memory cache                  |
//+------------------------------------------------------------------+
void CTradeRequestBase::Shutdown()
  {
  BaseShutdown();
  m_index_position.Shutdown();
  }
//+------------------------------------------------------------------+
//| Check positions                                                  |
//+------------------------------------------------------------------+
void CTradeRequestBase::Compact(void)
  {
  //---
  LOG_INFO(pluginLog,"compact started");
  //---
  UINT        deleted=0;
  TradeRequest *next=First();
  while(next)
    {
    TradeRequest *died=next;
    next=Next(next);
    //--- check time and position exist
    if(died->order_state==TradeRequest::ORDER_STATE_UNKNOWN)
      {
      Delete(died);
      deleted++;
      }
    }
  //--- compact base on disk and rebuild index
  if(deleted>0)
    {
    BaseCompact();
    IndexRebuild();
    }
  //---
  LOG_INFO(pluginLog,"compact finished");
  }
//+------------------------------------------------------------------+
//| Record get                                                       |
//+------------------------------------------------------------------+
bool CTradeRequestBase::RecordGet(LPCSTR unique_id,TradeRequest &record)
  {
  //--- find record
  TradeRequest **ptr=(TradeRequest **)m_index_position.Search(&unique_id,SearchByUniqueId);
  if(ptr&&*ptr)
    {
    record=*(*ptr);
    return(true);
    }
  //---
  return(false);
  }
//+------------------------------------------------------------------+
//| Record get                                                       |
//+------------------------------------------------------------------+
bool CTradeRequestBase::RecordGet(const UINT state,TradeRequest& record)
  {
  //--- find record
  TradeRequest **ptr=(TradeRequest **)m_index_position.Search(&state,SearchTradeRequestByState);
  if(ptr&&*ptr)
    {
    record=*(*ptr);
    return(true);
    }
  //---
  return(false);
  }
//+------------------------------------------------------------------+
//| Record update                                                    |
//+------------------------------------------------------------------+
bool CTradeRequestBase::RecordUpdate(TradeRequest &record,const bool allow_add/*=true*/)
  {
  //--- find record
  TradeRequest **ptr=(TradeRequest **)m_index_position.Search(&record.unique_id,SearchByUniqueId),*rec;
  if(ptr&&*ptr)
    {
    rec=*ptr;
    *rec=record;
    return(Update(rec));
    }
  //--- add new record
  if(allow_add)
    {
    TradeRequest *added=NULL;
    if(Add(&record,&added))
      {
      m_index_position.Insert(&added,SortByUniqueId);
      return(true);
      }
    }
  return(false);
  }
//+------------------------------------------------------------------+
//| Record delete                                                    |
//+------------------------------------------------------------------+
bool CTradeRequestBase::RecordDelete(LPCSTR unique_id)
  {
  //--- find record
  TradeRequest **ptr=(TradeRequest **)m_index_position.Search(&unique_id,SearchByUniqueId);
  if(ptr)
    {
    //--- delete record
    if(!Delete(*ptr))
      return(false);
    m_index_position.Delete(ptr);
    }
  return(true);
  }
//+------------------------------------------------------------------+
//| Sort function                                                    |
//+------------------------------------------------------------------+
int CTradeRequestBase::SortByUniqueId(const void *left,const void *right)
  {
  TradeRequest *lft=*(TradeRequest **)left;
  TradeRequest *rgh=*(TradeRequest **)right;
  //---
  return(StringType::Compare(lft->unique_id,rgh->unique_id));
  }
//+------------------------------------------------------------------+
//| Search function                                                  |
//+------------------------------------------------------------------+
int CTradeRequestBase::SearchByUniqueId(const void *left,const void *right)
  {
  LPCSTR        lft=*(LPCSTR *)left;
  TradeRequest *rgh=*(TradeRequest **)right;
  //---
  return(StringType::Compare(rgh->unique_id,lft));
  }
//+------------------------------------------------------------------+
//| Searching                                                        |
//+------------------------------------------------------------------+
int CTradeRequestBase::SearchTradeRequestByState(const void *left,const void *right)
  {
  UINT                lft=*(UINT *)left;
  TradeRequest *rgh=(TradeRequest *)right;
  //--- compare
  if(lft>rgh->order_state) return(1);
  if(lft<rgh->order_state) return(-1);
  //--- equal
  return(0);
  }
//+------------------------------------------------------------------+
