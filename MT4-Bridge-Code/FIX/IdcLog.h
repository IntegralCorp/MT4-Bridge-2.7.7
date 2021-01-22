#pragma once

#ifndef IDC_LOG_H
#define IDC_LOG_H

#include "..\QuickFix\Log.h"
#include "..\IO\Log.h"
#include "..\Tools\StringRef.h"

namespace FIX
  {
  //+------------------------------------------------------------------+
  //| Creates a screen based implementation of Log.                    |
  //| This displays all log events onto the standard output            |
  //+------------------------------------------------------------------+
  class IdcLogFactory: public LogFactory
    {
    private:
      CLog            *m_log;
      bool             m_use_settings;
      SessionSettings  &m_settings;

    public:
      IdcLogFactory(SessionSettings &m_settings);
      ~IdcLogFactory();
      //---
      bool Initialize(LPCSTR dir);
      void Shutdown();
      bool StartLogQueues();
      //---
      Log* create();
      Log* create(const SessionID &);
      void destroy(Log *log);

    private:
      CLog *init(const std::string &oName,const std::string &oFile,const Dictionary &settings);
    };
  //+------------------------------------------------------------------+
  //| Constructor                                                      |
  //+------------------------------------------------------------------+
  inline IdcLogFactory::IdcLogFactory(SessionSettings &settings):
    m_settings(settings),
    m_log(NULL),
    m_use_settings(true)
    {
    }
  //+------------------------------------------------------------------+
  //| Destructor                                                       |
  //+------------------------------------------------------------------+
  inline IdcLogFactory::~IdcLogFactory()
    {
    //---
    Shutdown();
    }
  //+------------------------------------------------------------------+
  //|                                                                  |
  //+------------------------------------------------------------------+
  inline void IdcLogFactory::Shutdown()
    {
    m_use_settings=false;
    //---
    if(m_log)
      {
      LOG_INFO(m_log,"------------------------------------ Stopping Event Logs ------------------------------------\r\n");
      delete m_log;
      m_log=NULL;
      }
    }
  //+------------------------------------------------------------------+
  //|                                                                  |
  //+------------------------------------------------------------------+
  inline bool IdcLogFactory::Initialize(LPCSTR dir)
    {
    //--- checks
    if(!dir)
      return(false);
    //---
    FIX::StringRef directory(dir);
    //---
    if(!m_log)
      {
      if((m_log=new(std::nothrow) CLog())==NULL)
        return(false);
      //---
      if(!m_log->Initialize("IDC.FIX.event"))
        return(false);
      }
    //---
    return(true);
    }
  //+------------------------------------------------------------------+
   //|                                                                  |
   //+------------------------------------------------------------------+
  inline bool IdcLogFactory::StartLogQueues()
    {
    if(!m_log)
      return(false);
    //---
    return(m_log->StartQueue());
    }
  //+------------------------------------------------------------------+
  //| Screen based implementation of Log.                              |
  //| This will display all log information onto the standard output   |
  //+------------------------------------------------------------------+
  class IdcLog: public Log
    {
    private:
      CLog        *fixLog;
      std::string  m_prefix;

    public:
      //--- constructors
      IdcLog(CLog *log): m_prefix("GLOBAL"),fixLog(log) { }
      IdcLog(const SessionID &sessionID,CLog *log): m_prefix(sessionID.toString()),fixLog(log) { }
      //----
      void clear() { }
      void backup() { }
      //---
      void onIncoming(const std::string &value) { }
      void onOutgoing(const std::string &value) { }
      void onEvent(const std::string &value);
    };
  //+------------------------------------------------------------------+
  //|                                                                  |
  //+------------------------------------------------------------------+
  inline void IdcLog::onEvent(const std::string &value)
    {
    //--- checks
    if(!fixLog)
      return;
    //---
    LOG_EVENT(fixLog,"%s\t%s",m_prefix.c_str(),value.c_str());
    }
  }
#endif //IDC_LOG_H