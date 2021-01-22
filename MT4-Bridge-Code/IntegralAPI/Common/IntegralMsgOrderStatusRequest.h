//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include "IntegralContextBase.h"
#include "..\..\API\MT4APITools.h"
//+------------------------------------------------------------------+
//| Execution structure                                              |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct IntegralOrderStatusRequest
  {
  FIX::Side         side;              // deal execution side
  FIX::CumQty       volume;            // deal volume
  FIX::LeavesQty    volume_remaind;    // unfilled order volume
  FIX::AvgPx        price;             // deal price
  FIX::Symbol       ccy_pair;          // symbol (integral)
  FIX::OrdRejReason reject_reason;     // order reject reason
  //---                               
  FIX::ExecID       external_id;       // Unique identifier assigned by sell-side (intergal)
  FIX::OrdStatus    status;            // Identifies current status of order.
  FIX::OrderID      order_id;          // Unique identifier for Order as assigned by sell-side
  FIX::ClOrdID      unique_id;         // Unique identifier for Order as assigned by the buy-side (broker)
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Array of orders                                                  |
//+------------------------------------------------------------------+
typedef TMTArray<IntegralOrderStatusRequest> IntegralOrderStatusRequestsArray;
//+------------------------------------------------------------------+
//| Order message                                                    |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct IntegralMsgOrderStatusRequest
  {
  //--- message header
  IntegralMsgHeader header;
  FIX::SessionID             session_id;
  //--- message data
  FIX43::OrderStatusRequest  request;
  //--- serialization/deserialization methods
  static bool       Write(IntegralMsgOrderStatusRequest &msg,CIntegralContextBase &context);
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Write message structure to buffer                                |
//+------------------------------------------------------------------+
inline bool IntegralMsgOrderStatusRequest::Write(IntegralMsgOrderStatusRequest &msg,CIntegralContextBase &context)
  {
  //--- send
  return(context.Send(msg.request,msg.session_id));
  }
//+------------------------------------------------------------------+
