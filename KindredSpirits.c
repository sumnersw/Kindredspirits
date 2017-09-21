// Stephen Sumner
// Assignment 4 - Kindred Spirits
// COP3502C - M/W/F 10:30 to 11:20

#include <stdio.h>
#include <stdlib.h>
#include "KindredSpirits.h"
#include "debug.h"

#define LO 3
#define FS 2
#define FR 1

// this make_node function is identical to proffesor Szumlanski's create_node
// function. I merely changed the name of the function to avoid conflicts in 
// the code, and I used calloc which I prefer to malloc when the function 
// contains pointers.

node *make_node(int data)
{
	node *n = calloc(1, sizeof(node));

	n->data = data;
	return n;
}

// The following stack functions are implemented based on Szumlanski's provided
// stack code. There may be changes depending upon their use in the assignment functions

typedef struct Stack
{
	int *array;			// the elements
	int size;			// current size, gets iterated
	int capacity;			// maximum size of the array

} Stack;

// creates a stack with an internal array of length 'capacity' based on Szumlanski's code
Stack *createStack(int capacity)
{
	Stack *s;
	if(capacity < 10)
	{
		if ((s = malloc(sizeof(Stack))) == NULL)
			return NULL;

		if ((s->array = malloc(sizeof(int) * 9)) == NULL)
		{
			free(s);
			return NULL;
		}
	
		s->size = 0;
		s->capacity = 9;
	
		return s;
	}
	else
	{
		if ((s = malloc(sizeof(Stack))) == NULL)
			return NULL;
	
		if ((s->array = malloc(sizeof(int) * capacity)) == NULL)
		{
			free(s);
			return NULL;
		}
	
		s->size = 0;
		s->capacity = capacity;
	
		return s;
	}
}

// destroy stack; coded defensively to avoid segfaults, based on Szumlanski's code
Stack *destroyStack(Stack *s)
{
	if (s == NULL)
		return NULL;

	free(s->array);
	free(s);

	return NULL;
}

// push a new element onto the top of the stack. Based on Szumlanski's code, 
// however, the isFull function should not need to be implemented. There will
// be another function used to count the number of nodes in the trees provided
// and the stacks will be created based on the numbers returned if they are
// equal in size. If they are unequal, there will be no need since the number
// of nodes being different would imply that the trees are not reflections.
// the stacks are only needed to determine the proper order of the elements in
// the trees.
void push(Stack *s, int data)
{
	//post-increment
	s->array[ s->size++ ] = data;
}

// remove the top element from the stack and return its value, based on 
// the code on stacks provided by Professor Szumlanski, that awesome dude.
int pop(Stack *s)
{
	int popped;
	popped = s->array[ --s->size ];

	return popped;

}

// these are custom helper functions to start the pre order and post order
// tree traversals that also push elements onto the stacks that are needed.

void preorder_Push(node *root, Stack *s)
{
	if (root == NULL)
		return;

	push(s, root->data);
	preorder_Push(root->left, s);
	preorder_Push(root->right, s);	
}

void postorder_Push(node *root, Stack *s)
{
	if (root == NULL)
		return;

	postorder_Push(root->left, s);
	postorder_Push(root->right, s);
	push(s, root->data);
}

// these are custom helper functions to count the nodes in the trees that are
// passed to the functions 

void inorder_count(node *root, int *count)
{
	if (root == NULL)
		return;

	inorder_count(root->left, count);
	*count = *count+1;
	inorder_count(root->right, count);
}

int count_nodes(node *root)
{
	int *count, retval;
	count = calloc(1, sizeof(int));

	inorder_count(root, count);

	retval = *count;
	free(count);

	return retval;
}

// Start of the main functions for the assignment


int isReflection(node *a, node *b)
{
	debug(FS, "isReflection");
	int flagleft, flagright, flagnode;

	// firstly first off the very first thing to do. check for 
	// Null nodes

	if(a == NULL && b == NULL)
	{
		return 1;
	}
	else if(a == NULL && b != NULL)
	{
		return 0;
	}
	else if(a != NULL && b == NULL)
	{
		return 0;
	}

	// first go all the way down the left branch to determine
	// the leftflag value
	
	if(a->left != NULL && b->right != NULL)
	{
		flagleft = isReflection(a->left, b->right);
	}
	else if(a->left == NULL && b->right == NULL)
	{
		flagleft = 1;
	}
	else
	{
		// if this point is reached, then the branches on either side
		// of this node are not equal, which automatically means they 
		// are not mirrors, and setting the flag to zero will affect the
		// value that this function returns recursively

		flagleft = 0;
	}

	// then go all the way down the right branch to determine 
	// the right flag value

	if(a->right != NULL && b->left != NULL)
	{
		flagright = isReflection(a->right, b->left);
	}
	else if(a->right == NULL && b->left == NULL)
	{
		flagright = 1;
	}
	else 
	{
		// if this point is reached, then the branches on either side
		// of this node are not equal, which automatically means they 
		// are not mirrors, and setting the flag to zero will affect the
		// value that this function returns recursively

		flagright = 0;
	}


	// this part of the code checks the left and right flags first
	// if either one are zero, than the two trees are not reflections
	// of each other, if both are 1, indicating reflections, the 
	// result of checking for data equality is returned, 1 if equal,
	// 0 if not equal

	if(flagleft == 0 || flagright == 0)
	{
		return 0;
	}
	else
	{
		if(a->data == b->data)
		{
			flagnode = 1; 
		}
		else
		{
			flagnode = 0;
		}
		return flagnode;
	}

}

node *makeReflection(node *root)
{
	// this function goes through each node of the tree and creates a node
	// with the data of the data passed in. It is made recursively, and 
	// the data from the right of the original node is passed into the left 
	// of the mirror node, This should mirror the structure and data of the 
	// tree, satisfying the definition for reflection provided in the 
	// assignment data.

	node *mirrornode;
	if (root == NULL)
		return NULL;

	mirrornode = make_node(root->data);
	mirrornode->left = makeReflection(root->right);
	mirrornode->right = makeReflection(root->left);

	return mirrornode;
}

int kindredSpirits(node *a, node *b)
{
	// To start the function off, each tree is counted for the number of nodes
	// in each tree. Afterwards, the first test compares the number of nodes 
	// in each tree, if they differ in size, than that automatically implies that
	// the two trees will not be kindred spirits since the data they contain will
	// be different from each other. 
	int i, size_a, size_b, check_a, check_b, flag1, flag2;
	flag1 = flag2 = 1;
	size_a = count_nodes(a);
	size_b = count_nodes(b);

	// the variables for the pointers to the dynamically created stacks are 
	// declared well before they may be needed. They also need to be initialized
	// to point to NULL 
	Stack *stack_a, *stack_b, *stack_c, *stack_d;
	stack_a = NULL;
	stack_b = NULL;
	stack_c = NULL;
	stack_d = NULL;

	// the first test for size equality
	if(size_a != size_b)
	{
		return 0;
	}

	// if the sizes are equal, there is a chance they could be kindred spirits
	// so now the stacks need to be created based on the number of nodes counted 
	// in each tree.
	stack_a = createStack(size_a);
	stack_b = createStack(size_b);
	stack_c = createStack(size_a);
	stack_d = createStack(size_b);

	// The next step is to push all the elements of tree a into stack_a
	// and all the elements of tree b into stack_b all of these are order n
	// operations.

	preorder_Push(a, stack_a);
	preorder_Push(b, stack_d);
	postorder_Push(b, stack_b);
	postorder_Push(a, stack_c);

	// now the data from each stack will be popped and compared for equality,
	// the function will exit at the first value that differs and return 0
	// if all the values are equal, than the function will return 1 indicating
	// that the two trees are kindred spirits per the definition provided
	// in the assignment.
	
	i = 0;
	while( flag1 != 0 && i < size_a)
	{
		check_a = pop(stack_a);
		check_b = pop(stack_b);
		if(check_a == check_b)
		{
			flag1 = 1;
		}
		else if(check_a != check_b)
		{
			flag1 = 0;
		}
		i++;
	}

	i = 0;
	while(flag2 != 0 && i < size_a)
	{
		check_a = pop(stack_c);
		check_b = pop(stack_d);
		if(check_a == check_b)
		{
			flag2 = 1;
		}
		else if(check_a != check_b)
		{
			flag2= 0;
		}
		i++;
	}
	
	// now that the flag has either been set to 0, or all the node values traversed
	// it is time to free all the dynamically allocated memory in order to return 
	// the results of the check for equality

	stack_a = destroyStack(stack_a);
	stack_b = destroyStack(stack_b);
	stack_c = destroyStack(stack_c);
	stack_d = destroyStack(stack_d);

	if(flag1 == 1)
	{
		return flag1;
	}
	else if(flag2 == 1)
	{
		return flag2;
	}
	else
		return 0;

}

double difficultyRating(void)
{
	double diffRating = 4.0;
	return diffRating;
}

double hoursSpent(void)
{
	double hours = 8.5;	
	return hours;
}

