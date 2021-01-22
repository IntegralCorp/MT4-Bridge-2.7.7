//+------------------------------------------------------------------+
//|                                                    DealerChecker |
//|                   Copyright 2001-2014, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "Configuration.h"
#include <fileapi.h>
#include <stdlib.h>


CConfiguration ExtConfig;
char           ExtProgramPath[250] = "";
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CConfiguration::CConfiguration() : m_cfg(NULL), m_cfg_total(0), m_cfg_max(0)
{
	m_filename[0] = 0;
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CConfiguration::~CConfiguration()
{
	m_sync.Lock();
	//---
	if (m_cfg != NULL) { delete[] m_cfg; m_cfg = NULL; }
	m_cfg_total = m_cfg_max = 0;
	//---
	m_sync.Unlock();
}
//+------------------------------------------------------------------+
//| Ìû òîëüêî ÷èòàåì êîíôèãóðàöèþ, íî íè÷åãî íå ìåíÿåì â íåé         |
//+------------------------------------------------------------------+
void CConfiguration::Load(LPCSTR filename)
{
	char        tmp[MAX_PATH], *cp, *start;
	CStringFile in;
	PluginCfg   cfg, *buf;
	//--- ïðîâåðêà
	if (filename == NULL) return;
	//--- ñîõðàíèì èìÿ êîíôèãóðàöèîííîãî ôàéëà
	m_sync.Lock();
	COPY_STR(m_filename, filename);
	//--- îòêðîåì ôàéë
	if (in.Open(m_filename, GENERIC_READ, OPEN_EXISTING))
	{
		while (in.GetNextLine(tmp, sizeof(tmp) - 1)>0)
		{
			if (tmp[0] == ';') continue;
			//--- ïðîïóñòèì ïðîáåëû
			start = tmp; while (*start == ' ') start++;
			if ((cp = strchr(start, '=')) == NULL) continue;
			*cp = 0;
			//--- çàíóëèì, áåðåì èìÿ ïàðàìåòðà
			ZeroMemory(&cfg, sizeof(cfg));
			COPY_STR(cfg.name, start);
			//--- îïÿòü ïðîïóñòèì ïðîáåëû
			cp++; while (*cp == ' ') cp++;
			COPY_STR(cfg.value, cp);
			//--- ïðîâåðÿåì
			if (cfg.name[0] == 0 || cfg.value[0] == 0) continue;
			//--- äîáàâëÿåì
			if (m_cfg == NULL || m_cfg_total >= m_cfg_max) // ìåñòî åñòü?
			{
				//--- ïåðåâûäåëèì íîâûé áóôåð
				if ((buf = new PluginCfg[m_cfg_total + 64]) == NULL) break;
				//--- ñêîïèðóåì îñòàòêè èç ñòàðîãî
				if (m_cfg != NULL)
				{
					if (m_cfg_total>0) memcpy(buf, m_cfg, sizeof(PluginCfg)*m_cfg_total);
					delete[] m_cfg;
				}
				//--- ïîäìåíèì áóôåð
				m_cfg = buf;
				m_cfg_max = m_cfg_total + 64;
			}
			//--- êîïèðóåì
			memcpy(&m_cfg[m_cfg_total++], &cfg, sizeof(PluginCfg));
		}
		//--- çàêðûâàåìñÿ
		in.Close();
	}
	//--- òåïåðü âîçüì¸ì è îòñîðòèðóåì ïî èìåíè (÷òîáû èñêàòü áûñòðî)
	if (m_cfg != NULL && m_cfg_total>0) qsort(m_cfg, m_cfg_total, sizeof(PluginCfg), SortByName);
	m_sync.Unlock();
}
//+------------------------------------------------------------------+
//| Ñáðîñ êîíôèãîâ íà äèñê                                           |
//+------------------------------------------------------------------+
void CConfiguration::Save(void)
{
	CStringFile out;
	char        tmp[512];
	//--- çàïèøåì âñå íà äèñê
	m_sync.Lock();
	if (m_filename[0] != 0)
		if (out.Open(m_filename, GENERIC_WRITE, CREATE_ALWAYS))
		{
		if (m_cfg != NULL)
			for (int i = 0; i<m_cfg_total; i++)
			{
			_snprintf(tmp, sizeof(tmp) - 1, "%s=%s\n", m_cfg[i].name, m_cfg[i].value);
			if (out.Write(tmp, strlen(tmp))<1) break;
			}
		//--- çàêðîåì ôàéë
		out.Close();
		}
	m_sync.Unlock();
	//---
}
//+------------------------------------------------------------------+
//| Íåáëîêèðóåìûé ïîèñê ïî èìåíè                                     |
//+------------------------------------------------------------------+
PluginCfg* CConfiguration::Search(LPCSTR name)
{
	PluginCfg *config = NULL;
	//---
	if (m_cfg != NULL && m_cfg_total>0)
		config = (PluginCfg*)bsearch(name, m_cfg, m_cfg_total, sizeof(PluginCfg), SearchByName);
	//---
	return(config);
}
//+------------------------------------------------------------------+
//| Äîáàâëåíèå/ìîäèôèêàöèÿ ïëàãèíà                                   |
//+------------------------------------------------------------------+
int CConfiguration::Add(const PluginCfg *cfg)
{
	PluginCfg *config, *buf;
	//--- ïðîâåðêè
	if (cfg == NULL || cfg->name[0] == 0) return(FALSE);
	//---
	m_sync.Lock();
	if ((config = Search(cfg->name)) != NULL) memcpy(config, cfg, sizeof(PluginCfg));
	else
	{
		//--- ìåñòî åñòü?
		if (m_cfg == NULL || m_cfg_total >= m_cfg_max)
		{
			//--- âûäåëèì ìåñòî
			if ((buf = new PluginCfg[m_cfg_total + 64]) == NULL) { m_sync.Unlock(); return(FALSE); }
			//--- ñêîïèðóåì îñòàòêè èç ñòàðîãî áóôåðà
			if (m_cfg != NULL)
			{
				if (m_cfg_total>0) memcpy(buf, m_cfg, sizeof(PluginCfg)*m_cfg_total);
				delete[] m_cfg;
			}
			//--- çàìåíèì áóôåð
			m_cfg = buf;
			m_cfg_max = m_cfg_total + 64;
		}
		//--- äîáàâëÿåì â êîíåö
		memcpy(&m_cfg[m_cfg_total++], cfg, sizeof(PluginCfg));
		//--- îòñîðòèðóåìñÿ
		qsort(m_cfg, m_cfg_total, sizeof(PluginCfg), SortByName);
	}
	m_sync.Unlock();
	//--- ñîõðàíèìñÿ, ïåðåçà÷èòàåìñÿ
	Save();
	//--- âûõîäèì
	return(TRUE);
}
//+------------------------------------------------------------------+
//| Âûñòàâëÿåì íàáîð íàñòðîåê                                        |
//+------------------------------------------------------------------+
int CConfiguration::Set(const PluginCfg *values, const int total)
{
	//--- ïðîâåðêè
	if (total<0) return(FALSE);
	//---
	m_sync.Lock();
	if (values != NULL && total>0)
	{
		//--- ìåñòî åñòü?
		if (m_cfg == NULL || total >= m_cfg_max)
		{
			//--- óäàëèì ñòàðûé è âûäåëèì íîâûé áóôåð
			if (m_cfg != NULL) delete[] m_cfg;
			if ((m_cfg = new PluginCfg[total + 64]) == NULL)
			{
				m_cfg_max = m_cfg_total = 0;
				m_sync.Unlock();
				return(FALSE);
			}
			//--- âûñòàâèì íîâûé ïðåäåë
			m_cfg_max = total + 64;
		}
		//--- ñêîïèðóåì âñåì ñêîïîì
		memcpy(m_cfg, values, sizeof(PluginCfg)*total);
	}
	//--- âûñòàâèì îáùåå êîëè÷åñòâî è îòñîðòèðóåìñÿ
	m_cfg_total = total;
	if (m_cfg != NULL && m_cfg_total>0) qsort(m_cfg, m_cfg_total, sizeof(PluginCfg), SortByName);
	m_sync.Unlock();
	//--- ñîõðàíèìñÿ
	Save();
	return(TRUE);
}
//+------------------------------------------------------------------+
//| Èùåì êîíôèã ïî èìåíè                                             |
//+------------------------------------------------------------------+
int CConfiguration::Get(LPCSTR name, PluginCfg *cfg)
{
	PluginCfg *config = NULL;
	//--- ïðîâåðêè
	if (name != NULL && cfg != NULL)
	{
		m_sync.Lock();
		if ((config = Search(name)) != NULL) memcpy(cfg, config, sizeof(PluginCfg));
		m_sync.Unlock();
	}
	//--- âåðíåì ðåçóëüòàò
	return(config != NULL);
}
//+------------------------------------------------------------------+
//| Èùåì êîíôèã                                                      |
//+------------------------------------------------------------------+
int CConfiguration::Next(const int index, PluginCfg *cfg)
{
	//--- ïðîâåðêè
	if (cfg != NULL && index >= 0)
	{
		m_sync.Lock();
		if (m_cfg != NULL && index<m_cfg_total)
		{
			memcpy(cfg, &m_cfg[index], sizeof(PluginCfg));
			m_sync.Unlock();
			return(TRUE);
		}
		m_sync.Unlock();
	}
	//--- íåóäà÷à
	return(FALSE);
}
//+------------------------------------------------------------------+
//| delete config                                                    |
//+------------------------------------------------------------------+
int CConfiguration::Delete(LPCSTR name)
{
	PluginCfg *config = NULL;
	//--- ïðîâåðêè
	if (name != NULL)
	{
		m_sync.Lock();
		if ((config = Search(name)) != NULL)
		{
			int index = config - m_cfg;
			if ((index + 1)<m_cfg_total) memmove(config, config + 1, sizeof(PluginCfg)*(m_cfg_total - index - 1));
			m_cfg_total--;
		}
		//--- îòñîðòèðóåìñÿ
		if (m_cfg != NULL && m_cfg_total>0) qsort(m_cfg, m_cfg_total, sizeof(PluginCfg), SortByName);
		m_sync.Unlock();
	}
	//--- âåðíåì ðåçóëüòàò
	return(config != NULL);
}
//+------------------------------------------------------------------+
//| Ñîðòèðîâêà ïî èìåíè                                              |
//+------------------------------------------------------------------+
int CConfiguration::SortByName(const void *left, const void *right)
{
	return strcmp(((PluginCfg*)left)->name, ((PluginCfg*)right)->name);
}
//+------------------------------------------------------------------+
//| Ïîèñê ïî èìåíè                                                   |
//+------------------------------------------------------------------+
int CConfiguration::SearchByName(const void *left, const void *right)
{
	return strcmp((char*)left, ((PluginCfg*)right)->name);
}
//+------------------------------------------------------------------+
//| Èùåì êîíôèã ïî èìåíè                                             |
//+------------------------------------------------------------------+
int CConfiguration::GetInteger(LPCSTR name, int *value, LPCSTR defvalue)
{
	PluginCfg *config = NULL;
	//--- ïðîâåðêè
	if (name != NULL && value != NULL)
	{
		m_sync.Lock();
		if ((config = Search(name)) != NULL) *value = atoi(config->value);
		else
			if (defvalue != NULL)
			{
			m_sync.Unlock();
			//--- ñîçäàäèì íîâóþ çàïèñü
			PluginCfg cfg = { 0 };
			COPY_STR(cfg.name, name);
			COPY_STR(cfg.value, defvalue);
			Add(&cfg);
			//--- âûñòàâèì çíà÷åíèå ïî óìîë÷àíèþ è âåðíåìñÿ
			*value = atoi(cfg.value);
			return(TRUE);
			}
		m_sync.Unlock();
	}
	//--- âåðíåì ðåçóëüòàò
	return(config != NULL);
}
//+------------------------------------------------------------------+
//| Èùåì êîíôèã ïî èìåíè                                             |
//+------------------------------------------------------------------+
int CConfiguration::GetString(LPCSTR name, LPTSTR value, const int maxlen, LPCSTR defvalue)
{
	PluginCfg *config = NULL;
	//--- ïðîâåðêè
	if (name != NULL && value != NULL)
	{
		m_sync.Lock();
		if ((config = Search(name)) != NULL)
		{
			strncpy(value, config->value, maxlen);
			value[maxlen] = 0;
		}
		else
			if (defvalue != NULL)
			{
			m_sync.Unlock();
			//--- ñîçäàäèì íîâóþ çàïèñü
			PluginCfg cfg = { 0 };
			COPY_STR(cfg.name, name);
			COPY_STR(cfg.value, defvalue);
			Add(&cfg);
			//--- âûñòàâèì çíà÷åíèå ïî óìîë÷àíèþ è âåðíåìñÿ
			strncpy(value, cfg.value, maxlen);
			value[maxlen] = 0;
			return(TRUE);
			}
		m_sync.Unlock();
	}
	//--- âåðíåì ðåçóëüòàò
	return(config != NULL);
}
//+------------------------------------------------------------------+
