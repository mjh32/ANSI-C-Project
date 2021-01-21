#include <stdio.h>
#include "header.c"
#include "listFunctions.c"
#include "functions.h"

// declaring global variables to store the configuration information, initialised to their default values.
char dataFileName[50] = "NULL";
double movementDuration[2] = {0, 604800};
double maximumSpeed = 50;
int numBars = 20;
double rangeLong[2] = {0, 0};
double rangeLat[2] = {0, 0};

/**
 * Function used to stop input looping, adapted from code found at https://bit.ly/3a3ps3n
 */
void cleanBuffer() {
    int n;
    while((n = getchar()) != EOF && n != '\n' );
}

/**
 * Function to read in the data from a configuration file
 * @param theFileName; string of the file name
 */
void scanFile(char theFileName[50]) {
    FILE *fp = fopen(theFileName, "r");
    if (fp == NULL) { // checking file does exist
        printf("\nError: Config File does not exist\n");
        return;
    }
    char currentLine[100];
    // scanning through each line and extracting the data
    fgets(currentLine, 100, fp);
    sscanf(currentLine, "data_file_name \"%[^\"]\"", dataFileName);
    fgets(currentLine, 100, fp);
    sscanf(currentLine, "movement_duration <%lf : %lf>", &movementDuration[0], &movementDuration[1]);
    fgets(currentLine, 100, fp);
    sscanf(currentLine, "maximum_speed %lf", &maximumSpeed);
    fgets(currentLine, 100, fp);
    sscanf(currentLine, "num_bars %d", &numBars);
    fgets(currentLine, 100, fp);
    sscanf(currentLine, "range_long < %lf : %lf >", &rangeLong[0], &rangeLong[1]);
    fgets(currentLine, 100, fp);
    sscanf(currentLine, "range_lat < %lf : %lf >", &rangeLat[0], &rangeLat[1]);
    fclose(fp);
    printf("\nConfiguration File successfully loaded in\n");
}

/**
 * Function that uses data from current configuration to create a linked list from data file and then creates a csv file output
 * @param lists; the number of config files entered
 * @param configFiles; array of strings of the file names
 */
void load(int lists, char*configFiles[]) {
    createHeaders((lists - 1)); // declaring the number of linked lists that will be used
    for (int i = 1; i < lists; i++) {
        if (strcmp(configFiles[1],"-1") != 0) {
            scanFile(configFiles[i]); // if a configuration file is present then the values are loaded in
        }
        if (strcmp(dataFileName,"NULL") == 0) { // check to see whether data file has been specified
            printf("\nError: No Data File specified\n");
            return;
        }
        createList(dataFileName, rangeLat, rangeLong); // creates the linked list from the data file
        char * outputFileName = strtok(dataFileName, ".");
        strcat(outputFileName, ".csv");
        listToFile((i-1), outputFileName, maximumSpeed, movementDuration); // will create a csv file within the same location and with the same name as the data file (with a different file extension)
    }
}

/**
 * Function to view and modify current configuration
 */
void tempSettings() {
    // prints out current configuration values and explains how to modify them to the user's liking
    printf("\nConfiguration Preview:\ndata_file_name \"%s\"\nmovement_duration <%lf : %lf>\nmaximum_speed %lf\nnum_bars %d\nrange_long < %f : %f >\nrange_lat < %f : %f >\n\nNote; If any data can not be provided for the following prompts, enter -1 to use default values or a non-number to keep current values. First prompt is required, -1 there will cancel and return to main menu.\nEnter data file name: ", dataFileName, movementDuration[0], movementDuration[1], maximumSpeed, numBars, rangeLong[0], rangeLong[1], rangeLat[0], rangeLat[1]);
    char fileName[50];
    scanf(" %s",fileName);
    if (strcmp(fileName, "-1") == 0) {
        return;
    }
    strcpy(dataFileName,fileName);
    // user is prompted for each data value and their chosen option is accounted for
    printf("Enter movement duration range (default is 0 to 604800), separated by a space: ");
    scanf(" %lf %lf", &movementDuration[0], &movementDuration[1]);
    cleanBuffer();
    if (movementDuration[0] == -1 || movementDuration[1] == -1) {
        movementDuration[0] = 0;
        movementDuration[1] = 604800;
    }
    printf("Enter the maximum speed (default is 50): ");
    scanf(" %lf", &maximumSpeed);
    cleanBuffer();
    if (maximumSpeed == -1) {
        maximumSpeed = 50;
    }
    printf("Enter the number of bars (default is 20): ");
    scanf(" %d", &numBars);
    cleanBuffer();
    if (numBars == -1) {
        numBars = 20;
    }
    printf("Enter the range of the Longitude (default is 0 to 0), separated by a space: ");
    scanf(" %lf %lf", &rangeLong[0], &rangeLong[1]);
    cleanBuffer();
    if (rangeLong[0] == -1 || rangeLong[1] == -1) {
        rangeLong[0] = 0;
        rangeLong[1] = 0;
    }
    printf("Enter the range of the Latitude (default is 0 to 0), separated by a space: ");
    scanf(" %lf %lf", &rangeLat[0], &rangeLat[1]);
    cleanBuffer();
    if (rangeLat[0] == -1 || rangeLat[1] == -1) {
        rangeLat[0] = 0;
        rangeLat[1] = 0;
    }
}

/**
 * Function to save current configuration to a file in the correct format
 * @param currentFileName; string of the file name
 */
void saveSettings(char currentFileName[50]) {
    if (strcmp(currentFileName,"-1") == 0) {
        printf("\nPlease enter a new configuration file name: ");
        char temp[50];
        scanf(" %s", temp);
        currentFileName = temp; // if no configuration file name has been specified the user is prompted to give one
    }
    FILE * fp = fopen(currentFileName,"w");
    if (fp == NULL) { // checks if the file can be made
        printf("\nError: Can not create Config File\n");
        return;
    }
    // prints the data out in the correct format to the configuration file
    fprintf(fp,"data_file_name \"%s\"\nmovement_duration <%lf : %lf>\nmaximum_speed %lf\nnum_bars %d\nrange_long < %lf : %f >\nrange_lat < %lf : %lf >", dataFileName, movementDuration[0], movementDuration[1], maximumSpeed, numBars, rangeLong[0], rangeLong[1], rangeLat[0], rangeLat[1]);
    fclose(fp);
}

/**
 * Main function that either calls the load function using command line arguments or enters a menu system to make use of the above functions
 * @param argc; numbers of arguments
 * @param argv; argument values
 * @return returns 0 if program closes successfully
 */
int main(int argc, char *argv[]) {
    if (argc > 1) {
        load(argc, argv); // if command line arguments are passed in then the program assumes they're configuration files and automates the process
    } else {
        bool menuLoop = true;
        argv[1] = "-1"; // filling in argument array so it can be used to hold the configuration file name or indicate if no file name has been given
        char fileName[50];
        while (menuLoop) {
            // prints the menu options
            printf("\nPlease select from one of the following options:\n 1. Load a configuration file\n 2. Load the data and generate the csv output file\n 3. Change current configuration\n 4. Update configuration settings\n 5. Quit\nPlease enter a number: ");
            int choice = 0;
            scanf("%d", &choice);
            cleanBuffer(); // catches invalid inputs
            switch (choice) {
                case 1: // configuration file is loaded
                    printf("\nPlease enter configuration file name: ");
                    scanf(" %s",fileName);
                    argv[1] = fileName;
                    scanFile(argv[1]);
                    break;
                case 2: // data is loaded and csv files are generated
                    if (strcmp(dataFileName,"NULL") == 0) { // configuration file will be prompted for if the data file name is left at value "NULL"
                        printf("\nPlease enter data file name: ");
                        scanf(" %s",fileName);
                        if (strcmp(fileName,"-1") != 0) {
                            strcpy(dataFileName, fileName);
                            load(2,argv);
                            menuLoop = false;
                            printf("\nOutput files created successfully, the program will now quit.");
                        }
                    } else {
                        load(2,argv);
                        menuLoop = false; // the program quits after outputting the csv files similarly to when it's given command line arguments
                        printf("\nOutput files created successfully, the program will now quit.");
                    }
                    break;
                case 3: // current configuration is changed
                    tempSettings();
                    break;
                case 4: // current configuration is saved to a file
                    saveSettings(argv[1]);
                    break;
                case 5: // program quits
                    menuLoop = false;
                    break;
                default: // invalid menu input
                    printf("\nError: That was not an option, please try again\n"); // shown for both out of range ints and any other characters
            }
        }
    }
    return 0;
}

/* command Line Arguments used in Development
Configs/config_12.txt
Configs/config_13.txt
Configs/config_15.txt
Configs/config_16.txt
Configs/config_17.txt
Configs/config_18.txt
Configs/config_19.txt
Configs/config_20.txt
*/
