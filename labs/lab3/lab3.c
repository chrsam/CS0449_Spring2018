// Chris Mananghaya (cam314)
// CS0449: Lab 3

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct Node
{
	struct Node* next;
	int value; 
} Node;

// 1.) use malloc() to allocate enough memory to hold one Node, and store the pointer to the allocated memory in a Node* variable.
// 2.) Since malloc() gives you memory which may be full of garbage, you are responsible for filling in the Node's fields with valid values.
//	   Set its value member to the value that was passed into this function, and its next member to null. 
// 3.) Return the Node*
Node* create_node(int value)
{
	Node* node_space = malloc(sizeof(Node));
	node_space->value = value;
	node_space->next = NULL;
	return node_space;
}


void print_list(Node* n)
{
	// for(; n != NULL; n = n->next)
	// {
	// 	printf("%d\n", n->value);
	// }

	if(n == NULL)
		return;
	print_list(n->next);
	printf("%d\n", n->value);
}

// Goes through the list, calling free on each of the nodes in the list. It should also do nothing if the parameter is NULL.
// Remember the other important rule of the heap: you can't access memory hat has been freed. So your loop will have to be
// a little different than before. 
void free_list(Node* n)
{
	while(n != NULL)
	{
		// printf("%d\n", n->value);
		Node* temp = n;
		temp = n->next;
		free(n);
		n = NULL;
		n = temp;
	}
}

int main()
{
	Node* new_node = create_node(500);
	printf("value = %d, next = %p\n", new_node->value, new_node->next);

	Node* a = create_node(10);
	Node* b = create_node(20);
	Node* c = create_node(30);
	a->next = b;
	b->next = c;
	print_list(a);
	free_list(a);

	Node* head = NULL;
	Node* list;
	char prompt[100];
	int input = 0;
	
	while (input != -1) {
	
		printf("Add an integer to the list: (or -1 to quit) ");
		fgets(prompt, sizeof(prompt), stdin);
		sscanf(prompt, "%d", &input);
		
		if (input == -1) {
			break;
		}
		
		Node* new = create_node(input);
		
		
		if (head == NULL) {
			head = new;
			list = new;
		} else {
			list->next = new;
			list = list->next;
		} 
	}
		print_list(head);
		free_list(head);

	return 0;
}