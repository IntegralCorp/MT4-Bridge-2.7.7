//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include <vector>

#include "IntegralContextBase.h"
#include "IntegralMsg.h"
#include "IntegralOrder.h"
//+------------------------------------------------------------------+
//| Execution structure                                              |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct IntegralExecution
  {
  enum EnDKReasonCode
    {
    MT4_ORDER_TIMED_OUT =0,
    MT4_RESTARTED       =1,
    DONT_KNOW_ORDER     =2
    };
  char      short_name[64];               // [SESSION] ShortName
  int       request_id;
  INT64     timestamp;                    // internal usage
  //---     
  char      fix_side;                     // deal execution side
  double    fix_volume;                   // deal volume                    256
  double    fix_volume_remaind;           // unfilled order volume
  double    fix_avg_px;                   // deal price
  char      fix_ccy_pair[64];             // symbol (integral)
  UINT      fix_reject_reason;            // order reject reason
  //---                              
  char      fix_external_id[256];         // Unique identifier assigned by sell-side (intergal)
  //--- IntegralOrder::EnOrderStates
  UINT      order_state;                  // Identifies current status of order.
  char      fix_order_id[256];            // Unique identifier for Order as assigned by sell-side
  char      fix_unique_id[256];           // Unique identifier for Order as assigned by the buy-side (broker)
  //--- EnDKReasonCode
  UINT      fix_reason;                   // 
  INT64     fix_sending_time;
  //----
  char      fix_text[256];
  char      message[2048];
  //+------------------------------------------------------------------+
  //| Is empty the structure                                           |            
  //+------------------------------------------------------------------+
  //bool IsEmpty(void)
  //  {
  //  //--- checks
  //  if(timestamp||!short_name)
  //    return(false);
  //  //--- checks
  //  if(!short_name.Empty())
  //    return(false);
  //  //--- empty
  //  return(true);
  //  }
  //--- print deal type
  static LPCSTR      PrintDealType(const IntegralExecution &deal,StringType &res);
  //--- print execution
  static LPCSTR      Print(const IntegralExecution &deal,StringType &res);
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Print execution                                                  |
//+------------------------------------------------------------------+
inline LPCSTR IntegralExecution::Print(const IntegralExecution &deal,StringType &res)
  {
  StringType32   deal_type;
  StringTypePath tmp;
  //--- clear resulting string
  res.Clear();
  //--- print deal type
  PrintDealType(deal,deal_type);
  //--- generate a resulting string
  res.Format("#s %s %.2lf",
             deal.fix_order_id,
             deal_type.Str(),
             double(deal.fix_volume));
  if(deal.fix_volume_remaind!=0)
    {
    tmp.Format("/%.2lf",double(deal.fix_volume_remaind));
    res.Append(tmp);
    }
  tmp.Format(" %s at market",deal.fix_ccy_pair);
  res.Append(tmp);
  //--- return result
  return(res.Str());
  }
//+------------------------------------------------------------------+
//| Print deal type                                                  |
//+------------------------------------------------------------------+
inline LPCSTR IntegralExecution::PrintDealType(const IntegralExecution &deal,StringType &res)
  {
  //--- form the string value of deal type
  switch(deal.fix_side)
    {
    case 'A': res.Assign("buy");               break;
    case 's': res.Assign("sell");              break;
    default:  res.Assign("unknown");           break;
    }
  //--- return result
  return(res.Str());
  }
//+------------------------------------------------------------------+
//| Array of orders                                                  |
//+------------------------------------------------------------------+
//typedef TMTArray<IntegralExecution> IntegralExecutionsArray;
typedef std::vector<const FIX43::ExecutionReport> IntegralExecutionsArray;
//+------------------------------------------------------------------+
//| Order message                                                    |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct IntegralMsgExecution
  {
  //--- message header
  IntegralMsgHeader header;
  //----
  FIX::SessionID    session_id;
  //--- message data
  IntegralExecution execution;
  //--- serialization/deserialization methods
  static bool       Read(const FIX43::ExecutionReport &report,IntegralMsgExecution &msg);
  static bool       Write(FIX43::ExecutionReport &message,/*LPCSTR short_name,*/FIX::SessionID &session_id,CIntegralContextBase &context);

  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Read message from buffer to structure                            |
//+------------------------------------------------------------------+
inline bool IntegralMsgExecution::Read(const FIX43::ExecutionReport &message,IntegralMsgExecution &msg)
  {
  FIX::Symbol       fix_ccy_pair;
  FIX::Side         fix_side;
  FIX::CumQty       fix_volume;        
  FIX::LeavesQty    fix_volume_remaind;
  FIX::AvgPx        fix_avg_px; 
  FIX::ExecID       fix_external_id; 
  FIX::OrdStatus    fix_status;      
  FIX::OrderID      fix_order_id;    
  FIX::ClOrdID      fix_unique_id; 
  FIX::OrdRejReason fix_reject_reason;
  FIX::SendingTime  fix_sending_time;
  bool res=false;
  //--- get header
  if(!IntegralMsgHeader::Read(message,msg.header))
    return(false);
  //--- checks
  if(msg.header.begin_string!=FIX::BeginString("FIX.4.3")||
     msg.header.type!=FIX::MsgType("8"))
    return(false);
  //--- get status
  if(!message.getFieldIfSet(fix_status))
    return(false);
  //---
  switch(fix_status.getValue())
    {
    case FIX::OrdStatus_FILLED:
      msg.execution.order_state=TradeRequest::ORDER_STATE_FILLED;
      break;
    case FIX::OrdStatus_EXPIRED:
      msg.execution.order_state=TradeRequest::ORDER_STATE_EXPIRED;
      break;
    case FIX::OrdStatus_CANCELED:
      msg.execution.order_state=TradeRequest::ORDER_STATE_CANCELLED;
      break;
    case FIX::OrdStatus_REJECTED:
      //--- set reject reason
      msg.execution.order_state=TradeRequest::ORDER_STATE_REJECTED;
      break;
      READ_MSG_TAG_UINT(fix_reject_reason,msg.execution.fix_reject_reason);
      break;
    default:
      //--- we don't process
      return(false);
    }
  //--- get side
  READ_MSG_FIELD_VALUE_CHAR(fix_side,msg.execution.fix_side);
  //--- get ccy pair
  READ_MSG_FIELD_VALUE_STR(fix_ccy_pair,msg.execution.fix_ccy_pair);
  //--- get avg price
  READ_MSG_TAG_DBL(fix_avg_px,msg.execution.fix_avg_px);
  //--- get unique id
  READ_MSG_FIELD_VALUE_STR(fix_unique_id,msg.execution.fix_unique_id);
  //--- get order id
  READ_MSG_FIELD_VALUE_STR(fix_order_id,msg.execution.fix_order_id);
  //--- get external id
  READ_MSG_FIELD_VALUE_STR(fix_external_id,msg.execution.fix_external_id);
  //--- get volume
  READ_MSG_TAG_DBL(fix_volume,msg.execution.fix_volume);
  //--- get remaining volume if exists
  READ_MSG_TAG_DBL(fix_volume_remaind,msg.execution.fix_volume_remaind);
  //--- optional data
  message.getFieldIfSet(fix_sending_time);
  msg.execution.fix_sending_time=SMTTime::MakeTime(&fix_sending_time.getValue().getTmUtc());
  //READ_MSG_FIELD_IF_SET(msg.execution.fix_account);
  //READ_MSG_FIELD_IF_SET(msg.execution.fix_currency);
  //READ_MSG_FIELD_IF_SET(msg.execution.fix_exec_inst);
  //READ_MSG_FIELD_IF_SET(msg.execution.fix_exec_type);
  //READ_MSG_FIELD_IF_SET(msg.execution.fix_expire_type);
  //READ_MSG_FIELD_IF_SET(msg.execution.fix_fut_sett_date);
  //READ_MSG_FIELD_IF_SET(msg.execution.fix_last_px);
  //READ_MSG_FIELD_IF_SET(msg.execution.fix_last_qty);
  //READ_MSG_FIELD_IF_SET(msg.execution.fix_min_qty);
  //READ_MSG_FIELD_IF_SET(msg.execution.fix_order_qty);
  //READ_MSG_FIELD_IF_SET(msg.execution.fix_order_type);
  //READ_MSG_FIELD_IF_SET(msg.execution.fix_peg_difference);
  //READ_MSG_FIELD_IF_SET(msg.execution.fix_price);
  //READ_MSG_FIELD_IF_SET(msg.execution.fix_product);
  //READ_MSG_FIELD_IF_SET(msg.execution.fix_security_type);
  //READ_MSG_FIELD_IF_SET(msg.execution.fix_settl_currency);
  //READ_MSG_FIELD_IF_SET(msg.execution.fix_settl_curr_amt);
  //READ_MSG_FIELD_IF_SET(msg.execution.fix_time_in_force);
  //READ_MSG_FIELD_IF_SET(msg.execution.fix_trade_date);
  //READ_MSG_FIELD_IF_SET(msg.execution.fix_transact_time);
  //--- successful
  return(true);
  }
//+------------------------------------------------------------------+
//| Write message structure to buffer                                |
//+------------------------------------------------------------------+
inline bool IntegralMsgExecution::Write(FIX43::ExecutionReport &message/*,LPCSTR short_name*/,FIX::SessionID &session_id,CIntegralContextBase &context)
  {
  //FIX::AvgPx   _AvgPx;
  //FIX::CumQty  _CumQty;
  //FIX::ExecID  _ExecID;
  //FIX::ClOrdID _ClOrdID;
  //FIX::OrderID _OrderID;
  bool res=false;
  //--- create report
  //message.getIfSet(_AvgPx);
  //message.getIfSet(_CumQty);
  //message.getIfSet(_ExecID);
  //message.getIfSet(_ClOrdID);
  //message.getIfSet(_OrderID);
  //--- prepare for sending
  IntegalMessageRecord send_msg;
  IntegalMessageRecord::Fill(message,session_id,send_msg);
  //--- send
  return(context.Send(message,session_id));
  }
//+------------------------------------------------------------------+
