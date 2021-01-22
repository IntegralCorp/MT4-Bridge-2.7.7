//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include "IntegralContextBase.h"
#include "IntegralMsg.h"
//+------------------------------------------------------------------+
//| Login message                                                    |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct ExchangeMsgLogon
  {
  //--- message header
  IntegralMsgHeader header;
  //--- session info
  FIX::SessionID    session;
  //--- logon message
  FIX43::Logon      message;
  //--- serialization methods
  static bool       Read(const FIX::SessionID &session,ExchangeMsgLogon &msg);
  static bool       Write(const ExchangeMsgLogon &msg,CIntegralContextBase &context);
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Array of sessions                                                  |
//+------------------------------------------------------------------+
typedef TMTArray<ExchangeMsgLogon> ExchangeMsgLoginsArray;
//+------------------------------------------------------------------+
//| Read message from buffer to message structure                    |
//+------------------------------------------------------------------+
inline bool ExchangeMsgLogon::Read(const FIX::SessionID &session,ExchangeMsgLogon &msg)
  {
  //--- get header
  if(!IntegralMsgHeader::Read(msg.message,msg.header))
    return(false);
  //--- get sender comp id
  StringType256 sender_comp_id(session.getSenderCompID().getString().c_str());
  //--- check
  if(sender_comp_id.Empty())
    return(false);
  //--- get login
  if(sender_comp_id.Compare(".quote")!=std::string::npos)
    {

    }
  else if(sender_comp_id.Compare(".order")!=std::string::npos)
    {

    }
  //--- successful
  return(true);
  }
//+------------------------------------------------------------------+
//| Write message structure to buffer                                |
//+------------------------------------------------------------------+
inline bool ExchangeMsgLogon::Write(const ExchangeMsgLogon &msg,CIntegralContextBase &context)
  {
  //--- send
  return(true);//context.Send(buffer,msg_buffer_size));
  }
//+------------------------------------------------------------------+
