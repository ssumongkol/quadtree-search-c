/* dict3.c
*
* Created by Supakorn Sumongkol (ssumongkol@student.unimelb.edu.au) 07/09/2022
*
* Read footpath data from data_file (.csv) and stored in quadtree (this struct
* consist of Rectangle which refer to the area of this quadtree node, and
* 4 quadtree node which represent each quadrant of the parent node) where
* each record will be represent as 2 quadtree node with the location as
* (start_lon, start_lat) for the 1st node and (end_lon, end_lat) for 2nd node. 
* Then, we recieve the input query as 1 pair of longtidue & latitude, searching
* for the quadtree node with the matching location, and print the records
* to output_file (.txt file) and accessing quadrant sequence to get this node
* to stdout.
*
* To run the program type: 
* ./dict3 3 data.csv output.txt bl_lon bl_lat tr_lon tr_lat < input.txt
*
* Parameters details:
* 	- stage: 3 (specify program implementation stage)
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

#define QUADTREE_SEARCH 3
#define MAX_LONG_DOUBLE_QUERY 30

int processArgs(int argc, char *argv[]);
void createQuadtree(FILE *data_file, quadtreeNode_t *root_node);
void quadtreeSearch(FILE *query_file, FILE *data_file, FILE *out_file,
quadtreeNode_t *root_node);

int main(int argc, char *argv[]) {
    int stage = processArgs(argc, argv);
    FILE *data_file = fopen(argv[2], "r");
    assert(data_file);
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
        case QUADTREE_SEARCH:
            quadtreeSearch(stdin, data_file, out_file, root_node);
			break;
		
		default:
			fprintf(stderr, "Error: Program stage %d, is not yet implemented\n"
			, stage);
			exit(EXIT_FAILURE);
    }

	fclose(data_file);
	fclose(out_file);
	return 0;
}

/* make sure to have enough arguments in the command line
* returns the specified stage value
* (insipired from processArgs(...) in W3.8 skeleton code)
*/
int processArgs(int argc, char *argv[]) {
    if (argc < 8 || atoi(argv[1]) != QUADTREE_SEARCH) {
        fprintf(stderr, "Error Invalid Arguments: %s required [stage, "
		"data_file_name, output_file_name, bl_lon, bl_lat, tr_lon, tr_lat] " 
		"as arguments, where:\n", argv[0]);
		fprintf(stderr, "       \t - stage: 3\n");
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

/*---------------- FUNCTIONS FOR WORKING WITH QUADTREE_SEARCH ----------------*/

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

/* read querying data from stdin as a pair of (longtitude, latitude) and
* searching through the quadtree
* print matching record in output_file(.txt) and accessing quadrant
* that occur on each search to stdout
* (insipired from arrayQuerying(...) in W3.8 skeleton code)
*/
void quadtreeSearch(FILE *query_file, FILE *data_file, FILE *out_file,
quadtreeNode_t *root_node) {
	char *query_lon = malloc((MAX_LONG_DOUBLE_QUERY+1) *sizeof(char));
	assert(query_lon);
	char *query_lat = malloc((MAX_LONG_DOUBLE_QUERY+1) *sizeof(char));
	assert(query_lat);
	char *ptr;
	double search_lon, search_lat;
	point2D_t *search_point = point2DCreate();
	
	createQuadtree(data_file, root_node);
	while (fscanf(query_file, "%s %s\n", query_lon, query_lat) == 2) {
		fprintf(out_file, "%s %s\n", query_lon, query_lat);
		printf("%s %s -->", query_lon, query_lat);
		search_lon = strtod(query_lon, &ptr);
		search_lat = strtod(query_lat, &ptr);
		assignLonLatPoint(search_lon, search_lat, search_point);
		searchDatapoint(search_point, root_node, out_file);
		printf("\n");
	}

	quadtreeFree(root_node);
	free(search_point);
	free(query_lon);
	free(query_lat);
}