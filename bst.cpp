/*bstt.h*/

//
// Threaded binary search tree
//

#pragma once

#include <iostream>
#include <vector>
using namespace std;

template<typename KeyT, typename ValueT>
class bstt
{
private:
  struct NODE
  {
    KeyT   Key;
    ValueT Value;
    NODE*  Left;
    NODE*  Right;
    bool   isThreaded;
  };
  NODE* inOrderTree; // pointer to tree that is inorder
  NODE* Root;  // pointer to root node of tree (nullptr if empty)
  int   Size;  // # of nodes in the tree (0 if empty)

  /* Post order traversal is necessary to delete the nodes as it is accesses the "deepest" nodes first, which are the ones that should be deleted first/ */
  void _postorder(NODE* cur){
	 if(cur == nullptr){
		 return;
	 }
	 else{
		 _postorder(cur->Left);
		 if(cur->isThreaded == false){
		 _postorder(cur->Right);
		 }
		 delete cur;
	 }
  }	
  // In order traversal is necessary for the dump function
  void _inorder(NODE* cur, ostream& output) const
  {
      if(cur == nullptr){
          return;
      }
      else{
          _inorder(cur->Left, output);
          if(cur->isThreaded && cur->Right != nullptr){
              output << "(" << cur->Key << "," << cur->Value << ",";
              cur = cur->Right;
              output << cur->Key << ")" <<endl;
              cur = nullptr;
           }
           else{
              output << "(" << cur->Key << "," << cur->Value << ")" <<endl;
			  _inorder(cur->Right, output);
           }
      }
   }
   // preorder traverseal is necessary for creating nodes in copy and =
  void _preorder(NODE* cur){
	   if(cur == nullptr){
		   return;
	   }
	   else{
		   insert(cur->Key, cur->Value);
		  _preorder(cur->Left);
		  if(cur->isThreaded == false){
			  _preorder(cur->Right);
		  }
	  }
  }
  
  
  // This is the Search function, but has added functionality in that it returns where the tree "fell out" if it did
  bool searchAdvanced(KeyT key, ValueT& value, NODE*& prev) const
  {
    NODE* cur = Root; //we do not want root to be null after search is ran, so we make a new NODE equal to Root that will be NULL when we're done
	prev = nullptr;
	while(cur != NULL){
		
		// if the key is found, we set value equal to the key's value and return true;
		if(cur->Key == key){
			value = cur->Value;
			prev = cur;
			return true;
		}
		// if the key we're looking for is less than the current key, we go to the left
		else if(cur->Key > key){
			prev = cur;
			cur = cur->Left;
		}
		// we check if the key we're looking for is is greater than the current key AND 
		// that the current Node isn't threaded. If so, we go right and set prev equal to cur
		else if(cur->isThreaded == false && cur->Key < key){
			prev = cur;
			cur = cur->Right;
		}
		// If we get here, the the key we're looking for is greater than the current key, but the Node 
		// is threaded, which means our node couldn't be found. Since this is the case, we set cur to be
		// null, which will let us exit the while loop. We also set prev equal to cur
		else if(cur->isThreaded ==true){
			prev = cur;
			cur = NULL;
		}
	}
	// Once cur is equal to null, we have traveresed the entire tree and not found the value. Return false
		return false;
  }
public:
  //
  // default constructor:
  //
  // Creates an empty tree.
  //
  bstt()
  {
    Root = nullptr;
	inOrderTree = nullptr;
    Size = 0;
  }

  //
  // copy constructor
  //
  bstt(const bstt& other)
  {
	 Size = 0;
	 Root = nullptr;
	_preorder(other.Root);
  }

	//
  // destructor:
  //
  // Called automatically by system when tree is about to be destroyed;
  // this is our last chance to free any resources / memory used by
  // this tree.
  //
  virtual ~bstt()
  {
    if(Size>0){
	_postorder(Root);
	}
	Size = 0;
	Root = nullptr;
  }
  
  //
  // operator=
  //
  // Clears "this" tree and then makes a copy of the "other" tree.
  //
  bstt& operator=(const bstt& other)
  {
	clear();
	_preorder(other.Root);
	return *this;
  }

  //
  // clear:
  //
  // Clears the contents of the tree, resetting the tree to empty.
  //
  void clear()
  {
	if(Size>0){
    _postorder(Root);
	}
	Size = 0;
	Root = nullptr;
  }

  // 
  // size:
  //
  // Returns the # of nodes in the tree, 0 if empty.
  //
  // Time complexity:  O(1) 
  //
  int size() const
  {
    return Size;
  }

  // 
  // search:
  //
  // Searches the tree for the given key, returning true if found
  // and false if not.  If the key is found, the corresponding value
  // is returned via the reference parameter.
  //
  // Time complexity:  O(lgN) on average
  //
  bool search(KeyT key, ValueT& value) const
  {
    NODE* cur = Root; //we do not want root to be null after search is ran, so we make a new NODE equal to Root that will be NULL when we're done
	while(cur != NULL){
		
		// the key is found, we set value equal to the key's value and return true;
		if(cur->Key == key){
			value = cur->Value;
			return true;
		}
		// if the key we're looking for is less than the current key, we go to the left
		else if(cur->Key > key){
			cur = cur->Left;
		}
		// we check if the key we're looking for is is greater than the current key AND that the current Node isn't threaded. 
		// If so, we go right
		else if(cur->isThreaded == false && cur->Key < key){
			cur = cur->Right;
		}
		// If we get here, the current Node the key we're looking for is greater than the current key, 
		// but the Node is threaded, which means our node couldn't be found
		// Since this is the case, we set cur to become null, thus exiting the while loop
		else if(cur->isThreaded ==true){
			cur = NULL;
		}
	}
	// Once cur is equal to null, we have traveresed the entire tree and not found the value. Return false
		return false;
  }

  //
  // insert
  //
  // Inserts the given key into the tree; if the key has already been insert then
  // the function returns without changing the tree.
  //
  // Time complexity:  O(lgN) on average
  //
  void insert(KeyT key, ValueT value)
  {
	NODE* fellOutHere = new NODE();
	// this search function does two things. 1 it checks to see if the key is already in the tree and exits out the function if it is. 2 it gets the
	// final non-nullptr node that was accessed by search before it went to NULL 
	if(searchAdvanced(key, value, fellOutHere) == true){
		return;
	}
	
	// This creates the basis of the new Node to be inserted
	NODE* insertNode = new NODE();
	insertNode->Key = key;
	insertNode->Value = value;
	insertNode->Left = nullptr;
	insertNode->Right = nullptr;
	insertNode->isThreaded = true;
	
	// If fellOutHere is null, the existing tree must be null, so set Root equal to the node to be inserted
	if(fellOutHere == NULL){
		Root = insertNode;
	}
	
	// If fellOutHere node's key is greater than the the key we're looking for, we put the new Node to its left 
	// and we make the the new Node thread to it
	else if(fellOutHere->Key > key){
		fellOutHere->Left = insertNode;
		insertNode->Right = fellOutHere;
	}
	// If the fellOutHere node's key is less than the key we're looking for, we put the new Node to 
	// it's right, have the new Node thread to whatever fellOutHere was threaded to and finally set 
	// fellOutHere to no longer be threaded (the new node will have taken over the thing it threaded to
	else if(fellOutHere->Key < key){
		insertNode->Right = fellOutHere->Right;
		fellOutHere->isThreaded = false;
		fellOutHere->Right = insertNode;
	}
	Size++;
  }

  //
  // []
  //
  // Returns the value for the given key; if the key is not found,
  // the default value ValueT{} is returned.
  //
  // Time complexity:  O(lgN) on average
  //
  ValueT operator[](KeyT key) const
  {
    ValueT value = ValueT{ };
	if(search(key,value) == true){
		return value;
	}
    return ValueT{ };
  }

  //
  // ()
  //
  // Finds the key in the tree, and returns the key to the "right".
  // If the right is threaded, this will be the next inorder key.
  // if the right is not threaded, it will be the key of whatever
  // node is immediately to the right.
  //
  // If no such key exists, or there is no key to the "right", the
  // default key value KeyT{} is returned.
  //
  // Time complexity:  O(lgN) on average
  //
  KeyT operator()(KeyT key) const
  {
	  NODE* hasKey = new NODE();
	  ValueT valOfKey = ValueT{ };
	  if(searchAdvanced(key,valOfKey,hasKey) == true){
		  if(hasKey->Right != nullptr){
			  return hasKey->Right->Key;
		  }
	  }
	  

    return KeyT{ };
  }

  //
  // begin
  //
  // Resets internal state for an inorder traversal.  After the 
  // call to begin(), the internal state denotes the first inorder
  // key; this ensure that first call to next() function returns
  // the first inorder key.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) on average
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
  void begin()
  {
	 NODE* cur = Root;
	 NODE* prev = nullptr;
	 while(cur!= NULL){
		 prev =cur;
		 cur=cur->Left;
	 }
	 inOrderTree = prev;
  }
	
  //
  // next
  //
  // Uses the internal state to return the next inorder key, and 
  // then advances the internal state in anticipation of future
  // calls.  If a key is in fact returned (via the reference 
  // parameter), true is also returned.
  //
  // False is returned when the internal state has reached null,
  // meaning no more keys are available.  This is the end of the
  // inorder traversal.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) on average
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
  bool next(KeyT& key)
  {
    
	if(inOrderTree == NULL){
		return false;
	}
	key = inOrderTree->Key;
	
		if(inOrderTree->isThreaded == true){
			inOrderTree = inOrderTree->Right;
			return true;
		}
		else if(inOrderTree->isThreaded == false){
			if(inOrderTree->Right != nullptr){
				inOrderTree = inOrderTree->Right;
			}
			while(inOrderTree->Left != nullptr){
				inOrderTree = inOrderTree->Left;
			}
			
			return true;
		}
		return true;
  }

  //
  // dump
  // 
  // Dumps the contents of the tree to the output stream, using a
  // recursive inorder traversal.
  //
  void dump(ostream& output) const
  {
	  NODE* cur = Root;
    output << "**************************************************" << endl;
    output << "********************* BSTT ***********************" << endl;

    output << "** size: " << this->size() << endl;
	_inorder(cur, output);
    //
    // inorder traversal, with one output per line: either 
    // (key,value) or (key,value,THREAD)
    
    // (key,value) if the node is not threaded OR thread==nullptr
    // (key,value,THREAD) if the node is threaded and THREAD denotes the next inorder key
   

    //
    // TODO
    //

    output << "**************************************************" << endl;
  }
	
};

