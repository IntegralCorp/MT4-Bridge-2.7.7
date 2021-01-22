//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include "stdafx.h"
#include "PluginContext.h"
//+------------------------------------------------------------------+
//|  Global object                                                   |
//+------------------------------------------------------------------+
CPluginContext ExtPluginContext;
//+------------------------------------------------------------------+
//|  Constructor                                                     |
//+------------------------------------------------------------------+
CPluginContext::CPluginContext()
  : manager_ui(0),
  //m_api_server(NULL),
  max_quote_latency(2000),
  //enable_async_orders(true),
  trading_dealer(1),
  //enable_unifeeder(true),
  log_journal(true),
  send_group_tag(true),
  comment_style(COMMENT_NONE)
  {
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CPluginContext::~CPluginContext()
  {
  //---
  Shutdown();
  }
//+------------------------------------------------------------------+
//|  clear all data                                                  |
//+------------------------------------------------------------------+
void CPluginContext::Shutdown()
  {
  //---
  Save();
  //---
  manager_ui       =0;
  max_quote_latency=0;
  comment_style    =COMMENT_NONE;
  //---
  //Shutdown();
  }
//+------------------------------------------------------------------+
//| Read                                                             |
//+------------------------------------------------------------------+
bool CPluginContext::Initialize(void)
  {
  //--- checks
  //if(!api_server)
  //  return(false);
  //--- remeber
  //m_api_server=api_server;
  Load();
  //--- successfull
  return(true);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CPluginContext::CheckManagerSTP(int manager)
  {
  StringType64 manager_str;
  bool res=false;
  //--- checks
  if(manager<1)
    return(true);
  //---
  manager_str.Format("%d",manager);
  //--- under lock
  m_sync.Lock();
  //---
  res=StringType::CheckGroupMask(stp_managers,manager_str.Str());
  //---
  m_sync.Unlock();
  return(res);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CPluginContext::Reload()
  {
  PluginCfg      cfg={0};
  UserRecord     ur ={0};
  StringType64   comment;
  StringType1024 managers_str;
  //---
  //enable_async_orders=true;
  managers_str.Assign("*");
  //---
  GetBool(1,"Admin Integration",&admin_integration,"0");
  //---
  GetString(2,"Comments",comment.Buffer(),comment.Max(),"NONE");
  comment.Refresh();
  //---
  GetInteger(3,"Dealer Account",&trading_dealer,"1");
  GetInteger(4,"Drop Price Latency",&max_quote_latency,"2000");
  //--- dont allow negative values
  if(max_quote_latency<0)
    max_quote_latency=0;
  //---
  GetBool(5,"Enable DEBUG Logs",&enable_debug_logs,"0");
  //---
  GetBool(6,"EOD Reports",&eod_report,"0");
  //----
  Delete("Enable UniFeeder");
  //GetBool(5,"Enable UniFeeder",&enable_unifeeder,"1");
  GetBool(7,"Log Journal",&log_journal,"1");
  GetBool(8,"Save FIX Prices",&save_price_updates,"0");
  //---
  GetBool(9,"Send Group Tag",&send_group_tag,"1");
  //---
  GetString(10,"STP Managers",managers_str.Buffer(),managers_str.Max(),"*");
  managers_str.Refresh();
  StringType::Copy(stp_managers,managers_str.Str());

  GetInteger(11,"UI Manager",&manager_ui,"0");
  //GetBool(11,"DealerConfirm Add to DB",&dealer_add_order_to_db,"0");
  Delete("DealerConfirm Add to DB");
  Delete("Async");
  Delete("Reports");
  Delete("Delete Failed Pendings");
  //GetBool(13,"SeqOrder per User",      &en_per_user_sequential_order,"1");
  //GetBool(14,"Global SeqOrder",        &en_sequential_order,"0");
  //GetBool(15,"SeqStopLTakeP",          &en_stopl_takep_seq,"0");
  //---
  Delete("SeqOrder per User");
  Delete("Global SeqOrder");
  Delete("SeqStopLTakeP");
  //---
  //GetBool(12,"Test",                   &test,"0");
  Delete("Test");
  //---
  if(comment.CompareNoCase("BEFORE")==0)
    comment_style =COMMENT_STYLE::COMMENT_BEFORE;
  else if(comment.CompareNoCase("AFTER")==0)
    comment_style = COMMENT_STYLE::COMMENT_AFTER;
  else
    comment_style = COMMENT_STYLE::COMMENT_NONE;
  }
//+------------------------------------------------------------------+
