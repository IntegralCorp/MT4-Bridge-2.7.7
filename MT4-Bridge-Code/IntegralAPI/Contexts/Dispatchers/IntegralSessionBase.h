//+------------------------------------------------------------------+
//|                                         MetaTrader 5 NFA.Reports |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "IntegralSession.h"
//+------------------------------------------------------------------+
//| Record for storing Request history                               |
//+------------------------------------------------------------------+
struct IntegralSessionRecord
  {
  std::string            frozen_string;
  CIntegralSessionRecord record;
  };
//+------------------------------------------------------------------+
//| Memory based integral session storage                            |
//+------------------------------------------------------------------+
class CIntegralSessionBase
  {
  private:
    TMTArray<IntegralSessionRecord *> m_sessions;
    CMTSync                           m_sync;         // syncronizer

  public:
     CIntegralSessionBase(void);
    ~CIntegralSessionBase(void);
    //--- clear/add/remove
    void                   Clear(void);
    bool                   Add(const std::string &frozen_string,const CIntegralSessionRecord &record);
    bool                   GetSession(const std::string &frozen_string,const UINT session_type,CIntegralSessionRecord &record);
    bool                   GetSession(const std::string &frozen_string,CIntegralSessionRecord &record);
    bool                   GetSessionByShortName(const std::string &short_name,const UINT session_type,CIntegralSessionRecord &record);
    bool                   GetOrderSession(const std::string &frozen_string,CIntegralSessionRecord &record);
    bool                   GetQuoteSession(const std::string &frozen_string,CIntegralSessionRecord &record);
    void                   ClearSubscriptions(const std::string &frozen_string,CIntegralSessionRecord &record);
    bool                   Update(const std::string &frozen_string,const UINT session_type,const UINT m_session_state);
    bool                   Update(const std::string &frozen_string,const UINT m_session_state);
    bool                   Update(const std::string &frozen_string,const CIntegralSessionRecord &record);
    bool                   Remove(const std::string &frozen_string,const UINT session_type);
    bool                   Remove(const std::string &frozen_string,const UINT session_type,CIntegralSessionRecord &record);
    void                   GetSessions(SessionIdentifiers &sessions);
    void                   GetSessions(SessionIdentifiers &sessions,const UINT m_session_state);
    //--- symbol
    bool                   SymbolExists(const std::string &frozen_string,LPCSTR symbol);

  private:
    //--- memory
    IntegralSessionRecord *IntegralSessionAllocate(void);
    void                   IntegralSessionFree(IntegralSessionRecord *record);
    //--- sort/search
    static int             SortIntegralSessions(const void *left,const void *right);
    static int             SearchIntegralSessions(const void *left,const void *right);
  };
//+------------------------------------------------------------------+
