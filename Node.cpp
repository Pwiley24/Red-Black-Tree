#include <iostream>
#include "Node.h"

using namespace std;

Node::Node(){
  value = NULL;
  right = NULL;
  left = NULL;
}

Node::~Node(){
  delete &value;
  right = NULL;
  left = NULL;
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
