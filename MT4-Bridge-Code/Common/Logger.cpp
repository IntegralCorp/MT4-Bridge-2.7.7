//+------------------------------------------------------------------+
//|                                      MetaTrader 5 Sample Gateway |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "Logger.h"
//+------------------------------------------------------------------+
//| Global objects                                                   |
//+------------------------------------------------------------------+
CLogger ExtLogger;
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
CLogger::CLogger()
   : m_api_server(NULL)
  {
//--- clear the buffer
   ZeroMemory(m_prebuf,sizeof(m_prebuf));
  }
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
CLogger::~CLogger()
  {
  }
//+------------------------------------------------------------------+
//| Set the gateway interface                                        |
//+------------------------------------------------------------------+
void CLogger::SetGatewayAPI(CServerInterface *api_server)
  {
//--- lock
   m_sync.Lock();
//--- remember the gateway interface
   m_api_server=api_server;
//--- unlock
   m_sync.Unlock();
  }
//+------------------------------------------------------------------+
//| Record to logs                                                   |
//+------------------------------------------------------------------+
void CLogger::Out(int code,LPCSTR format,...)
  {
   va_list arg_ptr;
//--- lock
   m_sync.Lock();
//--- if there is a gateway interface, record an entry to the journal
   if(m_api_server)
     {
      //--- parse the parameters
      va_start(arg_ptr,format);
      //--- form the string
      _vsnprintf_s(m_prebuf,_countof(m_prebuf)-1,_TRUNCATE,format,arg_ptr);
      //--- end of the list of arguments
      va_end(arg_ptr);
      //--- record to the journal
      m_api_server->LogsOut(code,"MT4.Bridge",m_prebuf);
     }
//--- unlock
   m_sync.Unlock();
  }
//+------------------------------------------------------------------+
