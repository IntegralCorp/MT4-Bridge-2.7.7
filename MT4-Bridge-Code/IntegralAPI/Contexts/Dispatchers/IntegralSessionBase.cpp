//+------------------------------------------------------------------+
//|                                         MetaTrader 5 NFA.Reports |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "StdAfx.h"
#include "IntegralSessionBase.h"
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CIntegralSessionBase::CIntegralSessionBase(void)
  {
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CIntegralSessionBase::~CIntegralSessionBase(void)
  {
  Clear();
  }
//+------------------------------------------------------------------+
//| Clear all sessions                                               |
//+------------------------------------------------------------------+
void CIntegralSessionBase::Clear(void)
  {
  m_sync.Lock();
  //---
  for(UINT i=0;i<m_sessions.Total();i++)
    IntegralSessionFree(m_sessions[i]);
  m_sessions.Shutdown();
  //--- unlock
  m_sync.Unlock();
  }
//+------------------------------------------------------------------+
//| Storing session                                                  |
//+------------------------------------------------------------------+
bool CIntegralSessionBase::Add(const std::string &frozen_string,const CIntegralSessionRecord &record)
  {
  m_sync.Lock();
  //---
  IntegralSessionRecord *rec=IntegralSessionAllocate();
  if(rec)
    {
    rec->frozen_string=frozen_string;
    rec->record       =record;
    if(m_sessions.Insert(&rec,SortIntegralSessions)!=NULL)
      {
      m_sync.Unlock();
      return(true);
      }
    IntegralSessionFree(rec);
    }
  m_sync.Unlock();
  return(false);
  }
//+------------------------------------------------------------------+
//| get session                                                      |
//+------------------------------------------------------------------+
bool CIntegralSessionBase::GetSession(const std::string &frozen_string,const UINT session_type,CIntegralSessionRecord &record)
  {
  bool res=false;
  //---
  m_sync.Lock();
  //--- find record
  for(UINT pos=0;pos<m_sessions.Total();pos++)
    {
    const IntegralSessionRecord *ptr=m_sessions[pos];
    if(frozen_string==ptr->frozen_string)
      if(ptr->record.SessionType()==session_type)
        {
        //---
        record=ptr->record;
        res   =true;
        break;
        }
    }
  //---
  m_sync.Unlock();
  return(res);
  }
//+------------------------------------------------------------------+
//| get session                                                      |
//+------------------------------------------------------------------+
bool CIntegralSessionBase::GetSession(const std::string &frozen_string,CIntegralSessionRecord &record)
  {
  bool res=false;
  //---
  m_sync.Lock();
  //--- find record
  for(UINT pos=0;pos<m_sessions.Total();pos++)
    {
    const IntegralSessionRecord *ptr=m_sessions[pos];
    if(frozen_string==ptr->frozen_string)
      {
      //---
      record=ptr->record;
      res   =true;
      break;
      }
    }
  //---
  m_sync.Unlock();
  return(res);
  }
//+------------------------------------------------------------------+
//| get session                                                      |
//+------------------------------------------------------------------+
bool CIntegralSessionBase::GetSessionByShortName(const std::string &short_name,const UINT session_type,CIntegralSessionRecord &record)
  {
  bool res=false;
  //---
  m_sync.Lock();
  //--- find record
  for(UINT pos=0;pos<m_sessions.Total();pos++)
    {
    if(short_name==m_sessions[pos]->record.ShortName())
      if(m_sessions[pos]->record.SessionType()==session_type)
        {
        //---
        record=m_sessions[pos]->record;
        res   =true;
        break;
        }
    }
  //---
  m_sync.Unlock();
  return(res);
  }
//+------------------------------------------------------------------+
//| get session                                                      |
//+------------------------------------------------------------------+
bool CIntegralSessionBase::GetOrderSession(const std::string &frozen_string,CIntegralSessionRecord &record)
  {
  bool res=false;
  //---
  m_sync.Lock();
  //--- find record
  for(UINT pos=0;pos<m_sessions.Total();pos++)
    {
    if(m_sessions[pos]->frozen_string!=frozen_string||
       m_sessions[pos]->record.SessionType()!=CIntegralSessionRecord::Order) continue;
    //---
    record=m_sessions[pos]->record;
    res   =true;
    break;
    }
  //---
  m_sync.Unlock();
  return(res);
  }
//+------------------------------------------------------------------+
//| get session                                                      |
//+------------------------------------------------------------------+
bool CIntegralSessionBase::GetQuoteSession(const std::string &frozen_string,CIntegralSessionRecord &record)
  {
  bool res=false;
  //---
  m_sync.Lock();
  //--- find record
  for(UINT pos=0;pos<m_sessions.Total();pos++)
    {
    if(m_sessions[pos]->frozen_string!=frozen_string||
       m_sessions[pos]->record.SessionType()!=CIntegralSessionRecord::Quote) continue;
    //---
    record=m_sessions[pos]->record;
    res   =true;
    break;
    }
  //---
  m_sync.Unlock();
  return(res);
  }
//+------------------------------------------------------------------+
//| clear subscriptions                                              |
//+------------------------------------------------------------------+
void CIntegralSessionBase::ClearSubscriptions(const std::string &frozen_string,CIntegralSessionRecord &record)
  {
  //---
  m_sync.Lock();
  //--- find record
  for(UINT pos=0;pos<m_sessions.Total();pos++)
    {
    if(m_sessions[pos]->frozen_string!=frozen_string) continue;
    //---
    m_sessions[pos]->record.Clear();
    record=m_sessions[pos]->record;
    break;
    }
  m_sync.Unlock();
  }
//+------------------------------------------------------------------+
//| Updating session                                                 |
//+------------------------------------------------------------------+
bool CIntegralSessionBase::Update(const std::string &frozen_string,const UINT session_type,const UINT session_state)
  {
  bool res=false;
  //---
  m_sync.Lock();
  //--- find record
  for(UINT pos=0;pos<m_sessions.Total();pos++)
    {
    if(frozen_string!=m_sessions[pos]->frozen_string||
       m_sessions[pos]->record.SessionType()!=session_type) continue;
    //---
    m_sessions[pos]->record.FillSessionState(session_state);
    res   =true;
    break;
    }
  //---
  m_sync.Unlock();
  return(res);
  }
//+------------------------------------------------------------------+
//| Updating session                                                 |
//+------------------------------------------------------------------+
bool CIntegralSessionBase::Update(const std::string &frozen_string,const UINT session_state)
  {
  bool res=false;
  //---
  m_sync.Lock();
  //--- find record
  for(UINT pos=0;pos<m_sessions.Total();pos++)
    {
    if(frozen_string!=m_sessions[pos]->frozen_string) continue;
    //---
    m_sessions[pos]->record.FillSessionState(session_state);
    res   =true;
    break;
    }
  //---
  m_sync.Unlock();
  return(res);
  }
//+------------------------------------------------------------------+
//| Updating session                                                 |
//+------------------------------------------------------------------+
bool CIntegralSessionBase::Update(const std::string &frozen_string,const CIntegralSessionRecord &record)
  {
  bool res=false;
  //---
  m_sync.Lock();
  //--- find record
  for(UINT pos=0;pos<m_sessions.Total();pos++)
    {
    if(frozen_string!=m_sessions[pos]->frozen_string) continue;
    //---
    m_sessions[pos]->record=record;
    res   =true;
    break;
    }
  //---
  m_sync.Unlock();
  return(res);
  }
//+------------------------------------------------------------------+
//| Remove session                                                   |
//+------------------------------------------------------------------+
bool CIntegralSessionBase::Remove(const std::string &frozen_string,const UINT session_type)
  {
  bool res=false;
  //---
  m_sync.Lock();
  //--- find record
  for(UINT pos=0;pos<m_sessions.Total();pos++)
    {
    if(frozen_string!=m_sessions[pos]->frozen_string||
       m_sessions[pos]->record.SessionType()!=session_type) continue;
    //---
    IntegralSessionRecord *deleted=m_sessions[pos];
    m_sessions.Delete(&m_sessions[pos]);
    IntegralSessionFree(deleted);
    res   =true;
    break;
    }
  //---
  m_sync.Unlock();
  return(res);
  }
//+------------------------------------------------------------------+
//| Extract session                                                  |
//+------------------------------------------------------------------+
bool CIntegralSessionBase::Remove(const std::string &frozen_string,const UINT session_type,CIntegralSessionRecord &record)
  {
  bool res=false;
  //---
  m_sync.Lock();
  //--- find record
  for(UINT pos=0;pos<m_sessions.Total();pos++)
    {
    if(frozen_string!=m_sessions[pos]->frozen_string||
       m_sessions[pos]->record.SessionType()!=session_type) continue;
    //---
    IntegralSessionRecord *deleted=m_sessions[pos];
    m_sessions.Delete(&m_sessions[pos]);
    if(deleted)
      {
      record=deleted->record;
      IntegralSessionFree(deleted);
      //---
      res=true;
      }
    break;
    }
  //---
  m_sync.Unlock();
  return(res);
  }
//+------------------------------------------------------------------+
//| get sessions                                                     |
//+------------------------------------------------------------------+
void CIntegralSessionBase::GetSessions(SessionIdentifiers &sessions)
  {
  //---
  sessions.clear();
  //---
  m_sync.Lock();
  //---
  for(UINT pos=0;pos<m_sessions.Total();pos++)
    {
    sessions.push_back(m_sessions[pos]->record.SessionId());
    }
  m_sync.Unlock();
  }
//+------------------------------------------------------------------+
//| get sessions                                                     |
//+------------------------------------------------------------------+
void CIntegralSessionBase::GetSessions(SessionIdentifiers &sessions,const UINT m_session_state)
  {
  //---
  sessions.clear();
  //---
  m_sync.Lock();
  //---
  for(UINT pos=0;pos<m_sessions.Total();pos++)
    {
    if(m_sessions[pos]->record.SessionState()==m_session_state)
      sessions.push_back(m_sessions[pos]->record.SessionId());
    }
  m_sync.Unlock();
  }
//+------------------------------------------------------------------+
//| get sessions                                                     |
//+------------------------------------------------------------------+
bool CIntegralSessionBase::SymbolExists(const std::string &frozen_string,LPCSTR symbol)
  {
  //--- checks
  if(!symbol||frozen_string.empty())
    return(false);
  //---
  CLocker lock(m_sync);
  //---
  for(UINT pos=0;pos<m_sessions.Total();pos++)
    {
    if(m_sessions[pos]->frozen_string!=frozen_string) continue;
    //----
    return(m_sessions[pos]->record.SymbolExists(symbol));
    }
  //---
  return(false);
  }
//+------------------------------------------------------------------+
//| Allocate session                                                 |
//+------------------------------------------------------------------+
IntegralSessionRecord *CIntegralSessionBase::IntegralSessionAllocate(void)
  {
  IntegralSessionRecord *record=new(std::nothrow) IntegralSessionRecord;
  return(record);
  }
//+------------------------------------------------------------------+
//| Free session                                                     |
//+------------------------------------------------------------------+
void CIntegralSessionBase::IntegralSessionFree(IntegralSessionRecord *record)
  {
  if(record)
    delete record;
  }
//+------------------------------------------------------------------+
//| Sort sessions                                                    |
//+------------------------------------------------------------------+
int CIntegralSessionBase::SortIntegralSessions(const void *left,const void *right)
  {
  IntegralSessionRecord *lft=*(IntegralSessionRecord **)left;
  IntegralSessionRecord *rgh=*(IntegralSessionRecord **)right;
  //---
  return(StringType::Compare(lft->frozen_string.c_str(),rgh->frozen_string.c_str()));
  }
//+------------------------------------------------------------------+
//| Search sessions                                                  |
//+------------------------------------------------------------------+
int CIntegralSessionBase::SearchIntegralSessions(const void *left,const void *right)
  {
  LPCSTR                 lft=*(LPCSTR *)left;
  IntegralSessionRecord *rgh=*(IntegralSessionRecord **)right;
  //---
  return(StringType::Compare(lft,rgh->frozen_string.c_str()));
  }
//+------------------------------------------------------------------+
