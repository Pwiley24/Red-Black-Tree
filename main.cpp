//This program creates a red and black binary tree of nodes that stores integer values.
//you can search to see if a number exists in the tree, add numbers with a file or by number,
//delete values from the tree, and display the tree in the terminal (delete feature coming soon.
//author: Paige Wiley
//date: 04-30-2023


#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include "Node.h"

using namespace std;

void adjustNode(Node* current, Node* &root, char side);
void add_to_tree(Node* value, Node* current, Node* &root, char &side, int count);
void display_tree(Node* current, int depth);
Node* search_tree(Node* current, int value);
void delete_from_tree(Node* position, Node* parent, Node* &root);
Node* find_left_or_right(Node* current, char direction);
void rightRotate(Node* top, Node* bottom, Node* &root);
void leftRotate(Node* top, Node* bottom, Node* &root);
void case_1(Node* replaced, Node* &root);
void case_2(Node* replaced, Node* sibling, char sibSide, Node* &root);
void case_3(Node* sibling, Node* &root);
void case_4(Node* parent, Node* sibling);
void case_5(Node* sibling, char sibSide, Node* &root);
void case_6(Node* sibling, Node* parent, char sibSide, Node* &root);


int main(){
  char input[20];
  Node* root = NULL;
  bool running = true;
  char side;

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
	  add_to_tree(newNode, root, root, side, 0);
	  //the node has been established. Adjust nodes to match red tree black tree
	  adjustNode(newNode, root, side);
	}
      }else if(strcmp(input, "NUMBER") == 0){ //adding by number
	int number;
	cout << "What is the number you'd like to add?" << endl;
	cin >> number;
	cin.ignore(10, '\n');

	Node* newNode = new Node();
	newNode->setValue(number);
	add_to_tree(newNode, root, root, side, 0);
	//the node has been established. Adjust nodes to match red tree black tree
	adjustNode(newNode, root, side);
      }
    }else if(strcmp(input, "DELETE") == 0){//delete a value in tree
      int number;
      cout << "What number would you like to delete?" << endl;
      cin >> number;
      cin.ignore(10, '\n');
      Node* position = search_tree(root, number);
      if(position != NULL){//value exists to delete
	delete_from_tree(position, position->getParent(), root);
	root->setColor('b');
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
  char ogColor = position->getColor();
  Node* sibling = NULL;
  char sibSide;
  Node* replace = NULL;
  
  
  //Case one: no children
  if(position->getLeft() == NULL &&
     position->getRight() == NULL){//zero children - delete child and set parent to left/right to null
    if(parent == NULL){//deleting the root with no children - head is null now
      root = NULL; 
    }
    //if this is it has not gone through recursion, check on position
    if(ogColor == 'b' && position != root && replace == NULL){//check to call cases
      //check case 2:      
      if(position->getParent() != NULL){
	sibling = position->getSib(position->getParent());
	if(position->getParent()->getLeft() == position){//position is left child so right is sibling
	  sibSide = 'r';
	}else if(position->getParent()->getRight() == position){//position is right child so left is sibling
          sibSide = 'l';
	}
	case_2(position, sibling, sibSide, root);
	
      }
    }
	 

    //delete position now
    if(parent->getLeft() == position){//set parent left to null
      parent->setLeft(NULL);
    }else{
      parent->setRight(NULL);
    }
    delete position;



       //Case two: child to left/right
   }else if((position->getLeft() != NULL &&
	   position->getRight() == NULL) ||
	   (position->getLeft() == NULL &&
	    position->getRight() != NULL)){//one child to left/right of position
    
    if(position->getLeft() != NULL){//set left value of position to new position and delete
      replace = position->getLeft();
      if(parent == NULL){//deleting root and replacing with left child
	replace->setParent(NULL);
        root = replace;
      }else{//not deleting root
        if(parent->getLeft() == position){
	  parent->setLeft(replace);
        }else{
  	  parent->setRight(replace);
        }
        replace->setParent(parent);
	
      }
    }else{//setting right of position
      replace = position->getRight();
      if(parent == NULL){//deleting root and replacing with right child
        replace->setParent(NULL);
        root = replace;
      }else{//not deleting root
        if(parent->getLeft() == position){
	  parent->setLeft(replace);
        }else{
	  parent->setRight(replace);
        }
        replace->setParent(parent);
      }
    }

 
    
    if(replace == root){//call case 1
      case_1(replace, root);
    }else if(replace->getColor() == 'b' &&
	     ogColor == 'b' ||
             replace == NULL){//cases 2-6 must need these specification to be called
      //establish sibling and sibling side
      if(replace->getParent() != NULL){
	sibling = replace->getSib(replace->getParent());
	if(replace->getParent()->getLeft() == replace){//replace is left child so right is sibling
	  sibSide = 'r';
	}else if(replace->getParent()->getRight() == replace){//replace is right child so left is sibling
	  sibSide = 'l';
	}
      

	if(replace == root){
	  case_1(replace, root);
	}else{
	  case_2(replace, sibling, sibSide, root);
	}

      }
      
    }  
    //recolor replace black
    replace->setColor('b');

    
    delete position;

        
    //Case three:
  }else if(position->getLeft() != NULL &&
	   position->getRight() != NULL){//two children - find farthest left position of right's left

    //STORE RIGHT ONE
    Node* rightOne = position->getRight();

    //FIND LEFT MOST
    Node* leftMost = find_left_or_right(rightOne, 'L');

    Node* swap = position;
    
    position->setValue(leftMost->getValue());

    leftMost->setValue(swap->getValue());
    
    delete_from_tree(leftMost, leftMost->getParent(), root);
    
    
  }
}



//Replaced is at the root and only one child
void case_1(Node* replaced, Node* &root){
  if(replaced == root){//double check
    root->setColor('b');
    return;
  }
  //establish sibling:
  if(replaced->getParent() != NULL &&
     replaced->getSib(replaced->getParent()) != NULL){
    char sibSide;
    if(replaced->getParent()->getLeft() == replaced){
       sibSide = 'r';
    }else{
       sibSide = 'l';
    }
    case_2(replaced, replaced->getSib(replaced->getParent()), sibSide, root);  
  }
}


//Sibling of replaced is red. Rotate left or right
void case_2(Node* replaced, Node* sibling, char sibSide, Node* &root){
  if(sibling->getColor() == 'r'){
    if(sibSide == 'l'){//sibling is on left side -> rotate right
      rightRotate(sibling->getParent(), sibling, root);
    }else if(sibSide == 'r'){//sibling is on right side -> rotate left
      leftRotate(sibling->getParent(), sibling, root);
    }
    sibling->setColor('b');
    sibling->getParent()->setColor('r');

    //reestablish sibling:
    if(replaced->getParent() != NULL){
      sibling = replaced->getSib(replaced->getParent());
    }
  }



  //call case 3 if sibling becomes red:
  if(sibling->getColor() == 'b' &&
     (sibling->getLeft() == NULL ||
      sibling->getLeft()->getColor() == 'b') &&
     (sibling->getRight() == NULL ||
      sibling->getRight()->getColor() == 'b')){
    case_3(sibling, root);
  }

  
  //call case 4
  if(sibling->getParent()->getColor() == 'r' &&
     (sibling->getLeft() == NULL ||
      sibling->getLeft()->getColor() != 'r') &&
     (sibling->getRight() == NULL ||
      sibling->getRight()->getColor() != 'r')){//children are either black or NULL and parent of sibling is red
    case_4(sibling->getParent(), sibling); 
  }else if(sibling->getParent()->getColor() == 'r' &&
           sibling->getLeft() != NULL &&
           sibling->getLeft()->getColor() != 'r' &&
           (sibling->getRight() == NULL ||
           sibling->getRight()->getColor() != 'r')){
    case_4(sibling->getParent(), sibling);
  }

  //call case 5
  if(sibling->getColor() == 'b' &&
     (sibling->getLeft() == NULL ||
     sibling->getLeft()->getColor() == 'b') &&
     sibling->getRight() != NULL &&
     sibling->getRight()->getColor() == 'r' &&
     sibSide == 'l'){
    case_5(sibling, sibSide, root);//sibSide will be on the left side
  }else if(sibling->getColor() == 'b' &&
	   (sibling->getRight() == NULL ||
	   sibling->getRight()->getColor() == 'b') &&
	   sibling->getLeft() != NULL &&
	   sibling->getLeft()->getColor() == 'r' &&
	   sibSide == 'r'){
    case_5(sibling, sibSide, root);//sibside will be right side
  }

  if(replaced->getParent() != NULL){
    sibling = replaced->getSib(replaced->getParent());
  }
  

  
  //call case 6
  if(sibling->getColor() == 'b' &&
     (sibling->getLeft() == NULL ||
      sibling->getLeft()->getColor() == 'b') &&
     sibling->getRight() != NULL &&
     sibling->getRight()->getColor() == 'r' &&
     sibSide == 'r'){
    case_6(sibling, sibling->getParent(), sibSide, root);//sibSide will be on the right side

  }else if(sibling->getColor() == 'b' &&
	   (sibling->getRight() == NULL ||
	    sibling->getRight()->getColor() == 'b') &&
	   (sibling->getLeft() != NULL &&
	    sibling->getLeft()->getColor() == 'r') &&
	   sibSide == 'l'){
    case_6(sibling, sibling->getParent(), sibSide, root); //sibSide will be on the left side
  }
  
}

//sibling of replaced is black. Color sibling red and call case one on siblings parent
void case_3(Node* sibling, Node* &root){
  sibling->setColor('r');
  if(sibling->getParent() != NULL){
    if(sibling->getParent()->getColor() == 'b'){
      case_1(sibling->getParent(), root);
    }
  }
}


//Parent is red, sibling and siblings children are black
void case_4(Node* parent, Node* sibling){
  parent->setColor('b');
  sibling->setColor('r');
}


//sibling and siblings right child is black, sibling's left is red if right sibSide
//sibling and siblings left child is black, sibling's right is red if left sibSide
void case_5(Node* sibling, char sibSide, Node* &root){
  if(sibSide == 'r'){//rotate right
    sibling->getLeft()->setColor('b');
    rightRotate(sibling, sibling->getLeft(), root);
    
  }else if(sibSide == 'l'){//rotate left
    sibling->getRight()->setColor('b');
    leftRotate(sibling, sibling->getRight(), root);
    
  }
  sibling->setColor('r'); //double check

}


//sibling and siblings right child is black, sibling's left is red if left sibSide
//sibling and siblings left child is black, sibling's right is red if right sibSide
void case_6(Node* sibling, Node* parent, char sibSide, Node* &root){
  if(sibSide == 'l'){//rotate right
    sibling->getLeft()->setColor('b');
    rightRotate(sibling->getParent(), sibling, root);
    
  }else if(sibSide == 'r'){//rotate left
    sibling->getRight()->setColor('b');
    leftRotate(sibling->getParent(), sibling, root);
  }

  //adjust colors
  sibling->setColor(parent->getColor());
  parent->setColor('b');
  
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
void add_to_tree(Node* value, Node* current, Node* &root, char &side, int count){
  if(root == NULL){//first value
    root = value;
    root->setColor('b');
  }else{//not first
    if(current->getValue() > value->getValue()){ //value goes to left
      if(count == 0){//determines which side of tree value is on
	side = 'l';
      }
      if(current->getLeft() == NULL){//value goes to immediate left
	current->setLeft(value);
	value->setParent(current);
	value->setColor('r');
      }else{ //keep going down left of head
	count++;
	add_to_tree(value, current->getLeft(), root, side, count);
      }
    }else if(current->getValue() <= value->getValue()){//value goes to right
      if(count == 0){//determines which side of tree value is on
	side = 'r';
      }
      if(current->getRight() == NULL){//value goes to immediate right
	current->setRight(value);
	value->setParent(current);
	value->setColor('r');
      }else{//keep going down right of head
	count++;
	add_to_tree(value, current->getRight(), root, side, count);
      }
    }
  }
}

//readjusts the nodes in the tree to follow the R-B rules
//no two reds in a row
//current always starts as red
//case 1: new node is root
//case 2: parent node is red and root
//case 3: parent and uncle nodes are red
//case 4: parent node is red, uncle node is black/NULL, inserted node is "left grandchild"
//case 5: parent node is red, uncle node is black/NULL, inserted node is "right grandchild"
void adjustNode(Node* current, Node* &root, char side){
  if(current != root){//not the root -> cases 2-4
    Node* grand = NULL;
    Node* parent = NULL;
    Node* uncle = NULL;

    if(current->getParent() != NULL){
      parent = current->getParent();

      //can only have grandparent if you have a parent:
      if(current->getGrand(parent) != NULL){
	grand = current->getGrand(parent);

	//can only have unlce if you have grandparent:
	if(current->getUncle(grand, parent) != NULL){
	  uncle = current->getUncle(grand, parent);
	}
      }
    }
   
    if(parent != NULL &&
       parent->getColor() == 'r' &&
       parent == root){//case 2 (parent and root are red)
      //CASE 2: recolor root black
      root->setColor('b');
    }else if(parent->getColor() == 'r' &&
	     uncle != NULL &&
	     uncle->getColor() == 'r'){//case 3 (parent and uncle are red)
      //FOR CASE 3: recolor parent and uncle black. Grandparent becomes red
      parent->setColor('b');
      uncle->setColor('b');
      grand->setColor('r');
      //CASE 3: check up the tree with grandparent as current if grandparent's parent is red
      if(grand != NULL &&
	 grand->getParent() != NULL &&
	 grand->getParent()->getColor() == 'r'){
	adjustNode(grand, root, side);
      }
    }else if(parent != NULL &&
	     parent->getColor() == 'r' &&
	     (uncle == NULL || uncle->getColor() == 'b') &&
	     parent->getLeft() == current){//case 4 (parent=red, uncle=black or null, current="left grandchild"-->left of parent)
      if(grand->getLeft() == parent){
         rightRotate(grand, parent, root);
      }else{
         rightRotate(parent, current, root);
      }
      


      if(parent->getColor() == 'r'){//still needs to rotate left
	leftRotate(grand, current, root);
         current->setColor('b');
      }             
    }else if(parent->getColor() == 'r' &&
	     (uncle == NULL ||
	      uncle->getColor() == 'b') &&
	     parent->getRight() == current){//case 5 (same as case 4 but current is right of parent
      //FOR CASE 5: rotate left like in case 4
      if(grand->getRight() == parent){
         leftRotate(grand, parent, root);
      }else{
         leftRotate(parent, current, root);
      }

      if(parent->getColor() == ('r')){//still need to rotate right
         rightRotate(grand, current, root);
         current->setColor('b');
      }
    }
      
      
  }else{//case 1: make sure root is black
    root->setColor('b');
  }

  if(root->getColor() != 'b'){//ensure that root is always black
    root->setColor('b');
  }

}



/*
 * I used code from CodesDope
 * I used this code to get the outline for how to rotate nodes
 * to the left, then implemented it for the right.
 * URL: https://www.codesdope.com/course/data-structures-red-black-trees-insertion/
 */

void rightRotate(Node* top, Node* bottom, Node* &root){
  if(bottom->getRight() != NULL){//set top's left to bottom's right
   top->setLeft(bottom->getRight());
   bottom->getRight()->setParent(top);
  }else{
   top->setLeft(NULL);
  }


  if(top->getParent() != NULL){//set the parent of bottom to the top's parent
    bottom->getParent()->setParent(top->getParent());
  }else{//else dealing with the root
    bottom->setParent(NULL);
  }


  if(top->getParent() == NULL){
    root = bottom;
  }else if(top == top->getParent()->getRight()){//top is a right child
    top->getParent()->setRight(bottom);
    bottom->setParent(top->getParent());
  }else{//top is left child
    top->getParent()->setLeft(bottom);
    bottom->setParent(top->getParent());
  }

  bottom->setRight(top);
  top->setParent(bottom);

  //recolor nodes:
  if(top->getColor() == 'b'){//switch the coloring of the nodes
    top->setColor('r');
    bottom->setColor('b');
  }else{//otherwise make sure nodes are both red
    top->setColor('r');
    bottom->setColor('r');
  }
  

}


void leftRotate(Node* top, Node* bottom, Node* &root){

 if(bottom->getLeft() != NULL){
    top->setRight(bottom->getLeft());
    bottom->getLeft()->setParent(top);
  }else{
    top->setRight(NULL);
  }
  

  if(top->getParent() != NULL){//set parent of bottom to top's parent
    bottom->getParent()->setParent(top->getParent());
  }else{//dealing with the root
    bottom->setParent(NULL);
  }

  if(top->getParent() == NULL){
    root = bottom;
  }else if(top == top->getParent()->getLeft()){
    top->getParent()->setLeft(bottom);
    bottom->setParent(top->getParent());
  }else{
    top->getParent()->setRight(bottom);
    bottom->setParent(top->getParent());
  }

  bottom->setLeft(top);
  top->setParent(bottom);
   


  if(top->getColor() == 'b'){//switch coloring of nodes
    top->setColor('r');
    bottom->setColor('b');
  }else{//otherwise both nodes are same color
    top->setColor('r');
    bottom->setColor('r');
  }
}

