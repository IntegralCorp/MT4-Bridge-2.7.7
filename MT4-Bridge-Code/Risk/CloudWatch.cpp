#pragma once
#include "StdAfx.h"
#include "CloudWatch.h"
//+------------------------------------------------------------------+
//| Global objects                                                   |
//+------------------------------------------------------------------+
CloudWatch ExtCloudWatch;
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
CloudWatch::CloudWatch()
  :orderTotal(0),
  orderPartial(0),
  orderFailed(0),
  tickTotal(0),
  dropTotal(0),
  tickCount(0),
  latencyCount(0),
  latencyLast(0),
  tickTime(time(NULL)),
  tickLow(MAXINT),
  tickHigh(MININT)
  {
  }

CloudWatch::~CloudWatch()
  {
  //---
  Shutdown();
  //---
  m_sync.Lock();
  //---
  orderTotal  =0;
  orderPartial=0;
  orderFailed =0;
  //---
  tickTotal   =0;
  dropTotal   =0;
  tickCount   =0;
  latencyCount=0;
  latencyLast =0;
  tickTime    =0;
  tickLow     =0;
  tickHigh    =0;
  //---
  m_sync.Unlock();
  }
//+------------------------------------------------------------------+
//| Initialzie cloud watch                                           |
//+------------------------------------------------------------------+
bool CloudWatch::Initialize(CLog *log)
  {
  ConCommon cc={0};
  // checks
  if(!ExtServer||!log)
    return(false);
  //--- shutdown
  Shutdown();
  //--- remember
  m_log       =log;
  //---
  ExtServer->CommonGet(&cc);

  m_sync.Lock();

  { // IPs
  TCHAR path[MAX_PATH];
  GetModuleFileName(NULL,path,MAX_PATH);
  PathRemoveFileSpec(path);
  PathAppend(path,TEXT("config\\servers.ini"));

  std::ifstream oFile;
  oFile.open(path,std::ios::binary);
  if(!oFile)
    {
    std::cout<<"Failed to open the file."<<std::endl;
    m_sync.Unlock();
    return(false);
    }

  if(oFile.is_open())
    {
    char data[10240];
    //regex rgxHost("(([a-zA-Z]|[a-zA-Z][a-zA-Z0-9\-]*[a-zA-Z0-9])\.)*([A-Za-z]|[A-Za-z][A-Za-z0-9\-]*[A-Za-z0-9])");
    const std::regex rgxIP("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}");

    int i = 0;
    while(!oFile.eof())
      {
      oFile.getline(data,10240,'\0');
      std::string strData(data);
      std::smatch result;

      while(std::regex_search(strData,result,rgxIP))
        {
        ServerAddress += (std::string(i>0?"|":"")+result.str());
        strData = result.suffix().str();
        i++;
        }
      }

    oFile.close();
    }
  }
  //---
  PrintPlugins();

  /*int iLength = sizeof(cc.bind_adresses) / sizeof(cc.bind_adresses[0]);
  for (int i = 0; i < iLength; i++)
  {
  if (cc.bind_adresses[i] == NULL)
  continue;

  struct in_addr addr;
  addr.s_addr = cc.bind_adresses[i];
  char *dot_ip = inet_ntoa(addr);

  if (!std::string(dot_ip).empty())
  ServerAddress += (std::string(i > 0 ? "|" : "") + std::string(dot_ip));
  }*/

  ServerName = std::string(cc.owner).substr(0,std::string(cc.owner).find("(4.00 build")-1)+std::string(" - ")+std::string(cc.name);
  //ServerName = std::string(cc.owner).substr(0, std::string(cc.owner).find(" (")) + std::string(" - ") + std::string(cc.name);
  ServerBuild = /*std::to_string(cc.server_version) + std::string(".") +*/ std::to_string(cc.server_build);

  BridgeVersion = std::string("MT ")+std::string(VERSION_STRING);

  m_sync.Unlock();
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| Shutdown                                                         |
//+------------------------------------------------------------------+
void CloudWatch::Shutdown()
  {
  //---
  m_log       =NULL;
  }
//+------------------------------------------------------------------+
//| check prices                                                     |
//+------------------------------------------------------------------+
bool CloudWatch::Check(const IntegralMsgBook &integral_book)
  {
  FIX::UTCTIMESTAMP utc;
  FIX::SendingTime  sending_time;
  INT               max_quote_latency;
  //---
  //book.getHeader().get(sending_time);
  /*FIX::UTCTIMESTAMP fxi = FIX::UTCTIMESTAMP(sending_time.getValue();*/
  m_sync.Lock();
  //---
  int timeTaken=(integral_book.book.fxi<utc?(FIX::DateTime::SECONDS_PER_DAY*(utc.m_date-integral_book.book.fxi.m_date)+utc.m_time-integral_book.book.fxi.m_time):0);
  latencyLast=timeTaken;
  //---
  tickHigh   =max(timeTaken,tickHigh);
  tickLow    =min(timeTaken,tickLow);
  tickTotal +=timeTaken;
  tickCount++;
  //--- check
  if(tickTime+60<=time(NULL))
    {
    tickTime=time(NULL);
    //---
    MonitorPrices(tickCount,dropTotal,tickTotal,tickLow,tickHigh);
    LOG_INFO(pluginLog,"Tick Count: %d  Drop Count: %d/%d (%.2f%%)  Avg. Latency: %.2fms (%.2fms)  Avg. Ticks: %.0f/sec  [Low: %dms / High: %dms]",tickCount,dropTotal,latencyCount,(dropTotal*100.0)/tickCount,(tickTotal*1.0)/tickCount,((tickTotal*1.0)/tickCount)-tickLow,tickCount/60.0,tickLow,tickHigh);
    //--- re-initialize
    tickTotal   =0;
    dropTotal   =0;
    tickCount   =0;
    latencyCount=0;
    tickLow     =MAXINT;
    tickHigh    =MININT;
    }
  //---
  max_quote_latency=ExtPluginContext.MaxQuoteLatency();
  if((max_quote_latency>0)&&timeTaken>max_quote_latency)
    {
    dropTotal++;
    latencyCount++;
    //---
    if(ExtPluginContext.EnableDebugLogs())
      LOG_DEBUG(pluginLog,"Drop price due to latency. Time: %d, Max: %d, Msg: %s",
                timeTaken,
                max_quote_latency,
                integral_book.book.fix_message);
    //--- Drop Rate due to latency
    m_sync.Unlock();
    return(false);
    }
  //--- successfull
  m_sync.Unlock();
  return(true);
  }
//+------------------------------------------------------------------+
//| print plugin parameters                                          |
//+------------------------------------------------------------------+
void CloudWatch::PrintPlugins(void)
  {
  TCHAR path[MAX_PATH];
  GetModuleFileName(NULL,path,MAX_PATH);
  PathRemoveFileSpec(path);
  PathAppend(path,TEXT("config\\plugins.ini"));
  //---
  std::ifstream oFile;
  oFile.open(path,std::ios::binary);
  if(!oFile)
    {
    std::cout<<"Failed to open the file."<<std::endl;
    return;
    }

  if(oFile.is_open())
    {
    char data[10240];

    int i = 0;
    while(!oFile.eof())
      {
      i++;
      oFile.getline(data,10240,'\0');

      if(i==1)
        continue;

      std::string st(data);
      if(st.length()<=1)
        continue;

      if(st.find(".dll")!=std::string::npos)
        {
        LOG_INFO(m_log,"[%s]",st.c_str());
        }
      else
        {
        LOG_INFO(m_log,"  %s",st.c_str());
        }
      }

    oFile.close();
    }
  }

void CloudWatch::MonitorMachine(const std::string &log)
  {
  regex rgx("connections: ([0-9]+)  free memory: ([0-9]+) [a-z]{2}  cpu: ([0-9]+)%  net: ([0-9]+) [a-zA-Z/]+ sockets: ([0-9]+) threads: ([0-9]+) handles: ([0-9]+)");
  smatch result;
  regex_search(log,result,rgx);

  if(result.size()!=8)
    return;
  //--- under lock
  m_sync.Lock();
  //---
  Machine_Connections=result[1];        // connections count
  Machine_FreeMemory =result[2];        // free memory
  Machine_CPU        =result[3];        // cpu usage
  Machine_NetTraffic =result[4];        // network usage byte per.sec.
  Machine_Sockets    =result[5];        // reserved
  Machine_Threads    =result[6];
  Machine_Handles    =result[7];
  //--- unlock
  m_sync.Unlock();
  }

void CloudWatch::MonitorMachine(const PerformanceInfo *inf)
  {
  //--- checks
  if(!inf)
    return;

  m_sync.Lock();
  Machine_Connections=std::to_string(inf->users);
  Machine_FreeMemory =std::to_string(inf->freemem);
  Machine_CPU        =std::to_string(inf->cpu);
  Machine_NetTraffic =std::to_string(inf->network);
  Machine_Sockets    =std::to_string(inf->sockets);

  m_sync.Unlock();
  }

void CloudWatch::MonitorProcess(const std::string &log)
  {
  regex rgx("process cpu: ([0-9]+)%  net in: ([0-9]+) [a-zA-Z/]+  net out: ([0-9]+) [a-zA-Z/]+");
  smatch result;
  regex_search(log,result,rgx);

  if(result.size()!=4)
    return;
  //--- under lock
  m_sync.Lock();
  //---
  Process_CPU = result[1];
  Process_NetIn = result[2];
  Process_NetOut = result[3];
  //--- lock
  m_sync.Unlock();
  }

void CloudWatch::MonitorPrices(const int tickCount,const int dropTotal,const int tickTotal,const int tickLow,const int tickHigh)
  {
  //m_sync.Lock();
  //---
  Prices_Count   =std::to_string(tickCount);
  Prices_Drop    =std::to_string(dropTotal);
  //----
  Latency_Average=std::to_string(static_cast<int>(round((tickTotal*1.0)/tickCount)));
  Latency_Minimum=std::to_string(tickLow);
  Latency_Maximum=std::to_string(tickHigh);
  //---
  //m_sync.Unlock();
  }

void CloudWatch::MonitorOrders(bool isPartial,bool isFailed)
  {
  m_sync.Lock();

  if(isPartial)
    orderPartial++;

  if(isFailed)
    orderFailed++;

  orderTotal++;

  m_sync.Unlock();
  }

std::string CloudWatch::to_json(bool showHeader)
  {
  std::string oJSON;

  m_sync.Lock();

  if(showHeader)
    {
    oJSON += "n:"+ServerName+",";
    oJSON += "b:"+ServerBuild+",";
    oJSON += "a:"+ServerAddress+",";
    oJSON += "v:"+BridgeVersion+",";
    }
  else
    {
    Order_Count  =std::to_string(orderTotal);
    Order_Failed =std::to_string(orderFailed);
    Order_Partial=std::to_string(orderPartial);

    if(!Machine_Connections.empty()) oJSON += "mc:"+Machine_Connections+",";
    if(!Machine_CPU.empty())			 oJSON += "mp:"+Machine_CPU+",";
    if(!Machine_NetTraffic.empty())	 oJSON += "mn:"+Machine_NetTraffic+",";
    if(!Machine_Sockets.empty())		 oJSON += "ms:"+Machine_Sockets+",";
    if(!Machine_Threads.empty())		 oJSON += "mt:"+Machine_Threads+",";
    if(!Machine_Handles.empty())		 oJSON += "mh:"+Machine_Handles+",";

    if(!Machine_FreeMemory.empty())	 oJSON += "pm:"+Machine_FreeMemory+",";
    if(!Process_CPU.empty())			 oJSON += "pp:"+Process_CPU+",";
    if(!Process_NetIn.empty())		 oJSON += "pi:"+Process_NetIn+",";
    if(!Process_NetOut.empty())		 oJSON += "po:"+Process_NetOut+",";

    if(!Order_Count.empty())			 oJSON += "oc:"+Order_Count+",";
    if(!Order_Failed.empty())		    oJSON += "of:"+Order_Failed+",";
    if(!Order_Partial.empty())		 oJSON += "op:"+Order_Partial+",";

    if(!Prices_Count.empty())		    oJSON += "qc:"+Prices_Count+",";
    if(!Prices_Drop.empty())			 oJSON += "qd:"+Prices_Drop+",";

    if(!Latency_Average.empty())		 oJSON += "la:"+Latency_Average+",";
    if(!Latency_Minimum.empty())		 oJSON += "ll:"+Latency_Minimum+",";
    if(!Latency_Maximum.empty())		 oJSON += "lh:"+Latency_Maximum+",";
    }
  //---
  m_sync.Unlock();
  //
  return (oJSON.empty()?"":oJSON.substr(0,oJSON.length()-1));
  }