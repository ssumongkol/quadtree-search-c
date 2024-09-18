/* dictionary.c
*
* Created by Supakorn Sumongkol (ssumongkol@student.unimelb.edu.au) 15/08/2022
*
* contain data structure 'linkedListNode' and functions logic 
* related to 'linkedListNode'.
* 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dictionary.h"
#include "data.h"

struct linkedListNode {
	data_t *data;
	linkedListNode_t *next;
};

/* creates & returns an empty node
* (inspired from arrayCreate(...) in W3.8 skeleton code)
*/
linkedListNode_t *nodeCreate() {
	linkedListNode_t *node = malloc(sizeof(*node));
	assert(node);
	node->data = NULL;
	node->next = NULL;
	return node;
}

/* assign data_t element into node(linkedListNode)
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
void assignDataToNode(linkedListNode_t *node, data_t *d) {
	node->data = d;
}

/* assign pointer to the next node(linkedListNode) into node
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
void assignNextToNode(linkedListNode_t *current_node, 
linkedListNode_t *nextNode) {
	current_node->next = nextNode;
}

/* get node->data value
* return (data_t) node->data
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
data_t *getDataInNode(linkedListNode_t *node) {
	return node->data;
}

/* get node->next value
* return (linkedListNode_t) node->next
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
linkedListNode_t *getNextNode(linkedListNode_t *node) {
	return node->next;
}

/* get node->data->address value
* return (char *) node->data->address
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
char *getAddressInNode(linkedListNode_t *node) {
	return dataGetAddress(node->data);
}

/* get node->data->grade1in value
* return (dobule) node->data->grade1in
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
double getGrade1inInNode(linkedListNode_t *node) {
	return dataGetGrade1in(node->data);
}

// validate whether node->next is NULL or not?
int validateNextNode(linkedListNode_t *node) {
	if (node->next != NULL) {
		return 1;
	} else {
		return 0;
	}
}

/* print data in node to output_file(.txt) starting from the 1st to last node
* for debugging purpose only
* (inspired from arrayPrint(...) in W3.8 skeleton code)
*/
void dictionaryPrint(FILE *out_file, linkedListNode_t *head_node) {
	linkedListNode_t *current_node = head_node;

	while (current_node->next != NULL) {
		dataPrint(out_file, current_node->data);
		current_node = current_node->next;
	}
	dataPrint(out_file, current_node->data);
}

/* free memory used by linkedListNode_t "node" from 1st to last node
* (inspired from arrayFree(...) in W3.8 skeleton code)
*/
void dictionaryFree(linkedListNode_t *head_node) {
	linkedListNode_t *current_node, *prev_node;
	current_node = head_node;

	while (current_node != NULL) {
		dataFree(current_node->data);
		prev_node = current_node;
		current_node = current_node->next;
		free(prev_node);
	}
}
