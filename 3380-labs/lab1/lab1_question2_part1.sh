#!/bin/bash

#======================================================================
#| 404 page tracker
#|
#| Name: lab1_question2_part1.sh
#|
#| Written by: Andrew Abdulaziz - September 2024
#|
#| Purpose: Output the 404 pages and their count of a log file into a txt document then creating a second
#| txt document outputting the 10 most common 404 pages into a separate txt file
#|
#| usage: ./lab1_question2_part1.sh #name_of_logfile
#|
#| Description of parameters:
#| $1 - the name of the log file to view 404 pages of
#|
#| Subroutines/libraries required:
#| none
#|------------------------------------------------------------------


grep -e " 404 " $1 | awk '{print $6, $7}' | sort | uniq -c | sort -h -r  > all_logged_404_pages.txt

# grep returns the contentss of the log file. Awk parses out the 404 pages. Uniq weeds out duplicate pages and returns the count. Sort sorts it in order from highest to
# lowest count. The output will be put in to the all_logged_404_pages.txt file

cat all_logged_404_pages.txt | head -n 10 > top_10_404_pages.txt

# cat outputs the contents of the all_logged_404_pages.txt. Head -n 10 specifies we only need the first 10 lines. This will be outputted to the top_10_404_pages.txt file
