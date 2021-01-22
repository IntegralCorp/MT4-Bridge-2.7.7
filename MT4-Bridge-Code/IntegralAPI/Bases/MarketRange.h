//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include <new.h>
#include "..\..\API\MT4APITools.h"
#include "..\..\Common\StringTokenizer.h"
//+------------------------------------------------------------------+
//| Record for storing Groups                                        |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct MarketRange
  {
  char          text[1024];        // for checking purposes
  char          ccy_pair[64];      // currency pair      
  //----
  double        pips_factor;       // pips factor
  UINT          spot_precision;    // spot precision
  //--- fill
  static bool   Fill(StringType &text,MarketRange *rec);
  //--- print group
  static LPCSTR Print(const MarketRange &group,StringType &res);
  //---
  bool operator==(const MarketRange &a) const
    {
    return(StringType::Compare(text,a.text)==0);
    }
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Fill group fields                                                |
//+------------------------------------------------------------------+
inline bool MarketRange::Fill(StringType &text,MarketRange *rec)
  {
  int num_tokens;
  //--- check;
  if(text.Empty()||!rec)
    return(false);
  //---
  StringType::Copy(rec->text,text.Str());
  //---	clear spaces
  text.Replace(" ","");
  text.Refresh();
  //---
  StringTokenizer strtok=StringTokenizer(std::string(text.Str()),",");
  if(num_tokens=strtok.countTokens()!=3)
    return(false);
  //--- parse shortname
  StringType::Copy(rec->ccy_pair,strtok.nextToken().c_str());
  //---
  rec->pips_factor   =strtok.nextFloatToken();
  rec->spot_precision=strtok.nextIntToken();
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| Print rule                                                       |
//+------------------------------------------------------------------+
inline LPCSTR MarketRange::Print(const MarketRange &range,StringType &res)
  {
  //--- clear resulting string
  res.Clear();
  //---
  res.Format("%s, %0.2lf, %u",
             range.ccy_pair,
             range.pips_factor,
             range.spot_precision);
  //--- return result
  return(res.Str());
  }
//+------------------------------------------------------------------+
//| Array of trade groups                                            |
//+------------------------------------------------------------------+
typedef TMTArray<MarketRange>  MarketRangeRecordsArray;
//+------------------------------------------------------------------+