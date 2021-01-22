//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include "..\Common\Helpers.h"
#include "..\QuickFix\fix43\NewOrderSingle.h"
//+------------------------------------------------------------------+
//| Structure of receipt                                             |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct Receipt
  {
  char   session_id[256];
  char   unique_id[256];
  char   order[2048];
  time_t sent_time;
  //--- methods
  LPCSTR toString(StringType &res)
    {
    res.Clear();
    //---
    res.Format("%s|%s|%d|%s|\n",session_id,unique_id,sent_time,order);
    //--- result string
    return res.Str();
    }
  //+------------------------------------------------------------------+
  //| Is empty the structure                                           |
  //+------------------------------------------------------------------+
  bool IsEmpty(void)
    {
    //--- checks
    if(sent_time)
      return(false);
    //--- checks
    if(session_id[0]||unique_id[0]||order[0])
      return(false);
    //--- empty
    return(true);
    }
  //---
  static void   Fill(LPCSTR session,LPCSTR unique_id,const time_t oSentTime,const FIX43::NewOrderSingle &oOrder,Receipt &receipt);
  static void   Fill(LPCSTR text,Receipt &receipt);
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Fill structure                                                   |
//+------------------------------------------------------------------+
inline void Receipt::Fill(LPCSTR session_id,LPCSTR unique_id,const time_t oSentTime,const FIX43::NewOrderSingle &oOrder,Receipt &receipt)
  {
  StringType::Copy(receipt.session_id,session_id);
  StringType::Copy(receipt.unique_id,unique_id);
  receipt.sent_time=oSentTime;
  StringType::Copy(receipt.order,oOrder.toString().c_str());
  }
//+------------------------------------------------------------------+
//| Fill structure                                                   |
//+------------------------------------------------------------------+
inline void Receipt::Fill(LPCSTR oText,Receipt &receipt)
  {
  std::vector<std::string> args;
  SPluginHelpers::split(oText,"|",args,5);
  //---
  StringType::Copy(receipt.session_id,args[0].c_str());
  StringType::Copy(receipt.unique_id,args[1].c_str());
  receipt.sent_time=stoi(args[2]);
  StringType::Copy(receipt.order,args[3].c_str());
  }
//+------------------------------------------------------------------+
