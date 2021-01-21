#include <time.h>

/**
 * Defines the structure of a node in a linked list, adapted from code given in lecture 13
 */
typedef struct nodeTag * nodePointer;
typedef struct nodeTag {
    double latitude;
    double longitude;
    time_t timePoint;
    nodePointer next;
} node;