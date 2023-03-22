#include <iostream>
#include <cstring>
#include "Node.h"

using namespace std;

void add_to_tree(Node* value, Node* current);

int main(){
  char input[20];
  Node* head = NULL;
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
	ifstream file(fileName);
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
	  add_to_tree(newNode, head);
	}
      }
    }
  }
  return 0;
}

//add number value to tree
void add_to_tree(Node* value, Node* current){
  if(current == NULL){//first value
    current->setValue(value);
  }else{
    if(current->getValue() > value->getValue()){ //value goes to left
      if(current->getLeft() == NULL){//value goes to immediate left
	current->setLeft(value);
      }else{ //keep going down left of head
	add_to_tree(value, current->getLeft());
      }
    }else if(current->getValue() <= value->getValue()){//value goes to right
      if(current->getRight() == NULL){//value goes to immediate right
	current->setRight(value);
      }else{//keep going down right of head
	add_to_tree(value, current->getRight());
      }
    }
      
    }
  }
}
