/* array.h
*
* Created by Supakorn Sumongkol (ssumongkol@student.unimelb.edu.au) 07/09/2022
*
* the inferface of module 'array' of this project
* 
*/

#ifndef _ARRAY_H_
#define _ARRAY_H_
#include "data.h"

// constant defnitions
#define INIT_SIZE 2

// data definitions
typedef struct array array_t;

/* creates & returns an empty array
* (code from W3.8 skeleton code)
*/
array_t *arrayCreate();

/* track array size & element, expand the size when needed
* (code from W3.8 skeleton code)
*/
void arrayEnsureSize(array_t *arr);

/* create a sorted array using LinkedList using 'grade1in' as key
* (inspired from sortedArrayInsert(...) W3.8 skeleton code)
*/
void arrayInsertSorted(array_t *arr, data_t *d);

/* print the data inside the array
* (code from W3.8 skeleton code)
*/
void arrayPrint(array_t *arr, FILE *out_file);

/* shrinks the array, to reduce array size to the same
* as the number of element used
* (code from W3.8 skeleton code)
*/
void arrayShrink(array_t *arr);

/* free memory used by array "arr"
* (inspired from arrayFree(...) in W3.8 skeleton code)
*/
void arrayFree(array_t *arr);

#endif
