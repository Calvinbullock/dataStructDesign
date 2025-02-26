/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    Daniel Malasky, Calvin Bullock
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>
#include <utility>
#include <memory>     // for std::allocator
#include <functional> // for std::less
#include <utility>    // for std::pair

class TestBST; // forward declaration for unit tests
class TestSet;
class TestMap;

namespace custom
{

   template <typename TT>
   class set;
   template <typename KK, typename VV>
   class map;

/*****************************************************************
 * BINARY SEARCH TREE
 * Create a Binary Search Tree
 *****************************************************************/
template <typename T>
class BST
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestSet;
   friend class ::TestMap;

   template <class TT>
   friend class custom::set;

   template <class KK, class VV>
   friend class custom::map;
public:
   //
   // Construct
   //

   BST();
   BST(const BST &  rhs);
   BST(      BST && rhs);
   BST(const std::initializer_list<T>& il);
   ~BST();

   //
   // Assign
   //

   BST & operator = (const BST &  rhs);
   BST & operator = (      BST && rhs);
   BST & operator = (const std::initializer_list<T>& il);
   void swap(BST & rhs);

   //
   // Iterator
   //

   class iterator;
   iterator   begin() const noexcept;
   iterator   end()   const noexcept { return iterator(nullptr); }

   //
   // Access
   //

   iterator find(const T& t);

   //
   // Insert
   //

   std::pair<iterator, bool> insert(const T&  t, bool keepUnique = false);
   std::pair<iterator, bool> insert(      T&& t, bool keepUnique = false);

   //
   // Remove
   //

   iterator erase(iterator& it);
   void   clear() noexcept;

   //
   // Status
   //

   bool   empty() const noexcept { return size() == 0; }
   size_t size()  const noexcept { return numElements; }

private:

   class BNode;
   BNode * root;              // root node of the binary search tree
   size_t numElements;        // number of elements currently in the tree
};


/*****************************************************************
 * BINARY NODE
 * A single node in a binary tree. Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <typename T>
class BST <T> :: BNode
{
public:
   //
   // Construct
   //
   BNode()           : pLeft(nullptr), pRight(nullptr), pParent(nullptr), data(),             isRed(true) { }
   BNode(const T& t) : pLeft(nullptr), pRight(nullptr), pParent(nullptr), data(t),            isRed(true) { }
   BNode(T&& t)      : pLeft(nullptr), pRight(nullptr), pParent(nullptr), data(std::move(t)), isRed(true) { }

   // compare
   bool operator == (const BNode& rhs) const
   {
      return
      this->data    == rhs.data    &&                 // Actual data stored in the BNode
      this->pLeft   == rhs.pLeft   &&         // Left child - smaller
      this->pRight  == rhs.pRight  &&        // Right child - larger
      this->pParent == rhs.pParent &&       // Parent
      this->isRed   == rhs.isRed;
   }

   //
   // assign
   //
   void assign(BNode *& pDest, const BNode * pSrc);

   //
   // Insert
   //
   void addLeft (BNode * pNode);
   void addRight(BNode * pNode);
   void addLeft (const  T &  t);
   void addRight(const  T &  t);
   void addLeft (       T && t);
   void addRight(       T && t);

   //
   // Status
   //
   bool isRightChild(BNode * pNode) const { return pParent->pRight == this; }
   bool isLeftChild(BNode* pNode) const   { return pParent->pLeft  == this; }

   // balance the tree
   void balance();

   //
   // Remove
   //
   void clear(BNode *& pNode);

   //
   // Swap
   //
   void swap(BNode*& pRHS);

#ifdef DEBUG
   //
   // Verify
   //
   std::pair <T,T> verifyBTree() const;
   int findDepth() const;
   bool verifyRedBlack(int depth) const;
   int computeSize() const;
#endif // DEBUG

   //
   // Data
   //
   T data;                  // Actual data stored in the BNode
   BNode* pLeft;          // Left child - smaller
   BNode* pRight;         // Right child - larger
   BNode* pParent;        // Parent
   bool isRed;              // Red-black balancing stuff
};

/**********************************************************
 * BINARY SEARCH TREE ITERATOR
 * Forward and reverse iterator through a BST
 *********************************************************/
template <typename T>
class BST <T> :: iterator
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestSet;
   friend class ::TestMap;

   template <class KK, class VV>
   friend class custom::map;
public:
   // constructors and assignment
   iterator(BNode* p = nullptr): pNode()
   {
      this->pNode = p;
   }
   iterator(const iterator & rhs)
   {
      this->pNode = rhs.pNode;
   }
   iterator & operator = (const iterator & rhs)
   {
      this->pNode = rhs.pNode;
      return *this;
   }

   // compare
   bool operator == (const iterator & rhs) const
   {

      return this->pNode == rhs.pNode;
   }
   bool operator != (const iterator & rhs) const
   {
      return true;
   }

   // de-reference. Cannot change because it will invalidate the BST
   const T & operator * () const
   {
      return this->pNode->data;
   }

   // increment and decrement
   iterator & operator ++ ();
   iterator   operator ++ (int postfix)
   {
      return *this;
   }
   iterator & operator -- ();
   iterator   operator -- (int postfix)
   {
      return *this;;
   }

   // must give friend status to remove so it can call getNode() from it
   friend BST <T> :: iterator BST <T> :: erase(iterator & it);

private:

    // the node
    BNode * pNode;
};


/*********************************************
 *********************************************
 *********************************************
 ******************** BST ********************
 *********************************************
 *********************************************
 *********************************************/


 /*********************************************
  * BST :: DEFAULT CONSTRUCTOR
  ********************************************/
template <typename T>
BST <T> ::BST() : root(), numElements()
{
   numElements = 0;
   root = nullptr;
}

/*********************************************
 * BST :: COPY CONSTRUCTOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST ( const BST<T>& rhs)
{
   this->root = nullptr;
   this->numElements = rhs.numElements;

   *this = rhs;
   this->root->balance();
}

/*********************************************
 * BST :: MOVE CONSTRUCTOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST(BST <T> && rhs)
{
   this->numElements = rhs.numElements;
   this->root = rhs.root;

   rhs.numElements = 0;
   rhs.root = nullptr;
}

/*********************************************
 * BST :: INITIALIZER LIST CONSTRUCTOR
 * Create a BST from an initializer list
 ********************************************/
template <typename T>
BST <T> ::BST(const std::initializer_list<T>& il)
{
   numElements = 99;
   root = new BNode;
}

/*********************************************
 * BST :: DESTRUCTOR
 ********************************************/
template <typename T>
BST <T> :: ~BST()
{
   clear();
}


/*********************************************
 * BST :: ASSIGNMENT OPERATOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const BST <T> & rhs)
{
   root->assign(this->root, rhs.root);
   this->numElements = rhs.numElements;
   return *this;
}

/*********************************************
 * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
 * Copy nodes onto a BTree
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const std::initializer_list<T>& il)
{
   clear();
   for (auto t : il)
   {
      insert(t);
   }
   return *this;
}

/*********************************************
 * BST :: ASSIGN-MOVE OPERATOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (BST <T> && rhs)
{
   clear();
   swap(rhs);
   return *this;
}

/*********************************************
 * BST :: SWAP
 * Swap two trees
 ********************************************/
template <typename T>
void BST <T> :: swap (BST <T>& rhs)
{
   BNode * tempRoot = rhs.root;
   rhs.root = this->root;
   this->root = tempRoot;

   int tempElements = rhs.numElements;
   rhs.numElements = this->numElements;
   this->numElements = tempElements;
}

/*****************************************************
 * BST :: INSERT
 * Insert a node at a given location in the tree
 ****************************************************/
template <typename T>
std::pair<typename BST <T> :: iterator, bool> BST <T> :: insert(const T & t, bool keepUnique)
{
   //std::pair<iterator, bool> pairReturn(end(), false);
   //BNode * pCurr = this->root;

   //while (pCurr != nullptr)
   //{
   //   // go left (less than)
   //   if (t < pCurr->data)
   //      // next node exists
   //      if (pCurr->pLeft)
   //         pCurr = pCurr->pLeft;
   //      else
   //      {
   //         // create node
   //         pCurr->addLeft(t);

   //         // set return
   //         pairReturn.first = iterator(pCurr->pLeft);
   //         pairReturn.second = true;

   //         numElements++;
   //         pCurr->pLeft->balance();
   //         return pairReturn;
   //      }

   //   // go right (greater than)
   //   else
   //      // next node exists
   //      if (pCurr->pRight)
   //         pCurr = pCurr->pRight;
   //      else
   //      {
   //         // create node
   //         pCurr->addRight(t);

   //         // set return
   //         pairReturn.first = iterator(pCurr->pRight);
   //         pairReturn.second = true;

   //         numElements++;
   //         pCurr->pRight->balance();
   //         return pairReturn;
   //      }
   //}
   //return pairReturn;

   std::pair<iterator, bool> pairReturn(end(), false);
   return pairReturn;
}

template <typename T>
std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(T && t, bool keepUnique)
{
   std::pair<iterator, bool> pairReturn(end(), false);
   return pairReturn;
}

/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the iterator
 ************************************************/
template <typename T>
typename BST <T> ::iterator BST <T> :: erase(iterator & it)
{
   // it is at the end
   if (it == end())
      return end();

   // create temp it to move and return
   iterator temp = it;
   ++temp;

   // case 1 - Target has no children
   if (it.pNode->pRight == nullptr && it.pNode->pLeft == nullptr)
   {
      if (it.pNode->pParent != nullptr && it.pNode->pParent->pRight == it.pNode)
         it.pNode->pParent = nullptr;

      if (it.pNode->pParent != NULL && it.pNode->pParent->pLeft == it.pNode)
         it.pNode->pParent->pLeft = NULL;

      delete it.pNode;
   }

   // case 2 - one child, left child.
   else if (it.pNode->pRight == nullptr && it.pNode->pLeft != nullptr)
   {
      // hook up parent
      it.pNode->pLeft->pParent = it.pNode->pParent;

      // hook up child
      if (it.pNode->pParent != nullptr && it.pNode->pParent->pRight == it.pNode)
         it.pNode->pParent->pRight = it.pNode->pLeft;
      if (it.pNode->pParent != nullptr && it.pNode->pParent->pLeft == it.pNode)
         it.pNode->pParent->pLeft = it.pNode->pLeft;
      delete it.pNode;
   }

   // case 2 - one child, right child.
   else if (it.pNode->pLeft == nullptr && it.pNode->pRight != nullptr)
   {
      // hook up parent
      it.pNode->pRight->pParent = it.pNode->pParent;

      // hook up child
      if (it.pNode->pParent != nullptr && it.pNode->pParent->pRight == it.pNode)
         it.pNode->pParent->pRight = it.pNode->pRight;
      if (it.pNode->pParent != nullptr && it.pNode->pParent->pLeft == it.pNode)
         it.pNode->pParent->pLeft = it.pNode->pRight;
      delete it.pNode;
   }

   // case 3 - two kids
   else if (it.pNode->pLeft != nullptr && it.pNode->pRight != nullptr)
   {
      // TODO:
   }

   this->numElements -= 1;
   return temp;
}

/*****************************************************
 * BST :: CLEAR
 * Removes all the BNodes from a tree
 ****************************************************/
template <typename T>
void BST <T> ::clear() noexcept
{
   if (root)
   {
      root->clear(root);
   }
   root = nullptr;  // Prevent dangling pointer
   numElements = 0;
}

/*****************************************************
 * BST :: BEGIN
 * Return the first node (left-most) in a binary search tree
 ****************************************************/
template <typename T>
typename BST <T> :: iterator custom :: BST <T> :: begin() const noexcept
{
   if (empty())
      return end();
   BNode * pNode = root;

   while (pNode->pLeft)
      pNode = pNode->pLeft;

   return iterator(pNode);
}


/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <typename T>
typename BST <T> :: iterator BST<T> :: find(const T & t)
{
   BNode * p = this->root;

   while (p)
   {
      if (p->data == t)
         return iterator(p);
      else if (t < p->data)
         p = p->pLeft;
      else
         p = p->pRight;
   }

   return end();
}

/******************************************************
 ******************************************************
 ******************************************************
 *********************** B NODE ***********************
 ******************************************************
 ******************************************************
 ******************************************************/


/**********************************************
 * assign
 * copy the values from pSrc onto pDest preserving
 * as many of the nodes as possible.
 *********************************************/
template <typename T>
void BST <T> :: BNode :: assign(BNode * & pDest, const BNode * pSrc)
{
   // src is empty
   if (pSrc == nullptr)
   {
      clear(pDest);
      return;
   }

   // dest is empty, create a new node
   if (pDest == nullptr)
   {
      pDest = new BNode(pSrc->data);
      pDest->isRed = pSrc->isRed;
   }

   // Neither src nor dest is empty, update the node
   else
      pDest->data = pSrc->data;

   // Recursively loop through tree
   assign(pDest->pRight, pSrc->pRight);
   assign(pDest->pLeft, pSrc->pLeft);

   // Hookup parents
   if (pDest->pRight)
      pDest->pRight->pParent = pDest;
   if (pDest->pLeft)
      pDest->pLeft->pParent = pDest;
}


/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addLeft (BNode * pNode)
{
   if (pNode != nullptr)
      pNode->pParent = this;
   this->pLeft = pNode;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (BNode * pAdd)
{
   if (pAdd != nullptr)
      pAdd->pParent = this;
   this->pRight = pAdd;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft (const T & t)
{
   BNode * pAdd = new BNode(t);
   pAdd->pParent = this;
   this->pLeft = pAdd;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> ::BNode::addLeft(T && t)
{
   BNode * pAdd = new BNode(std::move(t));
   pAdd->pParent = this;
   this->pLeft = pAdd;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (const T & t)
{
   BNode * pAdd = new BNode(t);
   pAdd->pParent = this;
   this->pRight = pAdd;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> ::BNode::addRight(T && t)
{
   BNode* pAdd = new BNode(std::move(t));
   pAdd->pParent = this;
   this->pRight = pAdd;
}

#ifdef DEBUG
/****************************************************
 * BINARY NODE :: FIND DEPTH
 * Find the depth of the black nodes. This is useful for
 * verifying that a given red-black tree is valid
 ****************************************************/
template <typename T>
int BST <T> :: BNode :: findDepth() const
{
   // if there are no children, the depth is ourselves
   if (pRight == nullptr && pLeft == nullptr)
      return (isRed ? 0 : 1);

   // if there is a right child, go that way
   if (pRight != nullptr)
      return (isRed ? 0 : 1) + pRight->findDepth();
   else
      return (isRed ? 0 : 1) + pLeft->findDepth();
}

/****************************************************
 * BINARY NODE :: VERIFY RED BLACK
 * Do all four red-black rules work here?
 ***************************************************/
template <typename T>
bool BST <T> :: BNode :: verifyRedBlack(int depth) const
{
   bool fReturn = true;
   depth -= (isRed == false) ? 1 : 0;

   // Rule a) Every node is either red or black
   assert(isRed == true || isRed == false); // this feels silly

   // Rule b) The root is black
   if (pParent == nullptr)
      if (isRed == true)
         fReturn = false;

   // Rule c) Red nodes have black children
   if (isRed == true)
   {
      if (pLeft != nullptr)
         if (pLeft->isRed == true)
            fReturn = false;

      if (pRight != nullptr)
         if (pRight->isRed == true)
            fReturn = false;
   }

   // Rule d) Every path from a leaf to the root has the same # of black nodes
   if (pLeft == nullptr && pRight && nullptr)
      if (depth != 0)
         fReturn = false;
   if (pLeft != nullptr)
      if (!pLeft->verifyRedBlack(depth))
         fReturn = false;
   if (pRight != nullptr)
      if (!pRight->verifyRedBlack(depth))
         fReturn = false;

   return fReturn;
}


/******************************************************
 * VERIFY B TREE
 * Verify that the tree is correctly formed
 ******************************************************/
template <typename T>
std::pair <T, T> BST <T> :: BNode :: verifyBTree() const
{
   // largest and smallest values
   std::pair <T, T> extremes;
   extremes.first = data;
   extremes.second = data;

   // check parent
   if (pParent)
      assert(pParent->pLeft == this || pParent->pRight == this);

   // check left, the smaller sub-tree
   if (pLeft)
   {
      assert(!(data < pLeft->data));
      assert(pLeft->pParent == this);
      pLeft->verifyBTree();
      std::pair <T, T> p = pLeft->verifyBTree();
      assert(!(data < p.second));
      extremes.first = p.first;

   }

   // check right
   if (pRight)
   {
      assert(!(pRight->data < data));
      assert(pRight->pParent == this);
      pRight->verifyBTree();

      std::pair <T, T> p = pRight->verifyBTree();
      assert(!(p.first < data));
      extremes.second = p.second;
   }

   // return answer
   return extremes;
}

/*********************************************
 * COMPUTE SIZE
 * Verify that the BST is as large as we think it is
 ********************************************/
template <typename T>
int BST <T> :: BNode :: computeSize() const
{
   return 1 +
      (pLeft  == nullptr ? 0 : pLeft->computeSize()) +
      (pRight == nullptr ? 0 : pRight->computeSize());
}
#endif // DEBUG

/******************************************************
 * BINARY NODE :: BALANCE
 * Balance the tree from a given location
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: balance()
{
   if (this == nullptr)
      return;

   // Case 1: if we are the root, then color ourselves black and call it a day.
   if (pParent == nullptr)
   {
      isRed = false;
      return;
   }

   // Case 2: if the parent is black, then there is nothing left to do
   if (!pParent->isRed)
      return;

   BNode * pGranny  = nullptr;
   BNode * pAunt    = nullptr;
   BNode * pSibling = nullptr;
   
   // Set the Aunt & Granny
   // If granny exists
   if (pParent->pParent)
   {
      pGranny = pParent->pParent;
      // If parent is left child
      if (pParent == pGranny->pLeft)
         pAunt = pGranny->pRight; // aunt is right child
      else
         pAunt = pGranny->pLeft; // aunt is left child, parent is right
   }

   // Set the sibling
   // if we are left child
   if (this == pParent->pLeft)
      pSibling = pParent->pRight; // pSibling == right child
   else
      pSibling = pParent->pLeft; // pSibling == left child, we are right child

   bool isAuntBlackOrNull = (pAunt == nullptr || !pAunt->isRed);


   // Case 3: if the aunt and parent are red, then just recolor
   if (pAunt && pParent->isRed && pAunt->isRed)
   {
      pParent->isRed = false; // parent = black
      pAunt->isRed   = false; // aunt   = black
      pGranny->isRed = true;  // gma    = red
      pGranny->balance();     // recursively balance
      return;
   }

   //// Case 4: if the aunt non-existant or black, then we need to rotate
   //if (isAuntBlackOrNull)
   //{
   //   // Case 4a: We are parent's left and parent is granny's left
   //   if (pParent->pLeft == this && pGranny->pLeft == pParent)
   //   {
   //      pParent->addRight(std::move(pGranny)); // move granny to the left
   //      pGranny->addLeft(pParent->pLeft); // move the sibling to granny's right
   //      
   //      //root = pParent; // set the root?

   //      pParent->pRight->isRed = true; // set granny to red
   //      pParent->isRed = false;        // set parent to black
   //      return;
   //   }

      // case 4b: We are parent's right and parent is granny's right
      // parent is red and granny is black


      // Case 4c: We are parents's right and mom is granny's left
      //if (pParent->pRight == this && pParent->pParent->pLeft == pParent)
      //{

      //}
      //// case 4d: we are parent's left and mom is granny's right
      //if (pParent->pLeft == this && pParent->pParent->pRight == pParent)
      //{

      //}
   //}
}

/*****************************************************
 * DELETE BINARY TREE
 * Delete all the nodes below pThis including pThis
 * using postfix traverse: LRV
 ****************************************************/
template <typename T>
void BST <T> :: BNode :: clear(BNode *& pNode)
{
   if (!pNode)
      return;

   clear(pNode->pLeft);
   clear(pNode->pRight);
   delete pNode;
   pNode = nullptr;
}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <typename T>
void BST <T> ::BNode:: swap(BNode *& pRHS)
{
   std::swap(this, pRHS);
}

/*************************************************
 *************************************************
 *************************************************
 ****************** ITERATOR *********************
 *************************************************
 *************************************************
 *************************************************/

/**************************************************
 * BST ITERATOR :: INCREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator ++ ()
{
   // At the end
   if (!pNode)
      return *this;

   // case 1 - Has right child
   if (this->pNode->pRight)
   {
      this->pNode = this->pNode->pRight;

      while (this->pNode->pLeft)
         this->pNode = this->pNode->pLeft;

      return *this;
   }

   // case 2 - We have no right child and we are parent left child
   if (this->pNode->pRight == nullptr &&
       this->pNode->pParent->pLeft == pNode)
   {
      this->pNode = pNode->pParent;
      return *this;
   }

   // case 3 - We have no right child and we are parent right child
   if (this->pNode->pRight == nullptr &&
       this->pNode->pParent->pRight == this->pNode)
   {
      // Parent exists, we are right child
      while (this->pNode->pParent && this->pNode->pParent->pRight == this->pNode)
         this->pNode = this->pNode->pParent;

      this->pNode = pNode->pParent;
      return *this;
   }

   return *this;
}

/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator -- () {
   // At the end
   if (!pNode)
      return *this;

   // case 1 - Has left child
   if (this->pNode->pLeft)
   {
      this->pNode = this->pNode->pLeft;

      while (this->pNode->pRight)
         this->pNode = this->pNode->pRight;

      return *this;
   }

   // case 2 - We have no left child and we are parent right child
   if (this->pNode->pLeft == nullptr &&
       this->pNode->pParent->pRight == pNode)
   {
      this->pNode = pNode->pParent;
      return *this;
   }

   // case 3 - We have no left child and we are parent left child
   if (this->pNode->pLeft == nullptr &&
       this->pNode->pParent->pLeft == this->pNode)
   {
      // Parent exists, we are left child
      while (this->pNode->pParent && this->pNode->pParent->pLeft == this->pNode)
         this->pNode = this->pNode->pParent;

      this->pNode = pNode->pParent;
      return *this;
   }

   return *this;
}

} // namespace custom


