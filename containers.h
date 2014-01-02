/*
 * containers.h  
 *
 * a generic implementation of hash table and linked list objects, with syntax 
 * similar to the STL containers.
 *
 * Copyright (c) 2004-2005, Felix Deschamps. All rights reserved. 
 *
 * This file is part of the OpScript Active Script Engine. 
 * This program is made available under the terms of the 
 * Common Public License v1.0 which accompanies this distribution, 
 * and is available at http://opensource.org/licenses/cpl.php
 *
 */
#ifndef __containers_h__
#define __containers_h__

#include <stdio.h>
#include <string.h>
#ifndef alloca
#include <malloc.h>
#endif

template <class T>
struct tlist
{
protected:
  struct tnode
  {
    tnode *next;
    tnode *last;
    T data;
  };

  tnode *_m_head;
  tnode *_m_tail;
  size_t    _m_count;
public:

  tlist()
  {
    _m_head = _m_tail = 0;
    _m_count = 0;
  }

  ~tlist()
  {
    clear();
  }

  struct iterator
  {
    private:
      tnode *_m_curr;
      tlist *_m_list;
    public:
      iterator(){ _m_curr = 0; _m_list = 0;}
      iterator operator ++(){ _m_curr = _m_curr->next;return *this;}
      iterator operator ++(int){ _m_curr = _m_curr->next;return *this;}
      T &operator*(){return _m_curr->data;}
      bool operator==(iterator t){return (t._m_curr==_m_curr);}
      bool operator!=(iterator t){return (t._m_curr!=_m_curr);}
    friend tlist;
  };

  iterator begin()
  {
    iterator it;
    it._m_curr = _m_head;
    it._m_list = this;
    return it;
  }

  iterator end()
  {
    iterator it;
    it._m_curr = 0;
    it._m_list = this;
    return it;
  }

  void append(T d)
  {
    tnode *pnode = new tnode;
    pnode->next = 0;
    pnode->data = d;
    pnode->last = _m_tail;

    if(_m_tail)
      _m_tail->next = pnode;
   
    _m_tail=pnode;
    
    if(!_m_head)
      _m_head = _m_tail;
    _m_count++;
  }

  void prepend(T d)
  {
     tnode *pnode =new tnode;
     pnode->last = 0;
     pnode->data = d;
     pnode->next = _m_head;

     if(_m_head)
       _m_head->last = pnode;
     
     _m_head = pnode;
     
     if(!_m_tail)
       _m_tail=_m_head;
     _m_count++;
  }

  void remove(iterator &t)
  {
    tnode *ptmp=t._m_curr;

    if(t._m_list!=this)
      return;

    if(!t._m_curr)
      return;

    if(t._m_curr->next)
      t._m_curr->next->last = t._m_curr->last;
    if(t._m_curr->last)
      t._m_curr->last->next = t._m_curr->next;

    if(t._m_curr==t._m_list->_m_head)
      t._m_list->_m_head = t._m_curr->next;
    if(t._m_curr==t._m_list->_m_tail)
      t._m_list->_m_tail = t._m_curr->last;

    t._m_curr=ptmp->next;
    if(!t._m_curr)
      t._m_curr=ptmp->last;
    delete ptmp;
    t._m_list->_m_count--;
  }

  void clear()
  {
    _m_count = 0;
    while(_m_head)
    {
      _m_tail = _m_head;
      _m_head=_m_head->next;
      delete _m_tail;
    }
    _m_tail=0;
  }

  size_t size()
  {
    return _m_count;
  }
};

template <class T>
struct tstack : public tlist<T>
{
  void push(T t)
  {
    prepend(t);
  }

  void pop()
  {
    remove(begin());
  }

  T &operator*()
  {
    return _m_head->data;
  }
};

template <class T, int BUCKET_SIZE = 1001>
class thash
{
public:
  struct tnode 
  {
    tnode *next;
    T data;
    char *key;
  };

  tnode *_m_buckets[BUCKET_SIZE];
  size_t _m_count;

  inline unsigned get_key(const char *name)
  {
    const char *s;
    unsigned h;

    for (s = name, h = 0; *s; ++s)
      h = (h << 1) ^ *s;
    return h % BUCKET_SIZE;
  }

public:

  struct iterator
  {
    private:
      size_t _m_index;
      tnode *_m_curr;
      thash *_m_table;
    public:
      iterator(){ _m_curr = 0; _m_table = 0;_m_index=0;}
      iterator operator ++()
      {
        if(_m_curr&&(_m_curr=_m_curr->next))
          return *this;

        for(;(_m_index<BUCKET_SIZE)&&(!_m_curr);_m_index++)
          _m_curr=_m_table->_m_buckets[_m_index];
        return *this;
      }

      iterator operator ++(int)
      {
        return operator ++();
      }

      T &operator*(){return _m_curr->data;}
      bool operator==(iterator t)
      { 
        return (t._m_curr==_m_curr)&&(t._m_index==_m_index);
      }
      
      bool operator!=(iterator t)
      {
        return (t._m_curr!=_m_curr)||(t._m_index!=_m_index);
      }
    
      friend thash;
  };

  friend iterator;

  size_t size()
  {
    return _m_count;
  }

  thash()
  {
    for(int i=0;i<BUCKET_SIZE;i++)
      _m_buckets[i]=0;
    _m_count = 0;
  }

  ~thash()
  {
    clear();
  }

#ifdef _UNICODE_LOOKUP
  void insert(LPCOLESTR pname, T d)
  {
    USES_CONVERSION;
    insert(W2A(pname),d);
  }
#endif

  void insert(const char *name, T d)
  {
    unsigned key = get_key(name);

    tnode *ptmp = _m_buckets[key];
    tnode *new_node = new tnode;
    tnode *last;

    new_node->data = d;
    new_node->key  = strdup(name);

    _m_count++;

    if(!ptmp)
    {
      // insert as the first item...
      new_node->next = 0;
      //new_node->last = 0;
      _m_buckets[key] = new_node;
    }
    else
    {
      last = 0;
      // insert in alphabetical order...
      for(;;ptmp=ptmp->next)
      {
        switch(strcmp(name,ptmp->key))
        {
          case 0:
          case -1:
            // insert it right before this one...
            new_node->next = ptmp;

            if(!last)
              _m_buckets[key]=new_node;
            else
              last->next = new_node;
            return;
          break;
        }

        if(!ptmp->next)
        {
          // append it..
          ptmp->next = new_node;
          new_node->next = 0;
          return;
        }

        last = ptmp;
      }
    }
  }

  bool find(const char *n, tlist<T> &l)
  {
    unsigned key = get_key(n);
    bool b=false;

    tnode *ptmp = _m_buckets[key];

    for(;ptmp;ptmp=ptmp->next)
    {
      switch(strcmp(n,ptmp->key))
      {
        case 0:
          l.append(ptmp->data);
          b = true;
        break;
        case -1:
          return b;
        break;
      }
    }

    return b;
  }

#ifdef _UNICODE_LOOKUP
  bool find(LPCOLESTR n,tlist<T> &l)
  {
    USES_CONVERSION;
    const char *p = W2A(n);
    return find(p,l);
  }
#endif

  void clear()
  {
    int i;
    tnode *curr,*ptmp;

    for(i=0;i<BUCKET_SIZE;i++)
    {
      curr = _m_buckets[i];
      _m_buckets[i] = 0;

      while(curr)
      {
        ptmp = curr;
        curr = ptmp->next;
        free(ptmp->key);
        delete ptmp;
      }
    }

    _m_count = 0;
  }

  iterator begin()
  {
    iterator it;
    it._m_curr = 0;
    it._m_table = this;
    it++;
    return it;
  }

  iterator end()
  {
    iterator it;
    it._m_curr = 0;
    it._m_index = BUCKET_SIZE;
    it._m_table = this;
    return it;
  }
};

#endif//__containers_h__