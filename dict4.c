/* dict4.c
*
* Created by Supakorn Sumongkol (ssumongkol@student.unimelb.edu.au) 07/09/2022
*
* Read footpath data from data_file (.csv) and stored in quadtree (this struct
* consist of Rectangle which refer to the area of this quadtree node, and
* 4 quadtree node which represent each quadrant of the parent node) where
* each record will be represent as 2 quadtree node with the location as
* (start_lon, start_lat) for the 1st node and (end_lon, end_lat) for 2nd node. 
* Then, we recieve the input query as 2 pair of longtidue & latitude which
* represent a bottomleft & topright point of rectangle area, searching
* for all quadtree node within that area, and print the records to output_file 
* (.txt file) and accessing quadrant sequence to get this node to stdout.
*
* To run the program type: 
* ./dict4 4 data.csv output.txt bl_lon bl_lat tr_lon tr_lat < input.txt
*
* Parameters details:
* 	- stage: 4 (specify program implementation stage)
* 	- data.csv: data_file which stored data record on each line 
* 	- output.txt: output_file for printing matching data records
* 	- bl_lon: longitude value of the bottomleft point of root_node in quadtree
* 	- bl_lat: latitude value of the bottomleft point of root_node in quadtree
* 	- tr_lon: longitude value of the topright point of root_node in quadtree
* 	- tr_lat: latitude value of the topright point of root_node in quadtree
* 	- input.txt: input_file which stored searching lon & lat pair on each line
* 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"
#include "quadtree.h"
#include "array.h"

#define QUADTREE_RANGE_QUERY 4
#define MAX_LONG_DOUBLE_QUERY 30

int processArgs(int argc, char *argv[]);
void createQuadtree(FILE *in_file, quadtreeNode_t *root_node);
void quadtreeRangeQuery(FILE *query_file, FILE *in_file, FILE *out_file, 
quadtreeNode_t *root_node);

int main(int argc, char *argv[]) {
    int stage = processArgs(argc, argv);
    FILE *in_file = fopen(argv[2], "r");
    assert(in_file);
    FILE *out_file = fopen(argv[3], "w");
    assert(out_file);
	char *ptr;
	long double root_bl_x, root_bl_y, root_tr_x, root_tr_y;
	quadtreeNode_t *root_node = quadtreeNodeCreate();

	root_bl_x = strtold(argv[4], &ptr);
	root_bl_y = strtold(argv[5], &ptr);
	root_tr_x = strtold(argv[6], &ptr);
	root_tr_y = strtold(argv[7], &ptr);
	assignBLpointRec(root_bl_x, root_bl_y, getRectangle2DInNode(root_node));
	assignTRpointRec(root_tr_x, root_tr_y, getRectangle2DInNode(root_node));

    switch (stage) {
        case QUADTREE_RANGE_QUERY:
			quadtreeRangeQuery(stdin, in_file, out_file, root_node);
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
    if (argc < 8 || atoi(argv[1]) != QUADTREE_RANGE_QUERY) {
        fprintf(stderr, "Error Invalid Arguments: %s required [stage, "
		"data_file_name, output_file_name, bl_lon, bl_lat, tr_lon, tr_lat] " 
		"as arguments, where:\n", argv[0]);
		fprintf(stderr, "       \t - stage: 4\n");
		fprintf(stderr, "       \t - data_file_name: .csv input for "
		"building dictionary\n");
		fprintf(stderr, "       \t - output_file_name: .txt output file for "
		"printing result\n");
		fprintf(stderr, "       \t - root_node_bottomleft_x: longtitude(x) "
		"of bottom_left point of quadtree root node\n");
		fprintf(stderr, "       \t - root_node_bottomleft_y: latitude(y) "
		"of bottom_left point of quadtree root node\n");
		fprintf(stderr, "       \t - root_node_topright_x: longtitude(x) "
		"of top_right point of quadtree root node\n");
		fprintf(stderr, "       \t - root_node_topright_y: latitude(y) "
		"of top_right point of quadtree root node\n");
		exit(EXIT_FAILURE);
    }
    return atoi(argv[1]);
}

/*------------- FUNCTIONS FOR WORKING WITH QUADTREE_RANGE_QUERY -------------*/

/* reading data from data_file(.csv) and initialize the quadtree with the
* 'root_node' as root node of the quadtree using these data
* (insipired from *getStudentArray(...) in W3.8 skeleton code)
*/
void createQuadtree(FILE *data_file, quadtreeNode_t *root_node) {
	data_t *d;

	dataSkipHeaderLine(data_file);
	while (feof(data_file) == 0) {
		d = dataRead(data_file);
		assignDataToQuadtree(d, root_node);
	}
}

/* read querying data from stdin as a 2 pair of (longtitude, latitude) which
* represent bottomleft & topright point of rectangle area and
* searching for all datapoint in quadtree within this area.
* print all matching record in output_file(.txt) and accessing quadrant
* that occur on each search to stdout
* (insipired from arrayQuerying(...) in W3.8 skeleton code)
*/
void quadtreeRangeQuery(FILE *query_file, FILE *in_file, FILE *out_file, 
quadtreeNode_t *root_node) {
	char *string_bl_lon = malloc((MAX_LONG_DOUBLE_QUERY+1) *sizeof(char));
	assert(string_bl_lon);
	char *string_bl_lat = malloc((MAX_LONG_DOUBLE_QUERY+1) *sizeof(char));
	assert(string_bl_lat);
	char *string_tr_lon = malloc((MAX_LONG_DOUBLE_QUERY+1) *sizeof(char));
	assert(string_tr_lon);
	char *string_tr_lat = malloc((MAX_LONG_DOUBLE_QUERY+1) *sizeof(char));
	assert(string_tr_lat);
	char *ptr;
	long double search_bl_lon, search_bl_lat, search_tr_lon, search_tr_lat;
	rectangle2D_t *search_rec = rectangle2DCreate();
	
	createQuadtree(in_file, root_node);
	while (fscanf(query_file, "%s %s %s %s\n", string_bl_lon, string_bl_lat, 
	string_tr_lon, string_tr_lat) == 4) {
		array_t *result_arr = arrayCreate();
		fprintf(out_file, "%s %s %s %s\n", string_bl_lon, string_bl_lat, 
		string_tr_lon, string_tr_lat);
		printf("%s %s %s %s -->", string_bl_lon, string_bl_lat, string_tr_lon, 
		string_tr_lat);
		search_bl_lon = strtold(string_bl_lon, &ptr);
		search_bl_lat = strtold(string_bl_lat, &ptr);
		search_tr_lon = strtold(string_tr_lon, &ptr);
		search_tr_lat = strtold(string_tr_lat, &ptr);
		assignBLpointRec(search_bl_lon, search_bl_lat, search_rec);
		assignTRpointRec(search_tr_lon, search_tr_lat, search_rec);
		searchRectanglePoint(search_rec, root_node, result_arr, out_file);
		printf("\n");
		arrayPrint(result_arr, out_file);
		arrayFree(result_arr);
	}
	quadtreeFree(root_node);
	rectangleFree(search_rec);
	free(string_bl_lon);
	free(string_bl_lat);
	free(string_tr_lon);
	free(string_tr_lat);
}