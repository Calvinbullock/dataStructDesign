/***********************************************************************
 * Header:
 *    DEQUE
 * Summary:
 *    Our custom implementation of a deque 
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        deque                 : A class that represents a deque
 *        deque::iterator       : An iterator through a deque
 * Author
 *    Daniel Malasky, Calvin Bullock
 ************************************************************************/

#pragma once

// Debug stuff
#include <cassert>
#include <memory>   // for std::allocator

class TestDeque;    // forward declaration for TestDeque unit test class

namespace custom
{

/******************************************************
 * DEQUE
 *****************************************************/
template <typename T, typename A = std::allocator<T>>
class deque
{
   friend class ::TestDeque; // give unit tests access to the privates
public:

   // 
   // Construct
   //
   deque(const A & a = A()) 
   { 
      data = nullptr;
      numCells = 16;    
      numBlocks = 0;   
      numElements = 0; 
      iaFront = 0;               

   }
   deque(deque & rhs);
   ~deque()
   {
      clear();
   }

   //
   // Assign
   //
   deque & operator = (deque& rhs);

   // 
   // Iterator
   //
   class iterator;
   iterator begin() 
   { 
      return iterator(0, this);
   }
   iterator end()   
   { 
      return iterator(numElements, this);
   }

   // 
   // Access
   //
   T & front()       
   { 
      assert(numElements != 0);
      assert(nullptr != data[ibFromID(0)]);
      return data[ibFromID(0)][icFromID(0)];
   }
   const T & front() const 
   {
      assert(numElements != 0);
      assert(nullptr != data[ibFromID(0)]);
      return data[ibFromID(0)][icFromID(0)];
   }
   T & back()
   {
      assert(numElements != 0);
      assert(nullptr != data[ibFromID(numElements - 1)]);
      return data[ibFromID(numElements - 1)][icFromID(numElements - 1)];
   }
   const T & back() const
   {
      assert(numElements != 0);
      assert(nullptr != data[ibFromID(numElements - 1)]);
      return data[ibFromID(numElements - 1)][icFromID(numElements - 1)];
   }
   T & operator[](int id)
   {
      assert(id >= 0 && id < numElements);
      assert(nullptr != data[ibFromID(id)]);
      return data[ibFromID(id)][icFromID(id)];
   }
   const T & operator[](int id) const
   {
      assert(id >= 0 && id < numElements);
      assert(nullptr != data[ibFromID(id)]);
      return data[ibFromID(id)][icFromID(id)];
   }

   //
   // Insert
   //
   void push_back(const T & t);
   void push_back(T && t);
   void push_front(const T& t);
   void push_front(T&& t);

   //
   // Remove
   //
   void pop_front();
   void pop_back();
   void clear();

   //
   // Status
   //
   size_t size()  const { return numElements; }
   bool   empty() const { return numElements == 0; }
   
private:

   // TODO: fix divide by zero
   // array index from deque index
   int iaFromID(int id) const
   {
      return (id + iaFront) % (numCells * numBlocks);
   }

   // block index from deque index
   int ibFromID(int id) const
   {
      return iaFromID(id) / numCells;
   }

   // cell index from deque index
   int icFromID(int id) const
   {
      return iaFromID(id) % numCells;
   }

   // reallocate
   void reallocate(int numBlocksNew);

   A    alloc;                // use alloacator for memory allocation
   size_t numCells;           // number of cells in a block
   size_t numBlocks;          // number of blocks in the data array
   size_t numElements;        // number of elements in the deque
   int iaFront;               // array-centered index of the front of the deque
   T ** data;                 // array of arrays
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
template <typename T, typename A>
class deque <T, A> ::iterator
{
   friend class ::TestDeque; // give unit tests access to the privates
public:
   // 
   // Construct
   //
   iterator() : id(), d()
   {
   }
   iterator(int id, deque* d) : id(), d()
   {
      this->id = id;
      this->d = d;
   }
   iterator(const iterator& rhs) 
   {
      *this = rhs;
   }

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
      this->d = rhs.d;
      this->id = rhs.id;
      return *this;
   }

   // 
   // Compare
   //
   bool operator != (const iterator& rhs) const
   {
      return !(*this == rhs);
   }
   bool operator == (const iterator& rhs) const
   {
      return this->d == rhs.d && this->id == rhs.id;
   }

   // 
   // Access
   //
   T& operator * ()
   {
      return d->data[d->ibFromID(id)][d->icFromID(id)];
   }

   // 
   // Arithmetic
   //
   int operator - (iterator it) const
   {
      return this->id - it.id;
   }
   iterator& operator += (int offset)
   {
      this->id += offset;
      return *this;
   }
   iterator& operator ++ ()
   {
      ++(this->id);
      return *this;
   }
   iterator operator ++ (int postfix)
   {
      iterator temp = *this;
      ++(this->id);
      return temp;
   }
   iterator& operator -- ()
   {
      ++this->id;
      return *this;
   }
   iterator operator -- (int postfix)
   {
      iterator temp = *this;
      ++this->id;
      return temp;
   }

private:
   int id;
   deque* d;
};

/*****************************************
 * DEQUE :: COPY CONSTRUCTOR
 * Allocate the space for the elements and
 * call the copy constructor on each element
 ****************************************/
template <typename T, typename A>
deque <T, A> ::deque(deque& rhs) 
{
   *this = rhs;
}

/*****************************************
 * DEQUE :: COPY-ASSIGN
 * Allocate the space for the elements and
 * call the copy constructor on each element
 ****************************************/
template <typename T, typename A>
deque <T, A> & deque <T, A> :: operator = (deque & rhs)
{
   this->clear();
   for (auto it = rhs.begin(); it != rhs.end(); ++it)
   {
      this->push_back(*it);
   }
   
   //iterator itLHS = this->begin();
   //iterator itRHS = rhs.begin();

   //while (itLHS != this->end() && itRHS != rhs.end())
   //{
   //   *itLHS = *itRHS;
   //   ++itLHS;
   //   ++itRHS;
   //}



   return *this;
}

/*****************************************
 * DEQUE :: PUSH_BACK
 * add an element to the back of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_back(const T& t)
{
   // Reallocate the array of blocks as needed
   if (numElements == numBlocks * numCells)
      reallocate((numBlocks == 0) ? 0 : numBlocks * 2);

   // Allocate a new block as needed
   size_t ib = ibFromID(numElements);          // Find index of new block
   if (data[ib] == nullptr)
   {
      data[ib] = new T[numCells];   // Use an allocator or new
      //numBlocks++;
   }

   // Assign the value into the block
   new((void*)(&(data[ib][icFromID(numElements)]))) T(t);
   numElements++;
}

/*****************************************
 * DEQUE :: PUSH_BACK - move
 * add an element to the back of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_back(T && t)
{
}

/*****************************************
 * DEQUE :: PUSH_FRONT
 * add an element to the front of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_front(const T& t)
{
   // Reallocate the array of blocks as needed
   if (numElements == numBlocks * numCells)
      reallocate((numBlocks == 0) ? 0 : numBlocks * 2);

   // Adjust the front array index, wrapping as needed
   if (iaFront != 0)
      iaFront--;
   else
      iaFront = numBlocks * numCells - 1;

   // Allocate a new block as needed
   size_t ib = ibFromID(0);          // Find index of new block
   if (data[ib] == nullptr)
      data[ib] = new T[numCells];   // Use an allocator or new

   // Assign the value into the block
   data[ib][icFromID(numElements)] = t;
   numElements++;
}

/*****************************************
 * DEQUE :: PUSH_FRONT - move
 * add an element to the front of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_front(T&& t)
{
}

/*****************************************
 * DEQUE :: CLEAR
 * Remove all the elements from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::clear()
{
   // delete the elements
   for (size_t id = 0; id < numElements; id++)
      alloc.destroy(&data[ibFromID(id)][icFromID(id)]);

   // Delete the blocks themselves
   for (size_t ib = 0; ib < numBlocks; ib++)
   {
      if (data[ib] != nullptr)
      {
         alloc.destroy(&data[ib]);
         data[ib] = nullptr;
      }
   }

   numElements = 0;

   
}

/*****************************************
 * DEQUE :: POP FRONT
 * Remove the front element from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> :: pop_front()
{
   size_t idRemove = 0;

   // call deconsturctor on back element
   alloc.destroy(&data[ibFromID(idRemove)][icFromID(idRemove)]);

   // delete block as needed
   if (numElements == 1
      || (icFromID(idRemove) == (numCells - 1) && ibFromID(idRemove) != ibFromID(numCells - 1)))
   {
      alloc.destroy(&data[ibFromID(idRemove)]);
      data[ibFromID(idRemove)] = nullptr;
   }
   numElements--;
   iaFront = iaFromID(1);
}

/*****************************************
 * DEQUE :: POP BACK
 * Remove the back element from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::pop_back()
{
   size_t idRemove = numElements - 1;

   // call deconsturctor on back element
   alloc.destroy(&data[ibFromID(idRemove)][icFromID(idRemove)]);

   // delete block as needed
   if (numElements == 1
      || (icFromID(idRemove) == 0 && ibFromID(idRemove) != ibFromID(0)))
   {
      alloc.destroy(&data[ibFromID(idRemove)]);
      data[ibFromID(idRemove)] = nullptr;
   }
   numElements--;
}

/*****************************************
 * DEQUE :: REALLOCATE
 * Remove all the elements from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> :: reallocate(int numBlocksNew)
{
   // Allocate a new array of pointers that is the requested size
   T** dataNew = new T * [numBlocksNew];

   // Copy over the pointers, unwrapping as we go
   size_t ibNew = 0;
   for (size_t ibOld = 0; ibOld < numElements; ibOld+= numCells)
   {
      dataNew[ibNew] = data[ibFromID(ibOld)];
      ibNew++;
   }

   // Set all the block pointers to NULL when there are no block to point to
   while (ibNew < numBlocksNew)
   {
      dataNew[ibNew] = nullptr;
      ibNew++;
   }

   // If the back element is in the fron element's block, then move it
   if (numElements < 0 &&
      ibFromID(0) == ibFromID(numElements - 1) &&
      icFromID(0) > icFromID(numElements - 1))
   {
      size_t ibFrontOld = ibFromID(0);
      size_t ibBackOld = ibFromID(numElements - 1);
      size_t ibBackNew = numElements / numCells;
      dataNew[ibBackNew] = new T[numCells];
      for (size_t ic = 0; ic <= icFromID(numElements - 1); ic++) // check <
      {
         new((void*)(&(dataNew[ibBackNew][ic]))) T(std::move(data[ibBackOld][ic]));
      }

      // Change the deque's member variables with the new values
      if (data)
         alloc.destroy(&data);
      data = dataNew;
      numBlocks = numBlocksNew;
      iaFront = iaFront % numCells;
   }
}




} // namespace custom
