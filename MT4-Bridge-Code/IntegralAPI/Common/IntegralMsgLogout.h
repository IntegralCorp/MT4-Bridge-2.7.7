//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include "IntegralContextBase.h"
//+------------------------------------------------------------------+
//| Logout message                                                   |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct ExchangeMsgLogout
  {
  //--- message header
  IntegralMsgHeader header;
  //----
  char              short_name[256];
  FIX::SessionID    session_id;
  //--- logon message
  FIX43::Logout     message;
  //--- serialization methods
  static bool       Read(const FIX::SessionID &session,ExchangeMsgLogout &msg);
  static bool       Write(const ExchangeMsgLogout &msg,CIntegralContextBase &context);
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Read message from buffer to message structure                    |
//+------------------------------------------------------------------+
inline bool ExchangeMsgLogout::Read(const FIX::SessionID &session,ExchangeMsgLogout &msg)
  {
  //--- get header
  if(!IntegralMsgHeader::Read(msg.message,msg.header))
    return(false);
  //--- successful
  return(true);
  }
//+------------------------------------------------------------------+
//| Write message structure to buffer                                |
//+------------------------------------------------------------------+
inline bool ExchangeMsgLogout::Write(const ExchangeMsgLogout &msg,CIntegralContextBase &context)
  {
  //--- message
  FIX43::Logout  message;
  FIX::SessionID session_id;
  //--- prepare for sending
  session_id=msg.session_id;
  //--- send
  return(context.Send(message,session_id));
  }
//+------------------------------------------------------------------+
