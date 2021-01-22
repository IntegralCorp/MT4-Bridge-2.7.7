//+------------------------------------------------------------------+
//|                                                 MetaTrader 5 API |
//|                   Copyright 2000-2019, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include <string.h>
#include <math.h>
//+------------------------------------------------------------------+
//| Volume constants                                                 |
//+------------------------------------------------------------------+
#define MTAPI_VOLUME_DIV        (100.0)
#define MTAPI_VOLUME_DIGITS     (2)
#define MTAPI_VOLUME_MAX        ((UINT)10000000000)
//+------------------------------------------------------------------+
//| Price constants                                                  |
//+------------------------------------------------------------------+
#define MTAPI_PRICE_MAX         (100000000)
#define MTAPI_PRICE_DIGITS_MAX  (11)
//+------------------------------------------------------------------+
//| Mathematical operations                                          |
//+------------------------------------------------------------------+
class SMTMath
  {
private:
   static const double s_decimal[16];
   static const double s_fdecimal[16];
   static const double s_rounder_math;

public:
   //--- common functions
   static double     DecPow(INT digits);
   //--- price functions
   static double     PriceNormalize(const double price,UINT digits);
   static INT        PriceToInt(const double price,const UINT digits);
   static UINT       PriceToIntPos(const double price,const UINT digits);
   static double     PriceToDouble(const INT value,UINT digits);
   //--- volume functions
   static UINT       VolumeToInt(const double volume);
   static double     VolumeToDouble(const UINT volume);
   static double     VolumeToSize(const UINT volume,double contract_size);
   static UINT       VolumeFromSize(const double size,double contract_size);
   //--- money digits
   static double     MoneyAdd(const double left,const double right,const UCHAR digits);
   static bool       MoneyEqual(const double left,const double right,const UCHAR digits);
   static UINT       MoneyDigits(LPCSTR currency);
  };
//+------------------------------------------------------------------+
//| 16 powers                                                        |
//+------------------------------------------------------------------+
const __declspec(selectany) double SMTMath::s_decimal[16]=
  {
   1.0,
   10.0,
   100.0,
   1000.0,
   10000.0,
   100000.0,
   1000000.0,
   10000000.0,
   100000000.0,
   1000000000.0,
   10000000000.0,
   100000000000.0,
   1000000000000.0,
   10000000000000.0,
   100000000000000.0,
   1000000000000000.0,
   };
//+------------------------------------------------------------------+
//| Negative 16 powers                                               |
//+------------------------------------------------------------------+
const __declspec(selectany) double SMTMath::s_fdecimal[16]=
  {
   1.0,
   0.1,
   0.01,
   0.001,
   0.0001,
   0.00001,
   0.000001,
   0.0000001,
   0.00000001,
   0.000000001,
   0.0000000001,
   0.00000000001,
   0.000000000001,
   0.0000000000001,
   0.00000000000001,
   0.000000000000001,
   };
//+------------------------------------------------------------------+
//| Rounder constant                                                 |
//+------------------------------------------------------------------+
const __declspec(selectany) double SMTMath::s_rounder_math=0.5000001;
//+------------------------------------------------------------------+
//| Decimal power                                                    |
//+------------------------------------------------------------------+
inline double SMTMath::DecPow(INT digits)
  {
//--- check sign
   if(digits>0 && digits<_countof(s_decimal))
      return(s_decimal[digits]);
   else
      if(digits<0 && -digits<_countof(s_fdecimal))
         return(s_fdecimal[-digits]);
//--- use standard pow
   return(pow(10.0,digits));
  }
//+------------------------------------------------------------------+
//| Price normalization                                              |
//+------------------------------------------------------------------+
inline double SMTMath::PriceNormalize(const double val,UINT digits)
  {
   double dbl_integer;
//--- check digits
   if(digits>MTAPI_PRICE_DIGITS_MAX)
      digits=MTAPI_PRICE_DIGITS_MAX;
//--- calculate dividers
   double p=s_decimal[digits];
   dbl_integer=double(__int32(val));
   double dbl_fract=(val-dbl_integer)*p;
//--- check sign
   if(val>0) dbl_fract+=s_rounder_math;
   else      dbl_fract-=s_rounder_math;
//--- calc fractional part
   dbl_fract=double(__int32(dbl_fract));
//--- summary
   return(dbl_integer+dbl_fract/p);
  }
//+------------------------------------------------------------------+
//| Price conversion from double to integer                          |
//+------------------------------------------------------------------+
inline INT SMTMath::PriceToInt(const double price,const UINT digits)
  {
//--- check
   if(digits>MTAPI_PRICE_DIGITS_MAX || price<=0.0 || price>_I32_MAX)
      return(0);
//--- calculate
   return((INT)((price>=0.0) ? (price*s_decimal[digits]+s_rounder_math):(price*s_decimal[digits]-s_rounder_math)));
  }
//+------------------------------------------------------------------+
//| Price conversion from double to integer                          |
//+------------------------------------------------------------------+
inline UINT SMTMath::PriceToIntPos(const double price,const UINT digits)
  {
//--- check
   if(price<=0.0)
      return(PriceToInt(price,digits));
//--- calculate
   return((UINT)(price*s_decimal[digits]+s_rounder_math));
  }
//+------------------------------------------------------------------+
//| Price conversion from integer to double                          |
//+------------------------------------------------------------------+
inline double SMTMath::PriceToDouble(const INT value,UINT digits)
  {
//--- check
   if(digits>MTAPI_PRICE_DIGITS_MAX)
      digits=MTAPI_PRICE_DIGITS_MAX;
//--- divide
   return(PriceNormalize(double(value)/s_decimal[digits],digits));
  }
//+------------------------------------------------------------------+
//| Volume conversion from double to integer                         |
//+------------------------------------------------------------------+
inline UINT SMTMath::VolumeToInt(const double volume)
  {
   return(PriceToIntPos(volume,MTAPI_VOLUME_DIGITS));
  }
//+------------------------------------------------------------------+
//| Volume conversion from integer to double                         |
//+------------------------------------------------------------------+
inline double SMTMath::VolumeToDouble(const UINT volume)
  {
   return(PriceNormalize(volume/double(MTAPI_VOLUME_DIV),MTAPI_VOLUME_DIGITS));
  }
//+------------------------------------------------------------------+
//| Volume conversion from lots to amount                            |
//+------------------------------------------------------------------+
inline double SMTMath::VolumeToSize(const UINT volume,double contract_size)
  {
   return(PriceNormalize(volume/double(MTAPI_VOLUME_DIV)*contract_size,MTAPI_VOLUME_DIGITS));
  }
//+------------------------------------------------------------------+
//| Volume conversion from amount to lots                            |
//+------------------------------------------------------------------+
inline UINT SMTMath::VolumeFromSize(const double size,double contract_size)
  {
   return((UINT)PriceNormalize((size/contract_size)*double(MTAPI_VOLUME_DIV),0));
  }
//+------------------------------------------------------------------+
//| Money add                                                        |
//+------------------------------------------------------------------+
inline double SMTMath::MoneyAdd(const double left,const double right,const UCHAR digits)
  {
   return(PriceNormalize(left+right,digits));
  }
//+------------------------------------------------------------------+
//| Money Compare                                                    |
//+------------------------------------------------------------------+
inline bool SMTMath::MoneyEqual(const double left,const double right,const UCHAR digits)
  {
   return(fabs(left-right)<DecPow(-(digits+1)));
  }
//+------------------------------------------------------------------+
//| Digits by currency                                               |
//+------------------------------------------------------------------+
inline UINT SMTMath::MoneyDigits(LPCSTR currency)
  {
//--- check param
   if(currency)
     {
      if(_stricmp(currency,"JPY")==0) return(0);
      if(_stricmp(currency,"VND")==0) return(0);
     }
//--- 2 digits by default
   return(2);
  }
//+------------------------------------------------------------------+
