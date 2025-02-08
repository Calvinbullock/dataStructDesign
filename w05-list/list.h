/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Author
 *    Daniel Malasky, Calvin Bullock
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator

class TestList; // forward declaration for unit tests
class TestHash; // forward declaration for hash used later

namespace custom
{

/**************************************************
 * LIST
 * Just like std::list
 **************************************************/
template <typename T, typename A = std::allocator<T>>
class list
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   friend void swap(list& lhs, list& rhs);
public:
   
   //
   // Construct
   //
   
   list(const A& a = A())  
   {
      numElements = 0;
      pHead = pTail = nullptr;
   }
   list(list <T, A> & rhs, const A& a = A())
   {
      numElements = rhs.numElements;
      pHead = rhs.pHead;
      pTail = rhs.pTail;

      for (auto it = rhs.begin(); it != rhs.end(); ++it)
      {
         push_back(*it);
      }
   }
   list(list <T, A>&& rhs, const A& a = A());
   list(size_t num, const T & t, const A& a = A()); // NO TESTS
   list(size_t num, const A& a = A());
   list(const std::initializer_list<T>& il, const A& a = A()) 
   {
      for (auto it = il.begin(); it != il.end(); it++)
         push_back(*it);
   }
   template <class Iterator>
   list(Iterator first, Iterator last, const A& a = A())
   {
      for (auto it = first; it != last; it++)
         push_back(*it);
   }
   ~list()
   { 
      clear();
   }
   
   //
   // Assign
   //
   
   list <T, A> & operator = (list <T, A> &  rhs);
   list <T, A> & operator = (list <T, A> && rhs);
   list <T, A> & operator = (const std::initializer_list<T>& il);
   void swap(list <T, A>& rhs) 
   { 
      Node* tempHead = rhs.pHead;
      rhs.pHead = pHead;
      pHead = tempHead;

      Node* tempTail = rhs.pTail;
      rhs.pTail = pTail;
      pTail = tempTail;

      int tempElements = rhs.numElements;
      rhs.numElements = numElements;
      numElements = tempElements;
   }

   //
   // Iterator
   //
   
   class iterator;
   iterator begin()  { return iterator (pHead);   }
   iterator rbegin() { return iterator (pTail);   }
   iterator end()    { return iterator (nullptr); }
   
   //
   // Access
   //
   
   T & front();
   T & back();

   //
   // Insert
   //
   
   void push_front(const T &  data);
   void push_front(      T && data);
   void push_back (const T &  data);
   void push_back (      T && data);
   iterator insert(iterator it, const T &  data);
   iterator insert(iterator it,       T && data);

   //
   // Remove

   
   void pop_back();
   void pop_front();
   void clear();
   iterator erase(const iterator & it);

   //
   // Status
   //
   
   bool empty()  const { return size() == 0; }
   size_t size() const { return numElements;   }

private:
   // nested linked list class
   class Node;

   // member variables
   A    alloc;         // use alloacator for memory allocation
   size_t numElements; // though we could count, it is faster to keep a variable
   Node * pHead;       // pointer to the beginning of the list
   Node * pTail;       // pointer to the ending of the list
};

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <typename T, typename A>
class list <T, A> :: Node
{
public:
   //
   // Construct
   //

   Node()              : pNext(nullptr), pPrev(nullptr), data()                { }
   Node(const T& data) : pNext(nullptr), pPrev(nullptr), data(data)            { }
   Node(T&& data)      : pNext(nullptr), pPrev(nullptr), data(std::move(data)) { }

   //
   // Member Variables
   //

   T data;             // user data
   Node * pNext;       // pointer to next node
   Node * pPrev;       // pointer to previous node
};

/*************************************************
 * LIST ITERATOR
 * Iterate through a List, non-constant version
 ************************************************/
template <typename T, typename A>
class list <T, A> :: iterator
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   template <typename TT, typename AA>
   friend class custom::list;
   
public:
   // constructors, destructors, and assignment operator
   iterator()  
   {
      p = new list <T, A> ::Node;
   }
   iterator(Node * pRHS)
   {
      p = pRHS;
   }
   iterator(const iterator  & rhs) 
   {
      p = rhs.p;
   }
   iterator & operator = (const iterator & rhs)
   {
      this->p = rhs.p;
      return *this;
   }
   
   // equals, not equals operator
   bool operator == (const iterator & rhs) const { return this->p == rhs.p; }
   bool operator != (const iterator & rhs) const { return !(this->p == rhs.p); }

   // dereference operator, fetch a node
   T & operator * ()
   {
      return this->p->data;
   }

   // postfix increment
   iterator operator ++ (int postfix)
   {
      iterator temp = *this;
      if (this->p->pNext)
         this->p = p->pNext;

      return temp;
   }

   // prefix increment
   iterator & operator ++ ()
   {
      this->p = p->pNext;
      return *this;
   }
   
   // postfix decrement
   iterator operator -- (int postfix)
   {
      iterator temp = *this;
      if (this->p->pPrev)
         this->p = p->pPrev;
      return temp;
   }

   // prefix decrement
   iterator & operator -- ()
   {
      this->p = this->p->pPrev;
      return *this;
   }

   // two friends who need to access p directly
   friend iterator list <T, A> :: insert(iterator it, const T &  data);
   friend iterator list <T, A> :: insert(iterator it,       T && data);
   friend iterator list <T, A> :: erase(const iterator & it);

private:

   typename list <T, A> :: Node * p;
};

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T, typename A>
list <T, A> ::list(size_t num, const T & t, const A& a) 
{
   if(num)
   {
      Node* pHead = new Node(t);
      Node* pPrevious = pHead;
      Node* pNew = pHead;

      pHead->pPrev = nullptr;

      for (size_t i = 1; i < num; i++) // - 1?
      {
         pNew = new Node(t);
         pNew->pPrev = pPrevious;
         pNew->pPrev->pNext = pNew;
         pPrevious = pNew;
      }
      pNew->pNext = nullptr;
      pTail = pNew;
   }
   numElements = num;
}

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T, typename A>
list <T, A> ::list(size_t num, const A& a) 
{
   pHead = nullptr;
   pTail = nullptr;
   numElements = 0;
   for (size_t i = 0; i < num; i++)
   {
      push_back(T());
   }
}

/*****************************************
 * LIST :: MOVE constructors
 * Steal the values from the RHS
 ****************************************/
template <typename T, typename A>
list <T, A> ::list(list <T, A>&& rhs, const A& a) :
   numElements(rhs.numElements), pHead(rhs.pHead), pTail(rhs.pTail), alloc(a)
{
   pHead = rhs.pHead;
   pTail = rhs.pTail;
   numElements = rhs.numElements;

   rhs.pHead = nullptr;
   rhs.pTail = nullptr;
   rhs.numElements = 0;
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS 
 *********************************************/
template <typename T, typename A>
list <T, A>& list <T, A> :: operator = (list <T, A> && rhs)
{
   clear();
   swap(rhs);
   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T, typename A>
list <T, A> & list <T, A> :: operator = (list <T, A> & rhs)
{

   
   // Fill lhs data with rhs data until one reaches the end
   auto itRHS = rhs.begin();
   auto itLHS = begin();


   while (itRHS != rhs.end() && itLHS != end())
   {
      *itLHS = *itRHS;
      ++itRHS;
      ++itLHS;
   }

   // if rhs is longer
   if (itRHS != rhs.end())
   {
      while (itRHS != rhs.end())
      {
         push_back(*itRHS);
         ++itRHS;
      }
   }
   // clear lhs if rhs is empty
   else if (rhs.empty())
   {
      clear();
      return *this;
   }
   // if lhs is longer
   else if (itLHS != end())
   {
      Node* p = itLHS.p;
      pTail = p->pPrev;
      Node* pNext = p->pNext;
      while (p != nullptr)
      {
         pNext = p->pNext;
         delete p;
         p = pNext;
         numElements--;
      }
      pTail->pNext = nullptr;

   }

   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T, typename A>
list <T, A>& list <T, A> :: operator = (const std::initializer_list<T>& rhs)
{

   // Fill lhs data with rhs data until one reaches the end
   auto itRHS = rhs.begin();
   auto itLHS = begin();


   while (itRHS != rhs.end() && itLHS != end())
   {
      *itLHS = *itRHS;
      ++itRHS;
      ++itLHS;
   }

   // if rhs is longer
   if (itRHS != rhs.end())
   {
      while (itRHS != rhs.end())
      {
         push_back(*itRHS);
         ++itRHS;
      }
   }
   // clear lhs if rhs is empty
   else if (rhs.size() == 0)
   {
      clear();
      return *this;
   }
   // if lhs is longer
   else if (itLHS != end())
   {
      Node* p = itLHS.p;
      pTail = p->pPrev;
      Node* pNext = p->pNext;
      while (p != nullptr)
      {
         pNext = p->pNext;
         delete p;
         p = pNext;
         numElements--;
      }
      pTail->pNext = nullptr;

   }

   return *this;
}



/**********************************************
 * LIST :: CLEAR
 * Remove all the items currently in the linked list
 *     INPUT  :
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T, typename A>
void list <T, A> :: clear()
{
   Node * p = pHead;
   while (p)
   {
      Node * pNext = p->pNext;
      delete p;
      p = pNext;
   }
   pHead = pTail = nullptr;
   numElements = 0;
}

/*********************************************
 * LIST :: PUSH BACK
 * add an item to the end of the list
 *    INPUT  : data to be added to the list
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> :: push_back(const T & data)
{
   Node* pNew = new Node(data);

   pNew->pPrev = pTail;

   // if list is not empty
   if (pTail)
      pTail->pNext = pNew;
   else
      pHead = pNew;

   pTail = pNew;

   numElements++;
}

template <typename T, typename A>
void list <T, A> ::push_back(T && data)
{
   Node* pNew = new Node(std::move(data));

   pNew->pPrev = pTail;

   // if list is not empty
   if (pTail)
      pTail->pNext = pNew;
   else
      pHead = pNew;

   pTail = pNew;

   numElements++;
}

/*********************************************
 * LIST :: PUSH FRONT
 * add an item to the head of the list
 *     INPUT  : data to be added to the list
 *     OUTPUT :
 *     COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> :: push_front(const T & data)
{
   Node* pNew = new Node(data);

   pNew->pNext = pHead;

   // if list is not empty
   if (pHead)
      pHead->pPrev = pNew;
   else
      pTail = pNew;

   pHead = pNew;

   numElements++;
}

template <typename T, typename A>
void list <T, A> ::push_front(T && data)
{
   Node* pNew = new Node(std::move(data));

   pNew->pNext = pHead;

   // if list is not empty
   if (pHead)
      pHead->pPrev = pNew;
   else
      pTail = pNew;

   pHead = pNew;

   numElements++;
}


/*********************************************
 * LIST :: POP BACK
 * remove an item from the end of the list
 *    INPUT  : 
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> ::pop_back()
{
   if (numElements == 0)
      return;

   if (numElements == 1)
   {
      clear();
      return;
   }

   Node* pBack = this->pTail;
   pTail = pBack->pPrev;
   pTail->pNext = nullptr;
   delete pBack;
   pBack = nullptr;
   numElements--;
}

/*********************************************
 * LIST :: POP FRONT
 * remove an item from the front of the list
 *    INPUT  :
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> ::pop_front()
{
   if (numElements == 0)
      return;

   if (numElements == 1)
   {
      clear();
      return;
   }

   Node* pFront = this->pHead;
   pHead = pFront->pNext;
   pHead->pPrev = nullptr;
   delete pFront;
   pFront = nullptr;
   numElements--;
}

/*********************************************
 * LIST :: FRONT
 * retrieves the first element in the list
 *     INPUT  :
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T, typename A>
T & list <T, A> :: front()
{
   if (pHead)
      return pHead->data;

   throw "ERROR: unable to access data from an empty list";
}

/*********************************************
 * LIST :: BACK
 * retrieves the last element in the list
 *     INPUT  :
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T, typename A>
T & list <T, A> :: back()
{
   if (pTail)
      return pTail->data;

   throw "ERROR: unable to access data from an empty list";
}


/******************************************
 * LIST :: REMOVE
 * remove an item from the middle of the list
 *     INPUT  : an iterator to the item being removed
 *     OUTPUT : iterator to the new location 
 *     COST   : O(1)
 ******************************************/
template <typename T, typename A>
typename list <T, A> :: iterator  list <T, A> :: erase(const list <T, A> :: iterator & it)
{
   // iterator to return
   auto itNext = end();

   // empty list
   if (it.p == nullptr)
      return itNext;

   // if it++ is not end of list
   if (it.p->pNext)
   {
      it.p->pNext->pPrev = it.p->pPrev; // unhook it.p
      itNext = it.p->pNext;
   }
   else
      pTail = pTail->pPrev;


   if (it.p->pPrev)
      it.p->pPrev->pNext = it.p->pNext; // unhook it.p
   else
      pHead = pHead->pNext;

   delete it.p;
   numElements--;

   return itNext;
}

/******************************************
 * LIST :: INSERT
 * add an item to the middle of the list
 *     INPUT  : data to be added to the list
 *              an iterator to the location where it is to be inserted
 *     OUTPUT : iterator to the new item
 *     COST   : O(1)
 ******************************************/
template <typename T, typename A>
typename list <T, A> :: iterator list <T, A> :: insert(list <T, A> :: iterator it,
   const T & data) 
{
   // Insert into empty list
   if (this->empty())
   {
      this->pHead = new Node(data);
      this->pTail = pHead;
      this->numElements = 1;
      return this->begin();
   }
   // Insert onto the end of list
   else if (it == this->end())
   {
      Node* pNew = new Node(data);
      pTail->pNext = pNew;
      pNew->pPrev = pTail;
      pTail = pNew;
      numElements++;
      return iterator(pNew);
   }
   // Insert anywhere else
   else if (it != this->end())
   {
      // Create the new node
      Node* pNew = new Node(data);

      // Hook up pNew
      pNew->pPrev = it.p->pPrev;
      pNew->pNext = it.p;

      // Make old list aware of pNew
      // if not beginning
      if (pNew->pPrev)
         pNew->pPrev->pNext = pNew;
      else
         pHead = pNew;

      // if not end
      if (pNew->pNext)
         pNew->pNext->pPrev = pNew;
      else
         pTail = pNew;

      numElements++;

      return iterator(pNew);
   }
   return end();
}


/******************************************
 * LIST :: INSERT
 * add several items into the middle of the list
 *     INPUT  : data to be added to the list
 *              an iterator to the location where it is to be inserted
 *     OUTPUT : iterator to the new item
 *     COST   : O(1)
 ******************************************/
template <typename T, typename A>
typename list <T, A> ::iterator list <T, A> ::insert(list <T, A> ::iterator it,
   T && data)
{
   // Insert into empty list
   if (this->empty())
   {
      this->pHead = new Node(std::move(data));
      this->pTail = pHead;
      this->numElements = 1;
      return this->begin();
   }
   // Insert onto the end of list
   else if (it == this->end())
   {
      Node* pNew = new Node(std::move(data));
      pTail->pNext = pNew;
      pNew->pPrev = pTail;
      pTail = pNew;
      numElements++;
      return iterator(pNew);
   }
   // Insert anywhere else
   else if (it != this->end())
   {
      // Create the new node
      Node* pNew = new Node(std::move(data));

      // Hook up pNew
      pNew->pPrev = it.p->pPrev;
      pNew->pNext = it.p;

      // Make old list aware of pNew
      // if not beginning
      if (pNew->pPrev)
         pNew->pPrev->pNext = pNew;
      else
         pHead = pNew;

      // if not end
      if (pNew->pNext)
         pNew->pNext->pPrev = pNew;
      else
         pTail = pNew;

      numElements++;

      return iterator(pNew);
   }
   return end();
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS
 *********************************************/
template <typename T, typename A>
void swap(list <T, A> & lhs, list <T, A> & rhs)
{
   lhs.numElements = 99;
}

}; // namespace custom


