//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once

#ifndef INCLUDE_STRINGTOKENIZER_H
#define INCLUDE_STRINGTOKENIZER_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
//+------------------------------------------------------------------+
//| String Tokenizer                                                 |
//+------------------------------------------------------------------+
class StringTokenizer
{
public:
   StringTokenizer(const std::string& _str, const std::string& _delim);
  ~StringTokenizer(){};

   int         countTokens();
   bool        hasMoreTokens();
   std::string nextToken();
   int         nextIntToken();
   double      nextFloatToken();
   std::string nextToken(const std::string& delim);
   std::string remainingString();
   std::string filterNextToken(const std::string& filterStr);

private:

   std::string  token_str;
   std::string  delim;
};
//+------------------------------------------------------------------+
#endif