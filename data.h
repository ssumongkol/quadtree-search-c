/* data.h
*
* Created by Supakorn Sumongkol (ssumongkol@student.unimelb.edu.au) 15/08/2022
*
* the inferface of module 'data' of this project
* 
*/

#ifndef _DATA_H_
#define _DATA_H_
#include <stdio.h>

// constant defnitions
#define MAX_STRING_LEN 128
#define MAX_FIELD_READ 19

// data definitions
typedef struct data data_t;

/* skip the header line of in_file(.csv)
* (code from W3.8 skeleton code)
*/
void dataSkipHeaderLine(FILE *in_file);

/* reads a data from in_file(.csv) to build a data_t d
* using fscanf(File *in_file) & fgetc(File *in_file) 
* while keep track of the special case for the String
* returns the pointer to data_t d, or NULL if reading is unsuccessful.
* (inspired from studentRead(...) in W3.8 skeleton code)
*/
data_t *dataRead(FILE *f);

/* assign value to the correct variable inside struct data_t
* by tracking var_pointer(pointer specify which variable in data_t to add)
* (inspired from studentRead(...) in W3.8 skeleton code)
*/
void assignValueData(data_t *d, char* value, int *index);

/* prints a data stored in specific node to out_file(.txt file)
* for debugging purpose only
* (inspired from studentPrint(...) in W3.8 skeleton code)
*/
void dataPrint(FILE *f, data_t *d);

/* print data stored in specific node to out_file(.txt file) in desired format
* (inspired from studentPrint(...) in W3.8 skeleton code)
*/
void dataPrintFinal(FILE *out_file, data_t *d);

/* free the memory allocated to a data_t "d"
* (inspired from studentFree(...) in W3.8 skeleton code)
*/
void dataFree(data_t *d);

/* return address in data
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
char *dataGetAddress(data_t *d);

/* return grade1in in data
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
double dataGetGrade1in(data_t *d);

/* return start_lon in data
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
double dataGetStartLon(data_t *d);

/* return start_lat in data
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
double dataGetStartLat(data_t *d);

/* return end_lon in data
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
double dataGetEndLon(data_t *d);

/* return end_lat in data
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
double dataGetEndLat(data_t *d);

/* return footpath_id in data
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
double dataGetFootpathID(data_t *d);

/* return is_in_array in data
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
int dataGetIsInArray(data_t *d);

/* set is_in_array value according to the input value
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
void dataAssignIsInArray(int val, data_t *d);

#endif
