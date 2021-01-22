//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "ExecutionBase.h"
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CExecutionBase::CExecutionBase(void)
  {
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CExecutionBase::~CExecutionBase(void)
  {
  Clear();
  }
//+------------------------------------------------------------------+
//| Clear all requests                                               |
//+------------------------------------------------------------------+
void CExecutionBase::Clear(void)
  {
  //CLocker lock(m_sync);
  //---
  /*for(UINT i=0;i<m_executions.Total();i++)
    ExecutionFree(m_executions[i]);
  m_executions.Shutdown();*/
  }
//+------------------------------------------------------------------+
//| Storing request                                                  |
//+------------------------------------------------------------------+
//bool CExecutionBase::Add(LPCSTR unique_id,const IntegralExecution &record)
//  {
//  //--- chcks
//  if(!unique_id)
//    return(false);
//  //---
//  CLocker lock(m_sync);
//  //---
//  ExecutionReportRecord *rec=ExecutionAllocate();
//  if(rec)
//    {
//    StringType::Copy(rec->unique_id,unique_id);
//    rec->record =record;
//    if(m_executions.Insert(&rec,SortExecutions)!=NULL)
//      return(true);
//    ExecutionFree(rec);
//    }
//  return(false);
//  }
//+------------------------------------------------------------------+
//| Remove request                                                   |
//+------------------------------------------------------------------+
//bool CExecutionBase::Remove(LPCSTR unique_id)
//  {
//  CLocker lock(m_sync);
//  //---
//  ExecutionReportRecord **ptr=m_executions.Search(&unique_id,SearchExecutions);
//  if(ptr)
//    {
//    ExecutionReportRecord *deleted=*ptr;
//    m_executions.Delete(ptr);
//    ExecutionFree(deleted);
//    return(true);
//    }
//  return(false);
//  }
//+------------------------------------------------------------------+
//| Extract request                                                  |
//+------------------------------------------------------------------+
//bool CExecutionBase::Remove(LPCSTR unique_id,IntegralExecution &record)
//  {
//  CLocker lock(m_sync);
//  //---
//  ExecutionReportRecord **ptr=m_executions.Search(&unique_id,SearchExecutions);
//  if(ptr)
//    {
//    ExecutionReportRecord *deleted=*ptr;
//    m_executions.Delete(ptr);
//    if(deleted)
//      {
//      record =deleted->record;
//      ExecutionFree(deleted);
//      return(true);
//      }
//    }
//  return(false);
//  }
//+------------------------------------------------------------------+
//| Storing execution                                                |
//+------------------------------------------------------------------+
bool CExecutionBase::Add(const std::string &unique_id,const Ack &ack)
  {
  //--- chcks
  if(unique_id.empty())
    return(false);
  //---
  m_sync.Lock();
  //---
  try
    {
    fxi_orders[unique_id]=ack;
    m_sync.Unlock();
    return(true);
    }
  catch(const std::exception &ex)
    {
    LOG_ERROR(pluginLog,"%s",ex.what());
    }
  //---
  m_sync.Unlock();
  return(false);
  }
//+------------------------------------------------------------------+
//| Remove request                                                   |
//+------------------------------------------------------------------+
bool CExecutionBase::Remove(const std::string &unique_id)
  {
  //--- checks
  if(unique_id.empty())
    return(false);
  //---
  m_sync.Lock();
  //---
  try
    {
    // has()
    if(fxi_orders.find(unique_id)==fxi_orders.end())
      {
      m_sync.Unlock();
      return(false);
      }
    //---
    fxi_orders.erase(unique_id);
    //--- successfull
    m_sync.Unlock();
    return(true);
    }
  catch(const std::exception &ex)
    {
    LOG_ERROR(pluginLog,"%s",ex.what());
    }
  //---
  m_sync.Unlock();
  return(false);
  }
//+------------------------------------------------------------------+
//| Remove request                                                   |
//+------------------------------------------------------------------+
bool CExecutionBase::Remove(const std::string &unique_id,Ack &ack)
  {
  //--- checks
  if(unique_id.empty())
    return(false);
  //---
  m_sync.Lock();
  //---
  try
    {
    if(fxi_orders.find(unique_id)==fxi_orders.end())
      {
      m_sync.Unlock();
      return(false);
      }
    //---
    ack=fxi_orders[unique_id];
    fxi_orders.erase(unique_id);
    //---
    m_sync.Unlock();
    return(true);
    }
  catch(const std::exception &ex)
    {
    LOG_ERROR(pluginLog,"%s",ex.what());
    }
  //---
  m_sync.Unlock();
  return(false);
  }
//+------------------------------------------------------------------+
//| Get execution report                                             |
//+------------------------------------------------------------------+
bool CExecutionBase::Get(LPCSTR unique_id,Ack &ack)
  {
  //--- checks
  if(!unique_id)
    return(false);
  //---
  m_sync.Lock();
  //---
  try
    {
    if(fxi_orders.find(unique_id)==fxi_orders.end())
      {
      m_sync.Unlock();
      return(false);
      }
    //---
    ack=fxi_orders[unique_id];
    //---
    m_sync.Unlock();
    return(true);
    }
  catch(const std::exception &ex)
    {
    LOG_ERROR(pluginLog,"%s",ex.what());
    }
  //---
  m_sync.Unlock();
  return(false);
  }
//+------------------------------------------------------------------+
//| Allocate request                                                 |
//+------------------------------------------------------------------+
//ExecutionReportRecord *CExecutionBase::ExecutionAllocate(void)
//  {
//  ExecutionReportRecord *record=new(std::nothrow) ExecutionReportRecord;
//  return(record);
//  }
//+------------------------------------------------------------------+
//| Free request                                                     |
//+------------------------------------------------------------------+
//void CExecutionBase::ExecutionFree(ExecutionReportRecord *record)
//  {
//  if(record)
//    delete record;
//  }
//+------------------------------------------------------------------+
//| Sort requests                                                    |
//+------------------------------------------------------------------+
//int CExecutionBase::SortExecutions(const void *left,const void *right)
//  {
//  ExecutionReportRecord *lft=*(ExecutionReportRecord **)left;
//  ExecutionReportRecord *rgh=*(ExecutionReportRecord **)right;
//  //---
//  return(StringType::Compare(lft->unique_id,rgh->unique_id));
//  }
//+------------------------------------------------------------------+
//| Search requests                                                  |
//+------------------------------------------------------------------+
//int CExecutionBase::SearchExecutions(const void *left,const void *right)
//  {
//  LPCSTR &lft=*(LPCSTR *)left;
//  ExecutionReportRecord *rgh=*(ExecutionReportRecord **)right;
//  //---
//  return(StringType::Compare(lft,rgh->unique_id));
//  }
//+------------------------------------------------------------------+
