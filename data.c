/* data.c
*
* Created by Supakorn Sumongkol (ssumongkol@student.unimelb.edu.au) 07/09/2022
*
* contain data structure 'data' and functions logic related to 'data'.
* 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"

struct data {
	int footpath_id;
	char *address;
	char *clue_sa;
	char *asset_type;
	double deltaz;
	double distance;
	double grade1in;
	int mcc_id;
	int mccid_int;
	double rlmax;
	double rlmin;
	char *segside;
	int statusid;
	int streetid;
	int street_group;
	double start_lat;
	double start_lon;
	double end_lat;
	double end_lon;
	int is_in_array;
};

/* skip the header line of in_file(.csv)
* (code from W3.8 skeleton code)
*/
void dataSkipHeaderLine(FILE *in_file) {
	while (fgetc(in_file) != '\n');
}

/* reads a data from in_file(.csv) to build a data_t d
* using fscanf(File *in_file) & fgetc(File *in_file) 
* while keep track of the special case for the String
* returns the pointer to data_t d, or NULL if reading is unsuccessful.
* (inspired from studentRead(...) in W3.8 skeleton code)
*/
data_t *dataRead(FILE *in_file) {
	data_t *d = malloc(sizeof(*d));
	assert(d);
	char *temp_var = (char *) malloc((MAX_STRING_LEN + 1) * sizeof(char));
	assert(temp_var);
	char c;
	int var_pointer = 1;

	while (var_pointer <= MAX_FIELD_READ) {
		if ((var_pointer < 2 || var_pointer > 4) && (var_pointer != 12)) {
			if (var_pointer == MAX_FIELD_READ) {
				fscanf(in_file, "%[^\n]\n", temp_var);
			} else {
				fscanf(in_file, "%[^,],", temp_var);
			}
			assignValueData(d, temp_var, &var_pointer);
		} else {
			c = fgetc(in_file);
			if (c == ',') {
				strcpy(temp_var, "\0");
			} else if (c == '\"') {
				fscanf(in_file, "%[^\"]\",", temp_var);
			} else {
				ungetc(c, in_file);
				fscanf(in_file, "%[^,],", temp_var);
			}
			assignValueData(d, temp_var, &var_pointer);
		}
	}
	d->is_in_array = 0;
	free(temp_var);
	return d;
}

/* assign value to the correct variable inside struct data_t
* by tracking var_pointer(pointer specify which variable in data_t to add)
* (inspired from studentRead(...) in W3.8 skeleton code)
*/
void assignValueData(data_t *d, char* value, int *index) {
	char *ptr;

	switch (*index) {
		case 1:
			d->footpath_id = atoi(value);
			break;
		case 2:
			d->address = strdup(value);
			assert(d->address);
			break;
		case 3:
			d->clue_sa = strdup(value);
			assert(d->clue_sa);
			break;
		case 4:
			d->asset_type = strdup(value);
			assert(d->asset_type);
			break;
		case 5:
			d->deltaz = strtod(value, &ptr);
			break;
		case 6:
			d->distance = strtod(value, &ptr);
			break;
		case 7:
			d->grade1in = strtod(value, &ptr);
			break;
		case 8:
			d->mcc_id = atoi(value);
			break;
		case 9:
			d->mccid_int = atoi(value);
			break;
		case 10:
			d->rlmax = strtod(value, &ptr);
			break;
		case 11:
			d->rlmin = strtod(value, &ptr);
			break;
		case 12:
			d->segside = strdup(value);
			assert(d->segside);
			break;
		case 13:
			d->statusid = atoi(value);
			break;
		case 14:
			d->streetid = atoi(value);
			break;
		case 15:
			d->street_group = atoi(value);
			break;
		case 16:
			d->start_lat = strtod(value, &ptr);
			break;
		case 17:
			d->start_lon = strtod(value, &ptr);
			break;
		case 18:
			d->end_lat = strtod(value, &ptr);
			break;
		case 19:
			d->end_lon = strtod(value, &ptr);
			break;
		
		default:
			fprintf(stderr, "Error: assignValueData(...) in data.c, "
			"out of SWITCH CASE index range!!!\n");
			fprintf(stderr, "index: %d\n", *index);
	}
	(*index)++;
}

/* prints a data stored in specific node to out_file(.txt file)
* for debugging purpose only
* (inspired from studentPrint(...) in W3.8 skeleton code)
*/
void dataPrint(FILE *out_file, data_t *d) {
	fprintf(out_file, "%d,%s,%s,%s,%.6lf,%.6lf,%.6lf,%d,%d,%.6lf,%.6lf,%s,%d,"
	"%d,%d,%.6lf,%.6lf,%.6lf,%.6lf\n", d->footpath_id, d->address, d->clue_sa, 
	d->asset_type, d->deltaz, d->distance, d->grade1in, d->mcc_id, d->mccid_int, 
	d->rlmax, d->rlmin, d->segside, d->statusid, d->streetid, d->street_group, 
	d->start_lat, d->start_lon, d->end_lat, d->end_lon);
}

/* print data stored in specific node to out_file(.txt file) in desired format
* (inspired from studentPrint(...) in W3.8 skeleton code)
*/
void dataPrintFinal(FILE *out_file, data_t *d) {
	fprintf(out_file, "--> footpath_id: %d || address: %s || clue_sa: %s || "
	"asset_type: %s || deltaz: %.2lf || distance: %.2lf || grade1in: %.1lf || "
	"mcc_id: %d || mccid_int: %d || rlmax: %.2lf || rlmin: %.2lf || "
	"segside: %s || statusid: %d || streetid: %d || street_group: %d || "
	"start_lat: %.6lf || start_lon: %.6lf || end_lat: %.6lf || "
	"end_lon: %.6lf ||\n", d->footpath_id, d->address, d->clue_sa, 
	d->asset_type, d->deltaz, d->distance, d->grade1in, d->mcc_id, 
	d->mccid_int, d->rlmax, d->rlmin, d->segside, d->statusid, d->streetid, 
	d->street_group, d->start_lat, d->start_lon, d->end_lat, d->end_lon);
} 

/* free the memory allocated to a data_t "d"
* (inspired from studentFree(...) in W3.8 skeleton code)
*/
void dataFree(data_t *d) {
	free(d->address);
	free(d->clue_sa);
	free(d->asset_type);
	free(d->segside);
	free(d);
}

/* return address in data
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
char *dataGetAddress(data_t *d) {
	return d->address;
}

/* return grade1in in data
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
double dataGetGrade1in(data_t *d) {
	return d->grade1in;
}

/* return start_lon in data
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
double dataGetStartLon(data_t *d) {
	return d->start_lon;
}

/* return start_lat in data
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
double dataGetStartLat(data_t *d) {
	return d->start_lat;
}

/* return end_lon in data
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
double dataGetEndLon(data_t *d) {
	return d->end_lon;
}

/* return end_lat in data
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
double dataGetEndLat(data_t *d) {
	return d->end_lat;
}

/* return footpath_id in data
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
double dataGetFootpathID(data_t *d) {
	return d->footpath_id;
}

/* return is_in_array in data
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
int dataGetIsInArray(data_t *d) {
	return d->is_in_array;
}

/* set is_in_array value according to the input value
* (inspired from studentGetID(...) in W3.8 skeleton code)
*/
void dataAssignIsInArray(int val, data_t *d) {
	d->is_in_array = val;
}