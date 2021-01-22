//+------------------------------------------------------------------+
//|                                         MetaTrader 5 NFA.Reports |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "StdAfx.h"
#include "SettingsDispatcher.h"
#include "..\..\Bases\Gateway.h"
//+------------------------------------------------------------------+
//| Global object                                                    |
//+------------------------------------------------------------------+
//CSettingsDispatcher ExtSettingsDispatcher;
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
CSettingsDispatcher::CSettingsDispatcher(CGateway *gateway)
  :m_gateway(*gateway)
  {
  //---
  m_groups.Clear();
  m_instruments.Clear();
  m_routes.Clear();
  //---
  m_directory.Clear();
  }
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
CSettingsDispatcher::~CSettingsDispatcher(void)
  {
  //---
  Shutdown();
  //--- under lock
  m_sync.Lock();
  m_groups.Clear();
  m_instruments.Clear();
  m_routes.Clear();
  m_directory.Clear();
  //--- unlock
  m_sync.Unlock();
  //---
  if(dwChangeHandles)
    {
    FindCloseChangeNotification(dwChangeHandles);
    dwChangeHandles=NULL;
    }
  //---
  if(hDir)
    {
    CancelIoEx(hDir,NULL);
    CloseHandle(hDir);
    hDir=NULL;
    }
  }
//+------------------------------------------------------------------+
//| Initialize watchdog                                              |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::Initialize(void)
  {
  //---
  Shutdown();
  //---
  m_sync.Lock();
  //--- remember root directory
  m_directory.Format("%s",ExtProgramPath);
  LOG_INFO(pluginLog,"checking directory %s",m_directory.Str());
  //--- check directory
  if(!CMTFile::DirectoryCreate(m_directory))
    {
    LOG_ERROR(pluginLog,"%s directory create error",m_directory.Str());
    m_sync.Unlock();
    return(false);
    }
  //--- read rule settings
  if(!ReadRules())
    {
    m_sync.Unlock();
    return(false);
    }
  //---
  ReadRanges();
  //---  
  m_sync.Unlock();
  //--- set flag of thread operation
  InterlockedExchange(&m_thread_workflag,1);
  //--- start thread of processing external data
  if(!m_thread.Start(ProcessThreadWrapper,this,STACK_SIZE_COMMON))
    {
    LOG_ERROR(pluginLog,"failed to start context thread (%d)",::GetLastError());
    return(false);
    }
  //--- successful
  return(true);
  }
//+------------------------------------------------------------------+
//| Shutdown                                                         |
//+------------------------------------------------------------------+
void CSettingsDispatcher::Shutdown()
  {
  //--- reset thread's working flag
  InterlockedExchange(&m_thread_workflag,0);
  //--- waiting thread completion
  m_thread.Shutdown(1000);
  //---
  m_sync.Lock();
  //---
  m_directory.Clear();
  //---
  m_sync.Unlock();
  }
//+------------------------------------------------------------------+
//| Add book                                                         |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::OnSymbolBookApply(const IntegralMsgBook &integral_book)
  {
  InstrumentRecordsArray records;
  Instrument *instrument=NULL;
  bool res=true;
  //--- under lock
  m_sync.Lock();
  //--- find symbol in base  
  if(!GetInstrumentsByCcyPair(integral_book.book.short_name,integral_book.book.ccy_pair,records))
    {
    LOG_ERROR(pluginLog,"failed to apply ticks transaction: symbol %s not found",
              integral_book.book.ccy_pair);
    m_sync.Unlock();
    return(false);
    }
  //---
  m_sync.Unlock();
  //--- send bboks to mt4
  for(UINT pos=0;pos<records.Total();pos++)
    {
    instrument=&records[pos];
    if(!m_gateway.HistoryAddBook(integral_book,instrument))
      {
      if(ExtPluginContext.EnableDebugLogs())
        LOG_ERROR(pluginLog,"HistoryAddBook failed <%s>",integral_book.book.fix_message);
      }
    }
  //---
  records.Shutdown();
  //--- finished
  return(res);
  }
//+------------------------------------------------------------------+
//| Add tick                                                         |
//+------------------------------------------------------------------+
//bool CSettingsDispatcher::OnSymbolTickApply(SymbolTick &integral_tick)
//  {
//  FeedTick gateway_tick={0};
//  Instrument   record={0};
//  //--- find symbol in base
//  if(!GetSymbolByCcyPair(integral_tick.ccy_pair,record))
//    {
//    LOG_ERROR(pluginLog,"failed to apply ticks transaction: symbol %s not found",
//              integral_tick.ccy_pair);
//    return(false);
//    }
//  //--- send to Gateway API
//  return(m_gateway.HistoryAddTick(integral_tick,record));
//  }
//+------------------------------------------------------------------+
//| Process deal transaction                                         |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::OnSymbolDealApply(const FIX43::ExecutionReport &report)
  {
  Instrument   symbol={0};
  FIX::Symbol  fix_symbol;
  StringType64 symbol_str;
  //StringType64 order;
  FIX::ClOrdID clord_id;
  //--- checks
  if(!report.getIfSet(fix_symbol))
    {
    LOG_ERROR(pluginLog,"failed to get ccypair %s",report.toString());
    return(false);
    }
  //---
  if(!report.getIfSet(clord_id))
    {
    LOG_ERROR(pluginLog,"failed to get clord_id %s",report.toString());
    return(false);
    }
  //--- get symbol
  if(!symbol_str.ReadParamStr(clord_id.getValue().c_str(),"symbol:"))
    {
    LOG_ERROR(pluginLog,"failed to get symbol from clordid %s",report.toString());
    return(false);
    }
  //--- get symbol from base 
  if(!GetSymbolByName(symbol_str.Str(),symbol))
    {
    //--- log if order's symbol is not found
    LOG_ERROR(pluginLog,"failed to apply deal transaction: symbol %s not found",fix_symbol.getValue());
    //---
    return(false);
    }
  //--- pass deal transaction to gateway
  return(m_gateway.OnExecutionReportTrans(report,symbol));
  }
//+------------------------------------------------------------------+
//| find symbols by shortname                                        |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::GetInstruments(LPCSTR short_name,InstrumentRecordsArray &instruments)
  {
  bool res=true;
  //--- checks
  if(!short_name)
    return(false);
  //--- clear array
  instruments.Clear();
  //--- lock
  m_sync.Lock();
  //--- loop
  for(UINT pos=0;pos<m_instruments.Total();pos++)
    {
    if(StringType::Compare(m_instruments[pos].short_name,short_name)!=0) continue;
    //---
    if(!instruments.Add(&m_instruments[pos]))
      {
      LOG_ERROR(pluginLog,"adding to list failed <%s>",m_instruments[pos].symbol);
      }
    }
  //--- unlock
  m_sync.Unlock();
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| find symbols by shortname and ccy pair                                       |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::GetInstrumentsByCcyPair(LPCSTR short_name,LPCSTR ccy_pair,InstrumentRecordsArray &instruments)
  {
  bool res=true;
  //--- checks
  if(!short_name||!ccy_pair)
    return(false);
  //--- clear array
  instruments.Clear();
  //--- loop
  for(UINT pos=0;pos<m_instruments.Total();pos++)
    {
    if(StringType::Compare(m_instruments[pos].short_name,short_name)!=0||
       StringType::Compare(m_instruments[pos].ccy_pair,ccy_pair)!=0) continue;
    //---
    if(!instruments.Add(&m_instruments[pos]))
      {
      LOG_ERROR(pluginLog,"adding to list failed <%s>",m_instruments[pos].symbol);
      }
    }
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| find symbol by symbol name                                       |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::GetSymbolByName(LPCSTR symbol_name,Instrument &instrument)
  {
  bool res=true;
  //--- checks
  if(!symbol_name)
    return(false);
  //--- lock
  m_sync.Lock();
  //--- loop
  for(UINT pos=0;pos<m_instruments.Total();pos++)
    {
    if(StringType::Compare(m_instruments[pos].symbol,symbol_name)!=0) continue;
    //---
    instrument=m_instruments[pos];
    res=true;
    }
  //--- unlock
  m_sync.Unlock();
  //---
  return(res);
  }
//+------------------------------------------------------------------+
//| find symbols by ccy pair                                         |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::GetSymbolByCcyPair(LPCSTR ccy_pair,Instrument &instrument)
  {
  bool res=true;
  //--- checks
  if(!ccy_pair)
    return(false);
  //--- lock
  m_sync.Lock();
  //--- loop
  for(UINT pos=0;pos<m_instruments.Total();pos++)
    {
    if(StringType::Compare(m_instruments[pos].ccy_pair,ccy_pair)!=0) continue;
    //---
    instrument=m_instruments[pos];
    res=true;
    }
  //--- unlock
  m_sync.Unlock();
  //---
  return(res);
  }
//+------------------------------------------------------------------+
//| find groups by shortname                                         |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::GetGroups(LPCSTR short_name,GroupRecordsArray &groups)
  {
  bool res=true;
  //--- checks
  if(!short_name)
    return(false);
  //--- clear array
  groups.Clear();
  //--- lock
  m_sync.Lock();
  //--- loop
  for(UINT pos=0;pos<m_groups.Total();pos++)
    {
    if(StringType::Compare(m_groups[pos].short_name,short_name)!=0) continue;
    //---
    res=res&&groups.Add(&m_groups[pos]);
    }
  //--- unlock
  m_sync.Unlock();
  //---
  return(res);
  }
//+------------------------------------------------------------------+
//| find symbol by symbol name                                       |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::GetGroupByName(LPCSTR group_name,Group &group)
  {
  bool res=true;
  //--- checks
  if(!group_name)
    return(false);
  //--- lock
  m_sync.Lock();
  //--- loop
  for(UINT pos=0;pos<m_groups.Total();pos++)
    {
    if(StringType::Compare(m_groups[pos].group,group_name)!=0) continue;
    //---
    group=m_groups[pos];
    res=true;
    }
  //--- unlock
  m_sync.Unlock();
  //---
  return(res);
  }
//+------------------------------------------------------------------+
//| get rules                                                        |
//+------------------------------------------------------------------+
void CSettingsDispatcher::FillRule(IntegralOrder &order)
  {
  //--- lock
  m_sync.Lock();
  //--- loop
  for(UINT pos=0;pos<m_routes.Total();pos++)
    {
    if(Route::isMatch(order.login,std::string(order.group),std::string(order.symbol),order.volume/100.0,m_routes[pos]))
      {
      LOG_INFO(pluginLog,"Rule Matched: %s",m_routes[pos].text);
      order.cover_mode=m_routes[pos].cover_mode;
      break;
      }
    }
  //--- unlock
  m_sync.Unlock();
  }
//+------------------------------------------------------------------+
//| find market range by ccy pair                                    |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::GetMarketRangeByCcyPair(LPCSTR ccy_pair,MarketRange &range)
  {
  bool res=true;
  //--- checks
  if(!ccy_pair)
    return(false);
  //--- lock
  m_sync.Lock();
  //--- loop
  for(UINT pos=0;pos<m_ranges.Total();pos++)
    {
    if(StringType::Compare(m_ranges[pos].ccy_pair,ccy_pair)!=0) continue;
    //---
    range=m_ranges[pos];
    res=true;
    }
  //--- unlock
  m_sync.Unlock();
  //---
  return(res);
  }
//+------------------------------------------------------------------+
//| Read symbols                                                     |
//+------------------------------------------------------------------+
//bool CSettingsDispatcher::ReadSymbols(LPCSTR short_name,InstrumentRecordsArray &instruments)
//  {
//  Instrument     record,*ptr;
//  ConSymbol      symbol={0};
//  std::ifstream  ccyFile;
//  StringType1024 line;
//  StringType1024 text;
//  //--- checks
//  if(!ExtServer||!short_name)
//    return(false);
//  //--- clear
//  instruments.Clear();
//  //---
//  LOG_INFO(pluginLog,"read instruments.dat file");
//  //---
//  StringTypePath file;
//  file.Format("%s\\instruments.dat",m_directory.Str());
//  //--- file exists?
//  if(!std::ifstream(file.Str()).good())
//    {
//    LOG_INFO(pluginLog,"instruments.dat file doesn't exist, so create it");
//    //--- create
//    if(CreateSymbols(short_name))
//      {
//      LOG_INFO(pluginLog,"instruments.dat file created");
//      return(true);
//      }
//    LOG_ERROR(pluginLog,"instruments.dat file creation failed");
//    //---
//    return(false);
//    }
//  //---
//  ccyFile.open(file.Str(),std::ios::in);
//  //--- check
//  if(!ccyFile.is_open())
//    return(false);
//  //--- process
//  while(!ccyFile.eof())
//    {
//    ccyFile.getline(line.Buffer(),line.Max());
//    line.Refresh();
//    //--- check
//    if(line.Empty())
//      continue;
//    //---
//    std::vector<std::string> args;
//    SPluginHelpers::split(line.Str(),"=",args);
//    //--- subscribe
//    if(args.size()!=2||args[0].empty()||args[1].empty())
//      continue;
//    //--- add to list
//    text.Assign(args[1].c_str());
//    //--- create record
//    ZeroMemory(&record,sizeof(Instrument));
//    StringType::Copy(record.symbol,args[0].c_str());
//    if(!Instrument::Fill(text,&record))
//      {
//      continue;
//      }
//    if(StringType::Compare(record.short_name,short_name)!=0) continue;
//    //-- get symbol info
//    if(ExtServer->SymbolsGet(record.symbol,&symbol)==FALSE)  continue;
//    //--- exist?
//    if((ptr=m_instruments.Search(&record.symbol,SearchSymbols))==NULL)
//      {
//      //--- add new
//      if((ptr=m_instruments.Insert(&record,SortSymbols))==NULL)
//        continue;
//      }
//    //--- fill other info
//    ptr->digits        =symbol.digits;
//    ptr->contract_size =symbol.contract_size;
//    ptr->spread        =symbol.spread;
//    ptr->spread_balance=symbol.spread_balance;
//    ptr->exec_mode     =symbol.exemode;
//    ptr->point         =symbol.point;
//    ptr->type          =symbol.type;
//    ptr->point         =symbol.point;
//    }
//  //--- close
//  ccyFile.close();
//  //--- fill instruments
//  for(UINT pos=0;pos<m_instruments.Total();pos++)
//    instruments.Add(&m_instruments[pos]);
//  //--- successfull
//  return(true);
//  }
//+------------------------------------------------------------------+
//| Check symbols file                                               |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::CheckInstrumentsFile(LPCSTR short_name)
  {
  //--- checks
  if(!ExtServer||!short_name)
    return(false);
  //--- under lock
  m_sync.Lock();
  //---
  StringTypePath file;
  file.Format("%s\\instruments.dat",m_directory.Str());
  //--- file exists?
  if(std::ifstream(file.Str()).good())
    {
    m_sync.Unlock();
    return(true);
    }
  //---
  LOG_INFO(pluginLog,"instruments.dat file doesn't exist, so create it");
  //--- create
  if(!CreateDefualtInstruments(short_name,file.Str()))
    {
    LOG_ERROR(pluginLog,"instruments.dat file create failed");
    m_sync.Unlock();
    return(false);
    }
  LOG_INFO(pluginLog,"instruments.dat file created");
  //--- successfull
  m_sync.Unlock();
  return(true);
  }
//+------------------------------------------------------------------+
//| Check groups file                                                |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::CheckGroupsFile(LPCSTR short_name)
  {
  //--- checks
  if(!ExtServer||!short_name)
    return(false);
  //--- under lock
  m_sync.Lock();
  //---
  StringTypePath file;
  file.Format("%s\\groups.dat",m_directory.Str());
  //--- file exists?
  if(std::ifstream(file.Str()).good())
    {
    m_sync.Unlock();
    return(true);
    }
  //---
  LOG_INFO(pluginLog,"groups.dat file doesn't exist, so create it");
  //--- create
  if(!CreateDefaultGroups(short_name,file.Str()))
    {
    LOG_ERROR(pluginLog,"groups.dat file create failed");
    m_sync.Unlock();
    return(false);
    }
  LOG_INFO(pluginLog,"groups.dat file created");
  //--- successfull
  m_sync.Unlock();
  return(true);
  }
//+------------------------------------------------------------------+
//| Read symbols                                                     |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::ReadSymbols(void)
  {
  Instrument     record,*ptr;
  ConSymbol      symbol={0};
  std::ifstream  ccyFile;
  StringType1024 line;
  StringType1024 text;
  //--- checks
  if(!ExtServer)
    return(false);
  //---
  LOG_INFO(pluginLog,"Read Symbols");
  //--- under lock
  m_sync.Lock();
  //---
  StringTypePath file;
  file.Format("%s\\instruments.dat",m_directory.Str());
  //--- file exists?
  if(!std::ifstream(file.Str()).good())
    {
    m_sync.Unlock();
    return(false);
    }
  //--- open file
  ccyFile.open(file.Str(),std::ios::in);
  //--- check
  if(!ccyFile.is_open())
    {
    m_sync.Unlock();
    return(false);
    }
  //--- clear instruments
  m_instruments.Clear();
  //--- process
  while(!ccyFile.eof())
    {
    ccyFile.getline(line.Buffer(),line.Max());
    line.Refresh();
    //--- check
    if(line.Empty())
      continue;
    //---
    std::vector<std::string> args;
    SPluginHelpers::split(line.Str(),"=",args);
    //--- subscribe
    if(args.size()!=2||args[0].empty()||args[1].empty())
      continue;
    //--- add to list
    text.Assign(args[1].c_str());
    //--- create record
    ZeroMemory(&record,sizeof(Instrument));
    StringType::Copy(record.symbol,args[0].c_str());
    if(!Instrument::Fill(text,&record))
      {
      continue;
      }
    //-- get symbol info
    if(ExtServer->SymbolsGet(record.symbol,&symbol)==FALSE)
      {
      LOG_ERROR(pluginLog,"symbols get failed <%s>",record.symbol);
      continue;
      }
    //--- exist?
    if((ptr=m_instruments.Search(&record.symbol,SearchSymbols))==NULL)
      {
      //--- add new
      if((ptr=m_instruments.Insert(&record,SortSymbols))==NULL)
        continue;
      }
    //--- fill other info
    ptr->digits=symbol.digits;
    ptr->contract_size=symbol.contract_size;
    ptr->spread=symbol.spread;
    ptr->spread_balance=symbol.spread_balance;
    ptr->exec_mode=symbol.exemode;
    ptr->point=symbol.point;
    ptr->type=symbol.type;
    ptr->point=symbol.point;
    }
  //--- close
  ccyFile.close();
  //--- successfull
  m_sync.Unlock();
  return(true);
  }
//+------------------------------------------------------------------+
//| Read market ranges                                               |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::ReadRanges(void)
  {
  MarketRange    record,*ptr;
  std::ifstream  ccyFile;
  StringType1024 line;
  //--- checks
  if(!ExtServer)
    return(false);
  //---
  LOG_INFO(pluginLog,"Read Market Ranges");
  //--- under lock
  m_sync.Lock();
  //---
  StringTypePath file;
  file.Format("%s\\qc.dat",m_directory.Str());
  //--- file exists?
  if(!std::ifstream(file.Str()).good())
    {
    m_sync.Unlock();
    return(false);
    }
  //--- open file
  ccyFile.open(file.Str(),std::ios::in);
  //--- check
  if(!ccyFile.is_open())
    {
    m_sync.Unlock();
    return(false);
    }
  //--- clear instruments
  m_instruments.Clear();
  //--- process
  while(!ccyFile.eof())
    {
    ccyFile.getline(line.Buffer(),line.Max());
    line.Refresh();
    //--- check
    if(line.Empty())
      continue;
    //--- create record
    ZeroMemory(&record,sizeof(MarketRange));
    if(!MarketRange::Fill(line,&record))
      {
      continue;
      }
    //--- exist?
    if((ptr=m_ranges.Search(&record.ccy_pair,SearchRanges))==NULL)
      {
      //--- add new
      if((ptr=m_ranges.Insert(&record,SortRanges))==NULL)
        continue;
      }
    //--- fill other info
    ptr->spot_precision=record.spot_precision;
    ptr->pips_factor   =record.pips_factor;
    }
  //--- close
  ccyFile.close();
  //--- successfull
  m_sync.Unlock();
  return(true);
  }
//+------------------------------------------------------------------+
//| Create symbols                                                   |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::CreateDefualtInstruments(LPCSTR short_name,LPCSTR file_name)
  {
  ConSymbol      symbol={0};
  std::ofstream  ccyFile;
  StringType16   str;
  ConSymbolArray symbols;
  //--- checks
  if(!ExtServer||!short_name||!file_name)
    return(false);
  //---
  static std::vector<std::string> G7
    {
     "AED", "AUD", "BHD", "CAD", "CHF", "CNY", "CZK", "DKK", "EUR", "GBP",
     "HKD", "HUF", "ILS", "INR", "JPY", "MXN", "NOK","NZD", "PLN", "QAR",
     "RUB", "SEK", "SGD", "TRY", "USD", "XAG", "XAU", "ZAR"
    };
  //---
  ccyFile.open(file_name,std::ios::out|std::ios::trunc);
  if(!ccyFile.is_open())
    {
    LOG_ERROR(pluginLog,"failed to open instruments.dat file (%d)",::GetLastError());
    return(false);
    }
  //---  TODO: sort accoring to ccypair
  for(int i=0;ExtServer->SymbolsNext(i,&symbol)!=FALSE;i++)
    {
    symbols.push_back(symbol);
    }
  std::sort(symbols.begin(),symbols.end(),mysorter);
  //--
  for(auto &symbol:symbols)
    {
    //---
    std::string oSym(symbol.symbol);
    //--- checks
    if(symbol.margin_mode!=MARGIN_CALC_FOREX||oSym.length()<6)          continue;
    if(std::find(G7.begin(),G7.end(),oSym.substr(0,3))==G7.end()&&
       std::find(G7.begin(),G7.end(),oSym.substr(3,3))==G7.end())       continue;
    //---
    SMTFormat::FormatDouble(str,0.0,symbol.digits);
    //----
    StringType1024 config;
    config.Format("%s=%s, %s/%s, %s, %s, %s, %s, -1, BEST, 1M, Bid, 1k, GTD, 30, True",
                  symbol.symbol,
                  short_name,
                  oSym.substr(0,3).c_str(),
                  oSym.substr(3,3).c_str(),
                  str.Str(),
                  str.Str(),
                  str.Str(),
                  str.Str());
    //---
    ccyFile
      <<config.Str()
      <<std::endl;;
    }
  //--- close
  ccyFile.close();
  //----
  return(true);
  }
//+------------------------------------------------------------------+
//| Read groups                                                      |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::ReadGroups(void)
  {
  ConGroup       group={0};
  Group          record={0};
  std::ifstream  grpFile;
  StringType1024 line_remaind;
  StringType1024 line;
  StringType128  temp;
  std::vector<std::string> args;
  //---
  LOG_INFO(pluginLog,"Read Groups");
  //--- under lock
  m_sync.Lock();
  //---
  StringTypePath file;
  file.Format("%s\\groups.dat",ExtProgramPath);
  //--- checks
  if(!std::ifstream(file.Str()).good())
    {
    m_sync.Unlock();
    return(false);
    }
  //--- open
  grpFile.open(file.Str(),std::ios::in);
  //--- check
  if(!grpFile.is_open())
    {
    m_sync.Unlock();
    return(false);
    }
  //--- clear groups
  m_groups.Clear();
  //---
  while(!grpFile.eof())
    {
    grpFile.getline(line.Buffer(),line.Max()-1);
    line.Refresh();
    //--- check
    if(line.Empty())     continue;
    //---
    args.clear();
    SPluginHelpers::split(line.Str(),"=",args);
    //---
    if(!SetGroup(args[0],args[1]))
      {
      LOG_ERROR(pluginLog,"error setting group:%s",group.group);
      }
    }
  //--- close file
  grpFile.close();
  //---
  m_sync.Unlock();
  return(true);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::UpdateGroupCache(const ConGroup *cfg_group)
  {
  Group *group=NULL;
  //--- checks
  if(!cfg_group)
    return(false);
  //--- under lock
  m_sync.Lock();
  //---
  for(UINT pos=0;pos<m_groups.Total();pos++)
    {
    group=&m_groups[pos];
    if(StringType::Compare(cfg_group->group,group->group)!=0) continue;
    //---
    memcpy(group->secgroups,cfg_group->secgroups,sizeof(cfg_group->secgroups));
    }
  //--- unlock
  m_sync.Unlock();
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::UpdateSymbolCache(const ConSymbol *cs)
  {
  Instrument *instrument=NULL;
  //--- checks
  if(!cs)
    return(false);
  //--- under lock
  m_sync.Lock();
  //---
  for(UINT pos=0;pos<m_instruments.Total();pos++)
    {
    instrument=&m_instruments[pos];
    if(StringType::Compare(cs->symbol,instrument->symbol)!=0) continue;
    //--- fill info
    instrument->digits=cs->digits;
    instrument->contract_size=cs->contract_size;
    instrument->spread=cs->spread;
    instrument->spread_balance=cs->spread_balance;
    instrument->exec_mode=cs->exemode;
    instrument->point=cs->point;
    instrument->type=cs->type;
    instrument->point=cs->point;
    }
  //--- unlock
  m_sync.Unlock();
  return(true);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::CreateDefaultGroups(LPCSTR short_name,LPCSTR file_name)
  {
  ConGroup      group_cfg={0};
  Group         group={0};
  StringType512 line;
  std::ofstream group_file;
  //--- checks
  if(!ExtServer||!short_name||!file_name)
    return(false);
  //---
  group_file.open(file_name,std::ios::out|std::ios::trunc);
  //--- check
  if(!group_file.is_open())
    {
    LOG_ERROR(pluginLog,"failed to open groups.dat file (%d)",::GetLastError());
    return(false);
    }
  //--
  for(int pos=0;ExtServer->GroupsNext(pos,&group_cfg)!=FALSE;pos++)
    {
    Group::Fill(group_cfg,group);
    StringType::Copy(group.short_name,short_name);
    StringType::Terminate(group.on_behalf);
    //---
    Group::Print(group,line);
    //---
    group_file
      <<line.Str()
      <<std::endl;
    }
  //--- close
  group_file.close();
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
//bool CSettingsDispatcher::SetGroup(const std::string &group,const std::string &config)
//{
//  Group          record={},*ptr;
//  ConGroup       group_record={ 0 };
//  std::ofstream  groupFile;
//  StringTypePath file;
//  StringType1024 line;
//  //--- checks
//  if (group.empty()||config.empty())
//    return(false);
//  //--- exist
//  StringType::Copy(record.group,group.c_str());
//  if ((ptr=m_groups.Search(&group,SearchGroups))==NULL)
//  {
//    //--- add new
//    if ((ptr=m_groups.Insert(&record,SortGroups))==NULL)
//    {
//      LOG_ERROR(pluginLog,"group insert to list failed <Group: %s, Config: %s>",group.c_str(),config.c_str());
//      return(false);
//    }
//    //---
//  }
//  //--- log info
//  LOG_INFO(pluginLog,"Group: %s, Config: %s",group.c_str(),config.c_str());
//  //---
//  line.Assign(config.c_str());
//  if (!Group::Fill(line,&record))
//  {
//    LOG_ERROR(pluginLog,"group record fill failed <Group: %s, Config: %s>",group.c_str(),config.c_str());
//    return(false);
//  }
//  //--- groups get
//  if (ExtServer->GroupsGet(group.c_str(),&group_record)==FALSE)
//  {
//    LOG_ERROR(pluginLog,"group config: %s get failed",group.c_str());
//    return(false);
//  }
//  //---
//  memcpy(ptr->secgroups,group_record.secgroups,sizeof(group_record.secgroups));
//  //---
//  ptr->cover_mode=record.cover_mode;
//  ptr->enabled=record.enabled;
//  StringType::Copy(ptr->text,record.text);
//  StringType::Copy(ptr->short_name,record.short_name);
//  StringType::Copy(ptr->on_behalf,record.on_behalf);
//  //---
//  return(true);
//}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::SetGroup(std::string &group,std::string &config,const bool save_file,const bool force_update)
  {
  //--- checks
  if(group.empty()||config.empty())
    return(false);
  //--- under lock
  m_sync.Lock();
  //---
  if(!SetGroup(group,config))
    {
    m_sync.Unlock();
    return(false);
    }
  //--- unlock
  m_sync.Unlock();
  if(save_file)
    return(SaveGroups());
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::SetInstrument(std::string &symbol,std::string &config,const bool save_file,const bool force_update)
  {
  //--- checks
  if(symbol.empty()||config.empty())
    return(false);
  //--- under lock
  m_sync.Lock();
  //---
  if(!SetInstrument(symbol,config))
    {
    m_sync.Unlock();
    return(false);
    }
  //--- unlock
  m_sync.Unlock();
  //--- update file
  if(save_file)
    return(SaveSymbols());
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::SetRule(const int index,StringType &config,const bool save_file,const bool force_update)
  {
  //--- checks
  if(index<0||config.Empty())
    return(false);
  //--- under lock
  m_sync.Lock();
  //---
  if(!SetRule(index,config))
    {
    m_sync.Unlock();
    return(false);
    }
  //--- unlock
  m_sync.Unlock();
  if(save_file)
    return(SaveRules());
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::SetGroup(std::string &group,std::string &config)
  {
  Group          record={},*ptr;
  ConGroup       group_record={0};
  std::ofstream  groupFile;
  StringTypePath file;
  StringType1024 line;
  //--- checks
  if(group.empty()||config.empty())
    return(false);
  //--- exist
  StringType::Copy(record.group,group.c_str());
  if((ptr=m_groups.Search(&group,SearchGroups))==NULL)
    {
    //--- add new
    if((ptr=m_groups.Insert(&record,SortGroups))==NULL)
      {
      LOG_ERROR(pluginLog,"group insert to list failed <Group: %s, Config: %s>",group.c_str(),config.c_str());
      return(false);
      }
    }
  //---
  //--- log info
  LOG_INFO(pluginLog,"Group: %s, Config: %s",group.c_str(),config.c_str());
  //---
  line.Assign(config.c_str());
  if(!Group::Fill(line,&record))
    {
    LOG_ERROR(pluginLog,"group record fill failed <Group: %s, Config: %s>",group.c_str(),config.c_str());
    return(false);
    }
  //--- groups get
  if(ExtServer->GroupsGet(group.c_str(),&group_record)==FALSE)
    {
    LOG_ERROR(pluginLog,"group config: %s get failed",group.c_str());
    return(false);
    }
  //---
  memcpy(ptr->secgroups,group_record.secgroups,sizeof(group_record.secgroups));
  //---
  ptr->cover_mode=record.cover_mode;
  ptr->enabled=record.enabled;
  StringType::Copy(ptr->text,record.text);
  StringType::Copy(ptr->short_name,record.short_name);
  StringType::Copy(ptr->on_behalf,record.on_behalf);
  //--- success
  return(true);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::SetInstrument(std::string &symbol,std::string &config)
  {
  Instrument     record={},*ptr;
  ConSymbol      symbol_record={0};
  StringType1024 line;
  //--- checks
  if(symbol.empty()||config.empty()||!ExtServer)
    return(false);
  //--- exist
  StringType::Copy(record.symbol,symbol.c_str());
  if((ptr=m_instruments.Search(&symbol,SearchSymbols))==NULL)
    {
    //--- add new
    if((ptr=m_instruments.Insert(&record,SortSymbols))==NULL)
      {
      LOG_ERROR(pluginLog,"symbol insert to list failed <Symbol: %s, Config: %s>",symbol.c_str(),config.c_str());
      return(false);
      }
    }
  //---
  //--- log info
  LOG_INFO(pluginLog,"Symbol: %s, Config: %s",symbol.c_str(),config.c_str());
  //---
  line.Assign(config.c_str());
  if(!Instrument::Fill(line,ptr))
    {
    LOG_ERROR(pluginLog,"symbol record fill failed <Symbol: %s, Config: %s>",symbol.c_str(),config.c_str());
    return(false);
    }
  //--- groups get
  if(ExtServer->SymbolsGet(symbol.c_str(),&symbol_record)==FALSE)
    {
    LOG_ERROR(pluginLog,"symbol config: %s get failed",symbol.c_str());
    return(false);
    }
  //--- fill other info
  ptr->digits=symbol_record.digits;
  ptr->contract_size=symbol_record.contract_size;
  ptr->spread=symbol_record.spread;
  ptr->spread_balance=symbol_record.spread_balance;
  ptr->exec_mode=symbol_record.exemode;
  ptr->point=symbol_record.point;
  ptr->type=symbol_record.type;
  ptr->point=symbol_record.point;
  //--- success
  return(true);
  }
//+------------------------------------------------------------------+
//| Read groups                                                      |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::ReadRules(void)
  {
  StringTypePath file;
  StringType1024 rule_line;
  std::ifstream  rules_file;
  int            oLine=0;
  //---
  LOG_INFO(pluginLog,"reading rules.dat file started");
  //--- under lock
  m_sync.Lock();;
  //--- prepare path
  file.Format("%s\\%s",ExtProgramPath,"rules.dat");
  //--- clear
  m_routes.Clear();
  //--- read file
  if(!std::ifstream(file.Str()).good())
    {
    LOG_INFO(pluginLog,"rules.dat doesn't exist, so create it");
    //---
    rules_file.open(file.Str(),std::ios::out|std::ios::trunc);
    //---
    if(rules_file.is_open())
      rules_file.close();
    //---
    LOG_INFO(pluginLog,"rules.dat file created");
    }
  else
    {
    rules_file.open(file.Str(),std::ios::in);
    //--- open?
    if(!rules_file.is_open())
      {
      LOG_ERROR(pluginLog,"failed to open rules.dat file (%d)",::GetLastError());
      //---
      m_sync.Unlock();
      return(false);
      }
    //--- process file
    while(!rules_file.eof())
      {
      //--- get line
      rules_file.getline(rule_line.Buffer(),rule_line.Max()-1);
      rule_line.Refresh();
      //--- check
      if(rule_line.Empty())
        continue;
      //--- set rule
      if(!SetRule(++oLine,rule_line))
        {
        LOG_INFO(pluginLog,"'%d' set rule failed <%s>",oLine,rule_line.Str());
        }
      }
    //--- close
    rules_file.close();
    }
  LOG_INFO(pluginLog,"reading rules.dat file finished");
  //--- successful
  m_sync.Unlock();
  return(true);
  }
//+------------------------------------------------------------------+
//| set rule                                                         |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::SetRule(const int index,StringType &config)
  {
  Route record,*ptr;
  //--- checks
  if(index<0||config.Empty())
    return(false);
  //---- fill rule
  if(!Route::Fill(config,&record))
    return(false);
  //--- exist
  if((ptr=m_routes.Search(&index,SearchRoutes))==NULL)
    {
    //--- add new
    if((ptr=m_routes.Insert(&record,SortRoutes))==NULL)
      return(false);
    //--- set index
    ptr->index=index;
    }
  //---
  LOG_INFO(pluginLog,"Index: %d, Config: %s",index,config.Str());
  //--- set
  ptr->cover_mode=record.cover_mode;
  ptr->enabled=record.enabled;
  StringType::Copy(ptr->group,record.group);
  StringType::Copy(ptr->login,record.login);
  ptr->max_size=record.max_size;
  ptr->min_size=record.min_size;
  StringType::Copy(ptr->symbol,record.symbol);
  StringType::Copy(ptr->text,record.text);
  //---
  //--- ok
  return(true);
  }
//+------------------------------------------------------------------+
//| Save Instruments                                                 |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::SaveSymbols()
  {
  std::ofstream ccyFile;
  StringTypePath file;
  StringType2048 res;
  //---
  m_sync.Lock();
  file.Format("%s\\%s",m_directory.Str(),"instruments.dat");
  ccyFile.open(file.Str(),std::ios::out|std::ios::trunc);
  if(!ccyFile.is_open())
    {
    //---
    m_sync.Unlock();
    return(false);
    }
  //---
  LOG_INFO(pluginLog,"Saving Instruments to FILE: %s",file.Str());
  for(UINT pos=0;pos<m_instruments.Total();pos++)
    {
    Instrument::Print(m_instruments[pos],res);
    //---
    ccyFile
      <<res.Str()<<std::endl;
    }
  //--- close file
  ccyFile.close();
  //---
  m_sync.Unlock();
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| Save groups                                                      |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::SaveGroups()
  {
  std::ofstream ccyFile;
  StringTypePath file;
  StringType2048 res;
  //---
  m_sync.Lock();
  //--- prepare directory
  file.Format("%s\\%s",m_directory.Str(),"groups.dat");
  ccyFile.open(file.Str(),std::ios::out|std::ios::trunc);
  if(!ccyFile.is_open())
    {
    //---
    m_sync.Unlock();
    return(false);
    }
  //---
  LOG_INFO(pluginLog,"Saving grops to FILE: %s",file.Str());
  for(UINT pos=0;pos<m_groups.Total();pos++)
    {
    Group::Print(m_groups[pos],res);
    //---
    ccyFile
      <<res.Str()<<std::endl;
    }
  //--- close file
  ccyFile.close();
  //---
  m_sync.Unlock();
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| Save rules                                                       |
//+------------------------------------------------------------------+
bool CSettingsDispatcher::SaveRules()
  {
  std::ofstream ccyFile;
  StringTypePath file;
  StringType2048 res;
  //---
  m_sync.Lock();
  //--- prepare directory
  file.Format("%s\\%s",m_directory.Str(),"rules.dat");
  ccyFile.open(file.Str(),std::ios::out|std::ios::trunc);
  if(!ccyFile.is_open())
    {
    //---
    m_sync.Unlock();
    return(false);
    }
  //---
  LOG_INFO(pluginLog,"Saving grops to FILE: %s",file.Str());
  for(UINT pos=0;pos<m_routes.Total();pos++)
    {
    Route::Print(m_routes[pos],res);
    //---
    ccyFile
      <<res.Str()<<std::endl;
    }
  //--- close file
  ccyFile.close();
  //---
  m_sync.Unlock();
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| Save configs                                                     |
//+------------------------------------------------------------------+
void CSettingsDispatcher::SaveConfigs()
  {
  time_t         rawtime;
  struct tm *timeinfo;
  char           buffer[80];
  StringTypePath fileName;
  //--- checks
  if(!ExtServer)
    return;
  //---
  rawtime=ExtServer->TradeTime();
  //time(&rawtime);
  timeinfo=gmtime(&rawtime);
  strftime(buffer,80,".%Y-%m-%d-%H%M.ini",timeinfo);
  //--- under lock
  m_sync.Lock();
  //---
  const std::string fileData=GetConfigs();
  fileName.Format("%s\\%s%s",m_directory.Str(),"Logs\\IDC.Config",buffer);

  std::ofstream outfile(fileName.Str());

  if(outfile.fail()||!outfile.is_open())
    {
    LOG_ERROR(pluginLog,"Could not create: %s",fileName.Str());
    m_sync.Unlock();
    return;
    }
  //---
  outfile<<fileData<<std::endl;
  //--- close
  outfile.close();
  m_sync.Unlock();
  //--- show
  LOG_INFO(pluginLog,"Backup complete: %s",fileName.Str());
  }
//+------------------------------------------------------------------+
//| Get configs                                                      |
//+------------------------------------------------------------------+
std::string CSettingsDispatcher::GetConfigs()
  {
  StringTypePath file;
  std::ifstream  oFile;
  StringType1024 data;
  std::ostringstream toReturn;
  //--- fix settings
  file.Format("%s\\%s",m_directory.Str(),"FIX.Settings.config");
  //--- open file
  oFile.open(file.Str(),std::ios::in);
  if(oFile.is_open())
    {
    //--- read
    while(!oFile.eof())
      {
      data.Clear();
      oFile.getline(data.Buffer(),data.Max());
      data.Refresh();
      toReturn<<data.Str()<<std::endl;
      }
    //--- close
    oFile.close();
    }
  //--- groups settings
  file.Clear();
  file.Format("%s\\%s",m_directory.Str(),"groups.dat");
  //--- open file
  oFile.open(file.Str(),std::ios::in);
  if(oFile.is_open())
    {
    toReturn<<"[GROUPS]"<<std::endl;
    //--- read
    while(!oFile.eof())
      {
      data.Clear();
      oFile.getline(data.Buffer(),data.Max());
      data.Refresh();
      toReturn<<data.Str()<<std::endl;
      }
    //--- close file
    oFile.close();
    }
  //--- instruments settings
  file.Clear();
  file.Format("%s\\%s",m_directory.Str(),"instruments.dat");
  //--- open file
  oFile.open(file.Str(),std::ios::in);
  if(oFile.is_open())
    {
    toReturn<<"[SYMBOLS]"<<std::endl;
    //--- read
    while(!oFile.eof())
      {
      data.Clear();
      oFile.getline(data.Buffer(),data.Max());
      data.Refresh();
      toReturn<<data.Str()<<std::endl;
      }
    //--- close
    oFile.close();
    }
  //--- instruments settings
  file.Clear();
  file.Format("%s\\%s",m_directory.Str(),"rules.dat");
  //--- open file
  oFile.open(file.Str(),std::ios::in);
  if(oFile.is_open())
    {
    toReturn<<"[RULES]"<<std::endl;
    //--- read
    while(!oFile.eof())
      {
      data.Clear();
      oFile.getline(data.Buffer(),data.Max());
      data.Refresh();
      toReturn<<data.Str()<<std::endl;
      }
    //--- close
    oFile.close();
    }
  //--- return
  return toReturn.str();
  }
//+------------------------------------------------------------------+
//| Start thread of listening processing                             |
//+------------------------------------------------------------------+
UINT __stdcall CSettingsDispatcher::ProcessThreadWrapper(LPVOID param)
  {
  //--- check
  CSettingsDispatcher *pThis=reinterpret_cast<CSettingsDispatcher *>(param);
  if(pThis!=NULL) pThis->ProcessThread();
  //--- ok
  return(0);
  }
//+------------------------------------------------------------------+
//| Process messages                                                 |
//+------------------------------------------------------------------+
void CSettingsDispatcher::ProcessThread()
  {
  //--- check
  DWORD dwWaitStatus;
  //---
  hDir=CreateFile(ExtProgramPath,FILE_LIST_DIRECTORY,FILE_SHARE_WRITE|FILE_SHARE_READ|FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
  // Watch the directory for file creation and deletion. 
  dwChangeHandles=FindFirstChangeNotification(
    ExtProgramPath,                         // directory to watch 
    FALSE,                         // do not watch subtree 
    FILE_NOTIFY_CHANGE_LAST_WRITE); // watch file name changes 
  //---
  if(dwChangeHandles==INVALID_HANDLE_VALUE)
    {
    LOG_ERROR(pluginLog,"FindFirstChangeNotification function failed.");
    return;
    }
  // Make a final validation check on our handles.
  if(dwChangeHandles==NULL)
    {
    LOG_ERROR(pluginLog,"Unexpected NULL from FindFirstChangeNotification.");
    return;
    }
  // Change notification is set. Now wait on both notification 
  // handles and refresh accordingly. 

  //--- read groups
  ReadGroups();
  //--- read symbols
  ReadSymbols();
  //--- read rules
  ReadRules();

  // Wait for notification.
  LOG_INFO(pluginLog,"Monitoring directory for changes...");
  //--- loop of managing gateway connection
  while(InterlockedExchangeAdd(&m_thread_workflag,0)>0)
    {
    DWORD dwBytesReturned=0;
    bool isGroups=false,isSymbols=false,isRoutes=false;
    FILE_NOTIFY_INFORMATION strFileNotifyInfo[1024];
    //--- wait here
    dwWaitStatus=WaitForSingleObject(dwChangeHandles,INFINITE);
    //---
    switch(dwWaitStatus)
      {
      case WAIT_OBJECT_0:

        if(FindNextChangeNotification(dwChangeHandles)==FALSE)
          {
          LOG_ERROR(pluginLog,"FindNextChangeNotification function failed.");
          return;
          }

        if(ReadDirectoryChangesW(hDir,(LPVOID)&strFileNotifyInfo,sizeof(strFileNotifyInfo),FALSE,FILE_NOTIFY_CHANGE_LAST_WRITE,&dwBytesReturned,NULL,NULL)==0)
          {
          LOG_ERROR(pluginLog,"Could not identify changed file");
          }
        else
          {
          std::wstring tmp(strFileNotifyInfo[0].FileName,dwBytesReturned);
          std::string fileChanged(tmp.begin(),tmp.end());
          //--- check groups file
          if(fileChanged.find("groups.dat")!=std::string::npos)
            {
            isGroups=ReadGroups();
            }
          //--- check instruments file
          if(fileChanged.find("instruments.dat")!=std::string::npos)
            {
            isSymbols=ReadSymbols();
            }
          //--- check rules file
          if(fileChanged.find("rules.dat")!=std::string::npos)
            {
            isRoutes=ReadRules();
            }
          }
        //--- check
        if(!isGroups&&!isSymbols&&!isRoutes) continue;

        // A file was created, renamed, or deleted in the directory.
        // Refresh this directory and restart the notification.
        LOG_INFO(pluginLog,"File Modification occurred!");
        //--- save configs
        SaveConfigs();

        break;

      case WAIT_TIMEOUT:

        // A timeout occurred, this would happen if some value other 
        // than INFINITE is used in the Wait call and no changes occur.
        // In a single-threaded environment you might not want an
        // INFINITE wait.

        LOG_INFO(pluginLog,"No changes in the timeout period.");
        break;

      default:
        LOG_INFO(pluginLog,"Unhandled dwWaitStatus.");
        break;
      }
    }
  //--- update context state - processing thread stopped
  }
//+------------------------------------------------------------------+
//| Sort function                                                    |
//+------------------------------------------------------------------+
inline int CSettingsDispatcher::SortSymbols(const void *left,const void *right)
  {
  Instrument *lft=(Instrument *)left;
  Instrument *rgh=(Instrument *)right;
  //---
  return(StringType::Compare(lft->symbol,rgh->symbol));
  }
//+------------------------------------------------------------------+
//| Search function                                                  |
//+------------------------------------------------------------------+
inline int CSettingsDispatcher::SearchSymbols(const void *left,const void *right)
  {
  LPCSTR      lft=(LPCSTR)left;
  Instrument *rgh=(Instrument *)right;
  //---
  return(StringType::Compare(lft,rgh->symbol));
  }
//+------------------------------------------------------------------+
//| Search function                                                  |
//+------------------------------------------------------------------+
inline int CSettingsDispatcher::SearchSymbolsByPair(const void *left,const void *right)
  {
  LPCSTR      lft=(LPCSTR)left;
  Instrument *rgh=(Instrument *)right;
  //---
  return(StringType::Compare(lft,rgh->ccy_pair));
  }
//+------------------------------------------------------------------+
//| Sort function                                                    |
//+------------------------------------------------------------------+
inline int CSettingsDispatcher::SortGroups(const void *left,const void *right)
  {
  Group *lft=(Group *)left;
  Group *rgh=(Group *)right;
  //---
  return(StringType::Compare(lft->group,rgh->group));
  }
//+------------------------------------------------------------------+
//| Search function                                                  |
//+------------------------------------------------------------------+
inline int CSettingsDispatcher::SearchGroups(const void *left,const void *right)
  {
  LPCSTR  lft=(LPCSTR)left;
  Group *rgh=(Group *)right;
  //---
  return(StringType::Compare(lft,rgh->group));
  }
//+------------------------------------------------------------------+
//| Sort function                                                    |
//+------------------------------------------------------------------+
inline int CSettingsDispatcher::SortRoutes(const void *left,const void *right)
  {
  const Route &lft=*(const Route *)left;
  const Route &rgh=*(const Route *)right;
  //--- compare
  if(lft.index>rgh.index)
    return(1);
  if(lft.index<rgh.index)
    return(-1);
  //--- equal
  return(0);
  }
//+------------------------------------------------------------------+
//| Search function                                                  |
//+------------------------------------------------------------------+
inline int CSettingsDispatcher::SearchRoutes(const void *left,const void *right)
  {
  const INT &lft=*(const INT *)left;
  const Route &rgh=*(const Route *)right;
  //--- compare
  if(lft>rgh.index)
    return(1);
  if(lft<rgh.index)
    return(-1);
  //--- equal
  return(0);
  }
//+------------------------------------------------------------------+
//| Sort function                                                    |
//+------------------------------------------------------------------+
inline int CSettingsDispatcher::SortRanges(const void *left,const void *right)
  {
  const MarketRange &lft=*(const MarketRange *)left;
  const MarketRange &rgh=*(const MarketRange *)right;
  //--- compare
  return(StringType::Compare(lft.ccy_pair,rgh.ccy_pair));
  }
//+------------------------------------------------------------------+
//| Search function                                                  |
//+------------------------------------------------------------------+
inline int CSettingsDispatcher::SearchRanges(const void *left,const void *right)
  {
  LPCSTR       lft=(LPCSTR)left;
  MarketRange *rgh=(MarketRange *)right;
  //---
  return(StringType::Compare(lft,rgh->ccy_pair));
  }
//+------------------------------------------------------------------+
