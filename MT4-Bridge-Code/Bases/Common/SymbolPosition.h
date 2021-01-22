//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include <minwindef.h>
//---
#include "..\..\include\MT4ServerAPI.h"
#include "..\..\API\MT4APITools.h"
//+------------------------------------------------------------------+
//| Order structure                                                  |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct SymbolPosition
  {
  enum EnActivation
    {
    //--- no activation
    ACTIVATION_NONE=0,
    //--- stoploss, takeprofit, pendings
    ACTIVATION_SL,ACTIVATION_TP,ACTIVATION_PENDING,
    //--- stopout
    ACTIVATION_STOPOUT,
    //--- rollbacks
    ACTIVATION_SL_ROLLBACK     =-ACTIVATION_SL,
    ACTIVATION_TP_ROLLBACK     =-ACTIVATION_TP,
    ACTIVATION_PENDING_ROLLBACK=-ACTIVATION_PENDING,
    ACTIVATION_STOPOUT_ROLLBACK=-ACTIVATION_STOPOUT
    };
  char              symbol[12];                // symbol
  double            price;                     // weighted average opening position price
  INT               buy_volume;                // buy volume
  INT               sell_volume;               // sell volume
  //--- add volume to position with average price recalculation
  static void       PositionAddVolume(SymbolPosition &position,INT volume,double price,short cmd);
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Add volume to position with price recalculation                  |
//+------------------------------------------------------------------+
inline void SymbolPosition::PositionAddVolume(SymbolPosition &position,INT volume,double price,short cmd)
  {
  //--- checks
  if(cmd>OP_SELL_STOP)
    return;
  //--- add volume
  switch(cmd)
    {
    case OP_BUY:
    case OP_BUY_LIMIT:
    case OP_BUY_STOP:
      position.buy_volume+=volume;
      break;
    case OP_SELL:
    case OP_SELL_LIMIT:
    case OP_SELL_STOP:
      position.sell_volume+=volume;
    default:
      break;
    }
  //--- set it's price
  position.price+=(volume*price);
  }
//+------------------------------------------------------------------+
//| Array of positions                                               |
//+------------------------------------------------------------------+
typedef TMTArray<SymbolPosition> SymbolPositionsArray;
//+------------------------------------------------------------------+
