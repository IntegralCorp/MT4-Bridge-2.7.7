//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include <new.h>
#include <windows.h>
//---
#include "..\..\API\MT4APITools.h"
#include "..\..\Util\threadmap.h"
#include "Group.h"
//+------------------------------------------------------------------+
//| Rule records                                                     |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct Route
  {
  char       text[1024];              // for checking purposes
  int        index;                   // rule index
  char       group[16];               // group name
  //---                               
  char       symbol[64];              // symbol
  //--- Group::EnCoverMode
  UINT       cover_mode;              // Additional Exec Inst mode
  char       login[128];              // login
  int        min_size;                // min size
  int        max_size;                // max size
  bool       enabled;                 // enabled
  //--- fill
  static bool Fill(StringType &text,Route *rec);
  //--- print route
  static LPCSTR    Print(const Route &order,StringType &res);
  //---
  bool operator==(const Route &a) const
    {
    return(StringType::Compare(text,a.text)==0);
    }
  //--- check match
  static bool  isMatch(int _Login,std::string &_Group,std::string &_Symbol,double _Lots,const Route &route);

  private:
    //--- search
    static bool Search(const char *first,const char *second);
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Fill roule fields                                                |
//+------------------------------------------------------------------+
inline bool Route::Fill(StringType &text,Route *rec)
  {
  StringType64 elem;
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
  if(num_tokens=strtok.countTokens()!=7)
    return(false);
  //--- parse login
  StringType::Copy(rec->login,strtok.nextToken().c_str());
  //--- parse group
  StringType::Copy(rec->group,strtok.nextToken().c_str());
  //--- parse symbol
  StringType::Copy(rec->symbol,strtok.nextToken().c_str());
  //---
  rec->min_size=strtok.nextIntToken();
  rec->max_size=strtok.nextIntToken();
  //--- get cover mode
  elem.Assign(strtok.nextToken().c_str());
  //--- set cover mode
  if(elem.CompareNoCase("STP")==0||elem.CompareNoCase("True")==0)
    rec->cover_mode=Group::EnCoverMode::STP;
  else if(elem.CompareNoCase("NC")==0)
    rec->cover_mode=Group::EnCoverMode::NC;
  else if(elem.CompareNoCase("NCNPV")==0||elem.CompareNoCase("False")==0)
    rec->cover_mode=Group::EnCoverMode::NCNPV;
  else if(elem.CompareNoCase("YM")==0||elem.CompareNoCase("WH")==0)
    rec->cover_mode=Group::EnCoverMode::WH;
  else if(elem.CompareNoCase("WHNPV")==0)
    rec->cover_mode=Group::EnCoverMode::WHNPV;
  //---
  elem.Assign(strtok.nextToken().c_str());
  rec->enabled=(elem.CompareNoCase("True")==0);
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| Print rule                                                       |
//+------------------------------------------------------------------+
inline LPCSTR Route::Print(const Route &record,StringType &res)
  {
  StringType16 cover_mode;
  //--- clear resulting string
  res.Clear();
  //---
  Group::PrintCoverMode(record.cover_mode,cover_mode);
  res.Format("%s, %s, %s, %d, %d, %s, %s\r\n",
             record.login,
             record.group,
             record.symbol,
             record.min_size,
             record.max_size,
             cover_mode.Str(),
             record.enabled?"True":"False");
  //--- return result
  return(res.Str());
  }
//+------------------------------------------------------------------+
//| mathc?                                                           |
//+------------------------------------------------------------------+
inline bool Route::isMatch(int _Login,std::string &_Group,std::string &_Symbol,double _Lots,const Route &route)
  {
  bool res=true;
  //---
  if(!route.enabled)
    return(false);
  // Check Strings for Wildcard matching
  res=Route::Search(route.login,std::to_string(_Login).c_str());
  res=res&&Route::Search(route.group,_Group.c_str());
  res=res&&Route::Search(route.symbol,_Symbol.c_str());
  // Check Volume is within Limits
  res=res&&(route.min_size<0||route.min_size<=_Lots);
  res=res&&(route.max_size<0||route.max_size>=_Lots);
  //--- return result
  return(res);
  }
//+------------------------------------------------------------------+
//|  The main function that checks if two given strings              |
//|  match. The first string may contain wildcard characters         |
//+------------------------------------------------------------------+
inline bool Route::Search(const char *first,const char *second)
  {
  // If we reach at the end of both strings, we are done
  if(*first=='\0'&&*second=='\0')
    return true;
  // Make sure that the characters after '*' are present
  // in second string. This function assumes that the first
  // string will not contain two consecutive '*'
  if(*first=='*'&&*(first+1)!='\0'&&*second=='\0')
    return false;
  // If the first string contains '!', skip
  if(*first=='!')
    return !Route::Search(first+1,second);
  // If the first string contains '?', or current characters
  // of both strings match
  if(*first=='?'||*first==*second)
    return Route::Search(first+1,second+1);
  // If there is *, then there are two possibilities
  // a) We consider current character of second string
  // b) We ignore current character of second string.
  if(*first=='*')
    return Route::Search(first+1,second)||Search(first,second+1);
  //---
  return false;
  }
//+------------------------------------------------------------------+
//| Array of trade rules                                             |
//+------------------------------------------------------------------+
typedef TMTArray<Route>        RouteRecordsArray;
typedef threadmap<int,Route>   RoutesMap;
//+------------------------------------------------------------------+
