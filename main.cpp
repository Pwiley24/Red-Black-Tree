//This program creates a binary tree of nodes that stores integer values.
//you can search to see if a number exists in the tree, add numbers with a file or by number,
//delete values from the tree, and display the tree in the terminal.
//author: Paige Wiley
//date: 04-09-2023


#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include "Node.h"

using namespace std;

void adjustNode(Node* current, Node* &root);
void add_to_tree(Node* value, Node* current, Node* &root);
void display_tree(Node* current, int depth);
Node* search_tree(Node* current, int value);
void delete_from_tree(Node* position, Node* parent, Node* &root);
Node* find_left_or_right(Node* current, char direction);

int main(){
  char input[20];
  Node* root = NULL;
  bool running = true;

  while(running){
    cout << "Enter a command (ADD, DELETE, SEARCH, DISPLAY, QUIT):" << endl;
    cin.get(input, 20);
    cin.ignore(20, '\n');

    if(strcmp(input, "ADD") == 0){//adding values
      cout << "Add by FILE or NUMBER?" << endl;
      cin.get(input, 20);
      cin.ignore(20, '\n');

      if(strcmp(input, "FILE") == 0){//adding by file
	char fileName[50];
	cout << "What is the name of the file?" << endl;
	cin.get(fileName, 50);
	cin.ignore(50, '\n');

	vector<int> fileNums;
	ifstream file("number.txt");
	if(file.is_open()){
	  int a;
	  while(file >> a){
	    fileNums.push_back(a);
	  }
	  file.close();
	}else{
	  cout << "File could not be opened!" << endl;
	}

	vector<int>::iterator ptr;
	for(ptr = fileNums.begin(); ptr < fileNums.end(); ptr++){//add all values
	  Node* newNode = new Node();
	  newNode->setValue(*ptr);
	  add_to_tree(newNode, root, root);
	  //the node has been established. Adjust nodes to match red tree black tree
	  adjustNode(newNode, root);
	}
      }else if(strcmp(input, "NUMBER") == 0){ //adding by number
	int number;
	cout << "What is the number you'd like to add?" << endl;
	cin >> number;
	cin.ignore(10, '\n');

	Node* newNode = new Node();
	newNode->setValue(number);
	add_to_tree(newNode, root, root);
	//the node has been established. Adjust nodes to match red tree black tree
	adjustNode(newNode, root);
      }
    }else if(strcmp(input, "DELETE") == 0){//delete a value in tree
      int number;
      cout << "What number would you like to delete?" << endl;
      cin >> number;
      cin.ignore(10, '\n');
      Node* position = search_tree(root, number);
      if(position != NULL){//value exists to delete
	delete_from_tree(position, position->getParent(), root); 
      }else{
        cout << "Number doesn't exist in tree." << endl;
      }
      
    }else if(strcmp(input, "DISPLAY") == 0){//print the tree
      if(root != NULL){
        display_tree(root, 0);
      }else{
        cout << "Nothing to display" << endl;
      }
      
    }else if(strcmp(input, "SEARCH") == 0){//search for a number in tree
      int number;
      cout << "Enter a number to search" << endl;
      cin >> number;
      cin.ignore(10, '\n');
      if(search_tree(root, number) != NULL){//if true say so!
	cout << "Number exists in tree :)" << endl;
      }else{
	cout << "Number doesn't exist in tree :(" << endl;
      }
    }else if(strcmp(input, "QUIT") == 0){//quit program
      running = false;
    }
  }
  return 0;
}


//delete a number from the tree
//position is the node you are going to delete
//gathered from searching the tree for the node.
//parent = parent of position
//Note: parent can be null -> root of tree is being deleted
//cases: 0 children, 1 child, 2 children
void delete_from_tree(Node* position, Node* parent, Node* &root){
  //Case one:
  if(position->getLeft() == NULL &&
     position->getRight() == NULL){//zero children - delete child and set parent to left/right to null
    if(parent == NULL){//deleting the root with no children - head is null now
      root = NULL; 
    }else if(parent->getLeft() == position){//set parent left to null
      parent->setLeft(NULL);
    }else{
      parent->setRight(NULL);
    }
    delete position;


       //Case two:
   }else if((position->getLeft() != NULL &&
	   position->getRight() == NULL) ||
	   (position->getLeft() == NULL &&
	    position->getRight() != NULL)){//one child to left/right of position



    if(position->getLeft() != NULL){//set left value of position to new position and delete
      if(parent == NULL){//deleting root and replacing with left child
	position->getLeft()->setParent(NULL);
        root = position->getLeft();
      }else{//not deleting root
        if(parent->getLeft() == position){
	  parent->setLeft(position->getLeft());
        }else{
  	  parent->setRight(position->getLeft());
        }
        position->getLeft()->setParent(parent);
      }
    }else{//setting right of position
      if(parent == NULL){//deleting root and replacing with right child
        position->getRight()->setParent(NULL);
        root = position->getRight();
      }else{//not deleting root
        if(parent->getLeft() == position){
	  parent->setLeft(position->getRight());
        }else{
	  parent->setRight(position->getRight());
        }
        position->getRight()->setParent(parent);
      }
    }
    delete position;

    
    //Case three:
  }else if(position->getLeft() != NULL &&
	   position->getRight() != NULL){//two children - find farthest left position right's left

    //STORE RIGHT ONE
    Node* rightOne = position->getRight();

    //FIND LEFT MOST
    Node* leftMost = find_left_or_right(rightOne, 'L');

    position->setValue(leftMost->getValue());
    delete_from_tree(leftMost, leftMost->getParent(), root);
    
    
  }
}


//find the left most or right most node from a starting position
Node* find_left_or_right(Node* current, char direction){
  if(direction == 'L'){//find left most
    if(current->getLeft() != NULL){//more to the left - recursion
      return find_left_or_right(current->getLeft(), 'L');
    }else{//return current
      return current;
    }
  }else if(direction == 'R'){//find right most
    if(current->getRight() != NULL){
      return find_left_or_right(current->getRight(), 'R');
    }else{//return current
      return current;
    }
  }
  return NULL;
}

//searches the tree by binary searching
Node* search_tree(Node* current, int value){
  if(current->getValue() == value){//value matches the head value
    return current;
  }else{ //continue search
    if(current->getValue() <= value &&
       current->getRight() != NULL){//the value must be to the right
      return search_tree(current->getRight(), value);
    }else if(current->getValue() > value &&
	     current->getLeft() != NULL){//value must be to the left
      return search_tree(current->getLeft(), value);
    }
  }
  return NULL;
}

//prints numbers from the tree in order
void display_tree(Node* current, int depth){
  if(current->getRight() != NULL){ //still a value to the left
    display_tree(current->getRight(), (depth+1));
  }
  for(int i = 0; i < depth; i++){
    cout << '\t';
  }
  
  cout << current->getColor() << current->getValue() << endl;

  if(current->getLeft() != NULL){//still value to the right
    display_tree(current->getLeft(), (depth+1));
  }
}




//add number value to tree
void add_to_tree(Node* value, Node* current, Node* &root){
  if(root == NULL){//first value
    root = value;
    root->setColor('b');
  }else{//not first
    if(current->getValue() > value->getValue()){ //value goes to left
      if(current->getLeft() == NULL){//value goes to immediate left
	current->setLeft(value);
	value->setParent(current);
	value->setColor('r');
      }else{ //keep going down left of head
	add_to_tree(value, current->getLeft(), root);
      }
    }else if(current->getValue() <= value->getValue()){//value goes to right
      if(current->getRight() == NULL){//value goes to immediate right
	current->setRight(value);
	value->setParent(current);
	value->setColor('r');
      }else{//keep going down right of head
	add_to_tree(value, current->getRight(), root);
      }
    }
  }
}

//readjusts the nodes in the tree to follow the R-B rules
//no two reds in a row
//case 1: new node is root/head
//case 2: parent node is red and root
//case 3: parent and uncle nodes are red
//case 4: parent node is red, uncle node is black, inserted node is "left grandchild"
//case 4: parent node is red, uncle node is black, inserted node is "right grandchild"
void adjustNode(Node* current, Node* &root){
  if(current != root){//not the root -> cases 2-4
    if(current->getParent()->getColor() == 'r'){//check for two reds in a row
      //readjust!
      cout << "readjusting..." << endl;
      
    }
  }else{//case 1: make sure root is black
    
  }

}
