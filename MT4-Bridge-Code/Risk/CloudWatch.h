//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include <regex>
#include <string>
#include <fstream>
#include <atlbase.h>
//#include <string_view>
//---
#include "..\version.h"
#include "..\include\MT4ServerAPI.h"
#include "..\IO\Log.h"
#include "..\Common\PluginContext.h"
#include "..\IntegralAPI\Common\IntegralMsgBook.h"
using namespace std::tr1;
//+------------------------------------------------------------------+
//| cloud watch                                                      |
//+------------------------------------------------------------------+
class CloudWatch
  {
  private:
    CLog             *m_log;
    //---
    volatile int      tickTotal;
    volatile int      dropTotal;
    volatile int      tickCount;
    volatile int      latencyCount;
    volatile int      latencyLast;
    volatile int      tickTime;
    volatile int      tickLow;
    volatile int      tickHigh;
    //--- server
    std::string       ServerName;
    std::string       ServerBuild;
    std::string       ServerAddress;
    //--- version
    std::string       BridgeVersion;
    //--- machine                 
    std::string       Machine_Connections;
    std::string       Machine_FreeMemory;
    std::string       Machine_CPU;
    std::string       Machine_NetTraffic;
    std::string       Machine_Sockets;
    std::string       Machine_Threads;
    std::string       Machine_Handles;
    //--- process                 
    std::string       Process_CPU;
    std::string       Process_NetIn;
    std::string       Process_NetOut;
    //orders                 
    std::string       Order_Count;
    std::string       Order_Failed;
    std::string       Order_Partial;
    //--- prcies                 
    std::string       Prices_Count;
    std::string       Prices_Drop;
    //--- latency                 
    std::string       Latency_Average;
    std::string       Latency_Minimum;
    std::string       Latency_Maximum;
    //---
    volatile int      orderTotal;
    volatile int      orderPartial;
    volatile int      orderFailed;

    CMTSync           m_sync; // syncronizer

  public:
     //--- constructor/destructor
                      CloudWatch();
                     ~CloudWatch();
    //--- initialize/shutdown
    bool              Initialize(CLog *log);
    void              Shutdown();
    //--- methods
    bool              Check(const IntegralMsgBook &integral_book);
    void              PrintPlugins(void);
    void              MonitorMachine(const std::string &log);
    void              MonitorMachine(const PerformanceInfo *inf);
    void              MonitorProcess(const std::string &log);
    void              MonitorPrices(const int tickCount,const int dropTotal,const int tickTotal,const int tickLow,const int tickHigh);
    void              MonitorOrders(bool isPartial,bool isFailed);
    std::string       to_json(bool showHeader);
  };

extern CloudWatch  ExtCloudWatch;
//+------------------------------------------------------------------+
