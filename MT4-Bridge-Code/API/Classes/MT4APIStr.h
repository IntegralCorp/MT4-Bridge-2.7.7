//+------------------------------------------------------------------+
//|                                                 MetaTrader 4 API |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include <new.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <winnls.h>
//+------------------------------------------------------------------+
//| String base class                                                |
//+------------------------------------------------------------------+
class StringType
  {
  protected:
    char *m_str;                 //string body
    UINT              m_str_len;             //string length
    const UINT        m_str_max;             //string buffer max size
    static LPCSTR     s_param_seps;          //separator of parameters string

  public:
    StringType(char *buf,const UINT size):m_str(buf),m_str_len(0),m_str_max(size) { if(m_str) m_str[0]=0; }
    virtual          ~StringType() { m_str=NULL; m_str_len=0; }
    //--- base functionality
    void              Clear() { m_str_len=0; if(m_str) m_str[0]=0; }
    bool              Empty() const { return(m_str_len<1||m_str==NULL||m_str[0]==0); }
    UINT              Len()   const { return(m_str_len); }
    static UINT       Len(LPCSTR str) { return(str?(UINT)strlen(str):0); }
    UINT              Max()   const { return(m_str_max); }
    const char *Str()   const { return(m_str); }
    char *Buffer() { return(m_str); }
    std::string       toString() { return std::string(m_str); }
    void              Refresh() { m_str_len=(m_str?(UINT)strnlen(m_str,m_str_max):0); }
    const char        operator[](UINT ind) const { return((ind<m_str_len&&m_str)?m_str[ind]:'\0'); }
    bool              operator!() const { return(m_str_len==0); }
    StringType &operator= (const StringType &src) { if(this!=&src) Assign(src); return(*this); }
    //--- assign
    void              Assign(const StringType &from);
    void              Assign(LPCSTR from);
    void              Assign(LPCSTR from,const UINT from_len);
    //--- operations
    int               Format(LPCSTR fmt,...);
    void              ToLower();
    void              ToUpper();
    void              Trim(UINT len);
    void              TrimSpaces();
    static void       TrimSpaces(LPSTR str);
    void              ReplaceChar(char findchar,char repchar);
    void              Replace(char *findstr,const char *repstr);
    void              Delete(UINT pos,UINT len);
    template <UINT dstsize>
    static void       Delete(char(&dst)[dstsize],UINT pos,UINT len) { Delete(dst,dstsize,pos,len); }
    static void       Delete(LPSTR dst,UINT dstsize,UINT pos,UINT len);
    static void       ToLower(LPSTR str,UINT size);
    static void       ToUpper(LPSTR str,UINT size);
    template <UINT size>
    static void       ToLower(char(&str)[size]) { ToLower(str,size); }
    template <UINT size>
    static void       ToUpper(char(&str)[size]) { ToUpper(str,size); }
    template <UINT strsize>
    static int        FormatStr(char(&str)[strsize],LPCSTR fmt,...);
    static int        FormatStr(LPSTR str,UINT size,LPCSTR fmt,...);
    template <UINT strsize>
    static void       Terminate(char(&str)[strsize]) { str[strsize-1]='\0'; }
    //--- append
    void              Append(LPCSTR appstring);
    void              Append(const StringType &appstring);
    void              Append(const std::string &appstring);
    void              Append(char appchar);
    template <UINT dstsize>
    static void       Append(char(&dst)[dstsize],LPCSTR src) { Append(dst,dstsize,src); }
    static void       Append(LPSTR dst,UINT dstsize,LPCSTR src);
    //--- insert
    void              Insert(UINT pos,LPCSTR insstring);
    void              Insert(UINT pos,const StringType &insstring);
    void              Insert(UINT pos,char inschar);
    //--- copy
    void              Copy(LPSTR string,UINT maxlen) const;
    template <UINT dstsize>
    static void       Copy(char(&dst)[dstsize],LPCSTR src) { Copy(dst,dstsize,src); }
    template <UINT dstsize>
    static void       Copy(char(&dst)[dstsize],LPCSTR src,UINT srccount) { Copy(dst,dstsize,src,srccount); }
    static void       Copy(LPSTR dst,UINT dstsize,LPCSTR src);
    static void       Copy(LPSTR dst,UINT dstsize,LPCSTR src,UINT srccount);
    template <UINT dstsize>
    static void       Copy(char(&dst)[dstsize],LPCWSTR src) { Copy(dst,dstsize,src); }
    static void       Copy(LPSTR dst,UINT dstsize,LPCWSTR src);
    static void       Copy(LPWSTR dst,UINT dstsize,LPCSTR src);
    static void       CopyCodePage(LPWSTR dst,UINT dstsize,LPCSTR src,UINT codepage);
    //--- compare
    int               Compare(LPCSTR str) const;
    int               Compare(LPCSTR str,UINT count) const;
    int               Compare(const StringType &str) const;
    static int        Compare(LPCSTR str1,LPCSTR str2);
    static int        Compare(LPCSTR str1,LPCSTR str2,UINT count);
    //--- compare no case
    int               CompareNoCase(LPCSTR str) const;
    int               CompareNoCase(LPCSTR str,UINT count) const;
    int               CompareNoCase(const StringType &str) const;
    static int        CompareNoCase(LPCSTR str1,LPCSTR str2);
    static int        CompareNoCase(LPCSTR str1,LPCSTR str2,UINT count);
    //--- check groups mask
    static bool       CheckGroupMask(LPCSTR groupsmask,LPCSTR group);
    //--- find symbol & substring
    int               Find(LPCSTR substring,UINT startpos) const;
    int               Find(LPCSTR substring) const;
    int               FindNoCase(LPCSTR substring,UINT startpos) const;
    int               FindNoCase(LPCSTR substring) const { return FindNoCase(substring,(UINT)0); }
    int               FindR(LPCSTR substring) const;
    int               FindChar(const char character) const;
    int               FindRChar(const char character) const;
    static int        Find(LPCSTR str,LPCSTR substr);
    static int        FindNoCase(LPCSTR str,LPCSTR substr);
    static int        FindR(LPCSTR str,LPCSTR substr);
    static int        FindChar(LPCSTR str,char character);
    static int        FindRChar(LPCSTR str,char character);
    //---
    void              SubStr(StringType &dst,UINT start,UINT length) const;
    void              SubStr(char *dst,size_t dst_length,size_t start,size_t length) const;
    //--- error description method
    LPCSTR            ErrorMsg(const DWORD error);
    //--- parameter search
    bool              ReadParamStr(LPCSTR string,LPCSTR param);
    static UINT       ReadParamStr(LPCSTR string,LPCSTR param,LPSTR buf,const UINT maxchars,LPCSTR seps=NULL);
    static bool       ReadParamChar(LPCSTR string,LPCSTR param,char *data);
    static bool       ReadParamShort(LPCSTR string,LPCSTR param,short *data);
    static bool       ReadParamInt(LPCSTR string,LPCSTR param,int *data);
    static bool       ReadParamInt64(LPCSTR string,LPCSTR param,INT64 *data);
    static bool       ReadParamDouble(LPCSTR string,LPCSTR param,double *data);

  protected:
    StringType();
    StringType(const StringType &from);
    StringType(LPSTR buf,const UINT size,LPCSTR from):m_str(buf),m_str_len(0),m_str_max(size)
      {
      if(m_str) m_str[0]=0;
      Assign(from);
      }
  private:
    static bool        CheckGroupTemplate(LPSTR expr,LPCSTR group);
    static int         FindParameter(LPCSTR string,LPCSTR param,LPCSTR seps);
  };
//+------------------------------------------------------------------+
//| Initialize static variables                                      |
//+------------------------------------------------------------------+
__declspec(selectany) LPCSTR StringType::s_param_seps="|\x1;";
//+------------------------------------------------------------------+
//| Stack string template                                            |
//+------------------------------------------------------------------+
template <UINT buflen> class TMTStrStatic: public StringType
  {
  char              m_buffer[buflen];
  public:
    TMTStrStatic(): StringType(m_buffer,buflen) {}
    TMTStrStatic(LPCSTR from): StringType(m_buffer,buflen,from) {}
    TMTStrStatic(const TMTStrStatic &from): StringType(m_buffer,buflen) { Assign(from); }
    TMTStrStatic(const StringType &from): StringType(m_buffer,buflen) { Assign(from); }
  };
//+------------------------------------------------------------------+
//| Stack strings types                                              |
//+------------------------------------------------------------------+
typedef TMTStrStatic<4>    StringTypeCry;
typedef TMTStrStatic<16>   StringType16;
typedef TMTStrStatic<32>   StringType32;
typedef TMTStrStatic<64>   StringType64;
typedef TMTStrStatic<128>  StringType128;
typedef TMTStrStatic<256>  StringType256;
typedef TMTStrStatic<260>  StringTypePath;
typedef TMTStrStatic<512>  StringType512;
typedef TMTStrStatic<1024> StringType1024;
typedef TMTStrStatic<2048> StringType2048;
typedef TMTStrStatic<4096> StringType4096;
//+------------------------------------------------------------------+
//| Assign from StringType                                               |
//+------------------------------------------------------------------+
inline void StringType::Assign(const StringType &from)
  {
  //--- check
  if(m_str)
    {
    UINT newlen=from.Len();
    //--- calc new length
    if(newlen>=m_str_max) newlen=m_str_max-1;
    memcpy(m_str,from.Str(),newlen*sizeof(char));
    m_str_len=newlen;
    //--- terminate
    m_str[m_str_len]=0;
    }
  //---
  }
//+------------------------------------------------------------------+
//| Assign from ptr                                                  |
//+------------------------------------------------------------------+
inline void StringType::Assign(LPCSTR from)
  {
  //--- check
  if(m_str)
    {
    //--- check empty string
    if(!from) m_str_len=0;
    else
      {
      UINT newlen=(UINT)strnlen(from,m_str_max);
      //--- calculate new length and copy
      if(newlen>=m_str_max) newlen=m_str_max-1;
      memcpy(m_str,from,newlen*sizeof(char));
      m_str_len=newlen;
      }
    //--- terminate string
    m_str[m_str_len]=0;
    }
  }
//+------------------------------------------------------------------+
//| Assign from ptr with len                                         |
//+------------------------------------------------------------------+
inline void StringType::Assign(LPCSTR from,const UINT from_len)
  {
  //--- check
  if(m_str)
    {
    //--- check empty string
    if(!from) m_str_len=0;
    else
      {
      UINT newlen=(UINT)strnlen(from,(from_len<m_str_max)?(from_len):(m_str_max));
      //--- calculate new length and copy
      if(newlen>=m_str_max) newlen=m_str_max-1;
      memcpy(m_str,from,newlen*sizeof(char));
      m_str_len=newlen;
      }
    //--- terminate string
    m_str[m_str_len]=0;
    }
  }
//+------------------------------------------------------------------+
//| Copy from ptr with len                                           |
//+------------------------------------------------------------------+
inline void StringType::Copy(LPSTR string,UINT maxlen) const
  {
  if(string&&maxlen>0&&m_str) strncpy_s(string,maxlen,m_str,_TRUNCATE);
  }
//+------------------------------------------------------------------+
//| Static copy from string with len                                 |
//+------------------------------------------------------------------+
inline void StringType::Copy(LPSTR dst,UINT dstsize,LPCSTR src)
  {
  if(dst&&src&&dstsize>0)
    {
    //--- copy string
    strncpy_s(dst,dstsize,src,_TRUNCATE);
    //--- remove trash symbols from CRT in Debug
#ifdef _DEBUG
    for(UINT pos=Len(dst)+1;pos<dstsize;pos++) dst[pos]=(wchar_t)0;
#endif
    }
  }
//+------------------------------------------------------------------+
//| Static copy from string with len                                 |
//+------------------------------------------------------------------+
inline void StringType::Copy(LPSTR dst,UINT dstsize,LPCSTR src,UINT srccount)
  {
  if(dst&&src&&dstsize>0&&srccount>0)
    {
    //--- copy string
    strncpy_s(dst,dstsize,src,dstsize>srccount?srccount:_TRUNCATE);
    //--- remove trash symbols from CRT in Debug
#ifdef _DEBUG
    for(UINT pos=Len(dst)+1;pos<dstsize;pos++) dst[pos]=(wchar_t)0;
#endif
    }
  }
//+------------------------------------------------------------------+
//| Copy from one-byte to unicode string                             |
//+------------------------------------------------------------------+
inline void StringType::Copy(LPSTR dst,UINT dstsize,LPCWSTR src)
  {
  if(dst&&src&&dstsize>0)
    ::WideCharToMultiByte(CP_ACP,0,src,-1,dst,int(dstsize),NULL,NULL);
  }
//+------------------------------------------------------------------+
//| Copy from unicode to one-byte string                             |
//+------------------------------------------------------------------+
inline void StringType::Copy(LPWSTR dst,UINT dstsize,LPCSTR src)
  {
  if(dst&&src)
    ::MultiByteToWideChar(CP_ACP,0,src,-1,dst,int(dstsize));
  }
//+------------------------------------------------------------------+
//| Copy from unicode to one-byte string with codepage               |
//+------------------------------------------------------------------+
inline void StringType::CopyCodePage(LPWSTR dst,UINT dstsize,LPCSTR src,UINT codepage)
  {
  if(dst&&src)
    ::MultiByteToWideChar(codepage,0,src,-1,dst,int(dstsize));
  }
//+------------------------------------------------------------------+
//| String format                                                    |
//+------------------------------------------------------------------+
inline int StringType::Format(LPCSTR fmt,...)
  {
  int len=-1;
  //---
  if(fmt&&m_str)
    {
    va_list marker;
    //--- format text
    va_start(marker,fmt);
    len=_vsnprintf_s(m_str,m_str_max,_TRUNCATE,fmt,marker);
    va_end(marker);
    //--- check result
    if(len>=0) m_str_len=UINT(len);
    else       m_str_len=(UINT)strnlen(m_str,m_str_max);
    }
  //---
  return(len);
  }
//+------------------------------------------------------------------+
//| Convert to lower case                                            |
//+------------------------------------------------------------------+
inline void StringType::ToLower()
  {
  if(m_str) _strlwr_s(m_str,m_str_max);
  }
//+------------------------------------------------------------------+
//| Convert to upper case                                            |
//+------------------------------------------------------------------+
inline void StringType::ToUpper()
  {
  if(m_str) _strupr_s(m_str,m_str_max);
  }
//+------------------------------------------------------------------+
//| Trim symbols from right                                          |
//+------------------------------------------------------------------+
inline void StringType::Trim(UINT len)
  {
  if(m_str&&len<m_str_len)
    {
    m_str_len=len;
    m_str[m_str_len]=0;
    }
  }
//+------------------------------------------------------------------+
//| Trim spaces                                                      |
//+------------------------------------------------------------------+
inline void StringType::TrimSpaces()
  {
  UINT pos;
  //--- check
  if(m_str_len==0||!m_str) return;
  //--- remove lead spaces
  for(pos=0;m_str[pos]==' ';) pos++;
  if(pos>0)
    {
    memmove(&m_str[0],&m_str[pos],(m_str_len-pos+1)*sizeof(m_str[0]));
    m_str_len-=pos;
    }
  //--- remove last spaces
  for(pos=m_str_len;pos>0;pos--)
    {
    if(m_str[pos-1]==' ') m_str[pos-1]='\0';
    else break;
    }
  //--- refresh length
  m_str_len=pos;
  }
//+------------------------------------------------------------------+
//| Trim spaces                                                      |
//+------------------------------------------------------------------+
inline void StringType::TrimSpaces(LPSTR str)
  {
  size_t len,pos;
  //--- check
  if(!str||(len=strlen(str))==0) return;
  //--- remove lead spaces
  for(pos=0;str[pos]==' ';) pos++;
  if(pos>0)
    {
    memmove(&str[0],&str[pos],(len-pos+1)*sizeof(str[0]));
    len-=pos;
    }
  //--- remove last spaces
  for(pos=len;pos>0;pos--)
    if(str[pos-1]==' ') str[pos-1]='\0';
    else break;
  }
//+------------------------------------------------------------------+
//| Replace symbol                                                   |
//+------------------------------------------------------------------+
inline void StringType::ReplaceChar(char findchar,char repchar)
  {
  //--- check
  if(m_str_len==0||!m_str) return;
  //--- replace
  for(UINT pos=0;pos<m_str_len;pos++)
    if(m_str[pos]==findchar) m_str[pos]=repchar;
  }
//+------------------------------------------------------------------+
//| Replace substring                                                |
//+------------------------------------------------------------------+
inline void StringType::Replace(char *findstr,const char *repstr)
  {
  //--- check
  if(findstr&&repstr&&m_str)
    {
    char *from=m_str;
    char *cp;
    size_t   newsize=0,findstr_len=strlen(findstr);
    char *newstr=NULL;
    //--- search
    while((cp=strstr(from,findstr))!=NULL)
      {
      //--- buffer allocated?
      if(!newstr)
        {
        newsize=m_str_max;
        newstr=new(std::nothrow) char[newsize];
        //--- fail
        if(newstr==NULL) break;
        }
      *cp=0;
      m_str_len=_snprintf_s(newstr,newsize,_TRUNCATE,"%s%s%s",m_str,repstr,cp+findstr_len);
      //--- copy
      if(m_str_len>=m_str_max) m_str_len=m_str_max-1;
      memcpy(m_str,newstr,m_str_len*sizeof(wchar_t));
      m_str[m_str_len]=0;
      //--- next position
      from=m_str+(cp-m_str)+strlen(repstr);
      }
    //--- free buffer
    if(newstr)
      {
      delete[] newstr;
      newstr=NULL;
      }
    }
  }
//+------------------------------------------------------------------+
//| Delete len symbols from pos position                             |
//+------------------------------------------------------------------+
inline void StringType::Delete(UINT pos,UINT len)
  {
  //--- check
  if(pos>=m_str_len||len==0||!m_str) return;
  //--- remove
  if(pos+len>m_str_len) len=m_str_len-pos;
  if(pos+len<m_str_len)
    for(UINT i=pos;i<m_str_len-len;i++)
      m_str[i]=m_str[i+len];
  m_str_len-=len;
  m_str[m_str_len]='\0';
  }
//+------------------------------------------------------------------+
//| Delete len symbols from pos position                             |
//+------------------------------------------------------------------+
inline void StringType::Delete(LPSTR dst,UINT dstsize,UINT pos,UINT len)
  {
  UINT dstlen;
  //--- check
  if(!dst||dstsize==0) return;
  dstlen=Len(dst);
  if(pos>=dstlen-1)      return;
  //--- remove
  if(pos+len>dstlen) len=dstlen-pos;
  if(pos+len<dstlen)
    for(UINT i=pos;i<dstlen-len;i++)
      dst[i]=dst[i+len];
  dst[dstlen-len]='\0';
  }
//+------------------------------------------------------------------+
//| Convert to lower case                                            |
//+------------------------------------------------------------------+
inline void StringType::ToLower(LPSTR str,UINT size)
  {
  if(str&&size>0) _strlwr_s(str,size);
  }
//+------------------------------------------------------------------+
//| Convert to upper case                                            |
//+------------------------------------------------------------------+
inline void StringType::ToUpper(LPSTR str,UINT size)
  {
  if(str&&size>0) _strupr_s(str,size);
  }
//+------------------------------------------------------------------+
//| Static string format                                             |
//+------------------------------------------------------------------+
template<UINT strsize>
inline int StringType::FormatStr(char(&str)[strsize],LPCSTR fmt,...)
  {
  int len=-1;
  //---
  if(str&&strsize>1&&fmt)
    {
    va_list marker;
    //--- format text
    va_start(marker,fmt);
    len=_vsnprintf_s(str,strsize,_TRUNCATE,fmt,marker);
    va_end(marker);
    //--- check
    if(len>0) str[len]='\0';
    }
  //---
  return(len);
  }
//+------------------------------------------------------------------+
//| Static string format                                             |
//+------------------------------------------------------------------+
inline int StringType::FormatStr(LPSTR str,UINT size,LPCSTR fmt,...)
  {
  int len=-1;
  //---
  if(str&&size>1&&fmt)
    {
    va_list marker;
    //--- format text
    va_start(marker,fmt);
    len=_vsnprintf_s(str,size,_TRUNCATE,fmt,marker);
    va_end(marker);
    //--- check
    if(len>0) str[len]='\0';
    }
  //---
  return(len);
  }
//+------------------------------------------------------------------+
//| Append to end                                                    |
//+------------------------------------------------------------------+
inline void StringType::Append(LPCSTR appstring)
  {
  UINT len;
  //--- check appstring
  if(m_str&&appstring&&(len=(UINT)strlen(appstring))>0&&(len+m_str_len)<m_str_max)
    {
    memcpy(&m_str[m_str_len],appstring,len*sizeof(m_str[0]));
    m_str_len+=len;
    m_str[m_str_len]=0;
    }
  }
//+------------------------------------------------------------------+
//| Append to end                                                    |
//+------------------------------------------------------------------+
inline void StringType::Append(const StringType &appstring)
  {
  //--- check appstring
  if(m_str&&appstring.Len()>0&&(appstring.Len()+m_str_len)<m_str_max)
    {
    memcpy(&m_str[m_str_len],appstring.Str(),appstring.Len()*sizeof(m_str[0]));
    m_str_len+=appstring.Len();
    m_str[m_str_len]=0;
    }
  }
//+------------------------------------------------------------------+
//| Append to end                                                    |
//+------------------------------------------------------------------+
inline void StringType::Append(const std::string &appstring)
  {
  //--- check appstring
  if(m_str&&appstring.size()>0&&(appstring.size()+m_str_len)<m_str_max)
    {
    memcpy(&m_str[m_str_len],appstring.c_str(),appstring.size()*sizeof(m_str[0]));
    m_str_len+=appstring.size();
    m_str[m_str_len]=0;
    }
  }
//+------------------------------------------------------------------+
//| Append symbol to end                                             |
//+------------------------------------------------------------------+
inline void StringType::Append(char appchar)
  {
  //--- check appchar
  if(m_str&&appchar!='\0'&&(m_str_len+1)<m_str_max)
    {
    m_str[m_str_len]=appchar;
    m_str[++m_str_len]='\0';
    }
  }
//+------------------------------------------------------------------+
//| Append to end                                                    |
//+------------------------------------------------------------------+
inline void StringType::Append(LPSTR dst,UINT dstsize,LPCSTR src)
  {
  if(dst&&src)
    {
    //--- find destination and and calculate free size
    char *dst_it=dst;
    size_t   available=dstsize;
    while(available>0&&*dst_it!=0)
      {
      ++dst_it;
      --available;
      }
    //--- not enough free size?
    if(available==0)
      return;
    //--- copy and check
    char *dst_end=dst_it;
    char const *src_it=src;
    while((*dst_it++=*src_it++)!=0&&--available>0)
      {
      }
    //--- not enough free size?
    if(available==0)
      *dst_end=0;
    }
  }
//+------------------------------------------------------------------+
//| Substring insert                                                 |
//+------------------------------------------------------------------+
inline void StringType::Insert(UINT pos,LPCSTR insstring)
  {
  UINT len;
  //--- check insstring
  if(m_str&&insstring&&(len=(UINT)strlen(insstring))>0&&(len+m_str_len)<m_str_max)
    {
    memmove(&m_str[pos+len],&m_str[pos],(m_str_len-pos)*sizeof(m_str[0]));
    memcpy(&m_str[pos],insstring,len*sizeof(m_str[0]));
    m_str_len+=len;
    m_str[m_str_len]='\0';
    }
  }
//+------------------------------------------------------------------+
//| Substring insert                                                 |
//+------------------------------------------------------------------+
inline void StringType::Insert(UINT pos,const StringType &insstring)
  {
  //--- check insstring
  if(m_str&&insstring.Len()>0&&(insstring.Len()+m_str_len)<m_str_max)
    {
    memmove(&m_str[pos+insstring.Len()],&m_str[pos],(m_str_len-pos)*sizeof(m_str[0]));
    memcpy(&m_str[pos],insstring.Str(),insstring.Len()*sizeof(m_str[0]));
    m_str_len+=insstring.Len();
    m_str[m_str_len]='\0';
    }
  }
//+------------------------------------------------------------------+
//| Symbol insert                                                    |
//+------------------------------------------------------------------+
inline void StringType::Insert(UINT pos,char inschar)
  {
  //--- check inschar
  if(m_str&&inschar!='\0'&&(m_str_len+1)<m_str_max)
    {
    memmove(&m_str[pos+1],&m_str[pos],(m_str_len-pos)*sizeof(m_str[0]));
    m_str[pos]=inschar;
    m_str[++m_str_len]='\0';
    }
  }
//+------------------------------------------------------------------+
//| Compare                                                          |
//+------------------------------------------------------------------+
inline int StringType::Compare(LPCSTR str) const
  {
  if(m_str&&str) return strcmp(m_str,str);
  return(m_str_len<1?0:1);
  }
//+------------------------------------------------------------------+
//| Compare                                                          |
//+------------------------------------------------------------------+
inline int StringType::Compare(LPCSTR str,UINT count) const
  {
  if(m_str&&str) return strncmp(m_str,str,count);
  return(m_str_len<1?0:1);
  }
//+------------------------------------------------------------------+
//| Compare                                                          |
//+------------------------------------------------------------------+
inline int StringType::Compare(const StringType &str) const
  {
  return(m_str?strcmp(m_str,str.Str()):-1);
  }
//+------------------------------------------------------------------+
//| Compare                                                          |
//+------------------------------------------------------------------+
inline int StringType::Compare(LPCSTR str1,LPCSTR str2)
  {
  return strcmp(str1,str2);
  }
//+------------------------------------------------------------------+
//| Compare                                                          |
//+------------------------------------------------------------------+
inline int StringType::Compare(LPCSTR str1,LPCSTR str2,UINT count)
  {
  return strncmp(str1,str2,count);
  }
//+------------------------------------------------------------------+
//| Compare no case                                                  |
//+------------------------------------------------------------------+
inline int StringType::CompareNoCase(LPCSTR str) const
  {
  if(m_str&&str) return _stricmp(m_str,str);
  return(m_str_len<1?0:1);
  }
//+------------------------------------------------------------------+
//| Compare no case                                                  |
//+------------------------------------------------------------------+
inline int StringType::CompareNoCase(LPCSTR str,UINT count) const
  {
  if(m_str&&str) return _strnicmp(m_str,str,count);
  return(m_str_len<1?0:1);
  }
//+------------------------------------------------------------------+
//| Compare no case                                                  |
//+------------------------------------------------------------------+
inline int StringType::CompareNoCase(const StringType &str) const
  {
  if(m_str&&str.m_str) return _stricmp(m_str,str.m_str);
  return(m_str_len<1?0:1);
  }
//+------------------------------------------------------------------+
//| Compare no case                                                  |
//+------------------------------------------------------------------+
inline int StringType::CompareNoCase(LPCSTR str1,LPCSTR str2)
  {
  return _stricmp(str1,str2);
  }
//+------------------------------------------------------------------+
//| Compare no case                                                  |
//+------------------------------------------------------------------+
inline int StringType::CompareNoCase(LPCSTR str1,LPCSTR str2,UINT count)
  {
  return _strnicmp(str1,str2,count);
  }
//+------------------------------------------------------------------+
//| Check group correspondence to groups mask list                   |
//+------------------------------------------------------------------+
inline bool StringType::CheckGroupMask(LPCSTR groupsmask,LPCSTR group)
  {
  const char *tok_start;
  char           mask[256]={0};
  bool           found=false;
  int            pos;
  //--- check
  if(!groupsmask||!group) return(false);
  //--- look through groupmask
  for(tok_start=groupsmask;*tok_start;tok_start++)
    {
    //--- skip spaces and commas
    if(*tok_start==' '||*tok_start==',') continue;
    //--- copy mask
    for(pos=0; *tok_start&&*tok_start!=','&&pos<255; tok_start++,pos++) mask[pos]=*tok_start;
    //--- skip spaces and commas
    while(pos>0&&(mask[pos-1]==' '||mask[pos-1]==',')) pos--;
    mask[pos]=0;
    //--- check
    if(mask[0]=='!'&&CheckGroupTemplate(mask,group)) return(false);
    else
      if(CheckGroupTemplate(mask,group)) found=true;
    //--- template over
    if(!*tok_start) break;
    }
  //--- return result
  return(found);
  }
//+------------------------------------------------------------------+
//| Find substring                                                   |
//+------------------------------------------------------------------+
inline int StringType::Find(LPCSTR substring,UINT startpos) const
  {
  if(substring&&m_str&&m_str_len>0)
    if(startpos<m_str_len)
      {
      char *position=strstr(m_str+startpos,substring);
      if(position) return int(position-m_str);
      }
  return(-1);
  }
//+------------------------------------------------------------------+
//| Find substring                                                   |
//+------------------------------------------------------------------+
inline int StringType::Find(LPCSTR substring) const
  {
  if(substring&&m_str&&m_str_len>0)
    {
    char *position=strstr(m_str,substring);
    if(position)
      {
      return int(position-m_str);
      }
    }
  return(-1);
  }
//+------------------------------------------------------------------+
//| Find substring no case                                           |
//+------------------------------------------------------------------+
inline int StringType::FindNoCase(LPCSTR substring,UINT startpos) const
  {
  UINT sublen=0;
  //---
  if(substring&&(sublen=(UINT)strlen(substring))>0&&m_str&&m_str_len>0)
    if(startpos<m_str_len)
      for(UINT pos=startpos;m_str_len-pos>=sublen;pos++)
        if(_strnicmp(m_str+pos,substring,sublen)==0)
          return int(pos);
  return(-1);
  }
//+------------------------------------------------------------------+
//| Find substring reverse                                           |
//+------------------------------------------------------------------+
inline int StringType::FindR(LPCSTR substring) const
  {
  int pos=0,end=0,beg=-1;
  //--- check
  if(substring&&m_str&&m_str_len>0&&Len(substring)>0)
    {
    //--- find from end
    while((pos=StringType::Find(&m_str[end],substring))>=0)
      {
      beg=end+pos;
      end+=pos+1;
      }
    }
  //--- result
  return(beg);
  }
//+------------------------------------------------------------------+
//| Find symbol                                                      |
//+------------------------------------------------------------------+
inline int StringType::FindChar(const char character) const
  {
  if(m_str&&m_str_len>0)
    {
    char *ptr=strchr(m_str,character);
    if(ptr) return int(ptr-m_str);
    }
  return(-1);
  }
//+------------------------------------------------------------------+
//| Find symbol reverse                                              |
//+------------------------------------------------------------------+
inline int StringType::FindRChar(const char character) const
  {
  if(m_str&&m_str_len>0)
    {
    char *ptr=strrchr(m_str,character);
    if(ptr) return int(ptr-m_str);
    }
  return(-1);
  }
//+------------------------------------------------------------------+
//| Find substring                                                   |
//+------------------------------------------------------------------+
inline int StringType::Find(LPCSTR str,LPCSTR substr)
  {
  if(str&&substr&&substr[0]!='\0')
    {
    LPCSTR ptr=strstr(str,substr);
    if(ptr) return int(ptr-str);
    }
  return(-1);
  }
//+------------------------------------------------------------------+
//| Find substring no case                                           |
//+------------------------------------------------------------------+
inline int StringType::FindNoCase(LPCSTR str,LPCSTR substr)
  {
  UINT len=0,sublen=0;
  //---
  if(str&&substr&&(len=(UINT)strlen(str))>0&&(sublen=(UINT)strlen(substr))>0)
    for(UINT pos=0;len-pos>=sublen;pos++)
      if(_strnicmp(str+pos,substr,sublen)==0)
        return int(pos);
  return(-1);
  }
//+------------------------------------------------------------------+
//| Find substring reverse                                           |
//+------------------------------------------------------------------+
inline int StringType::FindR(LPCSTR str,LPCSTR substr)
  {
  int pos=0,end=0,beg=-1;
  //--- check
  if(str&&substr&&Len(str)>0&&Len(substr)>0)
    {
    //--- find
    while((pos=StringType::Find(&str[end],substr))>=0)
      {
      beg=end+pos;
      end+=pos+1;
      }
    }
  //--- result
  return(beg);
  }
//+------------------------------------------------------------------+
//| Find char                                                        |
//+------------------------------------------------------------------+
inline int StringType::FindChar(LPCSTR str,char character)
  {
  if(str)
    {
    LPCSTR ptr=strchr(str,character);
    if(ptr) return int(ptr-str);
    }
  return(-1);
  }
//+------------------------------------------------------------------+
//| Find symbol reverse                                              |
//+------------------------------------------------------------------+
inline int StringType::FindRChar(LPCSTR str,char character)
  {
  if(str)
    {
    LPCSTR ptr=strrchr(str,character);
    if(ptr) return int(ptr-str);
    }
  return(-1);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
inline void StringType::SubStr(StringType &dst,UINT start,UINT length) const
  {
  dst.Clear();
  //--- ????????
  if(m_str==NULL||m_str_len==0||start>=m_str_len||length==0) return;
  //---
  dst.Assign(m_str+start,length);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
inline void StringType::SubStr(char *dst,size_t dst_length,size_t start,size_t length) const
  {
  //--- 
  if(dst==NULL||start+length>m_str_len) return;
  if(dst_length<=length)                  return;
  if(m_str==NULL)                         return;
  //---
  Copy(dst,dst_length,m_str+start,length);
  }
//+------------------------------------------------------------------+
//| Check group by expression                                        |
//+------------------------------------------------------------------+
inline bool StringType::CheckGroupTemplate(LPSTR expr,LPCSTR group)
  {
  char *mask=NULL,*mask_ast=NULL;
  bool     ast=false;
  //--- check
  if(!expr||!group) return(false);
  //--- skip multiple '!'
  for(mask=expr; *mask=='!'; mask++) {}
  //--- check for '*' at start
  for(; *mask&&*mask=='*'; mask++)
    {
    ast=true;
    mask_ast=mask;
    }
  //--- group loop
  for(; *group&&*mask; group++)
    {
    //--- check symbols
    if(*group!=*mask)
      {
      if(!ast) return(false);
      else     continue;
      }
    //--- check word
    for(; *mask&&*mask!='*'&&*group; mask++,group++)
      if(*group!=*mask)
        {
        if(!ast) return(false);
        else
          {
          mask=mask_ast;
          break;
          }
        }
    //--- check group
    if(!*mask&&!*group) return(true);
    if(!*mask)
      {
      if(!ast) return(false);
      else     mask=mask_ast;
      }
    if(!*group)
      {
      if(*mask&&*mask!='*') return(false);
      else
        {
        for(; *mask&&*mask=='*'; mask++) {}
        if(!*mask) return(true);
        return(false);
        }
      }
    else
      {
      for(; *mask&&*mask=='*'; mask++)
        mask_ast=mask;
      if(!*mask) return(true);
      ast=true;
      group--;
      }
    }
  //--- mask empty?
  if(*mask) return(false);
  return(true);
  }
//+------------------------------------------------------------------+
//| Search for a parameter in the string                             |
//+------------------------------------------------------------------+
inline int StringType::FindParameter(LPCSTR string,LPCSTR param,LPCSTR seps)
  {
  const char *str,*beg;
  //--- checking
  if(!string||!param) return(-1);
  //--- remember the string beginning
  beg=string;
  //--- search for entries
  while(true)
    {
    if((str=strstr(beg,param))==NULL) return(-1);
    //--- if the pewvious symbol is a separator or entry from the first symbol, we've found
    if(((str!=beg)&&strchr(seps,*(str-1)))||(str==beg)) break;
    //--- not the full name if a parameter, go further
    beg=str+strlen(param);
    }
  //--- we've found, position of the value
  return(int(str-string+strlen(param)));
  }
//+------------------------------------------------------------------+
//| Windows error description                                        |
//+------------------------------------------------------------------+
inline LPCSTR StringType::ErrorMsg(DWORD error)
  {
  StringTypePath text;
  //--- check
  if(!m_str) return(m_str);
  //--- get description
  ::FormatMessageA(FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_FROM_SYSTEM,
                   NULL,error,
                   MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),
                   text.Buffer(),text.Max(),
                   NULL);
  text.Refresh();
  //--- remove \r & \n in last symbol
  if(text.m_str_len&&(text.m_str[text.m_str_len-1]=='\r'||text.m_str[text.m_str_len-1]=='\n'))
    {
    text.m_str[text.m_str_len-1]='\0';
    text.m_str_len--;
    }
  //--- remove \r & \n in prelast symbol
  if(text.m_str_len&&(text.m_str[text.m_str_len-1]=='\r'||text.m_str[text.m_str_len-1]=='\n'))
    {
    text.m_str[text.m_str_len-1]='\0';
    text.m_str_len--;
    }
  //--- format both code and description
  Format("%s (%u)",text.Str(),error);
  return(m_str);
  }
//+------------------------------------------------------------------+
//| Search for a string parameter in the string                      |
//+------------------------------------------------------------------+
inline bool StringType::ReadParamStr(LPCSTR string,LPCSTR param)
  {
  char *res,*end;
  int      index;
  //--- checking
  if(!string||!param||!m_str)                            return(false);
  //--- secrh for the parameter
  if((index=FindParameter(string,param,s_param_seps))<0) return(false);
  //--- take the result
  string+=index;
  for(res=m_str,end=m_str+m_str_max-1; *string!=0&&(strchr(s_param_seps,*string)==NULL)&&res<end;)
    *res++=*string++;
  *res='\0';
  m_str_len=UINT(res-m_str);
  //--- exit
  return(true);
  }
//+------------------------------------------------------------------+
//| Search for a string parameter in the string                      |
//+------------------------------------------------------------------+
inline UINT StringType::ReadParamStr(LPCSTR string,LPCSTR param,LPSTR buf,const UINT maxchars,LPCSTR seps/*=NULL*/)
  {
  char *res,*end;
  int      index;
  //--- checking
  if(!string||!param||!buf)                  return(0);
  //--- if separators are not specified, use the standard set
  if(!seps) seps=s_param_seps;
  //--- secrh for the parameter
  if((index=FindParameter(string,param,seps))<0) return(0);
  //--- take the result
  string+=index;
  for(res=buf,end=buf+maxchars-1; *string!=0&&(strchr(seps,*string)==NULL)&&res<end;)
    *res++=*string++;
  *res='\0';
  //--- exit
  return(UINT(res-buf));
  }
//+------------------------------------------------------------------+
//| Read the byte parameter                                          |
//+------------------------------------------------------------------+
inline bool StringType::ReadParamChar(LPCSTR string,LPCSTR param,char *data)
  {
  int index;
  //--- checking
  if(!string||!param||!data)                         return(false);
  //--- search for the parameter
  if((index=FindParameter(string,param,s_param_seps))<0) return(false);
  //--- take the result
  *data=(char)atoi(&string[index]);
  return(true);
  }
//+------------------------------------------------------------------+
//| Read the short parameter                                         |
//+------------------------------------------------------------------+
inline bool StringType::ReadParamShort(LPCSTR string,LPCSTR param,short *data)
  {
  int index;
  //--- checking
  if(!string||!param||!data)                         return(false);
  //--- secrh for the parameter
  if((index=FindParameter(string,param,s_param_seps))<0) return(false);
  //--- take the result
  *data=atoi(&string[index]);
  return(true);
  }
//+------------------------------------------------------------------+
//| Search for an integer parameter in the string                    |
//+------------------------------------------------------------------+
inline bool StringType::ReadParamInt(LPCSTR string,LPCSTR param,int *data)
  {
  int index;
  //--- checks
  if(!string||!param||!data) return (false);
  //--- secrh for the parameter
  if((index=FindParameter(string,param,s_param_seps))<0) return(false);
  //--- consider
  *data=atoi(&string[index]);
  return (true);
  }
//+------------------------------------------------------------------+
//| Search for an integer parameter in the string                    |
//+------------------------------------------------------------------+
inline bool StringType::ReadParamInt64(LPCSTR string,LPCSTR param,INT64 *data)
  {
  int index;
  //--- checks
  if(!string||!param||!data) return (false);
  //--- secrh for the parameter
  if((index=FindParameter(string,param,s_param_seps))<0) return(false);
  //--- consider
  *data=_atoi64(&string[index]);
  return (true);
  }
//+------------------------------------------------------------------+
//| Search for an integer parameter in the string                    |
//+------------------------------------------------------------------+
inline bool StringType::ReadParamDouble(LPCSTR string,LPCSTR param,double *data)
  {
    int index;
  //--- checks
  if(!string||!param||!data) return (false);
  //--- secrh for the parameter
  if((index=FindParameter(string,param,s_param_seps))<0) return(false);
  //--- consider
  *data=atof(&string[index]);
  return (true);
  }
//+------------------------------------------------------------------+
//| Error description macro                                          |
//+------------------------------------------------------------------+
#define MT_ERROR_MSG() StringType64().ErrorMsg(GetLastError())
//+------------------------------------------------------------------+
