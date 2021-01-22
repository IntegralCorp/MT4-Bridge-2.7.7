//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include "..\..\QuickFix\fix43\Message.h"
#include "..\..\API\MT4APITools.h"
#include "..\..\Tools\StringRef.h"
#include "..\..\Tools\smart_ptr.h"
//+------------------------------------------------------------------+
//| Macro for reading/writing message fields                         |
//+------------------------------------------------------------------+
//+------------------------------------------------------------------+
//| Read string value from specified field                           |
//+------------------------------------------------------------------+
#define READ_MSG_FIELD_VALUE_STR(field,field_str)\
   if(!CIntegralMsg::GetFieldValueStr(message,field,field_str)) return(false);\
//+------------------------------------------------------------------+
//| Read string value from specified field                           |
//+------------------------------------------------------------------+
#define READ_MSG_FIELD_VALUE_CHAR(field,field_str)\
   if(!CIntegralMsg::GetFieldValueChar(message,field,field_str)) return(false);\
//+------------------------------------------------------------------+
//| Read UINT value from specified tag                               |
//+------------------------------------------------------------------+
#define READ_MSG_TAG_UINT(field,field_str)\
   if(!CIntegralMsg::GetTagValueUINT(message,field,field_str)) return(false);\
//+------------------------------------------------------------------+
//| Read UINT64 value from specified tag                             |
//+------------------------------------------------------------------+
#define READ_MSG_TAG_UINT64(field,field_str)\
   if(!CIntegralMsg::GetTagValueUINT64(message,field,field_str)) return(false);\
//+------------------------------------------------------------------+
//| Read INT64 value from specified tag                             |
//+------------------------------------------------------------------+
#define READ_MSG_TAG_INT64(field,field_str)\
   if(!CIntegralMsg::GetTagValueINT64(message,field,field_str)) return(false);\
//+------------------------------------------------------------------+
//| Read double value from specified tag                             |
//+------------------------------------------------------------------+
#define READ_MSG_TAG_DBL(field,field_str)\
   if(!CIntegralMsg::GetTagValueDbl(message,field,field_str)) return(false);\
//+------------------------------------------------------------------+
//| Read field value from specified field                            |
//+------------------------------------------------------------------+
#define READ_MSG_FIELD_VALUE(field,field_value)\
   if(!CIntegralMsg::GetFieldValue(message,field,field_value)) return(false);\
//+------------------------------------------------------------------+
//| Read field value from specified field                            |
//+------------------------------------------------------------------+
#define READ_MSG_FIELD_VALUE_IF_SET(field,field_value)\
   if(!CIntegralMsg::GetFieldValue(message,field,field_value)) return(true);\
//+------------------------------------------------------------------+
//| Read field from specified filed                                  |
//+------------------------------------------------------------------+
#define READ_MSG_FIELD(field)\
   if(!CIntegralMsg::GetField(message,field)) return(false);\
//+------------------------------------------------------------------+
//| Read field from specified filed                                  |
//+------------------------------------------------------------------+
#define READ_MSG_FIELD_IF_SET(field)\
    if(!CIntegralMsg::GetField(message,field)) return(true);\
//+------------------------------------------------------------------+
//| Read field from specified filed                                  |
//+------------------------------------------------------------------+
#define READ_MSG_HEADER_FIELD(field)\
   if(!CIntegralMsg::GetHeaderField(message,field)) return(false);\
//+------------------------------------------------------------------+
//| Read field from specified filed                                  |
//+------------------------------------------------------------------+
#define READ_MSG_HEADER_FIELD_IF_SET(field)\
    if(!CIntegralMsg::GetHeaderField(message,field)) return(true);\
//+------------------------------------------------------------------+
//| Set filed value for specified field                              |
//+------------------------------------------------------------------+
#define WRITE_MSG_FIELD(field)\
   if(!CIntegralMsg::SetField(message,field)) return(false);\
//+------------------------------------------------------------------+
//| Set filed value for specified field                              |
//+------------------------------------------------------------------+
#define WRITE_MSG_FIELD_IF_SET(message_source,field)\
   if(!CIntegralMsg::SetFieldIfSet(message,message_source,field)) return(false);\
//+------------------------------------------------------------------+
//| Set header filed value for specified field                       |
//+------------------------------------------------------------------+
#define WRITE_MSG_HEADER_FIELD(field)\
   if(!CIntegralMsg::SetHeaderField(message,field)) return(false);\
//+------------------------------------------------------------------+
//| Set header filed value for specified field                       |
//+------------------------------------------------------------------+
#define WRITE_MSG_HEADER_FIELD_IF_SET(field)\
   if(!CIntegralMsg::SetHeaderField(message,field)) return(true);\
//+------------------------------------------------------------------+
//| Class with static methods of processing message data             |
//+------------------------------------------------------------------+
class CIntegralMsg
  {
  public:
    //--- check message protocol version
    static bool       CheckVersion(const FIX43::Message &message);
    //--- get message type
    static bool       GetType(const FIX43::Message &message,FIX::MsgType &msg_type);
    //--- get values of message tags
    static bool       GetFieldValueStr(const FIX43::Message &message,FIX::FieldBase &field,LPSTR field_str);
    static bool       GetFieldValueChar(const FIX43::Message &message,FIX::FieldBase &field,char &field_str);
    static bool       GetTagValueUINT(const FIX43::Message &message,FIX::FieldBase &field,UINT &tag_value);
    static bool       GetTagValueUINT64(const FIX43::Message &message,FIX::FieldBase &field,UINT64 &tag_value);
    static bool       GetTagValueINT64(const FIX43::Message &message,FIX::FieldBase &field,INT64 &tag_value);
    static bool       GetTagValueDbl(const FIX43::Message &message,FIX::FieldBase &field,double &tag_value);
    //---- get values of message fields
    template<typename T>
    static bool       GetFieldValue(const FIX43::Message &message,const FIX::FieldBase &field,T &field_value);
    //--- get fields of messages
    static bool       GetField(const FIX43::Message &message,FIX::FieldBase &field);
    static bool       GetHeaderField(const FIX43::Message &message,FIX::FieldBase &field);
    //--- set values of message fields
    static bool       SetFieldIfSet(FIX43::Message &message,FIX43::Message &message_source,FIX::FieldBase &field);
    static bool       SetField(FIX43::Message &message,const FIX::FieldBase &field);
    static bool       SetHeaderField(FIX43::Message &message,const FIX::FieldBase &field);
  };
//+------------------------------------------------------------------+
//| Message header                                                   |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct IntegralMsgHeader
  {
  //--- required
  FIX::BeginString      begin_string;
  FIX::MsgType          type;
  FIX::SenderCompID     sender_comp_id;
  FIX::TargetCompID     target_comp_id;
  FIX::MsgSeqNum        seq_number;
  FIX::SendingTime      send_time;
  //--- optional
  FIX::OnBehalfOfCompID on_behalf_of_comp_id;
  FIX::SenderSubID      sender_sub_id;
  FIX::DeliverToCompID  deliver_to_comp_id;
  FIX::TargetSubID      target_sub_id;
  FIX::OnBehalfOfSubID  on_behalf_of_sub_id;
  //--- serialization methods
  static bool           Read(const FIX43::Message message,IntegralMsgHeader &header);
  static bool           Write(const IntegralMsgHeader &header,FIX43::Message message);
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Read message to structure                                        |
//+------------------------------------------------------------------+
inline bool IntegralMsgHeader::Read(const FIX43::Message message,IntegralMsgHeader &header)
  {
  //--- check
  if(message.begin()==message.end())
    return(false);
  //--- get begin string
  READ_MSG_HEADER_FIELD(header.begin_string);
  //--- get type
  READ_MSG_HEADER_FIELD(header.type);
  //--- get sender comp ID
  READ_MSG_HEADER_FIELD(header.sender_comp_id);
  //--- get target comp id
  READ_MSG_HEADER_FIELD(header.target_comp_id);
  //--- get sequence number
  READ_MSG_HEADER_FIELD(header.seq_number);
  //--- get send time
  READ_MSG_HEADER_FIELD(header.send_time);
  //--- read optional data
  READ_MSG_HEADER_FIELD_IF_SET(header.on_behalf_of_comp_id);
  READ_MSG_HEADER_FIELD_IF_SET(header.sender_sub_id);
  READ_MSG_HEADER_FIELD_IF_SET(header.deliver_to_comp_id);
  READ_MSG_HEADER_FIELD_IF_SET(header.target_sub_id);
  READ_MSG_HEADER_FIELD_IF_SET(header.on_behalf_of_sub_id);
  //--- successful
  return(true);
  }
//+------------------------------------------------------------------+
//| Write structure to message                                       |
//+------------------------------------------------------------------+
inline bool IntegralMsgHeader::Write(const IntegralMsgHeader &header,FIX43::Message message)
  {
  //--- write sender sub id
  WRITE_MSG_HEADER_FIELD(FIX::SenderSubID(header.sender_comp_id));
  //--- write target comp id
  WRITE_MSG_HEADER_FIELD(FIX::TargetCompID(header.target_comp_id));
  //--- write deliver to comp id
  CIntegralMsg::SetHeaderField(message,FIX::DeliverToCompID(header.deliver_to_comp_id));
  //--- write on behalf of comp id
  CIntegralMsg::SetHeaderField(message,FIX::OnBehalfOfCompID(header.on_behalf_of_comp_id));
  //--- write sender sub id
  CIntegralMsg::SetHeaderField(message,FIX::SenderSubID(header.sender_sub_id));
  //--- write deliver to comp id
  CIntegralMsg::SetHeaderField(message,FIX::DeliverToCompID(header.deliver_to_comp_id));
  //--- write target sub id
  CIntegralMsg::SetHeaderField(message,FIX::TargetSubID(header.target_sub_id));
  //--- write on behalf of sub id
  CIntegralMsg::SetHeaderField(message,FIX::OnBehalfOfSubID(header.on_behalf_of_sub_id));
  //--- successful
  return(true);
  }
//+------------------------------------------------------------------+
