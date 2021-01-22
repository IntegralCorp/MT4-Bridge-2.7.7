//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include "..\..\QuickFix\MessageCracker.h"
#include "..\..\QuickFix\fix43\Message.h"
#include "..\..\QuickFix\fix43\NewOrderSingle.h"
#include "..\..\QuickFix\fix43\Logon.h"
#include "..\..\QuickFix\fix43\Logout.h"
#include "..\..\QuickFix\fix43\ExecutionReport.h"
#include "..\..\QuickFix\fix43\OrderStatusRequest.h"
#include "..\..\QuickFix\fix43\DontKnowTrade.h"
#include "..\..\QuickFix\fix43\MarketDataRequest.h"
#include "..\..\QuickFix\fix43\MarketDataSnapshotFullRefresh.h"
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
namespace FIX
  {
  USER_DEFINE_PRICE(UserDefinedSpread,7547);
  USER_DEFINE_STRING(OrderNotes,7549);
  //USER_DEFINE_BOOLEAN(BBookTradeInd, 7550);
  USER_DEFINE_STRING(AdditionalExecInst,7554);
  USER_DEFINE_STRING(BridgeVersion,7570);
  USER_DEFINE_STRING(OrderChannel,7571);
  USER_DEFINE_STRING(AccountId,7573);
  USER_DEFINE_INT(DKReasonCode,7574);
  USER_DEFINE_STRING(AccountGroupId,7575);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
struct IntegalMessageRecord
  {
  char           short_name[256];
  FIX::SessionID fix_session_id;
  FIX::Message   fix_message;
  //--- fill
  static void Fill(const FIX::Message &fix_message,/*LPCSTR short_name,*/const FIX::SessionID &session_id,IntegalMessageRecord &message);
  };
//+------------------------------------------------------------------+
//| Fill structure                                                   |
//+------------------------------------------------------------------+
inline void IntegalMessageRecord::Fill(const FIX::Message &source_message,/*LPCSTR short_name,*/const FIX::SessionID &session_id,IntegalMessageRecord &dest_message)
  {
  dest_message.fix_message   =source_message;
  dest_message.fix_session_id=session_id;
  //StringType::Copy(dest_message.short_name,short_name);
  }
//+------------------------------------------------------------------+
//| Interface of connection context                                  |
//+------------------------------------------------------------------+
class CIntegralContextBase:public FIX::Application,public FIX::MessageCracker
  {
  public:
    //--- constructor/destructor
                      CIntegralContextBase() { }
    virtual          ~CIntegralContextBase() { }
    //--- send data
    virtual bool Send(FIX::Message &message,FIX::SessionID &session_id)=0;
    virtual bool Send(FIX::Message &message)                           =0;
    //virtual bool Send(IntegalMessageRecord &message)                   =0;
    virtual bool SendAsync(const IntegalMessageRecord &message)        =0;
    //---
    virtual void onCreate(const FIX::SessionID &s)=0;
    virtual void onLogon(const FIX::SessionID &s)=0;
    virtual void onLogout(const FIX::SessionID &s)=0;
    virtual void toAdmin(FIX::Message &m,const FIX::SessionID &s)=0;
    virtual void toApp(FIX::Message &m,const FIX::SessionID &s) throw(FIX::DoNotSend)=0;
    virtual void fromAdmin(const FIX::Message &m,const FIX::SessionID &s) throw(FIX::FieldNotFound,FIX::IncorrectDataFormat,FIX::IncorrectTagValue,FIX::RejectLogon)=0;
    virtual void fromApp(const FIX::Message &m,const FIX::SessionID &s)	throw(FIX::FieldNotFound,FIX::IncorrectDataFormat,FIX::IncorrectTagValue,FIX::UnsupportedMessageType)=0;
    //---
    virtual void onMessage(const FIX43::Reject &m,const FIX::SessionID &s)=0;
    virtual void onMessage(const FIX43::BusinessMessageReject &m,const FIX::SessionID &s)=0;
    virtual void onMessage(const FIX43::MarketDataRequestReject &m,const FIX::SessionID &s)=0;
    virtual void onMessage(const FIX43::MarketDataSnapshotFullRefresh &m,const FIX::SessionID &s)=0;
    virtual void onMessage(const FIX43::TradeCaptureReport &m,const FIX::SessionID &s)=0;
    virtual void onMessage(const FIX43::ExecutionReport &m,const FIX::SessionID &s)=0;
    //--- process HeartBeat message
    virtual void onMessage(const FIX43::Heartbeat &m,const FIX::SessionID &s)=0;
  };
//+------------------------------------------------------------------+