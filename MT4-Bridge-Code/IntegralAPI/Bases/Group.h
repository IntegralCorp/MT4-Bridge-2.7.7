//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include <new.h>
#include "..\..\API\MT4APITools.h"
#include "..\..\Util\threadmap.h"
#include "..\..\Common\StringTokenizer.h"
//+------------------------------------------------------------------+
//| Record for storing Groups                                        |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct Group
  {
  //---
  enum EnCoverMode
    {
    STP=1,
    NC=2,
    NCNPV=3,
    WH=4,
    WHNPV=5
    };
  char          text[1024];        // for checking purposes
  char          group[16];         // group name
  //---         
  char          short_name[64];    // [SESSION] ShortName
  UINT          cover_mode;        // coverage mode
  char          on_behalf[128];    // on behalf of xxx
  bool          enabled;           // enabled
  //--- security group settings
  ConGroupSec   secgroups[MAX_SEC_GROUPS];
  //--- fill
  static bool   Fill(StringType &text,Group *rec);
  static bool   Fill(const ConGroup &group_cfg,Group &group);
  //--- print cover mode
  static LPCSTR PrintCoverMode(const UINT cover_mode,StringType &res);
  //--- print group
  static LPCSTR Print(const Group &group,StringType &res);
  //---
  bool operator==(const Group &a) const
    {
    return(StringType::Compare(text,a.text)==0);
    }
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Fill group fields                                                |
//+------------------------------------------------------------------+
inline bool Group::Fill(StringType &text,Group *rec)
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
  num_tokens=strtok.countTokens();
  //--- parse shortname
  StringType::Copy(rec->short_name,strtok.nextToken().c_str());
  //---
  elem.Assign(strtok.nextToken().c_str());
  //---
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
  if(num_tokens==3)
    {
    elem.Assign(strtok.nextToken().c_str());
    rec->enabled=(elem.CompareNoCase("True")==0);
    }
  else if(num_tokens==4)
    {
    StringType::Copy(rec->on_behalf,strtok.nextToken().c_str());
    //---
    elem.Assign(strtok.nextToken().c_str());
    rec->enabled=(elem.CompareNoCase("True")==0);
    }
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| Fill group fields                                                |
//+------------------------------------------------------------------+
inline bool Group::Fill(const ConGroup &group_cfg,Group &group)
  {
  StringType64 elem;
  //---
  group.cover_mode=Group::EnCoverMode::STP;
  group.enabled   =true;
  if(StringType::CheckGroupMask("manager*",group_cfg.group)||
     StringType::CheckGroupMask("demo*",group_cfg.group)||
     StringType::CheckGroupMask("preliminary*",group_cfg.group))
    {
    group.enabled=false;
    }
  memcpy(group.secgroups,group_cfg.secgroups,sizeof(group_cfg.secgroups));
  StringType::Copy(group.group,group_cfg.group);
  //---
  return(true);
  }
//+------------------------------------------------------------------+
//| Print order type                                                 |
//+------------------------------------------------------------------+
inline LPCSTR Group::PrintCoverMode(const UINT cover_mode,StringType &res)
  {
  //--- form the string value of cover mode
  switch(cover_mode)
    {
    case STP: res.Assign("STP");    break;
    case NC: res.Assign("NC");     break;
    case NCNPV: res.Assign("NCNPV");  break;
    case WH: res.Assign("WH");     break;
    case WHNPV: res.Assign("WHNPV");  break;
    default: res.Assign("");       break;
    }
  //--- return result
  return(res.Str());
  }
//+------------------------------------------------------------------+
//| Print rule                                                       |
//+------------------------------------------------------------------+
inline LPCSTR Group::Print(const Group &group,StringType &res)
  {
  StringType16  cover_mode;
  StringType256 on_behalf;
  //--- clear resulting string
  res.Clear();
  //---
  on_behalf.Assign(group.on_behalf);
  Group::PrintCoverMode(group.cover_mode,cover_mode);
  if(!on_behalf.Empty())
    res.Format("%s=%s, %s, %s, %s",
               group.group,
               group.short_name,
               cover_mode.Str(),
               on_behalf.Str(),
               group.enabled?"True":"False");
  else
     res.Format("%s=%s, %s, %s",
               group.group,
               group.short_name,
               cover_mode.Str(),
               group.enabled?"True":"False");
  //--- return result
  return(res.Str());
  }
//+------------------------------------------------------------------+
//| Array of trade groups                                            |
//+------------------------------------------------------------------+
typedef TMTArray<Group>              GroupRecordsArray;
typedef threadmap<std::string,Group> GroupsMap;
//+------------------------------------------------------------------+