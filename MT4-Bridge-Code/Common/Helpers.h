//+------------------------------------------------------------------+
//|                                         MetaTrader 5 NFA.Reports |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once

#include <string>
#include <vector>
//+------------------------------------------------------------------+
//| Namespace for helper functions                                   |
//+------------------------------------------------------------------+
class SPluginHelpers
  {
  public:
    //+------------------------------------------------------------------+
    //|                                                                  |
    //+------------------------------------------------------------------+
    template<class T>
    inline static std::string FormatWithCommas(T value)
      {
      std::stringstream ss;
      ss.imbue(std::locale(""));
      ss.precision(2);
      ss<<std::fixed<<value;
      return ss.str();
      }
    //+------------------------------------------------------------------+
    //|                                                                  |
    //+------------------------------------------------------------------+
    template<class K>
    inline static bool MapContains(const std::map<std::string,K> &mMap,const std::string &mKey)
      {
      return (mMap.find(mKey)!=mMap.end());
      }
    //+------------------------------------------------------------------+
    //|                                                                  |
    //+------------------------------------------------------------------+
    inline static std::string ReplaceStr(std::string &s,const std::string &toReplace,const std::string &replaceWith)
      {
      if(s.find(toReplace)==std::string::npos)
        return s;

      return(s.replace(s.find(toReplace),toReplace.length(),replaceWith));
      }
    //+---------------------------------------------------------------+
    //| split method                                                  |
    //+---------------------------------------------------------------+
    inline static void split(const std::string &str,const std::string &delim,std::vector<std::string> &output)
      {
      unsigned int offset=0;
      unsigned int delimIndex=0;

      delimIndex=str.find(delim,offset);

      while(delimIndex!=std::string::npos)
        {
        output.push_back(str.substr(offset,delimIndex-offset));
        offset+=delimIndex-offset+delim.length();
        delimIndex=str.find(delim,offset);
        }
      output.push_back(str.substr(offset));
      }
    //+---------------------------------------------------------------+
    //| split method                                                  |
    //+---------------------------------------------------------------+
    inline static void split(const std::string &str,const std::string &delim,std::vector<std::string> &output,int MaxLength)
      {
      unsigned int offset=0;
      unsigned int delimIndex=0;

      delimIndex=str.find(delim,offset);
      while(delimIndex!=std::string::npos)
        {
        if(output.size()==MaxLength-1)
          break;

        output.push_back(str.substr(offset,delimIndex-offset));
        offset+=delimIndex-offset+delim.length();
        delimIndex=str.find(delim,offset);
        }

      output.push_back(str.substr(offset));
      }
    //+---------------------------------------------------------------+
    //| rounding method                                               |
    //+---------------------------------------------------------------+
    static inline double roundTo(double x,double prec)
      {
      return round(x*pow(10,prec))/pow(10,prec);
      }
  };
//+------------------------------------------------------------------+
