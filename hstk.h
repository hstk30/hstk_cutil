#ifndef HSTK_H_
#define HSTK_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define TRUE 1
#define FALSE 0


void hstk_error_msg(char *msg);

void *Malloc(size_t size);
void *Realloc(void *ptr, size_t size);
void *Calloc(size_t number, size_t itemsize);
void Free(void *ptr);


#endif /* ifndef HSTK_H_ */
