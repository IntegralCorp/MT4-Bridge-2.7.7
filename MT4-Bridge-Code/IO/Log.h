#pragma once

#include <thread>
#include <queue>
#include <map>
//#include <mutex>
#include <atomic>
#include <concurrent_queue.h>
//#include "..\Queue\Queue.h"

#include "..\Common\Logger.h"
#include "..\Common\Const.h"
#include "..\Common\PluginContext.h"
#include "..\include\MT4ServerAPI.h"
#include "..\API\MT4APITools.h"
//---
#include "..\Util\DebugUtil.h"
#include "..\Util\ThreadMon.h"
#include "..\Util\AutoFree.h"

#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>

using namespace concurrency;

typedef enum
  {
  ESEVERITY_TRACE,
  ESEVERITY_DEBUG,
  ESEVERITY_INFO,
  ESEVERITY_MT4,
  ESEVERITY_WARNING,
  ESEVERITY_ERROR,
  ESEVERITY_ALERT,
  ESEVERITY_FATAL,
  ESEVERITY_JOURNAL,
  ESEVERITY_RISK,
  ESEVERITY_CRITICAL,
  ESEVERITY_ORDER,
  ESEVERITY_EVENT
  } ESeverity;

class LogRecord
  {
  public:
    ESeverity m_Severity;
    std::string s_Message;
    std::string s_DateStr;
    std::string s_TimeStr;
    time_t m_Time;
    int milliseconds = 0;

    LogRecord() { }

    LogRecord(ESeverity aSeverity,const char *apFilename,const char *apFuncName,int aLine,std::string &logMSG)
      {
      m_Severity = aSeverity;

      time_t rawtime = time(NULL);
      struct tm *timeinfo;

      timeinfo = gmtime(&rawtime);

      char m_DateStr[10];
      strftime(m_DateStr,sizeof(m_DateStr),"%m/%d/%y",timeinfo);
      s_DateStr = std::string(m_DateStr);

      char m_TimeStr[10];
      if(m_Severity==ESEVERITY_ORDER)
        strftime(m_TimeStr,sizeof(m_TimeStr),"%H%M%S",timeinfo);
      else
        strftime(m_TimeStr,sizeof(m_TimeStr),"%H:%M:%S",timeinfo);
      s_TimeStr = std::string(m_TimeStr);

      const auto currentTime = std::chrono::system_clock::now();
      time_t time = std::chrono::system_clock::to_time_t(currentTime);
      auto currentTimeRounded = std::chrono::system_clock::from_time_t(time);

      if(currentTimeRounded>currentTime)
        {
        --time;
        currentTimeRounded -= std::chrono::seconds(1);
        }

      milliseconds = std::chrono::duration_cast<std::chrono::duration<int,std::milli>>(currentTime-currentTimeRounded).count();

      char tmp_Message[1024] ={0};
      if(m_Severity==ESEVERITY_MT4||m_Severity==ESEVERITY_ORDER||m_Severity==ESEVERITY_EVENT)
        sprintf_s(tmp_Message,sizeof(tmp_Message),"");
      else
        sprintf_s(tmp_Message,sizeof(tmp_Message),"0x%08X\t%s(%d): ",std::this_thread::get_id().hash(),apFuncName,aLine); //apFilename

     //try		
     //{ 
     //vsprintf_s(tmp_Message + length, sizeof(tmp_Message) - length - 2, apFormat, apArgs);

      this->s_Message = tmp_Message+logMSG;
      std::replace(this->s_Message.begin(),this->s_Message.end(),static_cast<char>(0x0001),'|'); // replace all '[SOH]' to '|'
      this->s_Message.erase(std::remove(this->s_Message.begin(),this->s_Message.end(),'\r'),this->s_Message.end());
      this->s_Message.erase(std::remove(this->s_Message.begin(),this->s_Message.end(),'\n'),this->s_Message.end());
      //}
      //catch (...)
      //{ /* Maybe add a log */ }
      }
  };

class CLog
  {
  private:
    typedef std::map<ESeverity,std::string> SeverityMap;

  private:
    std::string m_directory;
    std::thread oThread;
    std::string m_file;
    std::atomic_bool stopThread,queueStarted;
    boost::lockfree::queue<LogRecord *,boost::lockfree::capacity<2048>> logQueue;
    //---
    static SeverityMap m_SeverityPrefix;

  public:
     CLog(void);
    ~CLog(void);

    bool Initialize(LPCSTR file);

    bool StartQueue(void);
    void Enqueue(ESeverity aSeverity,LPCTSTR apFilename,LPCTSTR aFunctionName,int aLine,const std::string apFormat,...);

  private:
    void DoWork(void);
    void ZipFiles(const std::string &openFile);
  };

extern CLog *pluginLog;
extern CLog *orderLog;


#define LOG_DEBUG(oLog, a, ...) if(oLog != NULL) oLog->Enqueue(ESEVERITY_DEBUG, __FILE__, __FUNCTION__, __LINE__, a, __VA_ARGS__)
#define LOG_INFO(oLog, a, ...) if(oLog != NULL) oLog->Enqueue(ESEVERITY_INFO, __FILE__, __FUNCTION__, __LINE__, a, __VA_ARGS__)
#define LOG_MT4(oLog, a, ...) if(oLog != NULL) oLog->Enqueue(ESEVERITY_MT4, __FILE__, __FUNCTION__, __LINE__, a, __VA_ARGS__)
#define LOG_WARNING(oLog, a, ...) if(oLog != NULL) oLog->Enqueue(ESEVERITY_WARNING, __FILE__, __FUNCTION__, __LINE__, a, __VA_ARGS__)
#define LOG_ERROR(oLog, a, ...) if(oLog != NULL) oLog->Enqueue(ESEVERITY_ERROR, __FILE__, __FUNCTION__, __LINE__, a, __VA_ARGS__)
#define LOG_ALERT(oLog, a, ...) if(oLog != NULL) oLog->Enqueue(ESEVERITY_ALERT, __FILE__, __FUNCTION__, __LINE__, a, __VA_ARGS__)
#define LOG_JOURNAL(oLog, a, ...) if(oLog != NULL) oLog->Enqueue(ESEVERITY_JOURNAL, __FILE__, __FUNCTION__, __LINE__, a, __VA_ARGS__)
#define LOG_RISK(oLog, a, ...) if(oLog != NULL) oLog->Enqueue(ESEVERITY_RISK, __FILE__, __FUNCTION__, __LINE__, a, __VA_ARGS__)
#define LOG_CRITICAL(oLog, a, ...) if(oLog != NULL) oLog->Enqueue(ESEVERITY_CRITICAL, __FILE__, __FUNCTION__, __LINE__, a, __VA_ARGS__)
#define LOG_ORDER(oLog, a, ...) if(oLog != NULL) oLog->Enqueue(ESEVERITY_ORDER, __FILE__, __FUNCTION__, __LINE__, a, __VA_ARGS__)
#define LOG_EVENT(oLog, a, ...) if(oLog != NULL) oLog->Enqueue(ESEVERITY_EVENT, __FILE__, __FUNCTION__, __LINE__, a, __VA_ARGS__)