//+------------------------------------------------------------------+
//|                                                    DealerChecker |
//|                   Copyright 2001-2014, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
 #include "stringfile.h"
#include <minwindef.h>
//#include <minwinbase.h>
//#include <synchapi.h>
#include "..\include\MT4ServerAPI.h"
//+------------------------------------------------------------------+
//| Simple synchronizer                                              |
//+------------------------------------------------------------------+
class CSync
{
private:
	CRITICAL_SECTION  m_cs;
public:
	CSync()   { ZeroMemory(&m_cs, sizeof(m_cs)); InitializeCriticalSection(&m_cs); }
	~CSync()  { DeleteCriticalSection(&m_cs);   ZeroMemory(&m_cs, sizeof(m_cs)); }
	inline void       Lock()   { EnterCriticalSection(&m_cs); }
	inline void       Unlock() { LeaveCriticalSection(&m_cs); }
};
//+------------------------------------------------------------------+
//| Simple configuration                                             |
//+------------------------------------------------------------------+
class CConfiguration
{
private:
	CSync             m_sync;                 // syncronizer
	char              m_filename[MAX_PATH];   // filename
	PluginCfg        *m_cfg;                  // ìàññèâ çàïèñåé
	int               m_cfg_total;            // îáùåå êîëè÷åñòâî çàïèñåé
	int               m_cfg_max;              // ìàêñèìàëüíîå êîëè÷åñòâî çàïèñåé

public:
	CConfiguration();
	~CConfiguration();
	//--- load
	void              Load(LPCSTR filename);
	//--- oprateions
	int               Add(const PluginCfg* cfg);
	int               Set(const PluginCfg *values, const int total);
	int               Get(LPCSTR name, PluginCfg* cfg);
	int               Next(const int index, PluginCfg* cfg);
	int               Delete(LPCSTR name);
	inline int        Total(void) { m_sync.Lock(); int total = m_cfg_total; m_sync.Unlock(); return(total); }

	int               GetInteger(LPCSTR name, int *value, LPCSTR defvalue = NULL);
	int               GetString(LPCSTR name, LPTSTR value, const int maxlen, LPCSTR defvalue = NULL);

private:
	void              Save(void);
	PluginCfg*        Search(LPCSTR name);
	static int        SortByName(const void *left, const void *right);
	static int        SearchByName(const void *left, const void *right);
};

extern CConfiguration    ExtConfig;
extern char              ExtProgramPath[250];
extern CServerInterface *ExtServer;
//+------------------------------------------------------------------+
