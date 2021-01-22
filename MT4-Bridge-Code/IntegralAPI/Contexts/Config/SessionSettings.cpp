//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "SessionSettings.h"

CFixSessionSettings FixSessionSettings;
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CFixSessionSettings::CFixSessionSettings(): m_cfg_total(0),m_cfg_max(0)
  {
  base_directory.Clear();
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CFixSessionSettings::~CFixSessionSettings()
  {
  m_sync.Lock();
  //---
  m_cfg_total=m_cfg_max=0;
  base_directory.Clear();
  //---
  m_sync.Unlock();
  }
//+------------------------------------------------------------------+
//| Extract xml                                                      |
//+------------------------------------------------------------------+
void CFixSessionSettings::ExtractXML(LPCSTR strCustomResName,int nResourceId,LPCSTR strOutputName)
  {
  HGLOBAL hResourceLoaded;		// handle to loaded resource 
  HRSRC hRes;						// handle/ptr. to res. info. 
  char *lpResLock;				// pointer to resource data 
  DWORD dwSizeRes;
  //---
  StringTypePath output_location;
  output_location.Format("%s\\%s",base_directory.Str(),strOutputName);

  //std::string strOutputLocation=std::string(m_path.Str())+std::string("\\")+strOutputName;

  HMODULE hModule=(HMODULE)ExtModule;

  // find location of the resource and get handle to it
  hRes=FindResource(hModule,MAKEINTRESOURCE(nResourceId),strCustomResName);

  // determine the size of the resource, so we know how much to write out to file!  
  dwSizeRes=SizeofResource(hModule,hRes);

  // loads the specified resource into global memory. 
  hResourceLoaded=LoadResource(hModule,hRes);

  // get a pointer to the loaded resource!
  lpResLock=(char *)LockResource(hResourceLoaded);

  std::ofstream outputFile(output_location.Str(),std::ios::binary);
  //--- write
  outputFile.write((const char *)lpResLock,dwSizeRes);
  //--- close
  outputFile.close();
  }
//+------------------------------------------------------------------+
//| Load Fix settings                                                |
//+------------------------------------------------------------------+
bool CFixSessionSettings::Initialize(const StringType &path,FIX::SessionSettings &settings)
  {
  //--- check
  if(path.Empty())
    return(false);
  //---
  m_sync.Lock();
  //--- remember path
  base_directory.Assign(path);
  // load settings
  if(!LoadSettings(settings))
    return(false);
  //--- remeber settings
  m_settings=settings;
  //---
  m_sync.Unlock();
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CFixSessionSettings::GetString(const FIX::SessionID &session_id,LPCSTR search,FIX::FieldBase &field)
  {
  m_sync.Lock();
  //---
  if(!m_settings.get(session_id).has(search))
    {
    m_sync.Unlock();
    return(false);
    }
  //---
  field.setString(m_settings.get(session_id).getString(search));
  //---
  m_sync.Unlock();
  return(true);
  }
//+------------------------------------------------------------------+
//| Load Fix settings                                                |
//+------------------------------------------------------------------+
bool CFixSessionSettings::LoadSettings(FIX::SessionSettings &settings)
  {
  StringTypePath settings_file;
  //---
  try
    {
    settings_file.Format("%s\\%s",base_directory.Str(),"FIX.Settings.config");
    //--- extract 
    ExtractXML("XML",IDR_XML1,"FIX43.xml");
    //--- set
    settings=FIX::SessionSettings(settings_file.toString());
    //----
    LOG_INFO(pluginLog,"sessions loaded");
    //---
    FIX::Dictionary dict=settings.get();
    //---
    dict.setString("ConnectionType","initiator");
    dict.setString("BeginString","FIX.4.3");
    //---
    if(!dict.has("StartDay"))  dict.setString("StartDay","Sunday");
    if(!dict.has("StartTime")) dict.setString("StartTime","20:00:00");
    if(!dict.has("EndDay"))    dict.setString("EndDay","Friday");
    if(!dict.has("EndTime"))   dict.setString("EndTime","23:00:00");
    //---
    dict.setString("TimeZone","UTC");
    dict.setString("UseLocalTime","N");
    dict.setString("HeartBtInt","15");
    dict.setString("ReconnectInterval","5");
    dict.setString("UseDataDictionary","Y");
    dict.setString("LogoutTimeout","5");
    dict.setString("ResetOnLogon","Y");
    dict.setString("SocketNodelay","Y");
    dict.setString("PersistMessages","N");
    dict.setString("CheckLatency","N");
    //---
    StringTypePath xml_file;
    xml_file.Format("%s\\%s",base_directory.Str(),"FIX43.xml");
    dict.setString("DataDictionary",std::string(xml_file.Str()));
    //---
    dict.setString("LogoutTimeout","1");
    dict.setString("ValidateFieldsHaveValues","N");
    //---
    settings.set(dict);

    if(!SaveSettings(settings))
      return(false);

    LOG_INFO(pluginLog,"sessings dict loaded");
    //--- successfull
    return(true);
    }
  catch(const std::exception &e)
    {
    LOG_ERROR(pluginLog,"creating settings failed [%s]",e.what());
    }
  //--- successfull
  return(false);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CFixSessionSettings::SaveSettings(const FIX::SessionSettings &settings)
  {
  StringTypePath file;
  StringType256  map_kep;
  std::ofstream fixFile;
  //---
  static std::map<std::string,std::string> WordMap=
    {
      {"ADMINSTREAM","AdminStream"},
      {"ADMINHOST","AdminHost"},
      {"ADMINPORT","AdminPort"},
      {"ADMINPATH","AdminPath"},
      {"BEGINSTRING","BeginString"},
      {"BGROUPS","BGroups"},
      {"CHECKLATENCY","CheckLatency"},
      {"CONNECTIONTYPE","ConnectionType"},
      {"DATADICTIONARY","DataDictionary"},
      {"DELIVERTOCOMPID","DeliverToCompID"},
      {"ENDDAY","EndDay"},
      {"ENDTIME","EndTime"},
      {"FILELOGPATH","FileLogPath"},
      {"FILESTOREPATH","FileStorePath"},
      {"HEARTBTINT","HeartBtInt"},
      {"LOGOUTTIMEOUT","LogoutTimeout"},
      {"MARKETDEPTH","MarketDepth"},
      {"PAPERTRADING","PaperTrading"},
      {"PASS","Pass"},
      {"PRIMEBROKER","PrimeBroker"},
      {"PERSISTMESSAGES","PersistMessages"},
      {"RECONNECTINTERVAL","ReconnectInterval"},
      {"RESETONLOGON","ResetOnLogon"},
      {"SENDERCOMPID","SenderCompID"},
      {"SENDERSUBID","SenderSubID"},
      {"SENDLIMITSASMARKET","SendLimitsAsMarket"},
      {"SENDSTOPSASMARKET","SendStopsAsMarket"},
      {"SHORTNAME","ShortName"},
      {"SOCKETCONNECTHOST","SocketConnectHost"},
      {"SOCKETCONNECTPORT","SocketConnectPort"},
      {"SOCKETCONNECTHOST1","SocketConnectHost1"},
      {"SOCKETCONNECTPORT1","SocketConnectPort1"},
      {"SOCKETCONNECTHOST2","SocketConnectHost2"},
      {"SOCKETCONNECTPORT2","SocketConnectPort2"},
      {"SOCKETCONNECTHOST3","SocketConnectHost3"},
      {"SOCKETCONNECTPORT3","SocketConnectPort3"},
      {"SOCKETCONNECTHOST4","SocketConnectHost4"},
      {"SOCKETCONNECTPORT4","SocketConnectPort4"},
      {"SOCKETCONNECTHOST5","SocketConnectHost5"},
      {"SOCKETCONNECTPORT5","SocketConnectPort5"},
      {"SOCKETNODELAY",           "SocketNodelay"},
      {"STARTDAY",                "StartDay"},
      {"STARTTIME",               "StartTime"},
      {"TARGETCOMPID",            "TargetCompID"},
      {"TIMEZONE",                "TimeZone"},
      {"USEDATADICTIONARY",       "UseDataDictionary"},
      {"USELOCALTIME",            "UseLocalTime"},
      {"USER",                    "User"},
      {"VALIDATEFIELDSHAVEVALUES","ValidateFieldsHaveValues"},
      {"LOGOUTTIMEOUT",           "LogoutTimeout"}
    };

  //--- set file 
  file.Format("%s\\%s",ExtProgramPath,"FIX.Settings.config");
  LOG_INFO(pluginLog,"Save FIX settings: %s",file.Str());
  //---
  fixFile.open(file.Str(),std::ios::out|std::ios::trunc);
  //--- check
  if(!fixFile.is_open())
    return(false);

  //const FIX::Dictionary& defaults(m_settings.m_defaults);
  const SettingsSorter defaults(settings.get().begin(),settings.get().end());
  //---
  if(defaults.size())
    {
    fixFile<<"[DEFAULT]"<<std::endl;
    LOG_INFO(pluginLog,"[DEFAULT]");

    FIX::Dictionary::iterator i;
    for(i=defaults.begin(); i!=defaults.end(); ++i)
      {
      fixFile<<(WordMap.find(i->first)!=WordMap.end()?WordMap[i->first]:i->first)<<"="<<i->second<<std::endl;
      LOG_INFO(pluginLog,"  %s=%s",(WordMap.find(i->first)!=WordMap.end()?WordMap[i->first]:i->first).c_str(),i->second.c_str());
      }

    fixFile<<std::endl;
    LOG_INFO(pluginLog,"[END]");
    }

  std::map<std::string,FIX::SessionID> sortedKeyMap;

  for(auto &item:settings.getSessions())
    {
    if(!settings.get(item).has("ShortName")||
       !settings.get(item).has("User")||
       !settings.get(item).has("SenderCompID"))
      continue;

    map_kep.Clear();
    map_kep.Append(settings.get(item).getString("ShortName"));
    map_kep.Append("_");
    map_kep.Append(item.getSenderCompID().getValue());
    map_kep.Append("_");
    map_kep.Append(settings.get(item).getString("User"));

    sortedKeyMap[map_kep.toString()]=item;
    }

  for(auto &item:sortedKeyMap)
    {
    fixFile<<"[SESSION]"<<std::endl;
    LOG_INFO(pluginLog,"[SESSION]");

    //const FIX::Dictionary& section = m_settings.get(item.second);
    const SettingsSorter section(settings.get(item.second).begin(),settings.get(item.second).end());

    if(!section.size())
      continue;

    //FIX::Dictionary::iterator i;
    //for (i = section.begin(); i != section.end(); ++i)
    for(auto &i:section)
      {
      if(defaults.find(i.first)!=defaults.end()) //if (defaults.has(i->first)) // && defaults.getString(i->first) == i->second
        continue;

      fixFile<<(WordMap.find(i.first)!=WordMap.end()?WordMap[i.first]:i.first)<<"="<<i.second<<std::endl;
      LOG_INFO(pluginLog,"  %s=%s",
               (WordMap.find(i.first)!=WordMap.end()?WordMap[i.first]:i.first).c_str(),i.second.c_str());
      }

    fixFile<<std::endl;
    LOG_INFO(pluginLog,"[END]");
    }
  //--- close file
  fixFile.close();
  //--- successful
  return(true);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int CFixSessionSettings::GetPos(const std::string &value)
  {
  static std::map<std::string,int> SortMap=
    {
    // [DEFAULT]
    {"BEGINSTRING",0},
    {"CONNECTIONTYPE",1},
    {"USEDATADICTIONARY",2},
    {"DATADICTIONARY",3},
    {"STARTDAY",4},
    {"STARTTIME",5},
    {"ENDDAY",6},
    {"ENDTIME",7},
    {"TIMEZONE",8},
    {"USELOCALTIME",9},
    {"RESETONLOGON",10},
    {"SOCKETNODELAY",11},
    {"LOGOUTTIMEOUT",12},
    {"RECONNECTINTERVAL",13},
    {"HEARTBTINT",14},
    {"CHECKLATENCY",15},
    {"PERSISTMESSAGES",16},
    {"VALIDATEFIELDSHAVEVALUES",17},

    // [SESSION]
    {"#",18},
    {"SHORTNAME",19},
    {"DELIVERTOCOMPID",20},
    {"TARGETCOMPID",21},
    {"SENDERCOMPID",22},
    {"SENDERSUBID",23},
    {"USER",24},
    {"PASS",25},
    {"SOCKETCONNECTHOST",26},
    {"SOCKETCONNECTPORT",27},
    {"SOCKETCONNECTHOST1",28},
    {"SOCKETCONNECTPORT1",29},
    {"SOCKETCONNECTHOST2",30},
    {"SOCKETCONNECTPORT2",31},
    {"SOCKETCONNECTHOST3",32},
    {"SOCKETCONNECTPORT3",33},
    {"SOCKETCONNECTHOST4",34},
    {"SOCKETCONNECTPORT4",35},
    {"SOCKETCONNECTHOST5",36},
    {"SOCKETCONNECTPORT5",37},
    {"MARKETDEPTH",38},
    {"PAPERTRADING",39},
    {"SENDLIMITSASMARKET",40},
    {"SENDSTOPSASMARKET",41},
    {"PRIMEBROKER",42},
    {"ADMINHOST",43},
    {"ADMINPORT",44},
    {"ADMINPATH",45},
    {"ADMINSTREAM",46},

    // [DEPRECATED]
    {"FILELOGPATH",47},
    {"FILESTOREPATH",48}
    };
  //--- find
  if(SortMap.find(value)!=SortMap.end())
    return SortMap[value];
  //---
  return SortMap.size(); // If not found, place at end
  }
//+------------------------------------------------------------------+
