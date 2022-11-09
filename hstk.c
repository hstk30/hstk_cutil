#include "hstk.h"


void hstk_error_msg(char *msg)
{
    fprintf(stderr, "Error: %s [%s]\n", msg, strerror(errno));
    exit(0);
}

void hstk_warn_msg(char *msg)
{
    fprintf(stderr, "Warn: %s\n", msg);
}

void *Malloc(size_t size)
{
    void *p = malloc(size);

    if (p == NULL)
		hstk_error_msg("Malloc error");
    return p;
}

void *Realloc(void *ptr, size_t size)
{
    void *p = realloc(ptr, size);

    if (p == NULL)
		hstk_error_msg("Realloc error");
    return p;
}

void *Calloc(size_t number, size_t itemsize)
{
    void *p = calloc(number, itemsize);

    if (p == NULL)
		hstk_error_msg("Calloc error");
    return p;
}

void Free(void *ptr)
{
    free(ptr);
}
