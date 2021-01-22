//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include "..\..\API\MT4APITools.h"
//#include "..\include\MT4ServerAPI.h"
#include "TradeRequest.h"
#include "..\..\Tools\RecordDataBase.h"
#include "..\..\IO\Log.h"
//+------------------------------------------------------------------+
//| Permanent storage for TradeRequest records                 |
//+------------------------------------------------------------------+
class CTradeRequestBase : protected TRecordDataBase<TradeRequest>
  {
private:
   TMTArray<TradeRequest*> m_index_position;

public:
                     CTradeRequestBase(void);
                    ~CTradeRequestBase(void);
   //--- 
   bool              Initialize(void);
   void              Shutdown(void);
   void              Compact(void);
   //---
   bool              RecordGet(LPCSTR unique_id,TradeRequest& record);
   bool              RecordGet(const UINT state,TradeRequest& record);
   bool              RecordUpdate(TradeRequest& record,const bool allow_add=true);
   bool              RecordDelete(LPCSTR unique_id);

private:
   bool              IndexRebuild(void);
   //--- logger
   virtual void      Out(const UINT code,LPCSTR msg,...);
   //--- sorting
   static int        SortByUniqueId(const void *left,const void *right);
   static int        SearchByUniqueId(const void *left,const void *right);
   static int        SearchTradeRequestByState(const void *left,const void *right);
  };
//+------------------------------------------------------------------+