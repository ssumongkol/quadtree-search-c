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
    datapoint_t *next_datapoint;
};

struct point2D {
    double x;
    double y;
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
    point->next_datapoint = NULL;
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

/* assign bottomleft or topright (lon, lat) value to a root node of quadtree
* (inspired from assignDataToNode(...) in my Assignment1 code)
*/
void assignRootNodeBoundary(int is_bl, long double x, long double y, 
quadtreeNode_t *node) {
    if (is_bl == 1) {
        node->rec->bl_point->x = x;
        node->rec->bl_point->y = y;
    } else if (is_bl == 0) {
        node->rec->tr_point->x = x;
        node->rec->tr_point->y = y;
    }
}

/* assign bottomleft & topright point in rectangle2D of child_node using
* bottomleft & topright point from parent_node
* (inspired from concept & visualization in 'Introduction to PR Quadtrees'
* section of the Assignment2)
*/
void assignNodeRecFromParent(int assign_quadrant, quadtreeNode_t *child_node,
quadtreeNode_t *parent_node) {
    switch (assign_quadrant) {
        case SW:
            child_node->rec->bl_point->x = parent_node->rec->bl_point->x;
            child_node->rec->bl_point->y = parent_node->rec->bl_point->y;
            child_node->rec->tr_point->x = (parent_node->rec->bl_point->x + parent_node->rec->tr_point->x) / 2;
            child_node->rec->tr_point->y = (parent_node->rec->bl_point->y + parent_node->rec->tr_point->y) / 2;
            break;
        case NW:
            child_node->rec->bl_point->x = parent_node->rec->bl_point->x;
            child_node->rec->bl_point->y = (parent_node->rec->bl_point->y + parent_node->rec->tr_point->y) / 2;
            child_node->rec->tr_point->x = (parent_node->rec->bl_point->x + parent_node->rec->tr_point->x) / 2;
            child_node->rec->tr_point->y = parent_node->rec->tr_point->y;
            break;
        case NE:
            child_node->rec->bl_point->x = (parent_node->rec->bl_point->x + parent_node->rec->tr_point->x) / 2;
            child_node->rec->bl_point->y = (parent_node->rec->bl_point->y + parent_node->rec->tr_point->y) / 2;
            child_node->rec->tr_point->x = parent_node->rec->tr_point->x;
            child_node->rec->tr_point->y = parent_node->rec->tr_point->y;
            break;
        case SE:
            child_node->rec->bl_point->x = (parent_node->rec->bl_point->x + parent_node->rec->tr_point->x) / 2;
            child_node->rec->bl_point->y = parent_node->rec->bl_point->y;
            child_node->rec->tr_point->x = parent_node->rec->tr_point->x;
            child_node->rec->tr_point->y = (parent_node->rec->bl_point->y + parent_node->rec->tr_point->y) / 2;
            break;
    }
}

void assignDataToQuadtree(data_t *data, quadtreeNode_t *node) {
    datapoint_t *new_datapoint1 = datapointCreate();
    datapoint_t *new_datapoint2 = datapointCreate();
    new_datapoint1->data = data;
    new_datapoint2->data = data;
    new_datapoint1->loc->x = dataGetStartLon(data);
    new_datapoint1->loc->y = dataGetStartLat(data);
    new_datapoint1->is_start_point = 1;
    new_datapoint2->loc->x = dataGetEndLon(data);
    new_datapoint2->loc->y = dataGetEndLat(data);
    new_datapoint2->is_start_point = 0;
    addPoint(new_datapoint1, node);
    addPoint(new_datapoint2, node);
}

void addPoint(datapoint_t *datapoint, quadtreeNode_t *node) {
    if (node->is_leafnode) {
        if (node->rec->datapoint != NULL) {
            if (validateSubDivide(datapoint, node->rec->datapoint)) {
                subDivideNode(node);
                addPoint(datapoint, node);
            } else {
                datapoint_t *cur_datapoint = node->rec->datapoint;
                if (dataGetFootpathID(datapoint->data) <= dataGetFootpathID(cur_datapoint->data)) {
                    datapoint->next_datapoint = cur_datapoint;
                    node->rec->datapoint = datapoint;
                } else {
                    while (1) {
                        if (cur_datapoint->next_datapoint == NULL) {
                            cur_datapoint->next_datapoint = datapoint;
                            break;
                        }
                        if (dataGetFootpathID(datapoint->data) <= dataGetFootpathID(cur_datapoint->next_datapoint->data)) {
                            datapoint->next_datapoint = cur_datapoint->next_datapoint;
                            cur_datapoint->next_datapoint = datapoint;
                            break;
                        }
                        cur_datapoint = cur_datapoint->next_datapoint;
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
                if (node->SW == NULL) {
                    node->SW = quadtreeNodeCreate();
                    assignNodeRecFromParent(SW, node->SW, node);
                }
                addPoint(datapoint, node->SW);
                break;
            case NW:
                if (node->NW == NULL) {
                    node->NW = quadtreeNodeCreate();
                    assignNodeRecFromParent(NW, node->NW, node);
                }
                addPoint(datapoint, node->NW);
                break;
            case NE:
                if (node->NE == NULL) {
                    node->NE = quadtreeNodeCreate();
                    assignNodeRecFromParent(NE, node->NE, node);
                }
                addPoint(datapoint, node->NE);
                break;
            case SE:
                if (node->SE == NULL) {
                    node->SE = quadtreeNodeCreate();
                    assignNodeRecFromParent(SE, node->SE, node);
                }
                addPoint(datapoint, node->SE);
                break;
        }
    }
}

int validateSubDivide(datapoint_t *a, datapoint_t *b) {
    if ((a->loc->x == b->loc->x) && (a->loc->y == b->loc->y)) {
        return 0;
    }
    return 1;
}

void subDivideNode(quadtreeNode_t *node) {
    int new_quad_data_point = determineQuadrant(node->rec->datapoint->loc, node->rec);
    quadtreeNode_t *sub_node = quadtreeNodeCreate();
    assignNodeRecFromParent(new_quad_data_point, sub_node, node);
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

int inRectangle(point2D_t *point2D, rectangle2D_t *rectangle) {
    if (point2D->x >= rectangle->bl_point->x 
    && point2D->x <= rectangle->tr_point->x
    && point2D->y >= rectangle->bl_point->y 
    && point2D->y <= rectangle->tr_point->y) {
        return 1;
    }
    return 0;
}

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
    return -1;
}

void searchDatapoint(double search_lon, double search_lat,
quadtreeNode_t *root_node, FILE *out_file) {
    quadtreeNode_t *final_search_quadrant;
    datapoint_t *cur_datapoint;
    point2D_t *search_point = point2DCreate();
    search_point->x = search_lon;
    search_point->y = search_lat;
    if (inRectangle(search_point, root_node->rec)) {
        final_search_quadrant = accessQuadrant(search_point, root_node);
        if (final_search_quadrant != NULL) {
            printf("\n");
            cur_datapoint = final_search_quadrant->rec->datapoint;
            while (cur_datapoint != NULL) {
                if (compareDataPoint2D(search_point, cur_datapoint->loc) == 1) {
                    dataPrintFinal(out_file, cur_datapoint->data);
                }
                cur_datapoint = cur_datapoint->next_datapoint;
            }
        } else {
            printf(" NOTFOUND\n");
        }
    } else {
        printf(" NOTFOUND\n");
    }
    free(search_point);
}

quadtreeNode_t *accessQuadrant(point2D_t *search_point, quadtreeNode_t *node) {
    if (node->is_leafnode) {
        return node;
    } else {
        int quadrant_to_access = determineQuadrant(search_point, node->rec);
        switch (quadrant_to_access) {
            case SW:
                printQuadrantText(quadrant_to_access);
                if (node->SW != NULL) {
                    accessQuadrant(search_point, node->SW);
                } else {
                    return NULL;
                }
                break;
            case NW:
                printQuadrantText(quadrant_to_access);
                if (node->NW != NULL) {
                    accessQuadrant(search_point, node->NW);
                } else {
                    return NULL;
                }
                break;
            case NE:
                printQuadrantText(quadrant_to_access);
                if (node->NE != NULL) {
                    accessQuadrant(search_point, node->NE);
                } else {
                    return NULL;
                }
                break;
            case SE:
                printQuadrantText(quadrant_to_access);
                if (node->SE != NULL) {
                    accessQuadrant(search_point, node->SE);
                } else {
                    return NULL;
                }
                break;
        }
    }
}

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

int compareDataPoint2D(point2D_t *a, point2D_t *b) {
    if ((lfabs(a->x - b->x) < EPSILON) && (lfabs(a->y - b->y) < EPSILON)) {
        return 1;
    }
    return 0;
}

long double lfabs(long double ld) {
    return (ld > 0) ? ld : -ld;
 }







void assignBLpointRec(rectangle2D_t *rec, long double x, long double y) {
    rec->bl_point->x = x;
    rec->bl_point->y = y;
}

void assignTRpointRec(rectangle2D_t *rec, long double x, long double y) {
    rec->tr_point->x = x;
    rec->tr_point->y = y;
}

// not sure
int rectangleOverlap(rectangle2D_t *rec_a, rectangle2D_t *rec_b) {
    if ((rec_a->tr_point->x < rec_b->bl_point->x) || 
    (rec_a->bl_point->x > rec_b->tr_point->x) ||
    (rec_a->tr_point->y < rec_b->bl_point->y) || 
    (rec_a->bl_point->y > rec_b->tr_point->y)) {
        return 0;
    }
    return 1;
}

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
                cur_datapoint = cur_datapoint->next_datapoint;
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

data_t *getDataInDatapoint(datapoint_t *d_point) {
    return d_point->data;
}












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

void rectangleFree(rectangle2D_t *rec) {
    free(rec->bl_point);
    free(rec->tr_point);
    if ((rec->datapoint) != NULL) {
        datapointFree(rec->datapoint);
    }
    free(rec);
}

void datapointFree(datapoint_t *d_point) {
    datapoint_t *prev_datapoint;
    datapoint_t *cur_datapoint = d_point;
    while (cur_datapoint != NULL) {
        free(cur_datapoint->loc);
        if (cur_datapoint->is_start_point == 1) {
            dataFree(cur_datapoint->data);
        }
        prev_datapoint = cur_datapoint;
        cur_datapoint = cur_datapoint->next_datapoint;
        free(prev_datapoint);
    }
}











// /* creates & returns an empty node
// * (inspired from arrayCreate(...) in W3.8 skeleton code)
// */
// linkedListNode_t *nodeCreate() {
// 	linkedListNode_t *node = malloc(sizeof(*node));
// 	assert(node);
// 	node->data = NULL;
// 	node->next = NULL;
// 	return node;
// }

// /* assign data_t element into node(linkedListNode)
// * (inspired from studentGetID(...) in W3.8 skeleton code)
// */
// void assignDataToNode(linkedListNode_t *node, data_t *d) {
// 	node->data = d;
// }

// /* assign pointer to the next node(linkedListNode) into node
// * (inspired from studentGetID(...) in W3.8 skeleton code)
// */
// void assignNextToNode(linkedListNode_t *current_node, 
// linkedListNode_t *nextNode) {
// 	current_node->next = nextNode;
// }

// /* get node->data value
// * return (data_t) node->data
// * (inspired from studentGetID(...) in W3.8 skeleton code)
// */
// data_t *getDataInNode(linkedListNode_t *node) {
// 	return node->data;
// }

// /* get node->next value
// * return (linkedListNode_t) node->next
// * (inspired from studentGetID(...) in W3.8 skeleton code)
// */
// linkedListNode_t *getNextNode(linkedListNode_t *node) {
// 	return node->next;
// }

// /* get node->data->address value
// * return (char *) node->data->address
// * (inspired from studentGetID(...) in W3.8 skeleton code)
// */
// char *getAddressInNode(linkedListNode_t *node) {
// 	return dataGetAddress(node->data);
// }

// /* get node->data->grade1in value
// * return (dobule) node->data->grade1in
// * (inspired from studentGetID(...) in W3.8 skeleton code)
// */
// double getGrade1inInNode(linkedListNode_t *node) {
// 	return dataGetGrade1in(node->data);
// }

// // validate whether node->next is NULL or not?
// int validateNextNode(linkedListNode_t *node) {
// 	if (node->next != NULL) {
// 		return 1;
// 	} else {
// 		return 0;
// 	}
// }

// /* print data in node to output_file(.txt) starting from the 1st to last node
// * for debugging purpose only
// * (inspired from arrayPrint(...) in W3.8 skeleton code)
// */
// void dictionaryPrint(FILE *out_file, linkedListNode_t *head_node) {
// 	linkedListNode_t *current_node = head_node;

// 	while (current_node->next != NULL) {
// 		dataPrint(out_file, current_node->data);
// 		current_node = current_node->next;
// 	}
// 	dataPrint(out_file, current_node->data);
// }

// /* free memory used by linkedListNode_t "node" from 1st to last node
// * (inspired from arrayFree(...) in W3.8 skeleton code)
// */
// void dictionaryFree(linkedListNode_t *head_node) {
// 	linkedListNode_t *current_node, *prev_node;
// 	current_node = head_node;

// 	while (current_node != NULL) {
// 		dataFree(current_node->data);
// 		prev_node = current_node;
// 		current_node = current_node->next;
// 		free(prev_node);
// 	}
// }
