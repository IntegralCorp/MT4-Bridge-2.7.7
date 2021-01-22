#include "stdafx.h"
#include <stdio.h>
#include "Log.h"
//#include "..\IntegralGateway.h"
#include <direct.h>

#include "Zip.h"
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <iostream>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#include <mutex>


CLog *pluginLog = NULL;
CLog *orderLog = NULL;

CLog::CLog(void)
  {
  /* this->oDir = (dir+"\\Logs");
   _mkdir(this->oDir.c_str());

   oFilename = dir+std::string(file);

   queueStarted.store(false,std::memory_order_release);*/
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool CLog::Initialize(LPCSTR file_name)
  {
  StringTypePath path;
  //--- checks
  if(!file_name)
    return(false);
  //--- remember
  path.Format("%s\\Logs",ExtProgramPath);
  m_directory=path.Str();
  //--- check directory
  if(!CMTFile::DirectoryCreate(path))
    {
    LOG_ERROR(pluginLog,"%s directory create error",path.Str());
    return(false);
    }
  //--- set
  path.Append("\\");
  m_file=path.Str()+std::string(file_name);
  //---
  queueStarted.store(false,std::memory_order_release);
  //---
  return(true);
  }

CLog::SeverityMap CLog::m_SeverityPrefix =
  {
   { ESEVERITY_TRACE,   "TRACE"      },
   { ESEVERITY_DEBUG,   "DEBUG"      },
   { ESEVERITY_INFO,    "INFO"       },
   { ESEVERITY_MT4,     "MT4"        },
   { ESEVERITY_WARNING, "WARNING"    },
   { ESEVERITY_ERROR,   "ERROR"      },
   { ESEVERITY_ALERT,   "ALERT"      },
   { ESEVERITY_FATAL,   "FATAL"      },
   { ESEVERITY_JOURNAL, "JOURNAL"    },
   { ESEVERITY_RISK,    "!RISK!"     },
   { ESEVERITY_CRITICAL,"!CRITICAL!" },
   { ESEVERITY_ORDER,   "ORDER"      },
   { ESEVERITY_EVENT,   "EVENT"      }
  };

CLog::~CLog()
  {
  try
    {
    stopThread.store(true,std::memory_order_release);
    oThread.join();
    }
  catch(std::exception &e)
    {
    LOG_ERROR(pluginLog,"std::exception {%s}",e.what());
    }
  }

bool CLog::StartQueue(void)
  {
  if((!queueStarted.load(std::memory_order_acquire)))
    {
    queueStarted.store(true,std::memory_order_release);
    stopThread.store(false,std::memory_order_release);
    oThread = std::thread(&CLog::DoWork,this);

    return true;
    }

  return false;
  }

void CLog::Enqueue(ESeverity aSeverity,LPCTSTR apFilename,LPCTSTR aFunctionName,int aLine,const std::string apFormat,...)
  {
  if(!ExtPluginContext.EnableDebugLogs()&&aSeverity==ESEVERITY_DEBUG)
    return;

  int final_n,n = ((int)apFormat.size())*2; /* Reserve two times as much as the length of the apFormat */
  std::string str;
  std::unique_ptr<char[]> formatted;
  va_list ap;
  while(1)
    {
    formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
    strcpy(&formatted[0],apFormat.c_str());
    va_start(ap,apFormat);
    final_n = vsnprintf(&formatted[0],n,apFormat.c_str(),ap);
    va_end(ap);
    if(final_n<0||final_n>=n)
      n += abs(final_n-n+1);
    else
      break;
    }

  std::string logMSG(formatted.get());

  //va_list pArgs;
  //va_start(pArgs, apFormat);

  LogRecord *log = new LogRecord(aSeverity,apFilename,aFunctionName,aLine,logMSG);

  if(aSeverity==ESEVERITY_JOURNAL||aSeverity==ESEVERITY_ORDER)
    {
    //char buf[512] = { 0 };
    //vsprintf_s(buf, sizeof(buf) - 1, apFormat, pArgs);
    ExtLogger.Out(CmdOK,logMSG.c_str());
    }

  //va_end(pArgs);

  // print to output buffer (std::cout)
  if(IsDebuggerPresent()&&log->m_Severity!=ESEVERITY_MT4)
    printf("%s\t%s.%03d\tMT4.Debug\t%s\n",this->m_SeverityPrefix[log->m_Severity].c_str(),log->s_TimeStr.c_str(),log->milliseconds,log->s_Message.substr(11).c_str());

  logQueue.push(log);
  }

void CLog::DoWork(void)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;

  FILE *pFile = NULL;
  std::string openFile;
  bool isOrders = boost::algorithm::ends_with(m_file,"IDC.Orders");

  while(!stopThread.load(std::memory_order_acquire))
    {
    time_t rawtime = ExtServer->TradeTime();
    struct tm *timeinfo;
    char buffer[80];

    //time(&rawtime);
    timeinfo = gmtime(&rawtime);
    if(isOrders)
      strftime(buffer,80,".%Y-%m-%d.log",timeinfo);
    else
      strftime(buffer,80,".%Y-%m-%d-%H00.log",timeinfo);

    std::string timeString(buffer);
    std::string currentFile = (m_file+timeString);

    if(currentFile!=openFile||pFile==NULL)
      {
      if(pFile!=NULL)
        {
        fclose(pFile);
        pFile = NULL;
        }

      if(pFile==NULL)
        {
        openFile = currentFile;
        pFile = fopen(openFile.c_str(),"a");

        //TODO, move to the 15th minute or similar, to ensure all files have completed being written to.
        if(boost::algorithm::ends_with(m_file,"IDC.MT4"))
          {
          std::thread([=]()
                      {
                      CThreadMon threadMon;
                      boost::this_thread::sleep(boost::posix_time::seconds(30));
                      this->ZipFiles(openFile);
                      }).detach();
          }
        }
      }

    LogRecord *log;
    while(logQueue.pop(log)) // while (pFile != NULL && logQueue.pop(log)) //while (logQueue.try_pop() > 0 && pFile != NULL)
      {
      try
        {
        // print to log file
        if(log->m_Severity==ESEVERITY_ORDER) // || isOrders
          {
          fprintf(pFile,":ID:%s %s\r\n",log->s_TimeStr.c_str(),log->s_Message.c_str());
          //zFile << ":ID:" << log->s_TimeStr.c_str() << " " << log->s_Message.c_str() << "\r\n";
          }
        else
          {
          fprintf(pFile,"[%s %s.%03d]\t%s\t%s\n",log->s_DateStr.c_str(),log->s_TimeStr.c_str(),log->milliseconds,this->m_SeverityPrefix[log->m_Severity].c_str(),log->s_Message.c_str());
          //zFile << "[" << log->s_DateStr.c_str() << " " << log->s_TimeStr.c_str() << "." << setfill('0') << setw(3) << log->milliseconds << "]\t" << this->m_SeverityPrefix[log->m_Severity].c_str() << "\t" << log->s_Message.c_str() << "\n";
          }

        delete log;
        }
      catch(...)
        {
        delete log;
        break;
        }
      }

    if(pFile!=NULL)
      {
      fflush(pFile);
      }

    Sleep(20);
    }

  if(pFile!=NULL)
    {
    fclose(pFile);
    pFile = NULL;
    }
  }

void CLog::ZipFiles(const std::string &openFile)
  {
  DebugUtil::CUnhandledExceptionFilter filter;
  DebugUtil::CTransparentSEHTraslator  translator;


  try
    {
    LOG_INFO(pluginLog,"ZipFiles(const std::string &openFile)");

    static std::mutex mtx;           // mutex for critical section
    std::lock_guard<std::mutex> l(mtx);

    std::wstring wDir(m_directory.length(),L' ');
    std::copy(m_directory.begin(),m_directory.end(),wDir.begin());
    boost::filesystem::path p(wDir);

    if(is_directory(p))
      {
      std::vector<boost::filesystem::directory_entry> v; // To save the file names in a vector.
      std::copy(boost::filesystem::directory_iterator(p),boost::filesystem::directory_iterator(),back_inserter(v));
      std::map<std::string,std::vector<std::string>> mapFiles;

      for(std::vector<boost::filesystem::directory_entry>::const_iterator it = v.begin(); it!=v.end(); ++it)
        {
        const std::wstring filePath = (*it).path().wstring();
        const std::wstring dateString = filePath.substr(filePath.length()-19,15);
        const std::wstring fileName = wDir+L"\\"+dateString.substr(dateString.find_last_of('.')+1).substr(dateString.find_last_of('\\')+1)+L".zip";
        const std::wstring filePure = fileName.substr(fileName.find_last_of('\\')+1);

        const std::string fPath(filePath.begin(),filePath.end());
        std::string zipPath(fileName.begin(),fileName.end());

        if(boost::filesystem::is_directory(fPath))
          continue;

        if(filePath.find(L".gz")!=std::string::npos)
          continue;

        if((filePath.find(L".zip")!=std::string::npos&&filePure.length()==19)||(filePath.find(L"IDC.Orders.")!=std::string::npos&&filePure.length()==14))
          {
          std::wstring nowDate(openFile.begin(),openFile.end());
          nowDate = nowDate.substr(nowDate.length()-19,10);

          if(filePure.find(nowDate)!=std::string::npos)
            continue;

          zipPath = zipPath.substr(0,zipPath.find_last_of('\\')+11)+".zip";
          }
        else if(filePath.find(L".zip")!=std::string::npos||filePath.find(L"IDC.Orders.")!=std::string::npos)
          continue;

        bool hasMinute = false;
        if(filePath.find(L".dmp")!=std::string::npos||filePath.find(L".ini")!=std::string::npos)
          {
          hasMinute = true;
          zipPath = zipPath.substr(0,zipPath.length()-6)+"00.zip";
          }

        std::wstring nowTime(openFile.begin(),openFile.end());
        nowTime = nowTime.substr(nowTime.length()-19,15);

        if(nowTime==dateString)
          continue;
        else if(hasMinute&&nowTime.substr(0,nowTime.length()-2)==dateString.substr(0,dateString.length()-2))
          continue;

        if(zipPath.find(openFile.substr(openFile.length()-19,10))==std::string::npos)
          {
          std::string dayZip = zipPath.substr(0,zipPath.find_last_of('\\')+11)+".zip";
          if(std::find(mapFiles[dayZip].begin(),mapFiles[dayZip].end(),zipPath)==mapFiles[dayZip].end())
            if(dayZip!=zipPath)
              mapFiles[dayZip].push_back(zipPath);
          }

        mapFiles[zipPath].push_back(fPath);
        }

      for(auto &zip:mapFiles)
        { //https://www.codeproject.com/Articles/7530/Zip-Utils-clean-elegant-simple-C-Win

        if(boost::filesystem::exists(zip.first))
          {
          std::string newName = zip.first;
          newName.replace(newName.length()-4,4,"_.zip");

          try
            {
            boost::filesystem::rename(zip.first,newName);
            zip.second.push_back(newName);
            LOG_INFO(pluginLog,"Rename Zip: %s",newName.c_str());
            }
          catch(...)
            {
            LOG_ERROR(pluginLog,"Could not rename Zip: %s",newName.c_str());
            continue;
            }
          }

        // Now Zip
        LOG_INFO(pluginLog,"Create Zip: %s",zip.first.c_str());

        HZIP hz = CreateZip(zip.first.c_str(),NULL);
        if(hz==NULL)
          {
          LOG_ERROR(pluginLog,"Could not create archive: %s",zip.first.c_str());
          continue;
          }

        std::function<void()> cleanupAsync = [=]()
          {
          CloseZip(hz);
          };

        CAutoFree autoFree(cleanupAsync);

        for(auto &file:zip.second)
          {
          const std::string fPath(file);
          const std::string fName = fPath.substr(fPath.find_last_of("/\\")+1);

          ZRESULT oRes = ZipAdd(hz,fName.c_str(),fPath.c_str());
          LOG_INFO(pluginLog,"Add %s => %s(0x%08x)",fPath.c_str(),zip.first.c_str(),oRes);

          if(oRes==ZR_OK)
            { // TODO, handle cases where the zipFile already exists, or that oRes != OK
            std::wstring wFile(fPath.length(),L' ');
            std::copy(fPath.begin(),fPath.end(),wFile.begin());
            boost::filesystem::path pFile(wFile);

            if(boost::filesystem::exists(pFile))
              if(!boost::filesystem::remove(pFile))
                LOG_ERROR(pluginLog,"Could not delete: %s",pFile.c_str());
            }
          }
        }
      }
    }
  catch(const std::exception &ex)
    {
    LOG_ERROR(pluginLog,"Could not delete: %s",ex.what());
    }
  }