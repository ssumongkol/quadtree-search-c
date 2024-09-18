/* data.h
*
* Created by Supakorn Sumongkol (ssumongkol@student.unimelb.edu.au) 15/08/2022
*
* the inferface of module 'linkedListNode' of this project
* 
*/

#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_
#include "data.h"

// data definitions
typedef struct linkedListNode linkedListNode_t;

/* creates & returns an empty node
* (inspired from arrayCreate(...) in W3.8 skeleton code)
*/
linkedListNode_t *nodeCreate();

/* assign data_t element into node(linkedListNode)
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
void assignDataToNode(linkedListNode_t *node, data_t *d);

void assignIsStartPointToNode(int val, linkedListNode_t *node)

/* assign pointer to the next node(linkedListNode) into node
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
void assignNextToNode(linkedListNode_t *current_node, 
linkedListNode_t *next_node);

/* get node->data value
* return (data_t) node->data
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
data_t *getDataInNode(linkedListNode_t *node);

/* get node->next value
* return (linkedListNode_t) node->next
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
linkedListNode_t *getNextNode(linkedListNode_t *node);

/* get node->data->address value
* return (char *) node->data->address
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
char *getAddressInNode(linkedListNode_t *node);

/* get node->data->grade1in value
* return (dobule) node->data->grade1in
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
double getGrade1inInNode(linkedListNode_t *node);

// validate whether node->next is NULL or not?
int validateNextNode(linkedListNode_t *node);

/* print data in node to output_file(.txt) starting from the 1st to last node
* for debugging purpose only
* (inspired from arrayPrint(...) in W3.8 skeleton code)
*/
void dictionaryPrint(FILE *out_file, linkedListNode_t *head_node);

/* free memory used by linkedListNode_t "node" from 1st to last node
* (inspired from arrayFree(...) in W3.8 skeleton code)
*/
void dictionaryFree(linkedListNode_t *head_node);

#endif
