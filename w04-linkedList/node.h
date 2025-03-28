/***********************************************************************
 * Header:
 *    NODE
 * Summary:
 *    One node in a linked list (and the functions to support them).
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        Node         : A class representing a Node
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    Calvin Bullock, Daniel Malasky
 ************************************************************************/

#pragma once

#include <cassert>     // for ASSERT
#include <iostream>    // for NULL

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <class T>
class Node
{
public:
   //
   // Construct
   //
   //
   // Construct
   //

   Node(             ) : pNext(nullptr), pPrev(nullptr), data(               ) { }
   Node(const T& data) : pNext(nullptr), pPrev(nullptr), data(data           ) { }
   Node(     T&& data) : pNext(nullptr), pPrev(nullptr), data(std::move(data)) { }

   //
   // Member variables
   //

   T data;                 // user data
   Node <T> * pNext;       // pointer to next node
   Node <T> * pPrev;       // pointer to previous node
};

/***********************************************
 * COPY
 * Copy the list from the pSource and return
 * the new list
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline Node <T> * copy(const Node <T> * pSource)
{
   // TRY 1
   
   if (pSource == nullptr)
      return nullptr;

   Node <T> *pDestination = new Node<T>(pSource->data);
   const Node <T>* pSrc = pSource->pNext;
   Node <T> * pDes = pDestination;

   // start at the second value
   for (; pSrc != nullptr; pSrc = pSrc->pNext)
   {
      pDes = insert(pDes, pSrc->data, true);
   }

   return pDestination;


   // TRY 2
   //Node <T> *pNew = new Node<T>(pSource->data);
   //Node <T> *pCurrent = new Node<T>(pSource->data);
   //while (pCurrent->pNext != nullptr)
   //{
   //   pNew->pNext = new Node<T>(pCurrent->data);
   //   pCurrent = pCurrent->pNext;
   //}

   //return pNew;


   
}

/***********************************************
 * Assign
 * Copy the values from pSource into pDestination
 * reusing the nodes already created in pDestination if possible.
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline void assign(Node <T> * & pDestination, const Node <T> * pSource)
{
   const Node<T> * pSrc = pSource;
   Node<T> * pDes = pDestination;
   Node<T> * pDesPrevious = nullptr;

   // Copy source values into data values
   while (pSrc != nullptr and pDes != nullptr)
   {
      pDes->data = pSrc->data;
      pDesPrevious = pDes; // Save the last position of pDes
      pDes = pDes->pNext;
      pSrc = pSrc->pNext;
   }

   // pDestination is empty
   if (pSrc != nullptr)
   {
      pDes = pDesPrevious;
      
      while (pSrc != nullptr)
      {
         pDes = insert(pDes, pSrc->data, true);
         // Set pHead
         if (pDestination == nullptr)
         {
            pDestination = pDes;
         }
         pSrc = pSrc->pNext;
      }
   }
   else if (pDes != nullptr)
   {
      bool setToNull = false;

      if (pDes->pPrev != nullptr)
      {
         pDes->pPrev->pNext = nullptr;
      }
      else
      {
         setToNull = true;
      }

      clear(pDes);

      if (setToNull)
      {
         pDestination = nullptr;
      }
   }

}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(Node <T>* &pLHS, Node <T>* &pRHS)
{
   //Node <T>* pHold = new Node<T>();
   std::swap(pLHS, pRHS);
}

/***********************************************
 * REMOVE
 * Remove the node pSource in the linked list
 *   INPUT  : the node to be removed
 *   OUTPUT : the pointer to the parent node
 *   COST   : O(1)
 **********************************************/
template <class T>
inline Node <T> * remove(const Node <T> * pRemove)
{

   Node <T>* pReturn;

   // if it doesn't exist
   if (pRemove == nullptr)
      return nullptr;

   // pPrev is not null
   if (pRemove->pPrev)
      pRemove->pPrev->pNext = pRemove->pNext;

   // pNext is not null
   if (pRemove->pNext)
      pRemove->pNext->pPrev = pRemove->pPrev;

   // return pointer to parent
   if (pRemove->pPrev)
      pReturn = pRemove->pPrev;
   else
      pReturn = pRemove->pNext;

   delete pRemove;
   return pReturn;
}


/**********************************************
 * INSERT
 * Insert a new node the the value in "t" into a linked
 * list immediately before the current position.
 *   INPUT   : t - the value to be used for the new node
 *             pCurrent - a pointer to the node before which
 *                we will be inserting the new node
 *             after - whether we will be inserting after
 *   OUTPUT  : return the newly inserted item
 *   COST    : O(1)
 **********************************************/
template <class T>
inline Node <T> * insert(Node <T> * pCurrent,
                  const T & t,
                  bool after = false)
{
   Node<T> * pNew = new Node<T>(t);

   // if there is a current add the new node before it
   if (pCurrent != nullptr && !after)
   {
      pNew->pNext = pCurrent;
      pNew->pPrev = pCurrent->pPrev;
      pCurrent->pPrev = pNew;

      // if reached the end of the list add nullptr
      if (pNew->pPrev)
      {
         pNew->pPrev->pNext = pNew;
      }
   }

   // if there is a current add the new node after it
   if (pCurrent != nullptr && after)
   {
      pNew->pNext = pCurrent->pNext;
      pNew->pPrev = pCurrent;
      pCurrent->pNext = pNew;

      // if reached the front of list add nullptr
      if (pNew->pNext)
      {
         pNew->pNext->pPrev = pNew;
      }
   }

   return pNew;
}

/******************************************************
 * SIZE
 * Find the size an unsorted linked list.
 *  INPUT   : a pointer to the head of the linked list
 *            the value to be found
 *  OUTPUT  : number of nodes
 *  COST    : O(n)
 ********************************************************/
template <class T>
inline size_t size(const Node <T> * pHead)
{
   int count = 0;
   for (auto p = pHead; p; p = p->pNext)
      count++;

   return count;
}

/***********************************************
 * DISPLAY
 * Display all the items in the linked list from here on back
 *    INPUT  : the output stream
 *             pointer to the linked list
 *    OUTPUT : the data from the linked list on the screen
 *    COST   : O(n)
 **********************************************/
template <class T>
inline std::ostream & operator << (std::ostream & out, const Node <T> * pHead)
{
   return out;
}

/*****************************************************
 * FREE DATA
 * Free all the data currently in the linked list
 *   INPUT   : pointer to the head of the linked list
 *   OUTPUT  : pHead set to NULL
 *   COST    : O(n)
 ****************************************************/
template <class T>
inline void clear(Node <T> * & pHead)
{
   while (pHead != nullptr)
   {
      Node <T>* pDelete = pHead;
      pHead = pHead->pNext;
      delete pDelete;
   }
}


