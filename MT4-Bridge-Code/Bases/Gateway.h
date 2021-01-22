//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
//---
#include "..\Bases\Trades\TradeDispatcher.h"
//#include "..\Bases\Quotes\QuoteDispatcher.h"
//---
#include "..\QuickFix\SessionID.h"
#include "..\include\MT4ServerAPI.h"
#include "..\Common\PluginContext.h"
#include "Common\SymbolPosition.h"
#include "..\Risk\CloudWatch.h"
#include "..\Tools\smart_ptr.h"
#include "..\Tools\StringFile.h"
#include "..\IntegralAPI\IntegralAPI.h"
//#include "..\IntegralAPI\Contexts\Dispatchers\SettingsDispatcher.h"
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
#define             PLUGIN_DEALER    (3333333)
//+------------------------------------------------------------------+
//| State of connection to Gateway API                               |
//+------------------------------------------------------------------+
enum EnGatewayAPIStatus
  {
  STATUS_API_STOPPED=0x00,                  // not configured yet
  STATUS_API_CONFIGURED=0x01,                  // Gateway settings received
  STATUS_API_FIRST=STATUS_API_STOPPED,
  STATUS_API_LAST=STATUS_API_CONFIGURED
  };
//+------------------------------------------------------------------+
//| State of connection to external trading system                   |
//+------------------------------------------------------------------+
enum EnConnectStatus
  {
  STATUS_DISCONNECTED=0x00,                     // no connection
  STATUS_CONNECTING=0x01,                       // connecting
  STATUS_CONNECTED=0x02,                        // connected
  STATUS_SYNCHRONIZED=0x04,                     // gateway synchronized with external trading system
  STATUS_FIRST=STATUS_DISCONNECTED,
  STATUS_LAST=STATUS_SYNCHRONIZED
  };
//+------------------------------------------------------------------+
//| Class of gateway                                                 |
//+------------------------------------------------------------------+
class CGateway
  {
  private:
    struct PriceVolumePair
      {
      double volume;
      double price;
      };

  private:
    typedef TMTArray<char[12],32> SymbolNamesArray;
    //typedef std::map<double, double> PriceVolumeMap;

  private:
    //--- constants
    enum EnConstants
      {
      RECONNECT_SHORT_TIMEOUT=3,            // reconnect short period, sec
      RECONNECT_LONG_TIMEOUT=10,           // reconnect big period, sec
      RECONNECT_LONG_COUNT=5,            // count of attempts to change period on long
      };
    //--- server settings
    PluginCfg *m_api_server_config;
    //--- external trading system API
    CIntegralAPI *m_api_integral;
    //--- trade dispatcher
    CTradeDispatcher     m_trade_dispatcher;
    //--- 
    //FIX::SessionSettings m_settings;
    //--- reconnect time/attempts
    INT64                m_connect_time;
    UINT                 m_connect_attempts;
    //--- state
    volatile UINT        m_status_api_server;         // Server API connection state
    volatile LONG        m_status_api_integral;       // integral trading system API connection state
    //--- databases
    CMTSync              m_sync;
    //--- file operations
    CStringFile          m_file;
    //--- data buffer
    char                 m_buffer[1024];

  public:
    //--- constructor/destructor
    CGateway();
    ~CGateway();
    //--- initialization/shutdown
    bool              Initialize(void);
    void              Shutdown();
    //--- check gateway state
    void              Check(const DWORD curtime);
    void              Scheduler(const time_t curtime);
    //---
    //--- methods called by Server API
    //---
    //--- dealing
    bool              OnTradeRequestApply(TradeRequest *record);
    bool              OnTradeRequestApply(RequestInfo *request,const UINT action);
    //--- end of day
    void              OnEODFinish(const GroupState *groups,const int groups_total,const DailyReport *reports,const int reports_total);
    //--- group/symbol config
    bool              UpdateGroupCache(const ConGroup *group);
    bool              UpdateSymbolCache(const ConSymbol *cs);
    //--- notify of execution meesage
    //bool              OnExecutionReport(LPCSTR unique_id,LPCSTR short_name);
    void              SendAcknowledgement(LPCSTR unique_id);
    bool              SendDontKnowTrade(LPCSTR unique_id,UINT reason,LPCSTR text);
    //--- notify of manager protocol
    int               OnManagerProtocol(const ULONG ip,const UserInfo *us,const unsigned char *in_data,const int in_size,unsigned char **out_data,int *out_size);
    //---- notify of log messages
    void              OnLog(const LogInfo *info);
    //--- send tick to Server API
    bool              HistoryAddBook(const IntegralMsgBook &integral_book,const Instrument *symbol);
    //bool              HistoryAddTick(SymbolTick &integral_tick,const Instrument &symbol);
    //--- order update
    bool              OrdersUpdate(TradeRecord *order,UserInfo *inf,const int mode);
    //---order close
    bool              OrdersClose(const int order,const int volume,const double close_price,LPCSTR comment,LPCSTR order_id);
    bool              OrdersUpdateClose(const int order,const int volume,const double close_price,LPCSTR order_id);
    int               OrdersOpen(const int login,const int cmd,LPCSTR symbol,const double open_price,const int volume,LPCSTR order_id,const double stop_loss,const double take_profit);
    int               OrdersAddOpen(const TradeRecord *trade,UserInfo *user,const int volume);
    int               OrdersUpdateActivate(const int order,const int volume,const double open_price,LPCSTR order_id);
    bool              AddComment(LPCSTR apPrefix,LPCSTR side,LPCSTR apExecID,StringType &comment);
    //---
    int               StopoutsFilter(const ConGroup *group,const ConSymbol *symbol,const int login,const double equity,const double margin);
    //--- checks
    bool              CheckGroupAndSymbol(const int login,LPCSTR symbol,LPCSTR group,bool add_logs=true);
    bool              CheckGroupAndSymbol(const int login,LPCSTR symbol);
    int               CheckHedgeAllowed(LPCSTR group,LPCSTR symbol,const int login,const short cmd);
    bool              CheckCloseOnly(const int ticket);
    bool              CheckCloseOnly(const int ticket,const ConSymbol *symbol);
    bool              CheckCloseBy(const int order,const int orderby);
    bool              CheckAlreadyClosed(const int order);
    bool              CheckMultipleCloseBy(const int login,LPCSTR symbol);

    int               CheckExecutionOnly(const int login,LPCSTR symbol);
    int               CheckExecutionOnly(const int login,LPCSTR symbol_name,UserRecord &user_record,ConSymbol &symbol);
    //--- requests
    bool              CheckRequest(const INT request_id);
    bool              CheckRequest(const INT login,LPCSTR symbol);
    bool              CheckRequestByOrder(const INT order);
    bool              CheckRequestByOrder(const INT order,const INT order_by);
    bool              CheckPendingRequests(const int login);
    bool              RemoveRequest(const INT request_id);
    bool              RemoveRequest(const INT request_id,TradeRequest &record);
    bool              RemoveRequestByOrder(const INT order,TradeRequest &record);
    bool              GetRequestByOrder(const INT order,TradeRequest &record);
    //--- admin integration
    bool              AdminIntegration(const UserRecord *info);
    //--- get/set server API state
    UINT              StatusGateway(const UINT status);
    UINT              StatusGateway();
    //--- get/set integral connection state
    LONG              StatusExchange();
    LONG              StatusExchange(const LONG status);
    //---
    //--- methods of notification from external trading system
    //---
    //--- notify of session loaded
    void              OnSessionLoaded(const FIX::SessionSettings &settings);
    //--- notify of logon
    void              OnLogon(PCSTR short_name);
    //--- notify of synchronization
    void              OnIntergalSyncComplete();
    //--- notify of closed connection
    void              OnIntergalConnectClosed();
    bool              OnExecutionReportTrans(const FIX43::ExecutionReport &report,const Instrument &symbol);
    //---
    bool              PendingsFilter(const ConGroup *group,const ConSymbol *symbol,const TradeRecord *trade,bool hedge_prohibited);
    //--- UI events
    std::string       GetConfigs(void);
    bool              SetGroup(std::string &group,std::string &config,const bool save_file,const bool force_update);
    bool              SetRule(const int index,StringType &config,const bool save_file,const bool force_update);
    bool              SetInstrument(std::string &symbol,std::string &config,const bool save_file,const bool force_update);

  public:
    bool              OnOrderOpen(const TradeRequest &record,StringType &message);
    bool              OnOrderClose(const TradeRequest &record,StringType &message);
    bool              ProcessMarketOpen(const TradeRequest &record,StringType &message);
    bool              ProcessMarketClose(const TradeRequest &record,StringType &message);
    bool              ProcessPendingOrderActivation(const TradeRequest &record,StringType &message);

  private:
    //--- connect/disconnect gateway
    bool                Connect();
    //void                Disconnect();
    //--- prepare data
    bool                ProcessBook(const Instrument *record,const IntegralMsgBook &integral_book,FeedTick &data);
    //static bool         ProcessTick(const Instrument *record,SymbolTick &integral_tick,FeedTick &data);
    //--- calculation
    bool                OrderIsBuy(const short cmd) const;
    double              CalcMarginAmount(const ConSymbol *symbol,const SymbolPosition *position,const double market_price,const double leverage,const double percentage) const;
    //--- rounding
    static double       RoundTo(double x,double prec);

  private:
    static double     ProcessPrice(const IntegralBook &book,const UINT type,double &total_amout,const Instrument *inst);
  };

extern CGateway *ExtGateway;
//+------------------------------------------------------------------+