#pragma once

#include <string>
#include <Windows.h>
#include <time.h>
#include <direct.h>

struct Position
  {
  int		BuyVolume = 0;
  double	BuySwaps = 0;
  double	BuyPnL = 0;

  int		SellVolume = 0;
  double	SellSwaps = 0;
  double	SellPnL = 0;
  };

class CReports
  {
  public:
    enum class Action
      {
      ORDER_OPEN = 1,
      ORDER_CLOSE = 2,
      ORDER_DELETE = 3,
      ORDER_CREATE = 4,
      ORDER_MODIFY = 5
      };

  public:
    CReports();
    ~CReports();
    //---
    bool Initialize(void);

    void AccountBalance(time_t rawtime,LPCTSTR apFormat,...);
    void ClosingPrice(time_t rawtime,LPCTSTR apFormat,...);
    void ExecutedTrade(time_t rawtime,LPCTSTR apFormat,...);
    void OpenPosition(time_t rawtime,LPCTSTR apFormat,...);
    void Transaction(time_t rawtime,LPCTSTR apFormat,...);

  private:
    StringTypePath m_directory;
    CMTSync        m_sync;
  };
 extern CReports ExtReports;
//+------------------------------------------------------------------+
