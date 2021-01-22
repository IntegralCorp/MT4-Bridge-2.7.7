//+------------------------------------------------------------------+
//|                                                 MetaTrader 5 API |
//|                   Copyright 2000-2019, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include <time.h>
#include "MT4APIStr.h"
#include "MT4APIMath.h"
#include "MT4APITime.h"
#include "..\..\include\MT4ServerAPI.h"
//+------------------------------------------------------------------+
//| Format strings                                                   |
//+------------------------------------------------------------------+
class SMTFormat
  {
private:
   //--- constants
   enum constants
     {
      SIG_DECIMAL    ='.',
      SIG_NEGATIVE   ='-',
      SIG_THOUSAND   =' ',
      VOLUME_K       ='K',
      VOLUME_M       ='M',
     };

public:
   //--- error
   static LPCSTR    FormatError(const UINT retcode);
   //--- double to string
   static LPCSTR    FormatDouble(StringType &str,double val,UINT digits);
   static LPCSTR    FormatMoney(StringType &str,double val,UINT digits);
   //--- price to string
   static LPCSTR    FormatPrice(StringType &str,double val,UINT digits,UINT extra_digits=0);
   static LPCSTR    FormatPrices(StringType &str,FeedTick& tick,const UINT digits);
   //--- volume & size
   static LPCSTR    FormatVolumeDouble(StringType &str,const double volume,const bool compact);
   static LPCSTR    FormatVolume(StringType &str,const UINT volume,const bool compact=true);
   static LPCSTR    FormatVolumeOrder(StringType &str,const UINT volume_initial,const UINT volume_current);
   static LPCSTR    FormatSize(StringType &str,const double size,const bool compact=true);
   static LPCSTR    FormatSizeOrder(StringType &str,const double size_initial,const double size_current);
   //--- date & time
   static LPCSTR    FormatDateTime(StringType &str,const SYSTEMTIME &st,bool useTime=true,bool useSec=false);
   static LPCSTR    FormatDateTime(StringType &str,INT64 ctm,bool useTime=true,bool useSec=false);
   static LPCSTR    FormatDateTimeMsc(StringType &str,const SYSTEMTIME &st,bool useTime=true,bool useSec=false);
   static LPCSTR    FormatDateTimeMsc(StringType &str,INT64 ctm,bool useTime=true,bool useSec=false);
   static LPCSTR    FormatTime(StringType &str,INT64 ctm,bool useSec=false);
   static LPCSTR    FormatTimeMsc(StringType &str,INT64 ctm,bool useSec=false);
   //--- IP & port
   static LPCSTR    FormatIP(StringType &str,const UINT ip,const UINT port=0);
   static LPCSTR    FormatIP(StringType &str,const USHORT* ip,const UINT port=0);
   //--- positions
   static LPCSTR    FormatPositionType(StringType &str,const UINT type);
   //--- orders
   static LPCSTR    FormatTradeCommand(StringType &str,const UINT type);
   static LPCSTR    FormatRequestStatus(StringType &str,const UINT status);
   static LPCSTR    FormatTradeRequestFlags(StringType &str,const UINT type);
   static LPCSTR    FormatGtcMode(StringType &str,const UINT type);
   static LPCSTR    FormatTradeRecordReason(StringType &str,UINT reason);
   static LPCSTR    FormatOrderPrice(StringType &str,const double price_order,const double price_trigger,const UINT digits);
   //--- deals
   static LPCSTR    FormatTradeAddMode(StringType &str,const UINT mode);
   static LPCSTR    FormatTradeUpdateMode(StringType &str,const UINT mode);
   //--- 
   static LPCSTR    FormatTradeRecordState(StringType &str,const UINT modify_flags);

private:
   //--- UINT32 to string
   static LPCSTR    FormatDouble(StringType &str,UINT val,UINT digits);
   static LPCSTR    FormatDoubleBrief(StringType &str,UINT val,UINT digits);
   static LPCSTR    FormatMoney(StringType &str,UINT val,UINT digits);
   //--- trim zero
   static void       TrimZeros(StringType &str,UINT extra_digits=UINT_MAX);
  };
//+------------------------------------------------------------------+
//| Retcode description                                              |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatError(const UINT retcode)
  {
//--- error description
   struct ErrorDesc
     {
      UINT          code;
      LPCSTR        desc;
     };
//--- errors
   static ErrorDesc errors[]=
     {
      { RET_OK,                    "Done"                             },
      { RET_OK_NONE,               "OK/None"                          },
      { RET_ERROR,                 "Common error"                     },
      //--- default
      { INT_MAX,                   "Network problem"                },
     };  
//--- search description
   for(UINT i=0;i<_countof(errors);i++)
      if(errors[i].code==retcode) return(errors[i].desc);
//--- default value
   return("Unknown error");
  }
//+------------------------------------------------------------------+
//| Format double value                                              |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatDouble(StringType &str,double val,UINT digits)
  {
   double valdec;
   UINT   digits_int=(digits<10)?digits:10;
//--- convert to integer
   valdec=val*SMTMath::DecPow((int)digits_int);
//--- check size
   if(valdec>=double(_I32_MAX/100i32) || valdec<=double(_I32_MIN/100i32))
     {
      StringType32 fmt;
      int    pos;
      //--- format using CRT
      fmt.Format("%%.0%ulf",digits_int);
      str.Format(fmt.Str(),val);
      //--- decimal point
      pos=str.FindRChar('.');
      if(pos>=0) str.Buffer()[pos]=SIG_DECIMAL;
      //---
      return(str.Str());
     }
//--- format fast
   if(val>=0)
      FormatDouble(str,UINT(valdec+0.5),digits_int);
   else
     {
      FormatDouble(str,UINT(0.5-valdec),digits_int);
      str.Insert(0,SIG_NEGATIVE);
     }
//---
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Format money                                                     |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatMoney(StringType &str,double val,UINT digits)
  {
   double valdec;
//--- convert to integer
   valdec=val*SMTMath::DecPow((int)digits);
//--- check size
   if(valdec>=double(_I32_MAX/100i32) || valdec<=double(_I32_MIN/100i32))
     {
      StringType32 fmt;
      int    pos;
      //--- format using CRT
      fmt.Format("%%.0%ulf",digits);
      str.Format(fmt.Str(),val);
      //--- decimal point
      pos=str.FindRChar('.');
      if(pos>=0) str.Buffer()[pos]=SIG_DECIMAL;
      //--- insert thousands delimeters
      for(pos=str.FindRChar('.')-3;pos>=3;pos-=3)
         str.Insert((UINT)pos,SIG_THOUSAND);
      //---
      return(str.Str());
     }
//--- format fast
   if(val>=0)
      FormatMoney(str,UINT(valdec+0.5),digits);
   else
     {
      FormatMoney(str,UINT(0.5-valdec),digits);
      str.Insert(0,SIG_NEGATIVE);
     }
//---
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Format price                                                     |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatPrice(StringType &str,double val,UINT digits,UINT extra_digits)
  {
//--- format with extra digits
   FormatDouble(str,val,digits+extra_digits);
//--- trim zero
   TrimZeros(str,extra_digits);
//--- return
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Current prices                                                   |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatPrices(StringType &str,FeedTick& tick,const UINT digits)
  {
   StringType32 sbid,sask;
//--- check last prices
   str.Format("%s / %s"     ,FormatPrice(sbid,tick.bid,digits),FormatPrice(sask,tick.ask,digits));
//--- return
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Format trade volume                                              |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatVolumeDouble(StringType &str,const double volume,const bool compact/*=true*/)
  {
//--- check M
   if(compact && volume>=1000000.0)
     {
      //--- 
      str.Format("%.8lf",volume/1000000.0);
      TrimZeros(str,8);
      str.Append(VOLUME_M);
      //--- 
      return(str.Str());
     }
//--- check K
   if(compact && volume>=1000.0)
     {
      //--- 
      str.Format("%.5lf",volume/1000.0);
      TrimZeros(str,5);
      str.Append(VOLUME_K);
      //--- 
      return(str.Str());
     }
//--- normal method
   FormatDouble(str,volume,MTAPI_VOLUME_DIGITS);
   TrimZeros(str,6);
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Format trade volume                                              |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatVolume(StringType &str,const UINT volume,const bool compact/*=true*/)
  {
   return(FormatVolumeDouble(str,SMTMath::VolumeToDouble(volume),compact));
  }
//+------------------------------------------------------------------+
//| Format order volume                                              |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatVolumeOrder(StringType &str,const UINT volume_initial,const UINT volume_current)
  {
   StringType64 vol_filled,vol_init;
//--- 
   str.Format("%s / %s",FormatVolume(vol_init,volume_initial),
                        FormatVolume(vol_filled,volume_initial-volume_current));
//--- summary
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Format trade amount                                              |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatSize(StringType &str,const double volume,const bool compact/*=true*/)
  {
   return(FormatVolumeDouble(str,volume,compact));
  }
//+------------------------------------------------------------------+
//| Format order amount                                              |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatSizeOrder(StringType &str,const double size_initial,const double size_current)
  {
   StringType64 sz_filled,sz_current;
//---
   str.Format("%s / %s",FormatSize(sz_filled,size_initial),
                        FormatSize(sz_current,size_initial-size_current));
//---
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Format date & time                                               |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatDateTime(StringType &str,const SYSTEMTIME &st,bool useTime/*=true*/,bool useSec/*=false*/)
  {
//---
   if(useTime)
     {
      if(useSec)
         str.Format("%04d.%02d.%02d %02d:%02d:%02d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
      else
         str.Format("%04d.%02d.%02d %02d:%02d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute);
     }
   else str.Format("%04d.%02d.%02d",st.wYear,st.wMonth,st.wDay);
//---
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Format date & time                                               |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatDateTime(StringType &str,INT64 ctm,bool useTime/*=true*/,bool useSec/*=false*/)
  {
   tm ttm={};
//---
   if(!SMTTime::ParseTime(ctm,&ttm)) return(str.Str());
//---
   if(useTime)
     {
      if(useSec)
         str.Format("%04d.%02d.%02d %02d:%02d:%02d",ttm.tm_year+1900,ttm.tm_mon+1,ttm.tm_mday,ttm.tm_hour,ttm.tm_min,ttm.tm_sec);
      else
         str.Format("%04d.%02d.%02d %02d:%02d",ttm.tm_year+1900,ttm.tm_mon+1,ttm.tm_mday,ttm.tm_hour,ttm.tm_min);
     }
   else str.Format("%04d.%02d.%02d",ttm.tm_year+1900,ttm.tm_mon+1,ttm.tm_mday);
//---
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Format date & time                                               |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatDateTimeMsc(StringType &str,const SYSTEMTIME &st,bool useTime/*=true*/,bool useSec/*=false*/)
  {
//---
   if(useTime)
     {
      if(useSec)
         str.Format("%04d.%02d.%02d %02d:%02d:%02d.%03u",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
      else
         str.Format("%04d.%02d.%02d %02d:%02d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute);
     }
   else str.Format("%04d.%02d.%02d",st.wYear,st.wMonth,st.wDay);
//---
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Format date & time                                               |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatDateTimeMsc(StringType &str,INT64 ctm,bool useTime/*=true*/,bool useSec/*=false*/)
  {
   tm ttm={};
//---
   if(!SMTTime::ParseTime(ctm/1000,&ttm))
      return(str.Str());
//---
   if(useTime)
     {
      if(useSec)
         str.Format("%04d.%02d.%02d %02d:%02d:%02d.%03d",ttm.tm_year+1900,ttm.tm_mon+1,ttm.tm_mday,ttm.tm_hour,ttm.tm_min,ttm.tm_sec,(int)(ctm%1000));
      else
         str.Format("%04d.%02d.%02d %02d:%02d",ttm.tm_year+1900,ttm.tm_mon+1,ttm.tm_mday,ttm.tm_hour,ttm.tm_min);
     }
   else str.Format("%04d.%02d.%02d",ttm.tm_year+1900,ttm.tm_mon+1,ttm.tm_mday);
//---
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Format time                                                      |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatTime(StringType &str,INT64 ctm,bool useSec/*=false*/)
  {
   tm ttm={};
//---
   if(!SMTTime::ParseTime(ctm,&ttm))
      return(str.Str());
//---
   if(useSec) str.Format("%02d:%02d:%02d",ttm.tm_hour,ttm.tm_min,ttm.tm_sec);
   else       str.Format("%02d:%02d",ttm.tm_hour,ttm.tm_min);
//---
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Format time                                                      |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatTimeMsc(StringType &str,INT64 ctm,bool useSec/*=false*/)
  {
   tm ttm={};
//---
   if(!SMTTime::ParseTime(ctm/1000,&ttm))
      return(str.Str());
//---
   if(useSec) str.Format("%02d:%02d:%02d.%03d",ttm.tm_hour,ttm.tm_min,ttm.tm_sec,(int)(ctm%1000));
   else       str.Format("%02d:%02d",ttm.tm_hour,ttm.tm_min);
//---
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Format IP[:port]                                                 |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatIP(StringType &str,const UINT ip,const UINT port/*=0*/)
  {
   if(port>0) str.Format("%d.%d.%d.%d:%u",ip&255,(ip>>8)&255,(ip>>16)&255,(ip>>24)&255,port);
   else       str.Format("%d.%d.%d.%d",   ip&255,(ip>>8)&255,(ip>>16)&255,(ip>>24)&255);
//---
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Format IP[:port]                                                 |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatIP(StringType &str,const USHORT *ip,const UINT port/*=0*/)
  {
   if(ip)
     {
      if(port>0)
         str.Format("[%04x:04x:04x:04x:04x:04x:04x:04x]:%u",
                    ip[0],ip[1],ip[2],ip[3],ip[4],ip[5],ip[6],ip[7],port);
      else
         str.Format("[%04x:04x:04x:04x:04x:04x:04x:04x]",
                    ip[0],ip[1],ip[2],ip[3],ip[4],ip[5],ip[6],ip[7]);
     }
//---
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Position type                                                    |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatPositionType(StringType &str,const UINT type)
  {
//--- clear
   str.Clear();
//--- check type
   switch(type)
     {
      case OP_BUY  : str.Assign("buy");  break;
      case OP_SELL : str.Assign("sell"); break;
     }
//--- return
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Order type                                                       |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatTradeCommand(StringType &str,const UINT type)
  {
//--- clear
   str.Clear();
//--- check type
   switch(type)
     {
      case OP_BUY            : str.Assign("buy");             break;
      case OP_SELL           : str.Assign("sell");            break;
      case OP_BUY_LIMIT      : str.Assign("buy limit");       break;
      case OP_SELL_LIMIT     : str.Assign("sell limit");      break;
      case OP_BUY_STOP       : str.Assign("buy stop");        break;
      case OP_SELL_STOP      : str.Assign("sell stop");       break;
      case OP_BALANCE        : str.Assign("balance");         break;
      case OP_CREDIT         : str.Assign("credit");          break;
     }
//--- return
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Order status                                                     |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatRequestStatus(StringType &str,const UINT status)
  {
//--- clear
   str.Clear();
//--- check type
   switch(status)
     {
      case DC_EMPTY    :  str.Assign("empty");     break;
      case DC_REQUEST  :  str.Assign("request");   break;
      case DC_LOCKED   :  str.Assign("locked");    break;
      case DC_ANSWERED :  str.Assign("answered");  break;
      case DC_RESETED  :  str.Assign("reseted");   break;
      case DC_CANCELED :  str.Assign("cancelled"); break;
     }
//--- return
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Order filling type                                               |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatTradeRequestFlags(StringType &str,const UINT type)
  {
//--- clear
   str.Clear();
//--- check type
   switch(type)
     {
      case TT_FLAG_NONE     : str.Assign("none");                break;
      case TT_FLAG_SIGNAL   : str.Assign("placed by signal");    break;
      case TT_FLAG_EXPERT   : str.Assign("placed by expert");    break;
      case TT_FLAG_GATEWAY  : str.Assign("placed by gateway");   break;
      case TT_FLAG_MOBILE   : str.Assign("placed by mobile");    break;
      case TT_FLAG_WEB      : str.Assign("placed by web");       break;
      case TT_FLAG_API      : str.Assign("placed by api");       break;
     }
//--- return
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Order expiration type                                            |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatGtcMode(StringType &str,const UINT type)
  {
//--- clear
   str.Clear();
//--- check type
   switch(type)
     {
      case ORDERS_DAILY         : str.Assign("daily");          break;
      case ORDERS_GTC           : str.Assign("gtc");            break;
      case ORDERS_DAILY_NO_STOPS: str.Assign("daily no stops"); break;
     }
//--- return
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Order reason                                                     |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatTradeRecordReason(StringType &str,UINT reason)
  {
//--- clear
   str.Clear();
//--- check type
   switch(reason)
     {
      case TR_REASON_CLIENT            : str.Assign("Client");           break;
      case TR_REASON_EXPERT            : str.Assign("Expert");           break;
      case TR_REASON_DEALER            : str.Assign("Dealer");           break;
      case TR_REASON_SIGNAL            : str.Assign("Signal");           break;
      case TR_REASON_GATEWAY           : str.Assign("Gateway");          break;
      case TR_REASON_MOBILE            : str.Assign("Mobile");           break;
      case TR_REASON_WEB               : str.Assign("Web");              break;
      case TR_REASON_API               : str.Assign("Api");              break;
     }
//--- return
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Order price                                                      |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatOrderPrice(StringType &str,const double price_order,const double price_trigger,const UINT digits)
  {
   StringType64 price,trigger;
//--- clear
   str.Clear();
//--- check trigger price
   if(price_trigger)
     {
      FormatPrice(price,price_order,digits);
      FormatPrice(trigger,price_trigger,digits);
      str.Format("%s (%s)",price.Str(),trigger.Str());
     }
   else
     {
      if(price_order!=0) FormatPrice(str,price_order,digits);
      else               str.Assign("market");
     }
//--- return
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Deal direction                                                   |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatTradeAddMode(StringType &str,const UINT entry)
  {
//--- clear
   str.Clear();
//--- check type   
   switch(entry)
     {
      case OPEN_NEW     : str.Assign("new");      break;
      case OPEN_CLOSE   : str.Assign("close");    break;
      case OPEN_RESTORE : str.Assign("restore");  break;
      case OPEN_API     : str.Assign("api");      break;
      case OPEN_ROLLOVER: str.Assign("rollover"); break;
     }
//--- return
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Deal direction                                                   |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatTradeUpdateMode(StringType &str,const UINT entry)
  {
//--- clear
   str.Clear();
//--- check type   
   switch(entry)
     {
      case UPDATE_NORMAL     : str.Assign("normal");      break;
      case UPDATE_ACTIVATE   : str.Assign("activate");    break;
      case UPDATE_CLOSE      : str.Assign("close");       break;
      case UPDATE_DELETE     : str.Assign("delete");      break;
     }
//--- return
   return(str.Str());
  }
//+------------------------------------------------------------------+
//| Modify flags                                                     |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatTradeRecordState(StringType &str,const UINT modify_flags)
  {
//--- description
   static struct
     {
      UINT     flag;
      LPCSTR  desc;
     } modify_flags_desc[]=
     {
      { TS_OPEN_NORMAL,     "open normal"    },
      { TS_OPEN_REMAND,     "open remainder" },
      { TS_OPEN_RESTORED,   "restored"       },
      { TS_CLOSED_NORMAL,   "closed normal"  },
      { TS_CLOSED_PART,     "closed partial" },
      { TS_CLOSED_BY,       "closed by"      },
      { TS_DELETED,         "deleted"        }
     };
//--- counter
   UINT added=0;
//--- collect description
   str.Clear();
   for(UINT i=0;i<_countof(modify_flags_desc);i++)
     if(modify_flags_desc[i].flag&modify_flags)
       {
        //--- add comma
        if(added++)
          str.Append(", ");
        //--- write
        str.Append(modify_flags_desc[i].desc);
       }
//--- result
   return(str.Str());
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatDouble(StringType &str,UINT val,UINT digits)
  {
   static const char cd[]={ '0','1','2','3','4','5','6','7','8','9',0 };
   char tmp[256]="",*cp,*dst;
   UINT    i,len;
//--- 
   str.Clear();
   cp=tmp;
//--- check
   if(digits>8) return(str.Str());
//--- 
   if(digits>0)
     {
      for(i=0;i<digits;i++)
        {
         *cp++=cd[val%10];
         val/=10;
        }
      *cp++=SIG_DECIMAL;
     }
//--- integer part
   *cp++=cd[val%10];
   for(val/=10,i=1;val!=0;val/=10,i++)
      *cp++=cd[val%10];
   *cp=0;
//--- check size
   len=UINT(cp-tmp);
   if(len>=str.Max()-1) return(str.Str());
//--- string reverse 
   dst=str.Buffer(); cp=tmp+len-1;
   while(cp>=tmp) *dst++=*cp--;
   *dst=0;
   str.Refresh();
//---
   return(str.Str());
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
inline LPCSTR SMTFormat::FormatMoney(StringType &str,UINT val,UINT digits)
  {
   static const char cd[]={ '0','1','2','3','4','5','6','7','8','9',0 };
   char tmp[256]="",*cp,*dst;
   UINT    i,len;
//--- 
   str.Clear();
   cp=tmp;
//--- checks
   if(digits>8) return(str.Str());
//---
   if(digits>0)
     {
      for(i=0;i<digits;i++)
        {
         *cp++=cd[val%10];
         val/=10;
        }
      *cp++=SIG_DECIMAL;
     }
//--- integer part
   *cp++=cd[val%10];
   for(val/=10,i=1;val!=0;val/=10,i++)
     {
      if(i%3==0) *cp++=SIG_THOUSAND;
      *cp++=cd[val%10];
     }
   *cp=0;
//--- check size
   len=UINT(cp-tmp);
   if(len>=str.Max()-1) return(str.Str());
//--- reverse string
   dst=str.Buffer(); cp=tmp+len-1;
   while(cp>=tmp) *dst++=*cp--;
   *dst=0;
   str.Refresh();
//---
   return(str.Str());
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
inline void SMTFormat::TrimZeros(StringType &str,UINT extra_digits)
  {
   UINT len=str.Len(),digits=0;
//--- remove zero
   while(len>0 && str[len-1]=='0' && digits<extra_digits) { len--; digits++; }
   str.Trim(len);
//--- remove points
   if(len>0 && str[len-1]=='.') str.Trim(len-1);
  }
//+------------------------------------------------------------------+