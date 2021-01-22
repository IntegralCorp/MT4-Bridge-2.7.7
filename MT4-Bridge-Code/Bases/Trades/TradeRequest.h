//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include "..\..\API\MT4APITools.h"
#include "..\..\include\MT4ServerAPI.h"
//--- fix
#include "..\..\QuickFix\fix43\OrderStatusRequest.h"
#include "..\..\QuickFix\fix43\DontKnowTrade.h"
#include "..\..\IO\Log.h"
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
struct RequestInfoRecord
  {
  UINT        action;
  UINT        type;
  UINT        state;
  INT         request_id;
  INT         ie_deviation;           // deviation on Instant Execution
  char        comment[32];
  //---
  char        unique_id[256];         // unique identifier for Order as assigned by the buy-side (broker).
  char        order_str[2048];
  char        exec_str[2048];
  double      fix_qty;
  char        text[32];
  //--- TradeRequest::EnOrderReason
  UINT        reason;
  //--- deal result         
  char        exec_id[256];           // Unique identifier assigned by sell-side (integral).
  char        order_id[256];          // Unique identifier assigned by sell-side (integral).
  int         remaining_volume;       // remaining volume
  //---
  RequestInfo request;
  };
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct TradeRequest
  {
  enum EnDKReasonCode
    {
    MT4_ORDER_TIMED_OUT=0,
    MT4_RESTARTED=1,
    DONT_KNOW_ORDER=2
    };
  //--- origin
  enum EnOrigin
    {
    ORIGIN_REQUEST=0,
    ORIGIN_ORDER=1
    };
  //--- enumerate operations on the order
  enum EnOrderActions
    {
    IDC_OPEN=0,
    IDC_CLOSE=1,
    IDC_PENDING=2,
    IDC_TP=3,
    IDC_SL=4,
    IDC_STOPOUT=5
    };
  //--- enumerate order states
  enum EnOrderStates
    {
    ORDER_STATE_UNKNOWN  =0,     // order started
    ORDER_STATE_SENDING  =1,     // order started
    ORDER_STATE_PLACED   =2,     // order send to Intergal
    ORDER_STATE_FILLED   =3,     // order filled
    ORDER_STATE_PARTIAL  =4,     // order filled
    ORDER_STATE_EXPIRED  =5,     // order filled
    ORDER_STATE_CANCELLED=6,     // order filled
    ORDER_STATE_REJECTED =7,     // order rejected
    ORDER_STATE_TIMEOUT  =8,     // order timeout
    ORDER_STATE_NOT_FOUND=9      // order not found
    };
  //--- order creation reasons
  enum EnOrderReason
    {
    ORDER_REASON_CLIENT    =0,     // order placed manually
    ORDER_REASON_DEALER    =1,     // order placed by dealer
    ORDER_REASON_ACTIVATION=2, // order placed due SL
    };
  UINT64         id;                    // internal use
   // //--- order
  char           order_str[2048];
  //--- session ino
  FIX::SessionID session_id;			// fix session id				   
  //--- mt4 info				
  INT            order;                  // order ticket  
  INT            login;                  // client login  															  
  INT            request_id;             // request id
  char           symbol[12];
  short          cmd;
  UINT           order_state;            // IntegralOrder::EnOrderStates
  UINT           reason;                 // EnOrderReason
  INT            ie_deviation;           // deviation on Instant Execution
  time_t         expiration;             // expiration time (server time in UNIX format) 
//---
  INT64          time_setup;             // after send to integral
  INT64          time_done;              // answer received
//---
  char           short_name[64];         // 
  char           unique_id[256];         // unique identifier for Order as assigned by the buy-side (broker).
  //--- IntegralOrder::EnOrderActions
  UINT           order_action;
  char           flags;                  // flags
  INT            volume;                 // requested volume  
  double         price_order;            // order price
  double         price_sl;               // order sl price
  double         price_tp;               // order tp price
  //---
  //--- group records
  //----  
  char           group[16];
  //--- Group::EnCoverMode
  UINT           cover_mode;
  char           on_behalf[64];         // on behalf of xxx
  //---
  //--- symbol records
  //----
  char           ccy_pair[64];          // currency pair (integral)
  char           base_cry[32];          // base currency         ?
  char           quote_cry[4];          // quote currency        ?
  //---          
  double         min_spread;			    // minimum spread
  double         bid_spread;			    // bid spread
  double         ask_spread;			    // ask spread
  double         max_spread;			    // maximum spread
  double         mrkt_range;			    // market range
  //--- Instrument::EnPriceAlgo                                    
  UINT           price_algo;            // price algo
  double         price_size;            // price size
  bool           bias_bid;              // bid biaz
  double         min_qty;               // min quantity
  char           time_in_force;         // =FIX::TimeInForce_GOOD_TILL_DATE;
  int            expiration_time;       // =30;
  bool           enabled;               // =false;
  double         lot_conversion;        // =1.0;
  //---
  int            digits;                // security precision
  double         spread;                // spread
  int            spread_balance;        // spread balance
  int            exec_mode;             // execution mode
  double         contract_size;         // contract size
  int            symbol_type;           // security group (see ConSymbolGroup)
  char           name[16];              // group name (con group)
//---
  double         prices[2];
  DWORD          request_time;           // request time set by MQ
//---
  time_t         open_time;
  bool           manager_order;
  int            manager;
  int            spread_diff;           // spread difference in compare with default security spread
  char           comment[32];
  //
  char           exec_str[2048];
  time_t         sent_time;
  //---
  UINT           type;
  //--- EnOrigin
  UINT           origin;
  char           text[32];
  //--- deal result         
  char           exec_id[256];           // Unique identifier assigned by sell-side (integral).
  char           order_id[256];          // Unique identifier assigned by sell-side (integral).
  int            result_volume;          // request result confirmed volume
  double         result_price;           // request result confirmed price
  double         fix_qty;
  //---- time
  INT64          timestamp;              // creation time
  //--- status request counter
  int            status_request;
  //---
  HANDLE         complete_event;

  //--- check whether the request is empty
  bool IsEmpty(void)
    {
    //--- checks
    if(id||login||request_id||order||order_state||request_time||
       volume||price_order||result_volume||
       result_price||time_setup||time_done||fix_qty)
      return(false);
    //--- checks
    if(symbol[0]|group[0]||comment[0]||unique_id[0]||/*session_id[0]||*/order_str[0]||exec_str[0]||text[0]||exec_id[0]||order_id[0])
      return(false);
    //--- empty
    return(true);
    }
  //--- read/writes
  bool              Read(CMTFile &file);
  bool              Read(CMTFile &file,const UINT version,bool &invalid_version);
  bool              Write(CMTFile &file) const;
  //--- print order type
  static LPCSTR     PrintRequestAction(const UINT type,StringType &res);
  static LPCSTR     PrintRequestAction(const UINT type,const short cmd,StringType &res);
  //--- fill request from RequestInfo
  static bool       Fill(const RequestInfo *info,const UINT action,const UINT reason,const UINT origin,TradeRequest &request);
  static bool       Fill(const RequestInfo *info,const UINT action,TradeRequest &request);
  //---
  static bool       Fill(const TradeRecord *trade,const UINT action,LPCSTR group,double price,TradeRequest &record);
  static bool       Fill(const StringType &str,FIX43::OrderStatusRequest &osr);
  static bool       Fill(const StringType &str,const UINT reason,LPCSTR text,FIX43::DontKnowTrade &dkt);
  //static bool       Fill(TradeRequest &request,LPCSTR exec_str,double cum_qty,double lot_conversion,double contract_size,LPCSTR clord_id,UINT ord_status);
  //--- generate order ID in an external trading system
  static INT        OrderExchangeIDNext();
  //static void       Clear(TradeRequest &order);
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Print order type                                                 |
//+------------------------------------------------------------------+
inline LPCSTR TradeRequest::PrintRequestAction(const UINT action,StringType &res)
  {
  //--- form the string value of order action type
  switch(action)
    {
    case TradeRequest::IDC_OPEN:    res.Assign("O");   break;
    case TradeRequest::IDC_CLOSE:   res.Assign("C");   break;
    case TradeRequest::IDC_PENDING: res.Assign("A");   break;
    case TradeRequest::IDC_TP:      res.Assign("T");   break;
    case TradeRequest::IDC_SL:      res.Assign("S");   break;
    case TradeRequest::IDC_STOPOUT: res.Assign("M");   break;
    default: res.Assign("U");                          break;
    }
  //--- return result
  return(res.Str());
  }
//+------------------------------------------------------------------+
//| Print order type                                                 |
//+------------------------------------------------------------------+
inline LPCSTR TradeRequest::PrintRequestAction(const UINT action,const short cmd,StringType &res)
  {
  res.Clear();
  //--- form the string value of order action type
  switch(action)
    {
    case TradeRequest::IDC_OPEN:
      if(cmd==OP_BUY)
        {
        res.Assign("Buy");
        break;
        }
      res.Assign("Sell");
      break;
    case TradeRequest::IDC_CLOSE:
      if(cmd==OP_BUY)
        {
        res.Assign("Sell");
        break;
        }
      res.Assign("Buy");
      break;
    case TradeRequest::IDC_PENDING:
      switch(cmd)
        {
        case OP_BUY_LIMIT:
          res.Assign("BuyLimit");
          break;
        case OP_SELL_LIMIT:
          res.Assign("SellLimit");
          break;
        case OP_BUY_STOP:
          res.Assign("BuyStop");
          break;
        case OP_SELL_STOP:
          res.Assign("SellStop");
          break;
        default:
          res.Assign("Activate");
          break;
        }
      break;
    case TradeRequest::IDC_TP:
      res.Assign("TakeProfit");
      break;
    case TradeRequest::IDC_SL:
      res.Assign("StopLoss");
      break;
    case TradeRequest::IDC_STOPOUT:
      res.Assign("StopOut");
      break;
    default:
      res.Assign("");
      break;
    }
  //--- return result
  return(res.Str());
  }
//+------------------------------------------------------------------+
//| fill request from RequestInfo                                    |
//+------------------------------------------------------------------+
inline bool TradeRequest::Fill(const RequestInfo *info,const UINT action,const UINT reason,const UINT origin,TradeRequest &request)
  {
  StringType32   text;
  // checks
  if(!info)
    return(false);
  //--- clear the structure
  ZeroMemory(&request,sizeof(request));
  //--- fill request
  request.request_id  =info->id;
  request.ie_deviation=info->trade.ie_deviation;
  request.order_action=action;
  request.login       =info->login;
  request.order       =info->trade.order;
  request.time_setup  =_time64(NULL);// ExtServer->TradeTime();
  request.volume      =info->trade.volume;
  request.price_order =info->trade.price;
  request.cmd         =info->trade.cmd;
  request.price_sl    =info->trade.sl;
  request.price_tp    =info->trade.tp;
  request.request_time=OURTIME(ExtServer->TradeTime());
  StringType::Copy(request.symbol,info->trade.symbol);
  StringType::Copy(request.group,info->group);
  StringType::Copy(request.comment,info->trade.comment);
  //---
  TradeRequest::PrintRequestAction(action,request.cmd,text);
  StringType::Copy(request.text,text.Str());
  //--- set reason
  request.reason   =reason;
  request.type     =info->trade.type;
  request.origin   =origin;
  request.timestamp=_time64(NULL);
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| fill request from RequestInfo                                    |
//+------------------------------------------------------------------+
inline bool TradeRequest::Fill(const RequestInfo *info,const UINT action,TradeRequest &request)
  {
  StringType32   text;
  // checks
  if(!info)
    return(false);
  //--- clear the structure
  ZeroMemory(&request,sizeof(request));
  //--- fill request
  request.request_id  =info->id;
  request.ie_deviation=info->trade.ie_deviation;
  request.order_action=action;
  request.login       =info->login;
  request.order       =info->trade.order;
  request.time_setup  =_time64(NULL);// ExtServer->TradeTime();
  request.volume      =info->trade.volume;
  request.price_order =info->trade.price;
  request.cmd         =info->trade.cmd;
  request.price_sl    =info->trade.sl;
  request.price_tp    =info->trade.tp;
  request.request_time=OURTIME(ExtServer->TradeTime());
  StringType::Copy(request.symbol,info->trade.symbol);
  StringType::Copy(request.group,info->group);
  StringType::Copy(request.comment,info->trade.comment);
  //---
  TradeRequest::PrintRequestAction(action,request.cmd,text);
  StringType::Copy(request.text,text.Str());
  //--- set reason
  //request.reason   =reason;
  request.type     =info->trade.type;
  //request.origin   =origin;
  request.timestamp=_time64(NULL);
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| fill request from RequestInfo                                    |
//+------------------------------------------------------------------+
inline bool TradeRequest::Fill(const TradeRecord *trade,const UINT action,LPCSTR group,double price,TradeRequest &request)
  {
  StringType32   text;
  //--- checks
  if(!trade||!group)
    return(false);
  //--- clear the structure
  ZeroMemory(&request,sizeof(request));
  //--- fill request
  request.request_id  =-trade->order;
  request.order_action=action;
  request.login       =trade->login;
  request.order       =trade->order;
  request.time_setup  =_time64(NULL);// ExtServer->TradeTime();
  request.cmd         =trade->cmd;
  request.volume      =trade->volume;
  request.price_order =price;
  request.price_sl    =trade->sl;
  request.price_tp    =trade->tp;
  request.request_time=OURTIME(ExtServer->TradeTime());
  StringType::Copy(request.symbol,trade->symbol);
  StringType::Copy(request.group,group);
  StringType::Copy(request.comment,trade->comment);
  //---
  TradeRequest::PrintRequestAction(action,request.cmd,text);
  StringType::Copy(request.text,text.Str());
  //--- set reason
  request.reason   =TradeRequest::ORDER_REASON_CLIENT;
  request.origin   =TradeRequest::ORIGIN_ORDER;
  request.timestamp=_time64(NULL);
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
inline bool TradeRequest::Fill(const StringType &str,FIX43::OrderStatusRequest &osr)
  {
  FIX::SenderSubID      sender_sub_id;
  FIX::DeliverToCompID  deliver_to_comp_id;
  FIX::OnBehalfOfSubID  on_behalf_of_sub_id;
  FIX::OnBehalfOfCompID on_behalf_of_comp_id;
  //--- checks
  if(str.Empty())
    return(false);
  //--- generate new order single from string
  FIX43::NewOrderSingle nos(std::string(str.Str()));
  //--- set order status request
  osr=FIX43::OrderStatusRequest(nos.get(FIX::ClOrdID()),nos.get(FIX::Side()));
  //---
  if(nos.getHeader().isSet(sender_sub_id))
    {
    osr.getHeader().set(nos.getHeader().get(sender_sub_id));
    }
  //---
  if(nos.getHeader().isSet(deliver_to_comp_id))
    {
    osr.getHeader().set(nos.getHeader().get(deliver_to_comp_id));
    }
  //---
  if(nos.getHeader().isSet(on_behalf_of_sub_id))
    {
    osr.getHeader().set(nos.getHeader().get(on_behalf_of_sub_id));
    }
  //---
  if(nos.getHeader().isSet(on_behalf_of_comp_id))
    {
    osr.getHeader().set(nos.getHeader().get(on_behalf_of_comp_id));
    }
  //---
  osr.setField(nos.getField(FIX::Symbol()));
  //--- set session ID
  osr.setSessionID(nos.getSessionID());
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
inline bool TradeRequest::Fill(const StringType &str,const UINT reason,LPCSTR text,FIX43::DontKnowTrade &dkt)
  {
  FIX::Side             side;
  FIX::CumQty           cum_qty;
  FIX::OrderQty         order_qty;
  FIX::ExecID           exec_id;
  FIX::ClOrdID          clord_id;
  FIX::OrderID          order_id;
  FIX::AccountId        account_id;
  FIX::Symbol           symbol;
  FIX::SenderSubID      sender_sub_id;
  FIX::OnBehalfOfCompID on_behalf_of_comp_id;
  FIX::OnBehalfOfSubID  on_behalf_of_sub_id;
  FIX::DeliverToCompID  deliver_to_comp_id;
  double                volume=0;
  //--- checks
  if(!text||str.Empty())
    return(false);
  //--- generate new order single from string
  FIX43::NewOrderSingle nos(std::string(str.Str()));
  //--- get fields
  nos.getFieldIfSet(side);
  nos.getFieldIfSet(cum_qty);
  nos.getFieldIfSet(exec_id);
  nos.getFieldIfSet(clord_id);
  nos.getFieldIfSet(order_id);
  nos.getFieldIfSet(account_id);
  nos.getFieldIfSet(symbol);
  //---
  switch(reason)
    {
    case MT4_ORDER_TIMED_OUT:
      LOG_RISK(pluginLog,"DK due to MT4 timeout for pending NewOrderSingle at Grid '%s' / %s",
               clord_id.getValue().c_str(),
               nos.toString().c_str());
      volume=order_qty.getValue();
      break;
    case MT4_RESTARTED:
      LOG_RISK(pluginLog,"DK due to MT4 restart for pending NewOrderSingle at Grid '%s' / %s",
               clord_id.getValue().c_str(),
               nos.toString().c_str());
      volume=order_qty.getValue();
      break;
    case DONT_KNOW_ORDER:
      LOG_RISK(pluginLog,"Unknown ExecutionReport for Order '%s' / '%s' / '%s' - %s",
               clord_id.getValue().c_str(),
               order_id.getValue().c_str(),
               exec_id.getValue().c_str(),
               nos.toString().c_str());
      //---
      if(nos.getFieldIfSet(cum_qty))
        volume=cum_qty.getValue();
      break;
    default:
      return(false);
    }
  //---
  dkt=FIX43::DontKnowTrade(order_id,exec_id,FIX::DKReason(FIX::DKReason_OTHER),side);
  dkt.setSessionID(nos.getSessionID());
  //---
  if(nos.getHeader().isSet(sender_sub_id))
    {
    nos.getHeader().getFieldIfSet(sender_sub_id);
    dkt.getHeader().setField(sender_sub_id);
    }
  //---
  if(nos.getHeader().isSet(on_behalf_of_comp_id))
    {
    nos.getHeader().getFieldIfSet(on_behalf_of_comp_id);
    dkt.getHeader().setField(on_behalf_of_comp_id);
    }
  //---
  if(nos.getHeader().isSet(on_behalf_of_sub_id))
    {
    nos.getHeader().getFieldIfSet(on_behalf_of_sub_id);
    dkt.getHeader().setField(on_behalf_of_sub_id);
    }
  //---
  if(nos.getHeader().isSet(deliver_to_comp_id))
    {
    nos.getHeader().getFieldIfSet(deliver_to_comp_id);
    dkt.getHeader().setField(deliver_to_comp_id);
    }
  //---
  dkt.setField(FIX::OrderQty(volume));
  dkt.setField(symbol);
  dkt.setField(clord_id);
  dkt.setField(account_id);
  dkt.setField(FIX::DKReasonCode(reason));
  dkt.setField(FIX::Text(text));
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| Generate order ID in an external trading system                  |
//+------------------------------------------------------------------+
inline INT TradeRequest::OrderExchangeIDNext()
  {
  //--- order counter
  static INT s_order_exchange_count(0);
  //--- order counter change synchronization object
  static CMTSync s_sync;
  //--- lock
  s_sync.Lock();
  //--- increment deal counter value
  s_order_exchange_count++;
  //--- unlock
  s_sync.Unlock();
  //--- return order counter value
  return(s_order_exchange_count);
  }
//+------------------------------------------------------------------+
//| Read POD                                                         |
//+------------------------------------------------------------------+
inline bool TradeRequest::Read(CMTFile &file)
  {
  return(file.Read(this,sizeof(*this))==sizeof(*this));
  }
//+------------------------------------------------------------------+
//| Write POD                                                        |
//+------------------------------------------------------------------+
inline bool TradeRequest::Write(CMTFile &file) const
  {
  return(file.Write(this,sizeof(*this))==sizeof(*this));
  }
//+------------------------------------------------------------------+
//| Read POD                                                         |
//+------------------------------------------------------------------+
inline bool TradeRequest::Read(CMTFile &file,const UINT version,bool &invalid_version)
  {
  invalid_version=true;
  return(false);
  }
//+------------------------------------------------------------------+
//| Array of trade requests                                          |
//+------------------------------------------------------------------+
typedef TMTArray<TradeRequest,256> TradeRequestArray;
//+------------------------------------------------------------------+