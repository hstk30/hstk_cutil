/* Some hash function
 * http://www.cse.yorku.ca/~oz/hash.html 
 */
#include<stdio.h>

unsigned long 
djb2_hash(char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) 
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}


unsigned long 
djb2_hash_xor(char *str) {
	unsigned long hash = 5381;
    int c;

    while ((c = *str++)) 
        hash = ((hash << 5) + hash) ^ c; /* hash * 33 ^ c */
	return hash;
}


unsigned long
sdbm_hash(char *str)
{
    unsigned long hash = 0;
    int c;

    while ((c = *str++))
        // hash * 65599 + c 
        hash = c + (hash << 6) + (hash << 16) - hash; 

    return hash;
}

