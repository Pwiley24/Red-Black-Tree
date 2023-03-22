#ifndef NODE_H
#define NODE_H
#include <iostream>

using namespace std;

class Node {
 public:
  Node();
  ~Node();
  void setValue(int newVal);
  int getValue();
  Node* getRight();
  Node* getLeft();
  void setRight(Node* newRight);
  void setLeft(Node* newLeft);

  int value;
  Node* previous;
  Node* left;
  Node* right;

};

#endif
