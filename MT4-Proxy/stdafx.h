// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "version.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <math.h>
#include <winsock2.h>
#include <process.h>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <string>
//---
#include "include\MT4ServerAPI.h"
#include "config\Configuration.h"
#include "config\stringfile.h"
//#include "config\CAutoHeapFree.h"

//+------------------------------------------------------------------+
//| Plugin information                                               |
//+------------------------------------------------------------------+
typedef void (APIENTRY *mAbout)(PluginInfo *info);
typedef int  (APIENTRY *mStartup)(CServerInterface *server);
typedef void (APIENTRY *mCleanup)(void);
//+------------------------------------------------------------------+
//| Plugin configuration                                             |
//+------------------------------------------------------------------+
typedef int  (APIENTRY *mPluginCfgAdd)(const PluginCfg *value);
typedef int  (APIENTRY *mPluginCfgSet)(const PluginCfg *values,const int total);
typedef int  (APIENTRY *mPluginCfgGet)(LPCSTR name,PluginCfg *value);
typedef int  (APIENTRY *mPluginCfgNext)(const int index,PluginCfg *value);
typedef int  (APIENTRY *mPluginCfgDelete)(LPCSTR name);
typedef int  (APIENTRY *mPluginCfgTotal)();
typedef int  (APIENTRY *mPluginCfgLog)(LPSTR value,int *len);
//+------------------------------------------------------------------+
//| Auxiliary Server Hook Functions                                  |
//+------------------------------------------------------------------+
typedef void (APIENTRY *mLog)(const LogInfo *info);
typedef void (APIENTRY *mScheduler)(const time_t curtime);
typedef void (APIENTRY *mPerformance)(const PerformanceInfo *inf);
typedef void (APIENTRY *mService)(const DWORD curtime);
//+------------------------------------------------------------------+
//| Client base Hook Functions                                       |
//+------------------------------------------------------------------+
typedef int  (APIENTRY *mUserConnect)(const unsigned long ip,LPCSTR ipstring);
typedef int  (APIENTRY *mUserLogin)(const UserInfo *info);
typedef void (APIENTRY *mUserLogout)(const UserInfo *info,const UCHAR type);
typedef int  (APIENTRY *mUserLoginExt)(const UserInfo *info,const UCHAR type);
typedef int  (APIENTRY *mUserDemo)(UserRecord *info);
typedef int  (APIENTRY *mUserCommand)(const UserInfo *info,ULONG ip,const unsigned char command);
typedef int  (APIENTRY *mUserChangePass)(const UserInfo *info,LPCSTR password,const int change_investor);
typedef int  (APIENTRY *mUserChangePassExt)(const UserInfo *info,LPCSTR password,const int change_investor);
typedef void (APIENTRY *mUserAdd)(const    UserRecord *info);
typedef void (APIENTRY *mUserUpdate)(const UserRecord *info);
typedef void (APIENTRY *mUserDelete)(const UserRecord *info);
typedef int  (APIENTRY *mUserArchive)(const UserRecord *info,const ConGroup *group);
typedef void (APIENTRY *mUserCompactDemo)(const UserRecord *info);
typedef void (APIENTRY *mUserCompactArchive)(const UserRecord *info);
typedef void (APIENTRY *mUserRestore)(const UserRecord *info);
typedef int  (APIENTRY *mUserInterestDaily)(UserRecord *user,const ConGroup *group,const double freemargin);
typedef int  (APIENTRY *mUserInterestMonth)(UserRecord *user,const ConGroup *group);
typedef void (APIENTRY *mUserFixBalance)(const UserRecord *user,const double prev_balance,const double new_balance);
//+------------------------------------------------------------------+
//| Datafeed Hook Functions                                          |
//+------------------------------------------------------------------+
typedef void (APIENTRY *mFeederData)(const  ConFeeder *feed,FeedData *inf);
typedef int  (APIENTRY *mFeederStart)(const ConFeeder *feed);
typedef int  (APIENTRY *mFeederCheck)(const ConFeeder *feed,const DWORD curtime,const DWORD lasttime);
typedef void (APIENTRY *mFeederStop)(const  ConFeeder *feed);
//+------------------------------------------------------------------+
//| Configuration Hook Functions                                     |
//+------------------------------------------------------------------+
typedef int  (APIENTRY *mGroupsAdd)(const ConGroup *group);
typedef int  (APIENTRY *mGroupsDelete)(const ConGroup *group);
typedef int  (APIENTRY *mSymbolsAdd)(const ConSymbol *symb);
typedef int  (APIENTRY *mSymbolsDelete)(const ConSymbol *symb);
typedef int  (APIENTRY *mManagersAdd)(const ConManager *man);
typedef int  (APIENTRY *mManagersDelete)(const ConManager *man);
//+------------------------------------------------------------------+
//| Trade activitiy Hook Functions                                   |
//+------------------------------------------------------------------+
typedef int  (APIENTRY *mTradeRequestFilter)(RequestInfo *request,const int isdemo);
typedef void (APIENTRY *mTradeRequestApply)(RequestInfo *request,const int isdemo);
typedef void (APIENTRY *mTradeRequestRestore)(RequestInfo *request);
typedef int  (APIENTRY *mTradeTransaction)(TradeTransInfo *trans,const UserInfo *user,int *request_id);
typedef int  (APIENTRY *mTradeCheckIE)(TradeTransInfo *trans,const UserInfo *user,const ConSymbol *symbol,double *prices);
typedef int  (APIENTRY *mTradeStopsFilter)(const ConGroup *group,const ConSymbol *symbol,const TradeRecord *trade);
typedef int  (APIENTRY *mTradeStopsApply)(const UserInfo *user,const ConGroup *group,const ConSymbol *symbol,TradeRecord *trade,const int isTP);
typedef int  (APIENTRY *mTradePendingsFilter)(const ConGroup *group,const ConSymbol *symbol,const TradeRecord *trade);
typedef int  (APIENTRY *mTradePendingsApply)(const UserInfo *user,const ConGroup *group,const ConSymbol *symbol,const TradeRecord *pending,TradeRecord *trade);
typedef int  (APIENTRY *mTradeStopoutsFilter)(const ConGroup *group,const ConSymbol *symbol,const int login,const double equity,const double margin);
typedef int  (APIENTRY *mTradeStopoutsApply)(const UserInfo *user,const ConGroup *group,const ConSymbol *symbol,TradeRecord *stopout);
typedef void (APIENTRY *mTradesAdd)(TradeRecord *trade,const UserInfo *user,const ConSymbol *symb);
typedef void (APIENTRY *mTradesAddExt)(TradeRecord *trade,const UserInfo *user,const ConSymbol *symb,const int mode);
typedef void (APIENTRY *mTradesAddRequest)(TradeRecord *trade,const RequestInfo *request,const UserInfo *user,const ConSymbol *symb,const int mode);
typedef void (APIENTRY *mTradesUpdate)(TradeRecord *trade,UserInfo *user,const int mode);
typedef void (APIENTRY *mTradesRestore)(const TradeRecord *trade);
typedef void (APIENTRY *mTradesCloseBy)(TradeRecord *ftrade,TradeRecord *strade,TradeRecord *remaind,ConSymbol *sec,UserInfo *user);
//+------------------------------------------------------------------+
//| Commissions & rollover calculation Hook Functions                |
//+------------------------------------------------------------------+
typedef void (APIENTRY *mTradeCommission)(TradeRecord *trade,const ConSymbol *sec,const ConGroup *grp);
typedef int  (APIENTRY *mTradeCommissionAgent)(TradeRecord *trade,const ConSymbol *sec,const UserInfo *user);
typedef int  (APIENTRY *mTradeRollover)(TradeRecord *trade,const double value,const OverNightData *data);
//+------------------------------------------------------------------+
//| Statements generation Hook Functions                             |
//+------------------------------------------------------------------+
typedef void (APIENTRY *mStatementsBaseAll)(const int month_state,const GroupState *groups,const int groups_total,const DailyReport *reports,const int reports_total);
typedef void (APIENTRY *mStatementsUser)(const int month_state,const UserRecord *user,const ConGroup *group,TradeRecord *first_trade,TradeRecord *last_trade,StateReport *state);
typedef void (APIENTRY *mStatementsRollovers)(const int month_state,const time_t ctm,const tm *ttm);
//+------------------------------------------------------------------+
//| Dealer activity Hook Functions                                   |
//+------------------------------------------------------------------+
typedef int  (APIENTRY *mDealerConnect)(const ConManager *manager);
typedef void (APIENTRY *mDealerDisconnect)(const ConManager *manager);
typedef int  (APIENTRY *mDealerGet)(const ConManager *manager,const RequestInfo *request);
typedef int  (APIENTRY *mDealerPrice)(const int id,const UserInfo *us,double *prices,const int in_stream);
typedef int  (APIENTRY *mDealerConfirm)(const int id,const UserInfo *us,double *prices);
typedef int  (APIENTRY *mDealerRequote)(const int id,const UserInfo *us,double *prices,const int in_stream);
typedef int  (APIENTRY *mDealerReset)(const int id,const UserInfo *us,const char flag);
typedef int  (APIENTRY *mDealerClientReject)(const int id,const UserInfo *us);
//+------------------------------------------------------------------+
//| Chart history, ticks filtration                                  |
//+------------------------------------------------------------------+
typedef int  (APIENTRY *mHistoryTickFilter)(const ConSymbol *symbol,FeedTick *inf);
typedef void (APIENTRY *mHistoryTickApply)(const ConSymbol *symbol,FeedTick *inf);
typedef int  (APIENTRY *mHistoryCheck)(const ConSymbol *symbol,RateInfo *rates,int *total);
//+------------------------------------------------------------------+
//| Internal mail & News Hook Functions                              |
//+------------------------------------------------------------------+
//typedef int  (APIENTRY *mNewsCheck)(FeedData *inf);
//typedef int  (APIENTRY *mMailCheck)(MailBoxHeader *mail, int *logins, const int total);
//+------------------------------------------------------------------+
//| Protocol extension Hook functions                                |
//+------------------------------------------------------------------+
typedef int   (APIENTRY *mTelnet)(const ULONG ip,char *buf,const int maxlen);
typedef char *(APIENTRY *mTelnetExt)(const ULONG ip,char *buf,int *data_len);
typedef int   (APIENTRY *mManagerProtocol)(const ULONG ip,const UserInfo *us,const unsigned char *in_data,const int in_size,unsigned char **out_data,int *out_size);


//---- macros
#define TERMINATE_STR(str) str[sizeof(str)-1]=0;
#define COPY_STR(dst,src) { strncpy(dst,src,sizeof(dst)-1); dst[sizeof(dst)-1]=0; }

// TODO: reference additional headers your program requires here