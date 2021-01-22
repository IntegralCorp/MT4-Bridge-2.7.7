//+------------------------------------------------------------------+
//|                                      MetaTrader 5 Sample Gateway |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "IntegralMsg.h"
#include "IntegralProtocolData.h"
#include "..\..\IO\Log.h"
//+------------------------------------------------------------------+
//| Check message protocol version                                   |
//+------------------------------------------------------------------+
bool CIntegralMsg::CheckVersion(const FIX43::Message &message)
  {
  FIX::BeginString version;
  bool res    =true;
  //--- get version
  READ_MSG_HEADER_FIELD(version);
  //--- check version
  if(version!=FIX::BeginString("FIX.4.3"))
    {
    LOG_ERROR(pluginLog,"invalid message protocol version");
    res=false;
    }
  //--- return result
  return(res);
  }
//+------------------------------------------------------------------+
//| Get message type                                                 |
//+------------------------------------------------------------------+
bool CIntegralMsg::GetType(const FIX43::Message &message,FIX::MsgType &msg_type)
  {
  //--- get type
  READ_MSG_HEADER_FIELD(msg_type);
  //--- successful
  return(true);
  }
//+------------------------------------------------------------------+
//| Get value of message tag                                         |
//+------------------------------------------------------------------+
bool CIntegralMsg::GetFieldValueStr(const FIX43::Message &message,FIX::FieldBase &field,LPSTR field_str)
  {
  //--- check
  if(!message.getFieldIfSet(field))
    return(false);
  //--- get tag value by filed
  StringType::Copy(field_str,field.getString().length()+1,field.getString().c_str());
  //--- return result
  return(true);
  }
//+------------------------------------------------------------------+
//| Get value of message tag                                         |
//+------------------------------------------------------------------+
bool CIntegralMsg::GetFieldValueChar(const FIX43::Message &message,FIX::FieldBase &field,char &field_str)
  {
  //--- check
  if(!message.getFieldIfSet(field)||field.getString().length()!=1)
    return(false);
  //--- get tag value by filed
  field_str=field.getString().c_str()[0];
  //--- return result
  return(true);
  }
//+------------------------------------------------------------------+
//| Get integer value of message tag                                 |
//+------------------------------------------------------------------+
bool CIntegralMsg::GetTagValueUINT(const FIX43::Message &message,FIX::FieldBase &field,UINT &tag_value)
  {
  //--- check
  if(!message.getFieldIfSet(field))
    return(false);
  //--- get tag data
  tag_value=atoi(field.getString().c_str());
  //--- return result
  return(true);
  }
//+------------------------------------------------------------------+
//| Get 64 bit integer value of message tag                          |
//+------------------------------------------------------------------+
bool CIntegralMsg::GetTagValueUINT64(const FIX43::Message &message,FIX::FieldBase &field,UINT64 &tag_value)
  {
  //--- check
  if(!message.getFieldIfSet(field))
    return(false);
  //--- get tag data
  tag_value=_atoi64(field.getString().c_str());
  //--- return result
  return(true);
  }
//+------------------------------------------------------------------+
//| Get 64 bit integer value of message tag                          |
//+------------------------------------------------------------------+
bool CIntegralMsg::GetTagValueINT64(const FIX43::Message &message,FIX::FieldBase &field,INT64 &tag_value)
  {
  //--- check
  if(!message.getFieldIfSet(field))
    return(false);
  //--- get tag data
  tag_value=_atoi64(field.getString().c_str());
  //--- return result
  return(true);
  }
//+------------------------------------------------------------------+
//| Get floating point value of message tag                          |
//+------------------------------------------------------------------+
bool CIntegralMsg::GetTagValueDbl(const FIX43::Message &message,FIX::FieldBase &field,double &tag_value)
  {
  //--- check
  if(!message.getFieldIfSet(field))
    return(false);
  //--- get tag data
  tag_value=atof(field.getString().c_str());
  //--- return result
  return(true);
  }
//+------------------------------------------------------------------+
//| Get value of message tag                                         |
//+------------------------------------------------------------------+
template<typename T>
bool CIntegralMsg::GetFieldValue(const FIX43::Message &message,const FIX::FieldBase &field,T &field_value)
  {
  //--- check
  if(!message.getFieldIfSet(field))
    return(false);
  //--- set field value
  field_value=field.getValue();
  //--- successfull
  return true;
  }
//+------------------------------------------------------------------+
//| Get value of message tag                                         |
//+------------------------------------------------------------------+
bool CIntegralMsg::GetField(const FIX43::Message &message,FIX::FieldBase &field)
  {
  //---
  return(message.getFieldIfSet(field));
  }
//+------------------------------------------------------------------+
//| Get value of message tag                                         |
//+------------------------------------------------------------------+
bool CIntegralMsg::GetHeaderField(const FIX43::Message &message,FIX::FieldBase &field)
  {
  //---
  return(message.getHeader().getFieldIfSet(field));
  }
//+------------------------------------------------------------------+
//| Set field of message                                             |
//+------------------------------------------------------------------+
bool CIntegralMsg::SetField(FIX43::Message &message,const FIX::FieldBase &field)
  {
  if(field.getString().empty())
    return(false);
  //---
  try
    {
    message.setField(field);
    return(true);
    }
  catch(const std::exception &)
    {
    }
  //---
  return(false);
  }
//+------------------------------------------------------------------+
//| Set field of message                                             |
//+------------------------------------------------------------------+
bool CIntegralMsg::SetFieldIfSet(FIX43::Message &message,FIX43::Message &message_source,FIX::FieldBase &field)
  {
  //---
  try
    {
    if(message_source.getFieldIfSet(field))
      {
      message.setField(field);
      }
    //---
    return(true);
    }
  catch(const std::exception &)
    {
    }
  //--- smthg went wrong
  return(false);
  }
//+------------------------------------------------------------------+
//| Set field of message                                             |
//+------------------------------------------------------------------+
bool CIntegralMsg::SetHeaderField(FIX43::Message &message,const FIX::FieldBase &field)
  {
  //--- checks
  if(field.getString().empty())
    return(false);
  //---
  try
    {
    message.getHeader().setField(field);
    return(true);
    }
  catch(const std::exception &)
    {
    }
  //---
  return(false);
  }
//+------------------------------------------------------------------+
