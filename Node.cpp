#include <iostream>
#include "Node.h"

using namespace std;

Node::Node(){
  value = 0;
  right = NULL;
  left = NULL;
  parent = NULL;
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
