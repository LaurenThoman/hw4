#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertLeft(const std::pair<const Key, Value>& new_item, AVLNode<Key, Value>* parent);
    void insertRight(const std::pair<const Key, Value>& new_item, AVLNode<Key, Value>* parent);
    void insertFix(AVLNode<Key, Value>* node, AVLNode<Key, Value>* parent);

    void removeFixLeft(AVLNode<Key, Value>* node, AVLNode<Key, Value>* parent, int ndiff);
    void removeFixRight(AVLNode<Key, Value>* node, AVLNode<Key, Value>* parent, int ndiff);
    void removeFix(AVLNode<Key, Value>* node, int diff);

    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);

    void fixLeftSubtree(AVLNode<Key, Value>* grand, AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
    void fixRightSubtree(AVLNode<Key, Value>* grand, AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
    void fixLeftRightCase(AVLNode<Key, Value>* grand, AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
    void fixRightLeftCase(AVLNode<Key, Value>* grand, AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
  if (this->root_ == nullptr) {
    AVLNode<Key, Value>* nRoot = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);

    nRoot->setBalance(0);
    nRoot->setLeft(nullptr);
    nRoot->setRight(nullptr);
    this->root_ = nRoot;
    return;
  }

  AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
  AVLNode<Key, Value>* parent = nullptr;

  while (curr != nullptr) {
    parent = curr;
    if (new_item.first < curr->getKey()){
      curr = curr->getLeft();
    }
    else if (new_item.first > curr->getKey()){
      curr = curr->getRight();
    }
    else {
      curr->setValue(new_item.second);
      return;
    }
  }

  AVLNode<Key, Value>* newN = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
  newN->setBalance(0);

  if (new_item.first < parent->getKey()) {
    parent->setLeft(newN);

    if (parent->getBalance() == 0) {
      parent->updateBalance(-1);
      insertFix(newN, parent);
    } 
    else {
      parent->setBalance(0);
    }

  } 
  else {
    parent->setRight(newN);

    if (parent->getBalance() == 0) {
      parent->updateBalance(1);
      insertFix(newN, parent);
    } 
    else {
      parent->setBalance(0);
    }
  }

}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
  AVLNode<Key, Value>* target = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));

  if (target == nullptr){
    return;
  }
  if (target->getLeft() != nullptr && target->getRight() != nullptr) {
    AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(target));
    nodeSwap(target, pred);
  }

  AVLNode<Key, Value>* child = (target->getRight() != nullptr) ? target->getRight() : target->getLeft();

  if (child != nullptr){
    child->setParent(target->getParent());
  }

  AVLNode<Key, Value>* parent = target->getParent();
  int diff = 0;

  if (parent == nullptr) {
    this->root_ = child;
  } 
  else { 
    if (target == parent->getLeft()) {
      parent->setLeft(child);
      diff = 1;
    } 
    else {
      parent->setRight(child);
      diff = -1;
    }
  }

  delete target;
  removeFix(parent, diff);
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

/*
*  method insertLeft()
*/
template <class Key, class Value>
void AVLTree<Key, Value>::insertLeft(const std::pair<const Key, Value>& new_item, AVLNode<Key, Value>* parent){

  AVLNode<Key, Value>* newN = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
  parent->setRight(newN);
  newN->setParent(parent);
  newN->setBalance(0);

  if (parent->getBalance() == 0) {
    parent->updateBalance(-1);
    insertFix(newN, parent);
  } 
  else {
    parent->setBalance(0);
  }

}


/*
*  method insertRight()
*/
template <class Key, class Value>
void AVLTree<Key, Value>::insertRight(const std::pair<const Key, Value>& new_item, AVLNode<Key, Value>* parent){

  AVLNode<Key, Value>* newN = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
  parent->setRight(newN);
  newN->setParent(parent);
  newN->setBalance(0);

  if (parent->getBalance() == 0) {
    parent->updateBalance(1);
    insertFix(newN, parent);
  } 
  else {
    parent->setBalance(0);
  }

}



/*
*  method insertFix()
*/
template <class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* node, AVLNode<Key, Value>* parent) {
  if (parent == nullptr || parent->getParent() == nullptr){
    return;
  }

  AVLNode<Key, Value>* grandP = parent->getParent();

  if(grandP->getLeft() == parent){
    fixLeftSubtree(grandP, parent, node);
  }
  else if(grandP->getRight() == parent){
    fixRightSubtree(grandP, parent, node);
  }
}


/*
*  method removeFixLeft()
*/
template <class Key, class Value>
void AVLTree<Key, Value>::removeFixLeft(AVLNode<Key, Value>* node, AVLNode<Key, Value>* parent, int ndiff){
  if(node->getBalance() -1 == -2){
    AVLNode<Key, Value>* leftChild = node->getLeft();

    if(leftChild->getBalance() == -1){

      rotateRight(node);
      node->setBalance(0);
      leftChild->setBalance(0);
      removeFix(parent, ndiff);
    }
    else if(leftChild->getBalance() == 0){
      rotateRight(node);
      node->setBalance(-1);
      leftChild->setBalance(1);
      return;
    }
    else if(leftChild->getBalance() == 1){
      AVLNode<Key, Value>* temp = leftChild->getRight();

      rotateLeft(leftChild);
      rotateRight(node);

      if(temp->getBalance() == 1){
        node->setBalance(0);
        leftChild->setBalance(-1);
      }
      else if(temp->getBalance() == 0){
        node->setBalance(0);
        leftChild->setBalance(0);
      }
      else if(temp->getBalance() == -1){
        node->setBalance(1);
        leftChild->setBalance(0);
      }

      temp->setBalance(0);
      removeFix(parent, ndiff);
    }


  }
  else if(node->getBalance() - 1 == -1){
    node->setBalance(-1);
    return;
  }
  else if(node->getBalance() - 1 == 0){
    node->setBalance(0);
    removeFix(parent, ndiff);
  }
  
}


/*
*  method removeFixRight()
*/
template <class Key, class Value>
void AVLTree<Key, Value>::removeFixRight(AVLNode<Key, Value>* node, AVLNode<Key, Value>* parent, int ndiff){
  if(node->getBalance() + 1 == 2){
    AVLNode<Key, Value>* rightChild = node->getRight();

    if(rightChild->getBalance() == 1){

      rotateLeft(node);
      node->setBalance(0);
      rightChild->setBalance(0);
      removeFix(parent, ndiff);
    }
    else if(rightChild->getBalance() == 0){
      rotateLeft(node);
      node->setBalance(1);
      rightChild->setBalance(-1);
      return;
    }
    else if(rightChild->getBalance() == -1){
      AVLNode<Key, Value>* temp = rightChild->getLeft();

      rotateRight(rightChild);
      rotateLeft(node);

      if(temp->getBalance() == -1){
        node->setBalance(0);
        rightChild->setBalance(1);
      }
      else if(temp->getBalance() == 0){
        node->setBalance(0);
        rightChild->setBalance(0);
      }
      else if(temp->getBalance() == 1){
        node->setBalance(-1);
        rightChild->setBalance(0);
      }

      temp->setBalance(0);
      removeFix(parent, ndiff);
    }


  }
  else if(node->getBalance() + 1 == 1){
    node->setBalance(1);
    return;
  }
  else if(node->getBalance() +1 == 0){
    node->setBalance(0);
    removeFix(parent, ndiff);
  }


}

/*
*  method removeFix()
*/
template <class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int diff){
  if(node == nullptr){
    return;
  }

  AVLNode<Key, Value>* par = node->getParent();
  int ndiff = -1;

  if(par != nullptr && node == par->getLeft()){
    ndiff = 1;
  }
  if(diff == -1){
    removeFixLeft(node, par, ndiff);
  }
  else if(diff == 1){
    removeFixRight(node, par, ndiff);
  }

}


/*
*  method rotateLeft()
*/
template <class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node) {
    
  AVLNode<Key, Value>* newN = node->getRight();
  AVLNode<Key, Value>* temp = newN->getLeft();
  AVLNode<Key, Value>* par = node->getParent();
  
  newN->setParent(par);
  
  if (par == NULL) {
    this->root_ = newN;
  } 
  else if (par->getLeft() == node) {
    par->setLeft(newN);
  } 
  else if (par->getRight() == node) {
    par->setRight(newN);
  }

  node->setParent(newN);
  node->setRight(temp);
  newN->setLeft(node);

  if (temp != NULL) {
    temp->setParent(node);
  }
}


/*
*  method rotateRight()
*/
template <class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node) {
  
  AVLNode<Key, Value>* newN = node->getLeft();
  AVLNode<Key, Value>* temp = newN->getRight();
  AVLNode<Key, Value>* par = node->getParent();
  
  newN->setParent(par);
  
  if (par == NULL){
    this->root_ = newN;
  } 
  else if (par->getLeft() == node){
    par->setLeft(newN);
  } 
  else if (par->getRight() == node){
    par->setRight(newN);
  }

  node->setParent(newN);
  node->setLeft(temp);
  newN->setRight(node);

  if (temp != NULL) {
    temp->setParent(node);
  }
}

/*
*  method fixLeftSubtree()
*/
template <class Key, class Value>
void AVLTree<Key, Value>::fixLeftSubtree(AVLNode<Key, Value>* grand, AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node){
  
  grand->updateBalance(-1);

  if(grand->getBalance() == 0){
    return;
  }
  else if(grand->getBalance() == -1){
    insertFix(parent, grand);
  }
  else if(grand->getBalance() == -2){
    if (node == parent->getLeft()){
      rotateRight(grand);
      parent->setBalance(0);
      grand->setBalance(0);
    }
    else if(node == parent->getRight()){
      fixLeftRightCase(grand, parent, node);
    }
  }

}

/*
*  method fixRightSubtree()
*/
template <class Key, class Value>
void AVLTree<Key, Value>::fixRightSubtree(AVLNode<Key, Value>* grand, AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node){
  
  grand->updateBalance(1);

  if(grand->getBalance() == 0){
    return;
  }
  else if(grand->getBalance() == 1){
    insertFix(parent, grand);
  }
  else if(grand->getBalance() == 2){
    if (node == parent->getRight()){
      rotateLeft(grand);
      parent->setBalance(0);
      grand->setBalance(0);
    }
    else if(node == parent->getLeft()){
      fixRightLeftCase(grand, parent, node);
    }
  }

}

/*
*  method fixLeftRightCase()
*/
template <class Key, class Value>
void AVLTree<Key, Value>::fixLeftRightCase(AVLNode<Key, Value>* grand, AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node){

  rotateLeft(parent);
  rotateRight(grand);

  int bal = node->getBalance();

  if (bal == -1) {
    parent->setBalance(0);
    grand->setBalance(1);
  }
  else if(bal == 0){
    parent->setBalance(0);
    grand->setBalance(0);
  }
  else if(bal == 1){ 
    parent->setBalance(-1);
    grand->setBalance(0);
  }
  node->setBalance(0);

}

/*
*  method fixRightLeftCase()
*/
template <class Key, class Value>
void AVLTree<Key, Value>::fixRightLeftCase(AVLNode<Key, Value>* grand, AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node){
  
  rotateRight(parent);
  rotateLeft(grand);

  int bal = node->getBalance();

  if (bal == 1) {
    parent->setBalance(0);
    grand->setBalance(-1);
  }
  else if(bal == 0){
    parent->setBalance(0);
    grand->setBalance(0);
  }
  else if(bal == -1){ 
    parent->setBalance(1);
    grand->setBalance(0);
  }
  node->setBalance(0);

}


#endif