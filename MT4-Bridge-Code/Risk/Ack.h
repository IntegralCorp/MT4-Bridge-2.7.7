//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include <string>
//---
#include "..\QuickFix\fix43\ExecutionReport.h"
#include "..\IO\Log.h"
//+------------------------------------------------------------------+
//| Structure with description                                       |
//+------------------------------------------------------------------+
struct Ack
  {
  std::string            ShortName;
  FIX43::ExecutionReport execReport;
  std::string            OrderID;
  FIX::SessionID         session_id;
  //int                    request_id;
  //--
  template<typename T,class F>
  T getValue(F &f,T defaultValue);
  //---
  template<typename T>
  T Get(T &f);
  //---
  template<typename T>
  void Set(FIX43::ExecutionReport &newER,T &field);
  //---
  const FIX43::ExecutionReport AsER();
  //--- filling of structure
  static bool            Fill(const std::string &short_name,const FIX43::ExecutionReport &er,/*int request_id,*/Ack &ack);
  static bool            Fill(const std::string &er,Ack &ack);
  };
//+------------------------------------------------------------------+
//| Filling of structure                                             |
//+------------------------------------------------------------------+
inline bool Ack::Fill(const std::string &short_name,const FIX43::ExecutionReport &er,/*int request_id,*/Ack &acknowledge)
  {
  //-- checks
  if(short_name.empty())
    return(false);
  //---
  try
    {
    FIX::BeginString begin_str;
    FIX::SenderCompID sender_comp_id;
    FIX::TargetCompID target_comp_id;

    er.getHeader().getIfSet(begin_str);
    er.getHeader().getIfSet(sender_comp_id);
    er.getHeader().getIfSet(target_comp_id);

    acknowledge.session_id= FIX::SessionID(begin_str.getString(),target_comp_id.getString(),sender_comp_id.getString());

    acknowledge.ShortName = short_name;
    acknowledge.execReport = er;
    //---
    acknowledge.OrderID =acknowledge.getValue<std::string>(FIX::OrderID(),"0");
    //---
    acknowledge.execReport.set(FIX::ExecType(FIX::ExecType_ORDER_STATUS));
    acknowledge.execReport.set(FIX::OrdStatus(FIX::OrdStatus_DONE_FOR_DAY));
    //---
    return(true);
    }
  catch(const std::exception &e)
    {
    LOG_ERROR(pluginLog,"error in ack [%s]",e.what());
    }
  //---
  return(false);
  }
//+------------------------------------------------------------------+
//| Filling of structure                                             |
//+------------------------------------------------------------------+
inline bool Ack::Fill(const std::string &er,Ack &ack)
  {
  FIX43::ExecutionReport exec;
  //-- checks
  if(er.empty())
    return(false);
  //---
  try
    {
    exec=FIX43::ExecutionReport(FIX::Message(er));

    FIX::BeginString begin_str;
    FIX::SenderCompID sender_comp_id;
    FIX::TargetCompID target_comp_id;

    exec.getHeader().getIfSet(begin_str);
    exec.getHeader().getIfSet(sender_comp_id);
    exec.getHeader().getIfSet(target_comp_id);

    ack.session_id= FIX::SessionID(begin_str.getString(),target_comp_id.getString(),sender_comp_id.getString());

    //acknowledge.ShortName = short_name;
    ack.execReport = exec;
    //---
    ack.OrderID =ack.getValue<std::string>(FIX::OrderID(),"0");
    //---
    ack.execReport.set(FIX::ExecType(FIX::ExecType_ORDER_STATUS));
    ack.execReport.set(FIX::OrdStatus(FIX::OrdStatus_DONE_FOR_DAY));
    //---
    return(true);
    }
  catch(const std::exception &e)
    {
    LOG_ERROR(pluginLog,"error in ack [%s]",e.what());
    }
  //---
  return(false);
  }
//+------------------------------------------------------------------+
//| Get value                                                        |
//+------------------------------------------------------------------+
template<typename T,class F>
inline T Ack::getValue(F &f,T defaultValue)
  {
  if(execReport.isSet(f))
    return execReport.get(f).getValue();
  //---
  return defaultValue;
  }
//+------------------------------------------------------------------+
//| Get                                                              |
//+------------------------------------------------------------------+
template<typename T>
inline T Ack::Get(T &f)
  {
  if(execReport.isSet(f))
    return execReport.get(f);
  //---
  return f;
  }
//+------------------------------------------------------------------+
//| Set                                                              |
//+------------------------------------------------------------------+
template<typename T>
inline void Ack::Set(FIX43::ExecutionReport &newER,T &field)
  {
  if(execReport.isSet(field))
    {
    newER.set(Get(field));
    }
  }
//+------------------------------------------------------------------+
//| As ER?                                                           |
//+------------------------------------------------------------------+
inline const FIX43::ExecutionReport Ack::AsER()
  {
  FIX43::ExecutionReport ER; 
  //---
  if(execReport.getHeader().isSet(FIX::TargetSubID()))
    ER.getHeader().set(FIX::SenderSubID(execReport.getHeader().get(FIX::TargetSubID()).getValue()));
  //---
  if(execReport.getHeader().isSet(FIX::OnBehalfOfCompID()))
    ER.getHeader().set(FIX::DeliverToCompID(execReport.getHeader().get(FIX::OnBehalfOfCompID()).getValue()));
  //---
  Set(ER,FIX::Account());
  Set(ER,FIX::AvgPx());
  Set(ER,FIX::ClOrdID());
  Set(ER,FIX::CumQty());
  Set(ER,FIX::Currency());
  Set(ER,FIX::ExecID());
  Set(ER,FIX::ExecInst());
  Set(ER,FIX::ExecType());
  Set(ER,FIX::ExpireTime());
  Set(ER,FIX::LastPx());
  Set(ER,FIX::LastQty());
  Set(ER,FIX::LeavesQty());
  Set(ER,FIX::MinQty());
  Set(ER,FIX::OrderID());
  Set(ER,FIX::OrderQty());
  Set(ER,FIX::OrdStatus());
  Set(ER,FIX::OrdType());
  Set(ER,FIX::PegDifference()); //PegOffsetValue
  Set(ER,FIX::Price());
  Set(ER,FIX::Product());
  Set(ER,FIX::SecurityType());
  Set(ER,FIX::SettlCurrAmt());
  Set(ER,FIX::SettlCurrency());
  Set(ER,FIX::FutSettDate()); //SettlDate
  Set(ER,FIX::Side());
  Set(ER,FIX::Symbol());
  Set(ER,FIX::TimeInForce());
  Set(ER,FIX::TradeDate());
  Set(ER,FIX::TransactTime());
  //---
  return ER;
  }
//+------------------------------------------------------------------+
