/***********************************************************************
* Header:
*    Set
* Summary:
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
*        set                 : A class that represents a Set
*        set::iterator       : An iterator through Set
* Author
 *    Daniel Malasky, Calvin Bullock
************************************************************************/

#pragma once

#include <cassert>
#include <iostream>
#include "bst.h"
#include <memory>     // for std::allocator
#include <functional> // for std::less

class TestSet;        // forward declaration for unit tests

namespace custom
{

//   class TestSet;

/************************************************
 * SET
 * A class that represents a Set
 ***********************************************/
template <typename T>
class set
{
   friend class ::TestSet; // give unit tests access to the privates
public:
   
   // 
   // Construct
   //
   set() : bst()
   {
   }
   set(const set &  rhs) : bst(rhs.bst)
   {
   }
   set(set && rhs) : bst(std::move(rhs.bst))
   {
   }
   set(const std::initializer_list <T> & il) 
   {
   }
   template <class Iterator>
   set(Iterator first, Iterator last) 
   {
   }
  ~set() { this->bst.clear(); }

   //
   // Assign
   //

   set & operator = (const set & rhs)
   {
      this->bst = rhs.bst;
      return *this;
   }
   set & operator = (set && rhs)
   {
      this->bst = std::move(rhs.bst);
      return *this;
   }
   set & operator = (const std::initializer_list <T> & il)
   {
      this->bst = il;
      return *this;
   }
   void swap(set& rhs) noexcept
   {
      bst.swap(rhs.bst);
   }

   //
   // Iterator
   //

   class iterator;
   iterator begin() const noexcept 
   { 
      return bst.begin();
   }
   iterator end() const noexcept 
   { 
      return bst.end();
   }

   //
   // Access
   //
   iterator find(const T& t) 
   { 
      return bst.find(t);
   }

   //
   // Status
   //
   bool empty() const noexcept 
   { 
      return bst.empty();
   }
   size_t size() const noexcept 
   { 
      return bst.size();
   }

   //
   // Insert
   //
   std::pair<iterator, bool> insert(const T& t)
   {
      std::pair<iterator, bool> p(iterator(), true);
      return p;
   }
   std::pair<iterator, bool> insert(T&& t)
   {
      std::pair<iterator, bool> p(iterator(), true);
      return p;
   }
   void insert(const std::initializer_list <T>& il)
   {
   }
   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
   }


   //
   // Remove
   //
   void clear() noexcept 
   {
      bst.clear();
   }
   iterator erase(iterator &it)
   { 
      return iterator(); 
   }
   size_t erase(const T & t) 
   {
      return 99;
   }
   iterator erase(iterator &itBegin, iterator &itEnd)
   {
      return iterator();
   }

private:
   
   custom::BST <T> bst;
};


/**************************************************
 * SET ITERATOR
 * An iterator through Set
 *************************************************/
template <typename T>
class set <T> :: iterator
{
   friend class ::TestSet; // give unit tests access to the privates
   friend class custom::set<T>;
public:
   // constructors, destructors, and assignment operator
   iterator() 
   {
   }
   iterator(const typename custom::BST<T>::iterator& itRHS) 
   {
      // this->it = itRHS.pNode;
   }
   iterator(const iterator & rhs) 
   { 
      // this = rhs;
   }
   iterator & operator = (const iterator & rhs)
   {
      this->it = rhs.it;
      return *this;
   }

   // equals, not equals operator
   bool operator != (const iterator & rhs) const 
   { 
      return !(this->it == rhs.it);
   }
   bool operator == (const iterator & rhs) const 
   { 
      return this->it == rhs.it;
   }

   // dereference operator: by-reference so we can modify the Set
   const T & operator * () const 
   { 
      return *it;
   }

   // prefix increment
   iterator & operator ++ ()
   {
      return *this;
   }

   // postfix increment
   iterator operator++ (int postfix)
   {
      return *this;
   }
   
   // prefix decrement
   iterator & operator -- ()
   {
      return *this;
   }
   
   // postfix decrement
   iterator operator-- (int postfix)
   {
      return *this;
   }
   
private:

   typename custom::BST<T>::iterator it;
};



}; // namespace custom



