//+------------------------------------------------------------------+
//|                                         MetaTrader 5 NFA.Reports |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "StdAfx.h"
#include "IntegralSession.h"
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CIntegralSessionRecord::CIntegralSessionRecord(void)
  {
  m_subscriptions.clear();
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CIntegralSessionRecord::~CIntegralSessionRecord(void)
  {
  m_subscriptions.clear();
  }
//+------------------------------------------------------------------+
//| Filling session id                                               |
//+------------------------------------------------------------------+
void CIntegralSessionRecord::FillSessionId(const FIX::SessionID &session_id)
  {
  m_session_id=session_id;
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CIntegralSessionRecord::FillShortName(const std::string &short_name)
  {
  m_short_name=short_name;
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CIntegralSessionRecord::FillSessionState(const UINT session_state)
  {
  m_session_state=session_state;
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CIntegralSessionRecord::FillSessionType(const UINT session_type)
  {
  m_session_type=session_type;
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CIntegralSessionRecord::AddSymbol(const std::string &symbol)
  {
  //---
  try
    {
    //--- checks
    if(std::find(m_subscriptions.begin(),m_subscriptions.end(),symbol)!=m_subscriptions.end())
      return(true);
    //---
    auto it = std::upper_bound(m_subscriptions.cbegin(),m_subscriptions.cend(),symbol); //1
    if(m_subscriptions.insert(it,symbol)==m_subscriptions.end()) //2 
      return(false);
    //----
    return(true);
    }
  catch(const std::exception &e)
    {
    LOG_ERROR(pluginLog,"%s",e.what());
    }
  //---
  return(false);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CIntegralSessionRecord::RemoveSymbol(const std::string &symbol)
  {
  SubscriptionsArray::iterator it;
  //---
  try
    {
    //--- checks
    if((it=std::find(m_subscriptions.begin(),m_subscriptions.end(),symbol))!=m_subscriptions.end())
      return(true);
    //---
    m_subscriptions.erase(it);
    //----
    return(true);
    }
  catch(const std::exception &e)
    {
    LOG_ERROR(pluginLog,"%s",e.what());
    }
  //---
  return(false);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CIntegralSessionRecord::SymbolExists(const std::string &symbol)
  {
  SubscriptionsArray::iterator it;
  //---
  try
    {
    //--- checks
    if((it=std::find(m_subscriptions.begin(),m_subscriptions.end(),symbol))!=m_subscriptions.end())
      return(true);
    //----
    return(false);
    }
  catch(const std::exception &e)
    {
    LOG_ERROR(pluginLog,"%s",e.what());
    }
  //---
  return(false);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CIntegralSessionRecord::Clear(void)
  {
  m_subscriptions.clear();
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
UINT CIntegralSessionRecord::SessionType(void) const
  {
  return(m_session_type);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
UINT CIntegralSessionRecord::SessionState(void) const
  {
  return(m_session_state);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
FIX::SessionID CIntegralSessionRecord::SessionId(void) const
  {
  return(m_session_id);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
LPCSTR CIntegralSessionRecord::SenderCompId(void) const
  {
  return(m_session_id.getSenderCompID().getString().c_str());
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
std::string CIntegralSessionRecord::ShortName(void) const
  {
  return(m_short_name);
  }
//+------------------------------------------------------------------+
