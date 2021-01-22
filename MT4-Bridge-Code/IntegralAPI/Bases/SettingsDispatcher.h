//+------------------------------------------------------------------+
//|                                         MetaTrader 5 NFA.Reports |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include <fstream>
#include <windows.h>
//---
#include "Group.h"
#include "Instrument.h"
#include "Route.h"
#include "MarketRange.h"
//--- common
#include "..\..\Common\Helpers.h"
#include "..\..\Common\Const.h"
//--- fix43
#include "..\..\QuickFix\fix43\ExecutionReport.h"
#include "..\..\QuickFix\fix43\MarketDataSnapshotFullRefresh.h"
//+------------------------------------------------------------------+
//| Preliminary declarations                                         |
//+------------------------------------------------------------------+
class CGateway;
//+------------------------------------------------------------------+
//| Settings                                                         |
//+------------------------------------------------------------------+
class CSettingsDispatcher
{
private:
  typedef std::vector<ConSymbol> ConSymbolArray;

private:
  //--- reference to gateway object
  CGateway &m_gateway;
  //--- 
  HANDLE                  dwChangeHandles,hDir;
  //--- database          
  GroupRecordsArray       m_groups;
  InstrumentRecordsArray  m_instruments;
  RouteRecordsArray       m_routes;
  MarketRangeRecordsArray m_ranges;
  StringTypePath          m_directory;
  CMTSync                 m_sync;
  //--- thread of messages processing
  CMTThread               m_thread;
  //--- flag of processing thread
  volatile long           m_thread_workflag;

public:
   CSettingsDispatcher(CGateway *gateway);
  ~CSettingsDispatcher();
  //---
  bool              Initialize(void);
  void              Shutdown(void);
  //--- add book
  bool              OnSymbolBookApply(const IntegralMsgBook &integral_book);
  //--- add ticks
  //bool              OnSymbolTickApply(SymbolTick &integral_tick);
  //--- process deal transaction
  bool              OnSymbolDealApply(const FIX43::ExecutionReport &report);
  //--- find symbols
  bool              GetInstruments(LPCSTR short_name,InstrumentRecordsArray &instruments);
  bool              GetSymbolByName(LPCSTR symbol_name,Instrument &instrument);
  bool              GetSymbolByCcyPair(LPCSTR ccy_pair,Instrument &instrument);
  //--- find groups
  bool              GetGroups(LPCSTR short_name,GroupRecordsArray &groups);
  bool              GetGroupByName(LPCSTR group_name,Group &group);
  //--- get rules
  void              FillRule(IntegralOrder &order);
  //--- get market range
  bool              GetMarketRangeByCcyPair(LPCSTR ccy_pair,MarketRange &range);
  //--- checks
  bool              CheckInstrumentsFile(LPCSTR short_name);
  bool              CheckGroupsFile(LPCSTR short_name);
  //--- read
  bool              ReadSymbols(void);
  bool              ReadRanges(void);
  bool              ReadGroups(void);
  //--- group/symbol config
  bool              UpdateGroupCache(const ConGroup *group);
  bool              UpdateSymbolCache(const ConSymbol *cs);
  //---
  std::string       GetConfigs(void);
  //---
  bool              SetGroup(std::string &group,std::string &config,const bool save_file,const bool force_update);
  bool              SetInstrument(std::string &symbol,std::string &config,const bool save_file,const bool force_update);
  bool              SetRule(const int index,StringType &config,const bool save_file,const bool force_update);

private:
  //--- creation
  bool              CreateDefualtInstruments(LPCSTR short_name,LPCSTR file_name);
  bool              CreateDefaultGroups(LPCSTR short_name,LPCSTR file_name);
  //---
  bool              GetInstrumentsByCcyPair(LPCSTR short_name,LPCSTR ccy_pair,InstrumentRecordsArray &instruments);
  //--- records   
  //bool              CreateSymbols(LPCSTR short_name);    
  bool              ReadRules(void);
  //---
  bool              SetGroup(std::string &group,std::string &config);
  bool              SetInstrument(std::string &symbol,std::string &config);
  bool              SetRule(const int index,StringType &config);
  //--- get/set settings
  bool              SaveSymbols(void);
  bool              SaveGroups(void);
  bool              SaveRules(void);
  void              SaveConfigs(void);

private:
  //--- thread of processing
  static UINT __stdcall ProcessThreadWrapper(LPVOID param);
  void              ProcessThread();
  //--- sorting/searching
  static int        SortSymbols(const void *left,const void *right);
  static int        SearchSymbols(const void *left,const void *right);
  static int        SearchSymbolsByPair(const void *left,const void *right);
  static int        SortGroups(const void *left,const void *right);
  static int        SearchGroups(const void *left,const void *right);
  static int        SortRoutes(const void *left,const void *right);
  static int        SearchRoutes(const void *left,const void *right);
  static int        SortRanges(const void *left,const void *right);
  static int        SearchRanges(const void *left,const void *right);
  //----
  struct myclass
  {
    bool operator() (ConSymbol i,ConSymbol j) { return (StringType::Compare(i.symbol,j.symbol)<0); }
  } mysorter;
};
//extern CSettingsDispatcher ExtSettingsDispatcher;
//+------------------------------------------------------------------+
