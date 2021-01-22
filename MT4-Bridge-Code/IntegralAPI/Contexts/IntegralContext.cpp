//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "IntegralContext.h"
//+------------------------------------------------------------------+
//| Disable: 'this' : used in base member initializer list           |
//+------------------------------------------------------------------+
#pragma warning(push)
#pragma warning(disable: 4355)
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
CIntegralContext::CIntegralContext(CIntegralAPI &exchange_api)
  : m_integral_api(exchange_api),
  m_state(STATE_STOPPED),
  m_thread_workflag(0),
  m_thread_checktime(_time64(NULL)+THREAD_TIMEOUT),
  m_initiator(NULL),
  m_log_factory(NULL),
  m_logon(0),
  m_heartbeat_time(0)
  {
  }
#pragma warning(pop)
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
CIntegralContext::~CIntegralContext()
  {
  //FIX::Session *session;
  //---
  Shutdown();
  //---
  //if(m_initiator)
  //  {
  //  if(!m_initiator->isStopped())
  //    {
  //    for(auto it:FIX::Session::getSessions())
  //      {
  //      if((session=FIX::Session::lookupSession(it))==NULL)
  //        continue;
  //      //---
  //      if(session->isLoggedOn())
  //        SendLogout(it,"shutdown");
  //      }
  //    }
  //  //---
  //  m_initiator->stop(false);
  //  }
  //---
  for(auto &item:m_logs)
    {
    LOG_INFO(item.second,"------------------------------------ Stopping FIX (%s) Logs ------------------------------------\r\n",item.first.c_str());
    delete item.second;
    item.second=NULL;
    }
  //--- delete log factory
  if(m_log_factory)
    {
    delete m_log_factory;
    m_log_factory=NULL;
    }
  //--- delete initiator
  if(m_initiator)
    {
    //delete m_initiator;
    m_initiator=NULL;
    }
  //--- lock
  m_trans_sync.Lock();
  //--- clear all transaction arrays
  m_trans_books.clear();
  //m_trans_ticks_tmp.Clear();
  m_trans_books_tmp.clear();
  m_trans_executions.clear();
  m_trans_executions_tmp.clear();
  //--- unlock
  m_trans_sync.Unlock();
  //--- clear
  m_message_send_sync.Lock();
  m_message_send.Clear();
  m_message_send_sync.Unlock();
  }
//+------------------------------------------------------------------+
//| Initialize context                                               |
//+------------------------------------------------------------------+
bool CIntegralContext::Initialize(void)
  {
  StringTypePath           file;
  StringTypePath           xml_file;
  FIX::MemoryStoreFactory *storeFactory;
  IntegralSession          session;
  //---
  if(StateGet()!=STATE_STOPPED)
    {
    LOG_ERROR(pluginLog,"failed to initialize context, context already initialized (%u)",StateGet());
    return(false);
    }
  //--- shutdown
  Shutdown();
  //---
  try
    {
    //--- check if initiator object exists
    if(m_initiator==NULL)
      {
      //--- read fix settings
      StringTypePath path;
      path.Assign(ExtProgramPath);
      if(!FixSessionSettings.Initialize(path,m_settings))
        return(false);
      //--- notify gateway
      //m_integral_api.OnSessionLoaded(m_settings);

        //--- create log factory
      if(!m_log_factory)
        if((m_log_factory=new(std::nothrow) FIX::IdcLogFactory(m_settings))==NULL)
          {
          LOG_ERROR(pluginLog,"failed to create logfactory instance (%d)",::GetLastError());
          return(false);
          }
      //---
      if(!m_log_factory->Initialize(path.Str()))
        return(false);
      //--- create store factory
      if((storeFactory=new(std::nothrow)FIX::MemoryStoreFactory())==NULL)
        {
        LOG_ERROR(pluginLog,"failed to create store factory instance (%d)",::GetLastError());
        return(false);
        }
      //--- create initiator object if needed
      if((m_initiator=new(std::nothrow) FIX::ThreadedSocketInitiator(*this,*storeFactory,m_settings,*m_log_factory))==NULL)
        {
        LOG_ERROR(pluginLog,"failed to create initiator instance (%d)",::GetLastError());
        //---
        return(false);
        }
      //--- start log queues ?
      m_log_factory->StartLogQueues();

      }
    //---
    m_initiator->start();
    //--- update context state - processing thread started
    StateSet(STATE_STARTED);
    //--- update time before starting processing thread
    //UpdateTime((INT64)THREAD_TIMEOUT);
    }
  catch(const std::exception &e)
    {
    LOG_CRITICAL(pluginLog,"%s",e.what());
    //---
    return(false);
    }
  //--- set thread working flag
  InterlockedExchange(&m_thread_workflag,1);
  //--- start thread of processing external connection data
  if(!m_thread.Start(ProcessThreadWrapper,this,STACK_SIZE_COMMON))
    {
    LOG_ERROR(pluginLog,"failed to start context thread (%d)",::GetLastError());
    return(false);
    }
  //--- successfull
  return(true);
  }
//+------------------------------------------------------------------+
//| Shutdown                                                         |
//+------------------------------------------------------------------+
void CIntegralContext::Shutdown()
  {
  FIX::Session *session;
  //--- if logged in, send logout message
  if(m_initiator)
    {
    if(!m_initiator->isStopped())
      {
      for(auto it:FIX::Session::getSessions())
        {
        if((session=FIX::Session::lookupSession(it))==NULL)
          continue;
        //---
        if(session->isLoggedOn())
          SendLogout(it,"shutdown");
        }
      m_initiator->stop();
      }
    }
  //--- reset flag of thread operation
  InterlockedExchange(&m_thread_workflag,0);
  //--- shutdown thread
  m_thread.Shutdown();
  //--- update context state
  StateSet(STATE_STOPPED);
  }
//+------------------------------------------------------------------+
//| Getting state                                                    |
//+------------------------------------------------------------------+
UINT CIntegralContext::StateGet()
  {
  //--- return state of connection context
  return((UINT)InterlockedExchangeAdd((volatile LONG *)&m_state,0));
  }
//+------------------------------------------------------------------+
//| Setting state                                                    |
//+------------------------------------------------------------------+
UINT CIntegralContext::StateSet(UINT state)
  {
  UINT old_state=(UINT)InterlockedExchange((volatile LONG *)&m_state,state);
  //--- notify after changing state
  if(old_state!=state)
    m_integral_api.OnContextStateUpdate(state);
  //--- return previous state of connection context
  return(old_state);
  }
//+------------------------------------------------------------------+
//| Send message                                                     |
//+------------------------------------------------------------------+
//bool CIntegralContext::Send(IntegalMessageRecord &message)
//  {
//  FIX::MsgType   type;
//  //SymbolTick     tick={0};
//  bool           res;
//  StringType16   type_str;
//  StringType1024 message_str;
//  //--- check connection state
//  if(StateGet()==STATE_STOPPED)
//    return(false);
//  //--- 
//  if((res=FIX::Session::sendToTarget(message.fix_message,message.fix_session_id)))
//    {
//    LOG_INFO(pluginLog,"%s",message.fix_message.toString().c_str());
//    //---
//    FIX::ClOrdID unique_id;
//    StringType256 session_id;
//
//    session_id.Assign(message.fix_message.getSessionID().toString().c_str());
//    message.fix_message.getFieldIfSet(unique_id);
//    ExtWatchTower.AddOrder(session_id.Str(),unique_id.getValue().c_str(),message.fix_message);
//    }
//  else
//    LOG_ERROR(pluginLog,"%s",message.fix_message.toString().c_str());
//  //--- store last heartbeat sending time
//  m_heartbeat_time=_time64(NULL);
//  //---
//  return(res);
//  }
//+------------------------------------------------------------------+
//| Send message                                                     |
//+------------------------------------------------------------------+
bool CIntegralContext::Send(FIX::Message &message,FIX::SessionID &session_id)
  {
  //FIX::Session *session;
  bool res=true;
  //--- check connection state
  if(StateGet()==STATE_STOPPED)
    return(false);
  //--- 
  try
    {
    if((res=FIX::Session::sendToTarget(message,session_id)))
      {
      //--- store last heartbeat sending time
      m_heartbeat_time=_time64(NULL);
      LOG_INFO(pluginLog,"%s",message.toString().c_str());
      }
    else
      LOG_ERROR(pluginLog,"%s",message.toString().c_str());
    //---
    return(res);
    }
  catch(const std::exception &e)
    {
    LOG_ERROR(pluginLog,"%s",e.what());
    }
  //---
  return(false);
  }
//+------------------------------------------------------------------+
//| Send message                                                     |
//+------------------------------------------------------------------+
bool CIntegralContext::Send(FIX::Message &message)
  {
  FIX::ClOrdID unique_id;
  StringType256 session_id;
  bool res=true;
  //--- check connection state
  if(StateGet()==STATE_STOPPED)
    return(false);
  //--- 
  if((res=FIX::Session::sendToTarget(message)))
    {
    LOG_INFO(pluginLog,"%s",message.toString().c_str());
    //---
    session_id.Assign(message.getSessionID().toString().c_str());
    message.getFieldIfSet(unique_id);
    //ExtWatchTower.AddOrder(session_id.Str(),unique_id.getValue().c_str(),message);
    }
  else
    LOG_ERROR(pluginLog,"%s",message.toString().c_str());
  //--- store last heartbeat sending time
  m_heartbeat_time=_time64(NULL);
  //---
  return(res);
  }
//+------------------------------------------------------------------+
//| Send message                                                     |
//+------------------------------------------------------------------+
bool CIntegralContext::SendAsync(const IntegalMessageRecord &message)
  {
  bool res=true;
  //--- check connection state
  if(StateGet()==STATE_STOPPED)
    return(false);
  //---
  LOG_INFO(pluginLog,"Add=> %s",message.fix_message.toString());
  //--- lock outbound packets buffer
  m_message_send_sync.Lock();
  //--- add data for sending
  res=res&&m_message_send.Add(&message);
  //--- unlock outbound packets buffer
  m_message_send_sync.Unlock();
  //--- store last heartbeat sending time
  m_heartbeat_time=_time64(NULL);
  //---
  return(res);
  }
//+------------------------------------------------------------------+
//| Check context                                                    |
//+------------------------------------------------------------------+
bool CIntegralContext::Check(const FIX::SessionID &session_id,const INT64 curtime)
  {
  bool res=true;
  //--- send heartbeat
  res=SendHeartBeat(session_id,curtime);
  //--- check time of context activity
  res=res&&curtime<m_thread_checktime;
  //--- successful
  return(res);
  }
//+------------------------------------------------------------------+
//| Check context                                                    |
//+------------------------------------------------------------------+
bool CIntegralContext::Check(const INT64 curtime)
  {
  bool res=true;
  //--- send heartbeat
  //res=SendHeartBeat(curtime);
  //--- check time of context activity
  //res=res&&curtime<m_thread_checktime;
  //--- successful
  return(res);
  }
//+------------------------------------------------------------------+
//| Get current sessions                                             |
//+------------------------------------------------------------------+
void CIntegralContext::GetCurrentSessions(std::map<std::string,IntegralSession> &sessions)
  {
  sessions.clear();
  //---
  //for(std::map<std::string,IntegralSession>::const_iterator session=m_sessions.begin();session!=m_sessions.end();++session)
  //  {
  //  if(session->second.session_type!=IntegralSession::EnSessionType::Order) continue;
  //  //---
  //  sessions.insert(*session);
  //  }
  }
//+------------------------------------------------------------------+
//| Get order session by shortname                                   |
//+------------------------------------------------------------------+
bool CIntegralContext::GetOrderSession(LPCSTR short_name,IntegralSession &order_session)
  {
  bool found=false;
  //--- checks
  if(!short_name)
    return(false);
  //--- 
  //for(std::map<std::string,IntegralSession>::const_iterator session=m_sessions.begin();session!=m_sessions.end();++session)
  //  {
  //  if(session->second.session_type!=IntegralSession::EnSessionType::Order||
  //     StringType::Compare(session->second.short_name,short_name)!=0) continue;
  //  //---
  //  order_session=session->second;
  //  found=true;
  //  }
  //---
  return(found);
  }
//+------------------------------------------------------------------+
//| Process received transactions                                    |
//+------------------------------------------------------------------+
bool CIntegralContext::TransApply(CSettingsDispatcher &records_base,bool &trans_applied)
  {
  bool res=false;
  //--- results of processing transactions
  bool symbols_applied=false,ticks_applied=false,books_applied=false,orders_applied=false,deals_applied=false;
  //--- clear arrays
  m_trans_books_tmp.clear();
  m_trans_executions_tmp.clear();
  //m_trans_ticks_tmp.Clear();
  //--- lock
  m_trans_sync.Lock();
  try
    {
    //--- get contents of arrays
    m_trans_books.swap(m_trans_books_tmp);
    m_trans_executions.swap(m_trans_executions_tmp);
    //m_trans_ticks.Swap(m_trans_ticks_tmp);
    }
  catch(const std::exception &e)
    {
    LOG_ERROR(pluginLog,"swap error [ %s ]",e.what());
    }
  //--- unlock
  m_trans_sync.Unlock();
  //--- process received transactions
  res=TransApplyBooks(records_base,m_trans_books_tmp,books_applied);
  //LOG_DEBUG(pluginLog,"TransApplyBooks %s",res?"true":"false");
  res=TransApplyDeals(records_base,m_trans_executions_tmp,deals_applied);
  //LOG_DEBUG(pluginLog,"TransApplyDeals %s <%d>",res?"true":"false",m_trans_executions_tmp.size());
  //--- flag of data availability
  trans_applied=symbols_applied||ticks_applied||books_applied||orders_applied||deals_applied;
  //--- return result
  return(res);
  }
//+------------------------------------------------------------------+
//| Process tick transactions                                        |
//+------------------------------------------------------------------+
//bool CIntegralContext::TransApplyTicks(CSettingsDispatcher &records_base,IntegralTicksArray &trans_ticks,bool &trans_applied)
//  {
//  bool res=true;
//  //--- iterate over all tick transactions
//  for(UINT i=0;i<trans_ticks.Total();i++)
//    {
//    //--- process transactions
//    if(records_base.OnSymbolTickApply(trans_ticks[i]))
//      {
//      //--- transaction accepted
//      trans_applied=true;
//      }
//    else
//      {
//      //--- notify of error
//      ExtLogger.Out(CmdErr,"failed to apply tick transaction (symbol %s)",trans_ticks[i].symbol);
//      //res=res&&false;
//      //break;
//      }
//    }
//  //--- return result
//  return(res);
//  }
//+------------------------------------------------------------------+
//| Process tick transactions                                        |
//+------------------------------------------------------------------+
bool CIntegralContext::TransApplyBooks(CSettingsDispatcher &records_base,IntegralBooksArray &trans_books,bool &trans_applied)
  {
  bool res=true;
  //--- iterate over all tick transactions
  for(UINT i=0;i<trans_books.size();i++)
    {
    //--- process transactions
    if(records_base.OnSymbolBookApply(trans_books[i]))
      {
      //--- transaction accepted
      trans_applied=true;
      }
    else
      {
      //--- notify of error
      LOG_DEBUG(pluginLog,"'%s' failed to apply ticks %0.5lf/%0.5lf %0.5lf/%0.5lf",
                trans_books[i].book.ccy_pair,
                trans_books[i].book.items[0].price,
                trans_books[i].book.items[0].size,
                trans_books[i].book.items[1].price,
                trans_books[i].book.items[1].size);
      //res=false;
      }
    }
  //--- return result
  return(res);
  }
//+------------------------------------------------------------------+
//| Process execution report transactions                            |
//+------------------------------------------------------------------+
bool CIntegralContext::TransApplyDeals(CSettingsDispatcher &records_base,IntegralExecutionsArray &trans_deals,bool &trans_applied)
  {
  FIX::ClOrdID fix_unique_id;
  bool         res=true;
  //----
  for(UINT i=0;i<trans_deals.size();i++)
    {
    //--- process transactions
    if(records_base.OnSymbolDealApply(trans_deals[i]))
      {
      //--- transaction accepted
      trans_applied=true;
      }
    }
  //--- return result
  return(res);
  }
//+------------------------------------------------------------------+
//| testing purposes                                                 |
//+------------------------------------------------------------------+
//bool CIntegralContext::OnTickReceived(const SymbolTick &tick)
//  {
//  bool res=true;
//  m_trans_sync.Lock();
//  if(!(res=m_trans_ticks.Add(&tick)))
//    {
//    LOG_ERROR(pluginLog,"%s tick can't be added to process list[]",tick.symbol);
//    }
//  m_trans_sync.Unlock();
//  //---
//  return(res);
//  }
//+------------------------------------------------------------------+
//| testing purposes                                                 |
//+------------------------------------------------------------------+
//bool CIntegralContext::OnExecutionReceived(const IntegralExecution &execution)
//  {
//  bool res=false;
//  //---
//  m_trans_sync.Lock();
//  try
//    {
//    m_trans_executions.push_back(execution);
//    LOG_INFO(pluginLog,"OnExecutionReceived => %s [size: %d]",execution.fix_unique_id,m_trans_executions.size());
//    res=true;
//    }
//  catch(const std::exception &e)
//    {
//    LOG_ERROR(pluginLog,"%s tick can't be added to process list[ %s ]",execution.short_name,e.what());
//    }
//  m_trans_sync.Unlock();
//  //---
//  return(res);
//  }
//+------------------------------------------------------------------+
//| send message to session                                          |
//+------------------------------------------------------------------+
bool CIntegralContext::SendOrder(IntegralOrder &order)
  {
  IntegralMsgOrder msg={};
  bool             res=false;
  //--- order data
  msg.order=order;
  msg.header={};
  //---
  try
    {
    //---
    //--- checks
    //---
    //--- check shortname
    if(!m_settings.get(order.session_id).has("ShortName"))
      return(false);
    //--- check sender sub id
    if(!FixSessionSettings.GetString(order.session_id,"SenderSubID",msg.header.sender_sub_id))
      return(false);
    //--- check deliver to comp id
    if(!FixSessionSettings.GetString(order.session_id,"DeliverToCompID",msg.header.deliver_to_comp_id))
      return(false);
    //--- check user
    if(!m_settings.get(order.session_id).has("User"))
      return(false);
    //--- paper trading
    //if(m_settings.get(session.session_id).has("PaperTrading"))
    //  msg.paper_trading=m_settings.get(session.session_id).getBool("PaperTrading");
    //--- stop as market
    if(m_settings.get(order.session_id).has("SendStopsAsMarket"))
      msg.stops_as_market=m_settings.get(order.session_id).getBool("SendStopsAsMarket");
    //--- limit as market
    if(m_settings.get(order.session_id).has("SendLimitsAsMarket"))
      msg.limits_as_market=m_settings.get(order.session_id).getBool("SendLimitsAsMarket");
    //---
    StringType::Copy(msg.user,m_settings.get(order.session_id).getString("User").data());
    //---
    //msg.header.sender_sub_id     =FIX::SenderSubID(m_settings.get(session_ptr.session_id).getString("SenderSubID"));
    //msg.header.deliver_to_comp_id=FIX::DeliverToCompID(m_settings.get(session_ptr.session_id).getString("DeliverToCompID"));
    msg.header.sender_comp_id=FIX::SenderCompID(order.session_id.getSenderCompID());
    msg.header.target_comp_id=FIX::TargetCompID(order.session_id.getTargetCompID());
    //--- admin info
    if(ExtPluginContext.AdminIntegration()&&
       m_settings.get(order.session_id).has("AdminHost")&&
       m_settings.get(order.session_id).has("AdminPath")&&
       m_settings.get(order.session_id).has("AdminPort")&&
       m_settings.get(order.session_id).has("AdminStream"))
      {
      std::vector<std::string> orgArgs;
      SPluginHelpers::split(msg.header.sender_comp_id,".",orgArgs);
      msg.header.on_behalf_of_sub_id =FIX::OnBehalfOfSubID(std::to_string(order.login));
      msg.header.on_behalf_of_comp_id=FIX::OnBehalfOfCompID(m_settings.get(order.session_id).getString("User")+std::string("@")+orgArgs[1]);
      //---
      msg.is_admin=true;
      }
    //---
    StringType256 on_behalf;
    on_behalf.Assign(order.on_behalf);
    if(!on_behalf.Empty()&&on_behalf.FindChar('@')>-1)
      {
      std::vector<std::string> orgArgs;
      SPluginHelpers::split(std::string(on_behalf.Str()),"@",orgArgs);
      //---
      if(!msg.is_admin)
        msg.header.on_behalf_of_sub_id=FIX::OnBehalfOfSubID(orgArgs[0]);
      //---
      msg.header.on_behalf_of_comp_id=FIX::OnBehalfOfCompID(orgArgs[1]);
      }
    //--- set session id
    msg.session_id=order.session_id;
    //msg.short_name
    //--- form order for sending
    if((res=IntegralMsgOrder::Write(msg,*this)))
      {
      //--- copy order message
      StringType::Copy(order.order_str,msg.order.order_str);
      }
    //--- store last heartbeat sending time
    m_heartbeat_time=_time64(NULL);
    //---
    return(res);
    }
  catch(const std::exception &e)
    {
    LOG_ERROR(pluginLog,"%s",e.what());
    }
  //--- oops!!!!
  return(false);
  }
//+------------------------------------------------------------------+
//| Send acknowledgement message                                     |
//+------------------------------------------------------------------+
bool CIntegralContext::SendAck(LPCSTR unique_id)
  {
  Ack                    fix_ack;
  //---
  FIX::OrderID           fix_order_id;
  FIX::ExecID            fix_exec_id;
  FIX::Side              fix_side;
  FIX::LeavesQty         fix_leaves_qty;
  FIX::CumQty            fix_cum_qty;
  FIX::AvgPx             fix_avx_px;
  bool                   res=false;
  //--- checks
  if(!unique_id)
    return(false);
  //--- get ack 
  if(!Ack::Fill(std::string(unique_id),fix_ack))
    //if(!m_executions.Get(unique_id,fix_ack))
    return(false);
  //---
  FIX43::ExecutionReport message=fix_ack.AsER();
  //---
  message.getFieldIfSet(fix_order_id);
  message.getFieldIfSet(fix_exec_id);
  message.getFieldIfSet(fix_side);
  message.getFieldIfSet(fix_leaves_qty);
  message.getFieldIfSet(fix_cum_qty);
  message.getFieldIfSet(fix_avx_px);
  //---
  if(!(res=IntegralMsgExecution::Write(message,fix_ack.session_id,*this)))
    {
    LOG_CRITICAL(pluginLog,"Ack: %s",message.toString().c_str());
    }
  /*else
    if(!m_executions.Remove(unique_id))
      {
      LOG_INFO(pluginLog,"failed to remove Ack: %s",message.toString().c_str());
      }*/
      //--
  return(res);
  }
//+------------------------------------------------------------------+
//| Send Dont know trade                                             |
//+------------------------------------------------------------------+
bool CIntegralContext::SendDontKnowTrade(LPCSTR order_str,const UINT reason,LPCSTR text)
  {
  IntegralMsgDontKnowTrade msg;
  FIX43::DontKnowTrade     dkt;
  double                   volume=0;
  FIX::SessionID           session_id;
  FIX::OnBehalfOfCompID    _OnBehalfOfCompID;
  FIX::OnBehalfOfSubID     _OnBehalfOfSubID;
  FIX::ClOrdID             clord_id;
  FIX::AccountId           _accountId;
  StringType2048           str;
  //--- checks
  if(!order_str||!text)
    return(false);
  //---
  //if(!ExtWatchTower.QueryDontKnowTrade(unique_id,reason,dkt,text,volume))
  //  return(false);
  str.Assign(order_str);
  if(!TradeRequest::Fill(str,reason,text,dkt))
    return(false);
  //---
  session_id=dkt.getSessionID();
  //---
  dkt.getHeader().set(FIX::SenderSubID(m_settings.get(session_id).getString("SenderSubID")));
  bool isAdminIntegration=(ExtPluginContext.AdminIntegration()&&
                           m_settings.get(session_id).has("AdminHost")&&
                           m_settings.get(session_id).has("AdminPath")&&
                           m_settings.get(session_id).has("AdminPort")&&
                           m_settings.get(session_id).has("AdminStream"));
  //---
  if(dkt.getHeader().isSet(_OnBehalfOfCompID))
    {
    //--- nothing todo
    }
  else if(isAdminIntegration)
    {
    std::vector<std::string> orgArgs;
    SPluginHelpers::split(m_settings.get(session_id).getString("SenderCompID"),".",orgArgs);

    if(orgArgs.size()>=2)
      dkt.getHeader().set(FIX::OnBehalfOfCompID(m_settings.get(session_id).getString("User")+std::string("@")+orgArgs[1]));
    }
  //---
  if(dkt.getHeader().isSet(_OnBehalfOfSubID))
    {
    //--- nothing todo
    }
  else if(isAdminIntegration)
    {
    dkt.getFieldIfSet(clord_id);
    //--- set login
    int login;
    if(StringType::ReadParamInt(clord_id.getString().c_str(),"login:",&login))
      {
      StringType64 login_str;
      login_str.Format("%d",login);
      dkt.getHeader().set(FIX::OnBehalfOfSubID(login_str.toString()));
      }
    }
  //---
  msg.session_id=session_id;
  msg.trade=dkt;
  //msg.code=
  if(!IntegralMsgDontKnowTrade::Write(msg,*this))
    return(false);
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| Send order status request                                        |
//+------------------------------------------------------------------+
bool CIntegralContext::SendOrderStatusRequest(LPCSTR order_str)
  {
  FIX43::OrderStatusRequest     request;
  IntegralMsgOrderStatusRequest message;
  bool res;
  StringType2048 str;
  //--- checks
  if(!order_str)
    return(false);
  //---
  str.Assign(order_str);
  if(!TradeRequest::Fill(str,request))
    return(false);

  //if(!ExtWatchTower.QueryStatus(unique_id,request))
  //  return(false);
  //---
  message.session_id=request.getSessionID();
  message.request=request;
  //---
  if(!(res=IntegralMsgOrderStatusRequest::Write(message,*this)))
    {
    LOG_CRITICAL(pluginLog,"OSR: %s",request.toString().c_str());
    }
  //---
  return(res);
  }
//+------------------------------------------------------------------+
//| Remove order status request                                      |
//+------------------------------------------------------------------+
//bool CIntegralContext::RemoveOrder(LPCSTR unique_id)
//  {
//  //--- checks
//  if(!unique_id)
//    return(false);
//  //---
//  if(!ExtWatchTower.RemoveOrder(unique_id))
//    return(false);
//  //---
//  return(true);
//  }
//+------------------------------------------------------------------+
//| Send HeartBeat                                                   |
//+------------------------------------------------------------------+
bool CIntegralContext::SendHeartBeat(const FIX::SessionID &session_id,const INT64 curtime)
  {
  bool                 res=true;
  FIX43::Heartbeat heartbeat;
  //--- check heartbeat time
  if(m_heartbeat_time+HEARBEAT_TIMEOUT>curtime)
    return(true);
  //--- protocol version
  heartbeat=FIX43::Heartbeat();
  //set session
  heartbeat.setSessionID(session_id);
  //--- form buffer for sending
  res=Send(heartbeat);
  //res=ExchangeMsgHeartBeat::Write(msg,*this);
//--- write to log in case of error
  if(!res)
    LOG_ERROR(pluginLog,"failed to send heartbeat");
  //--- return result
  return(res);
  }
//+------------------------------------------------------------------+
//| Send HeartBeat                                                   |
//+------------------------------------------------------------------+
bool CIntegralContext::SendHeartBeat(const INT64 curtime)
  {
  bool          res=true;
  FIX::Session *session;
  //--- checks
  if(!m_initiator)
    return(false);
  //--- check heartbeat time
  if(m_heartbeat_time+HEARBEAT_TIMEOUT>curtime||m_initiator->isStopped())
    return(true);
  //--- check sessions
  LOG_INFO(pluginLog,"checking isLoggedOn()");
  if((res=m_initiator->isLoggedOn()))
    {
    LOG_INFO(pluginLog,"context isLoggedOn");
    //--- speed up shutdown
    //UpdateTime((INT64)0);
    //---
    return(true);
    }
  //--- check sessions
  for(auto it:FIX::Session::getSessions())
    {
    if((session=FIX::Session::lookupSession(it))==NULL)
      return(false);
    if(!session->isLoggedOn())
      {
      session->logout("not connected");
      //--- logon
      session->logon();
      }
    }
  //--- return result
  return(true);
  }
//+------------------------------------------------------------------+
//| Check context                                                    |
//+------------------------------------------------------------------+
//void CIntegralContext::startFIX()
//  {
//  bool res;
//  //if(ExtBridgeConfig.EnableUnifeeder(&res)&&uniFeeder==NULL)
//  //  {
//  //  LOG_INFO(pluginLog,"Initialize UniFeeder");
//  //  if((uniFeeder=new(std::nothrow) IUniserverFeeder("MT4"))!=NULL)
//  //    {
//  //    uniFeeder->Start();
//  //    }
//  //  //---
//  //  //this->ReadRules(true);
//  //  this->SaveConfigs();
//  //  }
//
//  if(m_initiator->isStopped())
//    {
//    StateSet(STATE_STOPPED);
//    LOG_INFO(pluginLog,"FIX engine stopped, now starting");
//    m_initiator->start();
//
//    stopThread.store(false,std::memory_order_release);
//    fileMonitor=std::thread(&CIntegralContext::WatchDirectory,this);
//    healthThread=std::thread(&CIntegralContext::EnsureConnected,this);
//    }
//  }

//void CIntegralContext::stopFIX()
//  {
//  if(m_initiator==NULL)
//    return;
//
//  if(m_initiator->isStopped())
//    return;
//
//  LOG_INFO(pluginLog,"FIX engine still running, now stopping");
//
//  try
//    {
//    stopThread.store(true,std::memory_order_release);
//
//    FindCloseChangeNotification(this->dwChangeHandles);
//    CancelIoEx(this->hDir,NULL);
//    CloseHandle(this->hDir);
//
//    fileMonitor.join();
//    healthThread.join();
//    }
//  catch(std::exception&e)
//    {
//    LOG_ERROR(pluginLog,"std::exception {%s}",e.what());
//    }
//
//  if(!m_initiator->isStopped())
//    m_initiator->stop(false);
//
//  StateSet(STATE_STOPPED);
//
//  //delete this->m_log_factory;
//  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
//void CIntegralContext::ReConnect()
//  {
//  this->stopFIX();
//  //this->startFIX();
//  }

//void CIntegralContext::EnsureConnected()
//  {
//  DebugUtil::CUnhandledExceptionFilter filter;
//  DebugUtil::CTransparentSEHTraslator  translator;
//
//  //m_log_factory->StartLogQueues();
//  Sleep(1000); // Wait before starting
//
//  while(!stopThread.load(std::memory_order_acquire))
//    {
//    if(m_initiator==NULL)
//      return;
//
//    if(m_initiator->isStopped())
//      return;
//
//    ConTime ct={0};
//    m_api_server->TimeGet(&ct);
//
//    const time_t rawtime=m_api_server->TradeTime();
//    const time_t utime=time(NULL);
//    struct tm *timeinfo;
//
//    timeinfo=gmtime(&rawtime);
//
//    bool wasntConnected=false;
//    for(auto it:FIX::Session::getSessions())
//      {
//      if(m_sessions[it.toString()]==false)
//        {
//        // Check if Administrator / Administration / Time is enabled
//        if(ct.days[timeinfo->tm_wday][timeinfo->tm_hour]==TRUE)
//          {
//          //tcping.exe -t -4 -b 1 -j -f -s localhost 2523
//          if(m_settings.get(it).has("SocketConnectHost")&&m_settings.get(it).has("SocketConnectPort"))
//            NetworkMon(it.toString(),m_settings.get(it).getString("SocketConnectHost"),m_settings.get(it).getInt("SocketConnectPort"));
//
//          for(int i=1; i<=5; i++)
//            if(m_settings.get(it).has("SocketConnectHost"+i)&&m_settings.get(it).has("SocketConnectPort"+i))
//              NetworkMon(it.toString(),m_settings.get(it).getString("SocketConnectHost"+i),m_settings.get(it).getInt("SocketConnectPort"+i));
//
//          LOG_ERROR(pluginLog,"Session '%s' not connected, now restarting",it.toString().c_str());
//          FIX::Session::lookupSession(it)->logout("Not Connected");
//          FIX::Session::lookupSession(it)->logon();
//
//          LOG_ERROR(pluginLog,"Session '%s' is now restarted",it.toString().c_str());
//
//          wasntConnected=true;
//          }
//        }
//      }
//
//    //bool res;
//    /* if(ExtBridgeConfig.EnableUnifeeder(&res)&&uniFeeder==NULL)
//       {
//       LOG_INFO(pluginLog,"Initialize UniFeeder");
//       uniFeeder=new IUniserverFeeder("MT4");
//       }*/
//
//       //if(ExtBridgeConfig.EnableUnifeeder(&res)&&uniFeeder!=NULL&&!uniFeeder->isConnected())
//       //  {
//       //  // Check if Administrator / Administration / Time is enabled
//       //  if(ct.days[timeinfo->tm_wday][timeinfo->tm_hour]==TRUE)
//       //    {
//       //    LOG_INFO(pluginLog,"Restarting Unifeeder!");
//       //    uniFeeder->Start();
//       //    LOG_INFO(pluginLog,"Restarted Unifeeder!");
//       //    }
//       //  }
//
//    if(wasntConnected)
//      Sleep(15000);
//    else
//      Sleep(1000);
//    }
//  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CIntegralContext::onCreate(const FIX::SessionID &session_id)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;
  //---
  StringTypePath         file;
  std::string            short_name,sender_comp_id;
  UINT                   session_type;
  StringType256          message;
  //--- checks
  if(!CheckParameters(session_id,short_name,sender_comp_id,session_type,false))
    {
    return;
    }
  //---
  if((m_logs[session_id.toString()]=new(std::nothrow) CLog())==NULL)
    {
    LOG_ERROR(pluginLog,"log create failed Session:<%s>",session_id.toString());
    //--- inform create error
    //m_integral_api.OnCreate(short_name.c_str(),false);
    return;
    }
  //--- start logging
  file.Format("IDC.FIX.%s",sender_comp_id.c_str());
  bool res=m_logs[session_id.toString()]->Initialize(file.Str());
  //--- show
  LOG_INFO(m_logs[session_id.toString()],"------------------------- Starting FIX (%s) Logs -------------------------",
           session_id.toString());
  //---
  LOG_INFO(pluginLog,"FIX::OnCreate(%s)",session_id.toString());
  LOG_INFO(m_logs[session_id.toString()],session_id.toString());

  if(!m_integral_api.OnCreate(session_id,short_name.c_str(),session_type))
    {
    LOG_ERROR(pluginLog,"OnCreate session notify failed <%s>",
              session_id.toString().c_str());
    }
  }
//+------------------------------------------------------------------+
//| Logon message received                                           |
//+------------------------------------------------------------------+
void CIntegralContext::onLogon(const FIX::SessionID &session_id)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;
  //---
  //OrderStatusRequests    requests;
  CIntegralSessionRecord record;
  InstrumentRecordsArray instruments;
  std::string            short_name,sender_comp_id;
  UINT                   session_type;
  StringType256          message;
  //--- checks
  if(!CheckParameters(session_id,short_name,sender_comp_id,session_type))
    return;
  //--- checks
  if(m_logs.empty())
    {
    message.Format("logging not defined <%s>",session_id.toString().c_str());
    //--- logout
    SendLogout(session_id,message.Str());
    //--- retry later
    return;
    }
  //---
  bool isFirstLogin=m_logs[session_id.toString()]->StartQueue();
  //--- show
  LOG_INFO(pluginLog,"FIX::onLogon(%s)",session_id.toString());
  LOG_INFO(m_logs[session_id.toString()],session_id.toString());

  //watchTower->QueryForSession(session_id,isFirstLogin);
  //watchTower->Start();
  m_integral_api.OnLogon(session_id,session_type,short_name.c_str());
  }
//+------------------------------------------------------------------+
//| Logout message received                                          |
//+------------------------------------------------------------------+
void CIntegralContext::onLogout(const FIX::SessionID &session_id)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;
  //---
  UINT        session_type;
  std::string short_name,sender_comp_id;
  //--- checks
  if(!CheckParameters(session_id,short_name,sender_comp_id,session_type))
    return;
  //--- show
  LOG_INFO(pluginLog,"FIX::onLogout(%s)",session_id.toString().c_str());
  LOG_INFO(m_logs[session_id.toString()],session_id.toString().c_str());
  //--- notify logout
  m_integral_api.OnLogout(session_id,session_type,short_name.c_str());
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CIntegralContext::toAdmin(FIX::Message &message,const FIX::SessionID &session_id)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;
  //---
  FIX::MsgType           msg_type;
  CIntegralSessionRecord record={};
  //--- checks
  if(!message.getHeader().getFieldIfSet(msg_type))
    {
    return;
    }
  //---
  if(msg_type==FIX::MsgType_Logon)
    {
    LOG_INFO(pluginLog,"Set User/Pass/Version in Logon msg");
    //--- checks
    if(!(m_settings.get(session_id).has("User")||
         m_settings.get(session_id).has("Pass")||
         m_settings.get(session_id).has("SenderSubID")))
      {
      return;
      }
    //---
    message.setField(FIX::Username(m_settings.get(session_id).getString("User")));
    message.setField(FIX::Password(m_settings.get(session_id).getString("Pass")));
    message.setField(FIX::BridgeVersion(std::string("MT ")+std::string(VERSION_STRING)));
    message.getHeader().setField(FIX::SenderSubID(m_settings.get(session_id).getString("SenderSubID")));
    //---
    const std::string tmp=ExtCloudWatch.to_json(true);
    if(!tmp.empty())
      {
      message.getHeader().setField(FIX::SecureData(tmp));
      message.getHeader().setField(FIX::SecureDataLen(tmp.length()));
      }
    }
  else if(msg_type==FIX::MsgType_Heartbeat)
    {
    const std::string tmp=ExtCloudWatch.to_json(false);
    if(!tmp.empty())
      {
      message.getHeader().setField(FIX::SecureData(tmp));
      message.getHeader().setField(FIX::SecureDataLen(tmp.length()));
      }
    }
  //--- show
  LOG_INFO(m_logs[session_id.toString()],message.toString());
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CIntegralContext::toApp(FIX::Message &message,const FIX::SessionID &session_id)
throw(FIX::DoNotSend)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;

  //--- update time before starting processing thread
  //UpdateTime((INT64)THREAD_TIMEOUT);

  LOG_INFO(m_logs[session_id.toString()],message.toString().c_str());
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CIntegralContext::fromAdmin(const FIX::Message &message,const FIX::SessionID &session_id)
throw(FIX::FieldNotFound,FIX::IncorrectDataFormat,FIX::IncorrectTagValue,FIX::RejectLogon)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;

  LOG_INFO(m_logs[session_id.toString()],message.toString().c_str());

  try
    {
    //---
    crack(message,session_id);
    //--- update time before starting processing thread
    //UpdateTime((INT64)THREAD_TIMEOUT);
    }
  catch(const std::exception &e)
    {
    LOG_CRITICAL(pluginLog,"%s Message:<%s>",e.what(),message.toString());
    }
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CIntegralContext::fromApp(const FIX::Message &message,const FIX::SessionID &session_id)
throw(FIX::FieldNotFound,FIX::IncorrectDataFormat,FIX::IncorrectTagValue,FIX::UnsupportedMessageType)
  {
  FIX::MsgType msg_type;
  //---
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;
  //--- checks
  if(!message.getHeader().getFieldIfSet(msg_type))
    {
    LOG_ERROR(pluginLog,"message type NOT set <%s>",message.toString());
    return;
    }
  //--- check      (BugFix: only market data will be save)
  if(msg_type!=FIX::MsgType_MarketDataSnapshotFullRefresh||
     ExtPluginContext.SavePriceUpdates())
    {
    LOG_INFO(m_logs[session_id.toString()],message.toString());
    }
  //--- crack
  try
    {
    //---
    crack(message,session_id);
    //--- update time before starting processing thread
    //UpdateTime((INT64)THREAD_TIMEOUT);
    }
  catch(const std::exception &e)
    {
    LOG_CRITICAL(pluginLog,"%s Message:<%s>",e.what(),message.toString());
    }
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+  
bool CIntegralContext::Subscribe(const Instrument *instrument,const CIntegralSessionRecord &record)
  {
  FIX::MarketDepth market_depth;
  StringType128    temp;
  //--- checks
  if(!instrument||!instrument->enabled)
    {
    LOG_INFO(pluginLog,"Instrument '%s' is not enabled for pricing",instrument->ccy_pair);
    //---
    return(false);
    }
  //--- set marketdat request id
  temp.Format("MT4-%s-%s",instrument->short_name,instrument->ccy_pair);
  //---
  FIX::MDReqID					_MDReqID=FIX::MDReqID(temp.Str());
  FIX::SubscriptionRequestType	_SubscriptionRequestType=FIX::SubscriptionRequestType(FIX::SubscriptionRequestType_SNAPSHOT_PLUS_UPDATES);
  //---
  try
    {
    bool m_depth_exist=m_settings.get(record.SessionId()).has("MarketDepth");
    if(!m_depth_exist)
      return(false);

    std::string depth_str=m_settings.get(record.SessionId()).getString("MarketDepth");
    int depth=m_settings.get(record.SessionId()).getInt("MarketDepth");
    market_depth=FIX::MarketDepth((FIX::INT)depth);
    }
  catch(const std::exception &e)
    {
    LOG_ERROR(pluginLog,"%s",e.what());
    return(false);
    }

  // Limit the pricing tiers to 5, no matter what
  if(market_depth.getValue()<1||market_depth.getValue()>5)
    market_depth.setValue(5);

  FIX43::MarketDataRequest mdr(_MDReqID,_SubscriptionRequestType,market_depth);
  FIX43::MarketDataRequest::NoRelatedSym nrs;

  mdr.getHeader().set(FIX::SenderSubID(m_settings.get(record.SessionId()).getString("SenderSubID")));
  mdr.getHeader().set(FIX::DeliverToCompID(m_settings.get(record.SessionId()).getString("DeliverToCompID")));

  nrs.set(FIX::Symbol(instrument->ccy_pair));
  nrs.set(FIX::Product(FIX::Product_CURRENCY));
  mdr.addGroup(nrs);

  mdr.set(FIX::MDUpdateType(FIX::MDUpdateType_FULL_REFRESH));
  mdr.set(FIX::AggregatedBook(FIX::AggregatedBook_YES));

  FIX43::MarketDataRequest::NoMDEntryTypes nmt;
  nmt.set(FIX::MDEntryType(FIX::MDEntryType_BID));
  mdr.addGroup(nmt);
  nmt.set(FIX::MDEntryType(FIX::MDEntryType_OFFER));
  mdr.addGroup(nmt);
  //--- no more checks
  /*if(StringType::Len(instrument->ccy_pair)!=7)
    {
    LOG_ERROR(pluginLog,"Ignoring due to invalid configuration: %s=%s",instrument->symbol,instrument->ccy_pair);
    return(false);
    }*/

  std::string oCCY=instrument->base_cry;
  oCCY.append(instrument->quote_cry);
  /*if(StringType::Len(instrument->symbol)>=6&&StringType::Compare(instrument->symbol,oCCY.c_str(),6)!=0)
    {
    LOG_ERROR(pluginLog,"Inconsistent instrument configuration mapping: %s=%s",instrument->symbol,instrument->ccy_pair);
    return(false);
    }*/

  LOG_INFO(pluginLog,"MarketDataRequest: %s - %s",instrument->ccy_pair,mdr.toString().c_str());
  //---
  return(FIX::Session::sendToTarget(mdr,record.SessionId()));
  }
//+------------------------------------------------------------------+
//| start connection                                                 |
//+------------------------------------------------------------------+
bool CIntegralContext::Start(void)
  {
  //--- checks
  if(!m_initiator)
    return(false);
  //--- already logged on
  if(StateGet()<STATE_STARTED)
    return(false);
  //---
  try
    {
    m_initiator->start();
    StateSet(STATE_STARTED);
    //--- successfull
    return(true);
    }
  catch(const std::exception &e)
    {
    LOG_ERROR(pluginLog,"start failed [%s]",e.what());
    }
  //--- smth went wrong
  return(false);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CIntegralContext::CheckParameters(const FIX::SessionID &session_id,std::string &short_name,std::string &sender_comp_id,UINT &session_type,const bool send_logout)
  {
  StringType256 message;
  //--- checks
  if(!m_settings.get(session_id).has("ShortName")||
     !m_settings.get(session_id).has("SenderCompID"))
    {
    message.Format("session crediantials not found Session<%s>",session_id.toString());
    LOG_ERROR(pluginLog,message.Str());
    //--- oops!
    if(send_logout)
      SendLogout(session_id,message.Str());
    //---
    return(false);
    }
  //--- set values
  short_name=m_settings.get(session_id).getString("ShortName");
  sender_comp_id=m_settings.get(session_id).getString("SenderCompID");
  if(short_name.empty()||sender_comp_id.empty())
    {
    message.Format("session crediantials are empty Session<%s>",session_id.toString());
    LOG_ERROR(pluginLog,message.Str());
    //--- oops!
    if(send_logout)
      SendLogout(session_id,message.Str());
    //---
    return(false);
    }
  //--- get session type
  //--- session type
  if(sender_comp_id.find("quote.")!=std::string::npos)
    {
    //--- set session type
    session_type=CIntegralSessionRecord::Quote;
    }
  else if(sender_comp_id.find("order.")!=std::string::npos)
    {
    //--- set session type
    session_type=CIntegralSessionRecord::Order;
    }
  else
    {
    message.Format("session crediantials are not valid Session<%s>",session_id.toString());
    LOG_ERROR(pluginLog,message.Str());
    //--- oops!
    if(send_logout)
      SendLogout(session_id,message.Str());
    //---
    return(false);
    }
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void  CIntegralContext::SendLogout(const FIX::SessionID &session_id,LPCSTR message)
  {
  FIX::Session *session;
  //---
  if((session=FIX::Session::lookupSession(session_id))!=NULL)
    session->logout(message);
  //---
  LOG_INFO(pluginLog,message);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void  CIntegralContext::SendLogon(const FIX::SessionID &session_id)
  {
  FIX::Session *session;
  //---
  if((session=FIX::Session::lookupSession(session_id))!=NULL)
    session->logon();
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
//bool CIntegralContext::SaveSettings()
//  {
//  StringTypePath file;
//  StringType256  map_kep;
//  std::ofstream fixFile;
//  //---
//  static std::map<std::string,std::string> WordMap=
//    {
//      {"ADMINSTREAM","AdminStream"},
//      {"ADMINHOST","AdminHost"},
//      {"ADMINPORT","AdminPort"},
//      {"ADMINPATH","AdminPath"},
//      {"BEGINSTRING","BeginString"},
//      {"BGROUPS","BGroups"},
//      {"CHECKLATENCY","CheckLatency"},
//      {"CONNECTIONTYPE","ConnectionType"},
//      {"DATADICTIONARY","DataDictionary"},
//      {"DELIVERTOCOMPID","DeliverToCompID"},
//      {"ENDDAY","EndDay"},
//      {"ENDTIME","EndTime"},
//      {"FILELOGPATH","FileLogPath"},
//      {"FILESTOREPATH","FileStorePath"},
//      {"HEARTBTINT","HeartBtInt"},
//      {"LOGOUTTIMEOUT","LogoutTimeout"},
//      {"MARKETDEPTH","MarketDepth"},
//      {"PAPERTRADING","PaperTrading"},
//      {"PASS","Pass"},
//      {"PRIMEBROKER","PrimeBroker"},
//      {"PERSISTMESSAGES","PersistMessages"},
//      {"RECONNECTINTERVAL","ReconnectInterval"},
//      {"RESETONLOGON","ResetOnLogon"},
//      {"SENDERCOMPID","SenderCompID"},
//      {"SENDERSUBID","SenderSubID"},
//      {"SENDLIMITSASMARKET","SendLimitsAsMarket"},
//      {"SENDSTOPSASMARKET","SendStopsAsMarket"},
//      {"SHORTNAME","ShortName"},
//      {"SOCKETCONNECTHOST","SocketConnectHost"},
//      {"SOCKETCONNECTPORT","SocketConnectPort"},
//      {"SOCKETCONNECTHOST1","SocketConnectHost1"},
//      {"SOCKETCONNECTPORT1","SocketConnectPort1"},
//      {"SOCKETCONNECTHOST2","SocketConnectHost2"},
//      {"SOCKETCONNECTPORT2","SocketConnectPort2"},
//      {"SOCKETCONNECTHOST3","SocketConnectHost3"},
//      {"SOCKETCONNECTPORT3","SocketConnectPort3"},
//      {"SOCKETCONNECTHOST4","SocketConnectHost4"},
//      {"SOCKETCONNECTPORT4","SocketConnectPort4"},
//      {"SOCKETCONNECTHOST5","SocketConnectHost5"},
//      {"SOCKETCONNECTPORT5","SocketConnectPort5"},
//      {"SOCKETNODELAY","SocketNodelay"},
//      {"STARTDAY","StartDay"},
//      {"STARTTIME","StartTime"},
//      {"TARGETCOMPID","TargetCompID"},
//      {"TIMEZONE","TimeZone"},
//      {"USEDATADICTIONARY","UseDataDictionary"},
//      {"USELOCALTIME","UseLocalTime"},
//      {"USER","User"},
//      {"VALIDATEFIELDSHAVEVALUES","ValidateFieldsHaveValues"},
//      {"LOGOUTTIMEOUT","LogoutTimeout"}
//    };
//
//  //--- set file 
//  file.Format("%s\\%s",ExtProgramPath,"FIX.Settings.config");
//  LOG_INFO(pluginLog,"Save FIX settings: %s",file.Str());
//  //---
//  fixFile.open(file.Str(),std::ios::out|std::ios::trunc);
//  //--- check
//  if(!fixFile.is_open())
//    return(false);
//
//  //const FIX::Dictionary& defaults(m_settings.m_defaults);
//  const SettingsSorter defaults(m_settings.m_defaults.begin(),m_settings.m_defaults.end());
//  //---
//  if(defaults.size())
//    {
//    fixFile<<"[DEFAULT]"<<std::endl;
//    LOG_INFO(pluginLog,"[DEFAULT]");
//
//    FIX::Dictionary::iterator i;
//    for(i=defaults.begin(); i!=defaults.end(); ++i)
//      {
//      fixFile<<(WordMap.find(i->first)!=WordMap.end()?WordMap[i->first]:i->first)<<"="<<i->second<<std::endl;
//      LOG_INFO(pluginLog,"  %s=%s",(WordMap.find(i->first)!=WordMap.end()?WordMap[i->first]:i->first).c_str(),i->second.c_str());
//      }
//
//    fixFile<<std::endl;
//    LOG_INFO(pluginLog,"[END]");
//    }
//
//  std::map<std::string,FIX::SessionID> sortedKeyMap;
//
//  for(auto &item:m_settings.getSessions())
//    {
//    if(!m_settings.get(item).has("ShortName")||
//       !m_settings.get(item).has("User")||
//       !m_settings.get(item).has("SenderCompID"))
//      continue;
//
//    map_kep.Clear();
//    map_kep.Append(m_settings.get(item).getString("ShortName"));
//    map_kep.Append("_");
//    map_kep.Append(item.getSenderCompID().getValue());
//    map_kep.Append("_");
//    map_kep.Append(m_settings.get(item).getString("User"));
//
//    sortedKeyMap[map_kep.toString()]=item;
//    }
//
//  for(auto &item:sortedKeyMap)
//    {
//    fixFile<<"[SESSION]"<<std::endl;
//    LOG_INFO(pluginLog,"[SESSION]");
//
//    //const FIX::Dictionary& section = m_settings.get(item.second);
//    const SettingsSorter section(m_settings.get(item.second).begin(),m_settings.get(item.second).end());
//
//    if(!section.size())
//      continue;
//
//    //FIX::Dictionary::iterator i;
//    //for (i = section.begin(); i != section.end(); ++i)
//    for(auto &i:section)
//      {
//      if(defaults.find(i.first)!=defaults.end()) //if (defaults.has(i->first)) // && defaults.getString(i->first) == i->second
//        continue;
//
//      fixFile<<(WordMap.find(i.first)!=WordMap.end()?WordMap[i.first]:i.first)<<"="<<i.second<<std::endl;
//      LOG_INFO(pluginLog,"  %s=%s",
//               (WordMap.find(i.first)!=WordMap.end()?WordMap[i.first]:i.first).c_str(),i.second.c_str());
//      }
//
//    fixFile<<std::endl;
//    LOG_INFO(pluginLog,"[END]");
//    }
//  //--- close file
//  fixFile.close();
//  //--- successful
//  return(true);
//  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CIntegralContext::onMessage(const FIX43::BusinessMessageReject &message,const FIX::SessionID &session_id)
  {
  LOG_ERROR(pluginLog,"BusinessMessageReject: %s",message.toString().c_str());
  //--- update time before starting processing thread
  //UpdateTime((INT64)THREAD_TIMEOUT);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CIntegralContext::onMessage(const FIX43::Reject &message,const FIX::SessionID &session_id)
  {
  LOG_ERROR(pluginLog,"Reject: %s",message.toString().c_str());
  //--- update time before starting processing thread
  //UpdateTime((INT64)THREAD_TIMEOUT);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CIntegralContext::onMessage(const FIX43::MarketDataRequestReject &message,const FIX::SessionID &session_id)
  {
  LOG_ERROR(pluginLog,"MarketDataRequestReject: %s",message.toString().c_str());
  //--- update time before starting processing thread
  //UpdateTime((INT64)THREAD_TIMEOUT);
  }
//+----------------------------------------------------------------------------------------------------------+
//| Market data snapshot full refresh                                                                        |
//+----------------------------------------------------------------------------------------------------------+
void CIntegralContext::onMessage(const FIX43::MarketDataSnapshotFullRefresh &message,const FIX::SessionID &session_id)
  {
  //IntegralMsgBook   msg_book;
  FIX::UTCTIMESTAMP utc;
  FIX::UTCTIMESTAMP fxi;
  FIX::SendingTime  sending_time;
  FIX::Symbol symbol;
  IntegralMsgBook msg_book={};
  //--- check
  //if(!ExtPluginContext.EnableUnifeeder())
  //  return;
  //--- checks
  if(!m_settings.has(session_id)||!m_settings.get(session_id).has("ShortName"))
    return;
  //--- read snapshot
  std::string short_name=m_settings.get(session_id).getString("ShortName");
  if(!IntegralMsgBook::Read(message,short_name.c_str(),msg_book))
    {
    LOG_ERROR(pluginLog,"error reading market data [%s]",message.toString());
    return;
    }
  //---
  if(msg_book.book.items_total<1)
    return;
  //--- add to list
  m_trans_sync.Lock();

  try
    {
    m_trans_books.push_back(msg_book);
    }
  catch(const std::exception &e)
    {
    LOG_ERROR(pluginLog,"%s tick can't be added for processing { %s }", // [%0.5lf/%0.5lf]
              symbol.getString().c_str(),e.what());
    }
  m_trans_sync.Unlock();
  //--- update time before starting processing thread
  //UpdateTime((INT64)THREAD_TIMEOUT);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CIntegralContext::onMessage(const FIX43::TradeCaptureReport &report,const FIX::SessionID &session_id)
  {
  LOG_INFO(pluginLog,"FIX43::TradeCaptureReport: %s",session_id.toString().c_str());
  //--- update time before starting processing thread
  //UpdateTime((INT64)THREAD_TIMEOUT);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CIntegralContext::onMessage(const FIX43::ExecutionReport &report,const FIX::SessionID &session_id)
  {
  //IntegralMsgExecution execution;
  //---
  FIX::ClOrdID         clord_id;
  FIX::OrdStatus       order_status;
  FIX::CumQty          cum_qty;
  FIX::LeavesQty       leaves_qty;
  FIX::OrdRejReason    order_reject_reason;
  Ack                  ack;
  //IntegralSession      session;
  FIX::SenderCompID    sender_comp_id;
  StringType64         login;
  //---
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;
  //---
  LOG_INFO(pluginLog,"%s ER - %s",session_id.toString().data(),report.toString().data());

  //try
  //  {
    //--- check
  if(!report.getIfSet(clord_id)||
     !report.getIfSet(order_status)||
     !report.getHeader().getIfSet(sender_comp_id))
    {
    LOG_ERROR(pluginLog,"%s ER - %s",session_id.toString().data(),report.toString().data());
    return;
    }
  //--- check
  switch(order_status.getValue())
    {
    case FIX::OrdStatus_PENDING_NEW:
    case FIX::OrdStatus_NEW:
      return;

    case FIX::OrdStatus_FILLED:
    case FIX::OrdStatus_EXPIRED:
    case FIX::OrdStatus_CANCELED:
      StringType::ReadParamStr(clord_id.getString().c_str(),"login:",login.Buffer(),login.Max(),";");
      login.Refresh();
      LOG_INFO(pluginLog,"ExecutionReport: %s - %s",login.Str(),report.toString().c_str());
      //--- 
      report.getIfSet(cum_qty);
      report.getIfSet(leaves_qty);
      ExtCloudWatch.MonitorOrders(cum_qty>0&&leaves_qty>0,cum_qty==0);
      //LOG_DEBUG(pluginLog,"ExtCloudWatch.MonitorOrders(%f>0&&%f>0,cum_qty==0)",cum_qty.getValue(),leaves_qty.getValue());
      //---
      break;

    case FIX::OrdStatus_PARTIALLY_FILLED:
      break;

    case FIX::OrdStatus_REJECTED:
      if(report.getIfSet(order_reject_reason))
        if(order_reject_reason.getValue()==FIX::OrdRejReason_UNKNOWN_ORDER)
          {
          LOG_DEBUG(pluginLog,"Unknown Order received for %s, ignoring until threadTimeout",clord_id.getValue().c_str());
          //--- nothing todo
          return;
          }
      ExtCloudWatch.MonitorOrders(false,true);
      //LOG_DEBUG(pluginLog,"ExtCloudWatch.MonitorOrders(false,true)");
      break;

    default:
      return;
    }
  //--- check
  if(!m_settings.get(session_id).has("ShortName"))
    {
    LOG_INFO(pluginLog,"session_id does not have shortname <%s>",session_id.toString());
    return;
    }
  //--- get shortname
  std::string short_name=m_settings.get(session_id).getString("ShortName");
  //-- fill and add
  if(order_status.getValue()!=FIX::OrdStatus_PARTIALLY_FILLED)
    if(!Ack::Fill(short_name,report,ack))
      {
      LOG_ERROR(pluginLog,"acknowledgement filling failed: %s",report.toString().c_str());
      return;
      }
  //--- add to list
  m_trans_sync.Lock();
  //if(!m_executions.Add(clord_id.getValue().c_str(),ack))
  //  {
  //  //--- TODO: what we do?
  //  LOG_ERROR(pluginLog,"exec report can not be added to list [%s]",clord_id.getValue().c_str());
  //  }
  try
    {
    m_trans_executions.push_back(report);
    }
  catch(const std::exception &e)
    {
    LOG_ERROR(pluginLog,"%s",e.what());
    }
  //---

  m_trans_sync.Unlock();
  /*  }
  catch(const std::exception &e)
    {
    LOG_ERROR(pluginLog,"'%s'adding execution report to transaction list failed [ %s ]",
              clord_id.getValue().c_str(),
              e.what());
    }*/
    //--- update time before starting processing thread
    //UpdateTime((INT64)THREAD_TIMEOUT);
  //LOG_INFO(pluginLog,"%s finalized ER - %s",session_id.toString().data(),report.toString().data());
  }
//+------------------------------------------------------------------+
//| Process HeartBeat message                                        |
//+------------------------------------------------------------------+
void CIntegralContext::onMessage(const FIX43::Heartbeat &m,const FIX::SessionID &s)
  {
  //--- update time before starting processing thread
  //UpdateTime((INT64)THREAD_TIMEOUT);
  }
void CIntegralContext::UpdateSymbolCache(const ConSymbol *cs)
  {
  //Symbols[cs->symbol]=cs->spread;
  }
//+------------------------------------------------------------------+
//| Admin intergation                                                |
//+------------------------------------------------------------------+
bool CIntegralContext::AdminIntegration(const UserRecord *info,const Group &group)
  {
  Instrument instrument={0};
  //---
  if(!ExtServer||!info)
    return(false);
  //---
  bool atCCY=false;
  std::string oGroupSD;

  if(StringType::Compare(group.short_name,"@CCY")!=0)
    for(auto it:FIX::Session::getSessions())
      {
      /* || oInst.short_name != m_settings.get(it).getString("short_name")*/
      if(StringType::Compare(group.short_name,m_settings.get(it).getString("ShortName").c_str())!=0||it.getSenderCompID().getValue().find("order.")==std::string::npos)
        continue;

      if(!m_settings.get(it).has("AdminHost")||!m_settings.get(it).has("AdminPath")||!m_settings.get(it).has("AdminPort")||!m_settings.get(it).has("AdminStream"))
        return false;

      std::vector<std::string> leArgs;
      SPluginHelpers::split(m_settings.get(it).getString("SenderCompID"),".",leArgs);
      FIX::Dictionary d=m_settings.get(it);

      //std::string temp(group.on_behalf);
      StringType64 on_behalf;
      on_behalf.Assign(group.on_behalf);
      if(!on_behalf.Empty()&&on_behalf.FindChar('@')!=-1)
        {
        leArgs.clear();
        SPluginHelpers::split(on_behalf.Str(),"@",leArgs);
        }

      return HTTP(d.getString("AdminHost"),d.getInt("AdminPort"),d.getString("AdminPath"),info->login,leArgs[1],(d.has("PrimeBroker")?d.getString("PrimeBroker"):""),d.getString("DeliverToCompID"),d.getString("AdminStream")).Succeeded();
      }

  //if(StringType::Compare(group.short_name,"@CCY")==0)
  //  {
  //  atCCY=true;
  //  oGroupSD=group.on_behalf;
  //  // Stop searching Groups, iterate symbols for current Group
  //  }

  //if(!atCCY)
  //  return false; // will never occur, could use assert instead

  oGroupSD=group.on_behalf;

  ConGroup groupDef={0};
  if(!ExtServer->GroupsGet(info->group,&groupDef))
    {
    LOG_ERROR(pluginLog,"Cannot get information about group %s",info->group);
    return false;
    }

  std::list<int> secList;
  for(int i=0; i<MAX_SEC_GROUPS; i++)
    if(groupDef.secgroups[i].trade==TRUE)
      secList.push_back(i);

  bool hasUpdated=false;
  std::map<std::string,bool> sessionList;
  for(int i=0; i<MAX_SYMBOLS; i++)
    {
    ConSymbol symbol={0};
    if(ExtServer->SymbolsNext(i,&symbol)==FALSE)
      continue;

    if(std::find(secList.begin(),secList.end(),symbol.type)==secList.end())
      continue;

    //ZeroMemory(&instrument,sizeof(Instrument));			 
    //if(!m_integral_api.SymbolGet(tradeRequest.symbol.symbol,instrument))
    //	continue;

#pragma region work on this
    //sessionList[ccyMap[std::string(symbol.symbol)].short_name]=true;
#pragma endregion
    }

  /*for(UINT pos=0;pos<FIX::Session::numSessions();pos++)
    {

    }*/


  for(auto it:FIX::Session::getSessions())
    {
    for(auto &item:sessionList)
      {
      if(item.first!=m_settings.get(it).getString("ShortName")||it.getSenderCompID().getValue().find("order.")==std::string::npos)
        continue;

      if(!m_settings.get(it).has("AdminHost")||!m_settings.get(it).has("AdminPath")||!m_settings.get(it).has("AdminPort")||!m_settings.get(it).has("AdminStream"))
        continue;

      std::vector<std::string> leArgs;
      SPluginHelpers::split(m_settings.get(it).getString("SenderCompID"),".",leArgs);
      FIX::Dictionary d=m_settings.get(it);

      if(!oGroupSD.empty()&&oGroupSD.find("@")!=std::string::npos)
        {
        leArgs.clear();
        SPluginHelpers::split(oGroupSD,"@",leArgs);
        }

      hasUpdated|=HTTP(d.getString("AdminHost"),d.getInt("AdminPort"),d.getString("AdminPath"),info->login,leArgs[1],(d.has("PrimeBroker")?d.getString("PrimeBroker"):""),d.getString("DeliverToCompID"),d.getString("AdminStream")).Succeeded();
      }
    }
  //---
  return hasUpdated;
  }
//+------------------------------------------------------------------+
//| Start thread of connection processing                            |
//+------------------------------------------------------------------+
UINT __stdcall CIntegralContext::ProcessThreadWrapper(LPVOID param)
  {
  //--- check
  CIntegralContext *pThis=reinterpret_cast<CIntegralContext *>(param);
  if(pThis!=NULL) pThis->ProcessThread();
  //---
  return(0);
  }
//+------------------------------------------------------------------+
//| Process messages                                                 |
//+------------------------------------------------------------------+
void CIntegralContext::ProcessThread()
  {
  SessionIdentifiers session_ids;
  ConTime            time_cfg={0};
  //--- check
  /*if(m_initiator==NULL)
    {
    LOG_ERROR(pluginLog,"failed to start process thread, invalid initiator");
    return;
    }*/
    //---
    //if(!m_initiator->isStopped())
    //  return;
    //--- initialize connection
    //try
    //  {
    //  //--- update context state - processing thread started
    //  StateSet(STATE_STARTED);
    //  //--- start
    //  m_initiator->start();
    //  }
    //catch(const std::exception &e)
    //  {
    //  //--- notify of unsuccessful connection attempt
    //  LOG_ERROR(pluginLog,"failed to start [%s]",e.what());
    //  //--- update context state - processing thread stopped
    //  StateSet(STATE_STOPPED);
    //  //--- speed up shutdown
    //  UpdateTime((INT64)0);
    //  //--- return
    //  return;
    //  }
    //--- sleep  (1/2 second)
    //int t=0;
    //while(!m_initiator->isLoggedOn())
    //  {
    //  //--- if logout wait time exceeded
    //  if((t+=LOGON_WAIT)>LOGON_TIME)
    //    break;
    //  //--- sleep
    //  Sleep(LOGON_WAIT);
    //  }
    //--- write result to log
    //LOG_INFO(pluginLog,"connecting to integral <LoggedOn:%s>",m_initiator->isLoggedOn()?"true":"false");
    //--- flags of data processing
  bool data_send=false,session_connected=false;
  //--- loop of managing external connection
  while(InterlockedExchangeAdd(&m_thread_workflag,0)>0)
    {
    //--- process data for sending
    if(!SendCheck(data_send))
      {
      StateSet(STATE_STOPPED);
      break;
      }
    //--- check sessions
    //if(!m_initiator||m_initiator->isStopped())
    //  break;
    //---
    /*if(!SessionsCheck(session_connected))
      {
      StateSet(STATE_STOPPED);
      break;
      }*/
      //m_integral_api.GetSessions(session_ids,CIntegralSessionRecord::Disconnected);
      //if(session_ids.size()>0)
      //  {
      //  ExtServer->TimeGet(&time_cfg);
      //  //---
      //  const time_t rawtime = ExtServer->TradeTime();
      //  const time_t utime = time(NULL);
      //  struct tm *timeinfo;
      //  //---
      //  timeinfo = gmtime(&rawtime);
      //  //--- loop
      //  for(size_t pos=0;pos<session_ids.size();pos++)
      //    {
      //    if(time_cfg.days[timeinfo->tm_wday][timeinfo->tm_hour]!=TRUE) continue;
      //    //--- send logon
      //    SendLogon(session_ids[pos]);
      //    }
      //  }
      //--- update time of context activity
    //UpdateTime((INT64)THREAD_TIMEOUT);
    //--- if there was no data exchange, then sleep
    if(!data_send&&!session_connected)
      Sleep(THREAD_SLEEP);
    //else 
    //  Sleep(HEARBEAT_TIMEOUT);
    }
  //---
  /*m_sessions.GetSessionIds(session_ids);
  for(size_t pos=0;pos<session_ids.size();pos++)
    {
    SendLogout(session_ids[pos],"requested");
    }*/

    //--- update context state - processing thread stopped
  //StateSet(STATE_STOPPED);
  //--- close connection
  //m_initiator->stop(false);
  //--- speed up shutdown
  //UpdateTime((INT64)0);
  }
//+------------------------------------------------------------------+
//| Send data                                                        |
//+------------------------------------------------------------------+
bool CIntegralContext::SendCheck(bool &data_processed)
  {
  IntegralSession *session_ptr=NULL;
  bool res=true;
  //--- reset flag of data processing
  data_processed=false;
  //--- check socket state
  //if(order_session==NULL||!order_session->isLoggedOn())
  //  return(false);
  //--- clear temporary buffer for packets to be send
  m_message_send_tmp.Clear();
  //--- lock
  m_message_send_sync.Lock();
  //--- get packets buffer, leave empty buffer instead
  m_message_send.Swap(m_message_send_tmp);
  //--- unlock
  m_message_send_sync.Unlock();
  //--- if we have data, send them
  for(UINT pos=0;pos<m_message_send_tmp.Total();pos++)
    {
    //--- if data are already sent, set flag of data processing    
    if((res=res&&FIX::Session::sendToTarget(m_message_send_tmp[pos].fix_message,m_message_send_tmp[pos].fix_session_id)))
      {
      //--- set flag
      data_processed=true;
      }
    else
      {
      //--- write to log
      ExtLogger.Out(CmdErr,"failed to send message");
      }
    }
  //--- return result
  return(res);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CIntegralContext::SessionsCheck(bool &session_connected)
  {
  FIX::Session *session;
  SessionIdentifiers session_ids;
  FIX::HeartBtInt heartbeat;
  //--- set connection 
  session_connected=true;
  //--- get sessions
  m_integral_api.GetSessions(session_ids);
  if(session_ids.size()<1)
    return(false);
  //---
  for(size_t pos=0;pos<session_ids.size();pos++)
    {
    session=FIX::Session::lookupSession(session_ids[pos]);
    //--- check
    if(session->isLoggedOn()) continue;
    //--- why? session not logged in :))))
    session->logout("Not Connected");
    //--- 
    session->logon();
    //---
    session_connected=false;
    }
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| Sorting                                                          |
//+------------------------------------------------------------------+
int CIntegralContext::SortSessionsById(const void *left,const void *right)
  {
  const IntegralSession *lft=(const IntegralSession *)left;
  const IntegralSession *rgh=(const IntegralSession *)right;
  //--- compare
  return(StringType::Compare(lft->session_id.getSenderCompID().getValue().c_str(),rgh->session_id.getSenderCompID().getValue().c_str()));
  }
//+------------------------------------------------------------------+
//| Searching                                                        |
//+------------------------------------------------------------------+
int CIntegralContext::SearchSessionById(const void *left,const void *right)
  {
  LPCSTR                 lft=(LPCSTR)left;
  const IntegralSession *rgh=(const IntegralSession *)right;
  //--- compare
  return(StringType::Compare(lft,rgh->session_id.getSenderCompID().getValue().c_str()));
  }
//+------------------------------------------------------------------+
//| Searching                                                        |
//+------------------------------------------------------------------+
int CIntegralContext::SearchOrderSessionByShortName(const void *left,const void *right)
  {
  int res;
  //---
  LPCSTR                 lft=(LPCSTR)left;
  const IntegralSession *rgh=(const IntegralSession *)right;
  //--- check short name
  if((res=StringType::Compare(lft,rgh->short_name))!=0) return(res);
  //--- compare
  return(rgh->session_type!=IntegralSession::EnSessionType::Order);
  }
//+------------------------------------------------------------------+
//| Searching                                                        |
//+------------------------------------------------------------------+
int CIntegralContext::SearchQuoteSessionByShortName(const void *left,const void *right)
  {
  int res;
  //---
  LPCSTR                 lft=(LPCSTR)left;
  const IntegralSession *rgh=(const IntegralSession *)right;
  //--- check short name
  if((res=StringType::Compare(lft,rgh->short_name))!=0) return(res);
  //--- compare
  return(rgh->session_type!=IntegralSession::EnSessionType::Quote);
  }
//+------------------------------------------------------------------+
//| Searching                                                        |
//+------------------------------------------------------------------+
int CIntegralContext::SearchSessionByShortName(const void *left,const void *right)
  {
  //---
  LPCSTR                 lft=(LPCSTR)left;
  const IntegralSession *rgh=(const IntegralSession *)right;
  //--- check short name
  return(StringType::Compare(lft,rgh->short_name));
  }
//+------------------------------------------------------------------+
//| Sorting                                                          |
//+------------------------------------------------------------------+
int CIntegralContext::SearchShortNames(const void *left,const void *right)
  {
  LPCSTR lft=(LPCSTR)left;
  LPCSTR rgh=(LPCSTR)right;
  //--- compare
  return(StringType::Compare(lft,rgh));
  }
//+------------------------------------------------------------------+