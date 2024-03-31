// mymap.h: A replica of standard map library for C++
// 
// Author: Saloni Mhatre
// Net ID: smhatr4@uic.edu 
// 
// Originally made as a solution to Project 5 for CS 251 class taught by Prof. J. Hummel
//
#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <algorithm>  // for std::max()
#include <set>
#include <vector>

using namespace std;

template<typename keyType, typename valueType>
class mymap {
 private:
    struct NODE {
        keyType key;      // used to build BST
        valueType value;  // stored data for the map
        NODE* left;       // links to left child
        NODE* right;      // links to right child
        int nL;           // number of nodes in left subtree
        int nR;           // number of nodes in right subtree
        bool isThreaded;  // Threading leaves to next inorder parent
    };
    NODE* root;  // pointer to root node of the BST
    int size;    // # of key/value pairs in the mymap

    //
    // iterator:
    // This iterator is used so that mymap will work with a foreach loop.
    //
    struct iterator {
     private:
        NODE* curr;  // points to current in-order node for begin/end

     public:
        iterator(NODE* node) {
            curr = node;
        }

        keyType operator *() {
            return curr -> key;
        }

        bool operator ==(const iterator& rhs) {
            if (curr == nullptr && rhs.curr == nullptr) {
              return this == &rhs;
            }
            return curr == rhs.curr;
        }

        bool operator !=(const iterator& rhs) {
            if (curr == nullptr && rhs.curr == nullptr) {
              return this == &rhs;
            }
            return curr != rhs.curr;
        }

        bool isDefault() {
            return !curr;
        }
        
        //
        // operator++:
        //
        // This function should advance curr to the next in-order node.
        // O(logN)
        //
        iterator operator++() {
          // if Threaded: then the thread would lead to the next in-order node
          if (curr->isThreaded) {
              curr = curr->right;
          } else {
            // going one step right and all the way to left
            curr = curr->right;
            while (curr != nullptr && curr->left != nullptr) {
              curr = curr->left;
            }
          }
          return *this;
        }
        
        // getNR: gives the nR for the curr
        int getNR() {
          return curr->nR;
        }
        // getNL: gives the nL for the curr
        int getNL() {
          return curr->nL;
        }
    };
    
    //
    // private helper functions:
    //
    
    // this one returns the root NODE
    NODE* _getRoot() {
      return this->root;
    }
    // _goRightOf()
    // gets the Right Node.
    // Since the tree is threaded, we will have two choices
    // of getting a right node. We need the unthreaded right
    // Nodes of the tree
    //
    NODE* _goRightOf(NODE* N) {
      if (N->isThreaded)
        return nullptr;
      else
        return N->right;
    }

    // _goLeftOf: gets the left node.
    NODE* _goLeftOf(NODE* N) {
      return N->left;
    }
    
    //
    // removeTree(NODE N)
    //
    // will remove every NODE that branches out from N
    // and lastly the NODE N itself (Uses recursion)
    // A tree like:    (P)
    //                  N
    //                A   B
    //              C       D
    // Resulting tree: (P) where P is the Parent Node of N
    //
    void _removeTree(NODE* N) {
        if (N == nullptr)
          return;                   // Base Case: N is deleted
        
        // removing subtrees first
        if (!N->isThreaded) {
          _removeTree(N->right);    // passing all the right subtrees
        }
        _removeTree(N->left);       // passing all the left subtrees
        
        // deleting the passed NODE N
        delete(N);
        size--;
    }
    
    // newNode(key, value): creates a newNode
    NODE* _newNode(keyType key, valueType value) {
      NODE* newN = new NODE();

      newN->key = key;
      newN->value = value;
      newN->left = nullptr;
      newN->right = nullptr;
      newN->nL = 0;
      newN->nR = 0;
      newN->isThreaded = false;
      
      return newN;
    }
    
    // _search(): to search the tree
    NODE* _search(keyType key, bool& result) {
        NODE* cur = root;
        
        while (cur != nullptr) {
          if (cur->key == key) {
              result = true;
              return cur;
          } else if (key < cur->key) {
              cur = _goLeftOf(cur);
          } else {
              cur = _goRightOf(cur);
          }
        }
        result = false;
        return nullptr;
    }
    
    // _preOrder(output): traverses the tree pre-order
    void _preOrder(NODE* N, ostream& output) {
      if (N == nullptr)
        return;    // Base Case
      
      // printing the current Node's data
      output << "key: " << N->key << ", nL: " << N->nL << ", nR: " << N->nR << endl;
      
      // recursion to the all-left subtrees now
      _preOrder(_goLeftOf(N), output);
      // recursion to the all-right subtrees now
      _preOrder(_goRightOf(N), output);
    }
    
    // _inOrder(output): traverses the tree in-order
    void _inOrder(NODE* N, ostream& output) {
        if (N == nullptr)
          return;     // Base Case
        
        // recursion to the all-left subtrees first
        _inOrder(_goLeftOf(N), output);
        // outputing the key-value pairs
        output << "key: " << N->key << " value: " << N->value << endl;
        // recursion to the all-right subtrees now
        _inOrder(_goRightOf(N), output);
    }
    
    // _postOrder(output): traverses the tree in post-order
    void _postOrder(NODE* N, ostream& output) {
      if (N = nullptr)
        return;     // Base Case
      
      // recursion to the all left subtrees first
      _postOrder(_goLeftOf(N), output);
      // recursion to the all right subtrees now
      _postOrder(_goRightOf(N), output);
      // outputing the current Node's data
      output << "key: " << N->key << " value: " << N->value << endl;
    }
    
    // _bottomLeft: returns the leftmost leaf
    NODE* _bottomLeft(NODE* N) {
      while (N->left != nullptr)
        N = N->left;
      return N;
    }
    
    // _put(): puts the key:value pair into THIS tree: O(log N)
    void _put(keyType key, valueType value, vector<NODE*>& unbalanced) 
    {
      // EDGE CASE: if tree is empty, adding it right away
      // O(1)
      if (root == nullptr) {
          NODE* newN = _newNode(key, value);
          root = newN;
          size++;
          return;
      }
      
      // 1. checking if the node already exists or not: O(log N) + O(1)
      bool exists;
      NODE* tempNode = _search(key, exists);
      if (exists) {
          // already exists
          tempNode->value = value;  // updating the value;
          return;
      }
      // If here, means the node doesn't exist
      NODE* newN = _newNode(key, value);   // creating and allocating memory for new node
      
      // 2. Traversing through the *existing* tree: O(log N)
      NODE* prev = nullptr;
      NODE* cur = root;
      while (cur != nullptr) {
        // checking if the NODE is balanced or not
        if (!_seesawBalanced(cur)) {
          unbalanced.push_back(cur);
        }
        
        // key comparisons for insertion
        if (cur->key > key) {
          // going left
          cur->nL += 1;     // increasing the nX since it is going to increase either way
          prev = cur;
          cur = _goLeftOf(cur);
        } else {
          // going right
          cur->nR += 1;
          prev = cur;
          cur = _goRightOf(cur);
        }
      }
      
      // 3. Linking the newly created node to Tree now
      // prev should be pointing where we fall off the tree now
      // O(1)
      if (prev->key < key) {
      // going RIGHT of prev
      
        if (prev->isThreaded) {
          newN->right = prev->right;   // inheriting the thread from previous node.
          newN->isThreaded = true;     // changing newN's thread status
          prev->isThreaded = false;    // changing prev's thread status
        }
        prev->right = newN;
      } else {
      // going LEFT of prev
        
        newN->right = prev;
        newN->isThreaded = true;
        prev->left = newN;
      }
      
      // 4. increasing the size now: O(1)
      size++;
    }
    
    // _insert(): same as _put() O(log N),
    // but this one to prevent infinite recursion inside public: put()
    void _insert(keyType key, valueType value) {
      // EDGE CASE: if tree is empty, adding it right away
        // O(1)
        if (this->root == nullptr) {
            NODE* newN = _newNode(key, value);
            this->root = newN;
            size++;
            return;
        }

        // 1. checking if the node already exists or not: O(log N) + O(1)
        bool exists;
        NODE* tempNode = this->_search(key, exists);
        if (exists) {
            // already exists
            tempNode->value = value;  // updating the value;
            return;
        }
        // If here, means the node doesn't exist
        NODE* newN = _newNode(key, value);   // creating and allocating memory for new node

        // 2. Traversing through the *existing* tree: O(log N)
        NODE* prev = nullptr;
        NODE* cur = this->root;
        while (cur != nullptr) {
          // checking if the NODE is balanced or not
          if (!_seesawBalanced(cur)) {
          }

          // key comparisons for insertion
          if (cur->key > key) {
            // going left
            cur->nL += 1;     // increasing the nX since it is going to increase either way
            prev = cur;
            cur = _goLeftOf(cur);
          } else {
            // going right
            cur->nR += 1;
            prev = cur;
            cur = _goRightOf(cur);
          }
        }

        // 3. Linking the newly created node to Tree now
        // prev should be pointing where we fall off the tree now
        // O(1)
        if (prev->key < key) {
        // going RIGHT of prev

          if (prev->isThreaded) {
            newN->right = prev->right;   // inheriting the thread from previous node.
            newN->isThreaded = true;     // changing newN's thread status
            prev->isThreaded = false;    // changing prev's thread status
          }
          prev->right = newN;
        } else {
        // going LEFT of prev

          newN->right = prev;
          newN->isThreaded = true;
          prev->left = newN;
        }

        // 4. increasing the size now: O(1)
        this->size++;
    }
    
    // _deepCopy(): copies the other map into this
    void _deepCopy(NODE* N) {
      if (N == nullptr)
        return;
      else {
        _insert(N->key, N->value);  // inserting the value
        _deepCopy(_goLeftOf(N));    // copying left
        _deepCopy(_goRightOf(N));   // copying right
      }
    }
    
    // _toVector(): creates a vector of all key-value pairs from the provided node
    vector< pair<keyType, valueType> > _toVector(NODE* N) {
      vector< pair<keyType, valueType> > tree;
      
      iterator iter = iterator(N);
      while (iter != end()) {
        pair<keyType,valueType> p;
          p.first = *iter;
          p.second = get(*iter);
          tree.push_back(p);
          ++iter;
      }
      return tree;
    }
    
    // _seesawBalanced(): checks if the tree is seesaw balanced or not
    bool _seesawBalanced(NODE* N) {
      if (max(N->nR, N->nL) <= ( (2 * min(N->nR, N->nL)) + 1) )
        return true;
      else
        return false;
    }
    

 public:
    //
    // default constructor:
    //
    // Creates an empty mymap.
    // Time complexity: O(1)
    //
    mymap() {
      this->root = nullptr;
      this->size = 0;
    }

    //
    // copy constructor:
    //
    // Constructs a new mymap which is a copy of the "other" mymap.
    // Sets all member variables appropriately.
    // Time complexity: O(NlogN), where N is total number of nodes in threaded,
    // self-balancing BST.
    //
    mymap(const mymap& other) {
      root = nullptr;
      size = 0;
      _deepCopy(other.root);
    }
    
    //
    // vector constructor:
    //
    // constructs an *unbalanced* map given a vector of sorted
    // key-value pairs
    //
    mymap(const vector< pair<keyType, valueType> >& V) {
      this->root = nullptr;
      this->size = 0;
      
      int low = 0;
      int high = V.size()-1;
      int mid = (low + high) / 2;
      
      // inserting mid->low elements first
      for (int i = mid; i >= low; --i) {
        _insert(V[i].first, V[i].second);
      }
      // inserting mid->high elements now
      for (int i = mid; i <= high; ++i) {
        _insert(V[i].first, V[i].second);
      }
    }

    //
    // operator=:
    //
    // Clears "this" mymap and then makes a copy of the "other" mymap.
    // Sets all member variables appropriately.
    // Time complexity: O(n), where n is total number of nodes in threaded,
    // self-balancing BST.
    //
    mymap& operator=(const mymap& other) {
        // deleting the current tree first
        clear();
        this->root = nullptr;
        this->size = 0;
        
        _deepCopy(other.root);
        
        return *this;
    }

    // clear:
    //
    // Frees the memory associated with the mymap;
    // can be used for testing.
    // Time complexity: O(n), where n is total number of nodes in threaded,
    // self-balancing BST.
    //
    void clear() {
        if (root == nullptr)
          return;

        // removing Nodes from the root and including root.
        _removeTree(root);
        root = nullptr;
    }

    //
    // destructor:
    //
    // Frees the memory associated with the mymap.
    // Time complexity: O(n), where n is total number of nodes in threaded,
    // self-balancing BST.
    //
    ~mymap() {
        if (root == nullptr)
          return;
        
        // removing Nodes from the root and including root.
        _removeTree(root);
    }

    //
    // put:
    //
    // Inserts the key/value into the threaded, self-balancing BST based on
    // the key.
    // Time complexity: O(logn + mlogm), where n is total number of nodes in the
    // threaded, self-balancing BST and m is the number of nodes in the
    // sub-tree that needs to be re-balanced.
    // Space complexity: O(1)
    //
    void put(keyType key, valueType value) {
      vector<NODE*> unbalanced;       // vector to store unbalanced nodes
      _put(key, value, unbalanced);   // inserting the value
      
      if (unbalanced.size() == 0)
        return;
        
      // deleting duplicates from the unbalanced vector
      set<NODE*> s;
      for (int i = 0; i < unbalanced.size(); ++i) {
        s.insert( unbalanced[i] );
      }
      unbalanced.assign(s.begin(), s.end());  // copying values from the start of set to end of set
      /*
      cout << "________________________________" << endl;
      for (NODE* N:unbalanced) {
        cout << "Line 528: " << N->key << endl;
      }
      cout << "________________________________" << endl;
      */
      
      // now "Balancing-Act"
      while (unbalanced.size() != 0)
      {
        // creating a temp vector that stores the keys from the current node
        vector< pair<keyType, valueType> > tempTree = _toVector(unbalanced[0]);
        // eg = { {1,1}, {2,2}, {3,3} }
        
        // creating a tree from that vector
        mymap<keyType, valueType> tempMap(tempTree);
        // cout << "Line 546: " << endl;
        // cout << tempMap.toString() << endl;
        
        // getting the node where the unbalance occured
        NODE* prev = nullptr;
        NODE* cur = this->root;
        while (cur != unbalanced[0])
        {
          prev = cur;
          if (unbalanced[0]->key < cur->key) {
            cur = this->_goLeftOf(cur);
          } else {
              cur = this->_goRightOf(cur);
          }
        }
        
        /* *******
        // removing the cur and everything under that
        // if the tree is to be fixed at the root
        if (prev == nullptr) {
          clear();
          this->root = nullptr;
          this->size = 0;
          _deepCopy(tempMap._getRoot());
          return;
        } ******/
        /*
        for (int i = 0; i < tempTree.size(); ++i) {
          pair<keyType, valueType> p = tempTree[i];
          this->_insert(p.first, p.second);
        } */
        
        
        // cur = the NODE of unbalance, prev->Next = cur (unthreaded)
        // removing everything including cur
        if (prev->left = cur) {
          this->_removeTree(cur);
          prev->left = tempMap._getRoot();
        } else {
          this->_removeTree(cur);
          prev->right = tempMap._getRoot();
        }
        
        // deleting the thus balanced tree from THE node
        unbalanced.erase(unbalanced.begin());
      }
      
    }

    //
    // contains:
    // Returns true if the key is in mymap, return false if not.
    // Time complexity: O(logn), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    bool contains(keyType key) {
        bool exists;
        NODE* tempNode = _search(key, exists);
        if (exists)
          return true;
        else
          return false;
    }

    //
    // get:
    //
    // Returns the value for the given key; if the key is not found, the
    // default value, valueType(), is returned (but not added to mymap).
    // Time complexity: O(logn), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    valueType get(keyType key) {
        bool result;
        NODE* N = _search(key, result);
        if (result)
          return N->value;
        else
          return valueType();
    }

    //
    // operator[]:
    //
    // Returns the value for the given key; if the key is not found,
    // the default value, valueType(), is returned (and the resulting new
    // key/value pair is inserted into the map).
    // Time complexity: O(logn + mlogm), where n is total number of nodes in the
    // threaded, self-balancing BST and m is the number of nodes in the
    // sub-trees that need to be re-balanced.
    // Space complexity: O(1)
    //
    valueType operator[](keyType key) {
      bool exists = false;
      NODE* N = _search(key, exists);  // O(log N)
      
      if (exists)
        return get(key);    // O(log N)
      else {
        put(key, valueType());  // O(log N)
        return valueType();
      }
    }

    //
    // Size:
    //
    // Returns the # of key/value pairs in the mymap, 0 if empty.
    // O(1)
    //
    int Size() {
        return this->size;
    }

    //
    // begin:
    //
    // returns an iterator to the first in order NODE.
    // Time complexity: O(logn), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    iterator begin() {
      if (root == nullptr)
        return iterator(nullptr);    // if tree is empty return the nullptr

      return iterator(_bottomLeft(root));
    }
    //
    // end:
    //
    // returns an iterator to the last in order NODE.
    // this function is given to you.
    // 
    // Time Complexity: O(1)
    //
    iterator end() {
        return iterator(nullptr);
    }

    //
    // toString:
    //
    // Returns a string of the entire mymap, in order.
    // Format for 8/80, 15/150, 20/200:
    // "key: 8 value: 80\nkey: 15 value: 150\nkey: 20 value: 200\n
    // Time complexity: O(n), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    string toString() {
        stringstream parser;
        _inOrder(root, parser);
        string pairs = parser.str();
        return pairs;
    }

    //
    // toVector:
    //
    // Returns a vector of the entire map, in order.  For 8/80, 15/150, 20/200:
    // {{8, 80}, {15, 150}, {20, 200}}
    // Time complexity: O(n), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    vector< pair<keyType, valueType> > toVector() {
      return _toVector(root); // returning a vector starting from root
    }

    //
    // checkBalance:
    //
    // Returns a string of mymap that verifies that the tree is properly
    // balanced.  For example, if keys: 1, 2, 3 are inserted in that order,
    // function should return a string in this format (in pre-order):
    // "key: 2, nL: 1, nR: 1\nkey: 1, nL: 0, nR: 0\nkey: 3, nL: 0, nR: 0\n";
    // Time complexity: O(n), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    string checkBalance() {
        stringstream parser;
        _preOrder(root, parser);
        string pairs = parser.str();
        return pairs;
    }
    
    
    
    //
    // DEBUGGER FUNCTIONS BELOW:
    //
    
    // checkThreads
    // only used for debugging
    // Will start at the bottom-most-left node
    // go right until end, print each key:value pair
    void checkThreads() {
      // going to the bottom-most-left first
      NODE* cur = root;
      while (cur->left != nullptr) {
        cur = cur->left;
      }
      // cur should be at the bottom-left node
      while (cur->right != nullptr) {
        cout << "key: " << cur->key;
        cout << " value: " << cur->value;
        cout << " nL = " << cur->nL;
        cout << " nR = " << cur->nR;
        cout << " isThreaded = ";
        if (cur->isThreaded)
          cout << "true";
        else
          cout << "false";
        cur = cur->right;    // moving right here
        cout << " threaded to " << cur->key;
        cout << endl;
      }
      // last cur
      cout << "key: " << cur->key;
      cout << " value: " << cur->value;
      cout << " nL = " << cur->nL;
      cout << " nR = " << cur->nR;
      cout << " isThreaded = ";
      if (cur->isThreaded)
        cout << "true";
      else
        cout << "false";
      cur = cur->right;
      cout << endl;
    }
    
    //
    // bottomLeft: gives the data from the bottomLeft node
    //
    string bottomLeft() {
      NODE* cur = _bottomLeft(root);
      string pair = "Key: ";
      pair += to_string(cur->key);
      pair += " Value: ";
      pair += to_string(cur->value);
      
      return pair;
    }
};
