#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include "Node.h"

using namespace std;

void add_to_tree(Node* value, Node* current, Node* &head);
void display_tree(Node* current, int depth);
Node* search_tree(Node* current, int value);
void delete_from_tree(Node* position, int value, Node* parent);

int main(){
  char input[20];
  Node* head = NULL;
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
	  add_to_tree(newNode, head, head);
	}
      }else if(strcmp(input, "NUMBER") == 0){ //adding by number
	int number;
	cout << "What is the number you'd like to add?" << endl;
	cin >> number;
	cin.ignore(10, '\n');

	Node* newNode = new Node();
	newNode->setValue(number);
	add_to_tree(newNode, head, head);
      }
    }else if(strcmp(input, "DELETE") == 0){//delete a value in tree
      int number;
      cout << "What number would you like to delete?" << endl;
      cin >> number;
      cin.ignore(10, '\n');
      Node* position = search_tree(head, number);
      if(position != NULL){//value exists to delete
	delete_from_tree(position, number, position->getParent()); 
      }
      
    }else if(strcmp(input, "DISPLAY") == 0){//print the tree
      display_tree(head, 0);
      
    }else if(strcmp(input, "SEARCH") == 0){//search for a number in tree
      int number;
      cout << "Enter a number to search" << endl;
      cin >> number;
      cin.ignore(10, '\n');
      if(search_tree(head, number) != NULL){//if true say so!
	cout << "Number exists in tree :)" << endl;
      }else{
	cout << "Number doesn't exist in tree :(" << endl;
      }
    }
  }
  return 0;
}


//delete a number from the tree
//position is the node you are going to delete
//gathered from searching the tree for the node.
//parent = parent of position
//cases: 0 children, 1 child, 2 children
void delete_from_tree(Node* position, int value, Node* parent){
  //Case one:
  if(position->getLeft() == NULL &&
     position->getRight() == NULL){//zero children - delete child and set parent to left/right to null
       if(parent->getLeft() == position){//set parent left to null
	 parent->setLeft(NULL);
       }else{
	 parent->setRight(NULL);
       }
       delete position;

       //Case two:     
  }else if(position->getLeft() != NULL){//one child to left of position
    if(parent->getLeft() == position){//set parent left to position's left
	parent->setLeft(position->getLeft());
    }else{//set parent left to position's right
      parent->setLeft(position->getRight());
    }
    delete position;
  }else if(position->getRight() != NULL){//one child to right of position
    if(parent->getLeft() == position){//set parent right to position's left
      parent->setRight(position->getLeft());
    }else{//set parent right to position's right
      parent->setRight(position->getRight());
    }
    delete position;

    //Case three:
  }else if(position->getLeft() != NULL &&
	   position->getRight() != NULL){//two children - find farthest left position right's left
    if(parent->getLeft() == position){//

    }

  }
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
  cout << current->getValue() << endl;

  if(current->getLeft() != NULL){//still value to the right
    display_tree(current->getLeft(), (depth+1));
  }
}




//add number value to tree
void add_to_tree(Node* value, Node* current, Node* &head){
  if(head == NULL){//first value
    head = value;
  }else{//not first
    if(current->getValue() > value->getValue()){ //value goes to left
      if(current->getLeft() == NULL){//value goes to immediate left
	current->setLeft(value);
	value->setParent(current);
      }else{ //keep going down left of head
	add_to_tree(value, current->getLeft(), head);
      }
    }else if(current->getValue() <= value->getValue()){//value goes to right
      if(current->getRight() == NULL){//value goes to immediate right
	current->setRight(value);
	value->setParent(current);
      }else{//keep going down right of head
	add_to_tree(value, current->getRight(), head);
      }
    }
  }
}
