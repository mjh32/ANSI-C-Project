#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// most functions were made by adapting the linked list code given from lecture 13

/**
 * Function to create a node for a linked list, adapted from code given in lecture 13
 * @param newLatitude; the latitude to be stored
 * @param newLongitude; the longitude to be stored
 * @param newTimePointer; the time value to be stored
 * @return returns the created node
 */
nodePointer makeNode(double newLatitude, double newLongitude, time_t newTimePointer) {
    nodePointer newNodePtr = malloc(sizeof(node));
    if (newNodePtr == NULL) {
        return NULL;
    } else {
        newNodePtr->latitude = newLatitude;
        newNodePtr->longitude = newLongitude;
        newNodePtr->timePoint = newTimePointer;
        newNodePtr->next = NULL; // left at next pointer at NULL so it can be added to a list by other parts of the program
        return newNodePtr;
    }
}

/**
 * Function to insert a node at the beginning of a linked list, adapted from code given in lecture 13
 * @param ptrToExistingHead; pointer to the current head node of the linked list
 * @param newNodePtr; node to be the new head
 */
void insertAtTop(nodePointer * ptrToExistingHead, nodePointer newNodePtr) {
    newNodePtr->next = *ptrToExistingHead;
    *ptrToExistingHead = newNodePtr;
}

/**
 * Function to insert a node at the end of a linked list, adapted from code given in lecture 13
 * @param ptrToExistingHead; pointer to the head node of the linked list
 * @param newNodePtr; node to be inserted at the end of the list
 */
void insertAtTail(nodePointer * ptrToExistingHead, nodePointer newNodePtr) {
    nodePointer * tempPtr = ptrToExistingHead;
    while (*tempPtr != NULL) {
        tempPtr = &((*tempPtr)->next); // integrates through the list till last node is found
    }
    newNodePtr->next = *tempPtr;
    *tempPtr = newNodePtr;
}

/**
 * Function to display all nodes in a linked list, adapted from code given in lecture 13
 * @param listPtr; pointer to the current node being printed
 */
void displayList(nodePointer listPtr) {
    printf("The nodes in the list are as follows.\n");
    while (listPtr != NULL){
        char buffer[19];
        struct tm *times = localtime(&listPtr->timePoint); //translates time_t back to tm struct for printing
        strftime(buffer, sizeof(buffer), "%d/%m/%y, %H:%M:%S", times);
        printf("node data is %.6f, %.6f, %s\n", listPtr->latitude, listPtr->longitude, buffer);
        listPtr = listPtr->next;
    }
}
