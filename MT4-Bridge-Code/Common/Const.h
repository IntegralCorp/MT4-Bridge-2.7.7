//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Copyright                                                         |
//+------------------------------------------------------------------+
#define Copyright             "Copyright 2017-2020, Integral Software"
//+------------------------------------------------------------------+
//| Version / build / date of the program                            |
//+------------------------------------------------------------------+
#define ProgramVersion        300
#define ProgramBuild          100
#define ProgramBuildDate      "20 Apr 2020"
#define ProgramName           "Integral Gateway"
#define ProgramDescription    "This is MetaTrader 4 gateway to Integral."
//+------------------------------------------------------------------+
//| global objects                                                   |
//+------------------------------------------------------------------+
extern char              ExtProgramPath[MAX_PATH];
extern CServerInterface *ExtServer;
extern UserInfo			 ExtManagerInfo;
//+------------------------------------------------------------------+
//| Macros of a size                                                 |
//+------------------------------------------------------------------+
#define KB UINT(1024)
#define MB UINT(1024*1024)
#define MAX_BUFFER 102400
//+------------------------------------------------------------------+
//| SL & TP database                                                 |
//+------------------------------------------------------------------+
#define TradeRequestBaseFile             "traderequest.dat"
#define TradeRequestBaseHeaderName       "TradeRequest"
#define TradeRequestBaseHeaderVersion    400
//+------------------------------------------------------------------+
//| The internal server time conversion                              |
//+------------------------------------------------------------------+
#define TIME_RATE            ((double)1.6777216)                         // Multiplier for conversion
#define STDTIME(custom_time) ((DWORD)((double)(custom_time)*TIME_RATE))  // converting the time to the standard 32-bit trading time __time32_t
#define OURTIME(stdtime)     ((DWORD)((double)(stdtime)/TIME_RATE))      // Reverse conversion
//+------------------------------------------------------------------+
//| Common constants                                                 |
//+------------------------------------------------------------------+
#define STACK_SIZE_COMMON     262144                  // size of thread stack
//+------------------------------------------------------------------+
//| Array of strings                                                 |
//+------------------------------------------------------------------+
typedef TMTArray<char[64],16> String64Array;
//+------------------------------------------------------------------+
