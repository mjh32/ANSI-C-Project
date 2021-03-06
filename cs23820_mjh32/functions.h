double degToRad(double angleDegrees);
double calculateDistance(nodePointer first, nodePointer second);
nodePointer makeNode(double newLatitude, double newLongitude, time_t newTimePointer);
void insertAtTop(nodePointer * ptrToExistingHead, nodePointer newNodePtr);
void insertAtTail(nodePointer * ptrToExistingHead, nodePointer newNodePtr);
void displayList(nodePointer listPtr);
void createHeaders(int numOfHeaders);
void printList(int listIndex);
bool checkGPS(nodePointer lastNode, nodePointer currentNode);
void createList(char fileName[50], double rangeLat[2], double rangeLon[2]);
void listToFile(int listIndex, char fileName[50], double maxSpeed, double moveMax[2]);
void cleanBuffer();
void scanFile(char theFileName[50]);
void load(int lists, char*configFiles[]);
void tempSettings();
void saveSettings(char currentFileName[50]);
int main(int argc, char *argv[]);