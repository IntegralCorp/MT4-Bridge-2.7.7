//+------------------------------------------------------------------+
//|                                                 MetaTrader 4 API |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Thread synchronization class                                     |
//+------------------------------------------------------------------+
class CMTSync
  {
private:
   CRITICAL_SECTION  m_cs;

public:
                     CMTSync(void)      { ZeroMemory(&m_cs,sizeof(m_cs)); InitializeCriticalSection(&m_cs); }
                    ~CMTSync(void)      { DeleteCriticalSection(&m_cs);   ZeroMemory(&m_cs,sizeof(m_cs));   }

   inline void       Lock(void)         { EnterCriticalSection(&m_cs);                   }
   inline void       Unlock(void)       { LeaveCriticalSection(&m_cs);                   }
   inline bool       TryLock(void)      { return(TryEnterCriticalSection(&m_cs)!=FALSE); }

private:
                     CMTSync(const CMTSync&)=delete;
   CMTSync&          operator=(const CMTSync&)=delete;
  };
//+------------------------------------------------------------------+
//| Smart binding around CMTSync, which itself causes unlock         |
//+------------------------------------------------------------------+
class CLocker
  {
   CMTSync           *m_sync;

public:
                     CLocker(CMTSync *sync)   { m_sync=sync;  if(sync) sync->Lock(); }
                     CLocker(CMTSync &sync)   { m_sync=&sync; sync.Lock(); }
                     CLocker()                { m_sync=NULL; }
                    ~CLocker(void)            { if(m_sync) { m_sync->Unlock(); m_sync=NULL; } }
   //---
   void operator=    (CMTSync *sync)          { if(m_sync) m_sync->Unlock(); m_sync=sync; if(sync) sync->Lock(); }
   void operator=    (CMTSync &sync)          { if(m_sync) m_sync->Unlock(); m_sync=&sync; sync.Lock(); }

private:
                     CLocker(const CLocker &) { m_sync=NULL; };
   void              operator=(const CLocker & arg)
     {
      if(&arg==this) return;
     }
  };
//+------------------------------------------------------------------+
//| Smart binding around CMTSync, which itself calls lock            |
//| opposite of CLocker                                              |
//| doesn’t automatically call Lock, Unlock                          |
//+------------------------------------------------------------------+
class CLockerEx
  {
public:
   //+------------------------------------------------------------------+
   //|                                                                  |
   //+------------------------------------------------------------------+
   CLockerEx(CMTSync &sync) : m_sync(sync), m_start(false), m_state(false)
     {
     }
   //+------------------------------------------------------------------+
   //|                                                                  |
   //+------------------------------------------------------------------+
   CLockerEx(CMTSync &sync, bool start) : m_sync(sync), m_start(start), m_state(start)
     {
     }
   //+------------------------------------------------------------------+
   //|                                                                  |
   //+------------------------------------------------------------------+
   ~CLockerEx()
     {
      if(m_state!=m_start)
        {
         if(m_start) m_sync.Lock();
         else        m_sync.Unlock();
        }
     }
   //+------------------------------------------------------------------+
   //|                                                                  |
   //+------------------------------------------------------------------+
   void Lock()
     {
      _ASSERT(m_state==false);
      m_sync.Lock();
      m_state=true;
     }
   //+------------------------------------------------------------------+
   //|                                                                  |
   //+------------------------------------------------------------------+
   void Unlock()
     {
      _ASSERT(m_state==true);
      m_sync.Unlock();
      m_state=false;
     }

private:
   CMTSync          &m_sync;
   bool              m_start;
   bool              m_state;

                     CLockerEx(const CLockerEx&);
   CLockerEx&        operator=(const CLockerEx&);
  };
//+------------------------------------------------------------------+
