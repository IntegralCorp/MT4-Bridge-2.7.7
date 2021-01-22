//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include <windows.h>
//---
#include "ExecutionBase.h"
#include "..\..\Common\Const.h"
#include "..\Common\SymbolPosition.h"
#include "..\..\IntegralAPI\Bases\Instrument.h"
#include "TradeRequestBase.h"
#include "..\..\MetaQuotes\MT4Helper.h"
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
#define MAX_REQUEST    1024
#define MAX_WAIT_TIME  120000
//+------------------------------------------------------------------+
//| Preliminary declarations                                         |
//+------------------------------------------------------------------+
class CIntegralAPI;
//+------------------------------------------------------------------+
//| Class of trade dispatcher                                        |
//+------------------------------------------------------------------+
class CTradeDispatcher
  {
  private:
    //--- requests
    TMTArray<TradeRequest> m_requests;
    //--- data type
    typedef threadmap<std::string,std::pair<double,double>> SpreadsMap;
    //--- constants
    enum constants
      {
      DEALER_ORDERS_TIMEOUT=SECONDS_IN_DAY,
      };
    //--- context timeouts
    enum EnExchangeContextTimeouts
      {
      THREAD_TIMEOUT=30000,                    // thread timout, sec.
      REQUEST_WAIT=5,                        // request not replied, sec.
      LOGON_TIME=2000,                     // logout time, msec
      LOGON_WAIT=50                        // logout message wait time, msec
      };

  private:
    //--- enumerate order flags
    enum EnTradeDispatcherConstants
      {
      MT_ORDER_ACTIVATION_FLAG=0x01,            // order activated on MT side
      MT_ORDER_ACTIVATION_TIMEOUT=5                // activation order expiration time
      };
    //--- constants
    enum EnConstants
      {
      CHECK_TIMEOUT=2,                         // trade timout, sec.
      THREAD_SLEEP=100,                       // thread sleep time, msec
      HEARBEAT_TIMEOUT=10                         // hearbeat timeout
      };

  private:
    //--- internal trading system interface
    CIntegralAPI              *m_api_integral;
    //---- spreads
    SpreadsMap                 m_spreads;
    CMTSync                    m_sync;                   // synchonizer
    //--- time of last activity of requset processing
    volatile INT64             m_request_checktime;

  public:
    //--- constructor/destructor
     CTradeDispatcher();
    ~CTradeDispatcher();
    //--- initialization/shutdown
    bool              Initialize(CIntegralAPI *api_integral);
    void              Shutdown();
    //--- update activity time
    INT64             UpdateTime(const INT64 timeout) { return(m_request_checktime=_time64(NULL)+timeout); }
    //---
    //--- methods called by Server API
    //---
    void              RequestsWrite();
    bool              RequestsRead();
    //--- requests
    bool              CheckRequest(const INT request_id);
    bool              CheckRequest(const INT login,LPCSTR symbol);
    bool              CheckRequestByOrder(const INT order);
    bool              CheckRequestByOrder(const INT order,const INT order_by);
    bool              CheckPendingRequests(const int login);
    bool              RemoveRequest(const INT request_id);
    bool              RemoveRequest(const INT request_id,TradeRequest &record);
    bool              RemoveRequestByOrder(const INT order,TradeRequest &record);
    bool              GetRequest(LPCSTR unique_id,TradeRequest &record);
    bool              GetRequestByOrder(const INT order,TradeRequest &record);
    bool              GetRequest(const INT request_id,TradeRequest &record);
    void              GetRequest(const INT delay,TradeRequestArray &records);
    //--- process selection of request
    bool              OnRequestLocked(TradeRequest *record);
    bool              OnRequestLocked(RequestInfo *request,const UINT action);
    //--- process confirmation of request
    //void              OnGatewayDealerAnswer(const MTAPIRES retcode,const IMTConfirm *confirm) const;
    //---
    //--- methods of notification from external trading system
    //---
    //--- notify of order transaction in external trading system
    //bool              OnExchangeOrderTrans(const ExchangeOrder &exchange_order,const ExchangeSymbol &symbol);
    //--- notify of spread
    void              OnSpreadReceived(LPCSTR symbol,const double prices[2]);
    //--- notify of deal transaction in external trading system
    bool              OnExecutionReportTrans(const FIX43::ExecutionReport &report,const Instrument &symbol);
    //--- notify orders logon
    void              OnLogon(PCSTR short_name);
    //---
    void              Check(const time_t curtime);
    //--- orders
    bool              OrdersClose(const int order,const int volume,const double close_price,LPCSTR comment,LPCSTR order_id);
    int               OrdersAddOpen(const int login,const int cmd,LPCTSTR symbol,const double open_price,const int volume,LPCSTR order_id,const double stop_loss=0,const double take_profit=0);
    int               OrdersAddOpen(const TradeRecord *trade,UserInfo *user,const int volume);
    int               OrdersAddOpenPending(const int login,const int cmd,LPCTSTR symbol,const double open_price,const int volume,const time_t expiration);
    int               OrdersAddOpenPending(const TradeRecord *trade,const int volume,LPCSTR order_id);
    bool              OrdersUpdateActivate(const int order,const int volume,const double open_price,LPCSTR order_id);
    bool              OrdersUpdateClose(const int order,const int volume,const double close_price,LPCSTR order_id);
    int               OrdersUpdateCancel(const int order);
    //--- checks
    bool              CheckCloseBy(const int order,const int orderby);
    bool              CheckMultipleCloseBy(const int login,LPCSTR symbol);
    //--- control related
    void              Scheduler(const time_t curtime);
    //--- prepare user info
    int               UserInfoGet(const int login,UserInfo *info);
    //--- prepare comment
    bool              AddComment(LPCSTR current_comment,LPCSTR apPrefix,LPCSTR apExecID,StringType &comment);

  private:
    //--- process trade requests
    bool              GatewayProcess(TradeRequest *record);
    //--- check if request can be executed automatically
    bool              GatewayProcessAuto(const RequestInfo *request);
    //--- execute request at integral
    bool              GatewayExecuteIntegral(TradeRequest *record);
    bool              GatewayExecuteIntegral(RequestInfo *request,const UINT action);
    bool              GatewayExecuteIntegralAsync(TradeRequest *record);
    //--- generate responses for request
    bool              SendRequestConfirm(const int retcode,const RequestInfo *request,const UserInfo &manager);
    //--- send order confirmation to MT
    //bool              SendOrderConfirm(const ExchangeOrder &exchange_order);
    //--- send operation on order to MT
    //bool              SendOrderExecution(const ExchangeOrder &exchange_order);
    //--- send a deal to MT
    bool              SendDealExecution(const FIX43::ExecutionReport &report,const Instrument &symbol);
    bool              SendOrderExecution(const TradeRequest &record,const Instrument &instrument,StringType &message);
    //--- send Dont know trade
    bool              SendDontKnowTrade(LPCSTR order_str,const UINT reason,LPCSTR text);
    //--- send ack
    bool              SendAck(LPCSTR unique_id);
    //--- enter operation type based on the operation in the request
    //void              GetOrderActionByRequestAction(const UINT &mt_request_action,UINT &exhange_order_action);
    //--- get current time of exchange
    INT64             GetExchangeTime();
    //--- log request and response
    bool              LogRequest(const TradeRequest *record) const;
    bool              LogAnswerGateway(const RequestInfo *request) const;

  public:
    bool              ProcessOrderOpen(const TradeRequest &record,StringType &message);
    bool              ProcessOrderClose(const TradeRequest &record,StringType &message);
    bool              ProcessMarketOpen(const TradeRequest &record,StringType &message);
    bool              ProcessMarketClose(const TradeRequest &record,StringType &message);
    bool              ProcessPendingOrderActivation(const TradeRequest &record,StringType &message);

  private:
    double            GetLotSize(LPCSTR symbol,LPCSTR group,const double lots);
    //--- process transactions
    bool              ProcessPendingOrder(const TradeRequest &record,const Instrument &instrument,StringType &message);
    bool              ProcessSLActivation(const TradeRequest &record,const Instrument &instrument,StringType &message);
    bool              ProcessTPActivation(const TradeRequest &record,const Instrument &instrument,StringType &message);
    bool              ProcessPositionStopOut(const TradeRequest &record,const Instrument &instrument,StringType &message);

  private:
    //--- memory
    //RequestRecord *RequestAllocate(void);
    //void              RequestFree(RequestRecord *record);
    virtual TradeRequest *RequestGetPtr(LPCSTR unique_id);
    virtual TradeRequest *RequestGetPtr(const INT request_id);
    virtual TradeRequest *RequestGetPtrByOrder(const INT order);
    virtual bool          RequestGet(const FIX::ClOrdID &unique_id,double cum_qty,double avg_px,const FIX::ClOrdID &clord_id,const FIX::ExecID &exec_id,const FIX::OrdStatus &ord_status,const FIX::OrderID &order_id,LPCSTR exchange_deal,const Instrument &instrument,TradeRequest &request);
    //--- sort/search
    static int        SortRequests(const void *left,const void *right);
    static int        SearchRequests(const void *left,const void *right);
  };
//+------------------------------------------------------------------+
