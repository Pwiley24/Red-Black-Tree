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
	  cout << "Current: " << newNode->getValue() << endl;
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

/*
//Delete a number from the tree
//5 cases:
//1 = if node is red node -> remove from tree
//2 = if root is DB (double black) -> remove DB and root is black
//3 = if DB sibling is black -> remove DB
//  = if DB;s sibling's children are black -> DB's siblings are red
//  = if DB's parent black, it becomes DB, otherwise make black
//4 = If DB's sibling red -> swap DB's and DB's sibling colors
//  = rotate at parent node in direction of DB node
//  = check cases if need to readjust
//5 =
void delete_from_tree(Node* deleting, Node* &root){
  Node* parent = NULL;
  Node* grand = NULL;
  Node* uncle = NULL;
  Node* sibling = NULL;
  Node* replace = NULL;
  char side;
  
  if(deleting->getParent() != NULL){//parent is not null
    parent = deleting->getParent();

    //sibling and grand parents only exist if there is a parent
    grand = deleting->getGrand(parent);
    sibling = deleting->getSib(parent);

    if(grand != NULL){//grand parent isn't null
      uncle = deleting->getUnlce(grand, parent);
    }
  

    if(parent->getLeft() == deleting){//node is the left child
      side = 'l';
    }else if(parent->getRight() == deleting){//node is right child
      side = 'r';
    }
  }


  
  //CASE 1: node is red and root -> remove
  if(deleting->getColor() ==  'r' &&
     deleting == root){
    delete deleting;
    root = NULL;

    
    //CASE 2: node is red and not root -> remove and set parent left/right to NULL
  }else if(deleting->getColor() == 'r' &&
	   deleting != root){
    replace = NULL;
    if(deleting->getLeft() != NULL){//replace will be left child of deleting
      replace = deleting->getLeft();
    }else if(deleting->getRight() != NULL){//replace will be right child of deleting
      replace = deleting->getRight();
    }
    
    if(side == 'r'){//set parent's right child to null
      parent->setRight(replace);
      delete deleting;
    }else{//set parent's left child to null
      parent->setLeft(replace);
      delete deleting;
    }




    //CASE 3: deleting is a DB node and is the root -> remove DB and root is black
    if((deleting->getColor() == 'B' ||
	deleting == NULL) &&
       deleting == root){
      replace = NULL;
      if(deleteing->getRight() != NULL){//right child will replace
	replace = deleting->getRight();
	side = 'r';
      }else if(deleting->getLeft() != NULL){//left child will replace
	replace = deleting->getLeft();
	side = 'l';
      }
      if(replace != NULL){//delete deleting, set root to replace, set replace parent null, set left if needed 
	replace->setParent(NULL);
	root = replace;
	if(side == 'r'){//if there was a left node connect it to the new root (replace)
	  replace->setLeft(deleting->getLeft());
	  deleting->getLeft()->setParent(replace);
	}
	delete deleting;
      }





      //CASE 4: deleting is DB, has black sibling, and sibling's children are black
    }else if((deleting->getColor() == 'B' ||
	      deleting == NULL) && //deleting is DB or null
	     sibling != NULL && //sibling is not null
	     sibling->getColor() == 'b' && //sibling is black
	     (sibling->getLeft()->getColor() == 'b' ||
	      sibling->getLeft() == NULL) && //sibling's left child is null or black
	     (sibling->getRight()->getColor() == 'b' ||
	      sibling->getRight() == NULL)){ //sibling's right child is null or black
      //remove DB
      //made DB's siblings red
      //if DB's parent is black made it DB, else make it black

      replace = NULL;
      
      //delete deleting:
      if(deleting == NULL){//deleting is null, 
	
      }else if(deleting->getRight() != NULL){//if deleting has right child, it replaces deleting
	replace = deleting->getRight();
	side = 'r';
      }else if(deleting->getLeft() != NULL){//deleting has only left child, it replaces deleting
	replace = deleting->getLeft();
	side = 'r'
      }

      if(replace != NULL){//delete deleting, 
	
      }
	       
    }
  }
  
  
}
*/

/*
void delete_from_tree(Node* deleting, Node* &root, char side){
  char ogColor = deleting->getColor();
  Node* x = NULL;
  char child;
  Node* parent = NULL;

  //set parent of deleting
  if(deleting->getParent() != NULL){
    parent = deleting->getParent();
  }

  //set child side deleting is of parent
  if(parent->getLeft() == deleting){
    child = 'l';
  }else if(parent->getRight() == deleting){
    child = 'r';
  }


  
  //Case 1: deleting has no children and is red (will be red if no children)
  if(deleting->getLeft() == NULL &&
     deleting->getRight() == NULL &&
     ogColor == 'r'){//remove this node from tree
    if(child == 'l'){//remove child to left of parent
      parent->setLeft(NULL);
    }else if(child == 'r'){//remove child to right of parent
      parent->setRight(NULL);
    }else if(root == deleting){
      root = NULL;
    }
    delete deleting;


    //Case 2: deleting has one child to the right (based on ordering, deleting's children can't both be null
  }else if(deleting->getLeft() == NULL){ //assign right child of deleting to x
    x = deleting->getRight();
    if(parent != NULL){//not the root
      if(child == 'l'){//deleting is to the left of parent
	parent->setLeft(x);
      }else if(child == 'r'){//deleting is to the right of parent
	parent->setRight(x);
      }
    }else{ //dealing with the root
      x = root;
      x->setParent(NULL);
    }
    x->setColor('b');
    delete deleting;



    //Case 3: deleting has one child to the left (based on ordering, deleting's children can't both be null
  }else if(deleting->getRight() == NULL){//assign left child of deleting to x
    x = deleting->getLeft();
    if(parent != NULL){//not the root
      if(child == 'l'){//deleting is to the left of parent
	parent->setLeft(x);
      }else if(child == 'r'){//deleting is to the right of parent
	parent->setRight(x);
      }
    }else{
      x = root;
      x->setParent(NULL);
    }
    x->setColor('b');
    //adjustNode before deleting
    delete deleting;



    //Case 3: deleting has two children - find largest left node to left of deleting.
    //Assign it to deleting and remove largest left node. 
  }else{

    //ASSIGN X
    if(side == 'l'){//dealing with the left side of the tree - closest left node is the largest
      x = deleting->getLeft(); 
    }else if(side == 'r'){//dealing with the right side of tree - largest left is farthest right of left sub
      x = deleting->getLeft();
      while(x->getRight() != NULL){
	x = x->getRight();
      }
    }

    //COPY X TO DELETING
    deleting->setValue(x->getValue());

    //DELETE X'S PARENT CONNECTION
    if(x->getParent()->getLeft() == x){//x is left child of its parent
      x->getParent()->setLeft(NULL);
    }else if(x->getParent()->getRight() == x){//x is right child of its parent
      x->getParent()->setRight(NULL);
    }

    adjustNode(x, root, side);
    delete x;

    
  }

  
}

*/

//delete a number from the tree
//position is the node you are going to delete
//gathered from searching the tree for the node.
//parent = parent of position
//Note: parent can be null -> root of tree is being deleted
//cases: 0 children, 1 child, 2 children
void delete_from_tree(Node* position, Node* parent, Node* &root, char side){
  char ogColor = position->getColor();
  Node* replace = NULL;

  
  //Case one: no children
  if(position->getLeft() == NULL &&
     position->getRight() == NULL){//zero children - delete child and set parent to left/right to null
    if(parent == NULL){//deleting the root with no children - head is null now
      root = NULL; 
    }else if(parent->getLeft() == position){//set parent left to null
      parent->setLeft(NULL);
    }else{
      parent->setRight(NULL);
    }

    if(replace == NULL){//run the null deletion function

    }else if(ogColor == replace->getColor() &&
	     ogColor == 'b'){
      adjustNode(replace, root, side);
    }else{
      replace->setColor('b');
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
      replace = position->getRight();
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

 
    if(replace == NULL){//call the NULL case deletion function
      adjustNull(root, side);
    }else if(ogColor == replace->getColor() &&
	     ogColor == black){//adjust the tree with the new node
      adjustNode(replace, root, side);
    }else{
      replace->setColor('b');
    }
    //if replace black call
    //or position is black
    if(replace->getColor() == 'b' ||
       ogColor == 'b'){
      adjustDeleting(replace, root);
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
    delete_from_tree(leftMost, leftMost->getParent(), root, side);
    
    
  }
}



//Deleting cases
//6 cases
void adjustDeleting(Node* replaced, Node* &root){
    Node* sibling = NULL;
    char childSide;
    
    if(replaced->getParent() != NULL){
      sibling = replaced->getSib(replaced->getParent());

      if(replaced->getParent()->getLeft() == replaced){//replaced is left child so sibling is right
	child = 'r';
      }else{
	child = 'l';
      }
    }
    

    //case 1: replacing the root
    if(replaced == root){
      root->setColor('b');
      return;
    }
    
    //case 2: sibling is red
    if(sibling != NULL &&
       sibling->getColor() == 'r'){
      if(childSide == 'l'){//sibling is on the left side -> rotate right
	rightRotate(sibling->getParent(), sibling, root);
      }else if(childSide == 'r'){//sibling is on the right side -> rotate left
	leftRotate(sibling->getParent(), sibling, root);
      }
    }

    //case 3: sibling is black 
    if(sibling != NULL &&
       sibling->getColor() == 'b'){
      sibling->setColor('r');
      adjustDeleting(sibling->getParent(), root);
    }

    //case 4: parent is red, sibling and their children are black

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
	cout << "value going to left" << endl;
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
	cout << "value going to right" << endl;
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

    cout << "current: " << current->getValue() << endl;
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
      
      //make sure coloring is correct
      display_tree(root, 0);

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

