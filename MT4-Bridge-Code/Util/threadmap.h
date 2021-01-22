#pragma once

#include <map>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

template <class K,class V,class Compare = std::less<K>,class Allocator = std::allocator<std::pair<const K,V> > >
class threadmap
  {
  private:
    typedef boost::shared_mutex Lock;
    typedef boost::shared_lock< Lock > ReadLock;
    typedef boost::unique_lock< Lock > WriteLock;

    Lock myLock;
    std::size_t opCount = 0;
    std::map<K,V,Compare,Allocator> _map;

  public:
    void set(K key,V value)
      {
      std::size_t st = 0;
      this->set(key,value,st);
      }

    void set(K key,V value,std::size_t &op)
      {
      WriteLock w_lock(myLock);
      this->opCount++;

      op = this->opCount;

      this->_map[key] = value;
      }

    V &get(K key)
      {
      ReadLock r_lock(myLock);

      return this->_map[key];
      }

    const V &operator[] (K key)
      {
      return this->get(key);
      }

    bool has(K key)
      {
      ReadLock r_lock(myLock);

      return (this->_map.find(key)!=this->_map.end());
      }

    bool erase(K key)
      {
      std::size_t st = 0;
      return this->erase(key,st);
      }

    bool erase(K key,std::size_t &op)
      {
      WriteLock w_lock(myLock);
      this->opCount++;

      op = this->opCount;

      return (this->_map.erase(key)>=1);
      }

    bool empty()
      {
      WriteLock w_lock(myLock);
      this->opCount++;

      return this->_map.empty();
      }

    std::map<K,V> map()
      {
      ReadLock r_lock(myLock);

      std::map<K,V> __map;
      __map.insert(this->_map.begin(),this->_map.end());

      return __map;
      }

    std::map<K,V> map(std::size_t &op)
      {
      ReadLock r_lock(myLock);

      std::map<K,V> __map;
      __map.insert(this->_map.begin(),this->_map.end());

      op = this->opCount;

      return __map;
      }

    std::size_t op()
      {
      return this->opCount;
      }
  };