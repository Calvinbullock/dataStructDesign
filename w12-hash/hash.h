/***********************************************************************
 * Header:
 *    HASH
 * Summary:
 *    Our custom implementation of std::unordered_Set
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        unordered_set           : A class that represents a hash
 *        unordered_set::iterator : An interator through hash
 * Author
 *    Daniel Malasky, Calvin Bullock
 ************************************************************************/

#pragma once

#include "list.h"     // because this->buckets[0] is a list
#include "vector.h"   // because this->buckets is a vector
#include <memory>     // for std::allocator
#include <functional> // for std::hash
#include <cmath>      // for std::ceil


class TestHash;             // forward declaration for Hash unit tests

namespace custom
{
/************************************************
 * UNORDERED SET
 * A set implemented as a hash
 ************************************************/
template <typename T,
          typename Hash = std::hash<T>,
          typename EqPred = std::equal_to<T>,
          typename A = std::allocator<T> >
class unordered_set
{
   friend class ::TestHash;   // give unit tests access to the privates
   template <class TT, class HHash, class EEqPred, class AA>
   friend void swap(unordered_set<TT,HHash,EEqPred,AA>& lhs, unordered_set<TT,HHash,EEqPred,AA>& rhs);
public:
   //
   // Construct
   //
   unordered_set() : maxLoadFactor(1.0), numElements(0), buckets(8)
   {
   }
   unordered_set(size_t numBuckets): maxLoadFactor(1.0), numElements(0), buckets(numBuckets)
   {
   }
   unordered_set(const unordered_set&  rhs)
      : maxLoadFactor(rhs.maxLoadFactor), numElements(rhs.numElements), buckets(rhs.buckets)
   {
   }
   unordered_set(unordered_set&& rhs)
      : maxLoadFactor(std::move(rhs.maxLoadFactor)),
      numElements(std::move(rhs.numElements)), buckets(std::move(rhs.buckets))
   {
      *this = std::move(rhs);
   }
   template <class Iterator>
   unordered_set(Iterator first, Iterator last)
   {
      //reserve(last - first);
      //for (auto it = first; it != last; ++first)
      //   insert(*it);
   }

   //
   // Assign
   //
   unordered_set& operator=(const unordered_set& rhs)
   {
      this->numElements = rhs.numElements;
      this->maxLoadFactor = rhs.maxLoadFactor;
      this->buckets = rhs.buckets;
      return *this;
   }
   unordered_set& operator=(unordered_set&& rhs)
   {
      this->numElements = std::move(rhs.numElements);
      this->maxLoadFactor = std::move(rhs.maxLoadFactor);
      this->buckets = std::move(rhs.buckets);
      rhs.numElements = 0;
      rhs.maxLoadFactor = 1.0;
      rhs.buckets.resize(8);
      return *this;
   }
   unordered_set& operator=(const std::initializer_list<T>& il)
   {
      return *this;
   }
   void swap(unordered_set& rhs)
   {
      std::swap(this->numElements, rhs.numElements);
      std::swap(this->maxLoadFactor, rhs.maxLoadFactor);
      std::swap(this->buckets, rhs.buckets);
   }

   //
   // Iterator
   //
   class iterator;
   class local_iterator;
   iterator begin()
   {
      //return iterator();
      for (auto itBucket = buckets.begin(); itBucket != buckets.end(); ++itBucket)
         if (!(*itBucket).empty())
            return iterator(buckets.end(), itBucket, (*itBucket).begin());
      return end();
   }
   iterator end()
   {
      return iterator(buckets.end(), buckets.end(), buckets[0].end());
   }
   local_iterator begin(size_t iBucket)
   {
      return local_iterator(buckets[iBucket].begin());
   }
   local_iterator end(size_t iBucket)
   {
      return local_iterator(buckets[iBucket].end());
   }

   //
   // Access
   //
   size_t bucket(const T& t)
   {
      return (size_t)hasher(t) % bucket_count();
   }
   iterator find(const T& t);

   //
   // Insert
   //
   custom::pair<iterator, bool> insert(const T& t);
   void insert(const std::initializer_list<T> & il);
   void rehash(size_t numBuckets);
   void reserve(size_t num)
   {
      rehash(num / maxLoadFactor);
   }

   //
   // Remove
   //
   void clear() noexcept
   {
       for (auto& bucket : buckets)
       {
          bucket.clear();
       }
       numElements = 0;
   }
   iterator erase(const T& t);

   //
   // Status
   //
   size_t size() const
   {
      return (size_t)numElements;
   }
   bool empty() const
   {
      return size() == 0;
   }
   size_t bucket_count() const
   {
      return (size_t)buckets.size();
   }
   size_t bucket_size(size_t i) const
   {
      return (size_t)buckets[i].size();
   }
   float load_factor() const noexcept
   {
      return (float)size() / bucket_count();
   }
   float max_load_factor() const noexcept
   {
      return maxLoadFactor;
   }
   void  max_load_factor(float m)
   {
      maxLoadFactor = m;
   }

private:

   size_t min_buckets_required(size_t num) const
   {
      return (size_t)num / maxLoadFactor;
   }

   Hash hasher;
   custom::vector<custom::list<T,A>> buckets;  // each bucket in the hash
   int numElements;                            // number of elements in the Hash
   float maxLoadFactor;                        // the ratio of elements to buckets signifying a rehash
   A alloc;
};


/************************************************
 * UNORDERED SET ITERATOR
 * Iterator for an unordered set
 ************************************************/
template <typename T, typename H, typename E, typename A>
class unordered_set <T, H, E, A> ::iterator
{
   friend class ::TestHash;   // give unit tests access to the privates
   template <typename TT, typename HH, typename EE, typename AA>
   friend class custom::unordered_set;
public:
   //
   // Construct
   iterator() : itList(), itVector(), itVectorEnd(nullptr)
   {
   }
   iterator(const typename custom::vector<custom::list<T> >::iterator& itVectorEnd,
            const typename custom::vector<custom::list<T> >::iterator& itVector,
            const typename custom::list<T>::iterator &itList)
            : itList(itList), itVector(itVector), itVectorEnd(itVectorEnd)
   {
   }
   iterator(const iterator& rhs) : itList(rhs.itList),
            itVector(rhs.itVector), itVectorEnd(rhs.itVectorEnd)
   {
   }

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
      this->itList = rhs.itList;
      this->itVector = rhs.itVector;
      this->itVectorEnd = rhs.itVectorEnd;
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
      return this->itList == rhs.itList
         && this->itVector == rhs.itVector
         && this->itVectorEnd == rhs.itVectorEnd;
   }

   //
   // Access
   //
   T& operator * ()
   {
      return *itList;
   }

   //
   // Arithmetic
   //
   iterator& operator ++ ();
   iterator operator ++ (int postfix)
   {
      return *this;
   }

private:
   typename vector<list<T>>::iterator itVectorEnd;
   typename list<T>::iterator itList;
   typename vector<list<T>>::iterator itVector;
};


/************************************************
 * UNORDERED SET LOCAL ITERATOR
 * Iterator for a single bucket in an unordered set
 ************************************************/
template <typename T, typename H, typename E, typename A>
class unordered_set <T, H, E, A> ::local_iterator
{
   friend class ::TestHash;   // give unit tests access to the privates

   template <typename TT, typename HH, typename EE, typename AA>
   friend class custom::unordered_set;
public:
   //
   // Construct
   //
   local_iterator() : itList()
   {
   }
   local_iterator(const typename custom::list<T>::iterator& itList) : itList()
   {
      this->itList = itList;
   }
   local_iterator(const local_iterator& rhs) : itList()
   {
      this->itList = rhs.itList;
   }

   //
   // Assign
   //
   local_iterator& operator = (const local_iterator& rhs)
   {
      this->itList = rhs.itList;
      return *this;
   }

   //
   // Compare
   //
   bool operator != (const local_iterator& rhs) const
   {
      return !(this == rhs);
   }
   bool operator == (const local_iterator& rhs) const
   {
      return this->itList == rhs.itList;
   }

   //
   // Access
   //
   T& operator * ()
   {
      return *itList;
   }

   //
   // Arithmetic
   //
   local_iterator& operator ++ ()
   {
      ++(this->itList);
      return *this;
   }
   local_iterator operator ++ (int postfix)
   {
      auto temp = *this;
      (this->itList)++;
      return temp;
   }

private:
   typename list<T>::iterator itList;
};


/*****************************************
 * UNORDERED SET :: ERASE
 * Remove one element from the unordered set
 ****************************************/
template <typename T, typename Hash, typename E, typename A>
typename unordered_set <T, Hash, E, A> ::iterator unordered_set<T,Hash,E,A>::erase(const T& t)
{
   // 1. Find element to be erased. Return end() if the element is not present.
   iterator itErase = find(t);
   if (itErase == end())
   {
      return itErase;
   }

   // 2. Determine the return value.
   iterator itReturn = itErase;
   ++itReturn;

   // 3. Erase the element from the unordered_set.
   (*itErase.itVector).erase(itErase.itList);
   numElements--;

   // 4. Return iterator to the next element.
   return itReturn;
}

/*****************************************
 * UNORDERED SET :: INSERT
 * Insert one element into the hash
 ****************************************/
template <typename T, typename H, typename E, typename A>
custom::pair<typename custom::unordered_set<T, H, E, A>::iterator, bool> unordered_set<T, H, E, A>::insert(const T& t)
{
   // 1. Find the bucket where the new element is to reside.
   size_t iBucket = bucket(t); // hash it?
   iterator itHash;

   // 2. See if the element is already there. If so, then return out.
   for (auto it = buckets[iBucket].begin(); it != buckets[iBucket].end(); ++it)
   {
      if (*it == t)
      {
         itHash = find(*it);
         return custom::pair<typename custom::unordered_set<T, H, E, A>::iterator, bool>(itHash, false); // itHash?
      }
   }

   // 3. Reserve more space if we are already at the limit.
   if (min_buckets_required(numElements + 1) > bucket_count())
   {
      reserve(numElements * 2);
      iBucket = bucket(t); // Recalculate bucket since rehashing might have changed it
   }

   // 4. Actually insert the new element on the back of the bucket.
   buckets[iBucket].push_back(t);
   itHash = find(t);
   numElements++;

   // 5. Return the results.
   return custom::pair<typename custom::unordered_set<T, H, E, A>::iterator, bool>(itHash, true);
}
template <typename T, typename H, typename E, typename A>
void unordered_set<T, H, E, A>::insert(const std::initializer_list<T> & il)
{
   return custom::pair<custom::unordered_set<T, H, E, A>::iterator, bool>(iterator(), true);
}

/*****************************************
 * UNORDERED SET :: REHASH
 * Re-Hash the unordered set by numBuckets
 ****************************************/
template <typename T, typename Hash, typename E, typename A>
void unordered_set<T, Hash, E, A>::rehash(size_t numBuckets)
{
   // If the current bucket count is sufficient, then do nothing.
   if (numBuckets <= bucket_count())
      return;

   // Create a new hash bucket.
   custom::vector<custom::list<T, A>> bucketsNew(numBuckets);

   // Insert all elements into the new hash table.
   for (auto it = begin(); it != end(); ++it)
      bucketsNew[hasher(*it) % numBuckets].push_back(std::move(*it));

   // Swap the old bucket for the new.
   std::swap(buckets, bucketsNew);
}


/*****************************************
 * UNORDERED SET :: FIND
 * Find an element in an unordered set
 ****************************************/
template <typename T, typename H, typename E, typename A>
typename unordered_set <T, H, E, A> ::iterator unordered_set<T, H, E, A>::find(const T& t)
{
   for (auto it = begin(); it != end(); ++it)
      if (*it == t)
         return it;
   return end();
}

/*****************************************
 * UNORDERED SET :: ITERATOR :: INCREMENT
 * Advance by one element in an unordered set
 ****************************************/
template <typename T, typename H, typename E, typename A>
typename unordered_set <T, H, E, A> ::iterator & unordered_set<T, H, E, A>::iterator::operator ++ ()
{
   // 1. only advance if we are not already at the end
   if (itVector == itVectorEnd)
   {
      return *this;
   }

   // 2. advance the list it, if we are not at the end, then we are done
   ++itList;
   if (itList != (*itVector).end())
   {
      return *this;
   }

   // 3. we are at the end of the list. Find the next bucket
   ++itVector;
   while (itVector != itVectorEnd && (*itVector).empty())
   {
      ++itVector;
   }
   if (itVector != itVectorEnd)
   {
      itList = (*itVector).begin();
   }
   return *this;
}

/*****************************************
 * SWAP
 * Stand-alone unordered set swap
 ****************************************/
template <typename T, typename H, typename E, typename A>
void swap(unordered_set<T,H,E,A>& lhs, unordered_set<T,H,E,A>& rhs)
{
      std::swap(lhs.numElements, rhs.numElements);
      std::swap(lhs.maxLoadFactor, rhs.maxLoadFactor);
      std::swap(lhs.buckets, rhs.buckets);
}

}
