/***********************************************************************
 * Header:
 *    TEST DEQUE
 * Summary:
 *    Unit tests for deque
 * Author
 *    Br. Helfrich
 ************************************************************************/

#pragma once

#ifdef DEBUG

#include "deque.h"
#include "unitTest.h"

#include <vector>
#include <cassert>
#include <memory>
#include <iostream>

class TestDeque : public UnitTest
{
public:
   void run()
   {
      reset();

      // Construct
      test_construct_default();
      test_constructCopy_empty();
      test_constructCopy_standard();

      // Assign
      test_assign_emptyToEmpty();
      test_assign_standardToEmpty();
      test_assign_emptyToStandard();
      test_assign_smallToBig();
      test_assign_bigToSmall();

      // Iterator
      test_iterator_increment_standardMiddle();
      test_iterator_dereference_read();
      test_iterator_dereference_update();

      // Access
      test_front_standardRead();
      test_front_standardWrite();
      test_back_standardRead();
      test_back_standardWrite();

      // Insert
      test_pushback_empty();
      test_pushback_standard();
      test_pushback_moveEmpty();
      test_pushback_moveStandard();
      test_pushfront_empty();
      test_pushfront_standard();
      test_pushfront_moveEmpty();
      test_pushfront_moveStandard();

      // Remove
      test_clear_empty();
      test_clear_standard();
      test_popback_empty();
      test_popback_standard();
      test_popfront_empty();
      test_popfront_standard();

      // Status
      test_size_empty();
      test_size_three();
      test_empty_empty();
      test_empty_three();

      report("Deque");
   }

   /***************************************
    * CONSTRUCT
    ***************************************/

    // default constructor, no allocations
   void test_construct_default()
   {  // exercise
      custom::deque<int> d;
      // verify
      assertEmptyFixture(d);
   }  // teardown


    // copy constructor of an empty list
   void test_constructCopy_empty()
   {  // setup
      custom::deque<int> dSrc;
      // exercise
      custom::deque<int> dDes(dSrc);
      // verify
      assertEmptyFixture(dSrc);
      assertEmptyFixture(dDes);
   }  // teardown

   // copy constructor of a 3-element collection
   void test_constructCopy_standard()
   {  // setup
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+      
      custom::deque<int> dSrc;
      setupStandardFixture(dSrc);
      // exercise
      custom::deque<int> dDes(dSrc);
      // verify
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+      
      assertStandardFixture(dSrc);
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+      
      assertStandardFixture(dDes);
      // teardown
   }


   /***************************************
    * SIZE EMPTY
    ***************************************/

    // size of graph with one node
   void test_size_empty()
   {  // setup
      custom::deque<int> l;
      // exercise
      size_t size = l.size();
      // verify
      assertUnit(0 == size);
      assertEmptyFixture(l);
   }  // teardown

   // size of graph with four nodes
   void test_size_three()
   {  // setup
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+      
      custom::deque<int> l;
      setupStandardFixture(l);
      // exercise
      size_t size = l.size();
      // verify
      assertUnit(3 == size);
      assertStandardFixture(l);
      // teardown
   }

   // graph with one node
   void test_empty_empty()
   {  // setup
      custom::deque<int> l;
      // exercise
      bool empty = l.empty();
      // verify
      assertUnit(true == empty);
      assertEmptyFixture(l);
   }  // teardown

   // graph with four nodes
   void test_empty_three()
   {  // setup
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+      
      custom::deque<int> l;
      setupStandardFixture(l);
      // exercise
      bool empty = l.empty();
      // verify
      assertUnit(false == empty);
      assertStandardFixture(l);
      // teardown
   }


   /***************************************
    * ASSIGN
    ***************************************/

    // use the assignment operator when both are empty
   void test_assign_emptyToEmpty()
   {  // setup
      custom::deque<int> dSrc;
      custom::deque<int> dDes;
      // exercise
      dDes = dSrc;
      // verify
      assertEmptyFixture(dSrc);
      assertEmptyFixture(dDes);
   }  // teardown

   // From the standard to fixture to an empty list
   void test_assign_standardToEmpty()
   {  // setup
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+      
      custom::deque<int> dSrc;
      setupStandardFixture(dSrc);
      custom::deque<int> dDes;
      // exercise
      dDes = dSrc;
      // verify
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(dSrc);
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(dDes);
      // teardown
   }

   // From the empty list to the standard to fixture
   void test_assign_emptyToStandard()
   {  // setup
      custom::deque<int> dSrc;
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::deque<int> dDes;
      setupStandardFixture(dDes);
      // exercise
      dDes = dSrc;
      // verify
      assertEmptyFixture(dSrc);
      assertEmptyFixture(dDes);
   }  // teardown

   void test_assign_bigToSmall()
   {  // setup
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::deque<int> dSrc;
      setupStandardFixture(dSrc);
      //       +----+   +----+
      //       | 85 | - | 99 |
      //       +----+   +----+
      custom::deque<int> dDes;
      dDes.container = {85, 99};
      // exercise
      dDes = dSrc;
      // verify
      // dSrc   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(dSrc);
      // dDes   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(dDes);
      // teardown
   }

   // assign a small list of 3 onto a larger one of 4
   void test_assign_smallToBig()
   {  // setup
      // dSrc   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::deque<int> dSrc;
      setupStandardFixture(dSrc);
      // dDes   pHead                      pTail
      //       +----+   +----+   +----+   +----+
      //       | 61 | - | 73 | - | 85 | - | 99 |
      //       +----+   +----+   +----+   +----+
      custom::deque<int> dDes;
      dDes.container = {61, 73, 85, 99};
      // exercise
      dDes = dSrc;
      // verify
      // dSrc   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(dSrc);
      // dDes   pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(dDes);
      // teardown
   }

   /***************************************
    * CLEAR
    ***************************************/

    // clear an empty fixture
   void test_clear_empty()
   {  // setup
      custom::deque<int> d;
      // exercise
      d.clear();
      // verify
      assertEmptyFixture(d);
   }  // teardown

   void test_clear_standard()
   {  // setup
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::deque<int> d;
      setupStandardFixture(d);
      // exercise
      d.clear();
      // verify
      assertEmptyFixture(d);
   }  // teardown


   /***************************************
    * PUSH BACK
    ***************************************/

    // push back to an empty list
   void test_pushback_empty()
   {  // setup
      custom::deque<int> d;
      int s(99);
      // exercise
      d.push_back(s);
      // verify
      //       +----+
      //       | 99 |
      //       +----+
      assertUnit(d.container.size() == 1);
      if (d.container.size() == 1)
         assertUnit(d.container.front() == 99);
      // teardown
   }

   // push an element onto the back of the standard fixture
   void test_pushback_standard()
   {  // setup
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::deque<int> d;
      setupStandardFixture(d);
      int s(99);
      // exercise
      d.push_back(s);
      // verify
      //       +----+   +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 | - | 99 |
      //       +----+   +----+   +----+   +----+
      assertUnit(d.container.size() == 4);
      if (d.container.size() == 4)
      {
         auto it = d.container.begin();
         assertUnit(it != d.container.end());
         if (it != d.container.end())
         {
            assertUnit(*it == 11);
            ++it;
            assertUnit(it != d.container.end());
            if (it != d.container.end())
            {
               assertUnit(*it == 26);
               ++it;
               assertUnit(it != d.container.end());
               if (it != d.container.end())
               {
                  assertUnit(*it == 31);
                  ++it;
                  assertUnit(it != d.container.end());
                  if (it != d.container.end())
                  {
                     assertUnit(*it == 99);
                     ++it;
                     assertUnit(it == d.container.end());
                  }
               }
            }
         }
      }
      // teardown     
   }

   // push back to an empty list
   void test_pushback_moveEmpty()
   {  // setup
      custom::deque<int> d;
      int s(99);
      // exercise
      d.push_back(std::move(s));
      // verify
      //       +----+
      //       | 99 |
      //       +----+
      assertUnit(d.container.size() == 1);
      if (d.container.size() == 1)
         assertUnit(d.container.front() == 99);
      // teardown      
   }

   // push an element onto the back of the standard fixture
   void test_pushback_moveStandard()
   {  // setup
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::deque<int> d;
      setupStandardFixture(d);
      int s(99);
      // exercise
      d.push_back(std::move(s));
      // verify
      //       +----+   +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 | - | 99 |
      //       +----+   +----+   +----+   +----+
      assertUnit(d.container.size() == 4);
      if (d.container.size() == 4)
      {
         auto it = d.container.begin();
         assertUnit(it != d.container.end());
         if (it != d.container.end())
         {
            assertUnit(*it == 11);
            ++it;
            assertUnit(it != d.container.end());
            if (it != d.container.end())
            {
               assertUnit(*it == 26);
               ++it;
               assertUnit(it != d.container.end());
               if (it != d.container.end())
               {
                  assertUnit(*it == 31);
                  ++it;
                  assertUnit(it != d.container.end());
                  if (it != d.container.end())
                  {
                     assertUnit(*it == 99);
                     ++it;
                     assertUnit(it == d.container.end());
                  }
               }
            }
         }
      }
      // teardown  
   }

   /***************************************
    * PUSH FRONT
    ***************************************/

    // push an element onto the front of an empty list
   void test_pushfront_empty()
   {  // setup
      custom::deque<int> d;
      int s(99);
      // exercise
      d.push_front(s);
      // verify
      //       +----+
      //       | 99 |
      //       +----+
      assertUnit(d.container.size() == 1);
      if (d.container.size() == 1)
         assertUnit(d.container.front() == 99);
      // teardown
   }

   // push an element onto the front of the standard fixture
   void test_pushfront_standard()
   {  // setup
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::deque<int> d;
      setupStandardFixture(d);
      int s(99);
      // exercise
      d.push_front(s);
      // verify
      //       +----+   +----+   +----+   +----+
      //       | 99 | - | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+   +----+
      assertUnit(d.container.size() == 4);
      if (d.container.size() == 4)
      {
         auto it = d.container.begin();
         assertUnit(it != d.container.end());
         if (it != d.container.end())
         {
            assertUnit(*it == 99);
            ++it;
            assertUnit(it != d.container.end());
            if (it != d.container.end())
            {
               assertUnit(*it == 11);
               ++it;
               assertUnit(it != d.container.end());
               if (it != d.container.end())
               {
                  assertUnit(*it == 26);
                  ++it;
                  assertUnit(it != d.container.end());
                  if (it != d.container.end())
                  {
                     assertUnit(*it == 31);
                     ++it;
                     assertUnit(it == d.container.end());
                  }
               }
            }
         }
      }
      // teardown  
   }

   // push an element onto the front of an empty list
   void test_pushfront_moveEmpty()
   {  // setup
      custom::deque<int> d;
      int s(99);
      // exercise
      d.push_front(std::move(s));
      // verify
      //       +----+
      //       | 99 |
      //       +----+
      // verify
      //       +----+
      //       | 99 |
      //       +----+
      assertUnit(d.container.size() == 1);
      if (d.container.size() == 1)
         assertUnit(d.container.front() == 99);
      // teardown
   }

   // push an element onto the front of the standard fixture
   void test_pushfront_moveStandard()
   {  // setup
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::deque<int> d;
      setupStandardFixture(d);
      int s(99);
      // exercise
      d.push_front(std::move(s));
      // verify
      //       +----+   +----+   +----+   +----+
      //       | 99 | - | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+   +----+
      assertUnit(d.container.size() == 4);
      if (d.container.size() == 4)
      {
         auto it = d.container.begin();
         assertUnit(it != d.container.end());
         if (it != d.container.end())
         {
            assertUnit(*it == 99);
            ++it;
            assertUnit(it != d.container.end());
            if (it != d.container.end())
            {
               assertUnit(*it == 11);
               ++it;
               assertUnit(it != d.container.end());
               if (it != d.container.end())
               {
                  assertUnit(*it == 26);
                  ++it;
                  assertUnit(it != d.container.end());
                  if (it != d.container.end())
                  {
                     assertUnit(*it == 31);
                     ++it;
                     assertUnit(it == d.container.end());
                  }
               }
            }
         }
      }
      // teardown  
   }

   /***************************************
    * POP BACK
    ***************************************/

    // remove an element from the back of an empty list
   void test_popback_empty()
   {  // setup
      custom::deque<int> d;
      // exercise
      d.pop_back();
      // verify
      assertEmptyFixture(d);
   }  // teardown

   // remove an element from the back of the standard fixture
   void test_popback_standard()
   {  // setup
      //       +----+   +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 | - | 99 |
      //       +----+   +----+   +----+   +----+
      custom::deque<int> d;
      setupStandardFixture(d);
      d.container.push_back(99);
      // exercise
      d.pop_back();
      // verify
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(d);
      // teardown
   }

   /***************************************
    * POP FRONT
    ***************************************/

    // remove an element from the front of an empty list
   void test_popfront_empty()
   {  // setup
      custom::deque<int> d;
      // exercise
      d.pop_front();
      // verify
      assertEmptyFixture(d);
   }  // teardown

   // remove an element from the front of the standard fixture
   void test_popfront_standard()
   {  // setup
      //       +----+   +----+   +----+   +----+
      //       | 99 | - | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+   +----+
      custom::deque<int> d;
      setupStandardFixture(d);
      d.container.push_front(99);
      // exercise
      d.pop_front();
      // verify
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(d);
      // teardown
   }


   /***************************************
    * FRONT and BACK
    ***************************************/

   // read the element off the front of the standard list
   void test_front_standardRead()
   {  // setup
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::deque<int> d;
      setupStandardFixture(d);
      int s(99);
      // exercise
      s = d.front();
      // verify
      assertUnit(s == int(11));
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(d);
      // teardown
   }

   // write the element to the front of the standard list
   void test_front_standardWrite()
   {  // setup
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::deque<int> d;
      setupStandardFixture(d);
      int s(99);
      // exercise
      d.front() = s;
      // verify
      //       +----+   +----+   +----+
      //       | 99 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertUnit(d.container.front() == int(99));
      d.container.front() = int(11);
      assertStandardFixture(d);
      // teardown
   }

   // read the element off the back of the standard list
   void test_back_standardRead()
   {  // setup
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::deque<int> d;
      setupStandardFixture(d);
      int s(99);
      // exercise
      s = d.back();
      // verify
      assertUnit(s == int(31));
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      assertStandardFixture(d);
      // teardown
   }

   // write the element to the back of the standard list
   void test_back_standardWrite()
   {  // setup
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      custom::deque<int> d;
      setupStandardFixture(d);
      int s(99);
      // exercise
      d.back() = s;
      // verify
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 99 |
      //       +----+   +----+   +----+
      assertUnit(d.container.back() == int(99));
      d.container.back() = int(31);
      assertStandardFixture(d);
      // teardown
   }



   /***************************************
    * ITERATOR
    ***************************************/

   // test the iterator to increment from the middle of the standard fixture
   void test_iterator_increment_standardMiddle()
   {  // setup
      custom::deque<int>::iterator it;
      std::list<int>::iterator itList;
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //                  it
      custom::deque<int> d;
      setupStandardFixture(d);
      itList = d.container.begin();
      ++itList;
      it.it = itList;
      // exercise
      ++it;
      // verify
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //                           it
      assertUnit(it.it == ++itList);
      assertStandardFixture(d);
      // teardown
      
   }

   // the the iterator's dereference operator to access an item from the list
   void test_iterator_dereference_read()
   {  // setup
      custom::deque<int>::iterator it;
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //                  it
      custom::deque<int> d;
      setupStandardFixture(d);
      std::list<int>::iterator itList = d.container.begin();
      ++itList;
      it.it = itList;
      int s(99);
      // exercise
      s = *it;
      // verify
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //                  it
      assertUnit(s == int(26));
      assertUnit(it.it == itList);
      assertStandardFixture(d);
      // teardown
      
   }

   // the the iterator's dereference operator to update an item from the list
   void test_iterator_dereference_update()
   {  // setup
      custom::deque<int>::iterator it;
      //       +----+   +----+   +----+
      //       | 11 | - | 26 | - | 31 |
      //       +----+   +----+   +----+
      //                  it
      custom::deque<int> d;
      setupStandardFixture(d);
      std::list<int>::iterator itList = d.container.begin();
      ++itList;
      it.it = itList;
      int s(99);
      // exercise
      *it = s;
      // verify
      //        pHead             pTail
      //       +----+   +----+   +----+
      //       | 11 | - | 99 | - | 31 |
      //       +----+   +----+   +----+
      //                  it
      assertUnit(*itList == 99);
      assertUnit(it.it == itList);
      *itList = 26;
      assertStandardFixture(d);
      // teardown
      
   }

   /****************************************************************
    * Setup Standard Fixture
    *        pHead             pTail
    *       +----+   +----+   +----+
    *       | 11 | - | 26 | - | 31 |
    *       +----+   +----+   +----+
    ****************************************************************/
   void setupStandardFixture(custom::deque<int>& d)
   {
      d.container.clear();
      d.container.push_back(11);
      d.container.push_back(26);
      d.container.push_back(31);
   }

   /****************************************************************
    * Verify Empty Fixture
    ****************************************************************/
   void assertEmptyFixtureParameters(const custom::deque<int>& d, int line, const char* function)
   {
      // verify the member variables
      assertIndirect(d.container.size() == 0);
   }

   /****************************************************************
    * Verify Standard Fixture
    *        pHead             pTail
    *       +----+   +----+   +----+
    *       | 11 | - | 26 | - | 31 |
    *       +----+   +----+   +----+
    ****************************************************************/
   void assertStandardFixtureParameters(const custom::deque<int>& d, int line, const char* function)
   {
      // verify the member variables
      assertIndirect(d.container.size() == 3);

      // verify the elements
      auto it = d.container.begin();

      assertIndirect(it != d.container.end());
      if (it != d.container.end())
      {
         assertIndirect(*it == 11);
         ++it;
         assertIndirect(it != d.container.end());
         if (it != d.container.end())
         {
            assertIndirect(*it == 26);
            ++it;

            assertIndirect(it != d.container.end());
            if (it != d.container.end())
            {
               assertIndirect(*it == 31);
               ++it;

               assertIndirect(it == d.container.end());
            }
         }
      }
   }
};

#endif // DEBUG
