//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include "IntegralContextBase.h"
#include "..\..\API\MT4APITools.h"
//+------------------------------------------------------------------+
//| Order message                                                    |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct IntegralMsgDontKnowTrade
  {
  //--- message header
  IntegralMsgHeader header;
  //--- FIX::DKReasonCode
  UINT              code;
  FIX::SessionID    session_id;
  //--- message data
  IntegralExecution execution;
  //--- execution report
  FIX43::DontKnowTrade trade;
  //--- calculate volume
  static void       PrintReason(const IntegralMsgDontKnowTrade &msg);
  //--- serialization/deserialization methods
  static bool       Write(IntegralMsgDontKnowTrade &msg,CIntegralContextBase &context);
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| calculate volume                                                 |
//+------------------------------------------------------------------+
inline void IntegralMsgDontKnowTrade::PrintReason(const IntegralMsgDontKnowTrade &msg)
  {
  switch(msg.code)
    {
    case IntegralExecution::MT4_ORDER_TIMED_OUT:
      LOG_RISK(pluginLog,"DK due to MT4 timeout for pending NewOrderSingle at Grid '%s' / %s",
               msg.execution.fix_unique_id,
               msg.execution.message);
      break;
    case IntegralExecution::MT4_RESTARTED:
      LOG_RISK(pluginLog,"DK due to MT4 restart for pending NewOrderSingle at Grid '%s' / %s",
               msg.execution.fix_unique_id,
               msg.execution.message);
      break;
    case IntegralExecution::DONT_KNOW_ORDER:
      LOG_RISK(pluginLog,"Unknown ExecutionReport for Order '%s' / '%s' / '%s' - %s",
               msg.execution.fix_unique_id,
               msg.execution.fix_order_id,
               msg.execution.fix_external_id,
               msg.execution.message);
      break;
    default:
      break;
    }
  }
//+------------------------------------------------------------------+
//| Write message structure                                          |
//+------------------------------------------------------------------+
inline bool IntegralMsgDontKnowTrade::Write(IntegralMsgDontKnowTrade &msg,CIntegralContextBase &context)
  {
  bool res=false;
  //FIX43::DontKnowTrade message(FIX::OrderID(msg.execution.fix_order_id),
  //                             FIX::ExecID(msg.execution.fix_external_id),
  //                             FIX::DKReason(FIX::DKReason_OTHER),
  //                             FIX::Side(msg.execution.fix_side));
  ////--- write header
  //if(!IntegralMsgHeader::Write(msg.header,message))
  //  return(false);
  ////--- print reason
  //IntegralMsgDontKnowTrade::PrintReason(msg);
  ////--- write order quantity
  //WRITE_MSG_FIELD(FIX::OrderQty(msg.execution.fix_volume));
  ////--- write symbol
  ////WRITE_MSG_FIELD(msg.execution.fix_ccy_pair);
  ////--- write unique id
  ////WRITE_MSG_FIELD(msg.execution.fix_unique_id);
  ////--- write account id
  ////WRITE_MSG_FIELD(msg.execution.fix_account_id);
  ////--- write reason
  //WRITE_MSG_FIELD(FIX::DKReasonCode(msg.code));
  ////--- write text
  //WRITE_MSG_FIELD(FIX::Text(msg.execution.fix_text));
  //--- prepare for sending
  //IntegalMessageRecord send_msg;
  //IntegalMessageRecord::Fill(msg.trade,"",msg.session_id,send_msg);
  //--- send
  return(context.Send(msg.trade,msg.session_id));
  }
//+------------------------------------------------------------------+
