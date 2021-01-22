//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
//---
#include "stdafx.h"
#include "TradeDispatcher.h"
#include "..\..\IntegralAPI\IntegralAPI.h"
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
CTradeDispatcher::CTradeDispatcher()
  :m_api_integral(NULL),
  m_request_checktime(_time64(NULL)+CHECK_TIMEOUT)
  {
  }
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
CTradeDispatcher::~CTradeDispatcher()
  {
  //---
  m_sync.Lock();
  //--- write to file
  RequestsWrite();
  //--- unlock
  m_sync.Unlock();
  //--- shutdown everything
  Shutdown();
  }
//+------------------------------------------------------------------+
//| Write requests                                                   |
//+------------------------------------------------------------------+
void CTradeDispatcher::RequestsWrite()
  {
  CMTFile          writer;
  StringTypePath   file_name;
  StringTypePath   directory;
  TradeRequest *ptr;
  //--- root directory
  directory.Format("%s\\%s",ExtProgramPath,"WatchTower");
  LOG_INFO(pluginLog,"checking directory %s",directory.Str());
  //--- check directory
  if(!CMTFile::DirectoryCreate(directory))
    {
    LOG_ERROR(pluginLog,"%s directory create error",directory.Str());
    return;
    }
  //---
  file_name.Format("%s\\requests.dat",directory.Str());
  if(!writer.OpenWrite(file_name.Str()))
    {
    LOG_ERROR(pluginLog,"failed to open file: %s <%d>",
              file_name.Str(),
              ::GetLastError());
    return;
    }
  //--- write  
  for(UINT i=0;i<m_requests.Total();i++)
    {
    ptr=&m_requests[i];
    if(ptr->complete_event!=NULL)
      {
      CloseHandle(ptr->complete_event);
      ptr->complete_event=NULL;
      }
    //--- only process orders
    if(ptr->order<1) continue;
    //---
    switch(ptr->order_state)
      {
      case TradeRequest::ORDER_STATE_PLACED:
      case TradeRequest::ORDER_STATE_PARTIAL:
        break;
      default:
        continue;
      }
    //--- set request id
    ptr->request_id=(ptr->request_id>0)?-ptr->order:ptr->request_id;
    //---
    if(!ptr->Write(writer))
      {
      LOG_ERROR(pluginLog,"error saving clordid: %s order:%d id:%d",
                ptr->unique_id,
                ptr->order,
                ptr->request_id);
      }
    }
  writer.Close();
  }
//+------------------------------------------------------------------+
//| Read requests                                                    |
//+------------------------------------------------------------------+
bool CTradeDispatcher::RequestsRead()
  {
  CMTFile             reader;
  StringTypePath      file_name;
  StringTypePath      directory;
  TradeRequest        request={0};
  //--- root directory
  directory.Format("%s\\%s",ExtProgramPath,"WatchTower");
  LOG_INFO(pluginLog,"checking directory %s",directory.Str());
  //--- check directory
  if(!CMTFile::DirectoryCreate(directory))
    {
    LOG_ERROR(pluginLog,"%s directory create error",directory.Str());
    return(false);
    }
  //---
  file_name.Format("%s\\requests.dat",directory.Str());
  //--- under lock
  m_sync.Lock();
  if(!reader.OpenRead(file_name.Str()))
    {
    m_sync.Unlock();
    return(false);
    }
  //--- read
  do
    {
    //--- add request
    ZeroMemory(&request,sizeof(request));
    //--- read the record
    if(reader.Read(&request,sizeof(request))!=sizeof(request))
      break;
    //---
    if(request.IsEmpty()) continue;
    //---
    request.request_id=request.request_id;
    //request.record=record;
    if(m_requests.Insert(&request,SortRequests)==NULL)
      {
      LOG_ERROR(pluginLog,"request can not be added to list { %s }",request.unique_id);
      }
    //---
    } while(!request.IsEmpty());
    //--- close
    reader.Close();
    //--- delete 
    DeleteFile(file_name.Str());
    //---
    m_sync.Unlock();
    //---
    return(true);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CTradeDispatcher::CheckRequest(const INT request_id)
  {
  //---
  m_sync.Lock();
  //--- exist?
  if(RequestGetPtr(request_id)==NULL)
    {
    m_sync.Unlock();
    return(false);
    }
  //---
  m_sync.Unlock();
  return(false);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CTradeDispatcher::CheckRequest(const INT login,LPCSTR symbol)
  {
  TradeRequest *ptr;
  //--- checks
  if(!symbol||login<1)
    return(false);
  //---
  m_sync.Lock();
  for(UINT i=0;i<m_requests.Total();i++)
    {
    ptr=&m_requests[i];
    if(ptr->login!=login||
       StringType::Compare(ptr->symbol,symbol)!=0)
      continue;
    //---
    m_sync.Unlock();
    return(true);
    }
  //---
  m_sync.Unlock();
  return(false);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CTradeDispatcher::CheckRequestByOrder(const INT order)
  {
  TradeRequest *ptr;
  bool          res=false;
  //--- checks
  if(order<1||!m_api_integral)
    return(false);
  //---
  m_sync.Lock();
  for(UINT i=0;i<m_requests.Total();i++)
    {
    ptr=&m_requests[i];
    if(ptr->order!=order)
      continue;
    //---
    res=true;
    break;
    }
  //---
  m_sync.Unlock();
  //--- send order status request
  if(res)
    m_api_integral->SendOrderStatusRequest(ptr->order_str);
  //---
  return(res);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CTradeDispatcher::CheckRequestByOrder(const INT order,const INT order_by)
  {
  TradeRequest *ptr;
  //--- checks
  if(order<1||order_by)
    return(false);
  //---
  m_sync.Lock();
  for(UINT i=0;i<m_requests.Total();i++)
    {
    ptr=&m_requests[i];
    if(!(ptr->order==order||ptr->order==order_by))
      continue;
    //---
    m_sync.Unlock();
    return(true);
    }
  //---
  m_sync.Unlock();
  return(false);
  }
//+------------------------------------------------------------------+
//| check pending requests of the client                             |
//+------------------------------------------------------------------+
bool CTradeDispatcher::CheckPendingRequests(const int login)
  {
  TradeRequest *ptr;
  //--- checks
  if(login<1)
    return(false);
  //--- 
  m_sync.Lock();
  //---
  for(UINT pos=0;pos<m_requests.Total();pos++)
    {
    ptr=&m_requests[pos];
    if(ptr->login==login&&ptr->order_action==TradeRequest::IDC_PENDING)
      {
      //---
      m_sync.Unlock();
      return(false);
      }
    }
  //---
  m_sync.Unlock();
  return(true);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CTradeDispatcher::RemoveRequest(const INT request_id)
  {
  TradeRequest *ptr;
  bool success;
  //---
  m_sync.Lock();
  //--- exist?
  if((ptr=RequestGetPtr(request_id))==NULL)
    {
    m_sync.Unlock();
    return(false);
    }
  //---
  success=m_requests.Delete(ptr);
  m_sync.Unlock();
  return(success);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CTradeDispatcher::RemoveRequest(const INT request_id,TradeRequest &record)
  {
  TradeRequest *ptr;
  //---
  m_sync.Lock();
  //--- exist?
  if((ptr=RequestGetPtr(request_id))==NULL)
    {
    m_sync.Unlock();
    return(false);
    }
  //---
  record=*ptr;
  m_requests.Delete(ptr);
  m_sync.Unlock();
  return(true);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CTradeDispatcher::RemoveRequestByOrder(const INT order,TradeRequest &record)
  {
  TradeRequest *ptr;
  //--- checks
  if(order<1)
    return(false);
  //---
  m_sync.Lock();
  //--- exist?
  if((ptr=RequestGetPtrByOrder(order))==NULL)
    {
    m_sync.Unlock();
    return(false);
    }
  //---
  record=*ptr;
  m_requests.Delete(ptr);
  m_sync.Unlock();
  return(true);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CTradeDispatcher::GetRequest(LPCSTR unique_id,TradeRequest &record)
  {
  TradeRequest *ptr;
  //--- checks
  if(!unique_id)
    return(false);
  //---
  m_sync.Lock();
  for(UINT i=0;i<m_requests.Total();i++)
    {
    ptr=&m_requests[i];
    if(StringType::Compare(ptr->unique_id,unique_id)!=0)
      continue;
    //---
    record=*ptr;
    //---
    m_sync.Unlock();
    return(true);
    }
  //---
  m_sync.Unlock();
  return(false);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CTradeDispatcher::GetRequestByOrder(const INT order,TradeRequest &record)
  {
  TradeRequest *ptr;
  //--- checks
  if(order<1)
    return(false);
  //---
  m_sync.Lock();
  for(UINT i=0;i<m_requests.Total();i++)
    {
    ptr=&m_requests[i];
    if(ptr->order!=order)
      continue;
    //---
    record=*ptr;
    //---
    m_sync.Unlock();
    return(true);
    }
  //---
  m_sync.Unlock();
  return(false);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CTradeDispatcher::GetRequest(const INT request_id,TradeRequest &record)
  {
  TradeRequest *ptr;
  //---
  m_sync.Lock();
  //--- exist?
  if((ptr=RequestGetPtr(request_id))!=NULL)
    {
    record=*ptr;
    //----
    m_sync.Unlock();
    return(true);
    }
  //---
  m_sync.Unlock();
  return(false);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CTradeDispatcher::GetRequest(const INT delay,TradeRequestArray &records)
  {
  time_t        currtime;
  TradeRequest *ptr;
  //--- clear
  records.Clear();
  //--- get the current time
  currtime=ExtServer->TradeTime();
  //---
  m_sync.Lock();
  for(UINT i=0;i<m_requests.Total();i++)
    {
    ptr=&m_requests[i];
    if(ptr->time_setup+delay<currtime||ptr->status_request>5)
      continue;
    //---
    ptr->status_request++;
    //--- add to list
    records.Add(ptr);
    }
  //---
  m_sync.Unlock();
  }
//+------------------------------------------------------------------+
//| Initialization                                                   |
//+------------------------------------------------------------------+
bool CTradeDispatcher::Initialize(CIntegralAPI *api_integral)
  {
  bool                  res=true;
  //--- check
  if(!api_integral||!ExtServer)
    return(false);
  //--- shutdown dispatcher
  Shutdown();
  //--- remember pointers to Exchange API
  m_api_integral=api_integral;
  //--- init databases
  //if(!m_integral_response.Initialize())
  //  {
  //  ExtServer->LogsOut(CmdErr,"","initialize response base error");
  //  return(false);
  //  }
  /*if(!m_pendings.Initialize())
    {
    ExtServer->LogsOut(CmdErr,"","initialize SL/TP base error");
    return(false);
    }*/
    //--- add them to list
    //---
  RequestsRead();
  //--- return result
  return(res);
  }
//+------------------------------------------------------------------+
//| Shutdown                                                         |
//+------------------------------------------------------------------+
void CTradeDispatcher::Shutdown()
  {
  //--- clear pointers to integral API
  m_api_integral=NULL;
  //---
  m_sync.Lock();
  //--- clean base
  m_requests.Clear();
  m_sync.Unlock();
  }
//+------------------------------------------------------------------+
//| Process selection of request                                     |
//+------------------------------------------------------------------+
bool CTradeDispatcher::OnRequestLocked(TradeRequest *record)
  {
  //--- log full info about received request
  LogRequest(record);
  //--- process request
  return(GatewayProcess(record));
  }
//+------------------------------------------------------------------+
//| Process selection of request                                     |
//+------------------------------------------------------------------+
bool CTradeDispatcher::OnRequestLocked(RequestInfo *request,const UINT action)
  {
  //--- log full info about received request
  //LogRequest(record);
  //--- process request
  return(GatewayExecuteIntegral(request,action));
  }
//+------------------------------------------------------------------+
//| Notify of spread                                                 |
//+------------------------------------------------------------------+
void CTradeDispatcher::OnSpreadReceived(LPCSTR symbol,const double prices[2])
  {
  //--- checks
  if(!symbol)
    return;
  //---
  try
    {
    m_spreads.set(symbol,std::pair<double,double>(prices[0],prices[1]));
    }
  catch(const std::exception &e)
    {
    LOG_ERROR(pluginLog,"spread add failed for %s error:<%s>",symbol,e.what());
    }
  }
//+------------------------------------------------------------------+
//| Notify of deal transaction in external trading system            |
//+------------------------------------------------------------------+
bool CTradeDispatcher::OnExecutionReportTrans(const FIX43::ExecutionReport &report,const Instrument &symbol)
  {
  return(SendDealExecution(report,symbol));
  }
//+------------------------------------------------------------------+
//| Notify of  orderslogon of external trading system                |
//+------------------------------------------------------------------+
void CTradeDispatcher::OnLogon(PCSTR short_name)
  {
  TradeRequest *ptr;
  //---
  if(!m_api_integral||!short_name)
    return;
  //--- under lock
  m_sync.Lock();
  for(UINT i=0;i<m_requests.Total();i++)
    {
    ptr=&m_requests[i];
    if(StringType::Compare(ptr->short_name,short_name)==0)
      {
      //--- reset counter
      ptr->status_request=0;
      m_api_integral->SendOrderStatusRequest(ptr->order_str);
      }
    }
  //---
  m_sync.Unlock();
  }
//+------------------------------------------------------------------+
//| Process requests                                                 |
//+------------------------------------------------------------------+
bool CTradeDispatcher::GatewayProcess(TradeRequest *record)
  {
  //--- checks
  if(!record)
    return(false);
  //--- check if a request can be confirmed automatically (we will implement later)
  if(record->origin==TradeRequest::ORIGIN_ORDER)
    {
    if(GatewayExecuteIntegralAsync(record))
      {
      //--- confirm request  
      return(true);
      }
    return(false);
    }
  //--- execute request at integral
  if(!GatewayExecuteIntegral(record))
    {
    //--- unable to send request to integral system, send request error
    return(false);
    }
  //--- ok confirm it
  return(true);
  }
//+------------------------------------------------------------------+
//| Check if request can be executed automatically                   |
//+------------------------------------------------------------------+
bool CTradeDispatcher::GatewayProcessAuto(const RequestInfo *request)
  {
  bool stop_order=false,confirm_auto=false;
  //--- check
  if(!request)
    return(false);
  //--- generate the flag on whether it is a request for a stop order or for a stop limit order
  if(request->trade.cmd==OP_BUY_STOP||
     request->trade.cmd==OP_SELL_STOP)
    {
    stop_order=true;
    };
  //--- analyze action types to generate the flag on whether a request can be confirmed automatically
  UINT action=request->trade.type;
  switch(action)
    {
    //--- only pending stop orders are confirmed automatically
    case TT_ORDER_PENDING_OPEN:
      {
      confirm_auto=true;
      break;
      }
      //--- automatically confirm closeby order
    case TT_ORDER_CLOSE_BY:
    case TT_ORDER_CLOSE_ALL:
    case TT_BR_ORDER_CLOSE_BY:
    case TT_BR_ORDER_CLOSE_ALL:
      {
      confirm_auto=true;
      break;
      }
      //--- modification
    case TT_ORDER_MODIFY:
    case TT_BR_ORDER_MODIFY:
      confirm_auto=true;
      break;
      //--- only stop orders are removed automatically
    case TT_ORDER_DELETE:
    case TT_BR_ORDER_DELETE:
      {
      confirm_auto=true;
      break;
      }
    }
  //--- return result
  return(confirm_auto);
  }
//+------------------------------------------------------------------+
//| Execute request at exchange                                      |
//+------------------------------------------------------------------+
bool CTradeDispatcher::GatewayExecuteIntegral(TradeRequest *request)
  {
  IntegralOrder  order={};
  TradeRequest *ptr=NULL;
  TradeRecord    old_trade={0};
  StringType32   text;
  HANDLE         event;
  //--- checks
  if(!request||!ExtServer||!m_api_integral)
    return(false);
  //--- swap type of operation for CLOSE transactions
  if(request->order_action==TradeRequest::IDC_CLOSE||
     request->order_action==TradeRequest::IDC_TP||
     request->order_action==TradeRequest::IDC_SL||
     request->order_action==TradeRequest::IDC_STOPOUT)
    {
    //--- get order 
    if(ExtServer->OrdersGet(request->order,&old_trade)!=TRUE)
      {
      LOG_ERROR(pluginLog,"'%d' orders get failed #%d",request->login,request->order);
      return(false);
      }
    //--- check already closed
    if(old_trade.close_time>0)
      {
      LOG_ERROR(pluginLog,"'%d' order already closed #%d",request->login,request->order);
      return(false);
      }
    }
  //--- under lock
  m_sync.Lock();
  //--- exist?
  if((ptr=RequestGetPtr(request->request_id))!=NULL)
    {
    //--- send 35=H message
    if(!m_api_integral->SendOrderStatusRequest(ptr->order_str))
      {
      LOG_ERROR(pluginLog,"send order status request failed <%s>",ptr->order_str);
      }
    //---
    m_sync.Unlock();
    return(false);
    }
  //--- fill the record
  if(!IntegralOrder::Fill(request,order))
    {
    LOG_ERROR(pluginLog,"Filling order failed for ReqID: %d, Symbol: %s, Price: %.5lf",
              request->request_id,
              request->symbol,
              request->price_order);
    //---
    m_sync.Unlock();
    return(false);
    }
  //--- set external id
  StringType::Copy(request->unique_id,order.unique_id);
  //--- set setup time
  request->time_setup=_time64(NULL);//ExtServer->TradeTime();
  //--- set prices
  if(m_spreads.has(order.symbol))
    {
    order.prices[0]=m_spreads[order.symbol].first;
    order.prices[1]=m_spreads[order.symbol].second;
    }
  //--- send order to intergral
  order.ie_deviation=request->ie_deviation;
  request->order_state=TradeRequest::ORDER_STATE_SENDING;
  //--- add request
  if((ptr=m_requests.Insert(request,SortRequests))==NULL)
    {
    m_sync.Unlock();
    return(false);
    }
  //---
  if((ptr->complete_event=CreateEvent(NULL,FALSE,FALSE,NULL))==NULL)
    {
    LOG_ERROR(pluginLog,"Trades: event creation error");
    //--- delete added
    m_requests.Delete(ptr);
    //---
    m_sync.Unlock();
    return(false);
    }
  //---
  event=ptr->complete_event;
  //---
  if(!m_api_integral->SendOrder(order))
    {
    LOG_ERROR(pluginLog,"'%d' Trades: order sending failed [reuest ID: #%d]",order.order,order.request_id);
    //---
    /*if(ptr->complete_event!=NULL)
      ResetEvent(ptr->complete_event);*/
      //--- delete added
    m_requests.Delete(ptr);
    //---
    m_sync.Unlock();
    return(false);
    }
  //--- send
  ptr->order_state=TradeRequest::ORDER_STATE_PLACED;
  StringType::Copy(ptr->order_str,order.order_str);
  StringType::Copy(ptr->short_name,order.short_name);
  ptr->session_id=order.session_id;
  //---
  m_sync.Unlock();
  //--- wait result
  if(WaitForSingleObject(event,MAX_WAIT_TIME)!=WAIT_OBJECT_0)
    {
    LOG_ERROR(pluginLog,"Trades: '%d' no answer [timeout] in queue #%d <id:%d> %s",
              order.login,
              order.order,
              order.request_id,
              order.unique_id);
    request->order_state=TradeRequest::ORDER_STATE_TIMEOUT;
    //---
    return(false);
    }
  //--- ok, find result
  m_sync.Lock();
  if((ptr=RequestGetPtr(request->request_id))!=NULL)
    {
    //--- set info
    request->result_volume=ptr->result_volume;
    request->result_price =ptr->result_price;
    request->order_state  =ptr->order_state;
    request->fix_qty      =ptr->fix_qty;
    StringType::Copy(request->order_id,ptr->order_id);
    StringType::Copy(request->order_str,ptr->order_str);
    StringType::Copy(request->exec_id,ptr->exec_id);
    StringType::Copy(request->exec_str,ptr->exec_str);
    StringType::Copy(request->text,ptr->text);
    //---
    m_sync.Unlock();
    return(true);
    }
  //ExtLogger.Out(CmdAtt,"could not find <%s> %d",order.unique_id,m_requests_total);
  //---
  request->order_state=TradeRequest::ORDER_STATE_NOT_FOUND;
  //--- unlock
  m_sync.Unlock();
  //--- we didn't find anything-error!
  return(false);
  }
//+------------------------------------------------------------------+
//| Execute request at exchange                                      |
//+------------------------------------------------------------------+
bool CTradeDispatcher::GatewayExecuteIntegral(RequestInfo *request,const UINT action)
  {
  IntegralOrder  order={};
  TradeRequest   trade={0},*ptr=NULL;
  TradeRecord    old_trade={0};
  StringType32   text;
  HANDLE         event;
  //--- checks
  if(!request||!ExtServer||!m_api_integral)
    return(false);
  //--- swap type of operation for CLOSE transactions
  if(action==TradeRequest::IDC_CLOSE||
     action==TradeRequest::IDC_TP||
     action==TradeRequest::IDC_SL||
     action==TradeRequest::IDC_STOPOUT)
    {
    //--- get order 
    if(ExtServer->OrdersGet(request->trade.order,&old_trade)!=TRUE)
      {
      LOG_ERROR(pluginLog,"'%d' orders get failed #%d",request->login,request->trade.order);
      return(false);
      }
    //--- check already closed
    if(old_trade.close_time>0)
      {
      LOG_ERROR(pluginLog,"'%d' order already closed #%d",request->login,request->trade.order);
      return(false);
      }
    }
  //--- under lock
  m_sync.Lock();
  //--- exist?
  if((ptr=RequestGetPtr(request->id))!=NULL)
    {
    //--- send 35=H message
    if(!m_api_integral->SendOrderStatusRequest(ptr->order_str))
      {
      LOG_ERROR(pluginLog,"send order status request failed <%s>",ptr->order_str);
      }
    //---
    m_sync.Unlock();
    return(false);
    }
  //---
  if(!TradeRequest::Fill(request,action,trade))
    {
    m_sync.Unlock();
    return(false);
    }
  //--- fill the record
  if(!IntegralOrder::Fill(&trade,order))
    {
    LOG_ERROR(pluginLog,"Filling order failed for ReqID: %d, Symbol: %s, Price: %.5lf",
              request->id,
              request->trade.symbol,
              request->trade.order);
    //---
    m_sync.Unlock();
    return(false);
    }
  //--- set external id
  StringType::Copy(trade.unique_id,order.unique_id);
  //--- set setup time
  trade.time_setup=_time64(NULL);//ExtServer->TradeTime();
  //--- set prices
  if(m_spreads.has(order.symbol))
    {
    order.prices[0]=m_spreads[order.symbol].first;
    order.prices[1]=m_spreads[order.symbol].second;
    }
  //--- send order to intergral
  order.ie_deviation=trade.ie_deviation;
  trade.order_state=TradeRequest::ORDER_STATE_SENDING;
  //--- add request
  if((ptr=m_requests.Insert(&trade,SortRequests))==NULL)
    {
    m_sync.Unlock();
    return(false);
    }
  //---
  if((ptr->complete_event=CreateEvent(NULL,FALSE,FALSE,NULL))==NULL)
    {
    LOG_ERROR(pluginLog,"Trades: event creation error");
    m_requests.Delete(ptr);
    //---
    m_sync.Unlock();
    return(false);
    }
  //---
  event=ptr->complete_event;
  //---
  if(!m_api_integral->SendOrder(order))
    {
    LOG_ERROR(pluginLog,"'%d' Trades: order sending failed [reuest ID: #%d]",order.order,order.request_id);
    //--- delete added
    m_requests.Delete(ptr);
    //---
    m_sync.Unlock();
    return(false);
    }
  //--- send
  ptr->order_state=TradeRequest::ORDER_STATE_PLACED;
  StringType::Copy(ptr->order_str,order.order_str);
  StringType::Copy(ptr->short_name,order.short_name);
  ptr->session_id=order.session_id;
  //---
  m_sync.Unlock();
  //--- wait result
  if(WaitForSingleObject(event,MAX_WAIT_TIME)!=WAIT_OBJECT_0)
    {
    LOG_ERROR(pluginLog,"Trades: '%d' no answer [timeout] in queue #%d <id:%d> %s",
              order.login,
              order.order,
              order.request_id,
              order.unique_id);
    ptr->order_state=TradeRequest::ORDER_STATE_TIMEOUT;
    //---
    return(false);
    }
  //--- ok, find result
  m_sync.Lock();
  if((ptr=RequestGetPtr(request->id))!=NULL)
    {
    //--- set info
    /*request->result_volume=ptr->result_volume;
    request->result_price =ptr->result_price;
    request->order_state  =ptr->order_state;
    request->fix_qty      =ptr->fix_qty;
    StringType::Copy(request->order_id,ptr->order_id);
    StringType::Copy(request->order_str,ptr->order_str);
    StringType::Copy(request->exec_id,ptr->exec_id);
    StringType::Copy(request->exec_str,ptr->exec_str);
    StringType::Copy(request->text,ptr->text);*/
    //---
    m_sync.Unlock();
    return(true);
    }
  //ExtLogger.Out(CmdAtt,"could not find <%s> %d",order.unique_id,m_requests_total);
  //---
  //request->order_state=TradeRequest::ORDER_STATE_NOT_FOUND;
  //--- unlock
  m_sync.Unlock();
  //--- we didn't find anything-error!
  return(false);
  }
//+------------------------------------------------------------------+
//| Execute request at exchange                                      |
//+------------------------------------------------------------------+
bool CTradeDispatcher::GatewayExecuteIntegralAsync(TradeRequest *request)
  {
  IntegralOrder  order={};
  TradeRecord    old_trade={0};
  TradeRequest *ptr;
  StringType32   text;
  //--- checks
  if(!ExtServer||!m_api_integral||!request)
    return(false);
  //--- check order for CLOSE transactions
  if(request->order_action==TradeRequest::IDC_CLOSE||
     request->order_action==TradeRequest::IDC_TP||
     request->order_action==TradeRequest::IDC_SL||
     request->order_action==TradeRequest::IDC_STOPOUT)
    {
    //--- get order 
    if(ExtServer->OrdersGet(request->order,&old_trade)!=TRUE)
      {
      LOG_ERROR(pluginLog,"'%d' orders get failed #%d",request->login,request->order);
      return(false);
      }
    //--- check already closed
    if(old_trade.close_time>0)
      {
      LOG_ERROR(pluginLog,"'%d' order already closed #%d",request->login,request->order);
      return(false);
      }
    }
  //--- under lock
  m_sync.Lock();
  //--- exist?
  if((ptr=RequestGetPtr(request->request_id))!=NULL)
    {
    //--- send 35=H message
    if(!m_api_integral->SendOrderStatusRequest(ptr->order_str))
      {
      LOG_ERROR(pluginLog,"send order status request failed <%s>",ptr->order_str);
      }
    //---
    m_sync.Unlock();
    return(false);
    }
  //--- fill the record
  if(!IntegralOrder::Fill(request,order))
    {
    LOG_ERROR(pluginLog,"(%d) Filling order failed for ReqID: %d, Price: %.5lf",
              request->id,
              request->id,
              request->order);
    //---
    m_sync.Unlock();
    return(false);
    }
  //--- set external id
  StringType::Copy(request->unique_id,order.unique_id);
  //--- set time
  request->time_setup=_time64(NULL);
  //--- set order state
  request->order_state=TradeRequest::ORDER_STATE_SENDING;
  //--- add request
  if((ptr=m_requests.Insert(request,SortRequests))==NULL)
    {
    m_sync.Unlock();
    return(false);
    }
  //--- set prices
  if(m_spreads.has(request->symbol))
    {
    order.prices[0]=m_spreads[request->symbol].first;
    order.prices[1]=m_spreads[request->symbol].second;
    }
  //--- placed
  order.ie_deviation=request->ie_deviation;
  //--- send order to intergral
  if(!m_api_integral->SendOrder(order))
    {
    LOG_ERROR(pluginLog,"'%d' Trades: order sending failed [request ID: #%d]",order.order,order.request_id);
    //--- delete added
    m_requests.Delete(ptr);
    //---
    m_sync.Unlock();
    return(false);
    }
  StringType::Copy(ptr->order_str,order.order_str);
  StringType::Copy(ptr->short_name,order.short_name);
  ptr->order_state=TradeRequest::ORDER_STATE_PLACED;
  ptr->session_id =order.session_id;
  //--- successfull
  m_sync.Unlock();
  return(true);
  }
//+------------------------------------------------------------------+
//| Request confirmation                                             |
//+------------------------------------------------------------------+
bool CTradeDispatcher::SendRequestConfirm(const int retcode,const RequestInfo *request,const UserInfo &manager)
  {
  bool     res=false;
  double prices[2]={0};
  //--- check
  if(!ExtServer||!request)
    {
    LOG_ERROR(pluginLog,"failed to confirm trade request");
    return(false);
    }
  //--- set confirmation data
  prices[0]=request->prices[0];
  prices[1]=request->prices[1];
  res=(retcode==RET_OK)?true:false;
  //--- send confirmation
  if(res&&ExtServer->RequestsConfirm(request->id,&manager,prices)!=RET_OK)
    {
    LOG_ERROR(pluginLog,"'%d': request #%d failed to send confirm %d (%d)",request->login,request->id,retcode,res);
    res=false;
    }
  else
    {
    ExtServer->RequestsReset(request->id,&manager,DC_RESETED);
    LogAnswerGateway(request);
    }
  //--- return result
  return(res);
  }
//+------------------------------------------------------------------+
//| Send an order to MT                                              |
//+------------------------------------------------------------------+
bool CTradeDispatcher::SendOrderExecution(const TradeRequest &request,const Instrument &instrument,StringType &message)
  {
  bool        res=false;
  //--- process orders
  switch(request.order_action)
    {
    case TradeRequest::IDC_PENDING:
      res=ProcessPendingOrder(request,instrument,message);
      break;

    case TradeRequest::IDC_TP:
      res=ProcessTPActivation(request,instrument,message);
      break;

    case TradeRequest::IDC_SL:
      res=ProcessSLActivation(request,instrument,message);
      break;

    case TradeRequest::IDC_STOPOUT:
      res=ProcessPositionStopOut(request,instrument,message);
      break;

    case TradeRequest::IDC_OPEN:
      switch(request.reason)
        {
        case TradeRequest::ORDER_REASON_ACTIVATION:
          res=ProcessPendingOrderActivation(request,message);
          break;
        case TradeRequest::ORDER_REASON_CLIENT:
          res=ProcessOrderOpen(request,message);
          break;
        default:
          res=ProcessMarketOpen(request,message);
          break;
        }
      break;

    case TradeRequest::IDC_CLOSE:
      //res=ProcessMarketClose(request,message);
      switch(request.reason)
        {
        case TradeRequest::ORDER_REASON_CLIENT:
          res=ProcessOrderClose(request,message);
          break;
        case TradeRequest::ORDER_REASON_DEALER:
          res=ProcessMarketClose(request,message);
          break;
        default:
          break;
        }
      break;
    default:
      break;
    }
  //---
  return(res);
  }
//+------------------------------------------------------------------+
//| Process order fill                                               |
//+------------------------------------------------------------------+
bool CTradeDispatcher::SendDontKnowTrade(LPCSTR order_str,const UINT reason,LPCSTR text)
  {
  //--- checks
  if(!order_str||!text||!m_api_integral)
    return(false);
  //---
  return(m_api_integral->SendDontKnowTrade(order_str,reason,text));
  }
//+------------------------------------------------------------------+
//| Process order fill                                               |
//+------------------------------------------------------------------+
bool CTradeDispatcher::SendAck(LPCSTR unique_id)
  {
  //--- checks
  if(!unique_id||!m_api_integral)
    return(false);
  //---
  return(m_api_integral->SendAck(unique_id));
  }
//+------------------------------------------------------------------+
//| Return lot size for trade                                        |
//+------------------------------------------------------------------+
double CTradeDispatcher::GetLotSize(LPCSTR symbol,LPCSTR group_name,const double current_lots)
  {
  ConSymbol cs={0};
  ConGroup  group={0};
  //--- checks
  if(!ExtServer||!symbol||!group_name)
    {
    ExtLogger.Out(CmdAtt,"GetLotSize() smthg wrong");
    return(current_lots);
    }
  // snap lot size to broker-happy value
  if(ExtServer->SymbolsGet(symbol,&cs)==FALSE)
    {
    ExtLogger.Out(CmdAtt,"GetLotSize() SymbolsGet %s failed",symbol);
    return(current_lots);
    }
  //--- check group
  if(ExtServer->GroupsGet(group_name,&group)==FALSE)
    {
    ExtLogger.Out(CmdAtt,"GetLotSize() GroupsGet %s failed",group_name);
    return(current_lots);
    }
  //---
  double  lotStep=group.secgroups[cs.type].lot_step/100.0;
  double  minLot=group.secgroups[cs.type].lot_min/100.0;
  double  lots=round(current_lots/lotStep)*lotStep;
  //---
  if(lots<minLot)
    lots=minLot;
  //---
  return(lots);
  }
//+------------------------------------------------------------------+
//| Process order open transaction                                   |
//+------------------------------------------------------------------+
bool CTradeDispatcher::ProcessOrderOpen(const TradeRequest &request,StringType &message)
  {
  double        prices[2]={0};
  int           order=0;
  //---- cheks
  if(!m_api_integral||!ExtServer)
    return(false);
  //---
  switch(request.order_state)
    {
    case TradeRequest::ORDER_STATE_FILLED:
    case TradeRequest::ORDER_STATE_EXPIRED:
    case TradeRequest::ORDER_STATE_CANCELLED:
      prices[0]=request.result_price;
      prices[1]=request.result_price;
      //--- check volume
      if(request.result_volume<=0)
        {
        ExtLogger.Out(CmdAtt,"result volume is zero state:[%u] <%s>",
                      request.order_state,
                      request.unique_id);
        //--- send execution report
        if(!m_api_integral->SendAck(request.exec_str))
          {
          LOG_ERROR(pluginLog,"sending Ack failed <%s>",request.unique_id);
          }
        //---
        break;
        }
      //--- partial fill?
      if(request.volume>request.result_volume)
        {
        order=OrdersAddOpen(request.login,
                            request.cmd,
                            request.symbol,
                            request.result_price,
                            request.result_volume,
                            request.order_id,
                            request.price_sl,
                            request.price_tp);
        if(order<1)
          {
          message.Format("'%d' order open failed #%d",request.login,request.request_id);
          LOG_ERROR(pluginLog,message.Str());
          //--- send dont know
          if(!m_api_integral->SendDontKnowTrade(request.order_str,TradeRequest::DONT_KNOW_ORDER,message.Str()))
            {
            LOG_ERROR(pluginLog,"DontKnowTrade send failed <%s>",message.Str());
            }
          //---
          break;
          }
        //--- send acknowledgement
        if(!m_api_integral->SendAck(request.exec_str))
          {
          LOG_ERROR(pluginLog,"sending Ack failed <%s>",request.unique_id);
          }
        //--- write to report             
        message.Format("'%s' Open MT4 Position %d, FXI Execution ID %s, FXI Order ID %s, price %f, CumQty %0.2lf",
                       request.text,
                       order,
                       request.exec_id,
                       request.order_id,
                       request.result_price,
                       request.fix_qty);
        //--- show
        LOG_ORDER(orderLog,message.Str());
        break;
        }
      //--- confirm       
      return(true);

    case TradeRequest::ORDER_STATE_REJECTED:
    case TradeRequest::ORDER_STATE_TIMEOUT:
      //--- send acknowledgement
      if(!m_api_integral->SendAck(request.exec_str))
        {
        LOG_ERROR(pluginLog,"sending Ack failed <%s>",request.unique_id);
        }
      //---
      break;

    default:
      message.Format("'%d' request unknown state #%d",request.login,request.request_id);
      //--- send dont know trade
      if(!m_api_integral->SendDontKnowTrade(request.order_str,TradeRequest::DONT_KNOW_ORDER,message.Str()))
        {
        LOG_ERROR(pluginLog,"DontKnowTrade send failed <%s>",message.Str());
        }
      break;
    }
  //--- reset
  RemoveRequest(request.request_id);
  //---
  return(false);
  }
//+------------------------------------------------------------------+
//| Process order close transaction                                  |
//+------------------------------------------------------------------+
bool CTradeDispatcher::ProcessOrderClose(const TradeRequest &request,StringType &message)
  {
  double         prices[2]={0};
  //---- cheks
  if(!m_api_integral||!ExtServer)
    return(false);
  //--- process
  switch(request.order_state)
    {
    case TradeRequest::ORDER_STATE_FILLED:
    case TradeRequest::ORDER_STATE_EXPIRED:
    case TradeRequest::ORDER_STATE_CANCELLED:
      prices[0]=request.result_price;
      prices[1]=request.result_price;
      //--- check volume
      if(request.result_volume<=0)
        {
        ExtLogger.Out(CmdAtt,"if(request.request.trade.volume<=0) [%d] <%s>",
                      request.result_volume,
                      request.unique_id);
        //--- send execution report
        if(!m_api_integral->SendAck(request.exec_str))
          {
          LOG_ERROR(pluginLog,"sending Ack failed <%s>",request.unique_id);
          }
        //---
        break;
        }
      //--- partial fill?
      if(request.volume>request.result_volume)
        {
        if(!OrdersClose(request.order,request.result_volume,request.result_price,request.comment,request.order_id))
          {
          message.Format("'%d' order #%d close failed",request.login,request.order);
          ExtLogger.Out(CmdErr,message.Str());
          //---
          break;
          }
        //--- send acknowledgement
        if(!m_api_integral->SendAck(request.exec_str))
          {
          LOG_ERROR(pluginLog,"sending Ack failed <%s>",request.unique_id);
          }
        //--- send execution report             
        message.Format("'%s' Close MT4 Position %d, FXI Execution ID %s, FXI Order ID %s, price %f, CumQty %0.2lf",
                       request.text,
                       request.order,
                       request.exec_id,
                       request.order_id,
                       request.result_price,
                       request.fix_qty);
        //--- show
        LOG_ORDER(orderLog,message.Str());
        break;
        }
      //---
      return(true);

    case TradeRequest::ORDER_STATE_REJECTED:
    case TradeRequest::ORDER_STATE_TIMEOUT:
      //--- send Ack
      if(!SendAck(request.exec_str))
        {
        LOG_ERROR(pluginLog,"sending Ack failed <%s>",request.unique_id);
        }
      //---
      break;

    default:
      message.Format("'%d' request for order #%d not found <unkbown state>",request.login,request.order);
      if(!m_api_integral->SendDontKnowTrade(request.order_str,TradeRequest::DONT_KNOW_ORDER,message.Str()))
        {
        LOG_ERROR(pluginLog,"DontKnowTrade send failed <%s>",message.Str());
        }
      break;
    }
  //--- reset
  RemoveRequest(request.request_id);
  //---
  return(false);
  }
//+------------------------------------------------------------------+
//| Process manager order transaction                                |
//+------------------------------------------------------------------+
bool CTradeDispatcher::ProcessMarketOpen(const TradeRequest &request,StringType &message)
  {
  double prices[2]={0};
  int    order=0;
  //---- cheks
  if(!m_api_integral||!ExtServer)
    return(false);
  //--- process
  switch(request.order_state)
    {
    case TradeRequest::ORDER_STATE_FILLED:
    case TradeRequest::ORDER_STATE_EXPIRED:
    case TradeRequest::ORDER_STATE_CANCELLED:
      prices[0]=request.result_price;
      prices[1]=request.result_price;
      //--- check volume
      if(request.result_volume<=0)
        {
        //--- send execution report
        m_api_integral->SendAck(request.exec_str);
        //--- remove request from list
        RemoveRequest(request.request_id);
        //--- delete request
        ExtServer->RequestsDelete(request.request_id);
        //---
        return(true);
        }
      //--- confirm
      order=OrdersAddOpen(request.login,
                          request.cmd,
                          request.symbol,
                          request.result_price,
                          request.result_volume,
                          request.order_id,
                          request.price_sl,
                          request.price_tp);
      if(order<1)
        {
        message.Format("'%d' order open failed #%d",request.login,request.request_id);
        LOG_ERROR(pluginLog,message.Str());
        //---
        break;
        }
      //--- send execution report             
      message.Format("'%s' Open MT4 Position %d, FXI Execution ID %s, FXI Order ID %s, price %f, CumQty %0.2lf",
                     request.text,
                     order,
                     request.exec_id,
                     request.order_id,
                     request.result_price,
                     request.fix_qty);
      //--- show
      LOG_ORDER(orderLog,message.Str());
      //---
      m_api_integral->SendAck(request.exec_str);
      //---
      RemoveRequest(request.request_id);
      //--- delete request
      ExtServer->RequestsDelete(request.request_id);
      return(true);

    case TradeRequest::ORDER_STATE_REJECTED:
    case TradeRequest::ORDER_STATE_TIMEOUT:
      //--- send Ack
      m_api_integral->SendAck(request.exec_str);
      //---
      RemoveRequest(request.request_id);
      //--- delete request
      ExtServer->RequestsDelete(request.request_id);
      return(true);

    default:
      message.Format("'%d' request for order #%d not found <unkbown state>",request.login,request.order);
      break;
    }
  //--- delete request
  ExtServer->RequestsDelete(request.request_id);
  //---
  return(false);
  }
//+------------------------------------------------------------------+
//| Process manager order transaction                                |
//+------------------------------------------------------------------+
bool CTradeDispatcher::ProcessMarketClose(const TradeRequest &request,StringType &message)
  {
  double prices[2]={0};
  int    order=0;
  //---- cheks
  if(!m_api_integral||!ExtServer)
    return(false);
  //--- process
  switch(request.order_state)
    {
    case TradeRequest::ORDER_STATE_FILLED:
    case TradeRequest::ORDER_STATE_EXPIRED:
    case TradeRequest::ORDER_STATE_CANCELLED:
      prices[0]=request.result_price;
      prices[1]=request.result_price;
      //--- check volume
      if(request.result_volume<=0)
        {
        //--- send execution report
        m_api_integral->SendAck(request.exec_str);
        //--- remove request from list
        RemoveRequest(request.request_id);
        //--- delete request
        ExtServer->RequestsDelete(request.request_id);
        //---
        return(true);
        }
      //--- confirm
      if(!OrdersClose(request.order,request.result_volume,request.result_price,request.comment,request.order_id))
        {
        message.Format("'%d' order #%d close failed",request.login,request.order);
        ExtLogger.Out(CmdErr,message.Str());
        //---
        break;
        }
      //--- send execution report
      m_api_integral->SendAck(request.exec_str);
      //--- remove request from list
      RemoveRequest(request.request_id);
      //--- delete request          TODO: check this id
      ExtServer->RequestsDelete(request.request_id);
      return(true);
      //---
      break;

    case TradeRequest::ORDER_STATE_REJECTED:
    case TradeRequest::ORDER_STATE_TIMEOUT:
      //--- send Ack
      m_api_integral->SendAck(request.exec_str);
      //---
      RemoveRequest(request.request_id);
      //--- delete request
      ExtServer->RequestsDelete(request.request_id);
      return(true);

    default:
      message.Format("'%d' request for order #%d not found <unkbown state>",request.login,request.order);
      break;
    }
  //--- delete request
  ExtServer->RequestsDelete(request.request_id);
  //---
  return(false);
  }
//+------------------------------------------------------------------+
//| Process pending order transaction                                |
//+------------------------------------------------------------------+
bool CTradeDispatcher::ProcessPendingOrderActivation(const TradeRequest &request,StringType &message)
  {
  INT           login;
  StringType256 str;
  StringType64  price;
  StringType64  side;
  TradeRecord   record={0};
  //--- checks
  if(!ExtServer||!m_api_integral)
    return(false);
  //--- check login
  if((login=ExtServer->TradesFindLogin(request.order)!=request.login))
    {
    message.Format("'%d' login not found for #%d",request.login,request.order);
    LOG_ERROR(pluginLog,message.Str());
    //---
    return(false);
    }
  //Get the trade record with the specified ticket
  if(ExtServer->OrdersGet(request.order,&record)==FALSE)
    {
    message.Format("'%d' order get failed for #%d",login,request.order);
    LOG_ERROR(pluginLog,message.Str());
    //---
    return(false);
    }
  //--- activate pending order
  if(request.result_volume>0)
    {
    if(!OrdersUpdateActivate(request.order,request.result_volume,request.result_price,request.order_id))
      {
      //---
      message.Format("'%d' order update activation failed #%d",request.login,request.order);
      LOG_ERROR(pluginLog,message.Str());
      //--- delete request
      ExtServer->RequestsDelete(request.request_id);
      return(false);
      }
    //---
#pragma region MyRegion
    //str.Format("'%s' Open MT4 Position %d, FXI Execution ID %s, FXI Order ID %s, price %f, CumQty %0.2lf",
    //           request.text,
    //           request.order,
    //           request.exec_id,
    //           request.order_id,
    //           request.result_price,
    //           request.fix_qty);
    //LOG_ORDER(orderLog,str.Str());
#pragma endregion
    //--- open partial if necessarry
    if(request.volume>request.result_volume)
      if(!(OrdersAddOpenPending(&record,request.volume-request.result_volume,request.order_id)>0))
        {
        message.Format("'%d' order add open pending reamining part failed #%d",login,request.order);
        LOG_ERROR(pluginLog,message.Str());
        }
    }
  else
    {
    SMTFormat::FormatPrice(price,record.open_price,record.digits);
    SMTFormat::FormatTradeCommand(side,record.cmd);
    ExtLogger.Out(CmdTrade,"'%d': reject '%d' pending activation of %s <order #%d> %0.2lf %s at %s <execId:%s, orderId:%s>",
                  ExtManagerInfo.login,
                  record.login,
                  side.Str(),
                  record.order,
                  record.volume/100.0,
                  record.symbol,
                  price.Str(),
                  request.exec_id,
                  request.order_id);
    }
  //--- delete request
  ExtServer->RequestsDelete(request.request_id);
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| Process pending order transaction                                |
//+------------------------------------------------------------------+
bool CTradeDispatcher::ProcessPendingOrder(const TradeRequest &request,const Instrument &instrument,StringType &message)
  {
  INT           login;
  StringType256 str;
  StringType64  price;
  StringType64  side;
  TradeRecord   record={0};
  //--- checks
  if(!ExtServer||!m_api_integral)
    return(false);
  //--- check login
  if((login=ExtServer->TradesFindLogin(request.order)!=request.login))
    {
    message.Format("'%d' login not found for #%d",request.login,request.order);
    LOG_ERROR(pluginLog,message.Str());
    //---
    return(false);
    }
  //Get the trade record with the specified ticket
  if(ExtServer->OrdersGet(request.order,&record)==FALSE)
    {
    message.Format("'%d' order get failed for #%d",login,request.order);
    LOG_ERROR(pluginLog,message.Str());
    //---
    return(false);
    }
  //--- activate pending order
  if(request.result_volume>0)
    {
    if(!OrdersUpdateActivate(request.order,request.result_volume,request.result_price,request.order_id))
      {
      //-- remove
      RemoveRequest(request.request_id);
      //---
      message.Format("'%d' order update activation failed #%d",request.login,request.order);
      LOG_ERROR(pluginLog,message.Str());
      //---
      return(false);
      }
    //---
#pragma region MyRegion
    /*str.Format("'%s' Open MT4 Position %d, FXI Execution ID %s, FXI Order ID %s, price %f, CumQty %0.2lf",
               request.text,
               request.order,
               request.exec_id,
               request.order_id,
               request.result_price,
               request.fix_qty);
    LOG_ORDER(orderLog,str.Str());*/
#pragma endregion
    //---
    RemoveRequest(request.request_id);
    //--- open partial if necessarry
    if(request.volume>request.result_volume)
      if(!(OrdersAddOpenPending(&record,request.volume-request.result_volume,request.order_id)>0))
        {
        message.Format("'%d' order add open pending reamining part failed #%d",login,request.order);
        LOG_ERROR(pluginLog,message.Str());
        //---
        return(false);
        }
    }
  else
    {
    SMTFormat::FormatPrice(price,record.open_price,record.digits);
    SMTFormat::FormatTradeCommand(side,record.cmd);
    ExtLogger.Out(CmdTrade,"'%d': reject '%d' pending activation of %s <order #%d> %0.2lf %s at %s <execId:%s, orderId:%s>",
                  ExtManagerInfo.login,
                  record.login,
                  side.Str(),
                  record.order,
                  record.volume/100.0,
                  record.symbol,
                  price.Str(),
                  request.exec_id,
                  request.order_id);
    //--- remove from list
    RemoveRequest(request.request_id);
    }
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| Process stop loss transactions                                   |
//+------------------------------------------------------------------+
bool CTradeDispatcher::ProcessSLActivation(const TradeRequest &request,const Instrument &instrument,StringType &message)
  {
  StringType256 str;
  StringType64  price;
  StringType64  side;
  //--- checks
  if(!m_api_integral)
    return(false);
  //--- check
  if(request.result_volume<=0||request.result_price<=0)
    {
    SMTFormat::FormatPrice(price,request.price_order,instrument.digits);
    SMTFormat::FormatTradeCommand(side,request.cmd);
    ExtLogger.Out(CmdTrade,"'%d': reject '%d' sl activation of %s <order #%d> %0.2lf %s at %s <execId:%s, orderId:%s>",
                  ExtManagerInfo.login,
                  request.login,
                  side.Str(),
                  request.order,
                  request.volume/100.0,
                  request.symbol,
                  price.Str(),
                  request.exec_id,
                  request.order_id);
    //--- remove from list
    RemoveRequest(request.request_id);
    //---
    return(true);
    }
  //--- close order
  if(!OrdersClose(request.order,request.result_volume,request.result_price,request.comment,request.order_id))
    {
    message.Format("'%d' order close failed #%d [sl]",request.login,request.order);
    LOG_ERROR(pluginLog,message.Str());
    //--- send 35=H
    if(request.status_request<5)
      if(m_api_integral->SendOrderStatusRequest(request.order_str))
        return(true);
    //---
    message.Format("'%d' order status request send failed #%d <%s>",request.login,request.order,request.order_str);
    LOG_ERROR(pluginLog,message.Str());
    return(false);
    //---
    }
#pragma region MyRegion
  //--- prepare message
  //str.Format("'%s' Close MT4 Position %d, FXI Execution ID %s, FXI Order ID %s, price %f, CumQty %0.2lf",
  //           request.text,
  //           request.order,
  //           request.exec_id,
  //           request.order_id,
  //           request.result_price,
  //           request.fix_qty);
  //LOG_ORDER(orderLog,str.Str());
#pragma endregion
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| Process take profit transactions                                 |
//+------------------------------------------------------------------+
bool CTradeDispatcher::ProcessTPActivation(const TradeRequest &request,const Instrument &instrument,StringType &message)
  {
  StringType256 str;
  StringType64  price;
  StringType64  side;
  //--- checks
  if(!m_api_integral)
    return(false);
  //--- check
  if(request.result_volume<=0||request.result_price<=0)
    {
    SMTFormat::FormatPrice(price,request.price_order,instrument.digits);
    SMTFormat::FormatTradeCommand(side,request.cmd);
    ExtLogger.Out(CmdTrade,"'%d': reject '%d' tp activation of %s <order #%d> %0.2lf %s at %s <execId:%s, orderId:%s>",
                  ExtManagerInfo.login,
                  request.login,
                  side.Str(),
                  request.order,
                  request.volume/100.0,
                  request.symbol,
                  price.Str(),
                  request.exec_id,
                  request.order_id);
    //--- remove from list
    RemoveRequest(request.request_id);
    //---
    return(true);
    }
  //--- close order
  if(!OrdersClose(request.order,request.result_volume,request.result_price,request.comment,request.order_id))
    {
    //--- send 35=H
    if(request.status_request<5)
      if(m_api_integral->SendOrderStatusRequest(request.order_str))
        return(true);
    //---
    message.Format("'%d' order close failed #%d [tp]",request.login,request.order);
    LOG_ERROR(pluginLog,message.Str());
    return(false);
    }
#pragma region MyRegion
  //--- prepare message
  /*str.Format("'%s' Close MT4 Position %d, FXI Execution ID %s, FXI Order ID %s, price %f, CumQty %0.2lf",
             request.text,
             request.order,
             request.exec_id,
             request.order_id,
             request.result_price,
             request.fix_qty);
  LOG_ORDER(orderLog,str.Str());*/
#pragma endregion
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| Process stop out order                                           |
//+------------------------------------------------------------------+
bool CTradeDispatcher::ProcessPositionStopOut(const TradeRequest &request,const Instrument &instrument,StringType &message)
  {
  StringType256 str;
  StringType64  price;
  StringType64  side;
  //--- checks
  if(!m_api_integral)
    return(false);
  //--- check
  if(request.result_volume<=0||request.result_price<=0)
    {
    SMTFormat::FormatPrice(price,request.price_order,instrument.digits);
    SMTFormat::FormatTradeCommand(side,request.cmd);
    ExtLogger.Out(CmdTrade,"'%d': reject '%d' StopOut activation of %s <order #%d> %0.2lf %s at %s <execId:%s, orderId:%s>",
                  ExtManagerInfo.login,
                  request.login,
                  side.Str(),
                  request.order,
                  request.volume/100.0,
                  request.symbol,
                  price.Str(),
                  request.exec_id,
                  request.order_id);
    //--- remove from list
    RemoveRequest(request.request_id);
    //---
    return(true);
    }
  //--- close order
  if(!OrdersClose(request.order,request.result_volume,request.result_price,request.comment,request.order_id))
    {
    //--- send 35=H
    if(request.status_request<5)
      if(m_api_integral->SendOrderStatusRequest(request.order_str))
        return(true);
    //---
    message.Format("'%d' order close failed #%d [stopout]",request.login,request.order);
    LOG_ERROR(pluginLog,message.Str());
    return(false);
    }
#pragma region MyRegion

  //--- prepare message
  /*str.Format("'%s' Close MT4 Position %d, FXI Execution ID %s, FXI Order ID %s, price %f, CumQty %0.2lf",
             request.text,
             request.order,
             request.exec_id,
             request.order_id,
             request.result_price,
             request.fix_qty);
  LOG_ORDER(orderLog,str.Str());*/
#pragma endregion
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| Send an executiom to MT                                          |
//+------------------------------------------------------------------+
bool CTradeDispatcher::SendDealExecution(const FIX43::ExecutionReport &exchange_deal,const Instrument &instrument)
  {
  FIX::ClOrdID          clord_id;
  FIX::CumQty           cum_qty;
  FIX::AvgPx            avg_px;
  FIX::OrdStatus        ord_status;
  FIX::ExecID           exec_id;
  FIX::OrderID          order_id;
  FIX::OnBehalfOfCompID on_behalf;
  FIX::ExecType         exec_type;
  double                volume=0;
  ConGroup              group={0};
  TradeRequest          request={0};
  StringType256         message;
  bool                  res=false;
  //--- checks
  if(!m_api_integral)
    return(false);
  //--- read required fields
  if(!exchange_deal.getIfSet(clord_id)||
     !exchange_deal.getIfSet(cum_qty)||
     !exchange_deal.getIfSet(avg_px)||
     !exchange_deal.getIfSet(ord_status)||
     !exchange_deal.getIfSet(exec_id)||
     !exchange_deal.getIfSet(order_id))
    {
#pragma region process

    message.Format("message parse error <%s>",exchange_deal.toString().c_str());
    LOG_ERROR(pluginLog,message.Str());
    //---
    FIX43::NewOrderSingle nos=FIX43::NewOrderSingle(exchange_deal);
    FIX::SessionID session_id=exchange_deal.getSessionID();
    nos.setSessionID(FIX::SessionID(session_id.getBeginString(),session_id.getTargetCompID(),session_id.getSenderCompID()));
    //---
    if(!SendDontKnowTrade(nos.toString().c_str(),TradeRequest::DONT_KNOW_ORDER,message.Str()))
      {
      //--- TODO: what can we do?
      LOG_ERROR(pluginLog,"Send DontKnowTrade failed <%s>",message.Str());
      }
    //---
    return(false);

#pragma endregion process
    }
  //--- find request
  if(!RequestGet(clord_id,cum_qty.getValue(),avg_px.getValue(),clord_id,exec_id,ord_status,order_id,exchange_deal.toString().c_str(),instrument,request))
    {
#pragma region process

    //--- could not find
    if(exchange_deal.getIfSet(exec_type))
      {
      switch(exec_type.getValue())
        {
        case FIX::ExecType_ORDER_STATUS:
          return(true);
        default:
          break;
        }
      }
    //--- still partial
    if(ord_status.getValue()==FIX::OrdStatus_PARTIALLY_FILLED)
      return(true);
    //---
    message.Format("unknown replay <%s>",clord_id.getString().c_str());
    LOG_ERROR(pluginLog,message.Str());
    //prepare
    FIX43::NewOrderSingle nos=FIX43::NewOrderSingle(exchange_deal);
    FIX::SessionID session_id=exchange_deal.getSessionID();
    nos.setSessionID(FIX::SessionID(session_id.getBeginString(),session_id.getTargetCompID(),session_id.getSenderCompID()));
    //--- replace
    if(nos.getHeader().getIfSet(on_behalf))
      {
      nos.getHeader().setField(FIX::DeliverToCompID(on_behalf.getValue()));
      nos.getHeader().removeField(on_behalf.getField());
      }
    //---
    int login;
    if(StringType::ReadParamInt(clord_id.getString().c_str(),"login:",&login))
      {
      StringType64 login_str;
      login_str.Format("%d",login);
      nos.setField(FIX::AccountId(login_str.toString()));
      }
    //--- send
    if(!SendDontKnowTrade(nos.toString().c_str(),IntegralExecution::DONT_KNOW_ORDER,message.Str()))
      {
      LOG_ERROR(pluginLog,"Send DontKnowTrade failed <%s>",message.Str());
      }
    return(false);

#pragma endregion process
    }

#pragma region old
  //--- fill response
  //StringType::Copy(ptr->record.exec_str,exchange_deal.toString().c_str());
  //volume=SMTMath::PriceNormalize(cum_qty.getValue()/instrument.lot_conversion,7);
  //ptr->record.result_price=avg_px.getValue()*instrument.lot_conversion;
  ////--- convert intergal size to mt volume
  //ptr->record.result_volume=SMTMath::VolumeFromSize(volume,instrument.contract_size);
  //if(cum_qty.getValue()>0)
  //  {
  //  dFilledLots=ptr->record.result_volume;
  //  filledFracPart=modf(dFilledLots,&filledIntPart);
  //  shouldIgnore=(abs((ptr->record.result_volume-dFilledLots)<(1.0/SMTMath::DecPow(7))))?true:false;
  //  if(filledFracPart!=0&&!shouldIgnore)
  //    {
  //    LOG_RISK(pluginLog,"Rounding error for order %s (%s)!! Grid volume (%0.3f) wasn't a multiple of 0.01 lots (contract size %0.3f) round(%g) filled(%g) residual(%g)!",
  //             clord_id.getValue().c_str(),
  //             exec_id.getValue().c_str(),
  //             cum_qty.getValue(),
  //             instrument.contract_size,
  //             filledIntPart,
  //             dFilledLots,
  //             filledFracPart);
  //    //---
  //    ptr->record.result_volume=SMTMath::PriceToIntPos(filledIntPart,0);
  //    }
  //  }
  ////--- check order status
  //switch(ord_status)
  //  {
  //  case FIX::OrdStatus_FILLED:
  //    ptr->record.order_state=TradeRequest::ORDER_STATE_FILLED;
  //    break;
  //  case FIX::OrdStatus_EXPIRED:
  //    ptr->record.order_state=TradeRequest::ORDER_STATE_EXPIRED;
  //    break;
  //  case FIX::OrdStatus_CANCELED:
  //    ptr->record.order_state=TradeRequest::ORDER_STATE_CANCELLED;
  //    break;
  //  case FIX::OrdStatus_REJECTED:
  //    ptr->record.order_state=TradeRequest::ORDER_STATE_REJECTED;
  //    break;
  //  case FIX::OrdStatus_PARTIALLY_FILLED:
  //    ptr->record.order_state=TradeRequest::ORDER_STATE_PARTIAL;
  //    //--- unlock
  //    m_sync.Unlock();
  //    return(true);
  //  default:
  //    message.Format("unknown order status <%s>",ord_status.getString().c_str());
  //    LOG_ERROR(pluginLog,message.Str());
  //    if(!SendDontKnowTrade(ptr->record.order_str,TradeRequest::DONT_KNOW_ORDER,message.Str()))
  //      {
  //      //--- TODO: what can we do?
  //      LOG_ERROR(pluginLog,"Send DontKnowTrade failed <%s>",message.Str());
  //      }
  //    //--- unlock
  //    m_sync.Unlock();
  //    return(false);
  //  }
  ////--- result received
  //ptr->record.time_done=ExtServer->TradeTime();
  ////--- set received volume
  //ptr->record.fix_qty=cum_qty.getValue()/instrument.lot_conversion;
  ////--- set execution message as assigned by integral
  //StringType::Copy(ptr->record.order_id,order_id.getString().c_str());
  //StringType::Copy(ptr->record.exec_id,exec_id.getString().c_str());
  ////--- add or update response
  //IntegralTradeRecord::Fill(&ptr->record,response);
  //RecordUpdate(response);
  ////---
  //request=ptr->record;
  ////--- unlock
  //m_sync.Unlock();

#pragma endregion old

  switch(ord_status.getValue())
    {
    case FIX::OrdStatus_PARTIALLY_FILLED:
      return(true);
    default:
      break;
    }
  //--- fill response
  //IntegralTradeRecord::Fill(&request,response);
  //if(!RecordUpdate(response))
  //  {
  //  //---- send 35=H
  //  if(!m_api_integral->SendOrderStatusRequest(request.order_str))
  //    {
  //    return(true);
  //    }
  //  //---
  //  return(false);
  //  }
  //--- process
  if(request.complete_event!=NULL)
    SetEvent(request.complete_event);
  else
    {
    //--- process reply
    if(!(res=SendOrderExecution(request,instrument,message)))
      {
      if(!SendDontKnowTrade(request.order_str,TradeRequest::DONT_KNOW_ORDER,message.Str()))
        {
        LOG_ERROR(pluginLog,"DontKnowTrade send failed <%s>",message.Str());
        }
      }
    //---
    return(res);
    }
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| Get position pointer by unique id                                |
//+------------------------------------------------------------------+
TradeRequest *CTradeDispatcher::RequestGetPtr(LPCSTR unique_id)
  {
  //--- go through all request and find one for specified unique_id
  for(UINT i=0;i<m_requests.Total();i++)
    if(StringType::Compare(m_requests[i].unique_id,unique_id)==0)
      return(&m_requests[i]);
  //--- return result
  return(NULL);
  }
//+------------------------------------------------------------------+
//| Get position pointer for request id                              |
//+------------------------------------------------------------------+
TradeRequest *CTradeDispatcher::RequestGetPtr(const INT request_id)
  {
  //--- go through all request and find one for specified unique_id
  for(UINT i=0;i<m_requests.Total();i++)
    if(m_requests[i].request_id==request_id)
      return(&m_requests[i]);
  //--- return result
  return(NULL);
  }
//+------------------------------------------------------------------+
//| Get position pointer by order                                    |
//+------------------------------------------------------------------+
TradeRequest *CTradeDispatcher::RequestGetPtrByOrder(const INT order)
  {
  //--- go through all request and find one for specified unique_id
  for(UINT i=0;i<m_requests.Total();i++)
    if(m_requests[i].order==order)
      return(&m_requests[i]);
  //--- return result
  return(NULL);
  }
//+------------------------------------------------------------------+
//| Get position for login                                           |
//+------------------------------------------------------------------+
bool CTradeDispatcher::RequestGet(const FIX::ClOrdID &unique_id,double cum_qty,double avg_px,const FIX::ClOrdID &clord_id,const FIX::ExecID &exec_id,const FIX::OrdStatus &ord_status,const FIX::OrderID &order_id,LPCSTR exchange_deal,const Instrument &instrument,TradeRequest &request)
  {
  double         filledIntPart,dFilledLots,filledFracPart;
  TradeRequest *ptr=NULL;
  bool           shouldIgnore,res=false;
  StringType256  message;
  //--- lock
  m_sync.Lock();
  //--- get position pointer
  ptr=RequestGetPtr(unique_id.getString().c_str());
  //--- copy position
  if(ptr)
    {
    //--- fill response
    StringType::Copy(ptr->exec_str,exchange_deal);
    double volume=SMTMath::PriceNormalize(cum_qty/instrument.lot_conversion,7);
    ptr->result_price=avg_px*instrument.lot_conversion;
    //--- convert intergal size to mt volume
    ptr->result_volume=SMTMath::VolumeFromSize(volume,instrument.contract_size);
    if(cum_qty>0)
      {
      dFilledLots=ptr->result_volume;
      filledFracPart=modf(dFilledLots,&filledIntPart);
      shouldIgnore=(abs((ptr->result_volume-dFilledLots)<(1.0/SMTMath::DecPow(7))))?true:false;
      if(filledFracPart!=0&&!shouldIgnore)
        {
        LOG_RISK(pluginLog,"Rounding error for order %s (%s)!! Grid volume (%0.3f) wasn't a multiple of 0.01 lots (contract size %0.3f) round(%g) filled(%g) residual(%g)!",
                 clord_id.getString(),
                 exec_id.getString(),
                 cum_qty,
                 instrument.contract_size,
                 filledIntPart,
                 dFilledLots,
                 filledFracPart);
        //---
        ptr->result_volume=SMTMath::PriceToIntPos(filledIntPart,0);
        }
      }
    //--- check order status
    switch(ord_status)
      {
      case FIX::OrdStatus_FILLED:
        ptr->order_state=TradeRequest::ORDER_STATE_FILLED;
        ptr->status_request++;
        break;
      case FIX::OrdStatus_EXPIRED:
        ptr->order_state=TradeRequest::ORDER_STATE_EXPIRED;
        ptr->status_request++;
        break;
      case FIX::OrdStatus_CANCELED:
        ptr->order_state=TradeRequest::ORDER_STATE_CANCELLED;
        ptr->status_request++;
        break;
      case FIX::OrdStatus_REJECTED:
        ptr->order_state=TradeRequest::ORDER_STATE_REJECTED;
        ptr->status_request++;
        break;
      case FIX::OrdStatus_PARTIALLY_FILLED:
        ptr->order_state=TradeRequest::ORDER_STATE_PARTIAL;
        break;
      default:
        message.Format("unknown order status <%s>",ord_status.getString().c_str());
        LOG_ERROR(pluginLog,message.Str());
        if(!SendDontKnowTrade(ptr->order_str,TradeRequest::DONT_KNOW_ORDER,message.Str()))
          {
          //--- TODO: what can we do?
          LOG_ERROR(pluginLog,"Send DontKnowTrade failed <%s>",message.Str());
          }
        //--- unlock
        m_sync.Unlock();
        return(false);
      }
    //--- result received
    ptr->time_done=ExtServer->TradeTime();
    //--- set received volume
    ptr->fix_qty=cum_qty/instrument.lot_conversion;
    //--- set execution message as assigned by integral
    StringType::Copy(ptr->order_id,order_id.getString().c_str());
    StringType::Copy(ptr->exec_id,exec_id.getString().c_str());
    //---
    ptr->timestamp=_time64(NULL);
    //---
    request=*ptr;
    }
  //--- unlock
  m_sync.Unlock();
  //--- return result
  return(ptr!=NULL);
  }
//+------------------------------------------------------------------+
//| Get current time of exchange                                     |
//+------------------------------------------------------------------+
INT64 CTradeDispatcher::GetExchangeTime()
  {
  //--- use UTC for exchange time
  return(_time64(NULL));
  }
//+------------------------------------------------------------------+
//| Log request                                                      |
//+------------------------------------------------------------------+
bool CTradeDispatcher::LogRequest(const TradeRequest *request) const
  {
  //--- show
  LOG_DEBUG(pluginLog,"(%d) request for user %d, manager %d, order %d, request %s %s %.2lf %s at %.5lf, IE Dev %d, time: %d, comment: <%s>",
            request->request_id,
            request->login,
            request->manager,
            request->order,
            SIntegralFormat::FormatTransactionName(request->type),
            SIntegralFormat::FormatCmdName(request->cmd),
            request->volume/100.0,
            request->symbol,
            request->price_order,
            request->ie_deviation,
            request->timestamp,
            request->comment);
  //--- successful
  return(true);
  }
//+------------------------------------------------------------------+
//| Log response                                                     |
//+------------------------------------------------------------------+
bool CTradeDispatcher::LogAnswerGateway(const RequestInfo *request) const
  {
  StringTypePath txt;
  //--- check
  if(!request)
    return(false);
  //--- format message
  txt.Append("LogAnswerGateway");
  /*if(request->id)
     txt.Format("%s (#%I64u %s)",confirm->Print(str),request->id,request->Print(tmp));
  else
     txt.Format("%s (%s)",confirm->Print(str),request->Print(tmp));*/
     //--- log formatted message
  LOG_INFO(pluginLog,"'%d': request #%d answered - %s",request->login,request->id,txt.Str());
  //--- successful
  return(true);
  }
//+------------------------------------------------------------------+
//| Prepare UserInfo for login                                       |
//+------------------------------------------------------------------+
int CTradeDispatcher::UserInfoGet(const int login,UserInfo *info)
  {
  UserRecord user={0};
  //---- cheks
  if(login<1||!info||!ExtServer)                     return(FALSE);
  //---- clear info
  ZeroMemory(info,sizeof(UserInfo));
  //---- get user record
  if(ExtServer->ClientsUserInfo(login,&user)==FALSE)  return(FALSE);
  //---- fill login
  info->login=user.login;
  //---- fill permissions
  info->enable=user.enable;
  info->enable_read_only=user.enable_read_only;
  //---- fill trade data
  info->leverage=user.leverage;
  info->agent_account=user.agent_account;
  info->credit=user.credit;
  info->balance=user.balance;
  info->prevbalance=user.prevbalance;
  //---- fill group
  StringType::Copy(info->group,user.group);
  if(ExtServer->GroupsGet(user.group,&info->grp)==FALSE) return(FALSE);
  //---- ok
  return(TRUE);
  }
//+------------------------------------------------------------------+
//| preapre comment                                                  |
//+------------------------------------------------------------------+
bool CTradeDispatcher::AddComment(LPCSTR current_comment,LPCSTR apPrefix,LPCSTR apExecID,StringType &comment)
  {
  StringType32 trade_comment;
  //--- checks
  if(!apPrefix||!apExecID||!current_comment)
    return(false);
  //--- check
  if(StringType::Find(current_comment,"so:")>=0||
     StringType::Find(current_comment,"deleted")>=0)
    {
    comment.Assign(current_comment);
    return(false);
    }
  //--- clear
  comment.Clear();
  //--- remember
  trade_comment.Assign(current_comment);
  //---
  /*if((index=StringType::Find(trade_comment.Str(),"to #"))>=0)
    {
    trade_comment.Delete(index,trade_comment.Len());
    }*/
    //--- prepare
  switch(ExtPluginContext.CommentStyle())
    {
    case CPluginContext::COMMENT_STYLE::COMMENT_BEFORE:
      comment.Format("%s%s%s%s",
                     apPrefix,
                     apExecID,
                     trade_comment.Empty()?"":" ",
                     trade_comment.Str());
      break;
    case CPluginContext::COMMENT_STYLE::COMMENT_AFTER:
      comment.Format("%s%s%s%s",
                     trade_comment.Str(),
                     trade_comment.Empty()?"":" ",
                     apPrefix,
                     apExecID);
      break;
    default:
      comment.Assign(trade_comment);
      return(false);
      break;
    }
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| Closing a BUY or SELL position using OrdersClose                 |
//+------------------------------------------------------------------+
bool CTradeDispatcher::OrdersClose(const int order,const int volume,const double close_price,LPCSTR comment,LPCSTR order_id)
  {
  UserInfo       info={0};
  TradeTransInfo trans={0};
  TradeRecord    trade={0};
  UserInfo       user={0};
  int            login;
  StringType32   comment_str;
  //--- Checks
  if(order<=0||volume<=0||close_price<=0||ExtServer==NULL)
    return(false);
  //--- Getting login from the order
  if((login=ExtServer->TradesFindLogin(order))<1)
    {
    LOG_ERROR(pluginLog,"trades find login failed #%d",order);
    return(false); // Error
    }
  //--- Getting client data
  if(UserInfoGet(login,&info)==FALSE)
    {
    LOG_ERROR(pluginLog,"'%d' user info get failed #%d",login,order);
    return(false); // Error
    }
  //---
  if(ExtServer->OrdersGet(order,&trade)==FALSE)
    {
    LOG_ERROR(pluginLog,"'%d' orders get failed #%d",login,order);
    return(false); // Error
    }
  //--- check
  if(trade.close_time>0)
    {
    LOG_ERROR(pluginLog,"'%d' order already closed #%d",login,order);
    return(false);
    }
  //--- Preparing transaction
  trans.order=order;
  trans.volume=volume;
  trans.price=close_price;
  //--- Check of tick size
  //--- Instrument check
  //--- Volume check
  //--- Freeze level check
  //--- prepare comment
  AddComment(comment,"C:",order_id,comment_str);
  StringType::Copy(trans.comment,comment_str.Str());
  //--- Closing position
  if(ExtServer->OrdersClose(&trans,&info)==FALSE)
    {
    LOG_ERROR(pluginLog,"'%d' orders close failed for #%d",login,order);
    return(false); // Error
    }
  //--- Position closed
  return(true);
  }
//+------------------------------------------------------------------+
//| Opening a BUY or SELL position using OrdersAdd                   |
//+------------------------------------------------------------------+
int CTradeDispatcher::OrdersAddOpen(const int login,const int cmd,LPCTSTR symbol,
                                    const double open_price,const int volume,
                                    LPCSTR order_id,const double stop_loss,
                                    const double take_profit)
  {
  UserInfo       info={0};
  ConGroup       grpcfg={0};
  ConSymbol      symcfg={0};
  TradeTransInfo trans={0};
  TradeRecord    trade={0};
  double         curprices[2]={0};
  int            order=0;
  StringType32   comment_str;
  //--- Checks
  if(login<=0||cmd<OP_BUY||cmd>OP_SELL||symbol==NULL||open_price<=0||volume<=0||ExtServer==NULL)
    return(0);
  //--- Receiving client data
  if(UserInfoGet(login,&info)==FALSE)
    return(0); // Error
//--- Receiving group configuration
  if(ExtServer->GroupsGet(info.group,&grpcfg)==FALSE)
    return(0); // Error
//--- Receiving symbol configuration
  if(ExtServer->SymbolsGet(symbol,&symcfg)==FALSE)
    return(FALSE); // Error
//--- Adding SL, TP, comment
//--- Check of long-only permission
//--- Check of close-only permission
//--- Check of tick size
//--- Instrument check
//--- Volume check
//--- Stop level check
//--- Margin check
//--- Receiving current prices
  if(ExtServer->HistoryPricesGroup(symbol,&grpcfg,curprices)!=RET_OK)
    return(0); // Error
//--- Preparing order
  trade.login=login;
  trade.cmd=cmd;
  trade.open_price=open_price;
  trade.sl=stop_loss;
  trade.tp=take_profit;
  trade.volume=volume;
  trade.close_price=(cmd==OP_BUY?curprices[0]:curprices[1]);
  trade.open_time=ExtServer->TradeTime();
  trade.digits=symcfg.digits;
  StringType::Copy(trade.symbol,symbol);
  //--- Adding SL, TP, comment
  AddComment(trade.comment,"O:",order_id,comment_str);
  StringType::Copy(trade.comment,comment_str.Str());
  //--- Adding an order
  if((order=ExtServer->OrdersAdd(&trade,&info,&symcfg))==0)
    return(0); // Error
//--- Position is open: return order
  return(order);
  }
//+------------------------------------------------------------------+
//| Opening a BUY or SELL position using OrdersAdd                   |
//+------------------------------------------------------------------+
int CTradeDispatcher::OrdersAddOpen(const TradeRecord *current_trade,UserInfo *user,const int volume)
  {
  ConGroup       grpcfg={0};
  ConSymbol      symcfg={0};
  TradeTransInfo trans={0};
  TradeRecord    trade={0};
  double         curprices[2]={0};
  int            order=0;
  //--- Checks
  if(current_trade->cmd<OP_BUY||current_trade->cmd>OP_SELL||current_trade==NULL||volume<=0||ExtServer==NULL)
    return(0);
  //--- Receiving group configuration
  if(ExtServer->GroupsGet(user->group,&grpcfg)==FALSE)
    return(0); // Error
//--- Receiving symbol configuration
  if(ExtServer->SymbolsGet(current_trade->symbol,&symcfg)==FALSE)
    return(FALSE); // Error
//--- Adding SL, TP, comment
//--- Check of long-only permission
//--- Check of close-only permission
//--- Check of tick size
//--- Instrument check
//--- Volume check
//--- Stop level check
//--- Margin check
//--- Receiving current prices
  if(ExtServer->HistoryPricesGroup(current_trade->symbol,&grpcfg,curprices)!=RET_OK)
    return(0); // Error
//--- Preparing order
  trade.login=current_trade->login;
  trade.cmd=current_trade->cmd;
  trade.open_price=current_trade->open_price;
  trade.volume=volume;
  trade.close_price=(current_trade->cmd==OP_BUY?curprices[0]:curprices[1]);
  trade.open_time=current_trade->open_time;
  trade.expiration=current_trade->expiration;
  trade.tp=current_trade->tp;
  trade.sl=current_trade->sl;
  trade.digits=symcfg.digits;
  StringType::Copy(trade.symbol,current_trade->symbol);
  //--- Adding SL, TP, comment
  //--- Adding an order
  if((order=ExtServer->OrdersAdd(&trade,user,&symcfg))==0)
    return(0); // Error
//--- Position is open: return order
  return(order);
  }
//+------------------------------------------------------------------+
//| Placing a pending order using OrdersAdd                          |
//+------------------------------------------------------------------+
int CTradeDispatcher::OrdersAddOpenPending(const int login,const int cmd,LPCTSTR symbol,const double open_price,const int volume,const time_t expiration)
  {
  UserInfo       info={0};
  ConGroup       grpcfg={0};
  ConSymbol      symcfg={0};
  TradeTransInfo trans={0};
  TradeRecord    trade={0};
  double         curprices[2]={0}; // bid/ask
  int            order=0;
  //--- Checks
  if(login<=0||cmd<OP_BUY_LIMIT||cmd>OP_SELL_STOP||
     symbol==NULL||open_price<=0||volume<=0||ExtServer==NULL) return(0);
  //--- Receiving client data
  if(UserInfoGet(login,&info)==FALSE)
    return(0); // Error
//--- Receiving group configuration
  if(ExtServer->GroupsGet(info.group,&grpcfg)==FALSE)
    return(0); // Error
//--- Receiving symbol configuration
  if(ExtServer->SymbolsGet(symbol,&symcfg)==FALSE)
    return(0); // Error
//--- Check of long-only permission
//--- Check of close-only permission
//--- Check of tick size
//--- Symbol check
//--- Volume check
//--- Check of expiration permission
//--- Check of stop levels
//--- Receiving current prices
  if(ExtServer->HistoryPricesGroup(symbol,&grpcfg,curprices)!=RET_OK)
    return(0); // Error
//--- Preparing a new trading state of an order
  trade.login=login;
  trade.cmd=cmd;
  trade.open_price=open_price;
  trade.volume=volume;
  trade.close_price=cmd==(OP_BUY_LIMIT||cmd==OP_BUY_STOP?curprices[0]:curprices[1]);
  trade.open_time=ExtServer->TradeTime();
  trade.digits=symcfg.digits;
  StringType::Copy(trade.symbol,symbol);
  //--- Adding SL, TP, comment
  //--- Placing the order
  if((order=ExtServer->OrdersAdd(&trade,&info,&symcfg))==0)
    return(0); // Error
//--- Order placed: return the order
  return(order);
  }
//+------------------------------------------------------------------+
//| Open pending order using OrdersAdd                               |
//+------------------------------------------------------------------+
int CTradeDispatcher::OrdersAddOpenPending(const TradeRecord *trade,const int volume,LPCSTR order_id)
  {
  UserInfo       info={0};
  ConGroup       grpcfg={0};
  ConSymbol      symcfg={0};
  TradeTransInfo trans={0};
  TradeRecord    pending={0};
  double         curprices[2]={0}; // bid/ask
  int            order=0;
  StringType32   comment_str;
  //--- Checks
  if(!trade||volume<=0||ExtServer==NULL) return(0);
  //--- Receiving client data
  if(UserInfoGet(trade->login,&info)==FALSE)
    return(0); // Error
//--- Receiving group configuration
  if(ExtServer->GroupsGet(info.group,&grpcfg)==FALSE)
    return(0); // Error
//--- Receiving symbol configuration
  if(ExtServer->SymbolsGet(trade->symbol,&symcfg)==FALSE)
    return(0); // Error
//--- Check of long-only permission
//--- Check of close-only permission
//--- Check of tick size
//--- Symbol check
//--- Volume check
//--- Check of expiration permission
//--- Check of stop levels
//--- Receiving current prices
  ExtServer->HistoryPricesGroup(trade->symbol,&grpcfg,curprices);
  //--- Preparing a new trading state of an order
  pending.login=trade->login;
  pending.cmd=trade->cmd;
  pending.open_price=trade->open_price;
  pending.volume=volume;
  pending.close_price=(trade->cmd==OP_BUY_LIMIT||trade->cmd==OP_BUY_STOP?curprices[0]:curprices[1]);
  pending.open_time=trade->open_time;
  pending.digits=symcfg.digits;
  StringType::Copy(pending.symbol,trade->symbol);
  //--- Adding SL, TP, comment
  pending.tp=trade->tp;
  pending.sl=trade->sl;
  //--- prepare comment
  //AddComment(pending.comment,"O:",order_id,comment_str);
  //StringType::Copy(pending.comment,comment_str.Str());
  //--- Placing the order
  if((order=ExtServer->OrdersAdd(&pending,&info,&symcfg))==0)
    return(0); // Error
//--- Order placed: return the order
  return(order);
  }
//+------------------------------------------------------------------+
//| Pending order activation using OrdersUpdate                      |
//+------------------------------------------------------------------+
bool CTradeDispatcher::OrdersUpdateActivate(const int order,const int volume,const double open_price,LPCSTR order_id)
  {
  TradeRecord    oldtrade={0},trade={0};
  ConSymbol      symcfg={0};
  ConGroup       grpcfg={0};
  UserInfo       info={0};
  double         curprices[2]={0},profit=0,margin=0,freemargin=0,prevmargin=0; // bid/ask
  TradeTransInfo trans={0};
  StringType32   comment;
  StringType64   time;
  //--- Checks
  if(order<=0||open_price<=0||ExtServer==NULL)
    return(false);
  //--- Getting order
  if(ExtServer->OrdersGet(order,&oldtrade)==FALSE)
    return(false); // Error
//--- Status check
  if(oldtrade.cmd<OP_BUY_LIMIT||oldtrade.cmd>OP_SELL_STOP)
    return(false);  // Order is activated
  //---
  if(oldtrade.close_time>0)
    {
    SMTFormat::FormatDateTimeMsc(time,oldtrade.close_time,true,true);
    LOG_CRITICAL(pluginLog,"'%d' order deleted #%d <close time: %s>",
                 oldtrade.login,
                 oldtrade.order,
                 time.Str());
    //---
    return(false);
    }
  //--- Receiving client data
  if(UserInfoGet(oldtrade.login,&info)==FALSE)
    return(false); // Error
//--- get group config
  if(ExtServer->GroupsGet(info.group,&grpcfg)==FALSE)
    {
    LOG_ERROR(pluginLog,"GroupsGet failed [%s]",info.group);
    return(false); // error
    }
  //--- Receiving symbol configuration
  if(ExtServer->SymbolsGet(oldtrade.symbol,&symcfg)==FALSE)
    return(false); // Error
//--- prepare transaction for checks
  trans.cmd=(oldtrade.cmd==OP_BUY_LIMIT||oldtrade.cmd==OP_BUY_STOP)?OP_BUY:OP_SELL;;
  trans.volume=volume;
  trans.price=open_price;
  StringType::Copy(trans.symbol,oldtrade.symbol);
  //--- Check of tick size
  //--- Check of long-only permission
  //--- Margin check
  margin=ExtServer->TradesMarginCheck(&info,&trans,&profit,&freemargin,&prevmargin);
  if((freemargin+grpcfg.credit)<0&&(symcfg.margin_hedged_strong!=FALSE||prevmargin<=margin))
    {
    if(OrdersUpdateCancel(oldtrade.order)==FALSE)
      {
      LOG_CRITICAL(pluginLog,"'%d' order delete failed #%d",oldtrade.login,order);
      }
    //---
    LOG_INFO(pluginLog,"'%d' not enough margin:%0.2lf freemargin:%0.2lf",
             oldtrade.order,
             margin,
             freemargin);
    return(false); // no enough margin
    }
  //---
  ExtServer->HistoryPricesGroup(oldtrade.symbol,&grpcfg,curprices);
  //--- Preparing new order state
  memcpy(&trade,&oldtrade,sizeof(TradeRecord));
  trade.cmd=(oldtrade.cmd==OP_BUY_LIMIT||oldtrade.cmd==OP_BUY_STOP)?OP_BUY:OP_SELL;
  trade.open_time=ExtServer->TradeTime();
  trade.open_price=open_price;
  trade.profit=0;
  trade.storage=0;
  trade.expiration=0;
  trade.volume=volume;
  trade.taxes=0;
  trade.close_price=(oldtrade.cmd==OP_BUY_LIMIT||oldtrade.cmd==OP_BUY_STOP?curprices[0]:curprices[1]);
  //--- Commission calculation
  ExtServer->TradesCommission(&trade,info.group,&symcfg);
  //--- Profit calculation
  ExtServer->TradesCalcProfit(info.group,&trade);
  //--- Calculation of conversion rate and margin
  trade.conv_rates[0]=ExtServer->TradesCalcConvertation(info.group,FALSE,trade.open_price,&symcfg);
  trade.margin_rate  =ExtServer->TradesCalcConvertation(info.group,TRUE,trade.open_price,&symcfg);
  //--- prepare comment
  AddComment(trade.comment,"O:",order_id,comment);
  StringType::Copy(trade.comment,comment.Str());
  //--- Order activation
  if(ExtServer->OrdersUpdate(&trade,&info,UPDATE_ACTIVATE)==FALSE)
    return(false); // Error
//--- Pending order active
  return(true);
  }
//+------------------------------------------------------------------+
//| Closing a position using OrdersUpdate               |
//+------------------------------------------------------------------+
bool CTradeDispatcher::OrdersUpdateClose(const int order,const int volume,const double close_price,LPCSTR order_id)
  {
  UserInfo     info={0};
  ConSymbol    symcfg={0};
  TradeRecord  oldtrade={0},trade={0};
  StringType32 comment;
  //--- Checks
  if(order<=0||ExtServer==NULL) return(false);
  //--- Getting order
  if(ExtServer->OrdersGet(order,&oldtrade)==FALSE)
    return(false); // Error
//--- Status check
  if(oldtrade.cmd<OP_BUY||oldtrade.cmd>OP_SELL)
    return(false); // 
//--- Checking close time
  if(oldtrade.close_time!=0)
    return(false); // Position is closed
//--- Receiving client data
  if(UserInfoGet(oldtrade.login,&info)==FALSE)
    return(false); // Error
//--- Receiving symbol configuration
  if(ExtServer->SymbolsGet(oldtrade.symbol,&symcfg)==FALSE)
    return(false); // Error
//--- Check of tick size
//--- Instrument check
//--- volume check
//--- Check of freeze level
//--- Preparing new order state
  memcpy(&trade,&oldtrade,sizeof(TradeRecord));
  trade.close_time=ExtServer->TradeTime();
  trade.close_price=close_price;
  trade.volume=volume;
  //--- Adding comment
  AddComment(trade.comment,"C:",order_id,comment);
  StringType::Copy(trade.comment,comment.Str());
  //--- Profit calculation
  ExtServer->TradesCalcProfit(info.group,&trade);
  //--- Calculation of conversion rates
  trade.conv_rates[1]=ExtServer->TradesCalcConvertation(info.group,FALSE,trade.close_price,&symcfg);
  //--- Agent commission calculation
  ExtServer->TradesCommissionAgent(&trade,&symcfg,&info);
  ExtServer->TradesCommission(&trade,info.group,&symcfg);
  //--- Position closure
  if(ExtServer->OrdersUpdate(&trade,&info,UPDATE_CLOSE)==FALSE)
    return(false); // Error
//--- Position closed
  return(true);
  }
//+------------------------------------------------------------------+
//| Canceling pending order using OrdersUpdate                       |
//+------------------------------------------------------------------+
int CTradeDispatcher::OrdersUpdateCancel(const int order)
  {
  TradeRecord oldtrade={0},trade={0};
  UserInfo    info={0};
  //--- Checks
  if(order<=0||ExtServer==NULL) return(FALSE);
  //--- Getting order
  if(ExtServer->OrdersGet(order,&oldtrade)==FALSE)
    return(FALSE); // Error
//--- Check
  if(oldtrade.cmd<OP_BUY_LIMIT||oldtrade.cmd>OP_SELL_STOP)
    return(FALSE); // Order is activated
//--- Check
  if(oldtrade.close_time!=0)
    return(FALSE); // Order is deleted
//--- Getting client data
  if(UserInfoGet(oldtrade.login,&info)==FALSE)
    return(FALSE); // Error
//--- Preparing new order state
  memcpy(&trade,&oldtrade,sizeof(TradeRecord));
  trade.close_time=ExtServer->TradeTime();
  trade.profit=0;
  trade.storage=0;
  trade.expiration=0;
  trade.taxes=0;
  //--- Adding comment
  StringType::Copy(trade.comment,"deleted [no money]");
  //--- Deleting order
  if(ExtServer->OrdersUpdate(&trade,&info,UPDATE_CLOSE)==FALSE)
    return(FALSE); // Error
//--- Pending order has been deleted
  return(TRUE);
  }
//+------------------------------------------------------------------+
//| check closeby                                                    |
//+------------------------------------------------------------------+
bool CTradeDispatcher::CheckCloseBy(const int order,const int orderby)
  {
  return(true);
  }
//+------------------------------------------------------------------+
//| check multiple closeby                                           |
//+------------------------------------------------------------------+
bool CTradeDispatcher::CheckMultipleCloseBy(const int login,LPCSTR symbol)
  {
  return(true);
  }
//+------------------------------------------------------------------+
//| Check request timeouts                                           |
//+------------------------------------------------------------------+
void CTradeDispatcher::Scheduler(const time_t curtime)
  {
  StringType64 time_str;
  //--- checks
  if(!ExtServer||!m_api_integral)
    return;
  //---
  }
//+------------------------------------------------------------------+
//| Add a new order to a queue for processing                        |
//+------------------------------------------------------------------+
//bool CTradeDispatcher::OrderAddOrUpdate(const IntegralOrder &order)
//  {
//  bool res=true;
//  //--- create an order waiting for handling
//  IntegralPendingOrder pending_order={0},*ptr;
//  //--- fill in order adding order id
//  pending_order.order_mt=order.order;
//  //--- lock
//  m_sync.Lock();
//  //--- add a new order to the queue
//  if((ptr=m_orders.Search(&pending_order.order_mt,SearchOrderByTicket))==NULL)
//    {
//    if((ptr=m_orders.Insert(&pending_order,SortOrdersByTicket))==NULL)
//      {
//      m_sync.Unlock();
//      return(false);
//      }
//    }
//  ptr->order=order;
//  //--- unlock
//  m_sync.Unlock();
//  //--- return result
//  return(res);
//  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CTradeDispatcher::Check(const time_t curtime)
  {
  StringType64  time_str;
  StringType64  curtime_str;
  TradeRequest *ptr;
  INT64         m_curtime;
  //--- checks
  if(!m_api_integral||!ExtServer)
    return;
  //--- under lock
  m_sync.Lock();
  //---
  m_curtime=_time64(NULL);

  for(UINT i=0;i<m_requests.Total();i++)
    {
    ptr=&m_requests[i];
    if(ptr->timestamp+REQUEST_WAIT>m_curtime)
      continue;
    //---
    switch(ptr->order_state)
      {
      case TradeRequest::ORDER_STATE_PLACED:
      case TradeRequest::ORDER_STATE_PARTIAL:
        break;
      default:
        continue;
      }
    //--- show
    SMTFormat::FormatTimeMsc(time_str,ptr->time_setup,true);
    SMTFormat::FormatTimeMsc(curtime_str,curtime,true);
    LOG_CRITICAL(pluginLog,"the request/order (%d/%d) is not replied since %I64d seconds {%s / %s} <%d>",
                 ptr->request_id,
                 ptr->order,
                 curtime-(ptr->time_setup+REQUEST_WAIT),
                 curtime_str.Str(),
                 time_str.Str(),
                 ptr->order_state);
    //--- send OrderStatusRequest
    ptr->status_request++;
    ptr->timestamp=curtime;
    m_api_integral->SendOrderStatusRequest(ptr->order_str);
    }
  //---
  m_sync.Unlock();
  //---
  //UpdateTime((INT64)CHECK_TIMEOUT);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
//bool CTradeDispatcher::OrderExists(const int ticket,IntegralOrder &order)
//  {
//  bool res=true;
//  IntegralPendingOrder *ptr=NULL;
//  //---
//  m_sync.Lock();
//  //---
//  if((ptr=m_orders.Search(&ticket,SearchOrderByTicket))==NULL)
//    {
//    m_sync.Unlock();
//    return(false);
//    }
//  //--- set order
//  order=ptr->order;
//  //---
//  m_sync.Unlock();
//  //---
//  return(res);
//  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
//bool CTradeDispatcher::RequestExists(const int id,const int order,TradeResponse &response)
//  {
//  TradeRequest *ptr;
//  //--- checks
//  if(id==0&&order==0)
//    return(false);
//  //---
//  ZeroMemory(&response,sizeof(TradeResponse));
//  //--- under lock
//  m_sync.Lock();
//  //---
//  for(INT pos=0;pos<m_requests_total;pos++)
//    {
//    ptr=&m_requests[pos];
//    if(ptr->request_id==id||ptr->order==order)
//      {
//      //--- fill response
//      StringType::Copy(response.unique_id,ptr->unique_id);
//      StringType::Copy(response.exec_id,ptr->exec_id);
//      StringType::Copy(response.order_id,ptr->order_id);
//      StringType::Copy(response.symbol,ptr->symbol);
//      StringType::Copy(response.short_name,ptr->short_name);
//      StringType::Copy(response.exec_str,ptr->exec_str);
//      //--- 
//      response.request_id=ptr->request_id;
//      response.type=ptr->order_action;
//      response.status=ptr->order_state;
//      response.price=ptr->result_price;
//      response.volume=ptr->result_volume;
//      response.fix_qty=ptr->fix_qty;
//      //---
//      m_sync.Unlock();
//      return(true);
//      }
//    }
//  //---
//  m_sync.Unlock();
//  //---
//  return(false);
//  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
//bool CTradeDispatcher::RequestExists(const int order,TradeResponse &response)
//  {
//  TradeRequest *ptr;
//  //--- checks
//  if(order<1)
//    return(false);
//  //--
//  ZeroMemory(&response,sizeof(TradeResponse));
//  //--- under lock
//  m_sync.Lock();
//  //---
//  for(INT pos=0;pos<m_requests_total;pos++)
//    {
//    ptr=&m_requests[pos];
//    //--- fill response
//    StringType::Copy(response.unique_id,ptr->unique_id);
//    StringType::Copy(response.exec_id,ptr->exec_id);
//    StringType::Copy(response.order_id,ptr->order_id);
//    StringType::Copy(response.symbol,ptr->symbol);
//    StringType::Copy(response.exec_str,ptr->exec_str);
//    //---
//    response.request_id=ptr->request_id;
//    response.type=ptr->order_action;
//    response.status=ptr->order_state;
//    response.price=ptr->result_price;
//    response.volume=ptr->result_volume;
//    response.fix_qty=ptr->fix_qty;
//    //--- successfull
//    m_sync.Unlock();
//    return(true);
//    }
//  //---
//  m_sync.Unlock();
//  return(false);
//  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
//bool CTradeDispatcher::RequestExists(const int order)
//  {
//  TradeRequest *ptr;
//  INT64         curtime=_time64(NULL);
//  StringType64  time_str;
//  StringType64  curtime_str;
//  //--- checks
//  if(order<1||!m_api_integral)
//    return(false);
//  //--- under lock
//  m_sync.Lock();
//  //---
//  for(INT pos=0;pos<m_requests_total;pos++)
//    {
//    ptr=&m_requests[pos];
//    if(ptr->order!=order)  continue;
//    //---
//    if(curtime>=(ptr->time_setup+REQUEST_WAIT)||ptr->status_request<5)
//      {
//      SMTFormat::FormatTimeMsc(time_str,ptr->time_setup,true);
//      SMTFormat::FormatTimeMsc(curtime_str,curtime,true);
//      LOG_CRITICAL(pluginLog,"the request/order (%d/%d) is not replied since %I64d seconds {%s / %s} <%d>",
//                   ptr->request_id,
//                   ptr->order,
//                   curtime-(ptr->time_setup+REQUEST_WAIT),
//                   curtime_str.Str(),
//                   time_str.Str(),
//                   ptr->order_state);
//      //--- send OrderStatusRequest
//      ptr->status_request++;
//      m_api_integral->SendOrderStatusRequest(ptr->order_str);
//      }
//    m_sync.Unlock();
//    return(true);
//    }
//  //---
//  m_sync.Unlock();
//  return(false);
//  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
//bool CTradeDispatcher::RequestExists(const int login,const int order)
//  {
//  TradeRequest *ptr;
//  INT64         curtime=_time64(NULL);
//  StringType64  time_str;
//  StringType64  curtime_str;
//  //--- checks
//  if(order<1||!m_api_integral)
//    return(false);
//  //--- under lock
//  m_sync.Lock();
//  //---
//  for(INT pos=0;pos<m_requests_total;pos++)
//    {
//    ptr=&m_requests[pos];
//    if(ptr->order==order||
//       (ptr->login==login&&ptr->order_action==TradeRequest::IDC_PENDING&&ptr->order!=0))
//      {
//      //---
//      if(curtime>=(ptr->time_setup+REQUEST_WAIT))
//        {
//        SMTFormat::FormatTimeMsc(time_str,ptr->time_setup,true);
//        SMTFormat::FormatTimeMsc(curtime_str,curtime,true);
//        LOG_CRITICAL(pluginLog,"the request/order (%d/%d) is not replied since %I64d seconds {%s / %s} <%d>",
//                     ptr->request_id,
//                     ptr->order,
//                     curtime-(ptr->time_setup+REQUEST_WAIT),
//                     curtime_str.Str(),
//                     time_str.Str(),
//                     ptr->order_state);
//        //--- send OrderStatusRequest
//        ptr->status_request++;
//        m_api_integral->SendOrderStatusRequest(ptr->order_str);
//        }
//      //---
//      m_sync.Unlock();
//      return(true);
//      }
//    }
//  //---
//  m_sync.Unlock();
//  return(false);
//  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
//bool CTradeDispatcher::RequestStatusUpdate(const int id,const int order,const UINT status)
//  {
//  TradeRequest *ptr;
//  //--- under lock
//  m_sync.Lock();
//  //---
//  for(INT pos=0;pos<m_requests_total;pos++)
//    {
//    ptr=&m_requests[pos];
//    if(ptr->request_id==id||ptr->order==order)
//      {
//      //--- update status
//      ptr->order_state=status;
//      //---
//      m_sync.Unlock();
//      return(true);
//      }
//    }
//  //---
//  m_sync.Unlock();
//  //---
//  return(false);
//  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
//bool CTradeDispatcher::RemoveIfExists(const int id,const int order,TradeResponse &response)
//  {
//  TradeRequest *ptr=NULL;
//  //---
//  m_sync.Lock();
//  //---
//  ZeroMemory(&response,sizeof(TradeResponse));
//  for(INT pos=0;pos<m_requests_total;pos++)
//    {
//    ptr=&m_requests[pos];
//    if(!((ptr->request_id==id&&id>0)||ptr->order==order)) continue;
//    //--- fill response                                               
//    StringType::Copy(response.symbol,ptr->symbol);
//    StringType::Copy(response.group,ptr->group);
//    StringType::Copy(response.exec_id,ptr->exec_id);
//    StringType::Copy(response.unique_id,ptr->unique_id);
//    StringType::Copy(response.order_id,ptr->order_id);
//    StringType::Copy(response.text,ptr->text);
//    StringType::Copy(response.exec_str,ptr->exec_str);
//    //--- set info   
//    response.request_id=ptr->request_id;
//    response.type=ptr->order_action;
//    response.status=ptr->order_state;
//    response.fix_qty=ptr->fix_qty;
//    response.volume=ptr->result_volume;
//    response.price=ptr->result_price;
//    response.order_mt=ptr->order;
//    //--- clear request
//    ptr->Clear();
//    //--- unlock
//    m_sync.Unlock();
//    return(true);
//    }
//  //---
//  m_sync.Unlock();
//  //---
//  return(false);
//  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
//bool CTradeDispatcher::RemoveByOrder(const int order)
//  {
//  TradeRequest *ptr;
//  bool res=false;
//  //--- checks
//  if(order<1)
//    return(false);
//  //--- under lock
//  m_sync.Lock();
//  //---
//  for(INT pos=0;pos<m_requests_total;pos++)
//    {
//    ptr=&m_requests[pos];
//    if(ptr->order==order)
//      {
//      LOG_DEBUG(pluginLog,"RemoveByOrder(%d)",order);
//      ptr->Clear();
//      m_sync.Unlock();
//      return(true);
//      }
//    }
//  //--- show
//  LOG_DEBUG(pluginLog,"order #%d remove from request storage failed",order);
//  //---
//  m_sync.Unlock();
//  return(false);
//  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
//bool CTradeDispatcher::RemoveById(const int id)
//  {
//  TradeRequest *ptr;
//  //--- under lock
//  m_sync.Lock();
//  //---
//  for(INT pos=0;pos<m_requests_total;pos++)
//    {
//    ptr=&m_requests[pos];
//    if(ptr->request_id==id)
//      {
//      LOG_DEBUG(pluginLog,"RemoveById(%d)",id);
//      ptr->Clear();
//      m_sync.Unlock();
//      return(true);
//      }
//    }
//  //---
//  m_sync.Unlock();
//  //--- show
//  LOG_DEBUG(pluginLog,"request #%d remove from request storage failed",id);
//  return(false);
//  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
//bool CTradeDispatcher::RemoveByUniqueId(LPCSTR unique_id)
//  {
//  TradeRequest *ptr;
//  //--- checks
//  if(!unique_id)
//    return(false);
//  //--- under lock
//  m_sync.Lock();
//  //---
//  for(INT pos=0;pos<m_requests_total;pos++)
//    {
//    ptr=&m_requests[pos];
//    if(StringType::Compare(ptr->unique_id,unique_id)==0)
//      {
//      LOG_DEBUG(pluginLog,"RemoveByUniqueId(%s)",unique_id);
//      ptr->Clear();
//      m_sync.Unlock();
//      return(true);
//      }
//    }
//  //---
//  m_sync.Unlock();
//  //--- show
//  LOG_DEBUG(pluginLog,"request <%s> remove from request storage failed",unique_id);
//  return(false);
//  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
//bool CTradeDispatcher::RemoveOrder(const int ticket)
//  {
//  bool res=true;
//  IntegralPendingOrder *ptr=NULL;
//  //---
//  m_sync.Lock();
//  //---
//  if((ptr=m_orders.Search(&ticket,SearchOrderByTicket))==NULL)
//    {
//    m_sync.Unlock();
//    return(false);
//    }
//  //--- remove order
//  res=m_orders.Remove(ptr,SortOrdersByTicket);
//  //---
//  m_sync.Unlock();
//  //---
//  return(res);
//  }
//+------------------------------------------------------------------+
//| Log response                                                     |
//+------------------------------------------------------------------+
//bool CTradeDispatcher::LogAnswerExchange(const ExchangeOrder &exchange_order,const IMTConfirm *confirm) const
//  {
//   MTAPISTR str={0};
////--- check
//   if(!confirm)
//      return(false);
////--- print formatted message
//   ExtLogger.Out(MTLogOK,L"'%I64u': request #%I64u answered - %s",exchange_order.login,exchange_order.request_id,confirm->Print(str));
////--- successful
//   return(true);
//  }
//+------------------------------------------------------------------+
//| Execution logging                                                |
//+------------------------------------------------------------------+
//bool CTradeDispatcher::LogExecution(MTAPIRES retcode,const IMTExecution *execution) const
//  {
//   MTAPISTR str={0};
////--- check
//   if(!execution)
//      return(false);
////--- exit if no trading actions are performed
//   if(retcode==MT_RET_OK_NONE)
//      return(true);
////--- check result
//   if(retcode==MT_RET_OK)
//      ExtLogger.Out(MTLogOK,L"'%I64u': execution send complete - %s",execution->Login(),execution->Print(str));
//   else
//      ExtLogger.Out(MTLogErr,L"'%I64u': execution send failed - %s(%d)",execution->Login(),execution->Print(str),retcode);
////--- successful
//   return(true);
//  }
//+------------------------------------------------------------------+
//| Sorting                                                          |
//+------------------------------------------------------------------+
//int CTradeDispatcher::SortOrdersByTicket(const void *left,const void *right)
//  {
//  const IntegralPendingOrder &lft=*(const IntegralPendingOrder *)left;
//  const IntegralPendingOrder &rgh=*(const IntegralPendingOrder *)right;
//  //--- compare
//  if(lft.order_mt>rgh.order_mt)
//    return(1);
//  if(lft.order_mt<rgh.order_mt)
//    return(-1);
//  //--- equal
//  return(0);
//  }
//+------------------------------------------------------------------+
//| Searching                                                        |
//+------------------------------------------------------------------+
//int CTradeDispatcher::SearchOrderByTicket(const void *left,const void *right)
//  {
//  const INT &lft=*(const INT *)left;
//  const IntegralPendingOrder &rgh=*(const IntegralPendingOrder *)right;
//  //--- compare
//  if(lft>rgh.order_mt)
//    return(1);
//  if(lft<rgh.order_mt)
//    return(-1);
//  //--- equal
//  return(0);
//  }
//+------------------------------------------------------------------+
//| Searching                                                        |
//+------------------------------------------------------------------+
//int CTradeDispatcher::SearchOrderByUniqueId(const void *left,const void *right)
//  {
//  LPCSTR                      lft=(LPCSTR)left;
//  const IntegralPendingOrder &rgh=*(const IntegralPendingOrder *)right;
//  //--- compare
//  return(StringType::Compare(lft,rgh.order.unique_id));
//  }
//+------------------------------------------------------------------+
//| Sorting                                                          |
//+------------------------------------------------------------------+
//int CTradeDispatcher::SortTradeRequestsByState(const void *left,const void *right)
//  {
//  TradeRequest *lft=(TradeRequest *)left;
//  TradeRequest *rgh=(TradeRequest *)right;
//  //--- compare
//  if(lft->order_state>rgh->order_state) return(1);
//  if(lft->order_state<rgh->order_state) return(-1);
//  //--- equal
//  return(0);
//  }
//+------------------------------------------------------------------+
//| Searching                                                        |
//+------------------------------------------------------------------+
//int CTradeDispatcher::SearchTradeRequestByState(const void *left,const void *right)
//  {
//  UINT                lft=*(UINT *)left;
//  TradeRequest *rgh=(TradeRequest *)right;
//  //--- compare
//  if(lft>rgh->order_state) return(1);
//  if(lft<rgh->order_state) return(-1);
//  //--- equal
//  return(0);
//  }
//+------------------------------------------------------------------+
//| Searching                                                        |
//+------------------------------------------------------------------+
//int CTradeDispatcher::SearchTradeRequestById(const void *left,const void *right)
//  {
//  const INT &lft=*(const INT *)left;
//  const TradeRequest &rgh=*(const TradeRequest *)right;
//  //--- compare
//  if(lft>rgh.request_id)
//    return(1);
//  if(lft<rgh.request_id)
//    return(-1);
//  //--- equal
//  return(0);
//  }
//+------------------------------------------------------------------+
//| Searching                                                        |
//+------------------------------------------------------------------+
//int CTradeDispatcher::SearchTradeRequestByUniqueId(const void *left,const void *right)
//  {
//  LPCSTR              lft=(LPCSTR)left;
//  TradeRequest *rgh=(TradeRequest *)right;
//  //--- compare
//  return(StringType::Compare(lft,rgh->unique_id));
//  }
//+------------------------------------------------------------------+
//| Searching                                                        |
//+------------------------------------------------------------------+
//int CTradeDispatcher::SearchTradeRequestByOrder(const void *left,const void *right)
//  {
//  const INT &lft=*(const INT *)left;
//  const TradeRequest *rgh=(TradeRequest *)right;
//  //--- compare
//  if(lft>rgh->order)
//    return(1);
//  if(lft<rgh->order)
//    return(-1);
//  //--- equal
//  return(0);
//  }
//+------------------------------------------------------------------+
//| Searching                                                        |
//+------------------------------------------------------------------+
//int CTradeDispatcher::SearchTradeRequestByLogin(const void *left,const void *right)
//  {
//  const INT &lft=*(const INT *)left;
//  const TradeRequest *rgh=(TradeRequest *)right;
//  //--- compare
//  if(lft>rgh->login)
//    return(1);
//  if(lft<rgh->login)
//    return(-1);
//  //--- equal
//  return(0);
//  }
//+------------------------------------------------------------------+
//| Searching                                                        |
//+------------------------------------------------------------------+
//int CTradeDispatcher::SearchTradeRequestByLoginAndSymbol(const void *left,const void *right)
//  {
//  const TradeRequest *lft=(TradeRequest *)left;
//  const TradeRequest *rgh=(TradeRequest *)right;
//  //--- compare
//  if(lft->login>rgh->login)
//    return(1);
//  if(lft->login<rgh->login)
//    return(-1);
//  //--- equal
//  return(StringType::Compare(lft->symbol,rgh->symbol));
//  }
//+------------------------------------------------------------------+
//| Sort orders in the array                                         |
//+------------------------------------------------------------------+
//int CTradeDispatcher::SortTradeResponses(const void *left,const void *right)
//  {
//  const TradeResponse &lft=*(const TradeResponse *)left;
//  const TradeResponse &rgh=*(const TradeResponse *)right;
//  //---
//  if(lft.request_id>rgh.request_id) return(1);
//  if(lft.request_id<rgh.request_id) return(-1);
//  //---
//  return(0);
//  }
//+------------------------------------------------------------------+
//| Search order in the array                                        |
//+------------------------------------------------------------------+
//int CTradeDispatcher::SearchTradeResponsesByOrder(const void *left,const void *right)
//  {
//  const INT &lft=*(const INT *)left;
//  const TradeResponse &rgh=*(TradeResponse *)right;
//  //---
//  if(lft>rgh.order_mt) return(1);
//  if(lft<rgh.order_mt) return(-1);
//  return(0);
//  }
//+------------------------------------------------------------------+
//| Search order in the array                                        |
//+------------------------------------------------------------------+
//int CTradeDispatcher::SearchTradeResponsesById(const void *left,const void *right)
//  {
//  const INT &lft=*(const INT *)left;
//  const TradeResponse &rgh=*(TradeResponse *)right;
//  //---
//  if(lft>rgh.request_id) return(1);
//  if(lft<rgh.request_id) return(-1);
//  return(0);
//  }
//+------------------------------------------------------------------+
//| Allocate request                                                 |
//+------------------------------------------------------------------+
//RequestRecord *CTradeDispatcher::RequestAllocate(void)
//  {
//  RequestRecord *record=new(std::nothrow) RequestRecord;
//  return(record);
//  }
//+------------------------------------------------------------------+
//| Free request                                                     |
//+------------------------------------------------------------------+
//void CTradeDispatcher::RequestFree(RequestRecord *record)
//  {
//  if(record)
//    delete record;
//  }
//+------------------------------------------------------------------+
//| Sort requests                                                    |
//+------------------------------------------------------------------+
int CTradeDispatcher::SortRequests(const void *left,const void *right)
  {
  TradeRequest *lft=(TradeRequest *)left;
  TradeRequest *rgh=(TradeRequest *)right;
  //--- by id
  if(lft->request_id>rgh->request_id) return(1);
  if(lft->request_id<rgh->request_id) return(-1);
  //--- by unique_id
  return(0);
  }
//+------------------------------------------------------------------+
//| Search requests                                                  |
//+------------------------------------------------------------------+
int CTradeDispatcher::SearchRequests(const void *left,const void *right)
  {
  INT           lft=*(INT *)left;
  TradeRequest *rgh=(TradeRequest *)right;
  //---
  if(lft>rgh->request_id) return(1);
  if(lft<rgh->request_id) return(-1);
  //---
  return(0);
  }
//+------------------------------------------------------------------+
