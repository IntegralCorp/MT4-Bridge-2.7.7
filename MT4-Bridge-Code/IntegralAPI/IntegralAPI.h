//+------------------------------------------------------------------+
//|                                      MetaTrader 5 Sample Gateway |
//|                   Copyright 2000-2019, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//---
#include "Common\IntegralProtocol.h"
#include "Contexts\IntegralContext.h"
#include "..\Bases\Trades\TradeDispatcher.h"
//#include "..\Bases\Quotes\QuoteDispatcher.h"
#include "Bases\SettingsDispatcher.h"
//+------------------------------------------------------------------+
//| Preliminary declarations                                         |
//+------------------------------------------------------------------+
class CGateway;
//+------------------------------------------------------------------+
//| Class of external trading system API                             |
//+------------------------------------------------------------------+
class CIntegralAPI
  {
  private:
    //--- constants
    enum EnConstants
      {
      TRANS_WAIT_TIME=50      // transactions waiting time
      };
    //--- Gateway API interface
    CGateway            *m_gateway;
    //---- bases
    CSettingsDispatcher  m_settings_dispatcher;
    //--- bases
    //---
    CIntegralSessionBase m_sessions;
    //--- context of connection to integral trading system
    CIntegralContext     m_integral_context;
    //--- thread of data processing
    CMTThread            m_thread;
    //--- thread's working flag
    volatile long        m_thread_workflag;

  public:
    //--- constructor/destructor
                      CIntegralAPI(CGateway *gateway);
                     ~CIntegralAPI(void);
    //--- initialization/shutdown
    bool              Initialize(void);
    void              Shutdown();
    //--- methods
    bool              CheckGroupAndSymbol(const int login,LPCSTR symbol,LPCSTR group,bool add_logs=true);
    //--- update of session loaded
    void              OnSessionLoaded(const FIX::SessionSettings &settings);
    //--- update state of connection context
    void              OnContextStateUpdate(const UINT state);
    //--- execution report received
    //bool              OnExecutionReport(const IntegralExecution &execution);
    //--- received messages from Integral
    bool              OnCreate(const FIX::SessionID &session_id,LPCSTR short_name,const UINT session_type);       // create
    //---
    bool              OnLogon(const FIX::SessionID &session_id,const UINT &session_type,LPCSTR short_name);      // logon
    void              OnLogout(const FIX::SessionID &session_id,const UINT &session_type,LPCSTR short_name);       // logout
    //--- admin integration
    bool              AdminIntegration(const UserRecord *info);
    //--- check state of Gateway API
    bool              Check();
    //--- send order
    bool              SendOrder(IntegralOrder &order);
    //--- send acknowledgement
    bool              SendAck(LPCSTR unique_id);
    //--- send Dont know trade
    bool              SendDontKnowTrade(LPCSTR order_str,const UINT reason,LPCSTR text);
    //--- send order status request
    bool              SendOrderStatusRequest(LPCSTR order_str);
    //--- remove order
    //bool              RemoveOrder(LPCSTR unique_id);
    //--- find symbol by ccy pair
    bool              SymbolGet(LPCSTR ccy_pair,Instrument &symbol);
    bool              SymbolGetByName(LPCSTR symbol_name,Instrument &symbol);
    //--- get sessions
    void              GetSessions(SessionIdentifiers &sessions,const UINT session_state);
    void              GetSessions(SessionIdentifiers &sessions);
    //--- group/symbol config
    bool              UpdateGroupCache(const ConGroup *group);
    bool              UpdateSymbolCache(const ConSymbol *cs);
    //--- UI data
    std::string       GetConfigs(void);
    bool              SetGroup(std::string &group,std::string &config,const bool save_file,const bool force_update);
    bool              SetRule(const int index,StringType &config,const bool save_file,const bool force_update);
    bool              SetInstrument(std::string &symbol,std::string &config,const bool save_file,const bool force_update);

  private:
    //--- thread of data processing
    static UINT __stdcall ProcessThreadWrapper(LPVOID param);
    void              ProcessThread();
    //--- process
    bool              OnQuoteSession(CIntegralSessionRecord &record,LPCSTR short_name);
    bool              OnOrderSession(CIntegralSessionRecord &record,LPCSTR short_name);
  };
//+------------------------------------------------------------------+