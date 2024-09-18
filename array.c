/* array.c
*
* Created by Supakorn Sumongkol (ssumongkol@student.unimelb.edu.au) 07/09/2022
*
* contain data structure 'array' and functions logic related to 'array'.
* 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <float.h>
#include <math.h>
#include "array.h"
#include "data.h"

struct array {
	data_t **data;
	int size;
	int n;
};

/* creates & returns an empty array
* (code from W3.8 skeleton code)
*/
array_t *arrayCreate() {
	array_t *arr = malloc(sizeof(*arr));
	assert(arr);
	int size = INIT_SIZE;
	arr->size = size;
	arr->data = malloc(size * sizeof(*(arr->data)));
	assert(arr->data);
	arr->n = 0;
	return arr;
}

/* track array size & element, expand the size when needed
* (code from W3.8 skeleton code)
*/
void arrayEnsureSize(array_t *arr) {
	if (arr->n == arr->size) {
		arr->size *= 2;
		arr->data = realloc(arr->data, arr->size * sizeof(*(arr->data)));
		assert(arr->data);
	}
}

/* create a sorted array using LinkedList using 'grade1in' as key
* (inspired from sortedArrayInsert(...) W3.8 skeleton code)
*/
void arrayInsertSorted(array_t *arr, data_t *d) {
    data_t *temp_data;
    int i, j;

    arrayEnsureSize(arr);
    arr->data[arr->n] = d;
    arr->n++;
    for (i = 1; i < arr->n; i++) {
        j = i - 1;
        temp_data = arr->data[i];
        while ((j >= 0) && 
        (dataGetFootpathID(temp_data) < dataGetFootpathID(arr->data[j]))) {
            arr->data[j+1] = arr->data[j];
            j--;
        }
        arr->data[j+1] = temp_data;
    }
}

/* print the data inside the array
* (code from W3.8 skeleton code)
*/
void arrayPrint(array_t *arr, FILE *out_file) {
    for (int i = 0; i < arr->n; i++) {
        dataPrintFinal(out_file, arr->data[i]);
        dataAssignIsInArray(0, arr->data[i]);
    }
}

/* shrinks the array, to reduce array size to the same
* as the number of element used
* (code from W3.8 skeleton code)
*/
void arrayShrink(array_t *arr) {
	if (arr->size != arr->n) {
		arr->size = arr->n > INIT_SIZE ? arr->n : INIT_SIZE;
		arr->data = realloc(arr->data, arr->size * sizeof(*(arr->data)));
		assert(arr->data);
	}
}

/* free memory used by array "arr"
* (inspired from arrayFree(...) in W3.8 skeleton code)
*/
void arrayFree(array_t *arr) {
	free(arr->data);
	free(arr);
}
