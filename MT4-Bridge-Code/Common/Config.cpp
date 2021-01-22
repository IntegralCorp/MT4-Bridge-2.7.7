#pragma once
#include "StdAfx.h"
#include "Config.h"
#include "..\API\MT4APITools.h"
#include "..\IO\Log.h"

//CIniConfig ExtConfig;
//+------------------------------------------------------------------+
//|  Constructor                                                     |
//+------------------------------------------------------------------+
CIniConfig::CIniConfig()
  {
  //---
  m_Settings.clear();
  m_FileName.clear();
  }
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
CIniConfig::~CIniConfig()
  {
  //---
  Save();
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
std::string CIniConfig::Trim(std::string &value,const std::string &drop)
  {
  std::string result=value.erase(value.find_last_not_of(drop)+1);
  //---
  return result.erase(0,result.find_first_not_of(drop));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CIniConfig::Parse(std::string &line)
  {
  if(line.substr(0,1).compare(";")==0||line.compare("")==0)
    return;

  std::string name,value;
  std::string::size_type location;

  //LOG_INFO(pluginLog,"Config.h:Parse:line=%s",line);
  location=line.find("=",0);
  if(location!=std::string::npos)
    {
    name=Trim(line.substr(0,location));
    value=Trim(line.substr(location+1,line.length()));
    //LOG_INFO(pluginLog, "Config.h:Parse:m_Setting:name=%s, value=%s", name, value);
    m_Settings[name]=value;
    }
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CIniConfig::Save()
  {
  std::ofstream stream;
  stream.open(m_FileName.c_str(),std::ios_base::out|std::ios_base::trunc);

  if(!stream.is_open())
    {
    // mt4log(4, "Error opening file %s", m_FileName.c_str()); // CmdErr == 4
    return false;
    }
  else  // DEBUG ONLY
    {
    // mt4log(0, "File opened %s", m_FileName.c_str());
    }

  std::map<std::string,std::string>::iterator line;

  for(line=m_Settings.begin(); line!=m_Settings.end(); line++)
    {
    //if (((*line).first).compare(name) == 0)
    //	stream  << name  << " = " << value <<  std::endl;
    //else
    stream<<(*line).first<<" = "<<(*line).second<<std::endl;
    } // for
  stream.close();
  return true;
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CIniConfig::SetFilename(const char *fileName)
  {
  //---
  m_FileName.assign(fileName);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
// Don't forget about Unicode max path http://msdn.microsoft.com/en-us/library/aa365247%28v=VS.85%29.aspx
void CIniConfig::SetFilename(HANDLE hModule)
  {
  TCHAR path[MAX_PATH];
  //---  
  GetModuleFileName((HMODULE)hModule,path,MAX_PATH);

  LOG_INFO(pluginLog,"Config.h:SetFilename:GetModuleFileName((HMODULE)hModule, path, MAX_PATH)-> path=%s",path);
  PathRenameExtension(path,".ini");
  //--- under lock
  m_sync.Lock();;
  //---
  m_FileName.assign(path);
  LOG_INFO(pluginLog,"Config.h:SetFilename:m_FileName=%s",m_FileName);
  //---
  m_sync.Unlock();
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CIniConfig::Load()
  {
  //---
  if(m_FileName.empty())
    return;

  std::string line;
  std::ifstream stream;
  stream.open(m_FileName.data(),std::ios::in);

  if(!stream.is_open())
    return;

  while(!stream.eof())
    {
    getline(stream,line);
    LOG_INFO(pluginLog,"Config.h:Load:line=%s",line);
    Parse(line);
    }
  stream.close();
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
std::string CIniConfig::Get(const char *name,const char *defValue)
  {
  //---
  return (m_Settings.find(name)!=m_Settings.end())?m_Settings[name]:defValue;
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int CIniConfig::Get(const char *name,int defValue)
  {
  //---
  int result=(m_Settings.find(name)!=m_Settings.end())?stoi(m_Settings[name]):defValue;
  return result;

  return (errno==ERANGE||errno==EINVAL)?defValue:result;
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
std::string CIniConfig::GetOrSet(const char *name,const char *defValue)
  {
  //---
  if(m_Settings.find(name)==m_Settings.end())
    Set(name,defValue);

  return (m_Settings.find(name)!=m_Settings.end())?m_Settings[name]:defValue;
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int CIniConfig::GetOrSet(const char *name,int defValue)
  {
  //---
  if(m_Settings.find(name)==m_Settings.end())
    {
    std::stringstream ss;
    std::string str;
    ss<<defValue;
    ss>>str;

    Set(name,str.c_str());
    }

  int result=(m_Settings.find(name)!=m_Settings.end())?stoi(m_Settings[name]):defValue;
  return result;

  return (errno==ERANGE||errno==EINVAL)?defValue:result;
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
std::string CIniConfig::GetOrException(const char *name)
  {
  //---
  if(m_Settings.find(name)!=m_Settings.end())
    {
    return m_Settings[name];
    }
  else
    {
    std::exception ex(std::string(std::string("Parameter [")
                      +name+"] not found in config file "+m_FileName).c_str());
    throw ex;
    }
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CIniConfig::GetByIndex(const int index,std::string &name,std::string &value)
  {
  //---
  if(index>=(int)m_Settings.size())
    return false;
  //---
  std::map<std::string,std::string>::iterator i(m_Settings.begin());
  std::advance(i,index);
  //---
  name.assign(i->first.c_str());
  value.assign(i->second.c_str());
  //---
  return true;
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CIniConfig::Set(const char *name,const char *value)
  {
  //---
  m_Settings[name]=value;
  return Save();
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CIniConfig::Remove(const char *name)
  {
  //---
  m_Settings.erase(name);
  return Save();
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int CIniConfig::Size()
  {
  //---
  return m_Settings.size();
  }
//+------------------------------------------------------------------+