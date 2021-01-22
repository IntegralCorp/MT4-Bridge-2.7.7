//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
#include "..\..\QuickFix\fix43\ExecutionReport.h"
#include "..\..\API\MT4APITools.h"
#include "..\..\IntegralAPI\Common\IntegralMsgExecution.h"
#include "..\..\Risk\Ack.h"
#include "..\..\Util\threadmap.h"
#include "..\..\IO\Log.h"
//+------------------------------------------------------------------+
//| Record for storing Request history                               |
//+------------------------------------------------------------------+
//struct ExecutionReportRecord
//  {
//  char                unique_id[256];
//  IntegralExecution   record;
//  };
//+------------------------------------------------------------------+
//| Memory based requests storage                                    |
//+------------------------------------------------------------------+
class CExecutionBase
  {
  private:
    typedef std::map<std::string,Ack> AckMap;

  private:
    AckMap                          fxi_orders;

    //TMTArray<ExecutionReportRecord *> m_executions;
    CMTSync                           m_sync;

  public:
                           CExecutionBase(void);
                          ~CExecutionBase(void);
    //--- clear/add/remove
    void                   Clear(void);
    //bool                   Add(LPCSTR unique_id,const IntegralExecution &report);
    //bool                   Remove(LPCSTR unique_id);   
    //bool                   Remove(LPCSTR unique_id,IntegralExecution &report);
    //---
    bool                   Add(const std::string &unique_id,const Ack &ack);
    bool                   Remove(const std::string &unique_id);
    bool                   Remove(const std::string &unique_id,Ack &ack);
    bool                   Get(LPCSTR unique_id,Ack &ack);

  private:
    //--- memory
    //ExecutionReportRecord *ExecutionAllocate(void);
    //void                   ExecutionFree(ExecutionReportRecord *record);
    //--- sort/search
    //static int             SortExecutions(const void *left,const void *right);
    //static int             SearchExecutions(const void *left,const void *right);
  };
//+------------------------------------------------------------------+
