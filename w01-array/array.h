/***********************************************************************
 * Header:
 *    Array
 * Summary:
 *    Our custom implementation of std::array
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *       array             : similar to std::array
 *       array :: iterator : an iterator through the array
 * Author
 *    Calvin Bullock
 ************************************************************************/

#pragma once

#include <stdexcept>

class TestArray;

namespace custom
{

/************************************************
 * ARRAY
 * A class that holds stuff
 ***********************************************/
template <int N>
struct array
{
public:
   //
   // Construct
   //

   // No constructors or destructors needed here

   //
   // Assign
   //

   // No assignment operators needed here

   //
   // Iterator
   //

   // Forward declaration for the iterator class
   class iterator;

   // Begin and end methods
   iterator begin() { return iterator(__elems_); }
   iterator end() { return iterator(__elems_ + N); }

   //
   // Access
   //

   // Subscript
   int& operator [] (size_t index)
   {
      // Perform bounds check in index
      if (index <= N)
         return *(__elems_ + index);

      throw std::out_of_range("Index out of bounds");
   }
   const int& operator [] (size_t index) const
   {
      // Perform bounds check in index
      if (index <= N)
         return *(__elems_ + index);

      throw std::out_of_range("Index out of bounds");
   }
   // At
   int& at(size_t index)
   {
      // Perform bounds check in index
      if (index <= N)
         return *(__elems_ + index);

      throw std::out_of_range("Index out of bounds");
   }
   const int& at(size_t index) const
   {
      // Perform bounds check in index
      if (index <= N)
         return *(__elems_ + index);

      throw std::out_of_range("Index out of bounds");

   }

   // Front and Back
   int& front() { return *(__elems_); }
   int& back() { return *(__elems_ + N - 1); }
   const int& front() const { return *(__elems_); }
   const int& back()  const { return *(__elems_ + N - 1); }

   //
   // Remove

   // No clear or erase here

   //
   // Status
   //

   // number of elements in the array
   size_t size()  const { return N; }
   bool empty() const { return N == 0; }

   //
   // Member Variable
   //

   // statically allocated array of T
   int __elems_[N];
};


/**************************************************
 * ARRAY ITERATOR
 * An iterator through array
 *************************************************/
template <int N>
class array <N> :: iterator
{
   friend class ::TestArray;
public:
   // constructors, destructors, and assignment operator
   iterator() { p = nullptr; }
   iterator(int * p) : p(p) { }
   iterator(const iterator & rhs) { p = rhs.p; }
   iterator & operator = (const iterator & rhs)
   {
      /* your code goes here; it should copy an iterator */
      p = rhs.p;
      return *this;
   }

   // equals, not equals operator
   bool operator != (const iterator & rhs) const { return !(p == rhs.p); }
   bool operator == (const iterator & rhs) const { return p == rhs.p; }

   // dereference operator
   int & operator * () { return *(p); }
   const int & operator * () const { return *(p); }

   // prefix increment
   iterator & operator ++ ()
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

private:
   int * p;
};



}; // namespace custom
