//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#include "StdAfx.h"
#include "Bases\Gateway.h"
#include "MetaQuotes\MT4Helper.h"
#include "Reports\Reports.h"
#include "Common\Helpers.h"
//+------------------------------------------------------------------+
//| Module handle                                                    |
//+------------------------------------------------------------------+
HANDLE           ExtModule=NULL;
//+------------------------------------------------------------------+
//| plugin description                                               |
//+------------------------------------------------------------------+
PluginInfo ExtPluginInfo={"MT4.Bridge",VERSION_FULL,"\251 2018-2021 Integral Development Corp.",{0}};
//+------------------------------------------------------------------+
//| Global objects                                                   |
//+------------------------------------------------------------------+
CServerInterface *ExtServer=NULL;
UserInfo		      ExtManagerInfo={0};
char              ExtProgramPath[MAX_PATH]="";
//+------------------------------------------------------------------+
//| DLL entry point                                                  |
//+------------------------------------------------------------------+
BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
  {
  StringTypePath path;
  int pos;
  //---
  switch(ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
      ExtModule=hModule;
      DebugUtil::CUnhandledExceptionFilter::idcModule=ExtModule;
      //--- current folder
      GetModuleFileName((HMODULE)hModule,path.Buffer(),path.Max());
      path.Refresh();
      if((pos=path.FindRChar('\\'))>0)
        path.Trim(pos);
      //--- remeber gateway path
      StringType::Copy(ExtProgramPath,path.Str());
      break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
      break;

    case DLL_PROCESS_DETACH:
      /*if(ExtGateway)
        {
        ExtGateway->Shutdown();
        delete ExtGateway;
        ExtGateway=NULL;
        }*/
      break;
    }
  //---
  return(TRUE);
  }
/********************************************************************************************************************************************\
|	Purpose - getting the plugin information.
|	  The hook is called by the server during the plugin loading and is intended for getting the plugin information shown in the MetaTrader
|	  Administrator. The hook is called as the very first, before any other hooks.
|
|	Parameters:
|	  info - plugin description to be filled out when the hook is called
\********************************************************************************************************************************************/
void APIENTRY iAbout(PluginInfo *info)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;
  //---
  StringType512 temp;
  //--- checks
  if(!info)
    {
    LOG_ERROR(pluginLog,"iAbout called with a NULL pointer");
    return;
    }
  //---
  memcpy(info,&ExtPluginInfo,sizeof(PluginInfo));
  //---
  temp.Format("iAbout function returning name %s, version %d.%d.%d, copyright %s",
              ExtPluginInfo.name,
              VERSION_MAJOR,
              VERSION_MINOR,VERSION_BUILD,
              ExtPluginInfo.copyright);
  //---  show
  LOG_JOURNAL(pluginLog,temp.Str());
  }

/********************************************************************************************************************************************\
|	Purpose - plugin initialization and call for the server interface.
|	  The hook is called after the plugin is loaded into the memory to provide it with the pointer to the server interface.
|
|	Parameters:
|	  server - server interface
|
|	Important: The pointer to the server interface is passed to the plugin only once when hook MtSrvStartup is called.
\********************************************************************************************************************************************/
int APIENTRY iStartup(CServerInterface *server)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;
  //---
  StringType512 temp;
  //---
  temp.Format("MtSrvStartup for '%s' version %d.%d.%d, copyright %s",
              ExtPluginInfo.name,
              VERSION_MAJOR,
              VERSION_MINOR,
              VERSION_BUILD,
              ExtPluginInfo.copyright);
  //---
  LOG_INFO(pluginLog,temp.Str());
  //---- checks
  if(server==NULL) { LOG_ERROR(pluginLog,"server is NULL"); return FALSE; }
  //--- check version
  if(server->Version()!=ServerApiVersion)
    {
    LOG_ERROR(pluginLog,"API version of server is %d, I can work with %d only",
              server->Version(),
              (int)ServerApiVersion);
    //---
    return FALSE;
    }
  //--- remember
  ExtServer=server;
  //--- initialize logger
  ExtLogger.SetGatewayAPI(server);
  //--- create gateway object
  if(!ExtGateway)
    if((ExtGateway=new(std::nothrow) CGateway())==NULL)
      {
      LOG_ERROR(pluginLog,"gateway create error");
      return(FALSE);
      }
  //---
  if((pluginLog=new(std::nothrow) CLog())==NULL)
    {
    LOG_ERROR(pluginLog,"pluginLog create error");
    return(FALSE);
    }
  //--- initialize
  if(!pluginLog->Initialize("IDC.MT4"))
    return(FALSE);
  //--- start
  pluginLog->StartQueue();
  //---
  if((orderLog=new(std::nothrow) CLog())==NULL)
    {
    LOG_ERROR(pluginLog,"orderlog create error");
    return(FALSE);
    }
  //--- start cloud watch
  if(!ExtCloudWatch.Initialize(pluginLog))
    return(FALSE);
  //--- initialize reports
  if(!ExtReports.Initialize())
    return(FALSE);
  //--- initialize
  if(!orderLog->Initialize("IDC.Orders"))
    return(FALSE);
  //---
  ExtLogger.Out(CmdOK,temp.Str());
  //--- start
  orderLog->StartQueue();
  //---
  LOG_DEBUG(pluginLog,"------------------------------------ Starting MT4 Logs ------------------------------------");
  LOG_INFO(pluginLog,"ExtProgramPath=%s",ExtProgramPath);
  //--- initalize gateway
  LOG_INFO(pluginLog,"initalizing gateway");
  if(!ExtGateway->Initialize())
    {
    LOG_ERROR(pluginLog,"gateway initialization error");
    return(FALSE);
    }
  //---- fill user info
  ZeroMemory(&ExtManagerInfo,sizeof(ExtManagerInfo));
  ExtManagerInfo.login=ExtPluginContext.TradingDealer();
  StringType::Copy(ExtManagerInfo.name,"IDC Virtual Dealer");
  StringType::Copy(ExtManagerInfo.ip,"MT4.Bridge");
  //--- reload parameters
  //ExtPluginContext.Reload();

  //---
 /* if(!ExtHealthMonitor)
    {
    LOG_ERROR(pluginLog,"health monitor failed");
    }
  else
    ExtHealthMonitor->Start(server,pluginLog);*/
    // set config file name
    //ExtBridgeConfig.SetFilename(ExtModule);
  //--- show
  LOG_INFO(pluginLog,"Startup version %s",VERSION_STRING);
  //----
  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - plugin deinitialization.
|	  The hook is called immediately before the plugin is dicarded from the memory.
|
|	Important: The server interface methods may not be reffered to after this hook has been called.
\********************************************************************************************************************************************/
void APIENTRY iCleanup(void)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;

  //--- delete gateway pointer
  if(ExtGateway)
    {
    //---
    delete ExtGateway;
    ExtGateway=NULL;
    }
  //--- clear pluginlog
  if(pluginLog)
    {
    LOG_INFO(pluginLog,"MtSrvCleanup complete");
    LOG_DEBUG(pluginLog,"------------------------------------ Stopping MT4 Logs ------------------------------------\r\n");

    delete pluginLog;
    pluginLog=NULL;
    }
  //--- clear order log
  if(orderLog)
    {
    delete orderLog;
    orderLog=NULL;
    }
  //---
  ExtLogger.Out(CmdOK,"Shutdown");
  //--- update the gateway instance in logger
  ExtLogger.SetGatewayAPI(NULL);
  //---
  ExtServer=NULL;
  }

//+------------------------------------------------------------------+
//| Standard configuration functions                                 |
//+------------------------------------------------------------------+
int APIENTRY iPluginCfgAdd(const PluginCfg *cfg)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;
  //--- checks
  if(!cfg)
    return(FALSE);
  //---
  if(ExtPluginContext.Add(0,cfg)==TRUE)
    {
    ExtLogger.Out(CmdErr,"parameter added name:%s value:%s",cfg->name,cfg->value);
    return(TRUE);
    }
  //---
  ExtLogger.Out(CmdErr,"parameter added name:%s value:%s failed",cfg->name,cfg->value);
  //---
  return (FALSE);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int APIENTRY iPluginCfgSet(const PluginCfg *values,const int total)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;
  //--- checks
  if(!values||total<1)
    return(FALSE);
  //--- set and reload parameters
  if(ExtPluginContext.Set(values,total)==TRUE)
    if(ExtPluginContext.Initialize())
      {
      ExtLogger.Out(CmdErr,"%d parameters reloaded",total);
      return(TRUE);
      }
  //---
  ExtLogger.Out(CmdErr,"reloading parameters failed");
  //---
  return (FALSE);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int APIENTRY iPluginCfgGet(LPCSTR name,PluginCfg *cfg)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;
  //--- checks
  if(!cfg||!name)
    return(FALSE);
  //---
  if(ExtPluginContext.Get(name,cfg)==TRUE)
    {
    ExtLogger.Out(CmdErr,"'%s' plugin config get processed",name);
    return(TRUE);
    }
  //---
  ExtLogger.Out(CmdErr,"'%s' plugin config get failed",name);
  //---
  return(FALSE);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int APIENTRY iPluginCfgNext(const int index,PluginCfg *cfg)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;
  //---
  if(ExtPluginContext.Next(index,cfg)==FALSE) return(FALSE);
  //---
  ExtLogger.Out(CmdOK,"'%d' plugin config next processed [%s: %s]",index,cfg->name,cfg->value);
  return(TRUE);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int APIENTRY iPluginCfgDelete(LPCSTR name)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;
  //--- checks
  if(!name)
    return(FALSE);
  //---
  if(ExtPluginContext.Delete(name)==TRUE)
    {
    ExtLogger.Out(CmdErr,"'%s' plugin config parameter deleted",name);
    return(TRUE);
    }
  //---
  ExtLogger.Out(CmdErr,"'%s' plugin config parameter delete failed",name);
  //---
  return(FALSE);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int APIENTRY iPluginCfgTotal()
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;
  //--- return total parameters
  return ExtPluginContext.Total();
  }
/********************************************************************************************************************************************\
|	Purpose - interception of the symbol adding/updating.
|	  The hook is called before one attempts to add/update a symbol.
|
|	Parameters:
|	  symb - description of the symbol
|
|	Return values:
|	  TRUE - default value - allow adding/updating of the symbol
|	  FALSE - disable adding/updating of the symbol
\********************************************************************************************************************************************/
int APIENTRY iSymbolsAdd(const ConSymbol *symb)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;
  //--- checks
  if(!symb||!ExtGateway)
    return(FALSE);
  //--- TODO:
  if(!ExtGateway->UpdateSymbolCache(symb))
    {
    LOG_ERROR(pluginLog,"Symbol Update '%s' failed",symb->symbol);
    return(FALSE);
    }
  //--- show
  LOG_INFO(pluginLog,"Symbol Updated '%s'",symb->symbol);
  //--- successful
  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - notify about adding a new account to the clients database.
|	  The server calls this hook after a new account has been successfully added to the clients database. This hook can be used to export
|	  accounts in the real-time mode.
|
|	Parameters:
|	  info - account description
\********************************************************************************************************************************************/
void APIENTRY iUserAdd(const UserRecord *info)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;
  //--- checks
  if(!info||!ExtGateway)
    return;
  //--- show
  LOG_INFO(pluginLog,"User Add '%d'",info->login);
  //---
  if(ExtPluginContext.AdminIntegration())
    if(!ExtGateway->AdminIntegration(info))
      {
      LOG_ERROR(pluginLog,"'%d' user add send failed",info->login);
      }
  }

/********************************************************************************************************************************************\
|	Purpose - notify about account changing.
|	  The server calls this hook after an account has been successfully changed in the clients database. The hook can be used to export
|	  clients' accounts in the real-time mode.
|
|	Parameters:
|	  info - account description
\********************************************************************************************************************************************/
void APIENTRY iUserUpdate(const UserRecord *info)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;
  //--- checks
  if(!info||!ExtGateway)
    return;
  //--- show
  LOG_INFO(pluginLog,"'%d' User Update",info->login);
  //---
  if(ExtPluginContext.AdminIntegration())
    if(!ExtGateway->AdminIntegration(info))
      {
      LOG_ERROR(pluginLog,"'%d' user update send failed",info->login);
      }
  }

/********************************************************************************************************************************************\
|	Purpose - filter connections by clients.
|	  The hook is called after the client has successfully authorized by the server. This hook can be used to filter the client's access to
|	  the server on basis of the own internal data.
|
|	Parameters:
|	  info - client info
|	  type - identify the type of the terminal connected
|
|	Return values:
|	  TRUE - default value - allow login for this client
|	  FALSE - block the client's login, return "account disabled" (RET_ACCOUNT_DISABLED), and close the connection
\********************************************************************************************************************************************/
int APIENTRY iUserLoginExt(const UserInfo *info,const UCHAR type)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;
  //--- 
  if(info)
    {
    //---
    LOG_INFO(pluginLog,"'%d': login (%s, %s)",info->login,info->ip,SIntegralFormat::FormatLoginType(type));
    }
  //--- ok
  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - additional check of trading transactions
|	  The hook named MtSrvTradeTransaction is called after the client's or manager's trading transaction (position opening or closing, order
|	  modifying, pending order triggering, etc.) has been primarily checked.
|	  If this hook does not return RET_OK, the transaction will not be performed. If the operation succeeds, the number of the request that
|	  conducted this transaction will be placed in the request_id. Otherwise, the return code will be passed to the client as an error code.
|
|	Parameters:
|	  trans - transaction
|	  user - description of the trader's or manager's account that performs a transaction
|	  request_id - the request number
\********************************************************************************************************************************************/
int APIENTRY iTradeTransaction(TradeTransInfo *trans,const UserInfo *user,int *request_id)
  {
  TradeTransInfo info={0};
  RequestInfo    request={0};
  UserRecord     record={0};
  ConGroup       group={0};
  ConSymbol      symbol={0};
  TradeRecord    trade={0};
  int            id,login;
  //--- smthg wrong dont touch
  if(!trans||!user||!ExtGateway||!ExtServer)
    return(RET_INVALID_DATA);
  //--- must be local copy
  info=*trans;
  //---
  if(info.type==TT_ORDER_IE_OPEN||info.type==TT_ORDER_MK_OPEN||info.type==TT_BR_ORDER_OPEN)
    trans->order=0; // ESF-2281
  //---
  switch(info.type)
    {
    case TT_ORDER_IE_OPEN:
    case TT_ORDER_MK_OPEN:
      //--- lets process
      return(RET_OK);

      // open pending order
    case TT_ORDER_PENDING_OPEN:
      //--- confirm
      return(RET_OK);

    case TT_ORDER_IE_CLOSE:                            // close position at  Instant Execution
    case TT_ORDER_MK_CLOSE:                            // close position at  Market Execution
      //--- set login
      login=user->login;
      //--- is it ours?
      if(!ExtGateway->CheckGroupAndSymbol(login,info.symbol))
        return(RET_OK);
      //--- in process?
      if(ExtGateway->CheckRequestByOrder(info.order))
        return(RET_TRADE_ORDER_LOCKED);
      //--- succesfull
      return(RET_OK);

      //--- client activity
    case TT_ORDER_MODIFY:                              // modify position or pending order
    case TT_ORDER_DELETE:                              // delete pending order
      //--- set login
      login=user->login;
      //--- is it ours?
      if(!ExtGateway->CheckGroupAndSymbol(login,info.symbol))
        return(RET_OK);
      //--- in process?
      if(ExtGateway->CheckRequestByOrder(info.order))
        return(RET_TRADE_ORDER_LOCKED);
      //--- succesfull
      return(RET_OK);

      //--- close hedged positions
    case TT_ORDER_CLOSE_BY:
      //--- set login
      login=user->login;
      //--- is it ours?
      if(!ExtGateway->CheckGroupAndSymbol(login,info.symbol))
        return(RET_OK);
      //---
      if(ExtGateway->CheckRequestByOrder(info.order,info.orderby))
        return(RET_TRADE_ORDER_LOCKED);
      //--- confirm
      return(RET_OK);

      //--- close all hedged positions for same security
    case TT_ORDER_CLOSE_ALL:
      //--- set login
      login=user->login;
      //--- is it ours?
      if(!ExtGateway->CheckGroupAndSymbol(login,info.symbol))
        return(RET_OK);
      //---
      if(ExtGateway->CheckMultipleCloseBy(user->login,info.symbol))
        return(RET_TRADE_ORDER_LOCKED);
      //--- confirm
      return(RET_OK);

      //--- open order (by manager)
    case TT_BR_ORDER_OPEN:
      //--- check stp manager
      if(!ExtPluginContext.CheckManagerSTP(user->login))
        return(RET_OK);
      //--- set login
      login=info.orderby;
      //--- is it ours?
      if(!ExtGateway->CheckGroupAndSymbol(login,info.symbol))
        return(RET_OK);
      //--- only process buy/sell
      if(info.cmd>OP_SELL)
        return(RET_OK);
      //--- process
      break;

      //--- activate pending order (by manager)
    case TT_BR_ORDER_ACTIVATE:
      //--- find login
      if((login=ExtServer->TradesFindLogin(info.order))<1)
        return(RET_BAD_ACCOUNT_INFO);
      //--- is it ours?
      if(!ExtGateway->CheckGroupAndSymbol(login,info.symbol))
        return(RET_OK);
      //--- check
      if(ExtGateway->CheckRequestByOrder(info.order))
        return(RET_TRADE_ORDER_LOCKED);
      //--- check stp manager
      if(!ExtPluginContext.CheckManagerSTP(user->login))
        return(RET_OK);
      //---
      switch(info.cmd)
        {
        case OP_BUY:
        case OP_BUY_LIMIT:
        case OP_BUY_STOP:
          trans->cmd=OP_BUY;
          break;
        case OP_SELL:
        case OP_SELL_LIMIT:
        case OP_SELL_STOP:
          trans->cmd=OP_SELL;
          break;
        default:
          return(RET_INVALID_DATA);
        }
      //---
      break;

    case TT_BR_ORDER_DELETE:                           // delete ANY open position
    case TT_BR_ORDER_MODIFY:                           // modify position or pending order (by manager)
      //--- find login
      if((login=ExtServer->TradesFindLogin(info.order))<1)
        return(RET_BAD_ACCOUNT_INFO);
      //--- is it ours?
      if(!ExtGateway->CheckGroupAndSymbol(login,info.symbol))
        return(RET_OK);
      //--- check
      if(ExtGateway->CheckRequestByOrder(info.order))
        return(RET_TRADE_ORDER_LOCKED);
      //--- confirm
      return(RET_OK);

      //--- close order (by manager)
    case TT_BR_ORDER_CLOSE:
      //--- find login
      if((login=ExtServer->TradesFindLogin(info.order))<1)
        return(RET_BAD_ACCOUNT_INFO);
      //--- is it ours?
      if(!ExtGateway->CheckGroupAndSymbol(login,info.symbol))
        return(RET_OK);
      //--- check
      if(ExtGateway->CheckRequestByOrder(info.order))
        return(RET_TRADE_ORDER_LOCKED);
      //--- check stp manager
      if(!ExtPluginContext.CheckManagerSTP(user->login))
        return(RET_OK);
      //---
      break;

      //--- close hedged positions (by manager)
    case TT_BR_ORDER_CLOSE_BY:
      //--- find login
      if((login=ExtServer->TradesFindLogin(info.order))<1||
         (login=ExtServer->TradesFindLogin(info.orderby))<1)
        return(RET_BAD_ACCOUNT_INFO);
      //--- is it ours?
      if(!ExtGateway->CheckGroupAndSymbol(login,info.symbol))
        return(RET_OK);
      //--- check
      if(ExtGateway->CheckRequestByOrder(info.order,info.orderby))
        return(RET_TRADE_ORDER_LOCKED);
      //--- confirm
      return(RET_OK);

      //--- close all hedged positions for same security (by manager)
    case TT_BR_ORDER_CLOSE_ALL:
      //--- find login
      if((login=ExtServer->TradesFindLogin(info.order))<1)
        return(RET_BAD_ACCOUNT_INFO);
      //--- is it ours?
      if(!ExtGateway->CheckGroupAndSymbol(login,info.symbol))
        return(RET_OK);
      //--- check
      if(ExtGateway->CheckMultipleCloseBy(login,info.symbol))
        return(RET_TRADE_ORDER_LOCKED);
      //--- confirm
      return(RET_OK);

      //--- skip
    case TT_BR_ORDER_COMMENT:                          // comment order (by manager)
    case TT_BR_BALANCE:                                // change balance & credit (by manager)
      return(RET_OK);

      //--- skip
    default:
      return(RET_OK);
    }
  //--- fill request structure for manager request
  if(ExtServer->ClientsUserInfo(login,&record)==FALSE)
    return(RET_BAD_ACCOUNT_INFO);
  //---
  request.login=login;
  StringType::Copy(request.group,record.group);
  request.balance=record.balance;
  request.credit=record.credit;
  request.trade.cmd=trans->cmd;
  request.trade.type=trans->type;
  request.manager=user->login;
  //request.reserved      =33;
  //--- set time
  request.time=OURTIME(ExtServer->TradeTime());
  //--- set
  request.trade=*trans;
  //--- add requote to request queue
  return(ExtServer->RequestsAdd(&request,FALSE,&id));
  }

//+------------------------------------------------------------------+
//| Filters and modifies an incoming stream of requests.             |
//|                                                                  |
//| The hook is called before a request is added to the processing   |
//| queue. The hook can be used to reject any requests connected     |
//| with pending orders. Filters and modifies an incoming stream of  |
//| requests. It can also be used for forced addition of an          |
//| expiration date under certain conditions.                        |
//+------------------------------------------------------------------+
int APIENTRY iTradeRequestFilter(RequestInfo *request,const int isdemo)
  {
  ConSymbol     symbol={0};
  RequestInfo   req;
  //--- checks     // isdemo: skip automated request dealing on the server.
  if(!request||isdemo==TRUE||!ExtGateway||!ExtServer)
    return(RET_OK);
  //--- must be local copy of request
  req=*request;
  //---get symbol
  if(ExtServer->SymbolsGet(req.trade.symbol,&symbol)==FALSE)
    return(RET_INVALID_DATA);
  //---check long only
  if(symbol.long_only!=FALSE&&(req.trade.cmd==OP_SELL||req.trade.cmd==OP_SELL_LIMIT||req.trade.cmd==OP_SELL_STOP))
    {
    LOG_INFO(pluginLog,"'%d' long only allowed #%d",req.login,req.trade.order);
    return(RET_TRADE_LONG_ONLY); // long only
    }
  //--- process
  return(ExtGateway->CheckHedgeAllowed(req.group,req.trade.symbol,req.login,req.trade.cmd));
  }
/********************************************************************************************************************************************\
|	Purpose - implementation of automated dealing plugins.
|	  The hook is called when the client's request is enqueued to be processed. The request received can be processed immediately and confirmed or
|	  rejected using the methods of CServerInterface::RequestsPrices, CServerInterface::RequestsConfirm, CServerInterface::RequestsReset, CServerInterface::RequestsRequote.
|	  If it takes some time to process a given request (for example, when connecting to an external server), the given order must be attached
|	  to the manager on behalf of which the plugin works (using the CServerInterface::RequestsLock method), and then the request must be processed in a separate thread.
|	  If the plugin leaves request untouched it will be left in request queue for processing by dealer.
|
|	Parameters:
|	  request - the client's request
|	  isdemo - flag of the request execution by automated dealer of the server
\********************************************************************************************************************************************/
void APIENTRY iTradeRequestApply(RequestInfo *request,const int isdemo)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;
  //---
  double              prices[2]    ={0};
  ConSymbol           cs           ={0};
  TradeRequest        trade_request={0};
  StringType128       cmd_str;
  RequestInfo         req;     // must be local copy of request
  StringType256       message;
  StringType32        comment;
  int                 order=0;
  //--- check parameters
  if(!request||!ExtGateway||!ExtServer||isdemo==TRUE)
    return;
  //--- Will we check this in general a request or maybe a requote? if yes then we do nothing
  //if(request->status!=DC_REQUEST||request->trade.type==TT_PRICES_REQUOTE)
  //  return;
  //--- local copy
  req=*request;
  // checks
  if(!ExtGateway->CheckGroupAndSymbol(req.login,req.trade.symbol,req.group))
    {
    //--- plugin won't handle
    return;
    }
  //--- set type
  switch(req.trade.type)
    {
    //--- Request Execution for prices
    case TT_PRICES_GET:
      ExtServer->HistoryPricesGroup(&req,prices);
      ExtServer->RequestsPrices(req.id,&ExtManagerInfo,prices,FALSE);
      break;

      //--- client activity
    case TT_ORDER_IE_OPEN:
    case TT_ORDER_MK_OPEN:
      //--- lock request
      if(!ExtServer->RequestsLock(req.id,ExtManagerInfo.login))
        {
        LOG_ERROR(pluginLog,"'%d' request lock failed for ID: %d, Side: %s, Symbol: %s, Price: %.5lf",
                  req.login,
                  req.id,
                  SIntegralFormat::FormatCmdName(req.trade.cmd),
                  req.trade.symbol,
                  req.trade.price);
        //--- refusal to execute client's request.
        ExtServer->RequestsReset(req.id,&ExtManagerInfo,DC_RESETED);
        break;
        }
      //--- fill request
      TradeRequest::Fill(&req,
                         TradeRequest::IDC_OPEN,
                         TradeRequest::ORDER_REASON_CLIENT,
                         TradeRequest::ORIGIN_REQUEST,
                         trade_request);
      //-- send order to Grid
      ExtLogger.Out(CmdTrade,"'%d' send request %d to grid <%s>",req.login,req.id,req.trade.symbol);
      if(!ExtGateway->OnTradeRequestApply(&trade_request))
        {
        //--- reset
        ExtServer->RequestsReset(req.id,&ExtManagerInfo,DC_RESETED);
        break;
        }
      //--- set prices
      request->trade.price=trade_request.result_price;
      prices[0]           =trade_request.result_price;
      prices[1]           =trade_request.result_price;
      //---
      if(ExtGateway->OnOrderOpen(trade_request,message))
        {
        ExtLogger.Out(CmdTrade,"confirm request %d/%d <%s>",
                      req.id,
                      trade_request.request_id,
                      trade_request.unique_id);
        //---
        int res;
        if((res=ExtServer->RequestsConfirm(req.id,&ExtManagerInfo,prices))==RET_OK)
          {
          ExtLogger.Out(CmdTrade,"request confirmed %d/%d %s result:<%d>",
                        req.id,
                        trade_request.request_id,
                        trade_request.unique_id,
                        res);
          //---
          break;
          }
        //---
        message.Format("'%d' request confirm failed <%s> error:<%d>",req.login,trade_request.unique_id,res);
        ExtLogger.Out(CmdTrade,message.Str());
        //--- confirm failed
        LOG_ERROR(pluginLog,message.Str());
        //----
        if(!ExtGateway->SendDontKnowTrade(trade_request.order_str,TradeRequest::DONT_KNOW_ORDER,message.Str()))
          {
          LOG_ERROR(pluginLog,"DontKnowTrade send failed <%s>",message.Str());
          }
        //--- remove request
        ExtGateway->RemoveRequest(req.id);

        break;
        }
      //---
      ExtServer->RequestsReset(req.id,&ExtManagerInfo,DC_RESETED);
      //---
      break;

      // client activity
    case TT_ORDER_IE_CLOSE:
    case TT_ORDER_MK_CLOSE:
      //--- check
      if(ExtGateway->CheckRequestByOrder(req.trade.order))
        {
        // refuse request since it is already send to grid
        ExtServer->RequestsReset(req.id,&ExtManagerInfo,DC_RESETED);
        break;
        }
      //--- lock request
      if(!ExtServer->RequestsLock(req.id,ExtManagerInfo.login))
        {
        LOG_ERROR(pluginLog,"'%d' RequestsLock failed for ID: %d, Side: %s, Symbol: %s, Price: %.5lf",
                  req.login,
                  req.id,
                  SIntegralFormat::FormatCmdName(req.trade.cmd),
                  req.trade.symbol,
                  req.trade.price);
        //--- reset
        ExtServer->RequestsReset(req.id,&ExtManagerInfo,DC_RESETED);
        break;
        }
      //--- fill request
      TradeRequest::Fill(&req,
                         TradeRequest::IDC_CLOSE,
                         TradeRequest::ORDER_REASON_CLIENT,
                         TradeRequest::ORIGIN_REQUEST,
                         trade_request);
      //-- send order to Grid
      ExtLogger.Out(CmdTrade,"'%d' send request %d to grid <%s>",req.login,req.id,req.trade.symbol);
      if(!ExtGateway->OnTradeRequestApply(&trade_request))
        {
        //--- reset
        ExtServer->RequestsReset(req.id,&ExtManagerInfo,DC_RESETED);
        break;
        }
      //--- set price
      request->trade.price=trade_request.result_price;
      prices[0]           =trade_request.result_price;
      prices[1]           =trade_request.result_price;
      if(ExtGateway->OnOrderClose(trade_request,message))
        {
        ExtLogger.Out(CmdTrade,"confirm request %d/%d <%s>",
                      req.id,
                      trade_request.request_id,
                      trade_request.unique_id);
        int res;
        if((res=ExtServer->RequestsConfirm(req.id,&ExtManagerInfo,prices))==RET_OK)
          {
          ExtLogger.Out(CmdTrade,"request confirmed %d/%d %s result:<%d>",
                        req.id,
                        trade_request.request_id,
                        trade_request.unique_id,
                        res);
          //---
          break;
          }
        //--- confirm failed            
        message.Format("'%d' request confirm failed <%s> error:<%d>",req.login,trade_request.unique_id,res);
        LOG_ERROR(pluginLog,message.Str());
        //----
        if(!ExtGateway->SendDontKnowTrade(trade_request.order_str,TradeRequest::DONT_KNOW_ORDER,message.Str()))
          {
          LOG_ERROR(pluginLog,"DontKnowTrade send failed <%s>",message.Str());
          }
        //--- remove request
        ExtGateway->RemoveRequest(req.id);
        //---
        break;
        }
      //---
      ExtServer->RequestsReset(req.id,&ExtManagerInfo,DC_RESETED);
      //---
      break;

      // manager activity
    case TT_BR_ORDER_OPEN:
      //--- lock request
      if(!ExtServer->RequestsLock(req.id,req.manager))
        {
        LOG_ERROR(pluginLog,"'%d' RequestsLock failed for CRC: %d, Price: %.5lf",req.id,req.trade.crc,req.trade.price);
        //--- delete
        ExtServer->RequestsDelete(req.id);
        break;
        }
      //--- fill record
      TradeRequest::Fill(&req,
                         TradeRequest::IDC_OPEN,
                         TradeRequest::ORDER_REASON_DEALER,
                         TradeRequest::ORIGIN_ORDER,
                         trade_request);
      //--- set manager
      trade_request.manager=req.manager;
      //--- wait here
      if(!ExtGateway->OnTradeRequestApply(&trade_request))
        {
        //--- delete
        ExtServer->RequestsDelete(req.id);
        break;
        }
      //---
      //if(!ExtGateway->ProcessMarketOpen(trade_request,message))
      //  {
      //  if(!ExtGateway->SendDontKnowTrade(trade_request.order_str,TradeRequest::DONT_KNOW_ORDER,message.Str()))
      //    {
      //    LOG_ERROR(pluginLog,"DontKnowTrade send failed <%s>",message.Str());
      //    }
      //  }
      break;

#pragma region old code

      //--- check result
      //switch(record.state)
      //  {
      //  case TradeRequest::ORDER_STATE_FILLED:
      //  case TradeRequest::ORDER_STATE_EXPIRED:
      //  case TradeRequest::ORDER_STATE_CANCELLED:
      //    //---
      //    prices[0]=request->prices[0]=record.request.trade.price;
      //    prices[1]=request->prices[1]=record.request.trade.price;
      //    //--- set execution information
      //    request->trade.price=record.request.trade.price;
      //    request->trade.volume=record.request.trade.volume;
      //    //--- check volume
      //    if(record.request.trade.volume<=0)
      //      {
      //      //--- send execution report
      //      ExtGateway->SendAcknowledgement(record.exec_str);
      //      //--- Remove
      //      ExtGateway->RemoveRequest(record.request_id);
      //      break;
      //      }
      //    //--- open order         
      //    if((order=ExtGateway->OrdersOpen(req.login,req.trade.cmd,req.trade.symbol,record.request.trade.price,record.request.trade.volume,record.order_id,req.trade.sl,req.trade.tp))<1)
      //      {
      //      //--- confirm failed
      //      LOG_ERROR(pluginLog,"'%d' request #%d confirm failed",req.login,req.id);
      //      ExtGateway->SendDontKnowTrade(record.order_str,TradeRequest::DONT_KNOW_ORDER,"order open failed");
      //      //--- Remove
      //      ExtGateway->RemoveRequest(record.request_id);
      //      }
      //    else
      //      {
      //      if(ExtGateway->RecordGet(req.id,response))
      //        {
      //        //--- prepare message
      //        message.Format("'%s' Open MT4 Position %d, FXI Execution ID %s, FXI Order ID %s, price %f, CumQty %0.2lf",
      //                       response.text,
      //                       order,
      //                       response.exec_id,
      //                       response.order_id,
      //                       response.price,
      //                       response.fix_qty);
      //        //---
      //        LOG_ORDER(orderLog,message.Str());
      //        }
      //      //---
      //      ExtGateway->SendAcknowledgement(record.exec_str);
      //      }
      //    //---
      //    break;

      //  case TradeRequest::ORDER_STATE_REJECTED:
      //  case TradeRequest::ORDER_STATE_TIMEOUT:
      //    //--- send Ack
      //    ExtGateway->SendAcknowledgement(record.exec_str);
      //    //--- Remove
      //    ExtGateway->RemoveRequest(record.request_id);
      //    break;

      //  default:
      //    message.Format("'%d' request not found #%d",req.login,req.id);
      //    ExtGateway->SendDontKnowTrade(record.order_str,TradeRequest::DONT_KNOW_ORDER,message.Str());
      //    //--- Remove
      //    ExtGateway->RemoveRequest(record.request_id);
      //    break;
      //  }
      ////--- delete
      //ExtServer->RequestsDelete(req.id);
      break;

#pragma endregion old code

    case TT_BR_ORDER_ACTIVATE:
      //--- lock request
      if(!ExtServer->RequestsLock(req.id,req.manager))
        {
        LOG_ERROR(pluginLog,"'%d' RequestsLock failed for CRC: %d, Price: %.5lf",req.id,req.trade.crc,req.trade.price);
        //--- delete
        ExtServer->RequestsDelete(req.id);
        break;
        }
      //--- fill request
      TradeRequest::Fill(&req,
                         TradeRequest::IDC_OPEN,
                         TradeRequest::ORDER_REASON_ACTIVATION,
                         TradeRequest::ORIGIN_ORDER,
                         trade_request);
      //--- set manager
      trade_request.manager=req.manager;
      //--- wait here
      if(!ExtGateway->OnTradeRequestApply(&trade_request))
        {
        //--- delete
        ExtServer->RequestsDelete(req.id);
        break;
        }
      //---
      //if(!ExtGateway->ProcessPendingOrderActivation(trade_request,message))
      //  {
      //  if(!ExtGateway->SendDontKnowTrade(trade_request.order_str,TradeRequest::DONT_KNOW_ORDER,message.Str()))
      //    {
      //    LOG_ERROR(pluginLog,"DontKnowTrade send failed <%s>",message.Str());
      //    }
      //  }
      break;

#pragma region old code

      ////--- check result
      //switch(record.state)
      //  {
      //  case TradeRequest::ORDER_STATE_FILLED:
      //  case TradeRequest::ORDER_STATE_EXPIRED:
      //  case TradeRequest::ORDER_STATE_CANCELLED:
      //    //---
      //    prices[0]=request->prices[0]=record.request.trade.price;
      //    prices[1]=request->prices[1]=record.request.trade.price;
      //    //--- set execution information
      //    request->trade.price=record.request.trade.price;
      //    request->trade.volume=record.request.trade.volume;
      //    if(record.request.trade.volume<=0)
      //      {
      //      //--- send execution report
      //      ExtGateway->SendAcknowledgement(record.exec_str);
      //      //--- Remove
      //      ExtGateway->RecordDelete(record.request_id);
      //      break;
      //      }
      //    //---
      //    if(ExtGateway->OrdersUpdateActivate(req.trade.order,record.request.trade.volume,record.request.trade.price,record.order_id)==FALSE)
      //      {
      //      LOG_ERROR(pluginLog,"'%d' order #%d open activate failed",req.login,req.trade.order);
      //      ExtGateway->SendDontKnowTrade(record.order_str,TradeRequest::DONT_KNOW_ORDER,"order activate failed");
      //      //--- Remove
      //      //ExtGateway->RecordDelete(record.request_id);
      //      //---
      //      break;
      //      }
      //    else //--- send execution report
      //      {
      //      ExtGateway->SendAcknowledgement(record.exec_str);
      //      //--- Remove
      //      ExtGateway->RecordDelete(record.request_id);
      //      }
      //    //---
      //    return;

      //  case TradeRequest::ORDER_STATE_REJECTED:
      //  case TradeRequest::ORDER_STATE_TIMEOUT:
      //    //--- send Ack
      //    ExtGateway->SendAcknowledgement(record.exec_str);
      //    //--- Remove
      //    ExtGateway->RecordDelete(record.request_id);
      //    break;

      //  default:
      //    message.Format("'%d' request not found #%d",req.login,req.id);
      //    ExtGateway->SendDontKnowTrade(record.order_str,TradeRequest::DONT_KNOW_ORDER,message.Str());
      //    //--- Remove
      //    ExtGateway->RecordDelete(record.request_id);
      //    break;
      //  }
      ////--- delete
      //ExtServer->RequestsDelete(req.id);
      ////ExtGateway->RemoveResponse(req.trade.order);
      //break;

#pragma endregion

    case TT_BR_ORDER_CLOSE:
      //--- lock request
      if(!ExtServer->RequestsLock(req.id,req.manager))
        {
        LOG_ERROR(pluginLog,"'%d' RequestsLock failed for CRC: %d, Price: %.5lf",req.id,req.trade.crc,req.trade.price);
        //--- delete
        ExtServer->RequestsDelete(req.id);
        break;
        }
      //--- fill request
      if(!TradeRequest::Fill(&req,
                             TradeRequest::IDC_CLOSE,
                             TradeRequest::ORDER_REASON_DEALER,
                             TradeRequest::ORIGIN_ORDER,
                             trade_request))
        {
        //--- delete
        ExtServer->RequestsDelete(req.id);
        break;
        }
      //--- set manager
      trade_request.manager=req.manager;
      //--- wait here
      if(!ExtGateway->OnTradeRequestApply(&trade_request))
        {
        //--- delete
        ExtServer->RequestsDelete(req.id);
        break;
        }
      //---
      //if(!ExtGateway->ProcessMarketClose(trade_request,message))
      //  {
      //  if(!ExtGateway->SendDontKnowTrade(trade_request.order_str,TradeRequest::DONT_KNOW_ORDER,message.Str()))
      //    {
      //    LOG_ERROR(pluginLog,"DontKnowTrade send failed <%s>",message.Str());
      //    }
      //  }
      break;

#pragma region old code

      ////--- check result
      //switch(record.state)
      //  {
      //  case TradeRequest::ORDER_STATE_FILLED:
      //  case TradeRequest::ORDER_STATE_EXPIRED:
      //  case TradeRequest::ORDER_STATE_CANCELLED:
      //    //--- set execution information
      //    prices[0]=request->prices[0]=record.request.trade.price;
      //    prices[1]=request->prices[1]=record.request.trade.price;
      //    request->trade.price=record.request.trade.price;
      //    //---
      //    request->trade.volume=record.request.trade.volume;
      //    if(record.request.trade.volume<=0)
      //      {
      //      //--- send execution report
      //      ExtGateway->SendAcknowledgement(record.exec_str);
      //      //--- Remove
      //      ExtGateway->RecordDelete(record.request_id);
      //      break;
      //      }
      //    //--- close order
      //    if(!ExtGateway->OrdersClose(req.trade.order,record.request.trade.volume,record.request.trade.price,record.comment,record.order_id))
      //      {
      //      ExtGateway->SendDontKnowTrade(record.order_str,TradeRequest::DONT_KNOW_ORDER,"order close failed");
      //      //--- Remove
      //      //ExtGateway->RemoveByUniqueId(record.unique_id);
      //      }
      //    else
      //      {
      //      //--- dont remove for comment update
      //      ExtGateway->AddComment(record);
      //      ExtGateway->SendAcknowledgement(record.exec_str);
      //      ExtGateway->RecordDelete(record.request_id);
      //      }
      //    //---
      //    break;

      //  case TradeRequest::ORDER_STATE_REJECTED:
      //  case TradeRequest::ORDER_STATE_TIMEOUT:
      //    //--- send Ack
      //    ExtGateway->SendAcknowledgement(record.exec_str);
      //    //--- Remove
      //    ExtGateway->RecordDelete(record.request_id);
      //    break;

      //  default:
      //    message.Format("'%d' request not found #%d",req.login,req.id);
      //    ExtGateway->SendDontKnowTrade(record.order_str,TradeRequest::DONT_KNOW_ORDER,message.Str());
      //    //--- Remove
      //    ExtGateway->RecordDelete(record.request_id);
      //    break;
      //  }
      ////--- we have remaining volume
      //ExtServer->RequestsDelete(req.id);
      //break;

#pragma endregion

    //--- we already checked
    case TT_ORDER_DELETE:
    case TT_ORDER_MODIFY:
      ExtServer->RequestsConfirm(req.id,&ExtManagerInfo,prices);
      break;

      //--- skip
    case TT_ORDER_REQ_OPEN:
    case TT_ORDER_REQ_CLOSE:
    case TT_ORDER_PENDING_OPEN:
      //--- confirm
      ExtServer->RequestsConfirm(req.id,&ExtManagerInfo,prices);
      break;

      //---
    case TT_ORDER_CLOSE_BY:
    case TT_BR_ORDER_CLOSE_BY:
      //--- confirm
      ExtServer->RequestsConfirm(req.id,&ExtManagerInfo,prices);
      break;

    case TT_ORDER_CLOSE_ALL:
    case TT_BR_ORDER_CLOSE_ALL:
      //--- confirm
      ExtServer->RequestsConfirm(req.id,&ExtManagerInfo,prices);
      break;

    case TT_PRICES_REQUOTE:     // skip High Level requotes
      break;

    default:
      LOG_ERROR(pluginLog,"'%d': invalid request type %d",req.login,req.trade.type);
      break;
    }
  //--- done
  }
/********************************************************************************************************************************************\
|	Purpose - for integration with external trading systems
|	  This hook is called similar to MtSrvTradesAdd - when an order is successfully added to the server database.
|	  But if the order is added due to execution of a request, the structure of an appropriate request is to be passed as request parameter.
|	  If adding an order is not related to a trading request (during automatic processing by the server, dealer trading, correction by an administrator or plugin, etc.), NULL is passed in request parameter.
|	  This hook allows users to clearly match the ticket of the newly placed order with the request, after confirmation of which that order has been generated.
|	  This is useful when integrating MetaTrader 4 with external trading systems.
|
|	Parameters:
|	  trade - the trade record to be added
|	  request - the client's request
|	  user - description of the account
|	  symbol - description of the symbol
|	  mode - ????
\********************************************************************************************************************************************/
void APIENTRY iTradesAddRequest(TradeRecord *trade,const RequestInfo *request,const UserInfo *user,const ConSymbol *symb,const int mode)
  {
  UserInfo            user_local={0};
  //IntegralTradeRecord response={0};
  TradeRequest        trade_request={0};
  StringType32        comment;
  StringType256       message;
  StringType32        type;
  //--- checks
  if(!trade||!user||!symb||!ExtGateway||trade->cmd>OP_SELL)                     return;
  //--- valid?
  if(!ExtGateway->CheckGroupAndSymbol(trade->login,trade->symbol,user->group))  return;
  //---
  switch(mode)
    {
    case OPEN_NEW:     // Record added\opened as a result of a new order creation (the client has sent a new order).
      break;
    case OPEN_CLOSE:   // Record added\opened as a remainder after a partial closure.
    case OPEN_RESTORE:  // Record added\opened as a result of reopening of a closed trading positions in MetaTrader 4 Administrator.
    case OPEN_ROLLOVER: // Record added\opened as a result of rollover calculation with reopening.
    case OPEN_API:       // Record added\opened as a result of CServerInterface::OrdersAdd call from a server plug-in
      return;
    default:
      LOG_CRITICAL(pluginLog,"'%d' #%d unknown mode <%d>",trade->login,trade->order,mode);
      return;
    }
  //---
  INT id=(request)?request->id:-trade->order;
  ExtLogger.Out(CmdTrade,"request confirmed and result received %d <%s>",id,trade->symbol);
  if(!ExtGateway->RemoveRequest(id,trade_request))
    {
    LOG_CRITICAL(pluginLog,"'%d' response not found <%d>",trade->login,id);
    return;
    }
  //---
  ExtLogger.Out(CmdTrade,"'%d' request confirmed %s @%0.5lf OrderId:%s",
                trade->login,
                trade->symbol,
                trade_request.result_price,
                trade_request.order_id);
  //--- set volume
  trade->api_data[0]=trade->order;
  trade->api_data[1]=trade_request.volume;
  //--- price came "0" once!!!
  if(trade_request.result_price<=0||trade_request.result_volume<=0)
    {
    StringType256 msg;
    msg.Format("'%d' response has zero value #%d <price:%0.7lf,volume:%d>",
               trade->login,
               trade->order,
               trade_request.result_price,
               trade_request.result_volume);
    LOG_CRITICAL(pluginLog,msg.Str());
    ExtGateway->SendDontKnowTrade(trade_request.unique_id,TradeRequest::DONT_KNOW_ORDER,msg.Str());
    return;
    }
  //---
  trade->volume    =trade_request.result_volume;
  trade->open_price=trade_request.result_price;
  //--- check margin rate
  if(trade->margin_rate==0)
    trade->margin_rate=ExtServer->TradesCalcConvertation(user->group,TRUE,trade->open_price,symb);
  //--- check conv rate
  if(trade->conv_rates[0]==0)
    trade->conv_rates[0]=ExtServer->TradesCalcConvertation(user->group,FALSE,trade->open_price,symb);
  //---
  memcpy(&user_local,user,sizeof(UserInfo));
  //---prepare comment
  ExtGateway->AddComment(trade->comment,"O:",trade_request.order_id,comment);
  StringType::Copy(trade->comment,comment.Str());
  if(!ExtGateway->OrdersUpdate(trade,&user_local,UPDATE_NORMAL))
    {
    LOG_CRITICAL(pluginLog,"'%d' order update failed",trade->order);
    }
  //--- send Ack
  ExtGateway->SendAcknowledgement(trade_request.exec_str);
  //--- prepare message
  message.Format("'%s' Open MT4 Position %d, FXI Execution ID %s, FXI Order ID %s, price %f, CumQty %0.2lf",
                 trade_request.text,
                 trade->order,
                 trade_request.exec_id,
                 trade_request.order_id,
                 trade_request.result_price,
                 trade_request.fix_qty);
  //---
  LOG_ORDER(orderLog,message.Str());
  }
/********************************************************************************************************************************************\
|	Purpose - interception of the trade record changes in the database.
|	  The hook is called immediately after the new trade record has been successfully added to the server database.
|	  This opportunity can be used to trace the changes made in the trade records on certain accounts.
|
|	Parameters:
|	  trade - the trade record to be added
|	  user - description of the account
|	  symbol - flag of the symbol description
|	  mode - the reason for update:
|		  UPDATE_NORMAL - normal update,
|		  UPDATE_ACTIVATE - order activation,
|		  UPDATE_CLOSE - closing of the order,
|		  UPDATE_DELETE - deletion of the order
\********************************************************************************************************************************************/
void APIENTRY iTradesUpdate(TradeRecord *trade,UserInfo *user,const int mode)
  {
  TradeRequest        request={0};
  StringType32        comment;
  StringType256       message;
  double              difference=0;
  TradeRecord *trades=NULL;
  int total=0;
  //--- checks
  if(!trade||!user||!ExtGateway||!ExtServer||!(mode==UPDATE_ACTIVATE||mode==UPDATE_CLOSE))
    return;
  //--- valid?
  if(!ExtGateway->CheckGroupAndSymbol(trade->login,trade->symbol,user->group))
    {
    return;
    }
  //--- show trade
  LOG_INFO(pluginLog,"(%d) Called with Cmd %s, order %d, user %d, mode %s, state %s, open %0.5f, close %0.5f, comment <%s>",
           trade->order,
           SIntegralFormat::FormatCmdName(trade->cmd),
           trade->order,
           user->login,
           SIntegralFormat::FormatUpdateName(mode),
           SIntegralFormat::FormatStateName(trade->state),
           trade->open_price,
           trade->close_price,
           trade->comment);
  //--- exist?
  if(!ExtGateway->GetRequestByOrder(trade->order,request))
    {
    LOG_ERROR(pluginLog,"request could not be found #%d",trade->order);
    return;
    }
  //--- check if partial?
  switch(request.order_state)
    {
    case TradeRequest::ORDER_STATE_UNKNOWN:
    case TradeRequest::ORDER_STATE_SENDING:
    case TradeRequest::ORDER_STATE_PLACED:
    case TradeRequest::ORDER_STATE_PARTIAL:
    case TradeRequest::ORDER_STATE_NOT_FOUND:
      return;
    default:
      break;
    }
  //---
  switch(mode)
    {
    case UPDATE_ACTIVATE:
      //--- prepare message
      message.Format("'%s' Open MT4 Position %d, FXI Execution ID %s, FXI Order ID %s, price %f, CumQty %0.2lf",
                     request.text,
                     trade->order,
                     request.exec_id,
                     request.order_id,
                     request.result_price,
                     request.fix_qty);
      //--- send execution report
      ExtGateway->SendAcknowledgement(request.exec_str);
      break;

    case UPDATE_CLOSE:
      //--- send execution report
      ExtGateway->SendAcknowledgement(request.exec_str);
      //---
      message.Format("'%s' Close MT4 Position %d, FXI Execution ID %s, FXI Order ID %s, price %f, CumQty %0.2lf",
                     request.text,
                     trade->order,
                     request.exec_id,
                     request.order_id,
                     request.result_price,
                     request.fix_qty);
      //--- prepare comment
      if(ExtGateway->AddComment(trade->comment,"C:",request.order_id,comment))
        {
        StringType::Copy(trade->comment,comment.Str());
        //--- Order activation
        ExtServer->OrdersUpdate(trade,user,UPDATE_NORMAL);
        }
      break;

    default:
      return;
    }
  //---
  if(StringType::Find(trade->comment,"deleted")<0)
    LOG_ORDER(orderLog,message.Str());
  //--- exist?
  if(!ExtGateway->RemoveRequestByOrder(trade->order,request))
    {
    LOG_ERROR(pluginLog,"request could not be found #%d",trade->order);
    }
  }
///********************************************************************************************************************************************\
  //|	Purpose - provides ability to create some special activitiy right after rollovers having been processed.
  //|
  //|	Parameters:
  //|	  month_state - falg of daily reports (TRUE - monthly report, FALSE - daily report)
  //|	  ctm - ???
  //|	  ttm - ???
  //\********************************************************************************************************************************************/
void APIENTRY iStatementsRollovers(const int month_state,const time_t ctm,const tm *ttm)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;
  //---
  ConSymbol    cs={0};
  double       rates[2]={0};
  //---
  if(!ExtPluginContext.EODReports()||month_state==TRUE||!ExtServer)
    return;
  //--- write prices
  for(int m_symb_total=0;ExtServer->SymbolsNext(m_symb_total,&cs)!=FALSE;m_symb_total++)
    {
    if(ExtServer->HistoryPrices(cs.symbol,rates,NULL,NULL)==RET_OK)
      {
      ExtReports.ClosingPrice(ctm,"%04d-%02d-%02d,%s,%.10g,%.10g",
                              1900+ttm->tm_year,
                              ttm->tm_mon+1,
                              ttm->tm_mday,
                              cs.symbol,
                              rates[0],
                              rates[1]);
      }
    }
  //---
  std::map<std::string,std::string> groupCCYs;
  for(int i=0; i<512; i++)
    {
    ConGroup group={0};
    if(ExtServer->GroupsNext(i,&group)==FALSE)
      continue;

    groupCCYs[group.group]=group.currency;
    }

  std::map<std::string,double> sizeCCYs;
  for(int i=0; i<MAX_SYMBOLS; i++)
    {
    ConSymbol sym={0};
    if(ExtServer->SymbolsNext(i,&sym)==FALSE)
      continue;

    sizeCCYs[sym.symbol]=sym.contract_size;
    }

  int totalUsers=0;
  UserRecord *users=ExtServer->ClientsAllUsers(&totalUsers);
  if(users!=NULL)
    {
    for(int i=0; i<totalUsers; i++)
      {
      int totalOpen=0;
      UserInfo ui={0};

      ExtReports.AccountBalance(ctm,"%04d-%02d-%02d,%d,%s,%s,%.2lf",1900+ttm->tm_year,ttm->tm_mon+1,ttm->tm_mday,users[i].login,users[i].group,groupCCYs[users[i].group].c_str(),users[i].balance);

      ui.login=users[i].login;
      COPY_STR(ui.group,users[i].group);
      COPY_STR(ui.name,users[i].name);
      ui.credit=users[i].credit;
      ui.balance=users[i].balance;
      ui.leverage=users[i].leverage;

      TradeRecord *trades=ExtServer->OrdersGetOpen(&ui,&totalOpen);

      if(trades!=NULL)
        {
        std::map<std::string,Position> userOrders;

        for(int j=0; j<totalOpen; j++)
          {
          Position p=(userOrders.find(trades[j].symbol))==userOrders.end()?Position():userOrders[trades[j].symbol];

          if(trades[j].cmd==OP_BUY)
            {
            p.BuyVolume+=trades[j].volume;
            p.BuyPnL+=trades[j].profit;
            p.BuySwaps+=trades[j].storage;
            }
          else if(trades[j].cmd==OP_SELL)
            {
            p.SellVolume+=trades[j].volume;
            p.SellPnL+=trades[j].profit;
            p.SellSwaps+=trades[j].storage;
            }

          userOrders[trades[j].symbol]=p;
          }

        for(auto &item:userOrders)
          ExtReports.OpenPosition(ctm,"%04d-%02d-%02d,%d,%s,%s,%s,%.2lf,%.2lf,%.2lf,%s,%.2lf,%.2lf,%.2lf,%S,%.2lf,%.2lf,%.2lf",
                                  1900+ttm->tm_year,
                                  ttm->tm_mon+1,
                                  ttm->tm_mday,
                                  users[i].login,
                                  users[i].group,
                                  item.first.c_str(),

                                  "B",
                                  (abs(item.second.BuyVolume)/100.0)*sizeCCYs[item.first],
                                  item.second.BuySwaps,
                                  item.second.BuyPnL,

                                  "S",
                                  (abs(item.second.SellVolume)/100.0)*sizeCCYs[item.first],
                                  item.second.SellSwaps,
                                  item.second.SellPnL,

                                  ((item.second.BuyVolume-item.second.SellVolume)>=0?"B":"S"),
                                  (abs(item.second.BuyVolume-item.second.SellVolume)/100.0)*sizeCCYs[item.first],
                                  (item.second.BuySwaps+item.second.SellSwaps),
                                  (item.second.BuyPnL+item.second.SellPnL)
          );

        HEAP_FREE(trades);
        trades=NULL;
        }
      }

    HEAP_FREE(users);
    users=NULL;
    }
  }

/********************************************************************************************************************************************\
|	Purpose - filtering of stopuots processing.
|	  The server calls this hook when positions are being closed by stopout. If this hook returns RET_OK, the position will be closed according
|	  to the standard procedure using the server automated dealer. Otherwise (the hook returns something other than RET_OK), the plugin notifies
|	  the server that the plugin will close the positions by stopouts independently (or the positions will not be closed at all).
|
|	Implementation of this hook must be optimized by time, otherwise the processing of stops and pending orders may be considerably delayed.
|	Inside of this hook, the methods of server interface named CServerInterface::MtSrvTradesAdd and CServerInterface::MtSrvTradesUpdate should
|	not be called in order to add/remove orders within the same group of users.
|
|	Parameters:
|	  group - description of the group
|	  symbol - description of the symbol
|	  login - the account number
|	  equity - the current funds
|	  margin - the current margin
\********************************************************************************************************************************************/
int APIENTRY iTradeStopoutsFilter(const ConGroup *group,const ConSymbol *symbol,const int login,const double equity,const double margin)
  {
  //--- checks
  if(!group||!symbol||login<1||!ExtGateway)
    return(RET_INVALID_DATA);
  //--- valid?
  if(!ExtGateway->CheckGroupAndSymbol(login,symbol->symbol,group->group))
    {
    // Return Default value, plugin won't handle
    return(RET_OK_NONE);
    }
  //--- show
  /*LOG_JOURNAL(pluginLog,"'%d' group %0.16s, symbol %0.12s, equity %.10g, margin %.10g",
              login,
              group->group,
              symbol->symbol,
              equity,
              margin);*/
              //---
  return(RET_OK);
  }

/********************************************************************************************************************************************\
|	Purpose - interception of position closing by stopout
|	  This hook is intended for interception and modifying of parameters of positions closed by stopout (for example, close price or comments).
|	  It is called after the necessary information (stopout) for pending order activation has been prepared.
|	  If this hook returns RET_OK, the position will be closed by automated dealer considering changes made by the hook. Otherwise, the position remains open.
|
|	Implementation of this hook must be optimized by time, otherwise the processing of stops and pending orders may be considerably delayed.
|	Inside of this hook, the methods of server interface named CServerInterface::MtSrvTradesAdd and CServerInterface::MtSrvTradesUpdate should
|	not be called in order to add/remove orders within the same group of users.
|
|	Parameters:
|	  user - description of the account
|	  group - description of the group to which the account belongs
|	  symbol - description of the symbol
|	  stopout - the position to be closed by stopout
\********************************************************************************************************************************************/
int APIENTRY iTradeStopoutsApply(const UserInfo *user,const ConGroup *group,const ConSymbol *symbol,TradeRecord *stopout)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;
  //---
  UserInfo           user_local={0};
  double             margin=0,freemargin=0,equity=0,prices[2]={0};
  StringType64       str_bid,str_ask;
  TradeRequest       trade_request={0};
  //---smthg wrong don't touch
  if(!user||!group||!symbol||!stopout||!ExtGateway)
    return(RET_OK);
  //--- valid?
  if(!ExtGateway->CheckGroupAndSymbol(user->login,symbol->symbol,group->group))
    {
    // Return Default value, plugin won't handle
    return(RET_OK);
    }
  //---
  if(stopout->close_price<=0.00f)
    return RET_INVALID_DATA;
  //--- check
  if(ExtGateway->CheckRequestByOrder(stopout->order))
    return(RET_TRADE_ORDER_LOCKED);
  //--- invalid stopout?
  if(stopout->close_price<=0.00f)
    {
    return RET_INVALID_DATA;
    }
  //--- get prices
  if(ExtServer->HistoryPricesGroup(symbol->symbol,group,prices)==RET_OK)
    {
    SMTFormat::FormatDouble(str_bid,prices[0],symbol->digits);
    SMTFormat::FormatDouble(str_ask,prices[1],symbol->digits);
    }
  //--- show
  memcpy(&user_local,user,sizeof(UserInfo));
  ExtServer->TradesMarginInfo(&user_local,&margin,&freemargin,&equity);
  LOG_JOURNAL(pluginLog,"'%d' SO - Order: #%d / margin: %0.2lf / freemargin: %0.2lf / close_price: %0.6lf / equity: %0.2lf <%s/%s>",
              stopout->login,
              stopout->order,
              margin,
              freemargin,
              stopout->close_price,
              equity,
              str_bid.Str(),
              str_ask.Str());
  //--- fill
  if(!TradeRequest::Fill(stopout,TradeRequest::IDC_STOPOUT,user->group,stopout->close_price,trade_request))
    return(RET_INVALID_DATA);
  //--- process stopout order
  if(ExtGateway->OnTradeRequestApply(&trade_request))
    {
    return RET_TRADE_PROCESS;
    }
  //--- failed
  return(RET_TRADE_BROKER_BUSY);
  }

/********************************************************************************************************************************************\
|	Purpose - filtering of StopLoss and TakeProfit orders processing.
|	  The server calls this hook when a StopLoss or a TakeProfit order triggers. If this hook returns RET_OK, the order will be processed
|	  according to a standard procedure (by the server automated dealer or a human dealer). Otherwise, if the hook returns something other than RET_OK,
|	  the plugin will notify the server about that the former will process the order independently (or that the order will not be processed at all).
|
|	Implementation of this hook must be optimized by time, otherwise the processing of StopLoss and TakeProfit orders may be considerably delayed.
|	Inside of this hook, the methods of server interface named CServerInterface::MtSrvTradesAdd and CServerInterface::MtSrvTradesUpdate should
|	not be called in order to add/remove orders within the same group of users.
|
|	Parameters:
|	  group - description of the group
|	  symbol - description of the symbol
|	  trade - trade position
\********************************************************************************************************************************************/
int APIENTRY iTradeStopsFilter(const ConGroup *group,const ConSymbol *symbol,const TradeRecord *trade)
  {
  //--- checks
  if(!group||!symbol||!trade||!ExtGateway)
    return(RET_INVALID_DATA);
  //--- check group and symbol
  if(!ExtGateway->CheckGroupAndSymbol(trade->login,trade->symbol,group->group))
    {
    //---- not handle
    return(RET_OK_NONE);
    }
  //--- We process
  return(RET_OK);
  }
/********************************************************************************************************************************************\
|	Purpose - interception of StopLoss and TakeProfit orders processing.
|	  This hook is intended for interception and modifying of parameters of the position order to be executed (for example, the position close price).
|	  It is called after the necessary information (trade) for execution of the Stop order has been prepared.
|	  If this hook returns RET_OK, the order is executed by the server automated dealer considering changes made by the hook. Otherwise, the order will not be processed.
|
|	Implementation of this hook must be optimized by time, otherwise the processing of StopLoss and TakeProfit orders may be considerably delayed.
|	Inside of this hook, the methods of server interface named CServerInterface::MtSrvTradesAdd and CServerInterface::MtSrvTradesUpdate should
|	not be called in order to add/remove orders within the same group of users.
|
|	Parameters:
|	  user - description of the account
|	  group - description of the group to which the account belongs
|	  symbol - description of the symbol
|	  trade - position to be closed
|	  isTP - flag showing that TakeProfit is being processed (TRUE), otherwise (FALSE) the closing is performed by StopLoss
\********************************************************************************************************************************************/
int APIENTRY iTradeStopsApply(const UserInfo *user,const ConGroup *group,const ConSymbol *symbol,TradeRecord *trade,const int isTP)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;
  //---
  UserInfo           info={0};
  double             margin,freemargin,equity;
  TradeRequest       trade_request={0};
  //--- smthg wrong don't touch
  if(!user||!group||!symbol||!trade||!ExtGateway)
    return(RET_INVALID_DATA);
  //--- check group and symbol
  if(!ExtGateway->CheckGroupAndSymbol(trade->login,trade->symbol,group->group))
    {
    //---- not handle
    return(RET_OK);
    }
  //--- check
  if(ExtGateway->CheckRequestByOrder(trade->order))
    return(RET_TRADE_ORDER_LOCKED);
  //--- prevent Prevent bad Stops (TP/SL)
  switch(trade->cmd)
    {
    case OP_BUY:
      if(isTP==TRUE&&trade->close_price<trade->tp)
        {
        LOG_ERROR(pluginLog,"Bad Prices.1: CMD: %s, isTP: %s, close_price: %.10g, tp: %.10g, sl: %.10g",
                  SIntegralFormat::FormatCmdName(trade->cmd),
                  (isTP?"TakeProfit":"StopLoss"),
                  trade->close_price,
                  trade->tp,
                  trade->sl);
        return RET_TRADE_BAD_PRICES;
        }
      else if(isTP==FALSE&&trade->close_price>trade->sl)
        {
        LOG_ERROR(pluginLog,"Bad Prices.2: CMD: %s, isTP: %s, close_price: %.10g, tp: %.10g, sl: %.10g",
                  SIntegralFormat::FormatCmdName(trade->cmd),
                  (isTP?"TakeProfit":"StopLoss"),
                  trade->close_price,
                  trade->tp,
                  trade->sl);
        return RET_TRADE_BAD_PRICES;
        }
      break;
    case OP_SELL:
      if(isTP==TRUE&&trade->close_price>trade->tp)
        {
        LOG_ERROR(pluginLog,"Bad Prices.3: CMD: %s, isTP: %s, close_price: %.10g, tp: %.10g, sl: %.10g",
                  SIntegralFormat::FormatCmdName(trade->cmd),
                  (isTP?"TakeProfit":"StopLoss"),
                  trade->close_price,
                  trade->tp,
                  trade->sl);
        return RET_TRADE_BAD_PRICES;
        }
      else if(isTP==FALSE&&trade->close_price<trade->sl)
        {
        LOG_ERROR(pluginLog,"Bad Prices.4: CMD: %s, isTP: %s, close_price: %.10g, tp: %.10g, sl: %.10g",
                  SIntegralFormat::FormatCmdName(trade->cmd),
                  (isTP?"TakeProfit":"StopLoss"),
                  trade->close_price,
                  trade->tp,
                  trade->sl);
        return RET_TRADE_BAD_PRICES;
        }
      break;
    default:
      break;
    }
  //--- show
  LOG_INFO(pluginLog,"'%d' Order %d, User %d, group %0.16s, symbol %0.12s, action %s, activation %d",
           user->login,
           trade->order,
           user->login,
           group->group,
           symbol->symbol,
           (isTP?"TakeProfit":"StopLoss"),
           trade->activation);
  //--- show user info
  memcpy(&info,user,sizeof(UserInfo));
  if(ExtServer->TradesMarginInfo(&info,&margin,&freemargin,&equity)==TRUE)
    {
    LOG_JOURNAL(pluginLog,"'%d' margin: %0.2lf / freemargin: %0.2lf / equity: %0.2lf",
                user->login,
                margin,
                freemargin,
                equity);
    }
  //---
  UINT action=((isTP==TRUE)?TradeRequest::IDC_TP:TradeRequest::IDC_SL);
  if(!TradeRequest::Fill(trade,action,user->group,(isTP==TRUE)?trade->tp:trade->sl,trade_request))
    return(RET_TRADE_BROKER_BUSY);
  //---
  if(ExtGateway->OnTradeRequestApply(&trade_request))
    {
    LOG_DEBUG(pluginLog,"'%d' order #%d send for %s",trade->login,trade->order,trade->symbol);
    return(RET_TRADE_PROCESS);
    }
  //--- failed
  return(RET_TRADE_BROKER_BUSY);
  }
/********************************************************************************************************************************************\
|	Purpose - filtering of the pending order activation.
|	  The server calls this hook when pending orders are activated. If this hook returns RET_OK, the order will be processed according to
|	  the standard procedure (by the server automated dealer or by a human dealer). Otherwise, if the hook returns something other than RET_OK,
|	  the plugin notifies the server that the plugin will process this order independently (or that the order will not be processed at all).
|
|	Implementation of this hook must be optimized by time, otherwise the pending order activation may be considerably delayed.
|	Inside of this hook, the methods of server interface named CServerInterface::MtSrvTradesAdd and CServerInterface::MtSrvTradesUpdate should
|	not be called in order to add/remove orders within the same group of users.
|
|	Parameters:
|	  group - description of the group
|	  symbol - description of the symbol
|	  trade - pending order
\********************************************************************************************************************************************/
int APIENTRY iTradePendingsFilter(const ConGroup *group,const ConSymbol *symbol,const TradeRecord *trade)
  {
  return(RET_OK);
  }
/********************************************************************************************************************************************\
|	Purpose - interception of the pending order activation.
|	  This hook is intended for interception and modifying of the order to be executed (for example, the position open price) and is called after
|	  the necessary information (trade) has been prepared for activation of the initial pending order.
|	  If this hook returns RET_OK, the order is executed by the server automated dealer considering the changes made by the hook. Otherwise, the order will not be processed.
|
|	Implementation of this hook must be optimized by time, otherwise the pending order activation may be considerably delayed.
|	Inside of this hook, the methods of server interface named CServerInterface::MtSrvTradesAdd and CServerInterface::MtSrvTradesUpdate should
|	not be called in order to add/remove orders within the same group of users.
|
|	Parameters:
|	  user - description of the account
|	  group - description of the group to which the account belongs
|	  symbol - description of the symbol
|	  pending - the initial pending order
|	  trade - the position to be opened
\********************************************************************************************************************************************/
int APIENTRY iTradePendingsApply(const UserInfo *user,const ConGroup *group,const ConSymbol *symbol,const TradeRecord *pending,TradeRecord *trade)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;
  //---
  double             margin,freemargin,prevmargin=0,profit=0;
  bool               hedge_prohibited=false;
  TradeRecord        local_trade={0};
  TradeTransInfo     trans={0};
  TradeRequest       trade_request={0};
  //--- smthg wrong don't touch
  if(!user||!group||!symbol||!pending||!trade||!ExtGateway||!ExtServer)
    return(RET_INVALID_DATA);
  //--- close only check
  local_trade=*trade;
  //--- check
  if(ExtGateway->CheckRequestByOrder(local_trade.order))
    return(RET_TRADE_ORDER_LOCKED);
  //--- valid?
  if(!ExtGateway->CheckGroupAndSymbol(local_trade.login,local_trade.symbol,user->group))
    {
    // Return Default value, plugin won't handle
    return RET_OK;
    }
  //--- check clients pending requests
  if(!ExtGateway->CheckPendingRequests(trade->login))
    return(RET_TRADE_TOO_MANY_REQ);
  //---  check close only
  if(!ExtGateway->CheckCloseOnly(trade->order,symbol))
    return(RET_TRADE_DISABLE);
  //--- show
  LOG_INFO(pluginLog,"'%d' Called with Cmd %s, order %d, state %s, open %0.5f, comment <%s>",
           user->login,
           SIntegralFormat::FormatCmdName(pending->cmd),
           pending->order,
           SIntegralFormat::FormatStateName(pending->state),
           pending->open_price,
           pending->comment);
  //--- prepare transaction for checks
  trans.cmd   =trade->cmd;
  trans.volume=trade->volume;
  trans.price =trade->open_price;
  StringType::Copy(trans.symbol,trade->symbol);
  margin=ExtServer->TradesMarginCheck(user,&trans,&profit,&freemargin,&prevmargin);
  if((freemargin+group->credit)<0&&(symbol->margin_hedged_strong!=FALSE||prevmargin<=margin))
    return(RET_TRADE_NO_MONEY);
  //---
  LOG_JOURNAL(pluginLog,"'%d' margin: %0.2lf / freemargin: %0.2lf / equity: %0.2lf",
              user->login,
              margin,
              freemargin,
              margin+freemargin);
  //--- fill
  if(!TradeRequest::Fill(pending,TradeRequest::IDC_PENDING,user->group,trade->open_price,trade_request))
    return(RET_TRADE_BROKER_BUSY);
  //--- process
  if(ExtGateway->OnTradeRequestApply(&trade_request))
    {
    //--- successfull
    return RET_TRADE_ACCEPTED;
    }
  //---
  LOG_DEBUG(pluginLog,"%d pending order #%d send failed",local_trade.login,local_trade.order);
  //--- failed
  return(RET_TRADE_BROKER_BUSY);
  }
/********************************************************************************************************************************************\
|	Purpose - notification about adding a new record in the log file.
|	  The hook is called when a new record is made in the server log file.
|
|	Parameters:
|	  info - record description
|
|	Important: If method LogsOut of the server interface is called from this hook, a recursion will be formed.
\********************************************************************************************************************************************/
void APIENTRY iLog(const LogInfo *info)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;

  StringType16 cmd;
  const std::string oIP(info->ip);
  const std::string oLog(info->msg);

  cmd.Assign(SIntegralFormat::FormatError(info->code));

  if(ExtPluginContext.LogJournal()&&oIP!="Filter"&&cmd.Compare("Unknown Cmd")!=0&&oLog.length()>2)
    LOG_MT4(pluginLog,"%s\t%s",cmd.Str(),oLog.substr(2).c_str());

  if(oIP=="Monitor")
    {
    std::vector<std::string> args;
    SPluginHelpers::split(oLog,"\t",args,4);

    if(args.size()<4)
      return;

    const std::string oError=args[3];

    if(oError.find("connections")!=std::string::npos)
      ExtCloudWatch.MonitorMachine(oError);
    else if(oError.find("process")!=std::string::npos)
      ExtCloudWatch.MonitorProcess(oError);
    }
  else if(oLog.find("reject")!=std::string::npos&&oLog.find(ExtManagerInfo.ip)==std::string::npos)
    {
    std::vector<std::string> args;
    SPluginHelpers::split(oLog,"\t",args,4);

    if(args.size()<4)
      return;

    std::string oLogin;
    std::string oSymbol;
    const std::string oError=args[3];
    args.clear();

    SPluginHelpers::split(oError," ",args,10);
    for(std::vector<std::string>::size_type i=0; i<args.size(); i++)
      {
      if(args[i].find("'")!=std::string::npos)
        {
        oLogin=args[i];
        oLogin.erase(std::remove(oLogin.begin(),oLogin.end(),'\''),oLogin.end());

        if(args.size()>=(i+3))
          {
          oSymbol=args[i+3];
          break;
          }
        }
      }

#pragma region why follow from logs?
    //TODO (Request->ID)
    //if(!oLogin.empty()&&!oSymbol.empty())
    //  {
    //  std::string uniqueID=oLogin+std::string("|")+oSymbol;
    //  LOG_DEBUG(pluginLog,"uniqueID: %s",uniqueID.c_str());

    //  if(fxiOrdersMap.has(uniqueID))
    //    {
    //    Ack execReport=fxiOrdersMap.get(uniqueID);
    //    TradeResponse tRes=responseMap[execReport.request.id];
    //    FIX::SessionID sID=execReport.execReport.getSessionID();
    //    FIX::Message m(execReport.AsER());
    //    m.setField(7573,oLogin);

    //    /*if(fixClient->SendDK(tRes._ClOrdID,DKReasonCode::DONT_KNOW_ORDER,m,FIX::SessionID(sID.getBeginString(),sID.getTargetCompID(),sID.getSenderCompID()),"Order rejected by 3rd party."))
    //      return;*/

    //      // if here, DK failed and will log RISK instead
    //    LOG_RISK(pluginLog,"%s",oError.c_str());
    //    return;
    //    }
    //  }

    // if here, previous logic failed and will log instead
    //LOG_RISK(pluginLog, "%s", oError.c_str());
#pragma endregion
    }
  }

/********************************************************************************************************************************************\
|	Purpose - notification about adding a new quote to the price stream of the trade server.
|	  the hook is called after a new quote has been added to the price stream as a result of filtering and multiple checks.
|
|	Parameters:
|	  symbol - symbol description
|	  inf - quote description
|
|	Important: Calling for method HistoryAddTick from this hook will form a recursion.
\********************************************************************************************************************************************/
void APIENTRY iHistoryTickApply(const ConSymbol *symbol,FeedTick *inf)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;

  //pricesMap[symbol->symbol]=*inf;
  }
//+------------------------------------------------------------------+
//| Notifies that the generation of daily or monthly reports on all  |
//| clients has completed on the server.                             |
//+------------------------------------------------------------------+
void APIENTRY iStatementsBaseAll(const int month_state,const GroupState *groups,const int groups_total,const DailyReport *reports,const int reports_total)
  {
  //--- checks
  if(!ExtGateway)
    return;
  //---- params check
  if(month_state==FALSE)
    ExtGateway->OnEODFinish(groups,groups_total,reports,reports_total);
  }
//+------------------------------------------------------------------+
//| Launches scheduled tasks on the server. The hook is called       |
//| periodically by the server to start external scheduled tasks.    |
//| MtSrvScheduler is called by the server approximately once every  |
//| 25 seconds, however the call precision is not guaranteed.  		|
//+------------------------------------------------------------------+
void APIENTRY iScheduler(const time_t curtime)
  {
  //---
  if(ExtGateway)
    ExtGateway->Scheduler(curtime);
  }
//+------------------------------------------------------------------+
//| Periodically starts service logic on the trade server. This hook |
//| can be used, for example, to monitor network operations and      |
//| streams in the plugin.                                           |
//| MtSrvService is called by the server approximately once every    |
//| 100 milliseconds, however the call precision is not guaranteed.	|
//+------------------------------------------------------------------+
void APIENTRY iService(const DWORD curtime)
  {
  //---
  if(ExtGateway)
    ExtGateway->Check(curtime);
  }
//+------------------------------------------------------------------+
//| Gets information about the overall system performance. The hook  |
//| is called once per minute.                                       |
//+------------------------------------------------------------------+
void APIENTRY iPerformance(const PerformanceInfo *inf)
  {
  if(!inf)
    return;
  //--- process
  ExtCloudWatch.MonitorMachine(inf);
  }
/********************************************************************************************************************************************\
|	Purpose - provides the ability to extend protocol between MetaTrader Server and MetaTrader Manager API.
|
|	Parameters:
|	  ip - IP address of the remote connection
|	  buf - buffer with the client's request also used for reply
|	  data_len - size of the buffer
|
|	Return values:
|	  data - data_len must be defined
\********************************************************************************************************************************************/
int APIENTRY iManagerProtocol(const ULONG ip,const UserInfo *us,const unsigned char *in_data,const int in_size,unsigned char **out_data,int *out_size)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;

  std::string request((const char *)in_data,in_size);

  struct in_addr addr;
  addr.s_addr=ip;
  char *dot_ip=inet_ntoa(addr);

  if(us->login!=ExtPluginContext.ManagerUI())
    {
    LOG_INFO(pluginLog,"Ignoring unauthorized manager protocol %d (%s): %s",us->login,dot_ip,request.c_str());
    return FALSE;
    }

  int oVersion=1;
  std::string oResponse;
  std::vector<std::string> args;
  SPluginHelpers::split(request,"|",args,3);

  if(args[0]=="idc-Ping")
    {
    oResponse="Pong";
    }
  else if(args[0]=="idc-Validate")
    {
    std::string oJSON;

    oJSON+="{";
    {
    oJSON+="\"API\":\""+std::to_string(oVersion)+"\",";
    oJSON+="\"IDC\":\""+(std::string("MT ")+std::string(VERSION_STRING))+"\",";
    }
    oJSON+="}";

    LOG_INFO(pluginLog,"Latest Version: %s / Installed Version: %s",args[1].c_str(),(std::string("MT ")+std::string(VERSION_STRING)).c_str());
    oResponse=oJSON;
    }
  else if(args[0]=="idc-Sync")
    {
    oResponse=ExtGateway->GetConfigs();
    }
  else if(args[0]=="idc-Ack")
    {
    LOG_INFO(pluginLog,"New MarginFX UI user: %s / %d@%s",args[1].c_str(),us->login,dot_ip);
    oResponse="done";
    }
  else if(args[0]=="idc-SetFIXCCY")
    {
    if(ExtGateway->SetInstrument(args[1],args[2],true,false))
      oResponse="done";
    else
      oResponse="error";
    }
  else if(args[0]=="idc-SetGroup")
    {
    if(ExtGateway->SetGroup(args[1],args[2],true,false))
      oResponse="done";
    else
      oResponse="error";
    }
  else if(args[0]=="idc-SetRule")
    {
    StringType2048 temp;
    //---
    temp.Assign(args[2].c_str());
    if(ExtGateway->SetRule(std::stoi(args[1]),temp,true,false))
      oResponse="done";
    else
      oResponse="error";
    }
  else if(args[0]=="idc-Restart")
    {
    ExtServer->ServerRestart();
    oResponse="done";
    }

  oResponse=SPluginHelpers::ReplaceStr(oResponse,",}","}");

  if(!oResponse.empty())
    {
    if(oResponse!="Pong")
      {
      LOG_INFO(pluginLog,"'%d' %s [%s] : [%s]",us->login,dot_ip,request.c_str(),oResponse.c_str());
      }

    /* return success message to FixServer */
    *out_data=(unsigned char *)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,oResponse.length()+1);
    strcpy((char *)*out_data,oResponse.c_str());
    *out_size=oResponse.length()+1;

    return TRUE;
    }

  /* Unknown message, return 0 */
  return FALSE;
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int APIENTRY iGroupsAdd(ConGroup *group)
  {
  if(!ExtGateway||!group)
    return(FALSE);
  //---
  if(!ExtGateway->UpdateGroupCache(group))
    {
    LOG_ERROR(pluginLog,"group update '%s' failed",group->group);
    return(FALSE);
    }
  //---
  LOG_INFO(pluginLog,"group updated '%s' failed",group->group);
  //---
  return(TRUE);
  }
//+------------------------------------------------------------------+
