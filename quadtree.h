/* quadtree.h
*
* Created by Supakorn Sumongkol (ssumongkol@student.unimelb.edu.au) 07/09/2022
*
* the inferface of module 'quadtree' of this project
* 
*/

#ifndef _QUADTREE_H_
#define _QUADTREE_H_
#include "data.h"
#include "array.h"

// constant defnitions
#define EPSILON 10e-12

// data definitions
typedef struct quadtreeNode quadtreeNode_t;

typedef struct rectangle2D rectangle2D_t;

typedef struct datapoint datapoint_t;

typedef struct point2D point2D_t;

enum Quadrant{SW, NW, NE, SE};

/* creates & returns an empty point2D_t object
* (inspired from arrayCreate(...) in W3.8 skeleton code)
*/
point2D_t *point2DCreate();

/* creates & returns an empty datapoint_t object
* (inspired from arrayCreate(...) in W3.8 skeleton code)
*/
datapoint_t *datapointCreate();

/* creates & returns an empty rectangle2D_t object
* (inspired from arrayCreate(...) in W3.8 skeleton code)
*/
rectangle2D_t *rectangle2DCreate();

/* creates & returns an empty quadtreeNode_t object
* (inspired from arrayCreate(...) in W3.8 skeleton code)
*/
quadtreeNode_t *quadtreeNodeCreate();

/* assign (lon, lat) value to a point2D_t object
* (inspired from assignDataToNode(...) in my Assignment1 code)
*/
void assignLonLatPoint(double lon, double lat, point2D_t *p);

/* assign bottom_left (lon, lat) value to a rectangle2D of the quadtree node
* (inspired from assignDataToNode(...) in my Assignment1 code)
*/
void assignBLpointRec(long double x, long double y, rectangle2D_t *rec);

/* assign top_right (lon, lat) value to a rectangle2D of the quadtree node
* (inspired from assignDataToNode(...) in my Assignment1 code)
*/
void assignTRpointRec(long double x, long double y, rectangle2D_t *rec);

/* assign bottomleft & topright point in rectangle2D of child_node using
* bottomleft & topright point in rectangle2D of parent_node
* (inspired from concept & visualization in 'Introduction to PR Quadtrees'
* section of the Assignment2)
*/
void assignNodeRecFromParent(int assign_quadrant, rectangle2D_t *child_rec,
rectangle2D_t *parent_rec);

/* stored data record into quadtree, by creating 2 datapoint_t object since
* 1 record must be inserted as 2 datapoint (1st: (start_lon, start_lat), 
* 2nd: (end_lon, end_lat) as its location respectively). Also, for the first
* datapoint, we set is_start_point = 1 for preventing the duplicate printing
* in the result.
* (inspired from concept & visualization in 'Introduction to PR Quadtrees'
* and 'Implementation Details' section of the Assignment2)
*/
void assignDataToQuadtree(data_t *d, quadtreeNode_t *node);

/* insert datapoint_t object into quadtree, by determining the leaf_node,
* if YES, check for existing datapoint_t object inside that node and subdivide
* the node / assign the datapoint_t. if NO, we determined the appropriate
* quadrant for this datapoint and called addPoint(...) again on that quadrant
* (recursive).
* (inspired from concept & visualization in 'Introduction to PR Quadtrees'
* and 'Implementation Details' section of the Assignment2)
*/
void addPoint(datapoint_t *datapoint, quadtreeNode_t *node);

/* validate the node existence, if it's not exist, we will initialise the node
* using the data from the rectangle2D of its parent node.
* (inspired from concept & visualization in 'Introduction to PR Quadtrees'
* and 'Implementation Details' section of the Assignment2)
*/
quadtreeNode_t *validateNodeExistence(int quadrant, quadtreeNode_t *check_node,
quadtreeNode_t *parent_node);

/* validate the location of 2 datapoint_t object. If it's the same, we should
* store these 2 datapoint_t object as LinkedList (we cannot separately assign
* these 2 datapoint into 2 separate quadrant), else we can subdivide this node.
* (inspired from concept & visualization in 'Introduction to PR Quadtrees'
* and 'Implementation Details' section of the Assignment2)
*/
int validateSubDivide(datapoint_t *a, datapoint_t *b);

/* subdivide the node, where we will create only a necessary sub-node.
* Then, set this node is_leaf to 0 (since this is not a leaf node anymore since
* we subdivide it) and re-assign the existing datapoint of the node into
* the sub-node.
* (inspired from concept & visualization in 'Introduction to PR Quadtrees'
* and 'Implementation Details' section of the Assignment2)
*/
void subDivideNode(quadtreeNode_t *node);

/* search the data(inside datapoint_t object) in the node using the pair of
* (lon, lat) reading from the query_file, printing the matching record
* to out_file and accessing quadrant while searching to stdout.
* (inspired from concept & visualization in 'Introduction to PR Quadtrees'
* and 'Implementation Details' section of the Assignment2)
*/
void searchDatapoint(point2D_t *search_point, quadtreeNode_t *root_node, 
FILE *out_file);

/* access the appropriate quadrant that this point2D should be in.
* return the appropriate quadrant(quadtreeNode_t object) if found, if that
* quadrant not exist, we return NULL.
* (inspired from concept & visualization in 'Introduction to PR Quadtrees'
* and 'Implementation Details' section of the Assignment2)
*/
quadtreeNode_t *accessQuadrant(point2D_t *search_point, quadtreeNode_t *node);

/* search all the data(inside datapoint_t object) in the node which inside a
* rectangle2D object provided from the query file, printing all matching record
* to out_file without duplicate and the accessing quadrant while searching 
* to stdout.
* (inspired from concept & visualization in 'Introduction to PR Quadtrees'
* and 'Implementation Details' section of the Assignment2)
*/
void searchRectanglePoint(rectangle2D_t *search_rec, quadtreeNode_t *node,
array_t *arr, FILE *out_file);

/* determine whether the point2D_t object is located inside this Rectangle2D
* object or not.
* (inspired 'Implementation Details' section of the Assignment2)
*/
int inRectangle(point2D_t *point2D, rectangle2D_t *rectangle);

/* determine whether the rec_a object is overlapping with rec_b object or not.
* (inspired 'Implementation Details' section of the Assignment2)
*/
int rectangleOverlap(rectangle2D_t *rec_a, rectangle2D_t *rec_b);

/* determine the appropriate quadrant that this point2D_t object should be
* located in.
* return int value which refer to the appropriate quadrant
* (inspired 'Implementation Details' section of the Assignment2)
*/
int determineQuadrant(point2D_t *p, rectangle2D_t *rec);

/* print the access quadrant as a text to stdout.
* (inspired 'Implementation Details' section of the Assignment2)
*/
void printQuadrantText(enum Quadrant q);

/* compare whether point_a & point_b are at the same location or not.
* return 1 if it's YES, else 0
* (inspired 'Implementation Details' section of the Assignment2 & ED discussion
* question number #640)
*/
int compareDataPoint2D(point2D_t *a, point2D_t *b);

/* auxlilary function to convert the long double value as positive (similar to
* fabs() function but receive long double as an input)
* return positive long double value
* (code from ED discussion question number #640)
*/
long double lfabs(long double ld);

/* return data_t object inside datapoint_t object
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
data_t *getDataInDatapoint(datapoint_t *d_point);

/* return rectangle2D_t object inside quadtreeNode_t object
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
rectangle2D_t *getRectangle2DInNode(quadtreeNode_t *node);

/* free memory used by quadtreeNode_t object
* (inspired from arrayFree(...) in W3.8 skeleton code)
*/
void quadtreeFree(quadtreeNode_t *node);

/* free memory used by rectangle2D_t object
* (inspired from arrayFree(...) in W3.8 skeleton code)
*/
void rectangleFree(rectangle2D_t *rec);

/* free memory used by datapoint_t object
* (inspired from arrayFree(...) in W3.8 skeleton code)
*/
void datapointFree(datapoint_t *d_point);

#endif
