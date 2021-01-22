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
struct IntegralMarketDataRequest
  {
  bool             enabled;           // ccy pair is enabled
  FIX::Symbol      ccy_pair;          // currency pair (integral)
  FIX::StringRef   symbol;            // symbol (mt4)
  FIX::StringRef   short_name;        // short name
  //---
  char             base_cry[32];      // base currency
  char             quote_cry[4];      // quote currency
  int	             market_depth;      // market depth
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Array of orders                                                  |
//+------------------------------------------------------------------+
typedef TMTArray<IntegralMarketDataRequest> IntegralMarketDataRequestsArray;
//+------------------------------------------------------------------+
//| Order message                                                    |
//+------------------------------------------------------------------+
struct IntegralMsgMarketDataRequest
  {
  //--- message header
  IntegralMsgHeader header;
   //---
  char              short_name[256];
  FIX::SessionID    session_id;
  //--- message data
  IntegralMarketDataRequest request;
  //--- serialization/deserialization methods
  static bool       Write(const IntegralMsgMarketDataRequest &msg,CIntegralContextBase &context);
  };
//+------------------------------------------------------------------+
//| Write message structure to buffer                                |
//+------------------------------------------------------------------+
inline bool IntegralMsgMarketDataRequest::Write(const IntegralMsgMarketDataRequest &msg,CIntegralContextBase &context)
  {
  FIX43::MarketDataRequest message;
  //--- checks
  //if(msg.request.ccy_pair.getString().size()!=7)
  //  {
  //  LOG_ERROR(pluginLog,"Ignoring due to invalid configuration: %s=%s",msg.request.symbol,msg.request.ccy_pair);
  //  //---
  //  return(false);
  //  }
  //--- check
  std::string oCCY=msg.request.base_cry;
  oCCY.append(msg.request.quote_cry);
  //if(msg.request.symbol.size()>=6&&StringType::Compare(msg.request.symbol.data(),oCCY.data(),6)!=0)
  //  {
  //  LOG_ERROR(pluginLog,"Inconsistent instrument configuration mapping: %s=%s",msg.request.symbol,msg.request.ccy_pair);
  //  //---
  //  return(false);
  //  }
  //---
  StringType128 temp;
  temp.Format("MT4-%s-%s",msg.request.short_name,msg.request.ccy_pair);
  //---
  WRITE_MSG_FIELD(FIX::MDReqID(temp.Str()));
  //---
  WRITE_MSG_FIELD(FIX::SubscriptionRequestType(FIX::SubscriptionRequestType_SNAPSHOT_PLUS_UPDATES));
  //---
  WRITE_MSG_FIELD(FIX::MarketDepth(msg.request.market_depth));
  // Limit the pricing tiers to 5, no matter what
  if(msg.request.market_depth<1||msg.request.market_depth>5)
    WRITE_MSG_FIELD(FIX::MarketDepth(5));//.setValue(5);
  //---
  FIX43::MarketDataRequest::NoRelatedSym nrs;
  //---  we will fill prev
  if(!IntegralMsgHeader::Write(msg.header,message))
    return(false);
  //mdr.getHeader().set(FIX::SenderSubID(m_settings.get(it).getString("SenderSubID")));
  //mdr.getHeader().set(FIX::DeliverToCompID(m_settings.get(it).getString("DeliverToCompID")));
  nrs.set(msg.request.ccy_pair);
  nrs.set(FIX::Product(FIX::Product_CURRENCY));
  message.addGroup(nrs);
  //---
  WRITE_MSG_FIELD(FIX::MDUpdateType(FIX::MDUpdateType_FULL_REFRESH));
  WRITE_MSG_FIELD(FIX::AggregatedBook(FIX::AggregatedBook_YES));
  //---
  FIX43::MarketDataRequest::NoMDEntryTypes nmt;
  nmt.set(FIX::MDEntryType(FIX::MDEntryType_BID));
  message.addGroup(nmt);
  nmt.set(FIX::MDEntryType(FIX::MDEntryType_OFFER));
  message.addGroup(nmt);
  //---
  LOG_INFO(pluginLog,"MarketDataRequest: %s - %s",msg.request.ccy_pair,message.toString().data());
  //--- prepare for sending
  message.setSessionID(msg.session_id);
  //--- send
  return(context.Send(message));
  }
//+------------------------------------------------------------------+
