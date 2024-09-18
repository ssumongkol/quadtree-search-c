/* main.c
*
* Created by Supakorn Sumongkol (ssumongkol@student.unimelb.edu.au) 15/08/2022
*
* Read data from data_file (.csv) and stored in linked_list / 
* sorted_array_linked_list, recieve input query as address / grade1in,
* search for the matching / nearest matching records, and print the records
* to output_file (.txt file) and some information on stdout based on the
* input arguments.
*
* To run the program type: 
* ./dict1 1 data.csv output.txt < input.txt
* or
* ./dict2 2 data.csv output.txt < input.txt
*
* Parameters details:
* 	- stage: [1, 2] (specify program implementation stage)
* 	- data.csv: data_file which stored data record on each line
* 	- input.txt: input_file which stored searching keyword on each line 
* 	- output.txt: output_file for printing matching data records 
* 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"
#include "dictionary.h"
#include "array.h"

#define LINKED_LIST 1
#define SORTED_LINKED_LIST_ARRAY 2
#define MIN_CMD LINKED_LIST
#define MAX_CMD SORTED_LINKED_LIST_ARRAY
#define MAX_DOUBLE_QUERY 20

int processArgs(int argc, char *argv[]);
linkedListNode_t *getDictionary(FILE *in_file);
void dictionaryQuerying(FILE *query_file, FILE *in_file, FILE *out_file);
void sortedDictionaryQuerying(FILE *query_file, FILE *in_file, FILE *out_file);

int main(int argc, char *argv[]) {
    int stage = processArgs(argc, argv);
    FILE *in_file = fopen(argv[2], "r");
    assert(in_file);
    FILE *out_file = fopen(argv[3], "w");
    assert(out_file);

    switch (stage) {
        case LINKED_LIST:
            dictionaryQuerying(stdin, in_file, out_file);
			break;
        case SORTED_LINKED_LIST_ARRAY:
			sortedDictionaryQuerying(stdin, in_file, out_file);
			break;
		
		default:
			fprintf(stderr, "Error: Program stage %d, is not yet implemented\n"
			, stage);
			exit(EXIT_FAILURE);
    }

	fclose(in_file);
	fclose(out_file);
	return 0;
}

/* make sure to have enough arguments in the command line
* returns the specified stage value
* (insipired from processArgs(...) in W3.8 skeleton code)
*/
int processArgs(int argc, char *argv[]) {
    if (argc < 4 || atoi(argv[1]) < MIN_CMD || atoi(argv[1]) > MAX_CMD) {
        fprintf(stderr, "Error Invalid Arguments: %s required [stage, "
		"data_file_name, output_file_name] as arguments, where:\n", argv[0]);
		fprintf(stderr, "       \t - stage: [1, 2]\n");
		fprintf(stderr, "       \t - data_file_name: .csv input for "
		"building dictionary\n");
		fprintf(stderr, "       \t - output_file_name: .txt output file for "
		"printing result\n");
		exit(EXIT_FAILURE);
    }
    return atoi(argv[1]);
}

/*----FUNCTIONS FOR WORKING WITH LINKEDLIST AND SORTED ARRAYS LINKEDLIST -----*/

/* read data records from in_file(.csv)
* returns 'head_node' of LinkedList structure
* (insipired from *getStudentArray(...) in W3.8 skeleton code)
*/
linkedListNode_t *getDictionary(FILE *in_file) {
	data_t *d;
	linkedListNode_t *head_node, *current_node, *new_node;
	head_node = NULL;

	dataSkipHeaderLine(in_file);
	while (feof(in_file) == 0) {
		d = dataRead(in_file);
		new_node = nodeCreate();
		assignDataToNode(new_node, d);
		if (!head_node) {
			head_node = new_node;
			current_node = new_node;
		} else {
			assignNextToNode(current_node, new_node);
			current_node = new_node;
		}
	}
	return head_node;
}

/* read querying data from stdin with 'address'
* using linear search, looping through each node in LinkedList
* print matching record in output_file(.txt) and matching info. in stdout
* (insipired from arrayQuerying(...) in W3.8 skeleton code)
*/
void dictionaryQuerying(FILE *query_file, FILE *in_file, FILE *out_file) {
	char *query = malloc((MAX_STRING_LEN+1) *sizeof(char));
	assert(query);
	int data_found;
	linkedListNode_t *current_node;
	linkedListNode_t *head_node = getDictionary(in_file);

	while (fscanf(query_file, "%[^\n]\n", query) == 1) {
		data_found = 0;
		current_node = head_node;
		fprintf(out_file, "%s\n", query);
		while (current_node != NULL) {
			if (strcmp(getAddressInNode(current_node), query) == 0) {
				dataPrintFinal(out_file, getDataInNode(current_node));
				data_found++;
			}
			current_node = getNextNode(current_node);
		}
		if (data_found == 0) {
			printf("%s --> NOTFOUND\n", query);
		} else {
			printf("%s --> %d\n", query, data_found);
		}
	}
	dictionaryFree(head_node);
	free(query);
}

/* read querying data from stdin with 'grade1in'
* create sorted_array from LinkedList using 'grade1in' as key
* using binary search to find a nearest matching record
* print the record in output_file(.txt) and nearest matching info. in stdout
* (insipired from arrayQuerying(...) in W3.8 skeleton code)
*/
void sortedDictionaryQuerying(FILE *query_file, FILE *in_file, FILE *out_file) {
	double double_query;
	char *ptr;
	char *query = malloc((MAX_DOUBLE_QUERY+1) * sizeof(char));
	assert(query);
	array_t *arr = arrayCreate();
	linkedListNode_t *nearest_node;
	linkedListNode_t *head_node = getDictionary(in_file);

	arrayInsertSorted(arr, head_node);
	while (fscanf(query_file, "%[^\n]\n", query) == 1) {
		fprintf(out_file, "%s\n", query);
		double_query = strtod(query, &ptr);
		nearest_node = arraySearch(arr, double_query);
		dataPrintFinal(out_file, getDataInNode(nearest_node));
		printf("%s --> %lf\n", query, getGrade1inInNode(nearest_node));
	}
	dictionaryFree(head_node);
	arrayFree(arr);
	free(query);
}

