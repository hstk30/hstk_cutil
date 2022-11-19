#ifndef HSTK_H_
#define HSTK_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

typedef char bool;
#define TRUE 1
#define FALSE 0

/* 0 mean function call success, 1 mean fail */
typedef int ret_flag;

void hstk_error_msg(char *msg);

void *Malloc(size_t size);
void *Realloc(void *ptr, size_t size);
void *Calloc(size_t number, size_t itemsize);
void Free(void *ptr);


#endif /* ifndef HSTK_H_ */
