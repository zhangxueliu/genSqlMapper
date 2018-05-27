#ifndef __COL_FIELD_H__
#define __COL_FIELD_H__

typedef struct {
	char columnName[255];
	char type[50];
	char nullable[2];
	char defaultValue[50];
	char comments[1024];
} colField_t;

#endif
