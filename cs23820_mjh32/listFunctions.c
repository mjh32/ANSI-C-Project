#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include "nodeFunctions.c"

// global variables to allow the handling of multiple linked lists at once
nodePointer *headers; // contains the first node for each linked list
size_t headerSize;
int currentHead = -1;

/**
 * Function to determine the size of headers array
 * @param numOfHeaders; number of headers
 */
void createHeaders(int numOfHeaders) {
    headerSize = numOfHeaders;
    headers = malloc(sizeof(nodePointer)*headerSize);
    if (headers == NULL) {
        exit(1);
    }
    memset(headers, 0, sizeof(headers));
}

/**
 * Function to print out all nodes within a linked list
 * @param listIndex; determines which list to print out
 */
void printList(int listIndex) {
    displayList(headers[listIndex]);
}

/**
 * Function to compare two pairs of coordinates between nodes
 * @param lastNode; first node to be compared
 * @param currentNode; second node to be compared
 * @return returns whether the coordinates are the same
 */
bool checkGPS(nodePointer lastNode, nodePointer currentNode) {
    if (lastNode->latitude == currentNode->latitude && lastNode->longitude == currentNode->longitude) {
        return false;
    } else {
        return true;
    }
}

/**
 * Function to create a linked list from data in a data file
 * @param fileName; string of the name of the data file
 * @param rangeLat; the range of the Latitude to be read in (currently unused)
 * @param rangeLon; the range of the Longitude to be read in (currently unused)
 */
void createList(char fileName[50], double rangeLat[2], double rangeLon[2]) {
    currentHead++; // iterates to the next linked list to be used
    // creating variables to be used in reading file lines and node creation
    char currentLine[100];
    double newLatitude;
    double newLongitude;
    time_t newTimePointer;
    struct tm times;
    nodePointer lastNode = NULL;
    nodePointer currentNode;
    FILE * fp = fopen(fileName, "r");
    if (fp == NULL) { // checks file exists
        printf("\nError: Data File does not exist\n");
        return;
    }
    // variables to keep track of the type of lines being read for the terminal output
    long totalLines = 0;
    long validLines = 0;
    long invalidLines = 0;
    long duplicateLines = 0;
    while (fgets(currentLine,100,fp)) {
        totalLines++;
        if (isdigit(currentLine[0])) { // all valid lines within the files will start with a number so using isdigit() will be able to determine a line's validity
            sscanf(currentLine, "%lf, %lf, %d/%d/%d, %d:%d:%d", &newLatitude, &newLongitude, &times.tm_mday, &times.tm_mon, &times.tm_year, &times.tm_hour, &times.tm_min, &times.tm_sec);
            /*if (newLatitude < rangeLat[0] || newLatitude > rangeLat[1]) {
                invalidLines++;
                continue;
            }
            if (newLongitude < rangeLon[0] || newLongitude > rangeLon[1]) {
                invalidLines++;
                continue;
            }*/ // the commented out lines of code were used to limit the range of the latitude and longitude according the configuration settings. the code sometimes caused a segmentation error and no cause was found thus it's unused
            validLines++;
            times.tm_mon = times.tm_mon - 1;
            newTimePointer = mktime(&times); // translates the tm struct variable to time_t for storage in the node
            currentNode = makeNode(newLatitude, newLongitude, newTimePointer);
            if (lastNode == NULL) { // in the case of the first line being read in so no comparisons are made
                lastNode = currentNode;
                continue;
            } else if (checkGPS(lastNode,currentNode)) { // once the gps coordinates have changed, the lastNode will be added to the list as it was the last one with matching coordinates
                if (headers[currentHead] == NULL) { // to add the first node to the linked list
                    lastNode->next = headers[currentHead];
                    headers[currentHead] = lastNode;
                } else {
                    insertAtTail(&headers[currentHead], lastNode);
                }
            } else {
                duplicateLines++;
            }
            lastNode = currentNode; // the last node is updated to be checked next time
        } else {
            invalidLines++;
        }
    }
    insertAtTail(&headers[currentHead], currentNode); // adds the last node created to the list as it will most likely not be added otherwise
    fclose(fp);
    // print the type of lines read and the number of times that it occurred to the terminal
    printf("\nDetails of Reading Lines from file %s:\n %ld total lines read\n %ld valid lines read\n %ld invalid lines read\n %ld duplicate coordinate lines read\n", fileName, totalLines, validLines, invalidLines, duplicateLines);
}

/**
 * Function to convert degree to radius, adapted from assignment specification
 * @param angleDegrees; the degrees to be converted
 * @return returns the radius
 */
double degToRad(double angleDegrees) {
    return ((angleDegrees) * M_PI / 180.0); // math.h was needed for this function to operate
}

/**
 * Function to calculate the distance between two points of the surface of the earth, adapted from the assignment specification
 * @param first; one node with latitude and longitude coordinates
 * @param second; second node with latitude and longitude coordinates
 * @return returns the distance between them in meters
 */
double calculateDistance(nodePointer first, nodePointer second) {
    double lon1 = degToRad(first->longitude);
    double lon2 = degToRad(second->longitude);
    double lat1 = degToRad(first->latitude);
    double lat2 = degToRad(second->latitude);
    double diffLon = lon2 - lon1;
    double diffLat = lat2 - lat1;
    double a = pow(sin(diffLat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(diffLon / 2),2); // CMakeLists.txt needed the line "target_link_libraries(cs23820_mjh32 m)" in order for the math to work here
    double c = 2 * asin(sqrt(a));
    double r = 6371;
    return((c * r)*1000);
}

/**
 * Function to run calculation using data from a linked list and print results to a csv file
 * @param listIndex; determines which list to use
 * @param fileName; string of the file to print to
 * @param maxSpeed; maximum speed allowed to be printed
 * @param moveMax; the range of movements allowed to be printed
 */
void listToFile(int listIndex, char fileName[50], double maxSpeed, double moveMax[2]) {
    nodePointer currentNode = headers[listIndex];
    long itemNum = -1;
    // variables to keep track of the type of behaviour being recorded for the terminal output
    double totalSta = 0;
    double totalFor = 0;
    double totalMov = 0;
    double totalImp = 0;
    FILE * fp = fopen(fileName, "w");
    if (fp == NULL) { // checks the file can be made
        printf("\nError: Can not create Data File\n");
        return;
    }
    while (currentNode->next != NULL) { // loops through each node in a list
        nodePointer first = currentNode;
        nodePointer second = currentNode->next; // two nodes will be compared for each entry in the csv file
        // following code calculates data needed for the csv file such as item number, distance, time, speed
        itemNum++;
        char buffer[19];
        struct tm *times = localtime(&first->timePoint);
        strftime(buffer, sizeof(buffer), "%d/%m/%y, %H:%M:%S", times); // formatting the time data
        double distance = calculateDistance(first, second);
        double timeTaken = difftime(second->timePoint, first->timePoint);
        if (timeTaken < moveMax[0] || timeTaken > moveMax[1]) { // working code to test whether the movement is within range
            currentNode = currentNode->next;
            continue;
        }
        double speed = distance/timeTaken;
        char behaviour[12];
        // matches speed to behaviour categories and stops any passing the maximum allowed speed
        if (speed >= 0 && speed < 0.02) {
            strcpy(behaviour,"Stationary");
            totalSta+=speed;
        } else if (speed >= 0.02 && speed < 0.33) {
            strcpy(behaviour,"Foraging");
            totalFor+=speed;
        } else if (speed >= 0.33 && speed < 12.00) {
            strcpy(behaviour,"Moving");
            totalMov+=speed;
        } else if (speed >= 12.00 && speed <= maxSpeed) {
            strcpy(behaviour,"Implausible");
            totalImp+=speed;
        } else if (speed > maxSpeed) {
            currentNode = currentNode->next;
            continue;
        }
        // prints data in correct format to the csv file
        fprintf(fp, "Movement:%ld, %s, %.6f, %.6f, %.6f, %.6f, %.3fm, %.1fs, %.3fm/s, %s\n", itemNum, buffer, first->longitude, first->latitude, second->longitude, second->latitude, distance, timeTaken, speed, behaviour);
        currentNode = currentNode->next;
    }
    fclose(fp);
    // prints the number of occurrences for each behaviour profile
    printf("\nTotal time in Behaviour Categories for sheep in %s:\n %.1fs Stationary\n %.1fs Foraging\n %.1fs Moving\n %.1fs Implausible\n", fileName, totalSta, totalFor, totalMov, totalImp);
}
