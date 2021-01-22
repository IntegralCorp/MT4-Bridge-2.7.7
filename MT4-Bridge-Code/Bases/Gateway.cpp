//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "Gateway.h"
//+------------------------------------------------------------------+
//| Global object                                                    |
//+------------------------------------------------------------------+
CGateway *ExtGateway=NULL;
//+------------------------------------------------------------------+
//| 'this' : used in base member initializer list                    |
//+------------------------------------------------------------------+
#pragma warning(disable: 4355)
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
CGateway::CGateway()
  : m_api_server_config(NULL),
  m_api_integral(NULL),
  m_trade_dispatcher(),
  m_connect_time(0),
  m_connect_attempts(0),
  m_status_api_server(STATUS_API_STOPPED),
  m_status_api_integral(STATUS_DISCONNECTED)
  {
  //m_groups.Clear();
  //m_routes.Clear();
  m_buffer[0]='\0';
  }
#pragma warning(default: 4355)
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
CGateway::~CGateway()
  {
  //--- shutdown everything
  Shutdown();
  }
//+------------------------------------------------------------------+
//| Initialization                                                   |
//+------------------------------------------------------------------+
bool CGateway::Initialize(void)
  {
  ConSymbol             symb={ 0 };
  ConGroup              grp={ 0 };
  StringTypePath        directory;
  int                   pos=0;
  //--- check
  if(!ExtServer)
    return(false);
  //--- shutdown everything
  Shutdown();
  //---
  directory.Format("%s",ExtProgramPath);
  //--- check directory
  if(!CMTFile::DirectoryCreate(directory))
    {
    LOG_ERROR(pluginLog,"%s directory create error",directory.Str());
    return(false);
    }
  //if(!ExtWatchTower.Initialize())
  //  return(false);
  //-- initialize context
  if(!ExtPluginContext.Initialize())
    return(false);
  //--- create object of integral trading system API
  if((m_api_integral=new(std::nothrow) CIntegralAPI(this))==NULL)
    {
    LOG_ERROR(pluginLog,"failed to create Integral API instance");
    return(false);
    }
  //---
  StatusGateway(STATUS_API_CONFIGURED);
  //--- successful
  return(true);
  }
//+------------------------------------------------------------------+
//| Shutdown                                                         |
//+------------------------------------------------------------------+
void CGateway::Shutdown()
  {
  LOG_INFO(pluginLog,"CGateway shutdown called");
  //--- update Server API state
  StatusGateway(STATUS_API_STOPPED);
  //--- shutdown integral trading system API
  if(m_api_integral)
    {
    //--- update state of integral connection
    StatusExchange(STATUS_DISCONNECTED);
    //--- shutdown integral trading system and delete pointer
    m_api_integral->Shutdown();
    delete m_api_integral;
    m_api_integral=NULL;
    }
  //--- clear databases and parameters
  //m_sync.Lock();
  //---
  //m_sync.Unlock();
  //---
  ExtPluginContext.Shutdown();
  //--- release server config
  if(m_api_server_config)
    {
    delete m_api_server_config;
    m_api_server_config=NULL;
    }
  //---
  }
//+------------------------------------------------------------------+
//| Check gateway state                                              |
//+------------------------------------------------------------------+
void CGateway::Check(const DWORD curtime)
  {
  //--- check
  if(!m_api_integral)
    return;
  //--- exit if Gateway is not ready for work
  if(StatusGateway()<=STATUS_API_STOPPED)
    return;
  //--- API is ready for work, check connection to integral trading system
  if(StatusExchange()>=STATUS_CONNECTED)
    {
    //m_trade_dispatcher.Check(STDTIME(curtime));
    //--- check connection to integral trading system
    if(!m_api_integral->Check())
      {
      //--- integral trading system haven't responded during timeout
      LOG_ERROR(pluginLog,"integral timed out");
      StatusExchange(STATUS_DISCONNECTED);
      return;
      }
    }
  //--- analyze state, start connection in the check thread,
  //--- it is safe, because integral trading system API is operating asynchronously
  if(StatusExchange()<=STATUS_CONNECTING)
    Connect();
  }
//+------------------------------------------------------------------+
//| Check request timeouts                                           |
//+------------------------------------------------------------------+
void CGateway::Scheduler(const time_t curtime)
  {
  //--- exit if Server API is not ready for work
  if(StatusGateway()<=STATUS_API_STOPPED)
    return;
  //---
  m_trade_dispatcher.Check(curtime);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
//bool CGateway::PaperTrading(LPCSTR symbol,LPCSTR group)
//  {
//  bool res=false;
//  //---
//  for(auto it:m_settings.getSessions())
//    {
//    if(it.getSenderCompID().getValue().find("order.")!=std::string::npos)
//      {
//      res=m_settings.get(it).has("PaperTrading")?m_settings.get(it).getBool("PaperTrading"):false;
//      break;
//      }
//    }
//  //---
//  return(res);
//  }
//+------------------------------------------------------------------+
//| Intercepts execution of a manager's custom command.              |
//+------------------------------------------------------------------+
int CGateway::OnManagerProtocol(const ULONG ip,const UserInfo *us,const unsigned char *in_data,const int in_size,unsigned char **out_data,int *out_size)
  {
  StringType1024 msg_in;
  StringType128 ip_str;
  //--- checks
  if(!us||!in_data||in_size<1||!ExtServer)
    return(FALSE);
  //--- control this
  //int size = reinterpret_cast<int>(in_size);
  //std::string input(reinterpret_cast<const char*>(in_data));
  //std::size_t pos = input.find("|");
  //--- process 
  msg_in.Format("%s",in_data);
  SMTFormat::FormatIP(ip_str,ip);
  //--- is it ours
  if(us->login!=ExtPluginContext.ManagerUI())
    {
    LOG_INFO(pluginLog,"Ignoring unauthorized manager protocol %d (%s): %s",us->login,ip_str.Str(),msg_in.Str());
    return FALSE;
    }
  //--- print in message
  LOG_INFO(pluginLog,"OnManagerProtocol: in_size = %d in_data = %s",in_size,in_data);
  //--- usage
  /**out_size=3;
  *out_data=(unsigned char *)HEAP_ALLOC(*out_size);
  memcpy(*out_data,"QQ",*out_size);*/
  //---
  return(TRUE);
  }
//+------------------------------------------------------------------+
//| Notification of a new entry added to the server log.             |
//+------------------------------------------------------------------+
void CGateway::OnLog(const LogInfo *info)
  {
  //--- checks
  if(!info)
    return;
  //--- TODO:xxx
  }
//+------------------------------------------------------------------+
//| Send tick to Server API                                          |
//+------------------------------------------------------------------+
bool CGateway::HistoryAddBook(const IntegralMsgBook &integral_book,const Instrument *symbol)
  {
  FeedTick tick={ 0 };
  FeedData data={ 0 };
  bool res=false;
  //--- checks
  if(!ExtServer||!symbol)
    return(false);
  //--- monitor
  if(!ExtCloudWatch.Check(integral_book))
    {
    LOG_DEBUG(pluginLog,"ExtCloudWatch.Check(integral_book) failed <%s>",
              integral_book.book.ccy_pair);
    return(false);
    }
  //--- prepare data
  if((res=ProcessBook(symbol,integral_book,tick)))
    {
    //--- send data
    data.feeder=1;
    data.ticks_count=1;
    //---
    StringType::Copy(data.ticks[0].symbol,tick.symbol);
    StringType::Copy(data.ticks[0].bank,tick.bank);
    data.ticks[0].bid=tick.bid;
    data.ticks[0].ask=tick.ask;
    //---
    ExtServer->HistoryAddTick(&data);
    }
  else
    {
    LOG_DEBUG(pluginLog,"ProcessBook %s <%s>",res?"true":"false",symbol->ccy_pair);
    }
  //---
  //--- return result
  return(res);
  }
//+------------------------------------------------------------------+
//| Send tick to Server API                                          |
//+------------------------------------------------------------------+
//bool CGateway::HistoryAddTick(SymbolTick &integral_tick,const Instrument &symbol)
//  {
//  FeedTick tick={0};
//  FeedData data={0};
//  bool res=false;
//  //--- checks
//  if(!ExtServer)
//    return(false);
//  //--- monitor
//  //if(!m_cloud_watch.Check(integral_book))
//  //  return(false);
//  //--- prepare data
//  if(!ProcessTick(&symbol,integral_tick,tick))
//    return(false);
//  //--- send data
//  data.feeder=1;
//  data.ticks_count=1;
//  //---
//  StringType::Copy(data.ticks[0].symbol,tick.symbol);
//  StringType::Copy(data.ticks[0].bank,tick.bank);
//  data.ticks[0].bid=tick.bid;
//  data.ticks[0].ask=tick.ask;
//  //---
//  ExtServer->HistoryAddTick(&data);
//  //--- return result
//  return(res);
//  }
//+------------------------------------------------------------------+
//| Updates a trade record (activation, closure, deletion or         |
//| modification)                                                    |
//+------------------------------------------------------------------+
bool CGateway::OrdersUpdate(TradeRecord *order,UserInfo *inf,const int mode)
  {
  //--- chceks
  if(!ExtServer||!order||!inf)
    return(false);
  //---
  return(ExtServer->OrdersUpdate(order,inf,mode)==TRUE);
  }
//+------------------------------------------------------------------+
//| Find and close order                                             |
//+------------------------------------------------------------------+
bool CGateway::OrdersClose(const int order,const int volume,const double close_price,LPCSTR comment,LPCSTR order_id)
  {
  //---
  return(m_trade_dispatcher.OrdersClose(order,volume,close_price,comment,order_id));
  }
//+------------------------------------------------------------------+
//| Find and close order                                             |
//+------------------------------------------------------------------+
bool CGateway::OrdersUpdateClose(const int order,const int volume,const double close_price,LPCSTR order_id)
  {
  //---
  return(m_trade_dispatcher.OrdersUpdateClose(order,volume,close_price,order_id));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int CGateway::OrdersOpen(const int login,const int cmd,LPCSTR symbol,const double open_price,const int volume,LPCSTR order_id,const double stop_loss,const double take_profit)
  {
  //---
  return(m_trade_dispatcher.OrdersAddOpen(login,cmd,symbol,open_price,volume,order_id,stop_loss,take_profit));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int CGateway::OrdersAddOpen(const TradeRecord *trade,UserInfo *user,const int volume)
  {
  // checks
  if(!trade||!user||volume<=0)
    return(FALSE);
  //---
  return(m_trade_dispatcher.OrdersAddOpen(trade,user,volume));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int CGateway::OrdersUpdateActivate(const int order,const int volume,const double open_price,LPCSTR order_id)
  {
  // checks
  if(order<1||volume<=0||open_price<=0||!order_id)
    return(FALSE);
  //---
  return(m_trade_dispatcher.OrdersUpdateActivate(order,volume,open_price,order_id));
  }
//+------------------------------------------------------------------+
//| preapre comment                                                  |
//+------------------------------------------------------------------+
bool CGateway::AddComment(LPCSTR current_comment,LPCSTR apPrefix,LPCSTR apExecID,StringType &comment)
  {
  //---
  return(m_trade_dispatcher.AddComment(current_comment,apPrefix,apExecID,comment));
  }
//+------------------------------------------------------------------+
//| filter stopout                                                   |
//+------------------------------------------------------------------+
int CGateway::StopoutsFilter(const ConGroup *group,const ConSymbol *symbol,const int login,const double equity,const double margin)
  {
  //--- checks
  if(!group||!symbol||login<1||!ExtServer)
    return(RET_OK_NONE);
  //--- show
  LOG_JOURNAL(pluginLog,"(%d) User %d, group %0.16s, symbol %0.12s, equity %.10g, margin %.10g",
              login,
              login,
              group->group,
              symbol->symbol,
              equity,
              margin);
  //--- valid
  if(!CheckGroupAndSymbol(login,symbol->symbol,group->group))
    {
    // Return Default value, plugin won't handle
    return(RET_OK_NONE);
    }
  //--- We will handle
  return(RET_OK);
  }
//+------------------------------------------------------------------+
//| check order validity                                             |
//+------------------------------------------------------------------+
bool CGateway::CheckGroupAndSymbol(const int login,LPCSTR symbol,LPCSTR group,bool add_logs)
  {
  //--- checks
  if(!symbol||!group||login<1||!m_api_integral)
    return(false);
  //---
  return(m_api_integral->CheckGroupAndSymbol(login,symbol,group,add_logs));
  }
//+------------------------------------------------------------------+
//| check order validity                                             |
//+------------------------------------------------------------------+
bool CGateway::CheckGroupAndSymbol(const int login,LPCSTR symbol)
  {
  UserRecord record={ 0 };
  //--- checks
  if(!symbol||login<1||!m_api_integral)
    return(false);
  //----
  if(ExtServer->ClientsUserInfo(login,&record)==FALSE)
    return(false);
  //---
  return(m_api_integral->CheckGroupAndSymbol(login,symbol,record.group));
  }
//+------------------------------------------------------------------+
//| check client can open hedged position                            |
//+------------------------------------------------------------------+
int CGateway::CheckHedgeAllowed(LPCSTR group,LPCSTR symbol,const int login,const short cmd)
  {
  ConGroup     group_cfg={ 0 };
  UserInfo     user={ 0 };
  TradeRecord *trades=NULL;
  TradeRecord *trade=NULL;
  int          total;
  //--- checks
  if(!group||!symbol||login<1||!ExtServer)
    return(RET_ERROR);
  //--- get group config
  if(ExtServer->GroupsGet(group,&group_cfg)==FALSE)
    {
    LOG_ERROR(pluginLog,"groups get failed for \" %s\"",group);
    return(RET_INVALID_DATA);
    }
  //--- hedge allowed?
  if(group_cfg.hedge_prohibited==FALSE)
    return(RET_OK);
  //---
  if(m_trade_dispatcher.UserInfoGet(login,&user)==FALSE)
    {
    LOG_ERROR(pluginLog,"'%d' user info get failed",login);
    return(RET_BAD_ACCOUNT_INFO);
    }
  //---
  bool found=false;
  //--- check positions
  if((trades=ExtServer->OrdersGetOpen(&user,&total))!=NULL)
    {
    for(int pos=0;pos<total;pos++)
      {
      trade=&trades[pos];
      if(StringType::Compare(trade->symbol,symbol)!=0) continue;
      if(OrderIsBuy(trade->cmd)&&!OrderIsBuy(cmd)||
         !OrderIsBuy(trade->cmd)&&OrderIsBuy(cmd))
        {
        found=true;
        break;
        }
      }
    //--- free
    HEAP_FREE(trades);
    trades=NULL;
    }
  //--- check
  if(found)
    return(RET_TRADE_HEDGE_PROHIBITED);
  //--- successful 
  return(RET_OK);
  }
//+------------------------------------------------------------------+
//| check close only                                                 |
//+------------------------------------------------------------------+
bool CGateway::CheckCloseOnly(const int ticket)
  {
  ConSymbol      cs={ 0 };
  TradeRecord trade={ 0 };
  UserInfo       ui={ 0 };
  //--- checks 
  if(!ExtServer||ticket<1)
    return(false);
  //---
  if(ExtServer->OrdersGet(ticket,&trade)!=TRUE)
    {
    LOG_ERROR(pluginLog,"order #%d get failed",ticket);
    return(false);
    }
  //---
  if(ExtServer->SymbolsGet(trade.symbol,&cs)==FALSE)
    {
    LOG_ERROR(pluginLog,"'%d' SymbolsGet failed for symbol:<%s>",trade.login,trade.symbol);
    return(false);
    }
  //---
  if(cs.trade==TRADE_CLOSE)
    {
    LOG_INFO(pluginLog,"'%d' only closing of already opened positions are possible for %s",
             trade.login,
             trade.symbol);
    //---
    if(m_trade_dispatcher.UserInfoGet(trade.login,&ui)!=TRUE)
      {
      LOG_ERROR(pluginLog,"(%d) Error getting user info for login '%d'",
                trade.order,
                trade.login);
      return false;
      }
    //--- prepare
    trade.close_time=ExtServer->TradeTime();
    trade.profit=0;
    trade.storage=0;
    trade.expiration=0;
    trade.taxes=0;
    StringType::Copy(trade.comment,"deleted [close only]");
    //---- delete order
    if(ExtServer->OrdersUpdate(&trade,&ui,UPDATE_CLOSE)==FALSE)
      LOG_ERROR(pluginLog,"OrdersUpdate (UPDATE_CLOSE) failed %d",trade.order);
    //---
    return(false);
    }
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| check close only                                                 |
//+------------------------------------------------------------------+
bool CGateway::CheckCloseOnly(const int ticket,const ConSymbol *cs)
  {
  TradeRecord trade={ 0 };
  UserInfo       ui={ 0 };
  //--- checks 
  if(!cs||ticket<1||!ExtServer)
    return(false);
  //---
  if(cs->trade!=TRADE_CLOSE)
    return(true);
  //--- show
  LOG_INFO(pluginLog,"'%d' only closing of already opened positions are possible for %s",
           trade.login,
           trade.symbol);
  //---
  if(ExtServer->OrdersGet(ticket,&trade)!=TRUE)
    {
    LOG_ERROR(pluginLog,"order #%d get failed",ticket);
    return(false);
    }
  //---
  if(m_trade_dispatcher.UserInfoGet(trade.login,&ui)!=TRUE)
    {
    LOG_ERROR(pluginLog,"(%d) Error getting user info for login '%d'",
              trade.order,
              trade.login);
    return false;
    }
  //--- prepare
  trade.close_time=ExtServer->TradeTime();
  trade.profit=0;
  trade.storage=0;
  trade.expiration=0;
  trade.taxes=0;
  StringType::Copy(trade.comment,"deleted [close only]");
  //---- delete order
  if(ExtServer->OrdersUpdate(&trade,&ui,UPDATE_CLOSE)==FALSE)
    LOG_ERROR(pluginLog,"OrdersUpdate (UPDATE_CLOSE) failed %d",trade.order);
  //---
  return(false);
  }
//+------------------------------------------------------------------+
//| check closeby                                                    |
//+------------------------------------------------------------------+
bool CGateway::CheckCloseBy(const int order,const int orderby)
  {
  //--- checks 
  if(order<1||orderby<1)
    return(false);
  //---
  return(m_trade_dispatcher.CheckCloseBy(order,orderby));
  }
//+------------------------------------------------------------------+
//| check closeby                                                    |
//+------------------------------------------------------------------+
bool CGateway::CheckAlreadyClosed(const int order)
  {
  TradeRecord old_trade={ 0 };
  //--- checks 
  if(order<1||!ExtServer)
    return(true);
  //--- get order 
  if(ExtServer->OrdersGet(order,&old_trade)!=TRUE)
    {
    LOG_ERROR(pluginLog,"orders get failed #%d",order);
    return(true);
    }
  //--- check already closed
  if(old_trade.close_time>0)
    {
    LOG_ERROR(pluginLog,"order already closed #%d",order);
    return(true);
    }
  //---
  return(false);
  }
//+------------------------------------------------------------------+
//| check closeby                                                    |
//+------------------------------------------------------------------+
bool CGateway::CheckMultipleCloseBy(const int login,LPCSTR symbol)
  {
  //--- checks 
  if(login<1||!symbol)
    return(false);
  //---
  return(m_trade_dispatcher.CheckRequest(login,symbol));
  }
//+------------------------------------------------------------------+
//| check pending orders of the client fr the specified symbol       |
//+------------------------------------------------------------------+
bool CGateway::CheckPendingRequests(const int login)
  {
  //--- checks 
  if(login<1)
    return(false);
  //---
  return(m_trade_dispatcher.CheckPendingRequests(login));
  }
//+------------------------------------------------------------------+
//| check pending orders of the client fr the specified symbol       |
//+------------------------------------------------------------------+
int CGateway::CheckExecutionOnly(const int login,LPCSTR symbol)
  {
  ConSymbol  cs={ 0 };
  ConGroup   group={ 0 };
  UserRecord record={ 0 };
  //--- checks 
  if(login<1||!symbol)
    return(RET_INVALID_DATA);
  //--- get user info
  if(ExtServer->ClientsUserInfo(login,&record)==FALSE)
    return(RET_BAD_ACCOUNT_INFO);
  //--- get group
  if(ExtServer->GroupsGet(record.group,&group)==FALSE)
    return(RET_INVALID_DATA);
  //--- get symbol
  if(ExtServer->SymbolsGet(symbol,&cs)==FALSE)
    return(RET_INVALID_DATA);
  //--- check auto
  if(group.secgroups[cs.type].execution!=EXECUTION_MANUAL)
    return(RET_TRADE_DISABLE);
  //---
  return(RET_OK);
  }
//+------------------------------------------------------------------+
//| check pending orders of the client fr the specified symbol       |
//+------------------------------------------------------------------+
int CGateway::CheckExecutionOnly(const int login,LPCSTR symbol_name,UserRecord &record,ConSymbol &symbol)
  {
  ConGroup group={ 0 };
  //--- checks 
  if(login<1||!symbol_name)
    return(RET_INVALID_DATA);
  //--- get uswr info
  if(ExtServer->ClientsUserInfo(login,&record)==FALSE)
    return(RET_BAD_ACCOUNT_INFO);
  //--- get symbol/group
  if(ExtServer->SymbolsGet(symbol_name,&symbol)==FALSE||
     ExtServer->GroupsGet(record.group,&group)==FALSE)
    return(RET_INVALID_DATA);
  //--- check auto
  if(group.secgroups[symbol.type].execution!=EXECUTION_MANUAL)
    return(RET_TRADE_DISABLE);
  //---
  return(RET_OK);
  }
//+------------------------------------------------------------------+
//| check ticket                                                     |
//+------------------------------------------------------------------+
//bool CGateway::OrderInProcess(const int login,const int order)
//{
//  //--- lets process the request
//  return(m_trade_dispatcher.RequestExists(login,order));
//}
//+------------------------------------------------------------------+
//| check ticket                                                     |
//+------------------------------------------------------------------+
//bool CGateway::RemoveIfExists(const int id,const int order,TradeResponse &response)
//  {
//  //--- lets process the request
//  return(m_trade_dispatcher.RemoveIfExists(id,order,response));
//  }
//+------------------------------------------------------------------+
//| remove requtes                                                   |
//+------------------------------------------------------------------+
//bool CGateway::RemoveByUniqueId(LPCSTR unique_id)
//  {
//  //--- lets process the request
//  return(m_trade_dispatcher.RemoveByUniqueId(unique_id));
//  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
//bool CGateway::Add(const INT request_id,const TradeRequest& record)
//  {
//  return(m_trade_dispatcher.AddRequest(request_id,record));
//  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CGateway::CheckRequest(const INT request_id)
  {
  return(m_trade_dispatcher.CheckRequest(request_id));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CGateway::CheckRequest(const INT login,LPCSTR symbol)
  {
  return(m_trade_dispatcher.CheckRequest(login,symbol));
  }
//+------------------------------------------------------------------+
//| search request by order                                          |
//+------------------------------------------------------------------+
bool CGateway::CheckRequestByOrder(const INT order)
  {
  return(m_trade_dispatcher.CheckRequestByOrder(order));
  }
//+------------------------------------------------------------------+
//| search request by order                                          |
//+------------------------------------------------------------------+
bool CGateway::CheckRequestByOrder(const INT order,const INT order_by)
  {
  return(m_trade_dispatcher.CheckRequestByOrder(order,order_by));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CGateway::RemoveRequest(const INT request_id)
  {
  return(m_trade_dispatcher.RemoveRequest(request_id));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CGateway::RemoveRequest(const INT request_id,TradeRequest &record)
  {
  return(m_trade_dispatcher.RemoveRequest(request_id,record));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CGateway::RemoveRequestByOrder(const INT order,TradeRequest &record)
  {
  return(m_trade_dispatcher.RemoveRequestByOrder(order,record));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CGateway::GetRequestByOrder(const INT order,TradeRequest &record)
  {
  return(m_trade_dispatcher.GetRequestByOrder(order,record));
  }
//+------------------------------------------------------------------+
//| Admin Integration                                                |
//+------------------------------------------------------------------+
bool CGateway::AdminIntegration(const UserRecord *info)
  {
  //--- checks
  if(!info||!m_api_integral)
    return(false);
  //--- 
  return(m_api_integral->AdminIntegration(info));
  }
//+------------------------------------------------------------------+
//| Notify of gateway start                                          |
//+------------------------------------------------------------------+
//void CGateway::OnGatewayStart()
//  {
////--- if gateway is already connected, write to log
//   if(StatusGateway()==STATUS_API_STARTED)
//     {
//      ExtLogger.Out(MTLogWarn,L"start skipped, remote connection already exist");
//      return;
//     }
////--- if gateway has not received settings, write to log
//   if(StatusGateway()!=STATUS_API_CONFIGURED)
//     {
//      ExtLogger.Out(MTLogWarn,L"start failed, gateway settings didn't receive");
//      return;
//     }
////--- write to log
//   ExtLogger.Out(MTLogOK,L"'%s' initialized",m_api_gateway_config->Name());
////--- update state of connection to external trading system (start connection process)
//   StatusExchange(STATUS_CONNECTING);
////--- update state of Gateway API
//   StatusGateway(STATUS_API_STARTED);
//  }
//+------------------------------------------------------------------+
//| Notify of gateway stop                                           |
//+------------------------------------------------------------------+
//void CGateway::OnGatewayStop()
//  {
////--- write to log
//   ExtLogger.Out(MTLogOK,L"stopped");
////--- update state
//   StatusGateway(STATUS_API_CONFIGURED);
////--- disconnect
//   Disconnect();
//  }
//+------------------------------------------------------------------+
//| process trade request                                            |
//+------------------------------------------------------------------+
bool CGateway::OnTradeRequestApply(TradeRequest *record)
  {
  //--- transfer to trades dispatcher
  return(m_trade_dispatcher.OnRequestLocked(record));
  }
//+------------------------------------------------------------------+
//| process trade request                                            |
//+------------------------------------------------------------------+
bool CGateway::OnTradeRequestApply(RequestInfo *request,const UINT action)
  {
  //--- transfer to trades dispatcher
  return(m_trade_dispatcher.OnRequestLocked(request,action));
  }
//+------------------------------------------------------------------+
//| End of day                                                       |
//+------------------------------------------------------------------+
void CGateway::OnEODFinish(const GroupState *groups,const int groups_total,const DailyReport *reports,const int reports_total)
  {
  //--- checks
  if(!groups||groups_total<1||!reports||reports_total<1||!ExtServer)
    return;
  //---
  if(!ExtPluginContext.EODReports())
    return;
  }
//+------------------------------------------------------------------+
//| notify of adding/updating of a client group.                     |
//+------------------------------------------------------------------+
bool CGateway::UpdateGroupCache(const ConGroup *group)
  {
  //--- checks
  if(!group||!m_api_integral)
    return(false);
  //---
  return(m_api_integral->UpdateGroupCache(group));
  }
//+------------------------------------------------------------------+
//| notify of adding/updating of a symbol.                           |
//+------------------------------------------------------------------+
bool CGateway::UpdateSymbolCache(const ConSymbol *cs)
  {
  //--- checks
  if(!cs||!m_api_integral)
    return(false);
  //---
  return(m_api_integral->UpdateSymbolCache(cs));
  }
//+------------------------------------------------------------------+
//| process excutions report                                         |
//+------------------------------------------------------------------+
void CGateway::SendAcknowledgement(LPCSTR unique_id)
  {
  //--- checks
  if(!m_api_integral)
    return;
  //--- send
  if(!m_api_integral->SendAck(unique_id))
    {
    }
  }
//+------------------------------------------------------------------+
//| Send dont know                                              |
//+------------------------------------------------------------------+
bool CGateway::SendDontKnowTrade(LPCSTR order_str,UINT reason,LPCSTR text)
  {
  //--- checks
  if(!order_str||!text||!m_api_integral)
    return(false);
  //----
  return(m_api_integral->SendDontKnowTrade(order_str,reason,text));
  }
//+------------------------------------------------------------------+
//| Set connection state                                             |
//+------------------------------------------------------------------+
UINT CGateway::StatusGateway(UINT state)
  {
  UINT old_state=(UINT)InterlockedExchange((volatile LONG *)&m_status_api_server,state);
  //--- notify after changing state
  if(old_state!=state)
    {
    //m_integral_api.OnContextStateUpdate(state);
    }
  //--- return previous state of connection context
  return(old_state);
  }
//+------------------------------------------------------------------+
//| Get connection state                                             |
//+------------------------------------------------------------------+
UINT CGateway::StatusGateway()
  {
  //--- return state of connection context
  return((UINT)InterlockedExchangeAdd((volatile LONG *)&m_status_api_server,0));
  }
//+------------------------------------------------------------------+
//| Get external connection state                                    |
//+------------------------------------------------------------------+
LONG CGateway::StatusExchange()
  {
  //--- get state
  return(InterlockedExchangeAdd(&m_status_api_integral,0));
  }
//+------------------------------------------------------------------+
//| Set external connection state                                    |
//+------------------------------------------------------------------+
LONG CGateway::StatusExchange(const LONG status)
  {
  LONG old_status=InterlockedExchange(&m_status_api_integral,status);
  //--- notify of gateway state changed
  if(ExtServer)
    {
    //--- check state
    if(status==STATUS_SYNCHRONIZED)
      {
      //--- connect to the trading request queue
      //m_api_server->DealerStart(IMTGatewayAPI::DEALER_FLAG_AUTOLOCK|IMTGatewayAPI::DEALER_FLAG_EXTERNAL_ACC);
      //--- update connection state
      //m_api_server->StateConnect(1);
      //--- clear reconnect time/attempts
      m_connect_time=0;
      m_connect_attempts=0;
      }
    else
      {
      //--- update connection state
//m_api_server->StateConnect(0);
      }
    }
  //--- write to log if disconnected
  if(old_status!=STATUS_DISCONNECTED&&status==STATUS_DISCONNECTED)
    {
    //--- if synchronized, disconnect from the request queue
    //if(old_status==STATUS_SYNCHRONIZED)
   //	m_api_server->DealerStop();
    //--- write to log
    //ExtLogger.Out(MTLogOK,L"connect to exchange closed");
    }
  //--- return old state
  return(old_status);
  }
//+------------------------------------------------------------------+
//| Notification of successful session load                          |
//+------------------------------------------------------------------+
void CGateway::OnSessionLoaded(const FIX::SessionSettings &settings)
  {
  //m_settings=settings;
  }
//+------------------------------------------------------------------+
//| Notification of successful logon                                 |
//+------------------------------------------------------------------+
void CGateway::OnLogon(PCSTR short_name)
  {
  if(!short_name)
    return;
  //--- notify trade dispatcher
  m_trade_dispatcher.OnLogon(short_name);
  //--- write to log
  ExtLogger.Out(CmdOK,"logon received from exchange");
  }
//+------------------------------------------------------------------+
//| Notification of successful gateway synchronization               |
//+------------------------------------------------------------------+
void CGateway::OnIntergalSyncComplete()
  {
  //--- update state
  StatusExchange(STATUS_SYNCHRONIZED);
  //--- write to log
  LOG_INFO(pluginLog,"synchronized with integral");
  }
//+------------------------------------------------------------------+
//| Notification of closed connection                                |
//+------------------------------------------------------------------+
void CGateway::OnIntergalConnectClosed()
  {
  //--- update state
  StatusExchange(STATUS_DISCONNECTED);
  }
//+------------------------------------------------------------------+
//| Notify of added symbol                                           |
//+------------------------------------------------------------------+
//bool CGateway::OnExchangeSymbolAdd(const ExchangeSymbol &exchange_symbol)
//  {
//  bool res=false;
//  //--- check
//  if(!m_api_server||!m_api_gateway_config)
//	 return(false);
//  //--- check if symbols import via gateway is allowed
//  if((m_api_gateway_config->Flags()&IMTConGateway::GATEWAY_FLAG_IMPORT_SYMBOLS)==0)
//	 return(false);
//  //--- create symbol
//  IMTConSymbol *gateway_symbol=NULL;
//  if((gateway_symbol=m_api_server->SymbolCreate())==NULL)
//	 {
//	 ExtLogger.Out(MTLogErr,L"failed to create symbol interface instance");
//	 return(false);
//	 }
//  //--- import symbol
//  res=ImportSymbol(exchange_symbol,gateway_symbol);
//  //--- log error message
//  if(!res)
//	 ExtLogger.Out(MTLogErr,L"failed to import symbol %s",exchange_symbol.symbol);
//  //--- add symbol to Gateway API
//  res=res&&(m_api_server->SymbolUpdate(gateway_symbol)==MT_RET_OK);
//  //--- release symbol interface
//  gateway_symbol->Release();
//  //--- return result
//  return(res);
//  }
//+------------------------------------------------------------------+
//| Notify of order transaction in external trading system           |
//+------------------------------------------------------------------+
//bool CGateway::OnExchangeOrderTrans(const ExchangeOrder &exchange_order,const ExchangeSymbol &symbol)
//  {
//  //--- pass order transaction to trade dispatcher
//  return(m_trade_dispatcher.OnExchangeOrderTrans(exchange_order,symbol));
//  }
//+------------------------------------------------------------------+
//| Notify of deal transaction in external trading system            |
//+------------------------------------------------------------------+
bool CGateway::OnExecutionReportTrans(const FIX43::ExecutionReport &report,const Instrument &symbol)
  {
  return(m_trade_dispatcher.OnExecutionReportTrans(report,symbol));
  }
//+------------------------------------------------------------------+
//| Filter pending orders                                            |
//+------------------------------------------------------------------+
bool CGateway::PendingsFilter(const ConGroup *group,const ConSymbol *symbol,const TradeRecord *trade,bool hedge_prohibited)
  {
  TradeRecord *trades=NULL;
  UserInfo       user={ 0 };
  int            total=0;
  SymbolPosition position={ 0 };
  //--- margin
  double         symbol_margin,symbol_margin_new,margin_rate;
  double         margin;         // Current and new margin
  double         freemargin,new_freemargin; // Current and new free margin
  double         equity;                    // Current equity
  //--- checks
  if(!group||!symbol||!trade||!ExtServer)
    return(false);
  //--- check hedge prohibited
  if(group->hedge_prohibited==TRUE)
    hedge_prohibited=true;
  //--- get user info
  if(m_trade_dispatcher.UserInfoGet(trade->login,&user)==FALSE)
    return(false);
  //--- Receive the current margin state and description (UserInfo) of a client by login.
  if(ExtServer->TradesMarginGet(trade->login,&user,&margin,&freemargin,&equity)==FALSE)
    {
    ExtLogger.Out(CmdErr,"'%d' trades marign get failed for #%d",trade->login,trade->order);
    return(false);
    }
  //---
  if((trades=ExtServer->OrdersGetOpen(&user,&total))!=NULL)
    {
    //--- loop orders
    for(int pos=0;pos<total;pos++)
      {
      const TradeRecord record=trades[pos];
      if(StringType::Compare(record.symbol,trade->symbol)==0)
        {
        //--- checks hedge allowed
        if(hedge_prohibited)
          {
          switch(record.cmd)
            {
            case OP_BUY:
              if(trade->cmd==OP_SELL)
                return(false);
              break;
            case OP_SELL:
              if(trade->cmd==OP_BUY)
                return(false);
              break;
            default:
              break;
            }
          }
        //--- don't process pending orders unless they are activated
        if(!(record.activation>SymbolPosition::EnActivation::ACTIVATION_NONE&&
             record.cmd>=OP_BUY_LIMIT))
          continue;
        }
      //---
      SymbolPosition::PositionAddVolume(position,record.volume,record.margin_rate,record.cmd);
      }
    //--- free trades
    HEAP_FREE(trades);
    trades=NULL;
    }
  //--- calculate percentage
  double percentage=symbol->margin_divider;
  for(int i=0;i<group->secmargins_total;i++)
    {
    if(StringType::Compare(group->secmargins[i].symbol,trade->symbol)==0)
      {
      if(group->secmargins[i].margin_divider!=DBL_MAX)
        percentage=group->secmargins[i].margin_divider;
      break;
      }
    }
  //--- lets calculate margin
  symbol_margin=CalcMarginAmount(symbol,&position,trade->close_price,user.leverage,percentage);
  //--- add new position   ? check margin rate - might be zero
  if(margin_rate=ExtServer->TradesCalcConvertation(group->group,TRUE,trade->open_price,symbol)<=0)
    {
    ExtLogger.Out(CmdErr,"'%d' marign rate calculation failed for #%d [group: %s]",trade->login,trade->order,group->group);
    return(false);
    }
  //--- add new volume
  SymbolPosition::PositionAddVolume(position,trade->volume,margin_rate,trade->cmd);
  //--- lets recalculate margin
  symbol_margin_new=CalcMarginAmount(symbol,&position,trade->close_price,user.leverage,percentage);
  //--- calculate and check new free margin
  new_freemargin=equity-(margin-symbol_margin+symbol_margin_new);
  if(new_freemargin>0||new_freemargin>freemargin)
    return(true);
  //---
  return(false);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
std::string CGateway::GetConfigs(void)
  {
  //--- checks
  if(!m_api_integral)
    return(std::string(""));
  //---
  return(m_api_integral->GetConfigs());
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CGateway::SetGroup(std::string &group,std::string &config,const bool save_file,const bool force_update)
  {
  //--- checks
  if(!m_api_integral)
    return(false);
  //---
  return(m_api_integral->SetGroup(group,config,save_file,force_update));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CGateway::SetRule(const int index,StringType &config,const bool save_file,const bool force_update)
  {
  //--- checks
  if(!m_api_integral)
    return(false);
  //---
  return(m_api_integral->SetRule(index,config,save_file,force_update));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CGateway::SetInstrument(std::string &symbol,std::string &config,const bool save_file,const bool force_update)
  {
  //--- checks
  if(!m_api_integral)
    return(false);
  //---
  return(m_api_integral->SetInstrument(symbol,config,save_file,force_update));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CGateway::OnOrderOpen(const TradeRequest &record,StringType &message)
  {
  //---
  return(m_trade_dispatcher.ProcessOrderOpen(record,message));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CGateway::OnOrderClose(const TradeRequest &record,StringType &message)
  {
  //---
  return(m_trade_dispatcher.ProcessOrderClose(record,message));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CGateway::ProcessMarketOpen(const TradeRequest &record,StringType &message)
  {
  //---
  return(m_trade_dispatcher.ProcessMarketOpen(record,message));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CGateway::ProcessMarketClose(const TradeRequest &record,StringType &message)
  {
  //---
  return(m_trade_dispatcher.ProcessMarketClose(record,message));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CGateway::ProcessPendingOrderActivation(const TradeRequest &record,StringType &message)
  {
  //---
  return(m_trade_dispatcher.ProcessPendingOrderActivation(record,message));
  }
//+------------------------------------------------------------------+
//| Connect                                                          |
//+------------------------------------------------------------------+
bool CGateway::Connect()
  {
  INT64 ctm=_time64(NULL);
  //--- check
  if(!m_api_integral/*||!m_api_gateway_config*/)
    {
    StatusExchange(STATUS_DISCONNECTED);
    return(false);
    }
  //--- check reconnect timeout
  if(ctm<m_connect_time)
    return(false);
  //--- set connect time
  m_connect_time=ctm+((m_connect_attempts++<RECONNECT_LONG_COUNT)?RECONNECT_SHORT_TIMEOUT:RECONNECT_LONG_TIMEOUT);
  //--- update state
  StatusExchange(STATUS_CONNECTED);
  //--- initialize trade dispatcher
  if(!m_trade_dispatcher.Initialize(m_api_integral))
    {
    LOG_ERROR(pluginLog,"failed to initialize trade dispatcher");
    StatusExchange(STATUS_DISCONNECTED);
    return(false);
    }
  //--- initialize external trading system API
  if(!m_api_integral->Initialize())
    {
    LOG_ERROR(pluginLog,"failed to initialize Integral API");
    StatusExchange(STATUS_DISCONNECTED);
    return(false);
    }
  //--- successful
  return(true);
  }
//+------------------------------------------------------------------+
//| Disconnect from server                                           |
//+------------------------------------------------------------------+
//void CGateway::Disconnect()
//  {
//  //--- stop integral trading system API
//  if(m_api_integral)
//    m_api_integral->Shutdown();
//  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CGateway::ProcessBook(const Instrument *inst,const IntegralMsgBook &integral_book,FeedTick &gateway_tick)
  {
  double                oBid,oAsk,totalAmount;
  double                prices[2]={ 0 };
  //--- checks
  if(!inst)
    return(false);
  //---
  ZeroMemory(&gateway_tick,sizeof(FeedTick));
  //--- process bids
  totalAmount=inst->price_size;
  oBid=ProcessPrice(integral_book.book,MTBookItem::ItemBuyMarket,totalAmount,inst);
  //--- process asks
  totalAmount=inst->price_size;
  oAsk=ProcessPrice(integral_book.book,MTBookItem::ItemSellMarket,totalAmount,inst);
  //---
  if(oBid<=0||oAsk<=0)
    {
    LOG_DEBUG(pluginLog,"%.10g / %.10g",oBid,oAsk);
    return false;
    }

  /*if(ExtBridgeConfig.SavePriceUpdates())
    LOG_DEBUG(pluginLog,"(MinSpread) '%s' Bid %.10g, Ask %.10g, Spread: %.10g, MinSpread: %.10g, SymSpread: %d, midPrice: %.10g, delta: %.10g, fBid: %.10g, fAsk: %.10g, fSpread: %.10g",sym.c_str(),oBid,oAsk,oSpread,inst.min_spread,symSpread,midPrice,deltaSpread,finalBid,finalAsk,(finalAsk-finalBid));

  Spreads.set(sym,std::pair<double,double>(oBid-finalBid,finalAsk-oAsk));
  }
else
  Spreads.set(sym,std::pair<double,double>(0,0));*/

  double finalBid=oBid-inst->bid_spread;
  double  finalAsk=oAsk+inst->ask_spread;

  int oDigits=inst->digits,symSpread=inst->spread;
  double oSpread=CGateway::RoundTo(finalAsk-finalBid,oDigits);
  double minSpread=inst->min_spread;

  if(symSpread>0)
    minSpread=max(symSpread/pow(10,oDigits),inst->min_spread);

  minSpread=max(minSpread,0); // MT4 cannot handle less than zero spread
  if(oSpread<minSpread)
    {
    int iBid=static_cast<int>(round(finalBid*pow(10,oDigits))),
      iAsk=static_cast<int>(round(finalAsk*pow(10,oDigits))),
      iSpread=static_cast<int>(round(minSpread*pow(10,oDigits)));

    double midPrice=(iAsk+iBid)/2.0;
    double deltaSpread=iSpread/2.0;

    if(iSpread%2==(iAsk+iBid)%2)
      {
      iBid=static_cast<int>(round(midPrice-deltaSpread));
      iAsk=static_cast<int>(round(midPrice+deltaSpread));
      }
    else if(inst->bias_bid)
      {
      iBid=static_cast<int>(round(floor(midPrice)-ceil(deltaSpread)));
      iAsk=static_cast<int>(round(ceil(midPrice)+floor(deltaSpread)));
      }
    else
      {
      iBid=static_cast<int>(round(ceil(midPrice)-floor(deltaSpread)));
      iAsk=static_cast<int>(round(floor(midPrice)+ceil(deltaSpread)));
      }

    int newSpread=iAsk-iBid;
    if(iSpread>newSpread)
      {
      if(inst->bias_bid)
        iAsk++;
      else
        iBid--;
      }
    else if(iSpread<newSpread)
      {
      if(inst->bias_bid)
        iAsk--;
      else
        iBid++;
      }

    finalBid=iBid/pow(10,oDigits);
    finalAsk=iAsk/pow(10,oDigits);
    midPrice=midPrice/pow(10,oDigits);
    deltaSpread=deltaSpread/pow(10,oDigits);

    if(ExtPluginContext.SavePriceUpdates())
      LOG_DEBUG(pluginLog,"(MinSpread) '%s' Bid %.10g, Ask %.10g, Spread: %.10g, MinSpread: %.10g, SymSpread: %d, midPrice: %.10g, delta: %.10g, fBid: %.10g, fAsk: %.10g, fSpread: %.10g",
                inst->symbol,
                oBid,
                oAsk,
                oSpread,
                inst->min_spread,
                symSpread,
                midPrice,
                deltaSpread,
                finalBid,
                finalAsk,
                (finalAsk-finalBid));
    //---
    prices[0]=oBid-finalBid;
    prices[1]=finalAsk-oAsk;
    }
  //--- notify
  m_trade_dispatcher.OnSpreadReceived(inst->symbol,prices);
  //---
  if(finalAsk-finalBid<=inst->max_spread||inst->max_spread<=0.00)
    {
    //--- import symbol
    StringType::Copy(gateway_tick.symbol,_countof(gateway_tick.symbol),inst->symbol);
    //--- import price source
    StringType::Copy(gateway_tick.bank,_countof(gateway_tick.bank),"Integral");
    //--- import bid price
    gateway_tick.bid=finalBid;
    //--- import ask price
    gateway_tick.ask=finalAsk;
    //---
    return(true);
    }
  //---
  LOG_DEBUG(pluginLog,"Dropped Rate %s %.10g/%.10g - MaxSpread: %.10g",
            gateway_tick.symbol,
            finalBid,
            finalAsk,
            inst->max_spread);
  //---
  return(false);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
//bool CGateway::ProcessTick(const Instrument *symbol,SymbolTick &integral_tick,FeedTick &gateway_tick)
//  {
//  //double             oBid;
//  //double             oAsk;
//  //double      totalAmount;
//  //--- checks
//  if(!symbol/*||symbol->price_algo!=Instrument::EnPriceAlgo::BEST*/)
//    return(false);
//  //---
//  ZeroMemory(&gateway_tick,sizeof(FeedTick));
//  //---
//  gateway_tick.bid=integral_tick.bid*symbol->lot_conversion;
//  gateway_tick.ask=integral_tick.ask*symbol->lot_conversion;
//  //---
//  if(gateway_tick.bid<=0||gateway_tick.ask<=0)
//    {
//    LOG_DEBUG(pluginLog,"%.10g / %.10g",gateway_tick.bid,gateway_tick.ask);
//    return(false);
//    }
//  //---
//  //totalAmount=symbol->price_size;
//  //--
//  double finalBid=gateway_tick.bid-symbol->bid_spread;
//  double finalAsk=gateway_tick.ask+symbol->ask_spread;
//
//  int oDigits     =symbol->digits;
//  int symSpread   =symbol->spread;
//  double oSpread  =CGateway::RoundTo(finalAsk-finalBid,oDigits);
//  double minSpread=symbol->min_spread;
//
//  if(symSpread>0)
//    minSpread=max(symSpread/pow(10,oDigits),symbol->min_spread);
//
//  minSpread=max(minSpread,0); // MT4 cannot handle less than zero spread
//
//  if(oSpread<minSpread)
//    {
//    int iBid=static_cast<int>(round(finalBid*pow(10,oDigits))),
//      iAsk=static_cast<int>(round(finalAsk*pow(10,oDigits))),
//      iSpread=static_cast<int>(round(minSpread*pow(10,oDigits)));
//
//    double midPrice=(iAsk+iBid)/2.0,
//      deltaSpread=iSpread/2.0;
//
//    if(iSpread%2==(iAsk+iBid)%2)
//      {
//      iBid=static_cast<int>(round(midPrice-deltaSpread));
//      iAsk=static_cast<int>(round(midPrice+deltaSpread));
//      }
//    else if(symbol->bias_bid)
//      {
//      iBid=static_cast<int>(round(floor(midPrice)-ceil(deltaSpread)));
//      iAsk=static_cast<int>(round(ceil(midPrice)+floor(deltaSpread)));
//      }
//    else
//      {
//      iBid=static_cast<int>(round(ceil(midPrice)-floor(deltaSpread)));
//      iAsk=static_cast<int>(round(floor(midPrice)+ceil(deltaSpread)));
//      }
//
//    int newSpread=iAsk-iBid;
//    if(iSpread>newSpread)
//      {
//      if(symbol->bias_bid)
//        iAsk++;
//      else
//        iBid--;
//      }
//    else if(iSpread<newSpread)
//      {
//      if(symbol->bias_bid)
//        iAsk--;
//      else
//        iBid++;
//      }
//
//    finalBid=iBid/pow(10,oDigits);
//    finalAsk=iAsk/pow(10,oDigits);
//    midPrice=midPrice/pow(10,oDigits);
//    deltaSpread=deltaSpread/pow(10,oDigits);
//
//    /*if(ExtBridgeConfig.SavePriceUpdates())
//      LOG_DEBUG(pluginLog,"(MinSpread) '%s' Bid %.10g, Ask %.10g, Spread: %.10g, MinSpread: %.10g, SymSpread: %d, midPrice: %.10g, delta: %.10g, fBid: %.10g, fAsk: %.10g, fSpread: %.10g",sym.c_str(),oBid,oAsk,oSpread,inst.min_spread,symSpread,midPrice,deltaSpread,finalBid,finalAsk,(finalAsk-finalBid));
//
//    Spreads.set(sym,std::pair<double,double>(oBid-finalBid,finalAsk-oAsk));
//    }
//  else
//    Spreads.set(sym,std::pair<double,double>(0,0));*/
//    }
//
//  if(finalAsk-finalBid<=symbol->max_spread||symbol->max_spread<=0.00)
//    {
//    //--- import symbol
//    StringType::Copy(gateway_tick.symbol,_countof(gateway_tick.symbol),symbol->symbol);
//    //--- import price source
//    StringType::Copy(gateway_tick.bank,_countof(gateway_tick.bank),"Integral");
//    //--- import bid price
//    gateway_tick.bid=integral_tick.bid=finalBid;
//    //--- import ask price
//    gateway_tick.ask=integral_tick.ask=finalAsk;
//    //---
//    return(true);
//    }
//  else
//    {
//    LOG_ERROR(pluginLog,"%.10g / %.10g",finalBid,finalAsk);
//    }
//  //---
//  return(false);
//  }
//+------------------------------------------------------------------+
//| Calculate margin amount in base currency for symbol              |
//+------------------------------------------------------------------+
double CGateway::CalcMarginAmount(const ConSymbol *symbol,const SymbolPosition *position,const double market_price,const double leverage,const double percentage) const
  {
  double amount=0;
  double average_price,hedged_margin,unhedged_margin;
  int hedged_volume,unhedged_volume;
  //--- checks
  if(!symbol||!position)
    return(0);
  //--- calculate avg price
  average_price=position->price/(position->buy_volume+position->sell_volume);
  //--- lets calculate margin
  hedged_volume=2*min(position->buy_volume,position->sell_volume);
  unhedged_volume=(position->buy_volume+position->sell_volume-hedged_volume);
  //--- if haven't anything - calculate by calc mode
  switch(symbol->margin_mode)
    {
    //--- forex
    case MARGIN_CALC_FOREX:
      hedged_margin=(average_price*hedged_volume*symbol->margin_hedged)/leverage*percentage/100;
      unhedged_margin=(average_price*unhedged_volume*symbol->contract_size)/leverage*percentage/100;
      break;
      //--- cfd
    case MARGIN_CALC_CFD:
      hedged_margin=(average_price*hedged_volume*symbol->margin_hedged*market_price)*percentage/100;
      unhedged_margin=(average_price*unhedged_volume*symbol->contract_size*market_price)*percentage/100;
      break;
      //--- futures
    case MARGIN_CALC_FUTURES:
      //--- it should be calculated used margin_initial or margin_maintenance
      hedged_margin=(average_price*hedged_volume*symbol->margin_initial)*percentage/100;
      unhedged_margin=(average_price*unhedged_volume*symbol->margin_initial)*percentage/100;
      break;
      //--- cfd index
    case MARGIN_CALC_CFDINDEX:
      hedged_margin=(average_price*hedged_volume*symbol->margin_hedged*market_price*symbol->tick_size/symbol->tick_value)*percentage/100;
      unhedged_margin=(average_price*hedged_volume*symbol->contract_size*market_price*symbol->tick_size/symbol->tick_value)*percentage/100;
      break;
      //--- cfd leverage
    case MARGIN_CALC_CFDLEVERAGE:
      hedged_margin=(average_price*hedged_volume*symbol->margin_hedged*market_price)/leverage*percentage/100;
      unhedged_margin=(average_price*unhedged_volume*symbol->contract_size*market_price)/leverage*percentage/100;
      break;
    default:
      return(0);
    }
  //--- calculate margin
  amount=hedged_margin+unhedged_margin;
  //--- normalize and return result
  return(SMTMath::PriceNormalize(amount,2));
  }
//+------------------------------------------------------------------+
//| order side BUY                                                   |
//+------------------------------------------------------------------+
bool CGateway::OrderIsBuy(const short cmd) const
  {
  //---
  switch(cmd)
    {
    case OP_BUY:
    case OP_BUY_LIMIT:
    case OP_BUY_STOP:
      return(true);
    default:
      break;
    }
  //---
  return(false);
  }
//+------------------------------------------------------------------+
//| rounding function                                                |
//+------------------------------------------------------------------+
inline double CGateway::RoundTo(double x,double prec)
  {
  return round(x*pow(10,prec))/pow(10,prec);
  }
//+------------------------------------------------------------------+
//| split                                                            |
//+------------------------------------------------------------------+
inline double CGateway::ProcessPrice(const IntegralBook &book,const UINT type,double &total_amout,const Instrument *inst)
  {
  double amountLeft=total_amout;
  double newPrice=0;
  double usedAmount=0;
  double lastPrice=0;
  //---
  for(UINT pos=0;pos<book.items_total;pos++)
    {
    if(book.items[pos].type!=type) continue;
    double availableVolume=book.items[pos].size;

    if(availableVolume>amountLeft)
      availableVolume=amountLeft;

    newPrice+=(availableVolume*(lastPrice=book.items[pos].price));
    usedAmount+=availableVolume;
    amountLeft-=availableVolume;

    if(inst->price_algo==Instrument::EnPriceAlgo::BEST&&newPrice>0)
      {
      total_amout=usedAmount; // Set Total = Used to avoid further calculations
      break;
      }
    else if(inst->price_algo==Instrument::EnPriceAlgo::DEPTH&&book.items[pos].size>=total_amout)
      {
      newPrice=book.items[pos].price*usedAmount; // Set price = Tier price * amount (so it can later be divided by same number)
      total_amout=usedAmount; // Set Total = Used to avoid further calculations
      break;
      }
    }
  //---
  if(usedAmount<total_amout)
    {
    amountLeft=total_amout-usedAmount;
    newPrice+=(amountLeft*lastPrice);
    usedAmount+=amountLeft;
    }
  //---
  return(newPrice/total_amout)*inst->lot_conversion;
  }
//+------------------------------------------------------------------+
//| split                                                            |
//+------------------------------------------------------------------+
//void CGateway::split(const std::string &str,const std::string &delim,std::vector<std::string> &output,int MaxLength)
//  {
//  unsigned int offset=0;
//  unsigned int delimIndex=0;
//
//  delimIndex=str.find(delim,offset);
//  while(delimIndex!=std::string::npos)
//    {
//    if(output.size()==MaxLength-1)
//      break;
//
//    output.push_back(str.substr(offset,delimIndex-offset));
//    offset+=delimIndex-offset+delim.length();
//    delimIndex=str.find(delim,offset);
//    }
//
//  output.push_back(str.substr(offset));
//  }
//+------------------------------------------------------------------+
//| Import symbol data                                               |
//+------------------------------------------------------------------+
//bool CGateway::ImportSymbol(const ExchangeSymbol &exchange_symbol,IMTConSymbol *gateway_symbol) const
//  {
//  bool res=false;
//  //--- check
//  if(!gateway_symbol)
//	 return(false);
//  //--- symbol name
//  res=gateway_symbol->Symbol(exchange_symbol.symbol)==MT_RET_OK;
//  //--- path
//  res=res&&gateway_symbol->Path(exchange_symbol.path)==MT_RET_OK;
//  //--- description
//  res=res&&gateway_symbol->Description(exchange_symbol.description)==MT_RET_OK;
//  //--- page
//  res=res&&gateway_symbol->Page(exchange_symbol.page)==MT_RET_OK;
//  //--- base currency
//  res=res&&gateway_symbol->CurrencyBase(exchange_symbol.currency_base)==MT_RET_OK;
//  //--- profit currency
//  res=res&&gateway_symbol->CurrencyProfit(exchange_symbol.currency_profit)==MT_RET_OK;
//  //--- margin currency
//  res=res&&gateway_symbol->CurrencyMargin(exchange_symbol.currency_margin)==MT_RET_OK;
//  //--- digits
//  res=res&&gateway_symbol->Digits(exchange_symbol.digits)==MT_RET_OK;
//  //--- tick flags
//  res=res&&gateway_symbol->TickFlags(exchange_symbol.tick_flags)==MT_RET_OK;
//  //--- calc mode
//  res=res&&gateway_symbol->CalcMode(exchange_symbol.calc_mode)==MT_RET_OK;
//  //--- exec mode
//  res=res&&gateway_symbol->ExecMode(exchange_symbol.exec_mode)==MT_RET_OK;
//  //--- chart mode
//  res=res&&gateway_symbol->ChartMode(exchange_symbol.chart_mode)==MT_RET_OK;
//  //--- fill flags
//  res=res&&gateway_symbol->FillFlags(exchange_symbol.fill_flags)==MT_RET_OK;
//  //--- expiration flags
//  res=res&&gateway_symbol->ExpirFlags(exchange_symbol.expir_flags)==MT_RET_OK;
//  //--- tick value
//  res=res&&gateway_symbol->TickValue(exchange_symbol.tick_value)==MT_RET_OK;
//  //--- tick size
//  res=res&&gateway_symbol->TickSize(exchange_symbol.tick_size)==MT_RET_OK;
//  //--- contract size
//  res=res&&gateway_symbol->ContractSize(exchange_symbol.contract_size)==MT_RET_OK;
//  //--- min volume
//  res=res&&gateway_symbol->VolumeMin(SMTMath::VolumeToInt(exchange_symbol.volume_min))==MT_RET_OK;
//  //--- max volume
//  res=res&&gateway_symbol->VolumeMax(SMTMath::VolumeToInt(exchange_symbol.volume_max))==MT_RET_OK;
//  //--- step volume
//  res=res&&gateway_symbol->VolumeStep(SMTMath::VolumeToInt(exchange_symbol.volume_step))==MT_RET_OK;
//  //--- margin flags
//  res=res&&gateway_symbol->MarginFlags(exchange_symbol.margin_flags)==MT_RET_OK;
//  //--- market depth
//  res=res&&gateway_symbol->TickBookDepth(exchange_symbol.market_depth)==MT_RET_OK;
//  //--- margin initial
//  if(exchange_symbol.margin_initial>0)
//	 res=res&&gateway_symbol->MarginInitial(exchange_symbol.margin_initial)==MT_RET_OK;
//  //--- margin maintenance
//  if(exchange_symbol.margin_maintenance>0)
//	 res=res&&gateway_symbol->MarginMaintenance(exchange_symbol.margin_maintenance)==MT_RET_OK;
//  //--- long margin
//  res=res&&gateway_symbol->MarginLong(exchange_symbol.margin_long)==MT_RET_OK;
//  //--- short margin
//  res=res&&gateway_symbol->MarginShort(exchange_symbol.margin_short)==MT_RET_OK;
//  //--- margin limit
//  if(exchange_symbol.margin_limit>0)
//	 res=res&&gateway_symbol->MarginLimit(exchange_symbol.margin_limit)==MT_RET_OK;
//  //--- margin stop
//  if(exchange_symbol.margin_stop>0)
//	 res=res&&gateway_symbol->MarginStop(exchange_symbol.margin_stop)==MT_RET_OK;
//  //--- margin stop limit
//  if(exchange_symbol.margin_stop_limit>0)
//	 res=res&&gateway_symbol->MarginStopLimit(exchange_symbol.margin_stop_limit)==MT_RET_OK;
//  //--- settlement price
//  if(exchange_symbol.settlement_price>0)
//	 res=res&&gateway_symbol->PriceSettle(exchange_symbol.settlement_price)==MT_RET_OK;
//  //--- price limit max
//  if(exchange_symbol.price_limit_max>0)
//	 res=res&&gateway_symbol->PriceLimitMax(exchange_symbol.price_limit_max)==MT_RET_OK;
//  //--- price limit min
//  if(exchange_symbol.price_limit_min>0)
//	 res=res&&gateway_symbol->PriceLimitMin(exchange_symbol.price_limit_min)==MT_RET_OK;
//  //--- time start
//  if(exchange_symbol.time_start!=0)
//	 res=res&&gateway_symbol->TimeStart(exchange_symbol.time_start)==MT_RET_OK;
//  //--- time expiration
//  if(exchange_symbol.time_expiration!=0)
//	 res=res&&gateway_symbol->TimeExpiration(exchange_symbol.time_expiration)==MT_RET_OK;
//  //--- trade mode
//  res=res&&gateway_symbol->TradeMode(exchange_symbol.trade_mode)==MT_RET_OK;
//  //--- return result
//  return(res);
//  }
//+------------------------------------------------------------------+
//| Update external account for client                               |
//+------------------------------------------------------------------+
//MTAPIRES CGateway::UpdateExternalAccount(const IMTRequest *request) const
//  {
//  MTAPIRES res=MT_RET_OK_NONE;
//  //--- check if external account already equals to MT login
//  if(_wtoi64(request->ExternalAccount())==request->Login())
//	 return(MT_RET_OK_NONE);
//  //--- check API
//  if(!m_api_server||!m_api_gateway_config)
//	 return(MT_RET_ERROR);
//  //--- format external account
//  CMTStr32 external_account;
//  external_account.Format(L"%I64u",request->Login());
//  //--- get user account interface
//  IMTUser *user_gateway=m_api_server->UserCreate();
//  //--- check interface      
//  if(!user_gateway)
//	 return(MT_RET_ERR_MEM);
//  //--- clear interface
//  user_gateway->Clear();
//  //--- set MT login
//  res=user_gateway->Login(request->Login());
//  //--- set external account
//  if(res==MT_RET_OK)
//	 res=user_gateway->ExternalAccountAdd(m_api_gateway_config->ID(),external_account.Str());
//  //--- update user
//  if(res==MT_RET_OK)
//	 res=m_api_server->GatewayAccountSet(0,user_gateway,NULL,NULL,NULL);
//  //--- release interface
//  if(user_gateway)
//	 user_gateway->Release();
//  //--- log if success
//  if(res==MT_RET_OK)
//	 ExtLogger.Out(MTLogErr,L"'%I64u': external account '%s' was set for dealer '%I64u'",request->Login(),external_account.Str(),m_api_gateway_config->ID());
//  //--- return result
//  return(res);
//  }
//+------------------------------------------------------------------+
//| Convert orders to Gateway API format                             |
//+------------------------------------------------------------------+
//MTAPIRES CGateway::ConvertOrders(const ExchangeOrdersArray &exchange_orders,IMTOrderArray *gateway_orders) const
//  {
//  IMTOrder *gateway_order=NULL;
//  ExchangeSymbol exchange_symbol;
//  CMTStr32       str;
//  MTAPIRES       res=MT_RET_OK;
//  //--- check
//  if(!m_api_server||!gateway_orders)
//	 return(false);
//  //--- go through all orders
//  for(UINT i=0;i<exchange_orders.Total()&&res==MT_RET_OK;i++)
//	 {
//	 //--- skip empty orders
//	 if(exchange_orders[i].volume==0)
//		continue;
//	 //--- get Gateway API order interface
//	 if((gateway_order=m_api_server->OrderCreate())!=NULL)
//		{
//		//--- set order ticket
//		res=gateway_order->OrderSet(exchange_orders[i].order_mt_id);
//		//--- format order external id
//		str.Format(L"%I64u",exchange_orders[i].order_exchange_id);
//		//--- set order external id
//		if(res==MT_RET_OK)
//		  res=gateway_order->ExternalID(str.Str());
//		//--- set symbol of order
//		if(res==MT_RET_OK)
//		  res=gateway_order->Symbol(exchange_orders[i].symbol);
//		//--- set order volume
//		if(res==MT_RET_OK)
//		  res=gateway_order->VolumeInitial(SMTMath::VolumeToInt((double)exchange_orders[i].volume));
//		if(res==MT_RET_OK)
//		  res=gateway_order->VolumeCurrent(SMTMath::VolumeToInt((double)exchange_orders[i].volume));
//		//--- set order prices
//		if(res==MT_RET_OK)
//		  res=gateway_order->PriceOrder(exchange_orders[i].price_order);
//		if(res==MT_RET_OK)
//		  res=gateway_order->PriceSL(exchange_orders[i].price_SL);
//		if(res==MT_RET_OK)
//		  res=gateway_order->PriceTP(exchange_orders[i].price_TP);
//		//--- set order type
//		if(res==MT_RET_OK)
//		  res=gateway_order->Type(exchange_orders[i].type_order);
//		//--- set order expiration type and time
//		if(res==MT_RET_OK)
//		  res=gateway_order->TypeTime(exchange_orders[i].type_time);
//		if(res==MT_RET_OK)
//		  res=gateway_order->TimeExpiration(exchange_orders[i].expiration_time);
//		//--- set activation flags for exchange execution
//		if(res==MT_RET_OK&&m_api_integral->SymbolGet(exchange_orders[i].symbol,exchange_symbol)&&exchange_symbol.exec_mode==ExchangeSymbol::EXECUTION_EXCHANGE)
//		  res=gateway_order->ActivationFlags(IMTOrder::ACTIV_FLAGS_NO_EXPIRATION|IMTOrder::ACTIV_FLAGS_NO_LIMIT);
//		//--- put login to comment
//		str.Format(L"%I64u",exchange_orders[i].login);
//		if(res==MT_RET_OK)
//		  res=gateway_order->Comment(str.Str());
//		//--- set order setup time
//		if(res==MT_RET_OK)
//		  res=gateway_order->TimeSetup(m_api_server->TimeCurrent());
//		//--- add order to array
//		if(res==MT_RET_OK)
//		  res=gateway_orders->Add(gateway_order);
//		else
//		  gateway_order->Release();
//		}
//	 else
//		res=MT_RET_ERR_MEM;
//	 }
//  //--- return result
//  return(res);
//  }
//+------------------------------------------------------------------+
//| Convert positions to Gateway API format                          |
//+------------------------------------------------------------------+
//MTAPIRES CGateway::ConvertPositions(const ExchangePositionsArray &exchange_positions,IMTPositionArray *gateway_positions) const
//  {
//  IMTPosition *gateway_position=NULL;
//  MTAPIRES       res=MT_RET_OK;
//  //--- check
//  if(!m_api_server||!gateway_positions)
//	 return(false);
//  //--- go through all positions
//  for(UINT i=0;i<exchange_positions.Total()&&res==MT_RET_OK;i++)
//	 {
//	 //--- skip empty positions
//	 if(exchange_positions[i].volume==0)
//		continue;
//	 //--- get Gateway API position interface
//	 if((gateway_position=m_api_server->PositionCreate())!=NULL)
//		{
//		//--- set position symbol
//		if(res==MT_RET_OK)
//		  res=gateway_position->Symbol(exchange_positions[i].symbol);
//		//--- set position volume
//		if(res==MT_RET_OK)
//		  gateway_position->Volume(SMTMath::VolumeToInt((double)abs(exchange_positions[i].volume)));
//		//--- set position action
//		if(res==MT_RET_OK)
//		  {
//		  if(exchange_positions[i].volume>0)
//			 res=gateway_position->Action(IMTPosition::POSITION_BUY);
//		  else
//			 res=gateway_position->Action(IMTPosition::POSITION_SELL);
//		  }
//		//--- set position open price
//		if(res==MT_RET_OK)
//		  res=gateway_position->PriceOpen(exchange_positions[i].price);
//		//--- write login to the comment
//		if(res==MT_RET_OK)
//		  {
//		  CMTStr32 comment;
//		  comment.Format(L"%I64u",exchange_positions[i].login);
//		  res=gateway_position->Comment(comment.Str());
//		  }
//		//--- set digits amount
//		if(res==MT_RET_OK)
//		  res=gateway_position->Digits(exchange_positions[i].digits);
//		//--- add position to array
//		if(res==MT_RET_OK)
//		  res=gateway_positions->Add(gateway_position);
//		else
//		  gateway_position->Release();
//		}
//	 else
//		res=MT_RET_ERR_MEM;
//	 }
//  //--- return result
//  return(res);
//  }
//+------------------------------------------------------------------+
