//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include <new.h>
#include <regex>
#include <vector>
//---
#include "..\..\API\MT4APITools.h"
#include "..\..\Util\threadmap.h"
#include "..\..\QuickFix\FixValues.h"
#include "..\..\Common\StringTokenizer.h"
#include "..\..\IO\Log.h"
//+------------------------------------------------------------------+
//| Record for storing Symbol Config                                 |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct Instrument
  {
  enum EnMarketRange
    {
    Value     =0,
    Percentage=1
    };
  //---
  enum EnPriceAlgo
    {
    BEST=0,
    DEPTH=1
    };
  char            text[1024];       // for checking purposes
  char            symbol[12];       // name (mt4)
  char            ccy_pair[64];     // target symbol (integral)
  //---           
  char            short_name[64];   // [SESSION] ShortName
  //---           
  char            base_cry[32];      // base currency         ?
  char            quote_cry[4];      // quote currency        ?
  //---           
  double          min_spread;			// minimum spread
  double          bid_spread;			// bid spread
  double          ask_spread;			// ask spread
  double          max_spread;			// maximum spread
  double          mrkt_range;			// market range
  //--- EnMarketRange
  UINT            range_type;
  //--- EnPriceAlgo          
  UINT            price_algo;        // price algo
  double          price_size;        // price size
  bool            bias_bid;          // bid biaz
  double          min_qty;           // min quantity
  char            time_in_force;     // =FIX::TimeInForce_GOOD_TILL_DATE;
  int             expiration_time;   // =30;
  bool            enabled;           // =false;
  double          lot_conversion;    // =1.0;
  //---           
  int             digits;            // security precision
  int             spread;            // spread
  int             spread_balance;    // spread balance
  int             exec_mode;         // execution mode
  double          point;
  double          contract_size;     // contract size
  int             type;              // security group (see ConSymbolGroup)
  char            name[16];          // group name (con group)
  //--- fill
  static bool     Fill(StringType &text,Instrument *rec);
  //--- print route
  static LPCSTR   Print(const Instrument &instrument,StringType &res);
  //---
  bool operator==(const Instrument &a) const
    {
    return(StringType::Compare(text,a.text)==0);
    }
  private:
    //--- parameter helpers
    static double ParseSize(const std::string &amnt);
    static double ParsePercentage(const std::string &amnt,bool &percentage);
    static double ReplaceCharWithValue(const std::string &amnt,double multiplier);
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Fill symbol fields                                               |
//+------------------------------------------------------------------+
inline bool Instrument::Fill(StringType &text,Instrument *rec)
  {
  StringType64 elem;
  StringType64 price_algo;
  bool         percentage=false;
  int          num_tokens;
  //--- check;
  if(text.Empty()||!rec)
    return(false);
  //---
  StringType::Copy(rec->text,text.Str());
  //---	clear spaces
  text.Replace(" ","");
  text.Refresh();
  //--- fill the fields
  rec->enabled=false;
  rec->time_in_force=FIX::TimeInForce_GOOD_TILL_DATE;
  rec->expiration_time=30;
  rec->lot_conversion=1.0;
  rec->bias_bid=true;
  //--- fill currency
  /*if(StringType::Len(rec->symbol)>=6)
    {
    StringType::Copy(rec->base_cry,_countof(rec->base_cry),rec->symbol,3);
    StringType::Copy(rec->quote_cry,_countof(rec->quote_cry),rec->symbol+3,3);
    }*/
    //---
  StringTokenizer strtok=StringTokenizer(std::string(text.Str()),",");
  if((num_tokens=strtok.countTokens())<14)
    {
    LOG_ERROR(pluginLog,"instrument parse error for text:%s [tokens:%d]",text.Str(),strtok.countTokens());
    return(false);
    }
  //--- parse short name
  StringType::Copy(rec->short_name,strtok.nextToken().c_str());
  //--- parse currency pair
  StringType::Copy(rec->ccy_pair,strtok.nextToken().c_str());
  //--- fill base and quote cry
  StringTokenizer ccytok=StringTokenizer(std::string(rec->ccy_pair),"/");
  if(ccytok.countTokens()!=2)
    return(false);
  //---
  StringType::Copy(rec->base_cry,ccytok.nextToken().c_str());
  StringType::Copy(rec->quote_cry,_countof(rec->quote_cry),ccytok.nextToken().c_str(),3);
  //--- fill spreads and range
  rec->min_spread=strtok.nextFloatToken();
  rec->bid_spread=strtok.nextFloatToken();
  rec->ask_spread=strtok.nextFloatToken();
  rec->max_spread=strtok.nextFloatToken();
  //---
  rec->mrkt_range=ParsePercentage(strtok.nextToken(),percentage);
  if(percentage)
    {
    rec->range_type=Instrument::Percentage;
    }
  //---
  price_algo.Assign(strtok.nextToken().c_str());
  if(price_algo.CompareNoCase("DEPTH")==0)
    rec->price_algo=EnPriceAlgo::DEPTH;
  else
    rec->price_algo=EnPriceAlgo::BEST;
  //--- read price size
  rec->price_size=ParseSize(strtok.nextToken());
  //--- bid biaz?
  if(!strtok.hasMoreTokens())
    {
    LOG_ERROR(pluginLog,"no more tokens [%s]",text.Str());
    return(false);
    }
  rec->bias_bid=((StringType::CompareNoCase(strtok.nextToken().c_str(),"Bid")==0)?true:false);
  //--- read minimum quantity
  rec->min_qty=ParseSize(strtok.nextToken());
  //---
  elem.Assign(strtok.nextToken().c_str());
  if(elem.CompareNoCase("FOK")==0)
    rec->time_in_force=FIX::TimeInForce_FILL_OR_KILL;
  else if(elem.CompareNoCase("IOC")==0)
    rec->time_in_force=FIX::TimeInForce_IMMEDIATE_OR_CANCEL;
  else
    rec->time_in_force=FIX::TimeInForce_GOOD_TILL_DATE;
  //---
  elem.Assign(strtok.nextToken().c_str());
  rec->expiration_time=max(min(atoi(elem.Str()),90),1);
  //LOG_INFO(pluginLog,"symbol enabled %s [instrument.expiration_time:%d]",temp.Str(),instrument.expiration_time);

  elem.Assign(strtok.nextToken().c_str());

  //LOG_INFO(pluginLog,"symbol enabled %s [%d]",temp.Str(),result);

  rec->enabled=(elem.CompareNoCase("True")==0||elem.CompareNoCase("1")==0);
  //---
  if(strtok.hasMoreTokens())
    rec->lot_conversion=strtok.nextFloatToken();
  //--- successful
  return(true);
  }
//+------------------------------------------------------------------+
//| Print instrument                                                 |
//+------------------------------------------------------------------+
inline LPCSTR Instrument::Print(const Instrument &instrument,StringType &res)
  {
  StringType16 min_spread,bid_spread,ask_spread,max_spread;
  StringType64 mrkt_range,price_size,min_qty,lot_conversion;
  //--- clear resulting string
  res.Clear();
  //---
  SMTFormat::FormatPrice(min_spread,instrument.min_spread,instrument.digits);
  SMTFormat::FormatPrice(bid_spread,instrument.bid_spread,instrument.digits);
  SMTFormat::FormatPrice(ask_spread,instrument.ask_spread,instrument.digits);
  SMTFormat::FormatPrice(max_spread,instrument.max_spread,instrument.digits);
  SMTFormat::FormatPrice(mrkt_range,instrument.mrkt_range,0);
  SMTFormat::FormatPrice(lot_conversion,instrument.lot_conversion,2);
  SMTFormat::FormatVolumeDouble(price_size,instrument.price_size,true);
  SMTFormat::FormatVolumeDouble(min_qty,instrument.min_qty,true);
  //---
  res.Format("%s=%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %d, %s",
             instrument.symbol,
             instrument.short_name,
             instrument.ccy_pair,
             min_spread.Str(),
             bid_spread.Str(),
             ask_spread.Str(),
             max_spread.Str(),
             mrkt_range.Str(),
             (instrument.price_algo==Instrument::EnPriceAlgo::BEST)?"BEST":"DEPTH",
             price_size.Str(),
             instrument.bias_bid?"Bid":"Ask",
             min_qty.Str(),instrument.time_in_force==FIX::TimeInForce_FILL_OR_KILL?"FOK":instrument.time_in_force==FIX::TimeInForce_IMMEDIATE_OR_CANCEL?"IOC":"GTD",
             instrument.expiration_time,
             instrument.enabled?"True":"False"
  );
  //---
  if(instrument.lot_conversion!=1)
    {
    res.Append(", ");
    res.Append(lot_conversion.Str());
    }
  //--- return result
  return(res.Str());
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
inline double Instrument::ReplaceCharWithValue(const std::string &amnt,double multiplier)
  {
  // separator of parameters string
  static char s_param_seps[]="kKmM%";
  StringType64 amnt_str;
  amnt_str.Assign(amnt.c_str());
  //---
  for(UINT pos=0;pos<_countof(s_param_seps)-1;pos++)
    {
    amnt_str.ReplaceChar(s_param_seps[pos],' ');
    }
  amnt_str.TrimSpaces();
  //double value=atof(amnt_str.Str());
  return max(atof(amnt_str.Str())*multiplier,0);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
inline double Instrument::ParseSize(const std::string &amnt)
  {
  double value;
  if(std::regex_match(amnt,std::regex("[0-9]{1,}[.]{0,1}[0-9]{0,}[kKmM%]{0,1}")))
    {
    if(std::regex_match(amnt,std::regex("[0-9]{1,}[.]{0,1}[0-9]{0,}[%]{1,1}")))
      {
      value=ReplaceCharWithValue(amnt,0.01);
      return min(value,1);
      }
    else if(std::regex_match(amnt,std::regex("[0-9]{1,}[.]{0,1}[0-9]{0,}[mM]{1,1}")))
      {
      value=ReplaceCharWithValue(amnt,1000000);
      return value;
      }
    else if(std::regex_match(amnt,std::regex("[0-9]{1,}[.]{0,1}[0-9]{0,}[kK]{1,1}")))
      {
      value=ReplaceCharWithValue(amnt,1000);
      return value;
      }
    //---
    return atof(amnt.c_str());
    }
  //---
  return ParseSize("100k");
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
inline double Instrument::ParsePercentage(const std::string &amnt,bool &percentage)
  {
  double value=0.0;
  //---
  if(std::regex_match(amnt,std::regex("[0-9]{1,}[.]{0,1}[0-9]{0,}[%]{1,1}")))
    {
    percentage=true;
    value=ReplaceCharWithValue(amnt,0.01);
    return min(value,1);
    }
  //---
  return atof(amnt.c_str());
  }
//+------------------------------------------------------------------+
//| Array of trade symbols                                           |
//+------------------------------------------------------------------+
typedef TMTArray<Instrument>    InstrumentRecordsArray;
//+------------------------------------------------------------------+