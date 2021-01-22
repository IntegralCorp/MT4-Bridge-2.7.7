//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include "..\..\API\MT4APITools.h"
#include "..\..\include\MT4ServerAPI.h"
#include "..\Bases\Instrument.h"
#include "..\Bases\\MarketRange.h"
#include "..\Bases\Group.h"
#include "..\..\Bases\Trades\TradeRequest.h"
//+------------------------------------------------------------------+
//| Order structure                                                  |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct IntegralOrder
  {
  //--- order
  char              order_str[2048];
  //--- session ino
  FIX::SessionID    session_id;
  //--- mt4 info
  int               order;                     // order ticket
  int               login;                     // client login
  int               request_id;                // request_id
  char              symbol[12];                // security (mt4)
  short             cmd;                       // order type (trade command)
  UINT              order_state;               // EnOrderStates
  UINT              reason;                    // EnOrderReason
  int               ie_deviation;              // deviation on Instant Execution
  time_t            expiration;                // expiration time (server time in UNIX format)
  //---             
  INT64             time_setup;                // order setup time
  INT64             time_done;                 // order filling/cancel time
  //--
  char              short_name[64];            //  ShortName
  char              unique_id[256];            // unique identifier assigned by the buy-side (broker).     
  //--- IntegralOrder::EnOrderActions
  UINT              request_action;            // 
  char              flags;                     // flags
  int               volume;                    // volume
  double            price_order;               // order price
  //---
  //--- group records
  //----
  char              group[16];             // client group
  //--- Group::EnCoverMode
  UINT              cover_mode;
  char              on_behalf[64];         // on behalf of xxx
  //---
  //--- symbol records
  //----
  char              ccy_pair[64];          // currency pair (integral)
  char              base_cry[32];          // base currency         ?
  char              quote_cry[4];          // quote currency        ?
  //---           
  double            min_spread;			    // minimum spread
  double            bid_spread;			    // bid spread
  double            ask_spread;			    // ask spread
  double            max_spread;			    // maximum spread
  double            mrkt_range;			    // market range
  //--- MarketRange::EnMarketRange
  UINT              range_type;
  double            pips_factor;           // pips factor
  //--- Instrument::EnPriceAlgo                                    
  UINT              price_algo;            // price algo
  double            price_size;            // price size
  bool              bias_bid;              // bid biaz
  double            min_qty;               // min quantity
  char              time_in_force;         // =FIX::TimeInForce_GOOD_TILL_DATE;
  int               expiration_time;       // =30;
  bool              enabled;               // =false;
  double            lot_conversion;        // =1.0;
  //                                       
  int               digits;                // security precision
  double            spread;                // spread
  int               spread_balance;        // spread balance
  int               exec_mode;             // execution mode
  double            contract_size;         // contract size
  int               symbol_type;           // security group (see ConSymbolGroup)
  char              name[16];              // group name (con group)
  //---             
  //bool              symbol_enabled;
  double            prices[2];
  //---             
  INT               request_time;
  time_t            open_time;
  bool              manager_order;
  int               manager;
  int               spread_diff;           // spread difference in compare with default security spread
  //--- fill order from request
  static bool       Fill(const TradeRequest *record,IntegralOrder &order);
  //--- fill fro symbol and group records
  static bool       Fill(const Instrument *symbol,const Group *group,IntegralOrder &order);
  //--- fill order from trade
  //static bool       Fill(const TradeRecord *trade,const UINT type,IntegralOrder &order);
  //--- generate order ID in an external trading system
  static INT        OrderExchangeIDNext();
  //--- generate unique ID in an integral trading system
  static LPCSTR     OrderUniqueIDNext(const IntegralOrder &order,StringType &res);
  static bool       OrderIsClose(const IntegralOrder &order);
  static bool       OrderIsBuy(const IntegralOrder &order,StringType &res);
  static bool       OrderIsStop(const IntegralOrder &order);
  static bool       OrderIsLimit(const IntegralOrder &order,const bool isStop);
  //--- print order
  //static LPCSTR     Print(const IntegralOrder &order,StringType &res);
  //--- print order type
  static LPCSTR     PrintOrderType(const IntegralOrder &order,StringType &res);
  //--- print order action type
  static LPCSTR     PrintOrderAction(const IntegralOrder &order,StringType &res);
  //--- print order type
  static LPCSTR     PrintRequestType(const UINT type,StringType &res);
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Fill order structure                                             |
//+------------------------------------------------------------------+
inline bool IntegralOrder::Fill(const TradeRequest *request,IntegralOrder &order)
  {
  StringType256 unique_id;
  //--- checks
  if(!request)
    return(false);
  //---
  ZeroMemory(&order,sizeof(IntegralOrder));
  //---
  order.request_id    =request->request_id;
  order.login         =request->login;
  StringType::Copy(order.symbol,request->symbol);
  StringType::Copy(order.group,request->group);
  order.cmd           =request->cmd;
  order.price_order   =request->price_order;
  //order.flags         =record->request.trade.flags;
  order.volume        =request->volume;
  order.order         =request->order;
  order.request_action=request->order_action;
  order.open_time     =request->open_time;
  order.order_state   =request->order_state;
  order.time_setup    =_time64(NULL);
  order.manager       =request->manager;
  order.manager_order =request->manager>0;
  //--- generate/set uniqueu id
  IntegralOrder::OrderUniqueIDNext(order,unique_id);
  StringType::Copy(order.unique_id,unique_id.Str());
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| Fill order structure                                             |
//+------------------------------------------------------------------+
inline bool IntegralOrder::Fill(const Instrument *instrument,const Group *group,IntegralOrder &order)
  {
  StringType256 unique_id;
  //--- checks
  if(!instrument||!group)
    return(false);
  //--- [SESSION] ShortName
  StringType::Copy(order.short_name,group->short_name);
  //--- ESF-1585
  if(StringType::Compare(group->short_name,"@CCY")==0)
    {
    StringType::Copy(order.short_name,instrument->short_name);
    }
  //---
  StringType::Copy(order.group,group->group);
  StringType::Copy(order.on_behalf,group->on_behalf);
  StringType::Copy(order.base_cry,instrument->base_cry);
  StringType::Copy(order.quote_cry,instrument->quote_cry);
  order.cover_mode   =group->cover_mode;
  //order.group_enabled=group->enabled;
  //--- security group settings
  //--- symbol
  StringType::Copy(order.symbol,instrument->symbol);            // name (mt4)
  StringType::Copy(order.ccy_pair,instrument->ccy_pair);        // target symbol (integral)
  order.min_spread=instrument->min_spread;			             // minimum spread
  order.bid_spread=instrument->bid_spread;			             // bid spread
  order.ask_spread=instrument->ask_spread;			             // ask spread
  order.max_spread=instrument->max_spread;			             // maximum spread
  order.mrkt_range=instrument->mrkt_range;			             // market range
  order.range_type=instrument->range_type;                      // range type
  //---
  order.price_algo     =instrument->price_algo;                 // price algo
  order.price_size     =instrument->price_size;                 // price size
  order.bias_bid       =instrument->bias_bid;                   // bid biaz
  order.min_qty        =instrument->min_qty;                    // min quantity
  order.time_in_force  =instrument->time_in_force;              // =FIX::TimeInForce_GOOD_TILL_DATE;
  order.expiration_time=instrument->expiration_time;            // =30;
  //order.symbol_enabled =symbol->enabled;                      // =false;
  order.lot_conversion =instrument->lot_conversion;             // =1.0;
  order.digits         =instrument->digits;                     // security precision
  order.spread         =instrument->spread;                     // spread
  order.spread_balance =instrument->spread_balance;             // spread balance
  order.exec_mode      =instrument->exec_mode;                  // execution mode
  order.contract_size  =instrument->contract_size;              // contract size
  order.symbol_type    =instrument->type;                       // security group (see ConSymbolGroup)
  //char            name[16];          // group name (con group)
  //order.volume         =SMTMath::VolumeToSize(UINT(order.volume/symbol->lot_conversion),symbol->contract_size);
  //--- set adjusted price
  //order.price_order    =order.price_order*symbol->lot_conversion;
  order.spread_diff    =group->secgroups[instrument->type].spread_diff;
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| Fill order structure                                             |
//+------------------------------------------------------------------+
//inline bool IntegralOrder::Fill(const TradeRecord *trade,const UINT type,IntegralOrder &order)
//  {
//  StringType256 unique_id;
//  //--- checks
//  if(!trade)
//    return(false);
//  //---
//  ZeroMemory(&order,sizeof(IntegralOrder));
//  //---
//  StringType::Copy(order.symbol,trade->symbol);
//  order.login       =trade->login;
//  order.cmd         =trade->cmd;
//  order.order       =trade->order;
//  //order.deal_status =FIX::OrdStatus_NEW;
//  order.expiration  =trade->expiration;
//  order.order       =trade->order;
//  order.price_order =trade->open_price;
//  order.volume      =trade->volume;
//  order.request_action=type;
//  order.request_id  =(-trade->order);
//  //--- generate/set uniqueu id
//  IntegralOrder::OrderUniqueIDNext(order,unique_id);
//  StringType::Copy(order.unique_id,unique_id.Str());
//  //---
//  return(true);
//  }
//+------------------------------------------------------------------+
//| Generate order ID in an external trading system                  |
//+------------------------------------------------------------------+
inline INT IntegralOrder::OrderExchangeIDNext()
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
//| close order request                                              |
//+------------------------------------------------------------------+
inline bool IntegralOrder::OrderIsClose(const IntegralOrder &order)
  {
  bool res=false;
  //---
  switch(order.request_action)
    {
    case TradeRequest::IDC_CLOSE:
    case TradeRequest::IDC_TP:
    case TradeRequest::IDC_SL:
    case TradeRequest::IDC_STOPOUT:
      res=true;
      break;
    default:
      break;
    }
  //---
  return(res);
  }
//+------------------------------------------------------------------+
//| order type                                                       |
//+------------------------------------------------------------------+
inline bool IntegralOrder::OrderIsBuy(const IntegralOrder &order,StringType &str)
  {
  bool res=false;
  //---
  str.Clear();
  switch(order.cmd)
    {
    case OP_BUY:
    case OP_BUY_LIMIT:
    case OP_BUY_STOP:
      str.Assign("B");
      res=true;
      break;
    default:
      str.Assign("S");
      break;
    }
  //---
  return(res);
  }
//+------------------------------------------------------------------+
//| order type                                                       |
//+------------------------------------------------------------------+
inline bool IntegralOrder::OrderIsStop(const IntegralOrder &order)
  {
  bool res=false;
  //---
  switch(order.cmd)
    {
    case OP_BUY_STOP:
    case OP_SELL_STOP:
      res=true;
      break;
    default:
      if(order.request_action==TradeRequest::IDC_SL)
        res=true;
      break;
    }
  //---
  return(res);
  }
//+------------------------------------------------------------------+
//| order type                                                       |
//+------------------------------------------------------------------+
inline bool IntegralOrder::OrderIsLimit(const IntegralOrder &order,const bool isStop)
  {
  //---
  return((order.cmd == OP_BUY_LIMIT || order.cmd == OP_SELL_LIMIT || order.request_action == TradeRequest::IDC_TP) || (order.exec_mode == EXE_INSTANT && order.ie_deviation == 0)) && order.request_action != TradeRequest::IDC_STOPOUT && !isStop; // && !tradeRequest.isManager
// && !tradeRequest.isManager
  }
//+------------------------------------------------------------------+
//| generate unique ID                                               |
//+------------------------------------------------------------------+
inline LPCSTR IntegralOrder::OrderUniqueIDNext(const IntegralOrder &order,StringType &str)
  {
  StringType16 type_str;
  short        cmd=OP_SELL;
  //--- order counter
  static UINT64 s_order_exchange_count(0);
  //--- order counter change synchronization object
  static CMTSync s_sync;
  //--- lock
  s_sync.Lock();
  //--- increment deal counter value
  s_order_exchange_count++;
  //--- unlock
  s_sync.Unlock();
  //---
  switch(order.cmd)
    {
    case OP_BUY:
    case OP_BUY_LIMIT:
    case OP_BUY_STOP:
      cmd=OP_BUY;
      break;
    default:
      break;
    }
  if(order.request_action==TradeRequest::IDC_CLOSE||
     order.request_action==TradeRequest::IDC_TP||
     order.request_action==TradeRequest::IDC_SL||
     order.request_action==TradeRequest::IDC_STOPOUT)
    {
    cmd=(cmd==OP_BUY)?OP_SELL:OP_BUY;
    }
  //--- print type
  PrintRequestType(order.request_action,type_str);
  //--- prepare with time
  str.Format("idc:%s;order:%d;man:%d;id:%d;time:%I64d;login:%d;symbol:%s;side:%s;volume:%0.2lf;internal:%I64u;",
             type_str.Str(),
             order.order,
             order.manager,
             order.request_id,
             order.time_setup,
             order.login,
             order.symbol,
             (cmd==OP_BUY)?"B":"S",
             order.volume/100.0,
             s_order_exchange_count);
  //--- return unique id value
  return(str.Str());
  }
//+------------------------------------------------------------------+
//| Print order                                                      |
//+------------------------------------------------------------------+
//inline LPCSTR IntegralOrder::Print(const IntegralOrder &order,StringType &res)
//  {
//  StringType32   order_type;
//  StringType32   order_action;
//  StringTypePath tmp;
//  //--- clear resulting string
//  res.Clear();
//  //--- print order type
//  PrintOrderType(order,order_type);
//  //--- print order action type
//  PrintOrderAction(order,order_action);
//  //--- print depending on order action type
//  if(order.request_type==TT_PRICES_GET)
//    {
//    //--- print price requesting order
//    res.Format("prices for %s %.2lf",order.symbol,double(order.volume));
//    }
//  else
//    {
//    //--- print orders of other action types
//    res.Format("#%d %s %s %.2lf %s at ",
//               order.order,
//               order_action.Str(),
//               order_type.Str(),
//               double(order.volume),
//               order.symbol);
//    //--- print price
//    if(order.price_order!=0)
//      tmp.Format("%.5lf",order.price_order);
//    else
//      tmp.Assign("market");
//    //---
//    res.Append(tmp);
//    //--- print Stop Loss if it is set
//    if(order.price_SL!=0)
//      {
//      tmp.Format(" sl: %.5lf",order.price_SL);
//      res.Append(tmp);
//      }
//    //--- print Take Profit if it is set
//    if(order.price_TP!=0)
//      {
//      tmp.Format(" tp: %.5lf",order.price_TP);
//      res.Append(tmp);
//      }
//    }
//  //--- return result
//  return(res.Str());
//  }
//+------------------------------------------------------------------+
//| Print order type                                                 |
//+------------------------------------------------------------------+
inline LPCSTR IntegralOrder::PrintOrderType(const IntegralOrder &order,StringType &res)
  {
  //--- form the string value of order type
  switch(order.cmd)
    {
    case OP_BUY:        res.Assign("buy");               break;
    case OP_SELL:       res.Assign("sell");              break;
    case OP_BUY_LIMIT:  res.Assign("buy limit");         break;
    case OP_SELL_LIMIT: res.Assign("sell limit");        break;
    case OP_BUY_STOP:   res.Assign("buy stop");          break;
    case OP_SELL_STOP:  res.Assign("sell stop");         break;
    default:            res.Assign("");                  break;
    }
  //--- return result
  return(res.Str());
  }
//+------------------------------------------------------------------+
//| Print order type                                                 |
//+------------------------------------------------------------------+
inline LPCSTR IntegralOrder::PrintOrderAction(const IntegralOrder &order,StringType &res)
  {
  //--- form the string value of order action type
  switch(order.request_action)
    {
    case TradeRequest::IDC_OPEN:    res.Assign("open");        break;
    case TradeRequest::IDC_CLOSE:   res.Assign("close");       break;
    case TradeRequest::IDC_PENDING: res.Assign("pending");     break;
    case TradeRequest::IDC_TP:      res.Assign("take profit"); break;
    case TradeRequest::IDC_SL:      res.Assign("stop loss");   break;
    case TradeRequest::IDC_STOPOUT: res.Assign("stopout");     break;
    default: res.Assign("unknown");                           break;
    }
  //--- return result
  return(res.Str());
  }
//+------------------------------------------------------------------+
//| Print order type                                                 |
//+------------------------------------------------------------------+
inline LPCSTR IntegralOrder::PrintRequestType(const UINT type,StringType &res)
  {
  //--- form the string value of order action type
  switch(type)
    {
    case TradeRequest::IDC_OPEN:    res.Assign("O");   break;
    case TradeRequest::IDC_CLOSE:   res.Assign("C");   break;
    case TradeRequest::IDC_PENDING: res.Assign("A");   break;
    case TradeRequest::IDC_TP:      res.Assign("T");   break;
    case TradeRequest::IDC_SL:      res.Assign("S");   break;
    case TradeRequest::IDC_STOPOUT: res.Assign("M");   break;
    default:
      res.Assign("U");
      break;
    }
  //--- return result
  return(res.Str());
  }
//+------------------------------------------------------------------+
//| Array of orders                                                  |
//+------------------------------------------------------------------+
typedef TMTArray<IntegralOrder> IntegralOrdersArray;
//+------------------------------------------------------------------+
