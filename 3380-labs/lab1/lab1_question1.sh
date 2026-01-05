#!/bin/bash

#======================================================================
#| Zip file creator
#|
#| Name: lab1_question1.sh
#|
#| Written by: Andrew Abdulaziz - September 2024
#|
#| Purpose: Zip up the contents of a directory then output the zip file in the home directory
#|
#| usage: ./lab1_question1.sh #name_of_directory
#|
#| Description of parameters:
#| $1 - the name of the directory to zip up
#|
#| Subroutines/libraries required:
#| none
#|------------------------------------------------------------------

zip -r ${HOME}/${LOGNAME}_$1.zip ${HOME}/*/$1

# ${HOME}/${LOGNAME}_$1.zip specifies the name of the zip file and the directory it will be stored in. This zip file will use the user's logname and the command line input to name the file and the file will be stored in the home directory
# ${HOME}/*/$1 specifies that the contents that will be zipped up will be the directory the name of the directory the user inputted
