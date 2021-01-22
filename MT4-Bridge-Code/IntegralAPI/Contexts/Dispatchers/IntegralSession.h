//+------------------------------------------------------------------+
//|                                         MetaTrader 5 NFA.Reports |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include <map>
#include <vector>
#include <algorithm>    // std::lower_bound, std::upper_bound, std::sort
//---
#include "..\..\..\QuickFix\SessionID.h"
#include "..\..\..\API\MT4APITools.h"
#include "..\..\..\IO\Log.h"
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
class CIntegralSessionRecord
  {
  private:
    typedef std::vector<std::string> SubscriptionsArray;

  private:
    std::string        m_short_name;
    FIX::SessionID     m_session_id;
    UINT               m_session_state;
    UINT               m_session_type;
    //---
    SubscriptionsArray m_subscriptions;

  public:
    //--- session type
    enum EnSessionType
      {
      Undefined=0,
      Quote    =1,
      Order    =2
      };
    //--- session state
    enum EnSessionState
      {
      None            =0,
      Initialized     =1,
      Created         =2,
      Disconnected    =3,     // Session disconnected.
      LoginInProgress =4,     // Login operation is in progress.     
      LogoutInProgress=5,     // Logout operation is in progress.
      Active          =6,     // Session is active (connected).
      Reconnecting    =7      // Session being reconnected.
      };

  public:
      CIntegralSessionRecord();
     ~CIntegralSessionRecord();
     //--- filling data
     void           FillSessionId(const FIX::SessionID &session_id);
     void           FillShortName(const std::string &short_name);
     void           FillSessionState(const UINT session_state);
     void           FillSessionType(const UINT session_type);
     //---
     bool           AddSymbol(const std::string &symbol);
     bool           RemoveSymbol(const std::string &symbol);
     bool           SymbolExists(const std::string &symbol);
     void           Clear(void);
     //--- access
     UINT           SessionType(void) const;
     UINT           SessionState(void) const;
     FIX::SessionID SessionId(void) const;
     LPCSTR         SenderCompId(void) const;
     std::string    ShortName(void) const;
  };   
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
typedef std::vector<FIX::SessionID> SessionIdentifiers;
//+------------------------------------------------------------------+
//| Sessions                                                         |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct IntegralSession
  {
  //--- session type
  enum EnSessionType
    {
    Undefined=0,
    Quote    =1,
    Order    =2
    };
  //--- session state
  enum EnSessionState
    {
    None            =0,
    Initialized     =1,
    Created         =2,
    Disconnected    =3,     // Session disconnected.
    LoginInProgress =4,     // Login operation is in progress.     
    LogoutInProgress=5,     // Logout operation is in progress.
    Active          =6,     // Session is active (connected).
    Reconnecting    =7      // Session being reconnected.
    };
  char           short_name[256];
  char           sender_comp_id[256];
  //bool           paper_trading;
  FIX::SessionID session_id;
  UINT           session_state;
  UINT           session_type;
  //--- filling of structure
  static bool    Fill(const FIX::SessionID &session_id,UINT state,LPCSTR short_name,/*const bool &paper_trading,*/IntegralSession &integral_session);
  // Returns the session state as a string.
  static LPCSTR  toString(UINT state,StringType &str);
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Filling of structure                                             |
//+------------------------------------------------------------------+
inline bool IntegralSession::Fill(const FIX::SessionID &session_id,
                                  UINT state,
                                  LPCSTR short_name,
                                  //const bool &paper_trading,
                                  IntegralSession &integral_session)
  {
  StringType256 sender_comp_id;
  UINT          type;
  //--- checks
  if(session_id.toString().empty())
    return(false);
  //---
  sender_comp_id.Assign(session_id.getSenderCompID().getValue().c_str());
  if(sender_comp_id.Empty())
    return(false);
  //---
  if(sender_comp_id.Find("quote.")>=0)
    type=Quote;
  else if(sender_comp_id.Find("order.")>=0)
    type=Order;
  else
    return(false);
  //---
  ZeroMemory(&integral_session,sizeof(IntegralSession));
  //--- fill
  StringType::Copy(integral_session.short_name,short_name);
  StringType::Copy(integral_session.sender_comp_id,session_id.getSenderCompID().getValue().c_str());
  integral_session.session_id   =session_id;
  integral_session.session_state=state;
  integral_session.session_type =type;
  //integral_session.paper_trading=paper_trading;
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| Returns the session state as a string                            |
//+------------------------------------------------------------------+
inline LPCSTR IntegralSession::toString(UINT state,StringType &str)
  {
  //---
  str.Clear();
  switch(state)
    {
    case None:             str.Assign("None");                            break;
    case Initialized:      str.Assign("Session initialized");             break;
    case Created:          str.Assign("Session created");                 break;
    case Disconnected:     str.Assign("Session disconnected");            break;
    case LoginInProgress:  str.Assign("Login operation is in progress");  break;
    case LogoutInProgress: str.Assign("Logout operation is in progress"); break;
    case Active:           str.Assign("Session is active (connected)");   break;
    case Reconnecting:     str.Assign("Session being reconnected");       break;
    default:               str.Assign("Unknown");                         break;
    }
  //---
  return(str.Str());
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
typedef std::map<std::string,IntegralSession> SessionsMap;
//+------------------------------------------------------------------+
