#pragma once
#include <string>
#include <map>
#include <exception>
#include <sstream>
#include <fstream>
#include <errno.h>
//---
#include <shlwapi.h>
// #pragma comment(lib, "shlwapi") // better include shlwapi.lib in project
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
class CIniConfig
  {
  protected:
	 std::map<std::string,std::string> m_Settings;	// settings map to store parameter / value
	 std::string                       m_FileName;	// filename
	 CMTSync                           m_sync;

  public:
	          CIniConfig();
	 virtual ~CIniConfig();
	 //---
	 void        SetFilename(const char *file_name);
	 // Don't forget about Unicode max path http://msdn.microsoft.com/en-us/library/aa365247%28v=VS.85%29.aspx
	 void        SetFilename(HANDLE hModule);
	 void        Load();
	 //---
	 std::string Get(const char *name,const char *def_value="");
	 int         Get(const char *name,int def_value);
	 std::string GetOrSet(const char *name,const char *def_value="");
	 int         GetOrSet(const char *name,int def_value);
	 std::string GetOrException(const char *name);
	 bool        GetByIndex(const int index,std::string &name,std::string &value);
	 bool        Set(const char *name,const char *value);
	 bool        Remove(const char *name);
	 int         Size();

  protected:
	 bool Save();
	 //---
	 std::string Trim(std::string &value,const std::string &drop=" ");
	 void Parse(std::string &line);	 
  };
//+------------------------------------------------------------------+