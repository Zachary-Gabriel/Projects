#include <iostream>
#include <sstream>

using namespace std;

class Node
{
public:
	int level = -101;
	int element = -1;
	Node* left = nullptr;
	Node* right = nullptr;
	Node* parent = nullptr;
};

// finds and sets the value that indicates which level of the tree the node is on.
int find_level (Node* head)
{
	int floor = 0;
	Node* block = head;

	while (block->parent != nullptr) 
	{
		block = block-> parent;
		++floor;
	}
	
	return floor;
}

// find the largest hight
int find_height (Node* head)
{
	int floor = 1;
	if (head == nullptr)
	{
		return 0;
	}

	if (floor <= find_height(head-> left) )
	{
		floor = 1 + find_height(head-> left);
	}

	if (floor <= find_height(head-> right) )
	{
		floor = 1 + find_height(head-> right);
	}
	return floor;
}

Node* add_left_left(Node* head)
{
	Node* v = head;
	Node* x = v-> right;
	
	Node* A = v-> left;
	Node* B = x-> left;
	Node* C = x-> right;

	x-> parent = head-> parent;
	if (head-> parent != nullptr)
	{
		Node* daddy = head-> parent;
		if (daddy-> left == head) daddy-> left = x;
		if (daddy-> right == head) daddy-> right = x;
	}

	x-> left = v;
	v-> parent = x;

	x-> right = C;
	if (C != nullptr) C-> parent = x;

	v-> left = A;
	if (A != nullptr) A-> parent = v;

	v-> right = B;
	if (B != nullptr) B-> parent = v;

	return x;
}

Node* add_right_right(Node* head)
{
	Node* v = head;
	Node* x = v-> left;
	Node* A = v-> right;
	Node* B = x-> right;
	Node* C = x-> left;
	
	x-> parent = head-> parent;
	if (head-> parent != nullptr)
	{
		Node* daddy = head-> parent;
		if (daddy-> left == head) daddy-> left = x;
		if (daddy-> right == head) daddy-> right = x;
	}

	x-> right = v;
	v-> parent = x;

	x-> left = C;
	if (C != nullptr) C-> parent = x;

	v-> right = A;
	if (A != nullptr) A-> parent = v;

	v-> left = B;
	if (B != nullptr) B-> parent = v;

	return x;
}

Node* add_right_left(Node* head)
{
	Node* v = head;
	Node* x = v-> right;
	Node* A = v-> left;
	Node* w = x-> left;
	Node* D = x-> right;	
	Node* B = w-> left;
	Node* C = w-> right;

	w-> parent = head-> parent;
	if (head-> parent != nullptr)
	{
		Node* daddy = head-> parent;
		if (daddy-> left == head) daddy-> left = w;
		if (daddy-> right == head) daddy-> right = w;
	}

	w-> right = x;
	x-> parent = w;
	
	w-> left = v;
	v-> parent = w;

	v-> left = A;
	if (A != nullptr) A-> parent = v;

	v-> right = B;
	if (B != nullptr) B-> parent = v;

	x-> left = C;
	if (C != nullptr) C-> parent = x;

	x-> right = D;
	if (D != nullptr) D-> parent = x;
	
	return w;
}

Node* add_left_right(Node* head)
{
	Node* v = head;
	Node* x = v-> left;
	Node* A = v-> right;
	Node* w = x-> right;
	Node* D = x-> left;	
	Node* B = w-> right;
	Node* C = w-> left;

	w-> parent = head-> parent;
	if (head-> parent != nullptr)
	{
		Node* daddy = head-> parent;
		if (daddy-> left == head) daddy-> left = w;
		if (daddy-> right == head) daddy-> right = w;
	}

	w-> left = x;
	x-> parent = w;
	
	w-> right = v;
	v-> parent = w;

	v-> right = A;
	if (A != nullptr) A-> parent = v;

	v-> left = B;
	if (B != nullptr) B-> parent = v;

	x-> right = C;
	if (C != nullptr) C-> parent = x;

	x-> left = D;
	if (D != nullptr) D-> parent = x;
	
	return w;
}

// deciding which left rotation to do
Node* parse_left_add_rotation(Node* node)
{
	Node* x = node-> right; // x	
	int left_height = find_height (x-> left);
	int right_height = find_height (x-> right);

	if (left_height < right_height) 
	{
		node = add_left_left (node);
		return node;
	}
	else
	{
		node = add_right_left (node);
		return node;
	}
}

// deciding which right rotation to do
Node* parse_right_add_rotation(Node* node)
{
	Node* x = node-> left; // x	
	int left_height = find_height (x-> left);
	int right_height = find_height (x-> right);

	if (left_height > right_height) 
	{
		node = add_right_right (node);
		return node;
	}
	else
	{
		node = add_left_right (node);
		return node;
	}
}

// rotation on add
Node* parse_add_rotation(Node* node)
{
	Node* leftNode = node-> left;
	Node* rightNode = node-> right;
	int left_height = find_height (leftNode);
	int right_height = find_height (rightNode);
	bool done = false;
	//
	while (node != nullptr) // root
	{
		if (done)
		{
			if (node-> parent == nullptr)
			{
				return node;
			}
			node = node-> parent;
		}
		else
		{
			if (-1 <= (left_height - right_height) && (left_height - right_height) <= 1 )
			{
				if (0 != left_height && (left_height - right_height) == 0) 
				{
					done = true;
				}
			}

			else
			{
				// left
				if (left_height < right_height)
				{
					node = parse_left_add_rotation (node);
				}

				//right
				else
				{
					node = parse_right_add_rotation (node);
				}
			}
			if (node-> parent == nullptr)
			{
				return node;
			}
			node = node-> parent;
			left_height = find_height (node-> left);
			right_height = find_height (node-> right);
		}
	}
	return node;
}

// the function that adds a node with the element of 'data' to the avl tree.
Node* AVL_add (Node* head, int data)
{
	Node* tempNode = head;
	// AVL_find
	while ( tempNode-> element != data )
	{
		if (tempNode-> element < data)
		{
			if (tempNode-> right != nullptr) tempNode = tempNode->right;
			else break;
		}
		else if (tempNode-> element > data)
		{
			if (tempNode-> left != nullptr) tempNode = tempNode->left;
			else break;
		}
		else 
		{
			break; // if c++ has a stroke
		}
	}

	// already exists
	if (tempNode-> element == data)
	{
		return head; 
	}

	//create node
	Node* newNode = new Node;
	newNode-> left = nullptr;
	newNode-> right = nullptr;
	newNode-> element = data;
	newNode-> parent = tempNode;
	

	// fit it into the list
	if (tempNode-> element < data)
	{
		tempNode-> right = newNode;
	}
	else if (tempNode-> element > data)
	{
		tempNode-> left = newNode;
	}

	newNode-> level = find_level(newNode);
	head = parse_add_rotation (newNode);
	return head; 
}

// the function that removes a node with the element of 'data' from the avl tree.
Node* AVL_delete (Node* head, int data)
{
	Node* parentNode = head;
	Node* deleteMe = head;

	while (deleteMe-> element != data ) // finding the node we want to delete
	{
		if (deleteMe-> element > data) // filter down the correct side of the tree
		{
			// if node does not exist, stop the function
			if (deleteMe-> left == nullptr)
			{
				return head;  
			} 
			deleteMe = deleteMe-> left;
		}

		else if (deleteMe-> element < data) // filter down the correct side of the tree
		{
			// if node does not exist, stop the function
			if (deleteMe-> right == nullptr) {
				return head; 
			}
			deleteMe = deleteMe-> right;
		}
		
		else break; // for when c++ has a stroke and fails to recognise the 'while' condition
	}

	deleteMe-> level = find_level(deleteMe);
	
	// making parent node
	parentNode = deleteMe-> parent;
	if (parentNode == nullptr) parentNode = deleteMe; // if head

	// no children
	if (deleteMe-> right == nullptr && deleteMe-> left == nullptr) 
	{
		if (deleteMe-> level == 0) // if head
		{
			// make new head that is my version of an empty tree (most of these are sanity checks):
			deleteMe-> level = 0;
			deleteMe-> element = -1;
			deleteMe-> left = nullptr;
			deleteMe-> right = nullptr;
			deleteMe-> parent = nullptr;
		}

		else
		{
			if (parentNode->right == deleteMe) parentNode->right = nullptr;
			else if (parentNode->left == deleteMe) parentNode->left = nullptr;
			delete deleteMe;
			head = parse_add_rotation (parentNode);
		}
	}

	// two children
	else if (deleteMe-> right != nullptr && deleteMe-> left != nullptr) 
	{	
		Node* leftNodeGoingRight = deleteMe-> left;
		int count = 0;
		while (leftNodeGoingRight-> right != nullptr)
		{
			++count;
			leftNodeGoingRight = leftNodeGoingRight-> right; // getting largest value in smaller-valued tree
		}

		deleteMe-> element = leftNodeGoingRight-> element; //swapping values

		parentNode = leftNodeGoingRight-> parent;
		if (count > 0)
		{
			Node* restOfTree = leftNodeGoingRight->left;
			parentNode-> right = restOfTree;
			if (restOfTree != nullptr)
			{
				restOfTree-> parent = parentNode;
				head = parse_add_rotation (restOfTree);
			}
			else
			{
				head = parse_add_rotation (parentNode);
			}
		}
		else
		{
			Node* restOfTree = leftNodeGoingRight->left;
			parentNode-> left = restOfTree;
			if (restOfTree != nullptr)
			{
				restOfTree-> parent = parentNode;
				head = parse_add_rotation (restOfTree);
			}
			else
			{
				head = parse_add_rotation (parentNode);
			}
		}
		delete leftNodeGoingRight;
	}

	// one child
	else if (deleteMe-> right != nullptr || deleteMe-> left != nullptr) 
	{
		if (deleteMe-> level == 0) // if head
		{
			if (deleteMe-> left != nullptr) // if head has left tree
			{
				head = deleteMe-> left;
			}
			
			if (deleteMe-> right != nullptr) // if head has right tree
			{
				head = deleteMe-> right;
			}

			head-> level = 0;
			head-> parent = nullptr;
			delete deleteMe;
		}

		else if (deleteMe == parentNode-> left) // if child came from left parent node
		{
			Node* temp = nullptr;
			if (deleteMe-> right != nullptr) // if child has rest of tree in the right node
			{
				temp = deleteMe-> right;
			}
			else if (deleteMe-> left != nullptr) // if child has rest of tree in the left node
			{
				temp = deleteMe-> left;
			}

			parentNode-> left = temp;
			temp-> parent = parentNode;
			head = parse_add_rotation (temp);
			delete deleteMe;	
		}

		else if (deleteMe == parentNode-> right) // if child came from right parent node
		{
			Node* temp = nullptr;
			if (deleteMe-> right != nullptr) // if child has rest of tree in the right node
			{
				temp = deleteMe-> right;
			}
			else if (deleteMe-> left != nullptr) // if child has rest of tree in the left node
			{
				temp = deleteMe-> left;
				
			}

			parentNode-> right = temp;
			temp-> parent = parentNode;
			head = parse_add_rotation (temp);
			delete deleteMe;	
		}
	}
	return head; 
}

// prints the elements of the avl tree in pre-order
void read_pre(Node* parentNode)
{
	if (parentNode-> element == -1)
	{
		cout << "EMPTY"; // if tree is empty
	}

	else
	{
		cout << parentNode-> element << ' ';
	}

	if (parentNode-> left != nullptr)
	{
		read_pre(parentNode-> left);
	}

	if (parentNode-> right != nullptr)
	{
		read_pre(parentNode-> right);
	}

	return ;
}

// prints the elements of the avl tree in post-order
void read_post(Node* parentNode)
{
	if (parentNode-> element == -1)
	{
		cout << "EMPTY"; // if tree is empty
	}

	else
	{
		cout << parentNode-> element << ' ';
	}
	
	if (parentNode-> right != nullptr)
	{
		read_post(parentNode-> right);
	}

	if (parentNode-> left != nullptr)
	{
		read_post(parentNode-> left);
	}

	return ;
}

// prints the elements of the avl tree in order.
void read_in(Node* parentNode)
{
	if (parentNode-> left != nullptr)
	{
		read_in(parentNode-> left);
	}
	
	if (parentNode-> element == -1)
	{
		cout << "EMPTY"; // if tree is empty
	}

	else
	{
		cout << parentNode-> element << ' ';
	}

	if (parentNode-> right != nullptr)
	{
		read_in(parentNode-> right);
	}
	return ;
}

// takes input using getline, converts the letters into functions and the digits into integers, then parses them appropriately
int main ()
{
	// preallocaing variables
	Node* head = new Node;
	bool add = false, rm = false;
	string input = "", word = "";
	int i, length, var = 0;

	std::getline(std::cin,input); // getting the input line
	length = input.size(); // getting the size of the input once for faster performance

	for (i = 0; i < length; ++i) // iterating through each character of the input string
	{
		if (input[i] == 'A') // for adding
		{
			var = 0;
			add = true;
		}
		
		else if (input[i] == 'D') // for deleting
		{
			var = 0;
			rm = true;
		}
		
		else if (48 <= int(input[i]) && int(input[i]) <= 57) // if the input is a number
		{
			var = 10* var + int(input[i]) - 48;
		}
		
		else if (input[i] == ' ') // symbolises next addition, removal, or view type
		{
			if (add) // when adding node to the tree
			{
				// cout << var << '\n';
				if (head->element == -1) // if tree doesn't exist, make this element the head
				{
					head-> element = var;
					head-> left = nullptr;
					head-> right = nullptr;
					head-> parent = nullptr;
					head-> level = 0;
				}
				
				else // if the head exists, make it (more of) a tree
				{
					head = AVL_add (head, var);
				}
			}

			else if (rm) // when removing node from the tree
			{
				// cout << var << '\n';
				head = AVL_delete (head, var);
			}

			// reset the numbers so we can add / remove nodes from the tree.
			var = 0;
			rm = false;
			add = false;
		}
		
		else // append letters of the final word to figure out how to view the tree
		{
			word += input[i];

			// use word to figure out how to print the output
			if (word == "IN") read_in(head);
			else if (word == "PRE") read_pre(head);
			else if (word == "POST") read_post(head);
		}
	}

	cout << '\n';
	return 0;
}