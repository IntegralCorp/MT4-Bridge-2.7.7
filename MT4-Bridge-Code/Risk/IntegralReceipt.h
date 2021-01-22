#pragma once
#include "Receipt.h"
//+------------------------------------------------------------------+
//| structure for storing an order waiting for execution             |
//+------------------------------------------------------------------+
struct IntegralReceipt
  {
  //--- receipt
  Receipt     receipt;
  //--- order add time
  char        unique_id[256];
  };
//+------------------------------------------------------------------+