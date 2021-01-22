// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
//----
PluginInfo			ExtPluginInfo={ "MT4.Plugin", VERSION_FULL, "\251 2018-2021 Integral Development Corp.", { 0 } };
CServerInterface *ExtServer=NULL;

#define MAX_BUFFER 1024

volatile bool  isOn=false;
HMODULE	      hLib=NULL;
char				LibFilename[MAX_PATH]={ 0 };
//////////////////////////////////////////////////////////////////////////
#pragma region Procs
volatile mAbout					pAbout=NULL;
volatile mStartup				pStartup=NULL;
volatile mCleanup				pCleanup=NULL;

volatile mPluginCfgAdd			pPluginCfgAdd=NULL;
volatile mPluginCfgSet			pPluginCfgSet=NULL;
volatile mPluginCfgGet			pPluginCfgGet=NULL;
volatile mPluginCfgNext			pPluginCfgNext=NULL;
volatile mPluginCfgDelete		pPluginCfgDelete=NULL;
volatile mPluginCfgTotal		pPluginCfgTotal=NULL;
volatile mPluginCfgLog			pPluginCfgLog=NULL;

volatile mLog					   pLog=NULL;
volatile mScheduler				pScheduler=NULL;
volatile mPerformance			pPerformance=NULL;
volatile mService				pService=NULL;

volatile mUserConnect			pUserConnect=NULL;
volatile mUserLogin				pUserLogin=NULL;
volatile mUserLogout			pUserLogout=NULL;
volatile mUserLoginExt			pUserLoginExt=NULL;
volatile mUserDemo				pUserDemo=NULL;
volatile mUserCommand			pUserCommand=NULL;
volatile mUserChangePass		pUserChangePass=NULL;
volatile mUserChangePassExt		pUserChangePassExt=NULL;
volatile mUserAdd				pUserAdd=NULL;
volatile mUserUpdate			pUserUpdate=NULL;
volatile mUserDelete			pUserDelete=NULL;
volatile mUserArchive			pUserArchive=NULL;
volatile mUserCompactDemo		pUserCompactDemo=NULL;
volatile mUserCompactArchive	pUserCompactArchive=NULL;
volatile mUserRestore			pUserRestore=NULL;
volatile mUserInterestDaily		pUserInterestDaily=NULL;
volatile mUserInterestMonth		pUserInterestMonth=NULL;
volatile mUserFixBalance		pUserFixBalance=NULL;

volatile mFeederData			pFeederData=NULL;
volatile mFeederStart			pFeederStart=NULL;
volatile mFeederCheck			pFeederCheck=NULL;
volatile mFeederStop			pFeederStop=NULL;

volatile mGroupsAdd				pGroupsAdd=NULL;
volatile mGroupsDelete			pGroupsDelete=NULL;
volatile mSymbolsAdd			pSymbolsAdd=NULL;
volatile mSymbolsDelete			pSymbolsDelete=NULL;
volatile mManagersAdd			pManagersAdd=NULL;
volatile mManagersDelete		pManagersDelete=NULL;

volatile mTradeRequestFilter	pTradeRequestFilter=NULL;
volatile mTradeRequestApply		pTradeRequestApply=NULL;
volatile mTradeRequestRestore	pTradeRequestRestore=NULL;
volatile mTradeTransaction		pTradeTransaction=NULL;
volatile mTradeCheckIE			pTradeCheckIE=NULL;
volatile mTradeStopsFilter		pTradeStopsFilter=NULL;
volatile mTradeStopsApply		pTradeStopsApply=NULL;
volatile mTradePendingsFilter	pTradePendingsFilter=NULL;
volatile mTradePendingsApply	pTradePendingsApply=NULL;
volatile mTradeStopoutsFilter	pTradeStopoutsFilter=NULL;
volatile mTradeStopoutsApply	pTradeStopoutsApply=NULL;
volatile mTradesAdd				pTradesAdd=NULL;
volatile mTradesAddExt			pTradesAddExt=NULL;
volatile mTradesAddRequest		pTradesAddRequest=NULL;
volatile mTradesUpdate			pTradesUpdate=NULL;
volatile mTradesRestore			pTradesRestore=NULL;
volatile mTradesCloseBy			pTradesCloseBy=NULL;

volatile mTradeCommission		pTradeCommission=NULL;
volatile mTradeCommissionAgent	pTradeCommissionAgent=NULL;
volatile mTradeRollover			pTradeRollover=NULL;

volatile mStatementsBaseAll		pStatementsBaseAll=NULL;
volatile mStatementsUser		pStatementsUser=NULL;
volatile mStatementsRollovers	pStatementsRollovers=NULL;

volatile mDealerConnect			pDealerConnect=NULL;
volatile mDealerDisconnect		pDealerDisconnect=NULL;
volatile mDealerGet				pDealerGet=NULL;
volatile mDealerPrice			pDealerPrice=NULL;
volatile mDealerConfirm			pDealerConfirm=NULL;
volatile mDealerRequote			pDealerRequote=NULL;
volatile mDealerReset			pDealerReset=NULL;
volatile mDealerClientReject	pDealerClientReject=NULL;

volatile mHistoryTickFilter		pHistoryTickFilter=NULL;
volatile mHistoryTickApply		pHistoryTickApply=NULL;
volatile mHistoryCheck			pHistoryCheck=NULL;

//volatile mNewsCheck				pNewsCheck = NULL;
//volatile mMailCheck				pMailCheck = NULL;

volatile mTelnet				   pTelnet=NULL;
volatile mTelnetExt				pTelnetExt=NULL;
volatile mManagerProtocol		pManagerProtocol=NULL;
#pragma endregion

inline void logToMT4(int errorCode,const char *format,...)
  {
  char buf[512]={0};

  va_list args;
  va_start(args,format);
  vsprintf_s(buf,sizeof(buf)-1,format,args);
  va_end(args);
  //return;
  //--- checks
  if(!ExtServer)
    {
    printf("%s\n",buf);
    return;
    }
  //---
  ExtServer->LogsOut(errorCode,ExtPluginInfo.name,buf);
  }

void split(const std::string &str,const std::string &delim,std::vector<std::string> &output)
  {
  unsigned int offset=0;
  unsigned int delimIndex=0;

  delimIndex=str.find(delim,offset);

  while(delimIndex!=std::string::npos)
    {
    output.push_back(str.substr(offset,delimIndex-offset));
    offset+=delimIndex-offset+delim.length();
    delimIndex=str.find(delim,offset);
    }
  output.push_back(str.substr(offset));
  }

//+------------------------------------------------------------------+
//| DLL entry point                                                  |
//+------------------------------------------------------------------+
BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID /*lpReserved*/)
  {
  char tmp[256],*cp;
  //----
  switch(ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
      //---- create configuration filename
      GetModuleFileName((HMODULE)hModule,tmp,sizeof(tmp)-5);
      //printf("tmp=%s \n", tmp);

      if((cp=strrchr(tmp,'.'))!=NULL)
        {
        *cp=0;
        strcat(tmp,".ini");
        }
      //---- load configuration
      ExtConfig.Load(tmp);
      break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
      break;

    case DLL_PROCESS_DETACH:
      //ExtProcessor.ShowStatus();
      break;
    }
  //----
  return(TRUE);
  }

void ReadSettings()
  {
  int i_on=0;
  int i_bebug_print=0;
  ExtConfig.GetInteger("_On",&i_on,"1");
  ExtConfig.GetString("_Bridge",LibFilename,MAX_PATH,".\\plugins\\Integral\\MT4.Bridge.idc");
  //THEO//ExtConfig.GetInteger("_DebugPrint", &i_bebug_print, "0");
  isOn=(i_on==1);
  }
//+------------------------------------------------------------------+
//| Shutdown                                                         |
//+------------------------------------------------------------------+
void Shutdown()
  {
  BOOL res=FALSE;
  //---
  logToMT4(CmdOK,"Shutdown starts");
  //--- check & free
  if(hLib!=NULL)
    {
    //proxy Cleanup
    if(pCleanup)
      pCleanup();
    //clear procs
    pAbout=NULL;
    pStartup=NULL;
    pCleanup=NULL;

    pPluginCfgAdd=NULL;
    pPluginCfgSet=NULL;
    pPluginCfgGet=NULL;
    pPluginCfgNext=NULL;
    pPluginCfgDelete=NULL;
    pPluginCfgTotal=NULL;
    pPluginCfgLog=NULL;

    pLog=NULL;
    pScheduler=NULL;
    pPerformance=NULL;
    pService=NULL;

    pUserConnect=NULL;
    pUserLogin=NULL;
    pUserLogout=NULL;
    pUserLoginExt=NULL;
    pUserDemo=NULL;
    pUserCommand=NULL;
    pUserChangePass=NULL;
    pUserChangePassExt=NULL;
    pUserAdd=NULL;
    pUserUpdate=NULL;
    pUserDelete=NULL;
    pUserArchive=NULL;
    pUserCompactDemo=NULL;
    pUserCompactArchive=NULL;
    pUserRestore=NULL;
    pUserInterestDaily=NULL;
    pUserInterestMonth=NULL;
    pUserFixBalance=NULL;

    pFeederData=NULL;
    pFeederStart=NULL;
    pFeederCheck=NULL;
    pFeederStop=NULL;

    pGroupsAdd=NULL;
    pGroupsDelete=NULL;
    pSymbolsAdd=NULL;
    pSymbolsDelete=NULL;
    pManagersAdd=NULL;
    pManagersDelete=NULL;

    pTradeRequestFilter=NULL;
    pTradeRequestApply=NULL;
    pTradeRequestRestore=NULL;
    pTradeTransaction=NULL;
    pTradeCheckIE=NULL;
    pTradeStopsFilter=NULL;
    pTradeStopsApply=NULL;
    pTradePendingsFilter=NULL;
    pTradePendingsApply=NULL;
    pTradeStopoutsFilter=NULL;
    pTradeStopoutsApply=NULL;
    pTradesAdd=NULL;
    pTradesAddExt=NULL;
    pTradesAddRequest=NULL;
    pTradesUpdate=NULL;
    pTradesRestore=NULL;
    pTradesCloseBy=NULL;

    pTradeCommission=NULL;
    pTradeCommissionAgent=NULL;
    pTradeRollover=NULL;

    pStatementsBaseAll=NULL;
    pStatementsUser=NULL;
    pStatementsRollovers=NULL;

    pDealerConnect=NULL;
    pDealerDisconnect=NULL;
    pDealerGet=NULL;
    pDealerPrice=NULL;
    pDealerConfirm=NULL;
    pDealerRequote=NULL;
    pDealerReset=NULL;
    pDealerClientReject=NULL;

    pHistoryTickFilter=NULL;
    pHistoryTickApply=NULL;
    pHistoryCheck=NULL;

    //pNewsCheck = NULL;
    //pMailCheck = NULL;

    pTelnet=NULL;
    pTelnetExt=NULL;
    pManagerProtocol=NULL;

    //--- why?
    Sleep(2000);
    //---
    logToMT4(CmdOK,"About to free the library");
    //---
    res=::FreeLibrary((HMODULE)hLib);
    //---
    if(res==TRUE)
      {
      logToMT4(CmdOK,"Library is freed");
      }
    else
      {
      logToMT4(CmdErr,"Failed to free the library");
      }
    //---
    hLib=NULL;
    }
  //---
  logToMT4(CmdOK,"Shutdown finishes");
  }

void Reinitialize()
  {
  logToMT4(CmdOK,"Reinitialize starts");

  if(!isOn&&hLib!=NULL)// only if we need to unload, and the library is loaded
    {
    Sleep(1000);//to make sure that all hooks have returned
    Shutdown();
    }

  if(isOn&&hLib==NULL)// only if we need to initialize, but the library is not loaded
    {
    //We load the library provided for us
    logToMT4(CmdOK,"About to load the library");
    //--- load
    if((hLib=::LoadLibrary(LibFilename))==NULL)  //we failed
      {
      DWORD nErrCode=::GetLastError();
      logToMT4(CmdErr,"Unable to load DLL: %s",LibFilename);
      return;
      }

    logToMT4(CmdOK,"Libary loaded %s",LibFilename);

    //now read the procs
    pAbout=(mAbout)GetProcAddress(hLib,"iAbout");
    pStartup=(mStartup)GetProcAddress(hLib,"iStartup");
    pCleanup=(mCleanup)GetProcAddress(hLib,"iCleanup");

    pPluginCfgAdd=(mPluginCfgAdd)GetProcAddress(hLib,"iPluginCfgAdd");
    pPluginCfgSet=(mPluginCfgSet)GetProcAddress(hLib,"iPluginCfgSet");
    pPluginCfgGet=(mPluginCfgGet)GetProcAddress(hLib,"iPluginCfgGet");
    pPluginCfgNext=(mPluginCfgNext)GetProcAddress(hLib,"iPluginCfgNext");
    pPluginCfgDelete=(mPluginCfgDelete)GetProcAddress(hLib,"iPluginCfgDelete");
    pPluginCfgTotal=(mPluginCfgTotal)GetProcAddress(hLib,"iPluginCfgTotal");
    pPluginCfgLog=(mPluginCfgLog)GetProcAddress(hLib,"iPluginCfgLog");
    pLog=(mLog)GetProcAddress(hLib,"iLog");
    pScheduler=(mScheduler)GetProcAddress(hLib,"iScheduler");
    pPerformance=(mPerformance)GetProcAddress(hLib,"iPerformance");
    pService=(mService)GetProcAddress(hLib,"iService");

    pUserConnect=(mUserConnect)GetProcAddress(hLib,"iUserConnect");
    pUserLogin=(mUserLogin)GetProcAddress(hLib,"iUserLogin");
    pUserLogout=(mUserLogout)GetProcAddress(hLib,"iUserLogout");
    pUserLoginExt=(mUserLoginExt)GetProcAddress(hLib,"iUserLoginExt");
    pUserDemo=(mUserDemo)GetProcAddress(hLib,"iUserDemo");
    pUserCommand=(mUserCommand)GetProcAddress(hLib,"iUserCommand");
    pUserChangePass=(mUserChangePass)GetProcAddress(hLib,"iUserChangePass");
    pUserChangePassExt=(mUserChangePassExt)GetProcAddress(hLib,"iUserChangePassExt");
    pUserAdd=(mUserAdd)GetProcAddress(hLib,"iUserAdd");
    pUserUpdate=(mUserUpdate)GetProcAddress(hLib,"iUserUpdate");
    pUserDelete=(mUserDelete)GetProcAddress(hLib,"iUserDelete");
    pUserArchive=(mUserArchive)GetProcAddress(hLib,"iUserArchive");
    pUserCompactDemo=(mUserCompactDemo)GetProcAddress(hLib,"iUserCompactDemo");
    pUserCompactArchive=(mUserCompactArchive)GetProcAddress(hLib,"iUserCompactArchive");
    pUserRestore=(mUserRestore)GetProcAddress(hLib,"iUserRestore");
    pUserInterestDaily=(mUserInterestDaily)GetProcAddress(hLib,"iUserInterestDaily");
    pUserInterestMonth=(mUserInterestMonth)GetProcAddress(hLib,"iUserInterestMonth");
    pUserFixBalance=(mUserFixBalance)GetProcAddress(hLib,"iUserFixBalance");

    pFeederData=(mFeederData)GetProcAddress(hLib,"iFeederData");
    pFeederStart=(mFeederStart)GetProcAddress(hLib,"iFeederStart");
    pFeederCheck=(mFeederCheck)GetProcAddress(hLib,"iFeederCheck");
    pFeederStop=(mFeederStop)GetProcAddress(hLib,"iFeederStop");

    pGroupsAdd=(mGroupsAdd)GetProcAddress(hLib,"iGroupsAdd");
    pGroupsDelete=(mGroupsDelete)GetProcAddress(hLib,"iGroupsDelete");
    pSymbolsAdd=(mSymbolsAdd)GetProcAddress(hLib,"iSymbolsAdd");
    pSymbolsDelete=(mSymbolsDelete)GetProcAddress(hLib,"iSymbolsDelete");
    pManagersAdd=(mManagersAdd)GetProcAddress(hLib,"iManagersAdd");
    pManagersDelete=(mManagersDelete)GetProcAddress(hLib,"iManagersDelete");

    pTradeRequestFilter=(mTradeRequestFilter)GetProcAddress(hLib,"iTradeRequestFilter");
    pTradeRequestApply=(mTradeRequestApply)GetProcAddress(hLib,"iTradeRequestApply");
    pTradeRequestRestore=(mTradeRequestRestore)GetProcAddress(hLib,"iTradeRequestRestore");
    pTradeTransaction=(mTradeTransaction)GetProcAddress(hLib,"iTradeTransaction");
    pTradeCheckIE=(mTradeCheckIE)GetProcAddress(hLib,"iTradeCheckIE");
    pTradeStopsFilter=(mTradeStopsFilter)GetProcAddress(hLib,"iTradeStopsFilter");
    pTradeStopsApply=(mTradeStopsApply)GetProcAddress(hLib,"iTradeStopsApply");
    pTradePendingsFilter=(mTradePendingsFilter)GetProcAddress(hLib,"iTradePendingsFilter");
    pTradePendingsApply=(mTradePendingsApply)GetProcAddress(hLib,"iTradePendingsApply");
    pTradeStopoutsFilter=(mTradeStopoutsFilter)GetProcAddress(hLib,"iTradeStopoutsFilter");
    pTradeStopoutsApply=(mTradeStopoutsApply)GetProcAddress(hLib,"iTradeStopoutsApply");
    pTradesAdd=(mTradesAdd)GetProcAddress(hLib,"iTradesAdd");
    pTradesAddExt=(mTradesAddExt)GetProcAddress(hLib,"iTradesAddExt");
    pTradesAddRequest=(mTradesAddRequest)GetProcAddress(hLib,"iTradesAddRequest");
    pTradesUpdate=(mTradesUpdate)GetProcAddress(hLib,"iTradesUpdate");
    pTradesRestore=(mTradesRestore)GetProcAddress(hLib,"iTradesRestore");
    pTradesCloseBy=(mTradesCloseBy)GetProcAddress(hLib,"iTradesCloseBy");

    pTradeCommission=(mTradeCommission)GetProcAddress(hLib,"iTradeCommission");
    pTradeCommissionAgent=(mTradeCommissionAgent)GetProcAddress(hLib,"iTradeCommissionAgent");
    pTradeRollover=(mTradeRollover)GetProcAddress(hLib,"iTradeRollover");

    pStatementsBaseAll=(mStatementsBaseAll)GetProcAddress(hLib,"iStatementsBaseAll");
    pStatementsUser=(mStatementsUser)GetProcAddress(hLib,"iStatementsUser");
    pStatementsRollovers=(mStatementsRollovers)GetProcAddress(hLib,"iStatementsRollovers");

    pDealerConnect=(mDealerConnect)GetProcAddress(hLib,"iDealerConnect");
    pDealerDisconnect=(mDealerDisconnect)GetProcAddress(hLib,"iDealerDisconnect");
    pDealerGet=(mDealerGet)GetProcAddress(hLib,"iDealerGet");
    pDealerPrice=(mDealerPrice)GetProcAddress(hLib,"iDealerPrice");
    pDealerConfirm=(mDealerConfirm)GetProcAddress(hLib,"iDealerConfirm");
    pDealerRequote=(mDealerRequote)GetProcAddress(hLib,"iDealerRequote");
    pDealerReset=(mDealerReset)GetProcAddress(hLib,"iDealerReset");
    pDealerClientReject=(mDealerClientReject)GetProcAddress(hLib,"iDealerClientReject");

    pHistoryTickFilter=(mHistoryTickFilter)GetProcAddress(hLib,"iHistoryTickFilter");
    pHistoryTickApply=(mHistoryTickApply)GetProcAddress(hLib,"iHistoryTickApply");
    pHistoryCheck=(mHistoryCheck)GetProcAddress(hLib,"iHistoryCheck");

    //pNewsCheck = (mNewsCheck)GetProcAddress(hLib, "iNewsCheck");
    //pMailCheck = (mMailCheck)GetProcAddress(hLib, "iMailCheck");

    pTelnet=(mTelnet)GetProcAddress(hLib,"iTelnet");
    pTelnetExt=(mTelnetExt)GetProcAddress(hLib,"iTelnetExt");
    pManagerProtocol=(mManagerProtocol)GetProcAddress(hLib,"iManagerProtocol");

    //log results
    if(pAbout) logToMT4(CmdOK,"Found iAbout");
    if(pStartup) logToMT4(CmdOK,"Found iStartup");
    if(pCleanup) logToMT4(CmdOK,"Found iCleanup");

    if(pPluginCfgAdd) logToMT4(CmdOK,"Found iPluginCfgAdd");
    if(pPluginCfgSet) logToMT4(CmdOK,"Found iPluginCfgSet");
    if(pPluginCfgGet) logToMT4(CmdOK,"Found iPluginCfgGet");
    if(pPluginCfgNext) logToMT4(CmdOK,"Found iPluginCfgNext");
    if(pPluginCfgDelete) logToMT4(CmdOK,"Found iPluginCfgDelete");
    if(pPluginCfgTotal) logToMT4(CmdOK,"Found iPluginCfgTotal");
    if(pPluginCfgLog) logToMT4(CmdOK,"Found iPluginCfgLog");

    if(pLog) logToMT4(CmdOK,"Found iLog");
    if(pScheduler) logToMT4(CmdOK,"Found iScheduler");
    if(pPerformance) logToMT4(CmdOK,"Found iPerformance");
    if(pService) logToMT4(CmdOK,"Found iService");

    if(pUserConnect) logToMT4(CmdOK,"Found iUserConnect");
    if(pUserLogin) logToMT4(CmdOK,"Found iUserLogin");
    if(pUserLogout) logToMT4(CmdOK,"Found iUserLogout");
    if(pUserLoginExt) logToMT4(CmdOK,"Found iUserLoginExt");
    if(pUserDemo) logToMT4(CmdOK,"Found iUserDemo");
    if(pUserCommand) logToMT4(CmdOK,"Found iUserCommand");
    if(pUserChangePass) logToMT4(CmdOK,"Found iUserChangePass");
    if(pUserChangePassExt) logToMT4(CmdOK,"Found iUserChangePassExt");
    if(pUserAdd) logToMT4(CmdOK,"Found iUserAdd");
    if(pUserUpdate) logToMT4(CmdOK,"Found iUserUpdate");
    if(pUserDelete) logToMT4(CmdOK,"Found iUserDelete");
    if(pUserArchive) logToMT4(CmdOK,"Found iUserArchive");
    if(pUserCompactDemo) logToMT4(CmdOK,"Found iUserCompactDemo");
    if(pUserCompactArchive) logToMT4(CmdOK,"Found iUserCompactArchive");
    if(pUserRestore) logToMT4(CmdOK,"Found iUserRestore");
    if(pUserInterestDaily) logToMT4(CmdOK,"Found iUserInterestDaily");
    if(pUserInterestMonth) logToMT4(CmdOK,"Found iUserInterestMonth");
    if(pUserFixBalance) logToMT4(CmdOK,"Found iUserFixBalance");

    if(pFeederData) logToMT4(CmdOK,"Found iFeederData");
    if(pFeederStart) logToMT4(CmdOK,"Found iFeederStart");
    if(pFeederCheck) logToMT4(CmdOK,"Found iFeederCheck");
    if(pFeederStop) logToMT4(CmdOK,"Found iFeederStop");

    if(pGroupsAdd) logToMT4(CmdOK,"Found iGroupsAdd");
    if(pGroupsDelete) logToMT4(CmdOK,"Found iGroupsDelete");
    if(pSymbolsAdd) logToMT4(CmdOK,"Found iSymbolsAdd");
    if(pSymbolsDelete) logToMT4(CmdOK,"Found iSymbolsDelete");
    if(pManagersAdd) logToMT4(CmdOK,"Found iManagersAdd");
    if(pManagersDelete) logToMT4(CmdOK,"Found iManagersDelete");

    if(pTradeRequestFilter) logToMT4(CmdOK,"Found iTradeRequestFilter");
    if(pTradeRequestApply) logToMT4(CmdOK,"Found iTradeRequestApply");
    if(pTradeRequestRestore) logToMT4(CmdOK,"Found iTradeRequestRestore");
    if(pTradeTransaction) logToMT4(CmdOK,"Found iTradeTransaction");
    if(pTradeCheckIE) logToMT4(CmdOK,"Found iTradeCheckIE");
    if(pTradeStopsFilter) logToMT4(CmdOK,"Found iTradeStopsFilter");
    if(pTradeStopsApply) logToMT4(CmdOK,"Found iTradeStopsApply");
    if(pTradePendingsFilter) logToMT4(CmdOK,"Found iTradePendingsFilter");
    if(pTradePendingsApply) logToMT4(CmdOK,"Found iTradePendingsApply");
    if(pTradeStopoutsFilter) logToMT4(CmdOK,"Found iTradeStopoutsFilter");
    if(pTradeStopoutsApply) logToMT4(CmdOK,"Found iTradeStopoutsApply");
    if(pTradesAdd) logToMT4(CmdOK,"Found iTradesAdd");
    if(pTradesAddExt) logToMT4(CmdOK,"Found iTradesAddExt");
    if(pTradesAddRequest) logToMT4(CmdOK,"Found iTradesAddRequest");
    if(pTradesUpdate) logToMT4(CmdOK,"Found iTradesUpdate");
    if(pTradesRestore) logToMT4(CmdOK,"Found iTradesRestore");
    if(pTradesCloseBy) logToMT4(CmdOK,"Found iTradesCloseBy");

    if(pTradeCommission) logToMT4(CmdOK,"Found iTradeCommission");
    if(pTradeCommissionAgent) logToMT4(CmdOK,"Found iTradeCommissionAgent");
    if(pTradeRollover) logToMT4(CmdOK,"Found iTradeRollover");

    if(pStatementsBaseAll) logToMT4(CmdOK,"Found iStatementsBaseAll");
    if(pStatementsUser) logToMT4(CmdOK,"Found iStatementsUser");
    if(pStatementsRollovers) logToMT4(CmdOK,"Found iStatementsRollovers");

    if(pDealerConnect) logToMT4(CmdOK,"Found iDealerConnect");
    if(pDealerDisconnect) logToMT4(CmdOK,"Found iDealerDisconnect");
    if(pDealerGet) logToMT4(CmdOK,"Found iDealerGet");
    if(pDealerPrice) logToMT4(CmdOK,"Found iDealerPrice");
    if(pDealerConfirm) logToMT4(CmdOK,"Found iDealerConfirm");
    if(pDealerRequote) logToMT4(CmdOK,"Found iDealerRequote");
    if(pDealerReset) logToMT4(CmdOK,"Found iDealerReset");
    if(pDealerClientReject) logToMT4(CmdOK,"Found iDealerClientReject");

    if(pHistoryTickFilter) logToMT4(CmdOK,"Found iHistoryTickFilter");
    if(pHistoryTickApply) logToMT4(CmdOK,"Found iHistoryTickApply");
    if(pHistoryCheck) logToMT4(CmdOK,"Found iHistoryCheck");

    //if (pNewsCheck) logToMT4(CmdOK, "Found iNewsCheck");
    //if (pMailCheck) logToMT4(CmdOK, "Found iMailCheck");

    if(pTelnet) logToMT4(CmdOK,"Found iTelnet");
    if(pTelnetExt) logToMT4(CmdOK,"Found iTelnetExt");
    if(pManagerProtocol) logToMT4(CmdOK,"Found iManagerProtocol");

    if(ExtServer&&pStartup&&isOn)
      pStartup(ExtServer);
    }

  logToMT4(CmdOK,"Reinitialize finishes");
  }

/********************************************************************************************************************************************\
|	Purpose - getting the plugin information.
|	  The hook is called by the server during the plugin loading and is intended for getting the plugin information shown in the MetaTrader
|	  Administrator. The hook is called as the very first, before any other hooks.                                        |
|
|	Parameters:
|	  info - plugin description to be filled out when the hook is called
\********************************************************************************************************************************************/
void APIENTRY MtSrvAbout(PluginInfo *info)
  {
  //--- checks
  if(!info)
    return;
  //---
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(info!=NULL)
    memcpy(info,&ExtPluginInfo,sizeof(PluginInfo));
  //---
  logToMT4(CmdOK,"iAbout function returning name %s, version %d.%d.%d, copyright %s",
           ExtPluginInfo.name,
           VERSION_MAJOR,
           VERSION_MINOR,VERSION_BUILD,
           ExtPluginInfo.copyright);
  //---
  if(pAbout&&isOn)
    pAbout(info);
  }

/********************************************************************************************************************************************\
|	Purpose - plugin initialization and call for the server interface.
|	  The hook is called after the plugin is loaded into the memory to provide it with the pointer to the server interface.
|
|	Parameters:
|	  server - server interface
|
|	Important: The pointer to the server interface is passed to the plugin only once when hook MtSrvStartup is called.
\********************************************************************************************************************************************/
int APIENTRY MtSrvStartup(CServerInterface *server)
  {
  //---- check version
  if(server==NULL)
    return FALSE;

  if(server->Version()!=ServerApiVersion)
    return FALSE;

  //---- save server interface link
  ExtServer=server;

  logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  //Sleep(60000);

  ReadSettings();
  Reinitialize();

  //----
  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - plugin deinitialization.
|	  The hook is called immediately before the plugin is dicarded from the memory.
|
|	Important: The server interface methods may not be reffered to after this hook has been called.
\********************************************************************************************************************************************/
void APIENTRY MtSrvCleanup(void)
  {
  logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  //proxy Cleanup
  //if(pCleanup)
  //  pCleanup();

  Shutdown();
  }

/********************************************************************************************************************************************\
|	Standard configuration functions
\********************************************************************************************************************************************/
int APIENTRY MtSrvPluginCfgAdd(const PluginCfg *value)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  int res=TRUE;
  if(strlen(value->name)>0&&value->name[0]=='_')
    {
    res=ExtConfig.Add(value);
    //---- ×èòàåì íàñòðîéêè
    ReadSettings();
    Reinitialize();

    return res;
    }

  //proxy 
  if(pPluginCfgAdd&&isOn)
    return pPluginCfgAdd(value);

  return FALSE;
  }

int APIENTRY MtSrvPluginCfgSet(const PluginCfg *values,const int total)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  int res=TRUE;

  if(total==0)
    return TRUE;

  PluginCfg *this_cfg=new PluginCfg[total];
  int this_cfg_total=0;
  PluginCfg *proxy_cfg=new PluginCfg[total];
  int proxy_cfg_total=0;

  for(int i=0; i<total; i++)
    {
    PluginCfg cfg=values[i];
    if(strlen(cfg.name)>0&&cfg.name[0]=='_')
      this_cfg[this_cfg_total++]=cfg;
    else
      proxy_cfg[proxy_cfg_total++]=cfg;
    }
  res=ExtConfig.Set(this_cfg,this_cfg_total);

  //---- ×èòàåì íàñòðîéêè
  ReadSettings();
  Reinitialize();

  if(pPluginCfgSet&&isOn)
    return pPluginCfgSet(proxy_cfg,proxy_cfg_total);

  return res;
  }

int APIENTRY MtSrvPluginCfgGet(LPCSTR name,PluginCfg *value)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(strlen(name)>0&&name[0]=='_')
    return ExtConfig.Get(name,value);
  else if(pPluginCfgGet&&isOn)
    return pPluginCfgGet(name,value);

  return FALSE;
  }

int APIENTRY MtSrvPluginCfgNext(const int index,PluginCfg *value)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(index<ExtConfig.Total())
    return ExtConfig.Next(index,value);

  if(pPluginCfgNext&&isOn)
    return pPluginCfgNext(index-ExtConfig.Total(),value);

  return FALSE;
  }

int APIENTRY MtSrvPluginCfgDelete(LPCSTR name)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  int res=FALSE;
  if(strlen(name)>0&&name[0]=='_')
    {
    res=ExtConfig.Delete(name);
    //---- ×èòàåì íàñòðîéêè
    ReadSettings();
    Reinitialize();
    }
  else if(pPluginCfgDelete&&isOn)
    return pPluginCfgDelete(name);

  return res;
  }

int APIENTRY MtSrvPluginCfgTotal()
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pPluginCfgTotal&&isOn)
    return pPluginCfgTotal()+ExtConfig.Total();

  return ExtConfig.Total();
  }

int APIENTRY MtSrvPluginCfgLog(LPSTR value,int *len)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pPluginCfgLog&&isOn)
    return pPluginCfgLog(value,len);

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - notification about adding a new record in the log file.
|	  The hook is called when a new record is made in the server log file.
|
|	Parameters:
|	  info - record description
|
|	Important: If method LogsOut of the server interface is called from this hook, a recursion will be formed.
\********************************************************************************************************************************************/
void APIENTRY MtSrvLog(const LogInfo *info)
  {
  if(pLog&&isOn)
    pLog(info);
  }
/********************************************************************************************************************************************\
|	Purpose - launching of the own tasks according to the schedule on the server.
|	  The hook is regularly called by the server to launch external tasks according to the schedule.
|
|	Parameters:
|	  curtime - the current server time
|
|	Important: This hook must be executed within the least possible period of time. Otherwise, the can occur problems of launching tasks
|			   according to the schedule on the trade server. If the hook execution takes too long time, its logic is better to be placed in
|	           a separate thread.
\********************************************************************************************************************************************/
void APIENTRY MtSrvScheduler(const time_t curtime)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pScheduler&&isOn)
    pScheduler(curtime);
  }

/********************************************************************************************************************************************\
|	Purpose - getting information about the general system performance.
|	  The hook is called regularly at the interval of 5 minutes to get information about system performance.
|
|	Parameters:
|	  inf - information about general system performance
|
|	Important: This hook must be executed within the least possible period of time. Otherwise, the can occur problems of launching tasks
|	           according to the schedule on the trade server. If the hook execution takes too long time, its logic is better to be placed in
|	           a separate thread.
\********************************************************************************************************************************************/
void APIENTRY MtSrvPerformance(const PerformanceInfo *inf)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pPerformance&&isOn)
    pPerformance(inf);
  }

/********************************************************************************************************************************************\
|	Purpose - regular launch of the service logic on the trade server.
|	  The hook is regularly called by the server to launch the service logic. This hook can be used, for example, to control network
|	  operations and streams in the plugin.
|
|	Parameters:
|	  curtime - the current server time
|
|	Important: This hook must be executed within the least possible period of time. Otherwise, the can occur problems of launching tasks
|	           according to the schedule on the trade server. If the hook execution takes too long time, its logic is better to be placed in
|	           a separate thread.
\********************************************************************************************************************************************/
void APIENTRY MtSrvService(const DWORD curtime)
  {
  //logToMT4(CmdWarn, "Enter %s", __FUNCTION__);

  if(pService&&isOn)
    pService(curtime);
  }

/********************************************************************************************************************************************\
|	Purpose - filter connections from predefined IP addresses.
|	  The hook is called before creation the network context for a client. This hook can be used to prohibit connections from anonymous proxies.
|
|	Parameters:
|	  ip - IP address numerically
|	  ipstring - IP address as a string
|
|	Return values:
|	  TRUE - the default value - allow connection
|	  FALSE - block this connection
\********************************************************************************************************************************************/
int APIENTRY MtSrvUserConnect(const unsigned long ip,LPCSTR ipstring)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pUserConnect&&isOn)
    return pUserConnect(ip,ipstring);

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - filter connections by clients.
|	  The hook is called after the client has successfully authorized by the server. This hook can be used to filter the client's access to
|	  the server on basis of the own internal data.
|
|	Parameters:
|	  info - client info
|
|	Return values:
|	  TRUE - default value - allow login for this client
|	  FALSE - block the client's login, return "account disabled" (RET_ACCOUNT_DISABLED), and close the connection
\********************************************************************************************************************************************/
int APIENTRY MtSrvUserLogin(const UserInfo *info)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pUserLogin&&isOn)
    return pUserLogin(info);

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - notification on breaking client connection.
|
|	Parameters:
|	  info - client info
|	  type - identify the type of the terminal connected
\********************************************************************************************************************************************/
void APIENTRY MtSrvUserLogout(const UserInfo *info,const UCHAR type)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pUserLogout&&isOn)
    pUserLogout(info,type);
  }

/********************************************************************************************************************************************\
|	Purpose - filter connections by clients.
|	  The hook is called after the client has successfully authorized by the server. This hook can be used to filter the client's access to
|	  the server on basis of the own internal data.
|
|	Parameters:
|	  info - client info
|	  type - identify the type of the terminal connected
|
|	Return values:
|	  TRUE - default value - allow login for this client
|	  FALSE - block the client's login, return "account disabled" (RET_ACCOUNT_DISABLED), and close the connection
\********************************************************************************************************************************************/
int APIENTRY MtSrvUserLoginExt(const UserInfo *info,const UCHAR type)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pUserLoginExt&&isOn)
    return pUserLoginExt(info,type);

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - notify about creation of a demo account.
|	  Hook MtSrvUserDemo is called directly before creation of a demo account and can make changes in the account to be created or block the creation.
|
|	Parameters:
|	  info - account description
|
|	Return values:
|	  RET_OK - default value - allow opening of a demo account
|	  RET_ххх - error code to be returned to the client
\********************************************************************************************************************************************/
int APIENTRY MtSrvUserDemo(UserRecord *info)
  {
  logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pUserDemo&&isOn)
    return pUserDemo(info);

  return RET_OK;
  }

/********************************************************************************************************************************************\
|	This hook is out of date.
\********************************************************************************************************************************************/
int APIENTRY MtSrvUserCommand(const UserInfo *info,ULONG ip,const unsigned char command)
  {
  //logToMT4(CmdWarn,"Enter %s / Login: '%d' / Command: '%u'",__FUNCTION__,info->login,(unsigned int)command);

  if(pUserCommand&&isOn)
    return pUserCommand(info,ip,command);

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - intercept changing of the password by the user.
|	  The hook is intended for interception of changing the password. May be used for notification about  password modification.
|
|	Parameters:
|	  user - client's description, flag read_only means that this is an investor's connection
|	  password - new password
|	  change_investor - TRUE if the investor's password is changed, FALSE if not
|
|	Return values:
|	  RET_OK - allow changing of the password; for example, the investor can be allowed to change the master password
|	  RET_ххх - block password changing; for example, the master account is not allowed to change the investor's password.
|	            The error code is returned as the reason for rejection.
\********************************************************************************************************************************************/
int APIENTRY MtSrvUserChangePass(const UserInfo *info,LPCSTR password,const int change_investor)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pUserChangePass&&isOn)
    return pUserChangePass(info,password,change_investor);

  return RET_OK;
  }

/********************************************************************************************************************************************\
|	Purpose - intercept changing of the password by the user.
|	  The hook is intended for interception of changing the password. May be used for notification about  password modification.
|	  The hook notifies of the password change regardless of its source (client, manager, administrator terminal, Manager API and Server API).
|	  If the hook returns RET_OK, the password is changed. If any other value is returned, the password remains unchanged.
|
|	Parameters:
|	  user - client's description, flag read_only means that this is an investor's connection
|	  password - new password
|	  change_investor - TRUE if the investor's password is changed, FALSE if not
|
|	Return values:
|	  RET_OK - allow changing of the password; for example, the investor can be allowed to change the master password
|	  RET_ххх - block password changing; for example, the master account is not allowed to change the investor's password.
|	            The error code is returned as the reason for rejection.
\********************************************************************************************************************************************/
int APIENTRY MtSrvUserChangePassExt(const UserInfo *info,LPCSTR password,const int change_investor)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pUserChangePassExt&&isOn)
    return pUserChangePassExt(info,password,change_investor);

  return RET_OK;
  }

/********************************************************************************************************************************************\
|	Purpose - notify about adding a new account to the clients database.
|	  The server calls this hook after a new account has been successfully added to the clients database. This hook can be used to export
|	  accounts in the real-time mode.
|
|	Parameters:
|	  info - account description
\********************************************************************************************************************************************/
void APIENTRY MtSrvUserAdd(const UserRecord *info)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pUserAdd&&isOn)
    pUserAdd(info);
  }

/********************************************************************************************************************************************\
|	Purpose - notify about account changing.
|	  The server calls this hook after an account has been successfully changed in the clients database. The hook can be used to export
|	  clients' accounts in the real-time mode.
|
|	Parameters:
|	  info - account description
\********************************************************************************************************************************************/
void APIENTRY MtSrvUserUpdate(const UserRecord *info)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pUserUpdate&&isOn)
    pUserUpdate(info);
  }

/********************************************************************************************************************************************\
|	Purpose - notify about deletion of the account from the clients database.
|	  The server call this hook after the account has been successfully deleted from the clients database. The hook can be used to export
|	  clients' accounts in the real-time mode.
|
|	Parameters:
|	  info - account description
\********************************************************************************************************************************************/
void APIENTRY MtSrvUserDelete(const UserRecord *info)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pUserDelete&&isOn)
    pUserDelete(info);
  }

/********************************************************************************************************************************************\
|	Purpose - interception of the account transfer to the archive database.
|
|	Parameters:
|	  info - account description
|	  group - account group description
\********************************************************************************************************************************************/
int APIENTRY MtSrvUserArchive(const UserRecord *info,const ConGroup *group)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pUserArchive&&isOn)
    return pUserArchive(info,group);

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - notifies that a demo account has been deleted.
|
|	Parameters:
|	  info - account description
\********************************************************************************************************************************************/
void APIENTRY MtSrvUserCompactDemo(const UserRecord *info)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pUserCompactDemo&&isOn)
    pUserCompactDemo(info);
  }

/********************************************************************************************************************************************\
|	Purpose - notifies that an account has been moved to archive.
|
|	Parameters:
|	  info - account description
\********************************************************************************************************************************************/
void APIENTRY MtSrvUserCompactArchive(const UserRecord *info)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pUserCompactArchive&&isOn)
    pUserCompactArchive(info);
  }

/********************************************************************************************************************************************\
|	Purpose - notifies that an account has been moved to archive.
|
|	Parameters:
|	  info - account description
\********************************************************************************************************************************************/
void APIENTRY MtSrvUserRestore(const UserRecord *info)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pUserRestore&&isOn)
    pUserRestore(info);
  }

/********************************************************************************************************************************************\
|	Purpose - intercept daily interest calculation for free assets.
|	  The hook is intended for implementation of non-standard interest calculation for free assets.
|	  The server calculates interests for free assets daily. The MtSrvUserInterestDaily hook is called to change calculation logic.
|	  By default, interests gained by the client within the day are accumulated by the server in the interestrate field of the UserRecord.
|	  When the month ends, the server calls the MtSrvUserInterestMonth hook to actual entering interests in the client's account: the server
|	  makes the balance operation that places accumulated money from the interestrate field on the client's account.
|	  If the hook returns FALSE, the standard calculation method will be used.
|
|	Parameters:
|	  user - account description
|	  group - description of the group where the account is located
|	  freemargin - the current free margin of the client
|
|	Return values:
|	  FALSE - default value - use the standard logic to calculate interests
|	  TRUE - specific implementation of interests calculation is used
\********************************************************************************************************************************************/
int APIENTRY MtSrvUserInterestDaily(UserRecord *user,const ConGroup *group,const double freemargin)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pUserInterestDaily&&isOn)
    return pUserInterestDaily(user,group,freemargin);

  return FALSE;
  }

/********************************************************************************************************************************************\
|	Purpose - intercept monthly update of interests charged on free assets.
|	  The hook is intended for implementation of non-standard monthly updating of interests charged on free assets.
|	  The server calculates interests on free assets daily. The MtSrvUserInterestDaily hook is called to change calculation logic.
|	  By default, interests gained by the client within the day are accumulated by the server in the interestrate field of the UserRecord.
|	  The server calls the MtSrvUserInterestMonth hook when the month ends in order to actually enter interests in the client's account:
|	  the server makes the balance operation that places accumulated money from the interestrate field on the client's account.
|	  If the hook returns FALSE, the standard calculation method will be used.
|
|	Parameters:
|	  user - account description
|	  group - description of the group where the account is located
|
|	Return values:
|	  FALSE - default value - use the standard logic to calculate interests to be placed in the client's account
|	  TRUE - specific implementation of interests calculation to be placed in the client's account is used
\********************************************************************************************************************************************/
int APIENTRY MtSrvUserInterestMonth(UserRecord *user,const ConGroup *group)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pUserInterestMonth&&isOn)
    return pUserInterestMonth(user,group);

  return FALSE;
  }

/********************************************************************************************************************************************\
|	Purpose - This hook is called after the client balance has been corrected via the administration terminal, Manager API or Server API.
|	  The hook contains the object of the client entry, for which the balance has been corrected, as well as old and new balance values.
|	  The balance field is also included in the passed client entry. Until recently, this field has been filled with the old balance value.
|	  Now, the corrected balance value is specified there instead.
|
|	Parameters:
|	  user - account description
|	  prev_balance - previous balance
|	  new_balance - new balance
\********************************************************************************************************************************************/
void APIENTRY MtSrvUserFixBalance(const UserRecord *user,const double prev_balance,const double new_balance)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pUserFixBalance&&isOn)
    pUserFixBalance(user,prev_balance,new_balance);
  }

/********************************************************************************************************************************************\
|	Purpose - called after successful data read from datafeed (successful call of CFeederSource::Read() method)
|
|	Parameters:
|	  feed - ConFeeder description
|	  inf - Feed Data
\********************************************************************************************************************************************/
void APIENTRY MtSrvFeederData(const ConFeeder *feed,FeedData *inf)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pFeederData&&isOn)
    pFeederData(feed,inf);
  }

/********************************************************************************************************************************************\
|	Purpose - called after successful datafeeder started
|
|	Parameters:
|	  feed - ConFeeder description
\********************************************************************************************************************************************/
int APIENTRY MtSrvFeederStart(const ConFeeder *feed)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pFeederStart&&isOn)
    return pFeederStart(feed);

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - called after successful data read from datafeed (successful call of CFeederSource::Read() method)
|
|	Parameters:
|	  feed - ConFeeder description
\********************************************************************************************************************************************/
int APIENTRY MtSrvFeederCheck(const ConFeeder *feed,const DWORD curtime,const DWORD lasttime)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pFeederCheck&&isOn)
    pFeederCheck(feed,curtime,lasttime);

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - called after successful datafeeder stopped
|
|	Parameters:
|	  feed - ConFeeder description
\********************************************************************************************************************************************/
void APIENTRY MtSrvFeederStop(const ConFeeder *feed)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pFeederStop&&isOn)
    pFeederStop(feed);
  }

/********************************************************************************************************************************************\
|	Purpose - interception of the group adding/updating.
|	  The hook is called before one attempts to add/update a group.
|
|	Parameters:
|	  group - description of the group
|
|	Return values:
|	  TRUE - default value - allow adding/updating of the group
|	  FALSE - disable adding/updating of the group
\********************************************************************************************************************************************/
int APIENTRY MtSrvGroupsAdd(const ConGroup *group)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pGroupsAdd&&isOn)
    return pGroupsAdd(group);

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - interception of group deletion.
|	  The hook is called before one attempts to delete a group from the trade server.
|
|	Parameters:
|	  group - description of the group
|
|	Return values:
|	  TRUE - default value - allow deletion of the group
|	  FALSE - disable group deletion
\********************************************************************************************************************************************/
int APIENTRY MtSrvGroupsDelete(const ConGroup *group)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pGroupsDelete&&isOn)
    return pGroupsDelete(group);

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - interception of the symbol adding/updating.
|	  The hook is called before one attempts to add/update a symbol.
|
|	Parameters:
|	  symb - description of the symbol
|
|	Return values:
|	  TRUE - default value - allow adding/updating of the symbol
|	  FALSE - disable adding/updating of the symbol
\********************************************************************************************************************************************/
int APIENTRY MtSrvSymbolsAdd(const ConSymbol *symb)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pSymbolsAdd&&isOn)
    return pSymbolsAdd(symb);

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - interception of symbol deletion.
|	  The hook is called before one attempts to delete a symbol from the trade server.
|
|	Parameters:
|	  symb - description of the symbol
|
|	Return values:
|	  TRUE - default value - allow deletion of the symbol
|	  FALSE - disable symbol deletion
\********************************************************************************************************************************************/
int APIENTRY MtSrvSymbolsDelete(const ConSymbol *symb)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pSymbolsDelete&&isOn)
    return pSymbolsDelete(symb);

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - interception of the manager adding/updating.
|	  The hook is called before one attempts to add/update a manager.
|
|	Parameters:
|	  man - description of the manager
|
|	Return values:
|	  TRUE - default value - allow adding/updating of the manager
|	  FALSE - disable adding/updating of the manager
\********************************************************************************************************************************************/
int APIENTRY MtSrvManagersAdd(const ConManager *man)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pManagersAdd&&isOn)
    return pManagersAdd(man);

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - interception of manager deletion.
|	  The hook is called before one attempts to delete a manager from the trade server.
|
|	Parameters:
|	  man - description of the manager
|
|	Return values:
|	  TRUE - default value - allow deletion of the manager
|	  FALSE - disable manager deletion
\********************************************************************************************************************************************/
int APIENTRY MtSrvManagersDelete(const ConManager *man)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pManagersDelete&&isOn)
    return pManagersDelete(man);

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - filtering and modifying of the thread of incoming requests.
|	  The hook should be called before the request is enqueued to be processed. This hook can be used for rejection of any requests about
|	  working with pending orders. It can also be applied, for example, if there is a need to set forecedly the order expiration date under certain conditions.
|	  If the hook does not return RET_OK, the request will not be enqueued. The return code will be passed to the client as an error code.
|
|	Parameters:
|	  request - the client's request
|	  isdemo - flag of the request execution by automated dealer of the server
\********************************************************************************************************************************************/
int APIENTRY MtSrvTradeRequestFilter(RequestInfo *request,const int isdemo)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pTradeRequestFilter&&isOn)
    return pTradeRequestFilter(request,isdemo);

  return RET_OK;
  }

/********************************************************************************************************************************************\
|	Purpose - implementation of automated dealing plugins.
|	  The hook is called when the client's request is enqueued to be processed. The request received can be processed immediately and confirmed or
|	  rejected using the methods of CServerInterface::RequestsPrices, CServerInterface::RequestsConfirm, CServerInterface::RequestsReset, CServerInterface::RequestsRequote.
|	  If it takes some time to process a given request (for example, when connecting to an external server), the given order must be attached
|	  to the manager on behalf of which the plugin works (using the CServerInterface::RequestsLock method), and then the request must be processed in a separate thread.
|	  If the plugin leaves request untouched it will be left in request queue for processing by dealer.
|
|	Parameters:
|	  request - the client's request
|	  isdemo - flag of the request execution by automated dealer of the server
\********************************************************************************************************************************************/
void APIENTRY MtSrvTradeRequestApply(RequestInfo *request,const int isdemo)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pTradeRequestApply&&isOn)
    pTradeRequestApply(request,isdemo);
  }

/********************************************************************************************************************************************\
|	Purpose - called after client's request is returned to the queue
|	  This hook is called in case a client's request is returned to the queue, after a request has been requoted by a dealer, in the absence of the 'Fast IE confirmation' flag.
|
|	Parameters:
|	  request - the client's request
\********************************************************************************************************************************************/
void APIENTRY MtSrvTradeRequestRestore(RequestInfo *request)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pTradeRequestRestore&&isOn)
    pTradeRequestRestore(request);
  }

/********************************************************************************************************************************************\
|	Purpose - additional check of trading transactions
|	  The hook named MtSrvTradeTransaction is called after the client's or manager's trading transaction (position opening or closing, order
|	  modifying, pending order triggering, etc.) has been primarily checked.
|	  If this hook does not return RET_OK, the transaction will not be performed. If the operation succeeds, the number of the request that
|	  conducted this transaction will be placed in the request_id. Otherwise, the return code will be passed to the client as an error code.
|
|	Parameters:
|	  trans - transaction
|	  user - description of the trader's or manager's account that performs a transaction
|	  request_id - the request number
\********************************************************************************************************************************************/
int APIENTRY MtSrvTradeTransaction(TradeTransInfo *trans,const UserInfo *user,int *request_id)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pTradeTransaction&&isOn)
    return pTradeTransaction(trans,user,request_id);

  return RET_OK;
  }

/********************************************************************************************************************************************\
|	Purpose - managing validation of prices in Instant Execution mode
|	  If the hook returns RET_OK_NONE, the request is checked by default, if the hook returns RET_OK, the request is accepted without a check,
|	  if any other value is returned, the client receives a requote with the current market prices.
|
|	Parameters:
|	  trans - transaction
|	  user - description of the trader's or manager's account that performs a transaction
|	  symbol - ConSymbol relating to transaction
|	  prices - Bid[0] / Ask[1]
\********************************************************************************************************************************************/
int APIENTRY MtSrvTradeCheckIE(TradeTransInfo *trans,const UserInfo *user,const ConSymbol *symbol,double *prices)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pTradeCheckIE&&isOn)
    return pTradeCheckIE(trans,user,symbol,prices);

  return RET_OK_NONE;
  }

/********************************************************************************************************************************************\
|	Purpose - filtering of StopLoss and TakeProfit orders processing.
|	  The server calls this hook when a StopLoss or a TakeProfit order triggers. If this hook returns RET_OK, the order will be processed
|	  according to a standard procedure (by the server automated dealer or a human dealer). Otherwise, if the hook returns something other than RET_OK,
|	  the plugin will notify the server about that the former will process the order independently (or that the order will not be processed at all).
|
|	Implementation of this hook must be optimized by time, otherwise the processing of StopLoss and TakeProfit orders may be considerably delayed.
|	Inside of this hook, the methods of server interface named CServerInterface::MtSrvTradesAdd and CServerInterface::MtSrvTradesUpdate should
|	not be called in order to add/remove orders within the same group of users.
|
|	Parameters:
|	  group - description of the group
|	  symbol - description of the symbol
|	  trade - trade position
\********************************************************************************************************************************************/
int APIENTRY MtSrvTradeStopsFilter(const ConGroup *group,const ConSymbol *symbol,const TradeRecord *trade)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pTradeStopsFilter&&isOn)
    return pTradeStopsFilter(group,symbol,trade);

  return RET_OK_NONE;
  }

/********************************************************************************************************************************************\
|	Purpose - interception of StopLoss and TakeProfit orders processing.
|	  This hook is intended for interception and modifying of parameters of the position order to be executed (for example, the position close price).
|	  It is called after the necessary information (trade) for execution of the Stop order has been prepared.
|	  If this hook returns RET_OK, the order is executed by the server automated dealer considering changes made by the hook. Otherwise, the order will not be processed.
|
|	Implementation of this hook must be optimized by time, otherwise the processing of StopLoss and TakeProfit orders may be considerably delayed.
|	Inside of this hook, the methods of server interface named CServerInterface::MtSrvTradesAdd and CServerInterface::MtSrvTradesUpdate should
|	not be called in order to add/remove orders within the same group of users.
|
|	Parameters:
|	  user - description of the account
|	  group - description of the group to which the account belongs
|	  symbol - description of the symbol
|	  trade - position to be closed
|	  isTP - flag showing that TakeProfit is being processed (TRUE), otherwise (FALSE) the closing is performed by StopLoss
\********************************************************************************************************************************************/
int APIENTRY MtSrvTradeStopsApply(const UserInfo *user,const ConGroup *group,const ConSymbol *symbol,TradeRecord *trade,const int isTP)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pTradeStopsApply&&isOn)
    return pTradeStopsApply(user,group,symbol,trade,isTP);

  return RET_OK;
  }

/********************************************************************************************************************************************\
|	Purpose - filtering of the pending order activation.
|	  The server calls this hook when pending orders are activated. If this hook returns RET_OK, the order will be processed according to
|	  the standard procedure (by the server automated dealer or by a human dealer). Otherwise, if the hook returns something other than RET_OK,
|	  the plugin notifies the server that the plugin will process this order independently (or that the order will not be processed at all).
|
|	Implementation of this hook must be optimized by time, otherwise the pending order activation may be considerably delayed.
|	Inside of this hook, the methods of server interface named CServerInterface::MtSrvTradesAdd and CServerInterface::MtSrvTradesUpdate should
|	not be called in order to add/remove orders within the same group of users.
|
|	Parameters:
|	  group - description of the group
|	  symbol - description of the symbol
|	  trade - pending order
\********************************************************************************************************************************************/
int APIENTRY MtSrvTradePendingsFilter(const ConGroup *group,const ConSymbol *symbol,const TradeRecord *trade)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pTradePendingsFilter&&isOn)
    return pTradePendingsFilter(group,symbol,trade);

  return RET_OK_NONE;
  }

/********************************************************************************************************************************************\
|	Purpose - interception of the pending order activation.
|	  This hook is intended for interception and modifying of the order to be executed (for example, the position open price) and is called after
|	  the necessary information (trade) has been prepared for activation of the initial pending order.
|	  If this hook returns RET_OK, the order is executed by the server automated dealer considering the changes made by the hook. Otherwise, the order will not be processed.
|
|	Implementation of this hook must be optimized by time, otherwise the pending order activation may be considerably delayed.
|	Inside of this hook, the methods of server interface named CServerInterface::MtSrvTradesAdd and CServerInterface::MtSrvTradesUpdate should
|	not be called in order to add/remove orders within the same group of users.
|
|	Parameters:
|	  user - description of the account
|	  group - description of the group to which the account belongs
|	  symbol - description of the symbol
|	  pending - the initial pending order
|	  trade - the position to be opened
\********************************************************************************************************************************************/
int APIENTRY MtSrvTradePendingsApply(const UserInfo *user,const ConGroup *group,const ConSymbol *symbol,const TradeRecord *pending,TradeRecord *trade)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pTradePendingsApply&&isOn)
    return pTradePendingsApply(user,group,symbol,pending,trade);

  return RET_OK;
  }

/********************************************************************************************************************************************\
|	Purpose - filtering of stopuots processing.
|	  The server calls this hook when positions are being closed by stopout. If this hook returns RET_OK, the position will be closed according
|	  to the standard procedure using the server automated dealer. Otherwise (the hook returns something other than RET_OK), the plugin notifies
|	  the server that the plugin will close the positions by stopouts independently (or the positions will not be closed at all).
|
|	Implementation of this hook must be optimized by time, otherwise the processing of stops and pending orders may be considerably delayed.
|	Inside of this hook, the methods of server interface named CServerInterface::MtSrvTradesAdd and CServerInterface::MtSrvTradesUpdate should
|	not be called in order to add/remove orders within the same group of users.
|
|	Parameters:
|	  group - description of the group
|	  symbol - description of the symbol
|	  login - the account number
|	  equity - the current funds
|	  margin - the current margin
\********************************************************************************************************************************************/
int APIENTRY MtSrvTradeStopoutsFilter(const ConGroup *group,const ConSymbol *symbol,const int login,const double equity,const double margin)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pTradeStopoutsFilter&&isOn)
    return pTradeStopoutsFilter(group,symbol,login,equity,margin);

  return RET_OK_NONE;
  }

/********************************************************************************************************************************************\
|	Purpose - interception of position closing by stopout
|	  This hook is intended for interception and modifying of parameters of positions closed by stopout (for example, close price or comments).
|	  It is called after the necessary information (stopout) for pending order activation has been prepared.
|	  If this hook returns RET_OK, the position will be closed by automated dealer considering changes made by the hook. Otherwise, the position remains open.
|
|	Implementation of this hook must be optimized by time, otherwise the processing of stops and pending orders may be considerably delayed.
|	Inside of this hook, the methods of server interface named CServerInterface::MtSrvTradesAdd and CServerInterface::MtSrvTradesUpdate should
|	not be called in order to add/remove orders within the same group of users.
|
|	Parameters:
|	  user - description of the account
|	  group - description of the group to which the account belongs
|	  symbol - description of the symbol
|	  stopout - the position to be closed by stopout
\********************************************************************************************************************************************/
int APIENTRY MtSrvTradeStopoutsApply(const UserInfo *user,const ConGroup *group,const ConSymbol *symbol,TradeRecord *stopout)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pTradeStopoutsApply&&isOn)
    return pTradeStopoutsApply(user,group,symbol,stopout);

  return RET_OK;
  }

/********************************************************************************************************************************************\
|	Purpose - interception of adding of the order to the database.
|	  The hook is called immediately after the new trade record has been added to the server database. This opportunity can be used to trace the trade records on certain accounts.
|
|	Parameters:
|	  trade - the trade record to be added
|	  user - description of the account
|	  symbol - description of the symbol
\********************************************************************************************************************************************/
void APIENTRY MtSrvTradesAdd(TradeRecord *trade,const UserInfo *user,const ConSymbol *symb)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pTradesAdd&&isOn)
    pTradesAdd(trade,user,symb);
  }

/********************************************************************************************************************************************\
|	Purpose - interception of adding of the order to the database.
|	  The hook is called immediately after the new trade record has been successfully added to the server database.
|	  This opportunity can be used to trace the changes made in the trade records on certain accounts. Unlike the MtSrvTradesAdd,
|	  this hook additionally passes the mode flag that gives reasons for which the trade record has been added.
|
|	Parameters:
|	  trade - the record
|	  user - flag of the account decription
|	  symbol - flag of the symbol description
|	  mode - the reason why the trade record was added:
|		  OPEN_NEW - adding of a new order/position,
|		  OPEN_CLOSE - adding of the rest of a closed position,
|		  OPEN_RESTORE - restoring of the record from backup,
|		  OPEN_API - opening of position using API,
|		  OPEN_ROLLOVER - opening of position by rollover
\********************************************************************************************************************************************/
void APIENTRY MtSrvTradesAddExt(TradeRecord *trade,const UserInfo *user,const ConSymbol *symb,const int mode)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pTradesAddExt&&isOn)
    pTradesAddExt(trade,user,symb,mode);
  }

/********************************************************************************************************************************************\
|	Purpose - for integration with external trading systems
|	  This hook is called similar to MtSrvTradesAdd - when an order is successfully added to the server database.
|	  But if the order is added due to execution of a request, the structure of an appropriate request is to be passed as request parameter.
|	  If adding an order is not related to a trading request (during automatic processing by the server, dealer trading, correction by an administrator or plugin, etc.), NULL is passed in request parameter.
|	  This hook allows users to clearly match the ticket of the newly placed order with the request, after confirmation of which that order has been generated.
|	  This is useful when integrating MetaTrader 4 with external trading systems.
|
|	Parameters:
|	  trade - the trade record to be added
|	  request - the client's request
|	  user - description of the account
|	  symbol - description of the symbol
|	  mode - ????
\********************************************************************************************************************************************/
void APIENTRY MtSrvTradesAddRequest(TradeRecord *trade,const RequestInfo *request,const UserInfo *user,const ConSymbol *symb,const int mode)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pTradesAddRequest&&isOn)
    pTradesAddRequest(trade,request,user,symb,mode);
  }

/********************************************************************************************************************************************\
|	Purpose - interception of the trade record changes in the database.
|	  The hook is called immediately after the new trade record has been successfully added to the server database.
|	  This opportunity can be used to trace the changes made in the trade records on certain accounts.
|
|	Parameters:
|	  trade - the trade record to be added
|	  user - description of the account
|	  symbol - flag of the symbol description
|	  mode - the reason for update:
|		  UPDATE_NORMAL - normal update,
|		  UPDATE_ACTIVATE - order activation,
|		  UPDATE_CLOSE - closing of the order,
|		  UPDATE_DELETE - deletion of the order
\********************************************************************************************************************************************/
void APIENTRY MtSrvTradesUpdate(TradeRecord *trade,UserInfo *user,const int mode)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pTradesUpdate&&isOn)
    pTradesUpdate(trade,user,mode);
  }

/********************************************************************************************************************************************\
|	Purpose - notification of a trade record restoration from the backup or archive.
|
|	Parameters:
|	  trade - the trade record being restored
\********************************************************************************************************************************************/
void APIENTRY MtSrvTradesRestore(const TradeRecord *trade)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pTradesRestore&&isOn)
    pTradesRestore(trade);
  }

/********************************************************************************************************************************************\
|	Purpose - interception of close by
|	  There is a hook for close by that gives you the first order, the second order and the new remainder order (if a remainder doesn't exist it gives NULL).
|
|	Parameters:
|	  ftrade - first trade
|	  strader - second trade
|	  remaind - new residual order (if any, otherwise NULL)
|	  sec - flag of the symbol description
|	  user - description of the account
\********************************************************************************************************************************************/
void APIENTRY MtSrvTradesCloseBy(TradeRecord *ftrade,TradeRecord *strade,TradeRecord *remaind,ConSymbol *sec,UserInfo *user)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pTradesCloseBy&&isOn)
    pTradesCloseBy(ftrade,strade,remaind,sec,user);
  }

/********************************************************************************************************************************************\
|	Purpose - interception of commission calculation at opening of a new position.
|	  This hook should be used to implement your own algorithms of commission calculation. When this hook is called by the server,
|	  the commission calculated by default is set in the commission field of the trade parameter.
|
|	Parameters:
|	  trade - the position to be opened
|	  symbol - description of the symbol
|	  group - description of the group to which the account belongs
\********************************************************************************************************************************************/
void APIENTRY MtSrvTradeCommission(TradeRecord *trade,const ConSymbol *sec,const ConGroup *grp)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pTradeCommission&&isOn)
    pTradeCommission(trade,sec,grp);
  }

/********************************************************************************************************************************************\
|	Purpose - interception of agent commission calculation at position closing.
|	  This hook should be used to implement your own algorithms of agent commission calculation. When this hook is called by the server,
|	  the commission value calculated by default is set in the commission_agent field of the trade parameter.
|
|	Parameters:
|	  trade - the position to be opened
|	  sec - description of the security
|	  user - description of a client
|
|	Return values:
|	  TRUE - default value - the agent commission is charged.
|	  FALSE - Otherwise, the agent commission is zeroed and the balance operation to an agent's account is not performed.
\********************************************************************************************************************************************/
int APIENTRY MtSrvTradeCommissionAgent(TradeRecord *trade,const ConSymbol *sec,const UserInfo *user)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pTradeCommissionAgent&&isOn)
    return pTradeCommissionAgent(trade,sec,user);

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - interception of swaps calculation when the position rolls over the end of trading day
|	  This hook should be used to implement your own of swap calculation algorithms. When this hook is called by the server,
|	  the swap value calculated by default is passed in the value parameter.
|
|	Parameters:
|	  trade - the trade position
|	  value - the precalculated value of the daily swap
|	  data - the structure of the swap calculation parameters for the given symbol
|
|	Return values:
|	  FALSE - default value - the accumulated swap for a position (the storage field of the trade parameter) will be increased by the value specified in the value parameter.
|	  TRUE - Otherwise, the hook must update the value of the storage field.
\********************************************************************************************************************************************/
int APIENTRY MtSrvTradeRollover(TradeRecord *trade,const double value,const OverNightData *data)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pTradeRollover&&isOn)
    return pTradeRollover(trade,value,data);

  return FALSE;
  }

/********************************************************************************************************************************************\
|	Purpose - notification about completion of daily/monthly reports generation on the server for all clients.
|	  The hook is called after the reports have been generated on the trade server, immediately before they are added to the daily reports database.
|
|	Parameters:
|	  month_state - flag of monthly reports (TRUE - monthly report, FALSE - daily report)
|	  groups - groups, for which the report has been generated
|	  groups_total - the total amount of group
|	  reports - reports about clients
|	  reports_total - the total amount of reports about clients
\********************************************************************************************************************************************/
void APIENTRY MtSrvStatementsBaseAll(const int month_state,const GroupState *groups,const int groups_total,const DailyReport *reports,const int reports_total)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pStatementsBaseAll&&isOn)
    pStatementsBaseAll(month_state,groups,groups_total,reports,reports_total);
  }

/********************************************************************************************************************************************\
|	Purpose - notification about completion of the daily or monthly report generation for the client.
|	  The hook is called after the report for one client has been generated on the trade server.
|
|	Parameters:
|	  month_state - falg of daily reports (TRUE - monthly report, FALSE - daily report)
|	  user - description of the client the report has been generated for
|	  group - the trade group the client belongs to
|	  first_trade - this parameter points at the leader of trades for a client
|	  last_trade - this parameter points at the end of the array containing trades for a client
|	  state - internal structure of the report for a client
\********************************************************************************************************************************************/
void APIENTRY MtSrvStatementsUser(const int month_state,const UserRecord *user,const ConGroup *group,TradeRecord *first_trade,TradeRecord *last_trade,StateReport *state)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pStatementsUser&&isOn)
    pStatementsUser(month_state,user,group,first_trade,last_trade,state);
  }

/********************************************************************************************************************************************\
|	Purpose - provides ability to create some special activitiy right after rollovers having been processed.
|
|	Parameters:
|	  month_state - falg of daily reports (TRUE - monthly report, FALSE - daily report)
|	  ctm - ???
|	  ttm - ???
\********************************************************************************************************************************************/
void APIENTRY MtSrvStatementsRollovers(const int month_state,const time_t ctm,const tm *ttm)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pStatementsRollovers&&isOn)
    pStatementsRollovers(month_state,ctm,ttm);
  }

/********************************************************************************************************************************************\
|	Purpose - notification on a dealer's connection from a trade server.
|
|	Parameters:
|	  manager - manager object
|
|	Return values:
|	  TRUE - default value - allow dealer to connect
|	  FALSE - Otherwise, don't allow
\********************************************************************************************************************************************/
int APIENTRY MtSrvDealerConnect(const ConManager *manager)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pDealerConnect&&isOn)
    return pDealerConnect(manager);

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - notification on a dealer's disconnection from a trade server.
|
|	Parameters:
|	  manager - manager object
\********************************************************************************************************************************************/
void APIENTRY MtSrvDealerDisconnect(const ConManager *manager)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pDealerDisconnect&&isOn)
    pDealerDisconnect(manager);
  }

/********************************************************************************************************************************************\
|	Purpose - interception of the dealer's getting the request from queue.
|	  The hook is called when a dealer attempts to take a request from queue to process it.
|
|	Parameters:
|	  manager - manager description
|	  request - description of the request the manager wants to take from queue
|
|	Return values:
|	  TRUE - default value - allow dealer to take the request for processing
|	  FALSE - prohibit dealer to take the request for processing
|
|	Important: The server interface methods of working with clients' requests must be called from this hook with much care keeping in mind the possibility of forming a recursion.
\********************************************************************************************************************************************/
int APIENTRY MtSrvDealerGet(const ConManager *manager,const RequestInfo *request)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pDealerGet&&isOn)
    return pDealerGet(manager,request);

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - interception of the dealer's quoting.
|	  The hook is called when a dealer replies to the quoting request.
|
|	Parameters:
|	  id - the current server time
|	  us - description of the dealer
|	  prices - dealer's prices
|	  in_stream - flag of the dealer's price throwing in the stream (TRUE - prices are being thrown in, FALSE - prices are not being thrown in)
|
|	Return values:
|	  TRUE - default value - confirm dealer's reply
|	  FALSE - reject dealer's reply, the client receiving "Trade disabled" (RET_TRADE_DISABLE)
|
|	Important: The server interface methods of working with clients' requests must be called from this hook with much care keeping in mind the possibility of forming a recursion.
\********************************************************************************************************************************************/
int APIENTRY MtSrvDealerPrice(const int id,const UserInfo *us,double *prices,const int in_stream)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  /*if(pDealerPrice&&isOn)
    return pDealerPrice(id,us,prices,in_stream);*/

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - interception of the dealer's confirmation of request.
|	  The hook is called when the dealer confirms the request.
|
|	Parameters:
|	  id - request number
|	  us - description of the dealer
|	  prices - dealer's prices
|
|	Return values:
|	  TRUE - default value - confirm the dealer's reply
|	  FALSE - reject the dealer's reply, the client receiving "Trade disabled" (RET_TRADE_DISABLE)
|
|	Important: The server interface methods of working with clients' requests must be called from this hook with much care keeping in mind the possibility of forming a recursion.
\********************************************************************************************************************************************/
int APIENTRY MtSrvDealerConfirm(const int id,const UserInfo *us,double *prices)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  /*if(pDealerConfirm&&isOn)
    return pDealerConfirm(id,us,prices);*/

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - interception of the delaer's requoting.
|	  The hook is called when the request is requoted by the dealer.
|
|	Parameters:
|	  id - request number
|	  us - description of the dealer
|	  prices - dealer's new prices
|	  in_stream - flag of the dealer's prices throwing in the stream (TRUE - prices are being thrown in, FALSE - prices are not being thrown in)
|
|	Return values:
|	  TRUE - default value - confirm the dealer's reply
|	  FALSE - reject the dealer's reply, the client receiving "Trade disabled" (RET_TRADE_DISABLE)
|
|	Important: The server interface methods of working with clients' requests must be called from this hook with much care keeping in mind the possibility of forming a recursion.
\********************************************************************************************************************************************/
int APIENTRY MtSrvDealerRequote(const int id,const UserInfo *us,double *prices,const int in_stream)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  /*if(pDealerRequote&&isOn)
    return pDealerRequote(id,us,prices,in_stream);*/

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - interception of the dealer's reject to execute the request.
|	  The hook is called when the dealer rejects to execute the request.
|
|	Parameters:
|	  curtime - request number
|	  us - description of the dealer
|	  flag - flag of the reject reason
|
|	Return values:
|	  TRUE - default value - confirm dealer's reply
|	  FALSE - reject dealer's reply, the client receiving "Trade disabled" (RET_TRADE_DISABLE)
|
|	Important: The server interface methods of working with clients' requests must be called from this hook with much care keeping in mind the possibility of forming a recursion.
\********************************************************************************************************************************************/
int APIENTRY MtSrvDealerReset(const int id,const UserInfo *us,const char flag)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  /*if(pDealerReset&&isOn)
    return pDealerReset(id,us,flag);*/

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - The hook intercepts a client's cancellation of a request placed earlier.
|
|	Parameters:
|	  id - request number
|	  us - description of the dealer
|
|	Return values:
|	  TRUE - default value - allow customer to cancel
|	  FALSE - reject client's reply, to cancel
|
|	Important: The server interface methods of working with clients' requests must be called from this hook with much care keeping in mind the possibility of forming a recursion.
\********************************************************************************************************************************************/
int APIENTRY MtSrvDealerClientReject(const int id,const UserInfo *us)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  /*if(pDealerClientReject&&isOn)
    return pDealerClientReject(id,us);*/

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - filtering of quotes thrown into the price stream on the trade server.
|	  The hook is called when a new quote is thrown into the price stream.
|
|	Parameters:
|	  symbol - symbol description
|	  inf - quote description
|
|	Return values:
|	  TRUE - default value - allow throwing the quotes into the price stream. At that the quotes are additionally verified with standard checks on the server (filtration).
|	  FALSE - disable throwing the quotes into the price stream
|
|	Important: Calling for method HistoryAddTick from this hook will form a recursion.
\********************************************************************************************************************************************/
int APIENTRY MtSrvHistoryTickFilter(const ConSymbol *symbol,FeedTick *inf)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pHistoryTickFilter&&isOn)
    return pHistoryTickFilter(symbol,inf);

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - notification about adding a new quote to the price stream of the trade server.
|	  the hook is called after a new quote has been added to the price stream as a result of filtering and multiple checks.
|
|	Parameters:
|	  symbol - symbol description
|	  inf - quote description
|
|	Important: Calling for method HistoryAddTick from this hook will form a recursion.
\********************************************************************************************************************************************/
void APIENTRY MtSrvHistoryTickApply(const ConSymbol *symbol,FeedTick *inf)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pHistoryTickApply&&isOn)
    pHistoryTickApply(symbol,inf);
  }

/********************************************************************************************************************************************\
|	Purpose - runs when click 'correct history' in MT Admin
\********************************************************************************************************************************************/
int APIENTRY MtSrvHistoryCheck(const ConSymbol *symbol,RateInfo *rates,int *total)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pHistoryCheck&&isOn)
    return pHistoryCheck(symbol,rates,total);

  return TRUE;
  }

/********************************************************************************************************************************************\
|	Purpose - interception of commands received via Telnet
|	  The hook is called when the trade server receives an unknown Telnet command. The pointer at the client's request buffer is passed to the hook.
|	  If the plugin can reply to this request, the hook must place the reply in the same buffer and return the size of its reply.
|
|	Parameters:
|	  ip - IP address of the remote connection
|	  buf - buffer with the client's request also used for reply
|	  maxlen - maximal size of the buffer
|
|	Return values:
|	  0 - default value - the plugin cannot process the request
|	  >0 - size of the reply to be sent to the client and placed in the buf
\********************************************************************************************************************************************/
int APIENTRY MtSrvTelnet(const ULONG ip,char *buf,const int maxlen)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pTelnet&&isOn)
    return pTelnet(ip,buf,maxlen);

  return 0;
  }

/********************************************************************************************************************************************\
|	Purpose - interception of commands received via Telnet
|	  the possibility to send its own buffer (selected using the HEAP_ALLOC macros) as a response to a Telnet command.
|
|	Parameters:
|	  ip - IP address of the remote connection
|	  buf - buffer with the client's request also used for reply
|	  data_len - size of the buffer
|
|	Return values:
|	  data - data_len must be defined
\********************************************************************************************************************************************/
char *APIENTRY MtSrvTelnetExt(const ULONG ip,char *buf,int *data_len)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);

  if(pTelnetExt&&isOn)
    return pTelnetExt(ip,buf,data_len);

  *data_len=0;
  return NULL;
  }

/********************************************************************************************************************************************\
|	Purpose - provides the ability to extend protocol between MetaTrader Server and MetaTrader Manager API.
|
|	Parameters:
|	  ip - IP address of the remote connection
|	  buf - buffer with the client's request also used for reply
|	  data_len - size of the buffer
|
|	Return values:
|	  data - data_len must be defined
\********************************************************************************************************************************************/
int APIENTRY MtSrvManagerProtocol(const ULONG ip,const UserInfo *us,const unsigned char *in_data,const int in_size,unsigned char **out_data,int *out_size)
  {
  //logToMT4(CmdWarn,"Enter %s",__FUNCTION__);
  // http://www.cplusplus.com/forum/windows/53113/

  if((MAX_BUFFER-1)<in_size)
    return FALSE;

  char Buffer[MAX_BUFFER]={ 0 };// = new char[in_size+1]; 
  memcpy_s(Buffer,MAX_BUFFER-1,in_data,in_size);
  // Buffer[in_size] = '\0';
  std::string request(Buffer);
  // std::string request(Buffer); 
  // delete [] Buffer;


  std::vector<std::string> args;
  split(request,"|",args);

  if(args[0]=="idc-proxy-onoff")
    {
    //sym.FromArgs(args);

    PluginCfg cfg={ 0 };
    strcpy(cfg.name,"_On");
    strcpy(cfg.value,args[1].c_str());

    int res=ExtConfig.Add(&cfg);
    //---- ×èòàåì íàñòðîéêè
    ReadSettings();
    Reinitialize();

    /* return success message to FixServer */
    *out_data=(unsigned char *)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,5);
    *out_size=5;
    strcpy((char *)*out_data,"done");

    return TRUE;
    }
  else if(pManagerProtocol&&isOn)
    return pManagerProtocol(ip,us,in_data,in_size,out_data,out_size);

  /* Unknown message, return 0 */
  return FALSE;
  }