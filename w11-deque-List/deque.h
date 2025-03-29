/***********************************************************************
 * Header:
 *    DEQUE
 * Summary:
 *    Our custom implementation of a deque 
 *        ____     _______        __
 *      .' __ '.  |  _____|   _  / /
 *      | (__) |  | |____    (_)/ /
 *      .`____'.  '_.____''.   / / _
 *     | (____) | | \____) |  / / (_)
 *     `.______.'  \______.' /_/
 *
 *    This will contain the class definition of:
 *        deque                 : A class that represents a binary search tree
 *        deque::iterator       : An iterator through BST
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

// Debug stuff
#include <cassert>  // for assert()
#include <list>     // for std::list

class TestDeque;    // forward declaration for TestDeque unit test class

namespace custom
{

/******************************************************
 * DEQUE
 *****************************************************/
template <typename T>
class deque
{
   friend class ::TestDeque; // give unit tests access to the privates
public:

   // 
   // Construct
   //
   deque() 
   { 
   }
   deque(deque& rhs)
   {
      this->container = rhs.container;
   }
   ~deque()
   {
   }

   //
   // Assign
   //
   deque & operator = (deque& rhs)
   {
      this->container = rhs.container;
      return *this;
   }

   // 
   // Iterator
   //
   class iterator;
   iterator begin() 
   { 
      return iterator(); 
   }
   iterator end()   
   { 
      return iterator(); 
   }

   // 
   // Access
   //
   T & front()       
   { 
      return container.front();
   }
   const T & front() const 
   {
      return container.front();
   }
   T & back()
   {
      return container.back();
   }
   const T & back() const
   {
      return container.back();
   }


   //
   // Insert
   //
   void push_back(const T& t)
   {
      container.push_back(t);
   }
   void push_back(T && t)
   {
      container.push_back(std::move(t));
   }
   void push_front(const T& t)
   {
      container.push_front(t);
   }
   void push_front(T&& t)
   {
      container.push_front(std::move(t));
   }

   //
   // Remove
   //
   void pop_front()
   {
      if (!container.empty())
         container.pop_front();
   }
   void pop_back()
   {
      if(!container.empty())
         container.pop_back();
   }
   void clear()
   {
      container.clear();
   }

   //
   // Status
   //
   size_t size()  const { return container.size(); }
   bool   empty() const { return container.size() == 0; }
   
private:
   std::list <T> container;
};

/**************************************************
 * DEQUE ITERATOR
 * An iterator through deque.  You only need to
 * support the following:
 *   1. Constructors (default and copy)
 *   2. Not equals operator
 *   3. Increment (prefix and postfix)
 *   4. Dereference
 * This particular iterator is a bi-directional meaning
 * that ++ and -- both work.  Not all iterators are that way.
 *************************************************/
template <typename T>
class deque <T> ::iterator
{
   friend class ::TestDeque; // give unit tests access to the privates
public:
   // 
   // Construct
   //
   iterator() 
   {
   }
   iterator(typename const std::list<T>::iterator& itList) 
   {
      it = itList;
   }
   iterator(const iterator& rhs) 
   { 
      it = rhs.it;
   }

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
      it = rhs.it;
      return *this;
   }

   // 
   // Compare
   //
   bool operator != (const iterator& rhs) const { return !(it == rhs.it); }
   bool operator == (const iterator& rhs) const { return it == rhs.it; }

   // 
   // Access
   //
   T& operator * ()
   {
      return *it;
   }

   // 
   // Arithmetic
   //
   iterator& operator ++ ()
   {
      ++(this->it);
      return *this;
   }
   iterator operator ++ (int postfix)
   {
      iterator temp = *this;
      ++(this->it);
      return temp;
   }
   iterator& operator -- ()
   {
      --(this->it);
      return *this;
   }
   iterator operator -- (int postfix)
   {
      iterator temp = *this;
      --(this->it);
      return temp;
   }

private:
   typename std::list<T>::iterator it;
};




} // namespace custom
