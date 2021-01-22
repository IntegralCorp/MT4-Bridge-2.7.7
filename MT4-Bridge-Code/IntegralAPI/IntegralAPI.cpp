//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#include "StdAfx.h"
#include "IntegralAPI.h"
#include "..\Bases\Gateway.h"
#include "..\Common\Const.h"
//+------------------------------------------------------------------+
//| 'this' : used in base member initializer list                    |
//+------------------------------------------------------------------+
#pragma warning(disable: 4355)
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
CIntegralAPI::CIntegralAPI(CGateway *gateway)
  : m_gateway(gateway),
  m_settings_dispatcher(gateway),
  m_integral_context(*this),
  m_thread_workflag(0)
  {
  m_sessions.Clear();
  }
#pragma warning(default: 4355)
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
CIntegralAPI::~CIntegralAPI()
  {
  //--- clear pointers
  m_gateway=NULL;
  //---
  //Shutdown();
  //--- shutdown sessions
  m_sessions.Clear();
  }
//+------------------------------------------------------------------+
//| Initialization                                                   |
//+------------------------------------------------------------------+
bool CIntegralAPI::Initialize(void)
  {
  StringType256 short_name;
  //--- shutdown
  Shutdown();
  //--- initialize settings
  if(!m_settings_dispatcher.Initialize())
    return(false);
  //--- initialize trade connection context
  if(!m_integral_context.Initialize())
    return(false);
  //--- set thread's working flag
  InterlockedExchange(&m_thread_workflag,1);
  //--- start thread of processing data
  if(!m_thread.Start(ProcessThreadWrapper,this,STACK_SIZE_COMMON))
    {
    LOG_ERROR(pluginLog,"failed to start exchange API work thread");
    return(false);
    }
  //--- successful
  return(true);
  }
//+------------------------------------------------------------------+
//| Shutdown                                                         |
//+------------------------------------------------------------------+
void CIntegralAPI::Shutdown()
  {
  //--- reset thread's working flag
  InterlockedExchange(&m_thread_workflag,0);
  //--- shutdown thread
  m_thread.Shutdown();
  //--- shutdown external connection context
  m_integral_context.Shutdown();
  //--- shutdown settings
  m_settings_dispatcher.Shutdown();
  }
//+------------------------------------------------------------------+
//| check order validity                                             |
//+------------------------------------------------------------------+
bool CIntegralAPI::CheckGroupAndSymbol(const int login,LPCSTR symbol_name,LPCSTR group_name,bool add_logs)
  {
  Instrument symbol_record={0};
  Group  group_record={0};
  ConSymbolGroup sym_group={0};
  StringType64 short_name;
  //--- chceks
  if(!symbol_name||!group_name||login<1)
    return(false);
  //---
  if(!m_settings_dispatcher.GetSymbolByName(symbol_name,symbol_record))
    {
    LOG_ERROR(pluginLog,"Failed to get symbol record %s",symbol_name);
    return false;
    }
  //---
  if(!m_settings_dispatcher.GetGroupByName(group_name,group_record))
    {
    LOG_ERROR(pluginLog,"Failed to get group record %s",group_name);
    return false;
    }
  //--- enabled?
  if(!symbol_record.enabled||!group_record.enabled)
    {
    LOG_DEBUG(pluginLog,"'%d' Symbol/Group Combination not Enabled: '%s'/'%s' ",login,symbol_name,group_name);
    return false;
    }
  //---
  if(group_record.secgroups[symbol_record.type].execution!=EXECUTION_MANUAL)
    {
    ExtServer->SymbolsGroupsGet(symbol_record.type,&sym_group);
    LOG_DEBUG(pluginLog,"Manual Execution not enabled for Group/Security: %s/%s",
              group_record.group,
              sym_group.name);
    return(false);
    }
  //--- successfull
  return(true);
  }
//+------------------------------------------------------------------+
//| Notify of context state update                                   |
//+------------------------------------------------------------------+
void CIntegralAPI::OnContextStateUpdate(const UINT state)
  {
  //--- check
  if(!m_gateway)
    return;
  //--- notify gateway, if context is in the data processing state
  if(m_integral_context.StateGet()==CIntegralContext::STATE_SYNCHRONIZED)
    {
    //--- notify gateway
    m_gateway->OnIntergalSyncComplete();
    }
  //--- notify gateway, if context is stopped
  if(state==CIntegralContext::STATE_STOPPED)
    {
    //--- notify gateway
    m_gateway->OnIntergalConnectClosed();
    }
  }
//+------------------------------------------------------------------+
//| Notify of session loaded                                         |
//+------------------------------------------------------------------+
void CIntegralAPI::OnSessionLoaded(const FIX::SessionSettings &settings)
  {
  if(m_gateway)
    m_gateway->OnSessionLoaded(settings);
  }
//+------------------------------------------------------------------+
//| Logout message received                                          |
//+------------------------------------------------------------------+
bool CIntegralAPI::OnCreate(const FIX::SessionID &session_id,LPCSTR short_name,const UINT session_type)
  {
  CIntegralSessionRecord record;
  //--- checks
  if(!short_name)
    return(false);
  //---
  if(!m_settings_dispatcher.CheckInstrumentsFile(short_name)||
     !m_settings_dispatcher.CheckGroupsFile(short_name))
    {
    return(false);
    }
  //--- set  
  record.FillSessionId(session_id);
  record.FillShortName(short_name);
  record.FillSessionState(CIntegralSessionRecord::Created);
  //--- set session type
  record.FillSessionType(session_type);
  //---
  if(!m_sessions.Add(session_id.toString(),record))
    {
    LOG_ERROR(pluginLog,"add to sessions failed Session:<%s>",session_id.toString());
    return(false);
    }
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| Logon message received                                           |
//+------------------------------------------------------------------+
bool CIntegralAPI::OnLogon(const FIX::SessionID &session_id,const UINT &session_type,LPCSTR short_name)
  {
  CIntegralSessionRecord record;
  StringType256          message;
  bool                   res=false;
  //--- checks
  if(!short_name)
    return(false);
  //--- get session
  if(!m_sessions.GetSession(session_id.toString(),session_type,record))
    {
    message.Format("session get failed Session:<%s>",session_id.toString().c_str());
    //--- logout
    m_integral_context.SendLogout(session_id,message.Str());
    //--- retry later
    return(false);
    }
  //---
  switch(session_type)
    {
    case CIntegralSessionRecord::Quote:
      //--- read settings files
      if(!m_settings_dispatcher.ReadSymbols())
        {
        LOG_ERROR(pluginLog,"OnLogon => ReadSymbols() failed");
        return(false);
        }
      //---
      res=OnQuoteSession(record,short_name);
      break;
    case CIntegralSessionRecord::Order:
      if(!m_settings_dispatcher.ReadGroups())
        {
        LOG_ERROR(pluginLog,"OnLogon => ReadGroups() failed");
        return(false);
        }
      //---
      if((res=OnOrderSession(record,short_name)))
        {
        m_gateway->OnLogon(short_name);
        }
      break;
    default:
      break;
    }
  //---
  return(res);
  }
//+------------------------------------------------------------------+
//| Logout message received                                          |
//+------------------------------------------------------------------+
void CIntegralAPI::OnLogout(const FIX::SessionID &session_id,const UINT &session_type,LPCSTR short_name)
  {
  //--- checks
  if(!short_name)
    return;
  //---
  if(!m_sessions.Update(session_id.toString(),session_type,CIntegralSessionRecord::Disconnected))
    {
    LOG_ERROR(pluginLog,"session state update failed <Session:%s Status:%d>",
              session_id.toString(),CIntegralSessionRecord::Disconnected);
    }
  }
//+------------------------------------------------------------------+
//| Admin Integration                                                |
//+------------------------------------------------------------------+
bool  CIntegralAPI::AdminIntegration(const UserRecord *info)
  {
  Group group={0};
  //--- checks
  if(!info)
    return(false);
  //---
  if(!m_settings_dispatcher.GetGroupByName(info->group,group))
    return(false);
  //---
  return(m_integral_context.AdminIntegration(info,group));
  }
//+------------------------------------------------------------------+
//| Check integral trading system API                                |
//+------------------------------------------------------------------+
bool CIntegralAPI::Check()
  {
  INT64 ctm=_time64(NULL);
  //--- check context
  return(m_integral_context.Check(ctm));
  }
//+------------------------------------------------------------------+
//| Send order                                                       |
//+------------------------------------------------------------------+
bool CIntegralAPI::SendOrder(IntegralOrder &order)
  {
  Instrument             instrument={0};
  Group                  group={0};
  RouteRecordsArray      rules;
  CIntegralSessionRecord record={};
  MarketRange            market_range={0};
  //--- get symbol
  if(!m_settings_dispatcher.GetSymbolByName(order.symbol,instrument))
    {
    LOG_ERROR(pluginLog,"record get for symbol %s failed for #%d",order.symbol,order.order);
    return(false);
    }
  //--- get group
  if(!m_settings_dispatcher.GetGroupByName(order.group,group))
    {
    LOG_ERROR(pluginLog,"record get for group %s failed for #%d",order.group,order.order);
    return(false);
    }
  //--- check
  if(!instrument.enabled||!group.enabled)
    {
    return(false);
    }
  //--- get rules
  m_settings_dispatcher.FillRule(order);
  //--- fill remainig info
  if(!IntegralOrder::Fill(&instrument,&group,order))
    {
    return(false);
    }
  //---
  if(order.range_type==Instrument::Percentage)
    {
    //---
    if(m_settings_dispatcher.GetMarketRangeByCcyPair(order.ccy_pair,market_range))
      {
      order.mrkt_range=order.mrkt_range*market_range.pips_factor*order.price_order;
      }
    else
      order.mrkt_range=-1.0;
    }
  //--- get session
  if(!m_sessions.GetSessionByShortName(order.short_name,CIntegralSessionRecord::Order,record))
    return(false);
  //--- check session state
  if(record.SessionState()!=CIntegralSessionRecord::Active)
    return(false);
  //--- set
  order.session_id=record.SessionId();
  //--- pass the order to context
  return(m_integral_context.SendOrder(order));
  }
//+------------------------------------------------------------------+
//| Send acknowledgement                                             |
//+------------------------------------------------------------------+
bool CIntegralAPI::SendAck(LPCSTR unique_id)
  {
  //--- pass the order to context
  return(m_integral_context.SendAck(unique_id));
  }
//+------------------------------------------------------------------+
//| Send Dont know trade                                             |
//+------------------------------------------------------------------+
bool CIntegralAPI::SendDontKnowTrade(LPCSTR order_str,const UINT reason,LPCSTR text)
  {
  //--- pass the order to context
  return(m_integral_context.SendDontKnowTrade(order_str,reason,text));
  }
//+------------------------------------------------------------------+
//| Send order status request                                        |
//+------------------------------------------------------------------+
bool CIntegralAPI::SendOrderStatusRequest(LPCSTR order_str)
  {
  //--- pass the order to context
  return(m_integral_context.SendOrderStatusRequest(order_str));
  }
//+------------------------------------------------------------------+
//| Find symbol by ccy pair                                          |
//+------------------------------------------------------------------+
bool CIntegralAPI::SymbolGet(LPCSTR ccy_pair,Instrument &symbol)
  {
  //--- find symbol
  return(m_settings_dispatcher.GetSymbolByCcyPair(ccy_pair,symbol));
  }
//+------------------------------------------------------------------+
//| Find symbol by name                                              |
//+------------------------------------------------------------------+
bool CIntegralAPI::SymbolGetByName(LPCSTR symbol_name,Instrument &symbol)
  {
  //--- find symbol
  return(m_settings_dispatcher.GetSymbolByName(symbol_name,symbol));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CIntegralAPI::GetSessions(SessionIdentifiers &sessions,const UINT session_state)
  {
  //--- get sessions
  m_sessions.GetSessions(sessions,session_state);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CIntegralAPI::GetSessions(SessionIdentifiers &sessions)
  {
  //--- get sessions
  m_sessions.GetSessions(sessions);
  }
//+------------------------------------------------------------------+
//| notify of adding/updating of a client group.                     |
//+------------------------------------------------------------------+
bool CIntegralAPI::UpdateGroupCache(const ConGroup *group)
  {
  //--- checks
  if(!group)
    return(false);
  //---
  return(m_settings_dispatcher.UpdateGroupCache(group));
  }
//+------------------------------------------------------------------+
//| notify of adding/updating of a symbol.                           |
//+------------------------------------------------------------------+
bool CIntegralAPI::UpdateSymbolCache(const ConSymbol *cs)
  {
  //--- checks
  if(!cs)
    return(false);
  //---
  return(m_settings_dispatcher.UpdateSymbolCache(cs));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
std::string CIntegralAPI::GetConfigs(void)
  {
  //---
  return(m_settings_dispatcher.GetConfigs());
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CIntegralAPI::SetGroup(std::string &group,std::string &config,const bool save_file,const bool force_update)
  {
  //---
  return(m_settings_dispatcher.SetGroup(group,config,save_file,force_update));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CIntegralAPI::SetRule(const int index,StringType &config,const bool save_file,const bool force_update)
  {
  //---
  return(m_settings_dispatcher.SetRule(index,config,save_file,force_update));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CIntegralAPI::SetInstrument(std::string &symbol,std::string &config,const bool save_file,const bool force_update)
  {
  Instrument             instrument={};
  CIntegralSessionRecord session={};
  //--- checks
  if(symbol.empty())
    return(false);
  //---
  if(!m_settings_dispatcher.SetInstrument(symbol,config,save_file,force_update))
    return(false);
  //---
  if(!m_settings_dispatcher.GetSymbolByName(symbol.c_str(),instrument)||!m_sessions.GetSessionByShortName(instrument.short_name,CIntegralSessionRecord::EnSessionType::Quote,session))
    return(false);
  //---
  return(m_integral_context.Subscribe(&instrument,session));
  }
//+------------------------------------------------------------------+
//| Start thread of data processing                                  |
//+------------------------------------------------------------------+
UINT __stdcall CIntegralAPI::ProcessThreadWrapper(LPVOID param)
  {
  //--- process work thread
  CIntegralAPI *pThis=reinterpret_cast<CIntegralAPI *>(param);
  if(pThis!=NULL) pThis->ProcessThread();
  //---
  return(0);
  }
//+------------------------------------------------------------------+
//| Exchange API work thread                                         |
//+------------------------------------------------------------------+
void CIntegralAPI::ProcessThread()
  {
  bool trans_applied=false;
  //--- loop of data processing
  while(InterlockedExchangeAdd(&m_thread_workflag,0)>0)
    {
    //--- process received data
    m_integral_context.TransApply(m_settings_dispatcher,trans_applied);
    //--- sleep if there are no transactions
    if(!trans_applied)
      Sleep(TRANS_WAIT_TIME);
    }
  //---
  LOG_INFO(pluginLog,"Exchange API work thread finished");
  }
//+------------------------------------------------------------------+
//| Quote session connected                                          |
//+------------------------------------------------------------------+
bool CIntegralAPI::OnQuoteSession(CIntegralSessionRecord &record,LPCSTR short_name)
  {
  InstrumentRecordsArray instruments;
  StringType256          message;
  StringType128          temp;
  //--- checks
  if(!short_name)
    return(false);
  //--- clear ccy pairs
  LOG_INFO(pluginLog,"OnQuoteSession <%s> [%s]",short_name,record.SessionId().toString());
  //
  m_sessions.ClearSubscriptions(record.SessionId().toString(),record);
  if(!m_settings_dispatcher.GetInstruments(short_name,instruments))
    {
    message.Format("ReadSymbols failed Session:<%s>",record.SessionId().toString().c_str());
    //--- logout
    m_integral_context.SendLogout(record.SessionId(),message.Str());
    //--- retry later
    return(false);
    }
  //--- subscribe
  for(UINT pos=0;pos<instruments.Total();pos++)
    {
    temp.Format("MT4-%s-%s",instruments[pos].short_name,instruments[pos].ccy_pair);
    if(record.SymbolExists(temp.Str())) continue;
    if(!record.AddSymbol(temp.Str()))   continue;
    //---
    if(!m_integral_context.Subscribe(&instruments[pos],record))
      {
      record.RemoveSymbol(temp.Str());
      continue;
      }
    }
  //--- update
  record.FillSessionState(CIntegralSessionRecord::Active);
  if(!m_sessions.Update(record.SessionId().toString(),record))
    {
    message.Format("session update failed Session:<%s>",record.SessionId().toString().c_str());
    //--- logout
    m_integral_context.SendLogout(record.SessionId(),message.Str());
    }
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| Order session connected                                          |
//+------------------------------------------------------------------+
bool CIntegralAPI::OnOrderSession(CIntegralSessionRecord &record,LPCSTR short_name)
  {
  StringType256 message;
  //--- checks
  if(!short_name)
    return(false);
  //--- update
  record.FillSessionState(CIntegralSessionRecord::Active);
  if(!m_sessions.Update(record.SessionId().toString(),record))
    {
    message.Format("session update failed Session:<%s>",record.SessionId().toString().c_str());
    //--- logout
    m_integral_context.SendLogout(record.SessionId(),message.Str());
    return(false);
    }
  //---
  return(true);
  }
//+------------------------------------------------------------------+
