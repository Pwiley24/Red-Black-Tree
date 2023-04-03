#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include "Node.h"

using namespace std;

void add_to_tree(Node* value, Node* current, Node* &head);
void display_tree(Node* current, int depth);

int main(){
  char input[20];
  Node* head = new Node();
  bool running = true;

  while(running){
    cout << "Enter a command (ADD, DELETE, QUIT):" << endl;
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

      //delete_from_tree(number); //FINISH HERE
      
    }else if(strcmp(input, "DISPLAY") == 0){//print the tree
      display_tree(head, 0);
      
    }else if(strcmp(input, "SEARCH") == 0){//search for a number in tree

    }
  }
  return 0;
}


//prints numbers from the tree in order
void display_tree(Node* current, int depth){
  if(current->getLeft() != NULL){ //still a value to the left
    display_tree(current->getLeft(), (depth+1));
  }
  for(int i = 0; i < depth; i++){
    cout << '\t';
  }
  cout << current->getValue() << endl;

  if(current->getRight() != NULL){//still value to the right
    display_tree(current->getRight(), (depth+1));
  }
}




//add number value to tree
void add_to_tree(Node* value, Node* current, Node* &head){
  if(head == NULL){//first value
    current->setValue(value->getValue());
    head = current;
  }else{//not first 
    if(current->getValue() > value->getValue()){ //value goes to left
      if(current->getLeft() == NULL){//value goes to immediate left
	current->setLeft(value);
      }else{ //keep going down left of head
	add_to_tree(value, current->getLeft(), head);
      }
    }else if(current->getValue() <= value->getValue()){//value goes to right
      if(current->getRight() == NULL){//value goes to immediate right
	current->setRight(value);
      }else{//keep going down right of head
	add_to_tree(value, current->getRight(), head);
      }
    }
  }
}
