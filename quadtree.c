/* quadtree.c
*
* Created by Supakorn Sumongkol (ssumongkol@student.unimelb.edu.au) 07/09/2022
*
* contain data structure 'quadtreeNode', 'rectangle2D', 'datapoint', 
* and 'point2D' along with their functions logic related to these struct
* 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <float.h>
#include <math.h>
#include "quadtree.h"
#include "array.h"
#include "data.h"

struct quadtreeNode {
    rectangle2D_t *rec;
    quadtreeNode_t *SW;
    quadtreeNode_t *NW;
    quadtreeNode_t *NE;
    quadtreeNode_t *SE;
    int is_leafnode;
};

struct rectangle2D {
    point2D_t *bl_point;
    point2D_t *tr_point;
    datapoint_t *datapoint;
};

struct datapoint {
    point2D_t *loc;
    data_t *data;
    int is_start_point;
    datapoint_t *next_dpoint;
};

struct point2D {
    long double x;
    long double y;
};

/* creates & returns an empty point2D_t object
* (inspired from arrayCreate(...) in W3.8 skeleton code)
*/
point2D_t *point2DCreate() {
    point2D_t *point = (point2D_t *) malloc(sizeof(*point)); 
    assert(point);
    return point;
}

/* creates & returns an empty datapoint_t object
* (inspired from arrayCreate(...) in W3.8 skeleton code)
*/
datapoint_t *datapointCreate() {
    datapoint_t *point = (datapoint_t *) malloc(sizeof(*point));
    assert(point);
    point->loc = point2DCreate();
    point->next_dpoint = NULL;
    return point;
}

/* creates & returns an empty rectangle2D_t object
* (inspired from arrayCreate(...) in W3.8 skeleton code)
*/
rectangle2D_t *rectangle2DCreate() {
    rectangle2D_t *rec = (rectangle2D_t *) malloc(sizeof(*rec)); 
    assert(rec);
    rec->datapoint = NULL;
    rec->bl_point = point2DCreate();
    rec->tr_point = point2DCreate();
    return rec;
}

/* creates & returns an empty quadtreeNode_t object
* (inspired from arrayCreate(...) in W3.8 skeleton code)
*/
quadtreeNode_t *quadtreeNodeCreate() {
    quadtreeNode_t *node = (quadtreeNode_t *) malloc(sizeof(*node));  
    assert(node);
    node->is_leafnode = 1;
    node->SW = NULL;
    node->NW = NULL;
    node->NE = NULL;
    node->SE = NULL;
    node->rec = rectangle2DCreate();
    return node;
}

/* assign (lon, lat) value to a point2D_t object
* (inspired from assignDataToNode(...) in my Assignment1 code)
*/
void assignLonLatPoint(double lon, double lat, point2D_t *p) {
    p->x = lon;
    p->y = lat;
}

/* assign bottom_left (lon, lat) value to a rectangle2D of the quadtree node
* (inspired from assignDataToNode(...) in my Assignment1 code)
*/
void assignBLpointRec(long double x, long double y, rectangle2D_t *rec) {
    rec->bl_point->x = x;
    rec->bl_point->y = y;
}

/* assign top_right (lon, lat) value to a rectangle2D of the quadtree node
* (inspired from assignDataToNode(...) in my Assignment1 code)
*/
void assignTRpointRec(long double x, long double y, rectangle2D_t *rec) {
    rec->tr_point->x = x;
    rec->tr_point->y = y;
}

/* assign bottomleft & topright point in rectangle2D of child_node using
* bottomleft & topright point in rectangle2D of parent_node
* (inspired from concept & visualization in 'Introduction to PR Quadtrees'
* section of the Assignment2)
*/
void assignNodeRecFromParent(int assign_quadrant, rectangle2D_t *child_rec,
rectangle2D_t *parent_rec) {
    switch (assign_quadrant) {
        case SW:
            child_rec->bl_point->x = parent_rec->bl_point->x;
            child_rec->bl_point->y = parent_rec->bl_point->y;
            child_rec->tr_point->x = (parent_rec->bl_point->x + 
            parent_rec->tr_point->x) / 2;
            child_rec->tr_point->y = (parent_rec->bl_point->y + 
            parent_rec->tr_point->y) / 2;
            break;
        case NW:
            child_rec->bl_point->x = parent_rec->bl_point->x;
            child_rec->bl_point->y = (parent_rec->bl_point->y + 
            parent_rec->tr_point->y) / 2;
            child_rec->tr_point->x = (parent_rec->bl_point->x + 
            parent_rec->tr_point->x) / 2;
            child_rec->tr_point->y = parent_rec->tr_point->y;
            break;
        case NE:
            child_rec->bl_point->x = (parent_rec->bl_point->x + 
            parent_rec->tr_point->x) / 2;
            child_rec->bl_point->y = (parent_rec->bl_point->y + 
            parent_rec->tr_point->y) / 2;
            child_rec->tr_point->x = parent_rec->tr_point->x;
            child_rec->tr_point->y = parent_rec->tr_point->y;
            break;
        case SE:
            child_rec->bl_point->x = (parent_rec->bl_point->x + 
            parent_rec->tr_point->x) / 2;
            child_rec->bl_point->y = parent_rec->bl_point->y;
            child_rec->tr_point->x = parent_rec->tr_point->x;
            child_rec->tr_point->y = (parent_rec->bl_point->y + 
            parent_rec->tr_point->y) / 2;
            break;
    }
}

/* stored data record into quadtree, by creating 2 datapoint_t object since
* 1 record must be inserted as 2 datapoint (1st: (start_lon, start_lat), 
* 2nd: (end_lon, end_lat) as its location respectively). Also, for the first
* datapoint, we set is_start_point = 1 for preventing the duplicate printing
* in the result.
* (inspired from concept & visualization in 'Introduction to PR Quadtrees'
* and 'Implementation Details' section of the Assignment2)
*/
void assignDataToQuadtree(data_t *data, quadtreeNode_t *node) {
    datapoint_t *new_datapoint1 = datapointCreate();
    datapoint_t *new_datapoint2 = datapointCreate();
    new_datapoint1->data = new_datapoint2->data = data;
    new_datapoint1->loc->x = dataGetStartLon(data);
    new_datapoint1->loc->y = dataGetStartLat(data);
    new_datapoint1->is_start_point = 1;
    new_datapoint2->loc->x = dataGetEndLon(data);
    new_datapoint2->loc->y = dataGetEndLat(data);
    new_datapoint2->is_start_point = 0;
    addPoint(new_datapoint1, node);
    addPoint(new_datapoint2, node);
}

/* insert datapoint_t object into quadtree, by determining the leaf_node,
* if YES, check for existing datapoint_t object inside that node and subdivide
* the node / assign the datapoint_t. if NO, we determined the appropriate
* quadrant for this datapoint and called addPoint(...) again on that quadrant
* (recursive).
* (inspired from concept & visualization in 'Introduction to PR Quadtrees'
* and 'Implementation Details' section of the Assignment2)
*/
void addPoint(datapoint_t *datapoint, quadtreeNode_t *node) {
    if (node->is_leafnode) {
        if (node->rec->datapoint != NULL) {
            if (validateSubDivide(datapoint, node->rec->datapoint)) {
                subDivideNode(node);
                addPoint(datapoint, node);
            } else {
                datapoint_t *cur_datapoint = node->rec->datapoint;
                if (dataGetFootpathID(datapoint->data) <= 
                dataGetFootpathID(cur_datapoint->data)) {
                    datapoint->next_dpoint = cur_datapoint;
                    node->rec->datapoint = datapoint;
                } else {
                    while (1) {
                        if (cur_datapoint->next_dpoint == NULL) {
                            cur_datapoint->next_dpoint = datapoint;
                            break;
                        }
                        if (dataGetFootpathID(datapoint->data) <= 
                        dataGetFootpathID(cur_datapoint->next_dpoint->data)) {
                            datapoint->next_dpoint = cur_datapoint->next_dpoint;
                            cur_datapoint->next_dpoint = datapoint;
                            break;
                        }
                        cur_datapoint = cur_datapoint->next_dpoint;
                    }
                }
            }
        } else {
            node->rec->datapoint = datapoint;
        }
    } else {
        int new_data_quadrant = determineQuadrant(datapoint->loc, node->rec);
        switch (new_data_quadrant) {
            case SW:
                node->SW = validateNodeExistence(SW, node->SW, node);
                addPoint(datapoint, node->SW);
                break;
            case NW:
                node->NW = validateNodeExistence(NW, node->NW, node);
                addPoint(datapoint, node->NW);
                break;
            case NE:
                node->NE = validateNodeExistence(NE, node->NE, node);
                addPoint(datapoint, node->NE);
                break;
            case SE:
                node->SE = validateNodeExistence(SE, node->SE, node);
                addPoint(datapoint, node->SE);
                break;
        }
    }
}

/* validate the node existence, if it's not exist, we will initialise the node
* using the data from the rectangle2D of its parent node.
* (inspired from concept & visualization in 'Introduction to PR Quadtrees'
* and 'Implementation Details' section of the Assignment2)
*/
quadtreeNode_t *validateNodeExistence(int quadrant, quadtreeNode_t *check_node,
quadtreeNode_t *parent_node) {
    if (check_node == NULL) {
        quadtreeNode_t *new_node = quadtreeNodeCreate();
        assignNodeRecFromParent(quadrant, new_node->rec, parent_node->rec);
        return new_node;
    }
    return check_node;
}

/* validate the location of 2 datapoint_t object. If it's the same, we should
* store these 2 datapoint_t object as LinkedList (we cannot separately assign
* these 2 datapoint into 2 separate quadrant), else we can subdivide this node.
* (inspired from concept & visualization in 'Introduction to PR Quadtrees'
* and 'Implementation Details' section of the Assignment2)
*/
int validateSubDivide(datapoint_t *a, datapoint_t *b) {
    if ((a->loc->x == b->loc->x) && (a->loc->y == b->loc->y)) {
        return 0;
    }
    return 1;
}

/* subdivide the node, where we will create only a necessary sub-node.
* Then, set this node is_leaf to 0 (since this is not a leaf node anymore since
* we subdivide it) and re-assign the existing datapoint of the node into
* the sub-node.
* (inspired from concept & visualization in 'Introduction to PR Quadtrees'
* and 'Implementation Details' section of the Assignment2)
*/
void subDivideNode(quadtreeNode_t *node) {
    int new_quad_data_point = determineQuadrant(node->rec->datapoint->loc, 
    node->rec);
    quadtreeNode_t *sub_node = quadtreeNodeCreate();
    assignNodeRecFromParent(new_quad_data_point, sub_node->rec, node->rec);
    switch (new_quad_data_point) {
        case SW:
            node->SW = sub_node;
            break;
        case NW:
            node->NW = sub_node;
            break;
        case NE:
            node->NE = sub_node;
            break;
        case SE:
            node->SE = sub_node;
            break;
    }
    node->is_leafnode = 0;
    addPoint(node->rec->datapoint, node);
    node->rec->datapoint = NULL;
}

/* search the data(inside datapoint_t object) in the node using the pair of
* (lon, lat) reading from the query_file, printing the matching record
* to out_file and accessing quadrant while searching to stdout.
* (inspired from concept & visualization in 'Introduction to PR Quadtrees'
* and 'Implementation Details' section of the Assignment2)
*/
void searchDatapoint(point2D_t *search_point, quadtreeNode_t *root_node, 
FILE *out_file) {
    quadtreeNode_t *final_search_quadrant;
    datapoint_t *cur_datapoint;
    if (inRectangle(search_point, root_node->rec)) {
        final_search_quadrant = accessQuadrant(search_point, root_node);
        if (final_search_quadrant != NULL) {
            cur_datapoint = final_search_quadrant->rec->datapoint;
            while (cur_datapoint != NULL) {
                if (compareDataPoint2D(search_point, cur_datapoint->loc) == 1) {
                    dataPrintFinal(out_file, cur_datapoint->data);
                }
                cur_datapoint = cur_datapoint->next_dpoint;
            }
        } else {
            printf(" NOTFOUND");
        }
    } else {
        printf(" NOTFOUND");
    }
}

/* access the appropriate quadrant that this point2D should be in.
* return the appropriate quadrant(quadtreeNode_t object) if found, if that
* quadrant not exist, we return NULL.
* (inspired from concept & visualization in 'Introduction to PR Quadtrees'
* and 'Implementation Details' section of the Assignment2)
*/
quadtreeNode_t *accessQuadrant(point2D_t *search_point, quadtreeNode_t *node) {
    quadtreeNode_t *res_node;
    if ((node->is_leafnode) || (node == NULL)) {
        res_node = node;
    } else {
        int quadrant_to_access = determineQuadrant(search_point, node->rec);
        switch (quadrant_to_access) {
            case SW:
                printQuadrantText(quadrant_to_access);
                res_node = accessQuadrant(search_point, node->SW);
                break;
            case NW:
                printQuadrantText(quadrant_to_access);
                res_node = accessQuadrant(search_point, node->NW);
                break;
            case NE:
                printQuadrantText(quadrant_to_access);
                res_node = accessQuadrant(search_point, node->NE);
                break;
            case SE:
                printQuadrantText(quadrant_to_access);
                res_node = accessQuadrant(search_point, node->SE);
                break;
        }
    }
    return res_node;
}

/* search all the data(inside datapoint_t object) in the node which inside a
* rectangle2D object provided from the query file, printing all matching record
* to out_file without duplicate and the accessing quadrant while searching 
* to stdout.
* (inspired from concept & visualization in 'Introduction to PR Quadtrees'
* and 'Implementation Details' section of the Assignment2)
*/
void searchRectanglePoint(rectangle2D_t *search_rec, quadtreeNode_t *node,
array_t *arr, FILE *out_file) {
    if (node->is_leafnode) {
        if (node->rec->datapoint != NULL) {
            datapoint_t *cur_datapoint = node->rec->datapoint;
            while (cur_datapoint != NULL) {
                if (inRectangle(cur_datapoint->loc, search_rec) && 
                (dataGetIsInArray(cur_datapoint->data) == 0)) {
                    arrayInsertSorted(arr, cur_datapoint->data);
                    dataAssignIsInArray(1, cur_datapoint->data);
                }
                cur_datapoint = cur_datapoint->next_dpoint;
            }
        }
    } else {
        if ((node->SW != NULL) && rectangleOverlap(search_rec, node->SW->rec)) {
            printQuadrantText(SW);
            searchRectanglePoint(search_rec, node->SW, arr, out_file);
        }
        if ((node->NW != NULL) && rectangleOverlap(search_rec, node->NW->rec)) {
            printQuadrantText(NW);
            searchRectanglePoint(search_rec, node->NW, arr, out_file);
        }
        if ((node->NE != NULL) && rectangleOverlap(search_rec, node->NE->rec)) {
            printQuadrantText(NE);
            searchRectanglePoint(search_rec, node->NE, arr, out_file);
        }
        if ((node->SE != NULL) && rectangleOverlap(search_rec, node->SE->rec)) {
            printQuadrantText(SE);
            searchRectanglePoint(search_rec, node->SE, arr, out_file);
        }
    }
}

/* determine whether the point2D_t object is located inside this Rectangle2D
* object or not.
* return 1 if it's YES, else 0
* (inspired 'Implementation Details' section of the Assignment2)
*/
int inRectangle(point2D_t *point2D, rectangle2D_t *rectangle) {
    if (point2D->x > rectangle->bl_point->x 
    && point2D->x <= rectangle->tr_point->x
    && point2D->y >= rectangle->bl_point->y 
    && point2D->y < rectangle->tr_point->y) {
        return 1;
    }
    return 0;
}

/* determine whether the rec_a object is overlapping with rec_b object or not.
* return 1 if it's YES, else 0
* (inspired 'Implementation Details' section of the Assignment2)
*/
int rectangleOverlap(rectangle2D_t *rec_a, rectangle2D_t *rec_b) {
    if ((rec_a->tr_point->x < rec_b->bl_point->x) || 
    (rec_a->bl_point->x > rec_b->tr_point->x) ||
    (rec_a->tr_point->y < rec_b->bl_point->y) || 
    (rec_a->bl_point->y > rec_b->tr_point->y)) {
        return 0;
    }
    return 1;
}

/* determine the appropriate quadrant that this point2D_t object should be
* located in.
* return int value which refer to the appropriate quadrant
* (inspired 'Implementation Details' section of the Assignment2)
*/
int determineQuadrant(point2D_t *p, rectangle2D_t *rec) {
    if (p->x >= (rec->bl_point->x + rec->tr_point->x)/2.0) {
        if (p->y >= (rec->bl_point->y + rec->tr_point->y)/2.0) {
            return NE;
        } else {
            return SE;
        }
    } else {
        if (p->y >= (rec->bl_point->y + rec->tr_point->y)/2.0) {
            return NW;
        } else {
            return SW;
        }
    }
}

/* print the access quadrant as a text to stdout.
* (inspired 'Implementation Details' section of the Assignment2)
*/
void printQuadrantText(enum Quadrant q) {
    switch (q) {
        case SW:
            printf(" SW");
            break;
        case NW:
            printf(" NW");
            break;
        case NE:
            printf(" NE");
            break;
        case SE:
            printf(" SE");
            break;
    }
}

/* compare whether point_a & point_b are at the same location or not.
* return 1 if it's YES, else 0
* (inspired 'Implementation Details' section of the Assignment2 & ED discussion
* question number #640)
*/
int compareDataPoint2D(point2D_t *a, point2D_t *b) {
    if ((lfabs(a->x - b->x) < EPSILON) && (lfabs(a->y - b->y) < EPSILON)) {
        return 1;
    }
    return 0;
}

/* auxlilary function to convert the long double value as positive (similar to
* fabs() function but receive long double as an input)
* return positive long double value
* (code from ED discussion question number #640)
*/
long double lfabs(long double ld) {
    return (ld > 0) ? ld : -ld;
}

/* return data_t object inside datapoint_t object
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
data_t *getDataInDatapoint(datapoint_t *d_point) {
    return d_point->data;
}

/* return rectangle2D_t object inside quadtreeNode_t object
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
rectangle2D_t *getRectangle2DInNode(quadtreeNode_t *node) {
    return node->rec;
}

/* free memory used by quadtreeNode_t object
* (inspired from arrayFree(...) in W3.8 skeleton code)
*/
void quadtreeFree(quadtreeNode_t *node) {
    if (!node->is_leafnode) {
        if (node->SW != NULL) {
            quadtreeFree(node->SW);
        }
        if (node->NW != NULL) {
            quadtreeFree(node->NW);
        } 
        if (node->NE != NULL) {
            quadtreeFree(node->NE);
        } 
        if (node->SE != NULL) {
            quadtreeFree(node->SE);
        }
    }
    rectangleFree(node->rec);
    free(node);
}

/* free memory used by rectangle2D_t object
* (inspired from arrayFree(...) in W3.8 skeleton code)
*/
void rectangleFree(rectangle2D_t *rec) {
    free(rec->bl_point);
    free(rec->tr_point);
    if ((rec->datapoint) != NULL) {
        datapointFree(rec->datapoint);
    }
    free(rec);
}

/* free memory used by datapoint_t object
* (inspired from arrayFree(...) in W3.8 skeleton code)
*/
void datapointFree(datapoint_t *d_point) {
    datapoint_t *prev_datapoint;
    datapoint_t *cur_datapoint = d_point;
    while (cur_datapoint != NULL) {
        free(cur_datapoint->loc);
        if (cur_datapoint->is_start_point == 1) {
            dataFree(cur_datapoint->data);
        }
        prev_datapoint = cur_datapoint;
        cur_datapoint = cur_datapoint->next_dpoint;
        free(prev_datapoint);
    }
}