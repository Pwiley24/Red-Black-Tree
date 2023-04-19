#include <iostream>
#include "Node.h"

using namespace std;

Node::Node(){
  value = 0;
  right = NULL;
  left = NULL;
  parent = NULL;
  color = NULL;
}

Node::~Node(){
  right = NULL;
  left = NULL;
  parent = NULL;
}

//return the parent node
Node* Node::getParent(){
  return parent;
}

//set the parent node
void Node::setParent(Node* newParent){
  parent = newParent;
}

//return right node
Node* Node::getRight(){
  return right;
}

//return left node
Node* Node::getLeft(){
  return left;
}

//set right value
void Node::setRight(Node* newRight){
  right = newRight;
}

//set left value
void Node::setLeft(Node* newLeft){
  left = newLeft;
}

//set value of node
void Node::setValue(int newVal){
  value = newVal;
}

//get value of node
int Node::getValue(){
  return value;
}


//get the grandparent of the node (parent of parent)
Node* Node::getGrand(Node* parent){
  if(parent != NULL){//not the root
    if(parent->getParent() != NULL){//there is a grandparent
      return parent->getParent();
    }
  }
  return NULL;
}

//get the sibling of the node (left or right child of parent)
Node* Node::getSib(Node* parent){
  if(parent != NULL){//not root
    if(parent->getLeft() != NULL &&
       parent->getLeft()->getValue() != value){//return left sibling
      return parent->getLeft();
    }else{//return right sibling
      return parent->getRight();
    }
  }
  return NULL;
}

//get the uncle of the node (sibling of the parent or grandparents other child)
Node* Node::getUncle(Node* grand, Node* parent){
  if(grand != NULL){//not root
    if(grand->getLeft() != NULL &&
       grand->getLeft() != parent){//uncle is on the left
      return grand->getLeft();
    }else if(grand->getRight() != NULL &&
	     grand->getRight() != parent){//uncle on the right
      return grand->getRight();
    }
  }
  return NULL;
}

//returns the color of the node (black or red)
char Node::getColor(){
  return color;
}

//set the color of the node to black or red (b = black, r = red)
void Node::setColor(char colour){
  color = colour;
}
