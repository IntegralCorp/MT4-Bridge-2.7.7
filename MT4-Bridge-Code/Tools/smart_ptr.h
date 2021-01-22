//+------------------------------------------------------------------+
//|                                                      TeamWox SDK |
//|                   Copyright 2006-2019, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once

//+------------------------------------------------------------------+
//| Class for automatic deletion of allocated memory                 |
//+------------------------------------------------------------------+
template <typename T>
class CSmartPtr
  {
private:
   T                *m_pntr;  // pointer to the stored object

public:
   //+------------------------------------------------------------------+
   //| Constructors                                                     |
   //+------------------------------------------------------------------+
                     CSmartPtr()        : m_pntr(nullptr) {}
                     CSmartPtr(T *pntr) : m_pntr(pntr) {}
   //+------------------------------------------------------------------+
   //| Destructor                                                       |
   //+------------------------------------------------------------------+
   ~CSmartPtr()
     {
      if(m_pntr) { delete m_pntr; m_pntr=NULL; }
     }
   //+------------------------------------------------------------------+
   //| Operators                                                        |
   //+------------------------------------------------------------------+
   operator          T*()           { return(m_pntr); }
   T*                operator -> () { return(m_pntr); }
   const T*          operator -> () const { return(m_pntr); }
   T**               operator &  () { return(&m_pntr); }
   //+------------------------------------------------------------------+
   //|                                                                  |
   //+------------------------------------------------------------------+
   CSmartPtr& operator=(T* pntr)
     {
      if(pntr!=m_pntr)
        {
         if(m_pntr!=NULL)
            delete m_pntr;
         m_pntr=pntr;
        }
      //---
      return(*this);
     }
   //+------------------------------------------------------------------+
   //| Functions                                                        |
   //+------------------------------------------------------------------+
   T* Detach()
     {
      T* temp=m_pntr;
      m_pntr=NULL;
      return(temp);
     }
   //+------------------------------------------------------------------+
   //|                                                                  |
   //+------------------------------------------------------------------+
   T* Pointer()
     {
      return(m_pntr);
     }
  };

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
template <typename T>
class CSmartArray
  {
private:
   T                *m_pntr;  // pointer to the stored object

public:
   //+------------------------------------------------------------------+
   //| Constructors                                                     |
   //+------------------------------------------------------------------+
                     CSmartArray()        : m_pntr(NULL) {}
                     CSmartArray(T *pntr) : m_pntr(pntr) {}
   //+------------------------------------------------------------------+
   //| Destructor                                                       |
   //+------------------------------------------------------------------+
   ~CSmartArray()
     {
      if(m_pntr) { delete[] m_pntr; m_pntr=NULL; }
     }
   //+------------------------------------------------------------------+
   //| Operators                                                        |
   //+------------------------------------------------------------------+
   operator          T*()           { return(m_pntr); }
   T*                operator -> () { return(m_pntr); }
   T**               operator &  () { return(&m_pntr); }
   CSmartArray& operator=(T* pntr)
     {
      if(pntr!=m_pntr)
        {
         if(m_pntr!=NULL) delete[] m_pntr;
         m_pntr=pntr;
        }
      //---
      return(*this);
     }
   //+------------------------------------------------------------------+
   //| Functions                                                        |
   //+------------------------------------------------------------------+
   T* Detach()
     {
      T* temp=m_pntr;
      m_pntr=NULL;
      return(temp);
     }
   T* Pointer()
     {
      return(m_pntr);
     }
   const T* Pointer() const
     {
      return(m_pntr);
     }
  };
//+------------------------------------------------------------------+
//| A class that tracks and works with it.                           |
//+------------------------------------------------------------------+
template <typename T>
class CSmartElems
  {
private:
   T                *m_ptr;            // pointer to a stored object or array
   bool              m_array;          // array flag

public:
   //+------------------------------------------------------------------+
   //| Constructors                                                     |
   //+------------------------------------------------------------------+
   CSmartElems(size_t count) : m_ptr(NULL),m_array(count>1)
     {
      if(count>1) m_ptr=new(std::nothrow) T[count];
      else        m_ptr=new(std::nothrow) T;
     }
   CSmartElems(T *ptr,bool arr) : m_ptr(ptr),m_array(arr) {}
   //+------------------------------------------------------------------+
   //| Destructor                                                       |
   //+------------------------------------------------------------------+
   ~CSmartElems()
     {
      if(m_ptr)
        {
         if(m_array) delete[] m_ptr;
         else        delete m_ptr;
         //---
         m_ptr=NULL;
        }
     }
   //+------------------------------------------------------------------+
   //| Operators                                                        |
   //+------------------------------------------------------------------+
   operator          T*()           { return(m_ptr); }
   T*                operator -> () { return(m_ptr); }
   T**               operator &  () { return(&m_ptr); }
   CSmartElems& operator=(T* ptr)
     {
      if(ptr!=m_ptr)
        {
         if(m_ptr)
           {
            if(m_array) delete[] m_ptr;
            else        delete m_ptr;
           }
         //---
         m_ptr=ptr;
        }
      //---
      return(*this);
     }
   //+------------------------------------------------------------------+
   //| Functions                                                        |
   //+------------------------------------------------------------------+
   T* Detach()
     {
      T* temp=m_ptr;
      m_ptr=NULL;
      return(temp);
     }
   T* Pointer()
     {
      return(m_ptr);
     }
  };
//+------------------------------------------------------------------+
//| Class to clear memory after completion                           |
//+------------------------------------------------------------------+
template <typename T,size_t len>
class CSecureBuffer
  {
private:
   T                 m_buffer[len];    // buffer

public:
   //+------------------------------------------------------------------+
   //| Constructor                                                      |
   //+------------------------------------------------------------------+
                     CSecureBuffer() { Clear(); }
   //+------------------------------------------------------------------+
   //| Destructor                                                       |
   //+------------------------------------------------------------------+
                    ~CSecureBuffer() { Clear(); }
   //+------------------------------------------------------------------+
   //| Operators                                                        |
   //+------------------------------------------------------------------+
   template <typename TN>
   operator          TN() const                 { return((TN)m_buffer);   }
   T*                operator -> ()             { return(m_buffer);       }
   T*                operator &  ()             { return(m_buffer);       }
   T&                operator[] (size_t index) { return(m_buffer[index]); }
   //+------------------------------------------------------------------+
   //| Funstions                                                        |
   //+------------------------------------------------------------------+
   void              Clear() { SecureZeroMemory(m_buffer,sizeof(T)*len); }
   size_t            Len()   { return(len);                              }
   T*                Ptr()   { return(m_buffer);                         }
  };
//+------------------------------------------------------------------+
