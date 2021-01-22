//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include "IntegralContextBase.h"
#include "..\..\IO\Log.h"
#include "..\..\API\MT4APITools.h"
#include "IntegralMsg.h"
//+------------------------------------------------------------------+
//| Book item transaction structure                                  |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct MTBookItem
  {
  //--- book item type
  enum EnBookItem
    {
    ItemReset      =0,                                    // reset book item
    ItemSellMarket =1,                                    // sell item by market
    ItemBuyMarket  =2                                     // buy item by market
    };
  double price;
  double size;
  //---
  UINT   type;
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Tick structure                                                   |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct IntegralBook
  {
  char                                    ccy_pair[64];               // ccy pair
  //int                                     m_date;
  //int                                     m_time;
  FIX::UTCTIMESTAMP fxi;
  char              fix_message[2048];
  //---
  char              short_name[64];                 // ShortName
  //---
  MTBookItem        items[32*4];                // book transactions
  UINT              items_total;                // book transactions count
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Tick message                                                     |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct IntegralMsgBook
  {
  //--- message data
  IntegralBook      book;
  //--- serialization methods
  static bool       Read(const FIX43::MarketDataSnapshotFullRefresh &market,LPCSTR short_name,IntegralMsgBook &msg);
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Read message from buffer to structure                            |
//+------------------------------------------------------------------+
inline bool IntegralMsgBook::Read(const FIX43::MarketDataSnapshotFullRefresh &message,LPCSTR short_name,IntegralMsgBook &msg)
  {
  //---
  FIX43::MarketDataSnapshotFullRefresh::NoMDEntries group;
  FIX::NoMDEntries    noMDEntries;
  FIX::Symbol         symbol;
   FIX::SendingTime   sending_time;
  FIX::MDEntryPx      mDEntryPx;
  FIX::MDEntrySize    mDEntrySize;
  FIX::MDEntryType    mDEntryType;
  FIX::QuoteCondition quoteCondition;
  //--- checks
  if(!short_name)
    return(false);
  //---
  ZeroMemory(&msg,sizeof(IntegralMsgBook));
  //--- set short name
  StringType::Copy(msg.book.short_name,short_name);
  //--- get entries
  if(!message.getIfSet(noMDEntries)||
     !message.getIfSet(symbol)||
     !message.getHeader().getIfSet(sending_time))
    return(false);
  //--- set time
  msg.book.fxi=sending_time.getValue();
  //--- set currency pair
  StringType::Copy(msg.book.ccy_pair,symbol.getString().c_str());
  StringType::Copy(msg.book.fix_message,message.toString().c_str());
  //---
  msg.book.items_total=0;
  for(int i=1; i<=noMDEntries.getValue(); i++)
    {
    message.getGroup(i,group);
    //---
    if(!group.getIfSet(mDEntryType)||
       !group.getIfSet(mDEntryPx)||
       !group.getIfSet(mDEntrySize)||
       !group.getIfSet(quoteCondition)) continue;
    //--- do not process 
    if(quoteCondition.getValue()!=FIX::QuoteCondition_OPEN)
      continue; // ESF-1826
    //--- process only bid/ask
    switch(mDEntryType.getValue())
      {
      case FIX::MDEntryType_BID:
        msg.book.items[msg.book.items_total].type =MTBookItem::EnBookItem::ItemBuyMarket;
        msg.book.items[msg.book.items_total].price=mDEntryPx.getValue();
        msg.book.items[msg.book.items_total].size =mDEntrySize.getValue();
        msg.book.items_total++;
        break;
      case FIX::MDEntryType_OFFER:
        msg.book.items[msg.book.items_total].type =MTBookItem::EnBookItem::ItemSellMarket;
        msg.book.items[msg.book.items_total].price=mDEntryPx.getValue();
        msg.book.items[msg.book.items_total].size =mDEntrySize.getValue();
        msg.book.items_total++;
        break;
      default:
        break;
      }
    }
  //---
  return(true);
  }
//+------------------------------------------------------------------+