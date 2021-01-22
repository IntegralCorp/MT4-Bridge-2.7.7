//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include "IntegralOrder.h"

#include "IntegralMsg.h"
#include "..\..\IO\Log.h"
#include "..\..\Common\Helpers.h"
#include "..\Bases\Group.h"
//#include "..\..\Risk\WatchTower.h"
#include "..\..\Bases\Trades\TradeRequest.h"
#include "IntegralContextBase.h"
//+------------------------------------------------------------------+
//| Order message                                                    |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct IntegralMsgOrder
  {
  //--- message header
  IntegralMsgHeader    header;
  //---
  //bool                 paper_trading;
  bool                 stops_as_market;
  bool                 limits_as_market;
  char                 user[256];
  //char                 short_name[256];
  bool                 is_admin;
  //----
  FIX::SessionID       session_id;      // session id
  //--- message data
  IntegralOrder        order;
  //--- serialization methods
  static bool          Write(IntegralMsgOrder &msg,CIntegralContextBase &context);
  //--- helper
  static double        GetSpread(const bool isBuy,const IntegralMsgOrder &message);
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Write structure to fix                                           |
//+------------------------------------------------------------------+
inline bool IntegralMsgOrder::Write(IntegralMsgOrder &msg,CIntegralContextBase &context)
  {
  FIX::HandlInst handle_inst;
  FIX::OrdType   ord_type;
  FIX::ClOrdID   unique_id;
  StringType16   buy_str;
  StringType16   symbol_str;
  StringType16   currency_str;
  bool           res=false;
  //---
  bool isClose=IntegralOrder::OrderIsClose(msg.order);
  bool isBuy=IntegralOrder::OrderIsBuy(msg.order,buy_str);
  //---- we dont need this; we reversed sides already
  if(isClose)
    isBuy=!isBuy;
  //----
  bool isStop=IntegralOrder::OrderIsStop(msg.order);
  bool isLimit=IntegralOrder::OrderIsLimit(msg.order,isStop);
  //---
  //isStop=(ExtPluginContext.EnableAsyncOrders()&&isStop);
  //---
  /*if(msg.paper_trading)
    {
    isLimit=false;
    isStop =false;
    }*/
    //---
  if(isLimit)
    isLimit=!msg.limits_as_market;
  //---
  if(isStop)
    isStop=!msg.stops_as_market;
  //--- create message
  handle_inst=FIX::HandlInst(FIX::HandlInst_AUTOMATED_EXECUTION_ORDER_PRIVATE_NO_BROKER_INTERVENTION);
  ord_type=FIX::OrdType(isLimit?FIX::OrdType_LIMIT:(isStop?FIX::OrdType_STOP:FIX::OrdType_MARKET));
  unique_id=FIX::ClOrdID(msg.order.unique_id);
  //---
  FIX43::NewOrderSingle message(unique_id,
                                handle_inst,
                                FIX::Side(isBuy?FIX::Side_BUY:FIX::Side_SELL),
                                FIX::TransactTime(),
                                ord_type);
  //--- write header
  /*if(!IntegralMsgHeader::Write(msg.header,message))
    {
    return(false);
    }*/
  if(!msg.header.sender_comp_id.getValue().empty())
    message.getHeader().set(msg.header.sender_comp_id);

  //WRITE_MSG_HEADER_FIELD(FIX::SenderSubID(msg.header.sender_comp_id));
  //--- write target comp id

  if(!msg.header.target_comp_id.getValue().empty())
    message.getHeader().set(msg.header.target_comp_id);

  //WRITE_MSG_HEADER_FIELD(FIX::TargetCompID(msg.header.target_comp_id));
  //--- write deliver to comp id
  if(!msg.header.deliver_to_comp_id.getValue().empty())
    message.getHeader().set(msg.header.deliver_to_comp_id);

  //CIntegralMsg::SetHeaderField(message,FIX::DeliverToCompID(msg.header.deliver_to_comp_id));
  //--- write on behalf of comp id
  if(!msg.header.on_behalf_of_comp_id.getValue().empty())
    message.getHeader().set(msg.header.on_behalf_of_comp_id);

  //CIntegralMsg::SetHeaderField(message,FIX::OnBehalfOfCompID(msg.header.on_behalf_of_comp_id));
  //--- write sender sub id
  if(!msg.header.sender_sub_id.getValue().empty())
    message.getHeader().set(msg.header.sender_sub_id);

  //CIntegralMsg::SetHeaderField(message,FIX::SenderSubID(msg.header.sender_sub_id));
  //--- write deliver to comp id
  if(!msg.header.deliver_to_comp_id.getValue().empty())
    message.getHeader().set(msg.header.deliver_to_comp_id);

  //CIntegralMsg::SetHeaderField(message,FIX::DeliverToCompID(msg.header.deliver_to_comp_id));
  //--- write target sub id
  if(!msg.header.target_sub_id.getValue().empty())
    message.getHeader().set(msg.header.target_sub_id);

  //CIntegralMsg::SetHeaderField(message,FIX::TargetSubID(msg.header.target_sub_id));
  //--- write on behalf of sub id
  if(!msg.header.on_behalf_of_sub_id.getValue().empty())
    message.getHeader().set(msg.header.on_behalf_of_sub_id);
  //CIntegralMsg::SetHeaderField(message,FIX::OnBehalfOfSubID(msg.header.on_behalf_of_sub_id));

  //---
  message.set(FIX::Currency(std::string(msg.order.base_cry)));
  message.set(FIX::ExecInst(FIX::MULTIPLECHARVALUE(isStop?"B R":"B")));

  double ordr_qty=SPluginHelpers::roundTo(((msg.order.volume/100.0)*msg.order.contract_size)*msg.order.lot_conversion,2);
  message.set(FIX::OrderQty(ordr_qty));


  //WRITE_MSG_FIELD(FIX::Currency(std::string(msg.order.base_cry)));
  //WRITE_MSG_FIELD(FIX::ExecInst(FIX::MULTIPLECHARVALUE(isStop?"B R":"B")));
  //WRITE_MSG_FIELD(FIX::OrderQty(SMTMath::PriceNormalize(msg.order.volume,2)));
  //---
  FIX::StringRef sym(msg.order.ccy_pair);
  double oBidSpread=msg.order.bid_spread;
  double oAskSpread=msg.order.ask_spread;
  //--- set spreads
  if(msg.order.prices[0]>0||msg.order.prices[1]>0)
    {
    oBidSpread=(msg.order.prices[0]>0)?msg.order.prices[0]:oBidSpread;
    oAskSpread=(msg.order.prices[1]>0)?msg.order.prices[1]:oAskSpread;
    }
  //---
  double groupSpread=IntegralMsgOrder::GetSpread(isBuy,msg);
  oBidSpread+=groupSpread;
  oAskSpread+=groupSpread;
  //---
  /*if(msg.paper_trading)
    {
    oBidSpread=0;
    oAskSpread=0;
    message.set(FIX::MinQty(0));
    }
  else
    {*/
  double min_qty=SPluginHelpers::roundTo((msg.order.min_qty<=1?(((msg.order.volume*msg.order.min_qty)/100.0)*msg.order.contract_size):min(msg.order.min_qty,SMTMath::VolumeToSize(msg.order.volume,msg.order.contract_size)))*msg.order.lot_conversion,2);
  //---
  message.set(FIX::MinQty(min_qty));
  //}

  if(isStop)
    message.set(FIX::StopPx(SPluginHelpers::roundTo((msg.order.price_order+(isBuy?oAskSpread*-1.0:oBidSpread*1.0))/msg.order.lot_conversion,msg.order.digits)));

  double price=SPluginHelpers::roundTo((msg.order.price_order+(isBuy?oAskSpread*-1.0:oBidSpread*1.0))/msg.order.lot_conversion,msg.order.digits);
  message.set(FIX::Price(price));

  //bool EnableAsyncOrders=true;
  bool forceFOK=false;//msg.paper_trading;
  bool forceIOC=false;
  /*switch(msg.order.request_action)
    {
    case TradeRequest::IDC_PENDING:
    case TradeRequest::IDC_SL:
    case TradeRequest::IDC_TP:
      if(!ExtPluginContext.EnableAsyncOrders())
        forceIOC=true;
      break;
    default:
      break;
    }*/
    //---
  bool forceGTD=false;
  switch(msg.order.request_action)
    {
    case TradeRequest::IDC_PENDING:
    case TradeRequest::IDC_SL:
    case TradeRequest::IDC_TP:
      //if(ExtPluginContext.EnableAsyncOrders())
      forceGTD=true;
      break;
    default:
      break;
    }
  //---
  message.set(FIX::TimeInForce(forceFOK?FIX::TimeInForce_FILL_OR_KILL:(forceIOC?FIX::TimeInForce_IMMEDIATE_OR_CANCEL:(forceGTD?FIX::TimeInForce_GOOD_TILL_DATE:msg.order.time_in_force))));
  //---
  int oExpire=min((msg.order.expiration>0?(msg.order.expiration-msg.order.request_time)-1:msg.order.expiration_time),msg.order.expiration_time);
  if(oExpire<=0&&msg.order.request_action==TradeRequest::IDC_PENDING) // Prevent expiry in MT4 before Grid response
    {
    LOG_ERROR(pluginLog,"Expiry Time less than a second away #%d",msg.order.order);
    return false;
    }
  else if(oExpire<=0)
    oExpire=msg.order.expiration_time;

  message.set(FIX::ExpireTime(FIX::UTCTIMESTAMP(0,0,oExpire,1,1,1970)));
  message.set(FIX::SecurityType(FIX::SecurityType_FOREIGN_EXCHANGE_CONTRACT));
  message.set(FIX::Symbol(msg.order.ccy_pair));        // Tag:55  
  message.set(FIX::Product(FIX::Product_CURRENCY));

  if(msg.order.cover_mode!=Group::EnCoverMode::STP)
    {
    StringType16 cover_mode;
    Group::PrintCoverMode(msg.order.cover_mode,cover_mode);
    message.setField(FIX::AdditionalExecInst(cover_mode.Str()));
    }

  /*if(msg.paper_trading)
    message.setField(FIX::PegOffsetValue(1000));
  else */
  if(msg.order.request_action==TradeRequest::IDC_SL||msg.order.request_action==TradeRequest::IDC_STOPOUT||msg.order.request_action==TradeRequest::IDC_PENDING)
    {
    // No range can be set
    }
  else if(msg.order.ie_deviation>0&&msg.order.exec_mode==EXE_INSTANT)
    message.setField(FIX::PegOffsetValue(SPluginHelpers::roundTo(msg.order.ie_deviation/msg.order.lot_conversion,1)));
  else if(msg.order.mrkt_range>=0&&!isLimit)
    {
    //message.setField(FIX::PegOffsetValue(SPluginHelpers::roundTo(msg.order.mrkt_range/msg.order.lot_conversion,1)));
    message.setField(FIX::PegOffsetValue(SPluginHelpers::roundTo(msg.order.mrkt_range/msg.order.lot_conversion,1)));
    }

  message.setField(FIX::Account(std::to_string(msg.order.login)));
  message.setField(FIX::AccountId(std::to_string(msg.order.login)));
  //---
  if(ExtPluginContext.SendGroupTag())
    message.setField(FIX::AccountGroupId(std::string(msg.order.group)));
  //---
  msg.order.spread=SMTMath::PriceNormalize((isBuy?oAskSpread:oBidSpread)/msg.order.lot_conversion,msg.order.digits);
  message.setField(FIX::UserDefinedSpread(msg.order.spread));

  //---
  FIX::TargetCompID target_comp_id;
  message.getHeader().getIfSet(target_comp_id);
  message.setField(target_comp_id.getValue()=="prod.fxgrid"?FIX::OrderChannel("FIX/Order/INTMT4"):FIX::OrderChannel("FIX/Order/MT4"));
  //--- ?
  message.setSessionID(msg.session_id);

  //--- prepare for sending
  //IntegalMessageRecord send_msg;
  //IntegalMessageRecord::Fill(message,msg.order.short_name,msg.session_id,send_msg);
  //--- send
  res=context.Send(message);
  if(res)
    {
    StringType::Copy(msg.order.order_str,message.toString().c_str());
    }
  //---
  return(res);//send_msg));
  }
//+------------------------------------------------------------------+
//| get spread info                                                  |
//+------------------------------------------------------------------+
inline double IntegralMsgOrder::GetSpread(const bool isBuy,const IntegralMsgOrder &message)
  {
  ConGroup groupDef={ 0 };
  int spread,result;
  double oTmp;
  //--- checks
  if(!message.order.group||!ExtServer)
    return(0);
  //---
  if(!ExtServer->GroupsGet(message.order.group,&groupDef))
    {
    LOG_ERROR(pluginLog,"Cannot get information about group %s",message.order.group);
    return 0;
    }
  //--- show
  LOG_DEBUG(pluginLog,"Symbol %0.12s, spread %d, spread balance %d, group difference %d",
            message.order.symbol,
            message.order.spread,
            message.order.spread_balance,
            groupDef.secgroups[message.order.symbol_type].spread_diff);
  //---
  spread=groupDef.secgroups[message.order.symbol_type].spread_diff;
  oTmp=(spread/2.0)+message.order.spread_balance;
  result=static_cast<int>(((message.order.bias_bid&&!isBuy)||(!message.order.bias_bid&&isBuy))?std::ceil(oTmp):std::floor(oTmp));
  //---
  LOG_DEBUG(pluginLog,"Spread is %d",result);
  return result/pow(10,message.order.digits);
  }
//+------------------------------------------------------------------+
