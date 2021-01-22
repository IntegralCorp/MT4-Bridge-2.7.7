//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include "..\API\MT4APITools.h"
#include "..\Tools\StringFile.h"
#include "..\IO\Log.h"
#include "Configuration.h"
//+------------------------------------------------------------------+
//| Array of strings                                                 |
//+------------------------------------------------------------------+
typedef TMTArray<char[1024],16> String1024Array;
typedef TMTArray<char[256],16>  String256Array;
//+------------------------------------------------------------------+
//| Configuration file wrapper                                       |
//+------------------------------------------------------------------+
class CPluginContext  : public CConfiguration
  {
  public:
    //---
    enum COMMENT_STYLE
      {
      COMMENT_NONE=0,
      COMMENT_BEFORE,
      COMMENT_AFTER
      };

  private:
    enum EnSettings
      {
      SETTING_UI      =0,
      SETTING_LATENCY =1,
      SETTING_ADMIN   =2,
      SETTING_UPDATES =3,
      SETTING_LOGS    =4,
      SETTING_PENDINGS=5,
      SETTING_GROUP   =6,
      SETTING_EOD     =11,
      SETTING_JOURNAL =12,
      SETTING_COMMENT =13,
      SETTING_MANAGERS=14
      };
    //--- params
    bool              admin_integration;                     // admin integration
    //bool              enable_async_orders;                   //
    UINT              comment_style;
    INT               trading_dealer;
    //bool              delete_failed_pendings;                // delete failed pendings
    INT               max_quote_latency;                     // max quote latency
    bool              enable_debug_logs;                     // enable debug logs
    //bool              enable_unifeeder;                      // enable unifeeder
    bool              log_journal;
    bool              save_price_updates;
    char              stp_managers[1024];
    bool              eod_report;
    int               manager_ui;                            // manager ui
    //bool              dealer_add_order_to_db;
    bool              send_group_tag;                        // send group tag
    //bool              en_per_user_sequential_order;           
    //bool              en_sequential_order;           
    //bool              en_stopl_takep_seq;  
    //bool              test;
    //--- Server API
    //CServerInterface *m_api_server;

  public:
    bool   AdminIntegration()    { m_sync.Lock(); bool tmp=admin_integration; m_sync.Unlock(); return(tmp);  }
    //bool   EnableAsyncOrders()   { m_sync.Lock(); bool tmp=enable_async_orders; m_sync.Unlock(); return(tmp); }     
    UINT   CommentStyle()        { m_sync.Lock(); UINT tmp=comment_style; m_sync.Unlock(); return(tmp); }
    INT    TradingDealer()       { m_sync.Lock(); INT  tmp=trading_dealer; m_sync.Unlock(); return(tmp); }
    //bool   DeleteFailedPendings(){ m_sync.Lock(); bool tmp=delete_failed_pendings; m_sync.Unlock(); return(tmp); }  
    INT    MaxQuoteLatency()     { m_sync.Lock(); INT  tmp=max_quote_latency; m_sync.Unlock(); return(tmp); }
    bool   EnableDebugLogs()     { m_sync.Lock(); bool tmp=enable_debug_logs; m_sync.Unlock(); return(tmp); }
    //bool   EnableUnifeeder()     { m_sync.Lock(); bool tmp=enable_unifeeder; m_sync.Unlock(); return(tmp); }
    bool   LogJournal()          { m_sync.Lock(); bool tmp=log_journal; m_sync.Unlock(); return(tmp); }
    bool   SavePriceUpdates()    { m_sync.Lock(); bool tmp=save_price_updates; m_sync.Unlock(); return(tmp); }
    LPCSTR STP()                 { m_sync.Lock(); StringType1024 tmp; tmp.Assign(stp_managers); m_sync.Unlock(); return(tmp.Str()); }
    bool   EODReports()          { m_sync.Lock(); bool tmp=eod_report; m_sync.Unlock(); return(tmp); }
    INT    ManagerUI()           { m_sync.Lock(); INT  tmp=manager_ui; m_sync.Unlock(); return(tmp); }
    //bool   DealerAddOrderToDB()  { m_sync.Lock(); bool tmp=dealer_add_order_to_db; m_sync.Unlock(); return(tmp); }
    bool   SendGroupTag()        { m_sync.Lock(); bool tmp=send_group_tag; m_sync.Unlock(); return(tmp); }
    //bool   EnPerUserSequentialOrder() { m_sync.Lock(); bool tmp=en_per_user_sequential_order; m_sync.Unlock(); return(tmp); }
    //bool   EnSequentialOrder()   { m_sync.Lock(); bool tmp=en_sequential_order; m_sync.Unlock(); return(tmp); }
    //bool   EnStoplTakepSeq()     { m_sync.Lock(); bool tmp=en_stopl_takep_seq; m_sync.Unlock(); return(tmp); }
    //bool   Test()                { m_sync.Lock(); bool tmp=test; m_sync.Unlock(); return(tmp); }
    bool   CheckManagerSTP(int manager);
    //---
              CPluginContext(void);
     virtual ~CPluginContext(void);
    //---
    virtual void      Reload();
    //--- clear all data
    void              Shutdown();
    //--- read/writes
    bool   Initialize(/*CServerInterface *api_server*/);
  };
extern CPluginContext ExtPluginContext;
//+------------------------------------------------------------------+
