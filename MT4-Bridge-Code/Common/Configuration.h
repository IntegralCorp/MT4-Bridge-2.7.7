//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include "..\include\MT4ServerAPI.h"
#include "..\API\MT4APITools.h"
#include "Logger.h"
#include "Misc\Common.h"
#include "..\Tools\StringFile.h"
#include "..\IO\Log.h"
#include "Const.h"
//+------------------------------------------------------------------+
//| Plugin configuration processing class                            |
//+------------------------------------------------------------------+
class CConfiguration
  {
protected:
   PluginCfg        *m_cfgs;           // parameters
   int               m_cfgs_total;     // count of parameters
   int               m_cfgs_max;       // maximum of parameters
   PluginCfg       **m_cfgs_index;     // index of parameters
   char              m_cfgname[64];    // config file name
   CMTSync           m_sync;           // synchronizer

public:
                     CConfiguration();
                    ~CConfiguration();
   //--- initialization of the database (reading the config file)
   int               Total(void);
   int               Add(const int pos,const PluginCfg *cfg);
   int               Delete(LPCSTR name);
   int               Next(const int index,PluginCfg* cfg);
   int               Get(const char *name,PluginCfg* cfg,const int pos=0);
   int               Set(const PluginCfg *values,const int total);
   //---
   //---- high-level access
   int               GetInteger(const int pos,const char *name,int *value   ,const char *defvalue=NULL);
   int               GetString(const int pos ,const char *name,char *value  ,const int size,const char *defvalue=NULL);
   int               GetFloat(const int pos  ,const char *name,double *value,const char *defvalue=NULL);
   int               GetBool(const int pos  ,const char *name,bool *value,const char *defvalue=NULL);

protected:
   void              Load();
   int               Save(void);
   void              Shutdown();
   virtual void      Reload() {}
   PluginCfg*        Search(LPCSTR name);
   static int        SortByName(const void *left,const void *right);
   static int        SortIndex(const void *left,const void *right);
   static int        SearchByName(const void *left,const void *right);
  };
//+------------------------------------------------------------------+
