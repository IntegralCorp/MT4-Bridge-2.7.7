//+------------------------------------------------------------------+
//|                                            MetaTrader Server API |
//|                   Copyright 2001-2014, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//---
#include "resource.h"
#include "version.h"
//#define _USE_32BIT_TIME_T
//#define _WINSOCK_DEPRECATED_NO_WARNINGS

//--- exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN
//+------------------------------------------------------------------+
//|Response database                                                 |
//+------------------------------------------------------------------+
#define SLTPBaseFile             "request.dat"
#define SLTPBaseHeaderName       "REQUEST"
#define SLTPBaseHeaderVersion    500
//---
#include <windows.h>
#include <ctime>
#include <time.h>
#include <stdio.h>
#include <map>
#include <iomanip>
#include <locale>
//---	server API
#include "include\MT4ServerAPI.h"
//---
//#include "FIX\Sorter.h"
#include "API\MT4APITypes.h"
#include "API\MT4APITools.h"
//---
#include "IntegralAPI\Bases\BasesTools.h"
//---
#include "Common\Const.h"
//--- quickfix
#include "QuickFix\MessageCracker.h"
#include "QuickFix\SessionSettings.h"
#include "QuickFix\SessionID.h"
#include "QuickFix\fix43\MarketDataSnapshotFullRefresh.h"
#include "QuickFix\fix43\OrderStatusRequest.h"
#include "QuickFix\fix43\MarketDataRequest.h"
#include "QuickFix\fix43\BusinessMessageReject.h"
#include "QuickFix\fix43\MarketDataRequestReject.h"
#include "QuickFix\fix43\TradeCaptureReport.h"
#include "QuickFix\fix43\Reject.h"
//--- cache

//---
#include "Fix\IdcLog.h"
#include "IO\Log.h"
//---
#include "Tools\smart_ptr.h"
#include "Common\Logger.h"
#include "Tools\StringRef.h"
#include "Tools\StringFile.h"
//#include "Common\Config.h"
#include "Common\PluginContext.h"
#include "Common\StringTokenizer.h"
//---
//#include "IntegralAPI\Common\SymbolBase.h"
//#include "IntegralAPI\Common\IntegralMsgExecution.h"
//#include "IntegralAPI\Common\IntegralMsgOrder.h"
//---
//#include "FIX\Sorter.h"
//#include "IO\Log.h"
//#include "Queue\TradeResponse.h"
#include "Risk\Ack.h"
//#include "Risk\CloudWatch.h"
//#include "MetaQuotes\ManagerMgr.h"
#include "Util\threadmap.h"
//#include "Util\AtomicUtil.h"

//#include "Util\DebugUtil.h"
//#include "IntegralAPI\Contexts\FixClient.h"
//#include "Risk\WatchTower.h"
//#include "Util\AutoFree.h"
//#include "Util\AutoHeapFree.h"
//#include "Util\ThreadMon.h"
//#include "IntegralGateway.h"

//---
//#include "resource.h"
//--- Exchange API
#include "IntegralAPI\IntegralAPI.h"
//+------------------------------------------------------------------+

//--- link to plugin description
extern PluginInfo        ExtPluginInfo;
//--- macros
#define TERMINATE_STR(str) str[sizeof(str)-1]=0;
#define COPY_STR(dst,src) { strncpy(dst,src,sizeof(dst)-1); dst[sizeof(dst)-1]=0; }
//+------------------------------------------------------------------+
//| Module descriptor (from DllMain)                                 |
//+------------------------------------------------------------------+
extern HANDLE ExtModule;
//+------------------------------------------------------------------+
//| Module descriptor (from DllMain)                                 |
//+------------------------------------------------------------------+
//extern FixClient	*fixClient;
//extern WatchTower	*watchTower;