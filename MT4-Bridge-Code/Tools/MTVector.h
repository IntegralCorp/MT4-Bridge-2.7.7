//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Cache vector template class                                      |
//+------------------------------------------------------------------+
template<class T>
class TCacheVector
  {
public:
   enum constants
     {
      VECTOR_POS_MAX=0xFFFFFF                   // maximum item position
     };

private:
   typedef TMTArray<T> TArray;                  // array type

private:
   const bool        m_size_fixed;              // fixed size flag
   TArray            m_items;                   // items array

public:
   explicit          TCacheVector(const bool size_fixed=false) : m_size_fixed(size_fixed) {}
                    ~TCacheVector(void) {}
   //--- clear
   void              Clear(void);
   //--- initialization
   bool              Initialize(const UINT size);
   //--- check for object constructed
   bool              Constructed(void) const          { return(m_items.Width()); }
   //--- total items
   UINT              Total(void) const                { return(m_items.Total()); }
   //--- item by position
   T*                Item(const UINT pos);
   const T*          Item(const UINT pos) const       { return(pos<m_items.Total() ? (const T*)m_items.At(pos) : nullptr); }
   //--- position by item
   int               Position(const T *item) const    { return(m_items.Position(item)); }
   //--- vector binary operation
   template<class Func>
   bool              Operation(const TCacheVector &r,const Func &func);
  };
//+------------------------------------------------------------------+
//| clear                                                            |
//+------------------------------------------------------------------+
template<class T>
void TCacheVector<T>::Clear(void)
  {
   m_items.Clear();
  }
//+------------------------------------------------------------------+
//| initialization                                                   |
//+------------------------------------------------------------------+
template<class T>
bool TCacheVector<T>::Initialize(const UINT size)
  {
//--- clear
   Clear();
//--- check empty size
   if(!size)
      return(true);
//--- reserve items memory
   if(!m_items.Reserve(size))
      return(false);
//--- check fixed flag size
   if(!m_size_fixed)
      return(true);
//--- resize array
   if(!m_items.Resize(size))
      return(false);
//--- zero items
   m_items.Zero();
//--- ok
   return(true);
  }
//+------------------------------------------------------------------+
//| item by position                                                 |
//+------------------------------------------------------------------+
template<class T>
T* TCacheVector<T>::Item(const UINT pos)
  {
//--- check existing item
   const UINT total=m_items.Total();
   if(pos<total)
      return((T*)m_items.At(pos));
//--- check fixed size flag
   if(m_size_fixed)
      return(nullptr);
//--- check realocation needed
   if(pos>=m_items.Max())
     {
      //--- check position
      if(pos>VECTOR_POS_MAX)
         return(nullptr);
      //--- double size reallocation
      if(!m_items.Reserve(max(pos+1,m_items.Max()*2)))
         return(nullptr);
     }
//--- resize array
   if(!m_items.Resize(pos+1))
      return(nullptr);
//--- new items pointer
   T *begin=(T*)m_items.At(total);
   if(!begin)
      return(nullptr);
//--- zero new items
   ZeroMemory(begin,(pos-total+1)*sizeof(T));
//--- return new item pointer
   return((T*)m_items.At(pos));
  }
//+------------------------------------------------------------------+
//| vector binary operation                                          |
//+------------------------------------------------------------------+
template<class T>
template<class Func>
bool TCacheVector<T>::Operation(const TCacheVector &r,const Func &func)
  {
//--- iterate all items of right vector
   for(UINT i=0,total=r.Total();i<total;i++)
      if(const T *r_item=r.Item(i))
        {
         //--- get own item
         T *item=Item(i);
         if(!item)
            return(false);
         //--- operation on items
         func(*item,*r_item);
        }
//--- ok
   return(true);
  }
//+------------------------------------------------------------------+