#include "linked_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *initialise_node(void)
{
	Node *node;

	if(!(node = (Node *) malloc(sizeof(Node)))) {
		fprintf(stderr, "error: unable to initialise node.\n");
		exit(EXIT_FAILURE);
	}
	node->next = node->prev = node->data = NULL;

	return node;
}

void free_node(Node *node)
{
	if(!node)
		return;
	free(node);
}

LinkedList *initialise_linked_list(void)
{
	LinkedList *list;

	if(!(list = (LinkedList *) malloc(sizeof(LinkedList)))) {
		fprintf(stderr, "error: unable to initialise linked list.\n");
		exit(EXIT_FAILURE);
	}
	list->head = list->tail = NULL;

	return list;
}

void free_linked_list(LinkedList *list)
{
	Node *next;

	while(list->head) {
		next = list->head->next;
		free_node(list->head);
		list->head = next;
	}
	free(list);
}

void append_linked_list(LinkedList *list, void *data)
{
	Node *node;

	node = initialise_node();

	node->data = data;
	node->prev = list->tail;
	if(list->tail) {
		list->tail->next = node;
	}
	list->tail = node;
	if(!list->head)
		list->head = node;
}

void prepend_linked_list(LinkedList *list, void *data)
{
	Node *node;

	node = initialise_node();

	node->data = data;
	node->next = list->head;
	if(list->head) {
		list->head->prev = node;
	}
	list->head = node;
	if(!list->tail)
		list->tail = node;
}

void remove_head_linked_list(LinkedList *list)
{
	Node *head;

	if(!list->head)
		return;
	head = list->head->next;
	free(list->head);
	list->head = head;
	if(list->head)
		list->head->prev = NULL;
	else
		list->tail = NULL;

}

void remove_tail_linked_list(LinkedList *list)
{
	Node *tail;

	if(!list->tail)
		return;
	tail = list->tail->prev;
	free_node(list->tail);
	list->tail = tail;
	if(list->tail)
		list->tail->next = NULL;
	else
		list->head = NULL;
}

void print_linked_list(LinkedList *list, void (*print_func)(void *))
{
	Node *next = list->head;

	while(next) {
		(*print_func)(next->data);
		next = next->next;
	}
}

void print_char(void *ptr)
{
	printf("%c\n", *((char *) ptr));
}

void print_int(void *ptr)
{
	printf("%d\n", *((int *) ptr));
}

void print_double(void *ptr)
{
	printf("%f\n", *((double *) ptr));
}

void print_string(void *ptr)
{
	printf("%s\n", (char *) ptr);
}
