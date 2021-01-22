//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include <iostream>
#include <fstream>
#include <thread>
#include <functional>
#include <list>
#include <atomic>
#include <map>
#include <algorithm>
#include <windows.h>
//---
#include "..\..\resource.h"
#include "..\..\Util\NetworkMon.h"
#include "..\..\IO\HTTP.h"
#include "..\Common\IntegralMsgOrder.h"
#include "..\..\Common\Helpers.h"
#include "..\..\Common\Const.h"
#include "..\..\version.h"
#include "Config\SessionSettings.h"
#include "..\..\IO\Log.h"
// quickfix
#include "..\..\QuickFix\FileStore.h"
#include "..\..\QuickFix\ThreadedSocketInitiator.h"
#include "..\..\QuickFix\SessionSettings.h"
#include "..\..\QuickFix\Log.h"
#include "..\..\QuickFix\Application.h"
#include "..\..\QuickFix\MessageCracker.h"
#include "..\..\QuickFix\Values.h"
#include "..\..\QuickFix\Mutex.h"
#include "..\..\QuickFix\fix43\Logon.h"
#include "..\..\QuickFix\fix43\NewOrderSingle.h"
#include "..\..\QuickFix\fix43\ExecutionReport.h"
#include "..\..\QuickFix\fix43\MarketDataRequest.h"
#include "..\..\QuickFix\fix43\BusinessMessageReject.h"
#include "..\..\QuickFix\fix43\MarketDataRequestReject.h"
#include "..\..\QuickFix\fix43\MarketDataSnapshotFullRefresh.h"
#include "..\..\QuickFix\fix43\ExecutionReport.h"
#include "..\..\QuickFix\fix43\Reject.h"
#include "..\..\QuickFix\fix43\DontKnowTrade.h"
#include "..\..\QuickFix\fix43\OrderStatusRequest.h"
#include "..\..\QuickFix\fix43\TradeCaptureReport.h"
#include "..\..\QuickFix\fix43\Heartbeat.h"
//--- bases
#include "..\..\Bases\Trades\TradeRequest.h"
//#include "..\..\Bases\Quotes\QuoteDispatcher.h"
#include "..\..\Bases\Trades\TradeDispatcher.h"
//---
#include "..\..\FIX\IdcLog.h"
//--- utils
#include "..\..\Util\AtomicUtil.h"
#include "..\..\Util\threadmap.h"
//---
#include "Dispatchers\IntegralSession.h"
#include "..\IntegralAPI.h"
//---
//#include "..\..\Risk\WatchTower.h"
#include "..\Bases\SettingsDispatcher.h"
#include "..\..\Common\PluginContext.h"
#include "Dispatchers\IntegralSessionBase.h"
#include "..\..\Risk\CloudWatch.h"
//+------------------------------------------------------------------+
//| Preliminary declarations                                         |
//+------------------------------------------------------------------+
class CIntegralAPI;
//+------------------------------------------------------------------+
//| Class of connection to external trading system context           |
//+------------------------------------------------------------------+
class CIntegralContext: public CIntegralContextBase
  {
  private:
    typedef TMTArray<IntegalMessageRecord,256> MessagesArray;
    typedef std::map<std::string,CLog *>        LogsMap;

  public:
    //--- context state
    enum EnExchangeContextState
      {
      STATE_STOPPED     =0x00,                     // stopped thread of processing connection messages
      STATE_STARTED     =0x01,                     // started thread of processing connection messages
      STATE_SYNCHRONIZED=0x02,                     // connection fully synchronized
      STATE_FIRST       =STATE_STOPPED,
      STATE_LAST        =STATE_SYNCHRONIZED
      };

  private:
    //--- context timeouts
    enum EnExchangeContextTimeouts
      {
      THREAD_TIMEOUT=20,                    // thread timout, sec.
      LOGON_TIME    =2000,                  // logout time, msec
      LOGON_WAIT    =50                    // logout message wait time, msec
      };
    //--- constants
    enum EnConstants
      {
      THREAD_SLEEP    =1000,                   // thread sleep time, msec
      HEARBEAT_TIMEOUT=15000                    // hearbeat timeout
      };
    //--- array of ticks
    //typedef TMTArray<IntegralBook> IntegralBooksArray;
    typedef std::vector<IntegralMsgBook> IntegralBooksArray;

  private:
    //--- interface of external trading system
    CIntegralAPI                 &m_integral_api;
    //--- quickfix initiator
    FIX::ThreadedSocketInitiator *m_initiator;
    //--- log factory
    FIX::IdcLogFactory           *m_log_factory;
    //--- quick fix settings
    FIX::SessionSettings          m_settings;
    //--- sessions
    //CIntegralSessionBase          m_sessions;
    CMTSync                       m_sync;                   // syncroonizer
    //---
    LogsMap                       m_logs;
    //SessionsMap                   m_sessions;
    //--- arrays of messages for sending
    MessagesArray                 m_message_send;
    MessagesArray                 m_message_send_tmp;
    //--- synchronizer of access to messages for sending
    CMTSync                       m_message_send_sync;
    //--- thread of messages processing
    CMTThread                     m_thread;
    //--- flag of processing thread
    volatile long                 m_thread_workflag;
    //--- time of last activity of processing thread
    volatile INT64                m_thread_checktime;
    //--- state of connection context
    volatile long                 m_state;
    //--- transaction of ticks
    //IntegralTicksArray            m_trans_ticks;
    //IntegralTicksArray            m_trans_ticks_tmp;
    //--- transactions of books
    IntegralBooksArray            m_trans_books;
    IntegralBooksArray            m_trans_books_tmp;
    //--- transaction of deals
    IntegralExecutionsArray       m_trans_executions;
    IntegralExecutionsArray       m_trans_executions_tmp;
    //---
    //CExecutionBase                m_executions;
    //--- synchronizer of access to transactions
    CMTSync                       m_trans_sync;
    //--- login state
    volatile long                 m_logon;
    //--- heartbeat time
    volatile INT64                m_heartbeat_time;

  public:
     CIntegralContext(CIntegralAPI &exchange_api);
    ~CIntegralContext();
    //--- initialization/shutdown
    bool              Initialize(void);
    void              Shutdown();
    //--- state of the connection context (for order and quote session)
    UINT              StateGet();
    UINT              StateSet(UINT state);
    //--- send message
    //virtual bool      Send(IntegalMessageRecord &message);
    virtual bool      Send(FIX::Message &message,FIX::SessionID &session_id);
    virtual bool      Send(FIX::Message &message);
    virtual bool      SendAsync(const IntegalMessageRecord &message);
    //--- update activity time
    //INT64             UpdateTime(const INT64 timeout) { return(m_thread_checktime=_time64(NULL)+timeout); }
    //--- check connection
    bool              Check(const FIX::SessionID &session_id,const INT64 curtime);
    bool              Check(const INT64 curtime);
    void              GetCurrentSessions(std::map<std::string,IntegralSession> &sessions);
    bool              GetOrderSession(LPCSTR short_name,IntegralSession &session);
    //--- process received transactions
    bool              TransApply(CSettingsDispatcher &records_base,bool &trans_applied);
    bool              TransApplyBooks(CSettingsDispatcher &records_base,IntegralBooksArray &trans_books,bool &trans_applied);
    //bool              TransApplyTicks(CSettingsDispatcher &records_base,IntegralTicksArray &trans_ticks,bool &trans_applied);
    bool              TransApplyDeals(CSettingsDispatcher &records_base,IntegralExecutionsArray &trans_deals,bool &trans_applied);
    //--- messages
    virtual void      onMessage(const FIX43::Reject &message,const FIX::SessionID &session);
    virtual void      onMessage(const FIX43::BusinessMessageReject &message,const FIX::SessionID &session);
    virtual void      onMessage(const FIX43::MarketDataRequestReject &message,const FIX::SessionID &session);
    virtual void      onMessage(const FIX43::MarketDataSnapshotFullRefresh &message,const FIX::SessionID &session);
    virtual void      onMessage(const FIX43::TradeCaptureReport &message,const FIX::SessionID &session);
    virtual void      onMessage(const FIX43::ExecutionReport &message,const FIX::SessionID &session);
    virtual void      onMessage(const FIX43::Heartbeat &message,const FIX::SessionID &session);
    //--- send trade order
    bool              SendOrder(IntegralOrder &order);
    //--- send trade acknowledgement
    bool              SendAck(LPCSTR unique_id);
    //--- send Dont know trade
    bool              SendDontKnowTrade(LPCSTR order_str,const UINT reason,LPCSTR text);
    //--- send/remove order status request
    bool              SendOrderStatusRequest(LPCSTR order_str);
    //bool              RemoveOrder(LPCSTR unique_id);
    //--- send dont know trade
    bool              SendDK(const IntegralExecution &exec) { return true; }
    //--- send HeartBeat
    bool              SendHeartBeat(const FIX::SessionID &session_id,const INT64 curtime);
    bool              SendHeartBeat(const INT64 curtime);
    //---
    void              SendLogout(const FIX::SessionID &session_id,LPCSTR message);
    void              SendLogon(const FIX::SessionID &session_id);
    //---
    bool              Subscribe(const Instrument *instrument,const CIntegralSessionRecord &record);
    //---
    bool              AdminIntegration(const UserRecord *info,const Group &group);
    void              UpdateSymbolCache(const ConSymbol *cs);

  private:
    bool              Start(void);

  private:
    //--- checks
    bool              CheckParameters(const FIX::SessionID &session_id,std::string &short_name,std::string &sender_comp_id,UINT &session_type,const bool send_logout=true);

  private:
    //--- notification methods
    virtual void      onCreate(const FIX::SessionID &session);
    virtual void      onLogon(const FIX::SessionID &session);
    virtual void      onLogout(const FIX::SessionID &session);
    virtual void      toAdmin(FIX::Message &message,const FIX::SessionID &session);
    virtual void      toApp(FIX::Message &message,const FIX::SessionID &session)	throw(FIX::DoNotSend);
    virtual void      fromAdmin(const FIX::Message &message,const FIX::SessionID &session) throw(FIX::FieldNotFound,FIX::IncorrectDataFormat,FIX::IncorrectTagValue,FIX::RejectLogon);
    virtual void      fromApp(const FIX::Message &message,const FIX::SessionID &session) throw(FIX::FieldNotFound,FIX::IncorrectDataFormat,FIX::IncorrectTagValue,FIX::UnsupportedMessageType);
    //--- thread of messages processing
    static UINT __stdcall ProcessThreadWrapper(LPVOID param);
    void              ProcessThread();
    //--- send/receive data
    bool              SendCheck(bool &data_processed);
    //--- check session status
    bool              SessionsCheck(bool &session_connected);
    //--- get/set logon-state
    //bool              IsLogon()            { return(InterlockedExchangeAdd(&m_logon,0)>0); }
    //bool              LogonSet(bool logon) { return(InterlockedExchange(&m_logon,logon?1:0)>0); }
    //--- sessions sorting/searching
    static int        SortSessionsById(const void *left,const void *right);
    static int        SearchSessionById(const void *left,const void *right);
    static int        SearchOrderSessionByShortName(const void *left,const void *right);
    static int        SearchQuoteSessionByShortName(const void *left,const void *right);
    static int        SearchSessionByShortName(const void *left,const void *right);
    static int        SearchShortNames(const void *left,const void *right);
  };
//+------------------------------------------------------------------+