//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include <windows.h>
#include <stdio.h>
#include "..\include\MT4ServerAPI.h"
#include "..\API\MT4APITools.h"
//+------------------------------------------------------------------+
//| Journal class                                                    |
//+------------------------------------------------------------------+
class CLogger
  {

private:
   //--- constants
   enum constants
     {
      PREBUF_SIZE    =16*1024                     // size of a preliminary buffer
     };
   //--- pointer to the gateway interface
   CServerInterface    *m_api_server;
   //--- message buffer
   char           m_prebuf[PREBUF_SIZE];
   //--- synchronizer
   CMTSync           m_sync;

public:
   //--- constructor/destructor
                     CLogger();
                    ~CLogger();
   //--- basic functional
   void              SetGatewayAPI(CServerInterface *api_m_server);
   void              Out(int code,LPCSTR msg,...);
  };

extern CLogger  ExtLogger;
//+------------------------------------------------------------------+
