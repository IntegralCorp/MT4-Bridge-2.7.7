//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include <iostream>
#include <fstream>
//---
#include "..\..\..\API\MT4APITools.h"
#include "..\..\..\QuickFix\SessionSettings.h"
#include "..\..\..\resource.h"
#include "..\..\..\IO\Log.h"
#include "..\..\..\Common\Const.h"
//+------------------------------------------------------------------+
//| Fix session configuration                                        |
//+------------------------------------------------------------------+
class CFixSessionSettings
  {
  private:
    struct FixSettingsSorter
      {
      bool operator()(const std::string &a,const std::string &b)
        {
        return GetPos(a)<GetPos(b);
        }
      };
    typedef std::map<std::string,std::string,FixSettingsSorter>  SettingsSorter;

  private:
    CMTSync                m_sync;                 // 
    char                   m_filename[MAX_PATH];   // 
    FIX::SessionSettings   m_settings;             // 
    int                    m_cfg_total;            // 
    int                    m_cfg_max;              // 
    StringTypePath         base_directory;

  public:
    CFixSessionSettings();
    ~CFixSessionSettings();
    //--- initialization of the database (reading the config file)
    bool              Initialize(const StringType &path,FIX::SessionSettings &settings);
    //--- access to records
    bool              GetString(const FIX::SessionID &session_id,LPCSTR search,FIX::FieldBase &field);

  private:
    //--- extract xml
    void              ExtractXML(LPCSTR strCustomResName,int nResourceId,LPCSTR strOutputName);
    bool              LoadSettings(FIX::SessionSettings &settings);
    bool              SaveSettings(const FIX::SessionSettings &settings);
    //---
    static int        GetPos(const std::string &value);
  };
 extern CFixSessionSettings FixSessionSettings;
//+------------------------------------------------------------------+
