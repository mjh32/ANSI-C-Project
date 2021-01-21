# ANSI-C-Project
**Introduction**

README file for CS23820 Assignment by mjh32 (2020)

In this package you will find this README file, the Clion project folder named "cs23820_mjh32", a video file named "mjh32ScreenCastDemonstration.mp4" and a two page design document named "mjh32DesignDocument.pdf".

**Clion Project Folder**

The Clion project contains the C files that contain the code for the project. It also contains all other files needed for the a C project to be opened and used.
These C files are connected in a hierarchy using the "#include" statement. The hierarchy is as follows: "main.c" -> "listFunctions.c" -> "nodeFunctions.c" -> "header.c"
There is also "functions.h" to store the function prototypes.
Standard libraries used were: stdio.h, ctype.h, stdbool.h, math.h, stdlib.h, string.h and time.h

The program requires configuration file(s) and data files to operate. The configuration files should be in the format specified by the assignment specification.
All files that the user wishes to use with the program should be placed in the "cmake-build-debug" directory. Once in there, the files can have their own directory structure but the path will need to be specified in the file name.
When running the main function in "main.c" file, a menu system will allow the user to load the files.
If you wish to run the program in batch mode the user will need to have the configuration file name(s) as program arguments.

**Video File**

The video demonstrates a clean building of the program and how to use all 5 menu options presented to the user showing them all functioning correctly.
The video also shows the program working with multiple program arguments in batch mode.

**Design Document**

The design document will reveal how the functions in the program were designed and the reasoning behind those decisions.
