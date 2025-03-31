#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;
 

// You may add any prototypes of helper functions here

int helperF(Node* node){
  
  if(node == nullptr){ // if null node
    return 0;
  }
  if(node->right == nullptr && node->left == nullptr ){ //  if node is leaf
    return 1;
  }

  int depthL = (node->left ? helperF(node->left) : 0);
  int depthR = (node->right ? helperF(node->right) : 0);


  if(node->left && node->right){ // if both children exist
    if(depthR == -1 || depthL == -1 || depthL != depthR){
      return -1;
    }
    else{
      return depthL+1;
    }
  }
  else if(node->left){ //  if child exists
    if(depthL == -1){
      return -1;
    }
    return depthL+1;
  }
  else{
    if(depthR == -1){
      return -1;
    }
    return depthR+1;
  }
}


bool equalPaths(Node * root)
{
    // Add your code below
    if (root == nullptr){
      return true;
    }

    return (helperF(root) != -1);
}

