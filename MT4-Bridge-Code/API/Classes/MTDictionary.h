//+------------------------------------------------------------------+
//|                                                 MetaTrader 4 API |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include "MTVector.h"
//+------------------------------------------------------------------+
//| Report dictionary class                                          |
//+------------------------------------------------------------------+
class CMTDictionary
  {
private:
   typedef TMTVector<LPCSTR> TVectorCStr;  // array type

private:
   TVectorCStr       m_strings;                 // strings array
   UINT              m_spectial_pos;            // special string position
   LPCSTR            m_spectial_string;         // special string

public:
                     CMTDictionary(void) : m_strings(false),m_spectial_pos(0),m_spectial_string(nullptr) {}
                    ~CMTDictionary(void) {}
   //--- clear
   void              Clear(void);
   //--- initialization
   bool              Initialize(const UINT size,const UINT spectial_pos=0,LPCSTR spectial_string=nullptr);
   //--- string by position
   LPCSTR            String(const UINT pos) const;
   //--- insert string by position
   bool              String(const UINT pos,LPCSTR string);
  };
//+------------------------------------------------------------------+
//| clear                                                            |
//+------------------------------------------------------------------+
inline void CMTDictionary::Clear(void)
  {
//--- delete strings
   for(UINT i=0,total=m_strings.Total();i<total;i++)
      if(LPCSTR *string=m_strings.Item(i))
         if(*string)
            delete [] *string;
//--- clear strings vector
   m_strings.Clear();
//--- clear special string
   m_spectial_pos=0;
   m_spectial_string=nullptr;
  }
//+------------------------------------------------------------------+
//| initialization                                                   |
//+------------------------------------------------------------------+
inline bool CMTDictionary::Initialize(const UINT size,const UINT spectial_pos,LPCSTR spectial_string)
  {
//--- clear
   Clear();
//--- reserve items memory
   if(!m_strings.Initialize(size))
      return(false);
//--- store special string
   m_spectial_pos=spectial_pos;
   m_spectial_string=spectial_string;
//--- ok
   return(true);
  }
//+------------------------------------------------------------------+
//| string by position                                               |
//+------------------------------------------------------------------+
inline LPCSTR CMTDictionary::String(const UINT pos) const
  {
//--- check special position
   if(m_spectial_string && m_spectial_pos==pos)
      return(m_spectial_string);
//--- get string by position
   const LPCSTR *string=m_strings.Item(pos);
   if(!string)
      return(nullptr);
//--- return string
   return(*string);
  }
//+------------------------------------------------------------------+
//| insert string by position                                        |
//+------------------------------------------------------------------+
inline bool CMTDictionary::String(const UINT pos,LPCSTR string)
  {
//--- check string
   if(!string)
      return(false);
//--- get string by position
   LPCSTR *dst=m_strings.Item(pos);
   if(!dst)
      return(false);
//--- check string exists
   if(*dst)
      return(true);
//--- get string length
   const UINT len=StringType::Len(string);
//--- allocate string buffer
   *dst=new char [len+1];
   if(!*dst)
      return(false);
//--- copy string to buffer   
   StringType::Copy((LPSTR)*dst,len+1,string,len);
//--- ok
   return(true);
  }
//+------------------------------------------------------------------+
