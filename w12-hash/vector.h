/***********************************************************************
 * Header:
 *    VECTOR
 * Summary:
 *    Our custom implementation of std::vector
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        vector                 : A class that represents a Vector
 *        vector::iterator       : An iterator through Vector
 * Author
 *    Calvin Bullock, Daniel Malasky
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
#include <new>      // std::bad_alloc
#include <memory>   // for std::allocator

#include <iostream> // TESTING

class TestVector; // forward declaration for unit tests
class TestStack;
class TestPQueue;
class TestHash;

namespace custom
{

   /*****************************************
    * VECTOR
    * Just like the std :: vector <T> class
    ****************************************/
   template <typename T, typename A = std::allocator<T>>
   class vector
   {
      friend class ::TestVector; // give unit tests access to the privates
      friend class ::TestStack;
      friend class ::TestPQueue;
      friend class ::TestHash;
   public:

      //
      // Construct
      //
      vector(const A& a = A());
      vector(size_t numElements, const A& a = A());
      vector(size_t numElements, const T& t, const A& a = A());
      vector(const std::initializer_list<T>& l, const A& a = A());
      vector(const vector& rhs);
      vector(vector&& rhs);
      ~vector();

      //
      // Assign
      //
      void swap(vector& rhs)
      {
         std::swap(numCapacity, rhs.numCapacity);
         std::swap(numElements, rhs.numElements);
         std::swap(data, rhs.data);
      }
      vector& operator = (const vector& rhs);
      vector& operator = (vector&& rhs);

      //
      // Iterator
      //
      class iterator;
      iterator begin()
      {
         return iterator(data);
      }
      iterator end()
      {
         return iterator(data + numElements);
      }

      //
      // Access
      //
      T& operator [] (size_t index);
      const T& operator [] (size_t index) const;
      T& front();
      const T& front() const;
      T& back();
      const T& back() const;

      //
      // Insert
      //
      void push_back(const T& t);
      void push_back(T&& t);
      void reserve(size_t newCapacity);
      void resize(size_t newElements);
      void resize(size_t newElements, const T& t);

      //
      // Remove
      //
      void clear()
      {
         for (size_t i = 0; i < numElements; ++i)
            alloc.destroy(&data[i]);
         numElements = 0;
      }
      void pop_back()
      {
         if (numElements > 0)
         {
            alloc.destroy(&data[numElements - 1]);
            numElements--;
         }

         return;
      }
      void shrink_to_fit();

      //
      // Status
      //
      size_t  size()          const { return numElements; }
      size_t  capacity()      const { return numCapacity; }
      bool empty()            const { return numElements == 0; }

   private:

      A    alloc;                // use allocator for memory allocation
      T* data;                 // user data, a dynamically-allocated array
      size_t  numCapacity;       // the capacity of the array
      size_t  numElements;       // the number of items currently used
   };

   /**************************************************
    * VECTOR ITERATOR
    * An iterator through vector.  You only need to
    * support the following:
    *   1. Constructors (default and copy)
    *   2. Not equals operator
    *   3. Increment (prefix and postfix)
    *   4. Dereference
    * This particular iterator is a bi-directional meaning
    * that ++ and -- both work.  Not all iterators are that way.
    *************************************************/
   template <typename T, typename A>
   class vector <T, A> ::iterator
   {
      friend class ::TestVector; // give unit tests access to the privates
      friend class ::TestStack;
      friend class ::TestPQueue;
      friend class ::TestHash;
   public:
      // constructors, destructors, and assignment operator
      iterator() { this->p = nullptr; }
      iterator(T* p) { this->p = p; }
      iterator(const iterator& rhs) { this->p = rhs.p; }
      iterator(size_t index, vector<T>& v) { this->p = v.data + index; }
      iterator& operator = (const iterator& rhs)
      {
         this->p = rhs.p;
         return *this;
      }

      // equals, not equals operator
      bool operator != (const iterator& rhs) const { return !(this->p == rhs.p); }
      bool operator == (const iterator& rhs) const { return rhs.p == this->p; }

      // dereference operator
      T& operator * ()
      {
         return *p;
      }

      // prefix increment
      iterator& operator ++ ()
      {
         p++;
         return *this;
      }

      // postfix increment
      iterator operator ++ (int postfix)
      {
         iterator temp = *this;
         p++;
         return temp;
      }

      // prefix decrement
      iterator& operator -- ()
      {
         p--;
         return *this;
      }

      // postfix decrement
      iterator operator -- (int postfix)
      {
         iterator temp = *this;
         p--;
         return temp;
      }

   private:
      T* p;
   };


   /*****************************************
    * VECTOR :: NON-DEFAULT constructors
    * non-default constructor: set the number of elements,
    * construct each element, and copy the values over
    ****************************************/
   template <typename T, typename A>
   vector <T, A> ::vector(const A& a) : data(nullptr), numElements(0), numCapacity(0), alloc(a)
   {

   }


   /*****************************************
    * VECTOR :: NON-DEFAULT constructors
    * non-default constructor: set the number of elements,
    * construct each element, and copy the values over
    ****************************************/
   template <typename T, typename A>
   vector <T, A> ::vector(size_t num, const T& t, const A& a) 
      : data(nullptr), numElements(0), numCapacity(0), alloc(a)
   {
      //do nothing if there is nothing to do
      if (num > 0)
      {
         data = alloc.allocate(num);
         numCapacity = num;

         // copy the value
         std::uninitialized_fill_n(data, num, t);
         numElements = num;
      }
   }

   /*****************************************
    * VECTOR :: INITIALIZATION LIST constructors
    * Create a vector with an initialization list.
    ****************************************/
   template <typename T, typename A>
   vector <T, A> ::vector(const std::initializer_list<T>& l, const A& a)
      : data(nullptr), numElements(0), numCapacity(0), alloc(a)
   {
      data = alloc.allocate(l.size());
      numCapacity = l.size();

      int i = 0;
      for (auto it = l.begin(); it != l.end(); ++it)
      {
         alloc.construct(&data[i], *it);
         i++;
      }

      numElements = l.size();

   }

   /*****************************************
    * VECTOR :: NON-DEFAULT constructors
    * non-default constructor: set the number of elements,
    * construct each element, and copy the values over
    ****************************************/
   template <typename T, typename A>
   vector <T, A> ::vector(size_t num, const A& a)
      : data(nullptr), numElements(0), numCapacity(0), alloc(a)
   {
      //if (num < 1)
      //   data = nullptr;
      //else
      //   data = new T[num];
      //numElements = num;
      //numCapacity = num;

      if (num > 0)
      {
         data = alloc.allocate(num);
         numCapacity = num;

         for (size_t i = 0; i < num; i++)
            new (&data[i]) T;

         numElements = num;
      }
   }

   /*****************************************
    * VECTOR :: COPY CONSTRUCTOR
    * Allocate the space for numElements and
    * call the copy constructor on each element
    ****************************************/
   template <typename T, typename A>
   vector <T, A> ::vector(const vector& rhs)
      : data(nullptr), numElements(0), numCapacity(0), alloc(rhs.alloc)
   {
      if (!rhs.empty())
      {
         data = alloc.allocate(rhs.numElements);
         numCapacity = rhs.numElements;

         for (size_t i = 0; i < rhs.numElements; i++)
         {
            alloc.construct(&data[i], rhs.data[i]);
         }

         numElements = rhs.numElements;
      }
   }

   /*****************************************
    * VECTOR :: MOVE CONSTRUCTOR
    * Steal the values from the RHS and set it to zero.
    ****************************************/
   template <typename T, typename A>
   vector <T, A> ::vector(vector&& rhs) : alloc(rhs.alloc)
   {
      data = rhs.data;
      rhs.data = nullptr;

      numElements = rhs.numElements;
      rhs.numElements = 0;

      numCapacity = rhs.numCapacity;
      rhs.numCapacity = 0;
   }

   /*****************************************
    * VECTOR :: DESTRUCTOR
    * Call the destructor for each element from 0..numElements
    * and then free the memory
    ****************************************/
   template <typename T, typename A>
   vector <T, A> :: ~vector()
   {
      if (numCapacity != 0)
      {
         assert(nullptr != data);
         // Call the destructor on all elements 
         for (size_t i = 0; i < numElements; i++)
            alloc.destroy(&data[i]);

         // then free all the memory
         alloc.deallocate(data, numCapacity);
      }
   }

   /***************************************
    * VECTOR :: RESIZE
    * This method will adjust the size to newElements.
    * This will either grow or shrink newElements.
    *     INPUT  : newCapacity the size of the new buffer
    *     OUTPUT :
    **************************************/
   template <typename T, typename A>
   void vector <T, A> ::resize(size_t newElements)
   {
      // decrease the size
      if (newElements < numElements)
      {
         // destroy those off the end
         for (size_t i = newElements; i < numElements; i++)
         {
            alloc.destroy(&data[i]);
         }
      }
      // grow as necessary
      else if (newElements > numElements)
      {
         // increase the capacity
         if (newElements > numCapacity)
            reserve(newElements);
         // add the new elements
         for (size_t i = numElements; i < newElements; i++)
            alloc.construct(&data[i]);
      }
      numElements = newElements;

   }

   template <typename T, typename A>
   void vector <T, A> ::resize(size_t newElements, const T& t)
   {
      // decrease the size
      if (newElements < numElements)
      {
         for (size_t i = newElements; i < numElements; i++)
         {
            alloc.destroy(&data[i]);
         }
      }
      else if (newElements > numElements)
      {
         // increase the capacity
         if (newElements > numCapacity)
            reserve(newElements);
         // add the new elements
         for (size_t i = numElements; i < newElements; i++)
            alloc.construct(&data[i], t);
      }
      numElements = newElements;

   }

   /***************************************
    * VECTOR :: RESERVE
    * This method will grow the current buffer
    * to newCapacity.  It will also copy all
    * the data from the old buffer into the new
    *     INPUT  : newCapacity the size of the new buffer
    *     OUTPUT :
    **************************************/
   template <typename T, typename A>
   void vector <T, A> ::reserve(size_t newCapacity)
   {
      // Do nothing if we are big enough
      if (newCapacity <= numCapacity)
         return;

      // allocate new array
      T* newData = alloc.allocate(newCapacity);

      // copy data from old array
      for (auto i = 0; i < numElements; i++)
         new ((void*)(newData + i)) T(std::move(data[i]));

      // delete the old and assign the new
      if (nullptr != data)
      {
         for (size_t i = 0; i < numElements; i++)
         {
            alloc.destroy(&data[i]);
         }
         alloc.deallocate(data, numCapacity);
      }

      data = newData;
      numCapacity = newCapacity;
   }

   /***************************************
    * VECTOR :: SHRINK TO FIT
    * Get rid of any extra capacity
    *     INPUT  :
    *     OUTPUT :
    **************************************/
   template <typename T, typename A>
   void vector <T, A> ::shrink_to_fit()
   {
      if (numElements == numCapacity)
         return;

      T* dataNew;
      
      if (numElements != 0)
      {
         dataNew = alloc.allocate(numElements);
         std::uninitialized_copy(data, data + numElements, dataNew);
      }
      else
         dataNew = nullptr;


      // delete the old and assign the new
      if (nullptr != data)
      {
         // call the destructor on the elements being destroyed
         for (size_t i = 0; i < numElements; i++)
            alloc.destroy(&data[i]);

         // actually delete the memory
         alloc.deallocate(data, numCapacity);
      }

      data = dataNew;
      numCapacity = numElements;
   }



   /*****************************************
    * VECTOR :: SUBSCRIPT
    * Read-Write access
    ****************************************/
   template <typename T, typename A>
   T& vector <T, A> :: operator [] (size_t index)
   {
      return (data[index]);
   }

   /******************************************
    * VECTOR :: SUBSCRIPT
    * Read-Write access
    *****************************************/
   template <typename T, typename A>
   const T& vector <T, A> :: operator [] (size_t index) const
   {
      return (data[index]);
   }

   /*****************************************
    * VECTOR :: FRONT
    * Read-Write access
    ****************************************/
   template <typename T, typename A>
   T& vector <T, A> ::front()
   {
      return data[0];
   }

   /******************************************
    * VECTOR :: FRONT
    * Read-Write access
    *****************************************/
   template <typename T, typename A>
   const T& vector <T, A> ::front() const
   {
      return data[0];
   }

   /*****************************************
    * VECTOR :: BACK
    * Read-Write access
    ****************************************/
   template <typename T, typename A>
   T& vector <T, A> ::back()
   {
      return data[numElements - 1];
   }

   /******************************************
    * VECTOR :: BACK
    * Read-Write access
    *****************************************/
   template <typename T, typename A>
   const T& vector <T, A> ::back() const
   {
      return data[numElements - 1];
   }

   /***************************************
    * VECTOR :: PUSH BACK
    * This method will add the element 't' to the
    * end of the current buffer.  It will also grow
    * the buffer as needed to accomodate the new element
    *     INPUT  : 't' the new element to be added
    *     OUTPUT : *this
    **************************************/
   template <typename T, typename A>
   void vector <T, A> ::push_back(const T& t)
   {
      if (0 == numCapacity)
         reserve(1);
      if (numElements == numCapacity)
         reserve(numCapacity * 2);
      new ((void*)(data + numElements++)) T(t);
   }

   template <typename T, typename A>
   void vector <T, A> ::push_back(T&& t)
   {
      if (0 == numCapacity)
         reserve(1);
      if (numElements == numCapacity)
         reserve(numCapacity * 2);
      new ((void*)(data + numElements++)) T(std::move(t));
   }

   /***************************************
    * VECTOR :: ASSIGNMENT
    * This operator will copy the contents of the
    * rhs onto *this, growing the buffer as needed
    *     INPUT  : rhs the vector to copy from
    *     OUTPUT : *this
    **************************************/
   template <typename T, typename A>
   vector <T, A>& vector <T, A> :: operator = (const vector& rhs)
   {


 
      // if this and rhs are the same size
      if (rhs.numElements == numElements)
      {
         // no allocation required. Just call the assignment operator
         for (size_t i = 0; i < rhs.numElements; i++)
            data[i] = rhs.data[i];
      }
      // if rhs is bigger than this
      else if (rhs.numElements > numElements)
      {
         // if there is sufficient capacity but we just need to construct
         if (rhs.numElements <= numCapacity)
         {
            // copy into the already filled slots
            for (size_t i = 0; i < numElements; i++)
               data[i] = rhs.data[i];

            // copy-construct the rest
            for (size_t i = numElements; i < rhs.numElements; i++)
               alloc.construct(&data[i], rhs.data[i]);

            numElements = rhs.numElements;
         }
         // if there is not sufficient room and we need to grow
         else
         {
            // create enough room
            T* pNew = alloc.allocate(rhs.numElements);

            // copy-construct the existing elements
            for (size_t i = 0; i < rhs.numElements; i++)
               alloc.construct(&pNew[i], rhs.data[i]);

            // get rid of the old buffer
            clear();

            alloc.deallocate(data, numCapacity);

            // set up the member variables
            data = pNew;
            numElements = rhs.numElements;
            numCapacity = rhs.numElements;
         }
      }

      // if this is bigger than rhs 
      else
      {
         //copy over the elements
         for (size_t i = 0; i < rhs.numElements; i++)
            data[i] = rhs.data[i];
         // destroy other spots
         for (size_t i = rhs.numElements; i < numElements; i++)
            alloc.destroy(&data[i]);
         numElements = rhs.numElements;
      }
      return *this;
   }
   template <typename T, typename A>
   vector <T, A>& vector <T, A> :: operator = (vector&& rhs)
   {
      swap(rhs);
      rhs.clear();
      rhs.shrink_to_fit();
      return *this;
   }




} // namespace custom

