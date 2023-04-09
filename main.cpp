#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include "Node.h"

using namespace std;

void add_to_tree(Node* value, Node* current, Node* &head);
void display_tree(Node* current, int depth);
Node* search_tree(Node* current, int value);
void delete_from_tree(Node* position, Node* parent, Node* &head);
Node* find_left_or_right(Node* current, char direction);

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
	delete_from_tree(position, position->getParent(), head); 
      }else{
        cout << "Number doesn't exist in tree." << endl;
      }
      
    }else if(strcmp(input, "DISPLAY") == 0){//print the tree
      if(head != NULL){
        display_tree(head, 0);
      }else{
        cout << "Nothing to display" << endl;
      }
      
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
//Note: parent can be null -> root/head of tree is being deleted
//cases: 0 children, 1 child, 2 children
void delete_from_tree(Node* position, Node* parent, Node* &head){
  //Case one:
  if(position->getLeft() == NULL &&
     position->getRight() == NULL){//zero children - delete child and set parent to left/right to null
    if(parent == NULL){//deleting the root/head with no children - head is null now
      head = NULL; 
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
      if(parent == NULL){//deleting head and replacing with left child
	position->getLeft()->setParent(NULL);
        head = position->getLeft();
      }else{//not deleting head
        if(parent->getLeft() == position){
	  parent->setLeft(position->getLeft());
        }else{
  	  parent->setRight(position->getLeft());
        }
        position->getLeft()->setParent(parent);
      }
    }else{//setting right of position
      if(parent == NULL){//deleting head and replacing with right child
        position->getRight()->setParent(NULL);
        head = position->getRight();
      }else{//not deleting head
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
    delete_from_tree(leftMost, leftMost->getParent(), head);
    
    
  }


    /*Node* posRMost = find_left_or_right(position, 'R'); //finds right most value of position
    posRMost->setParent(parent);//set right most parent to position's parent
    Node* mostLeft = find_left_or_right(posRMost, 'L');//finds leftest value of right most
    if(parent->getLeft() == position){//dealing with the left side of tree
      parent->setLeft(posRMost); //set parent to right most of position
      if(mostLeft != NULL){//there is a left node - set position's other child to this node 
	position->getLeft()->setParent(mostLeft);
	mostLeft->setLeft(position->getLeft());
      }else{ //no left on right most node - set position's other child to right most
	position->getLeft()->setParent(posRMost);
	posRMost->setLeft(position->getLeft());
      }
    }else{//dealing with the right side of tree
      parent->setRight(posRMost);//set parent to left most of position
      cout << parent->getRight()->getValue() << endl;
      if(mostLeft != NULL){//there is a right node - set position's other child to this node
	position->getRight()->setParent(mostLeft);
	mostLeft->setLeft(position->getRight());
      }else{//no right node on left most node - set position's other child to left most
	position->getRight()->setParent(posRMost);
	posRMost->setLeft(position->getLeft());
	}*/
    

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
