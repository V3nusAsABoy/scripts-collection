#!/bin/bash

#======================================================================
#| 404 page tracker
#|
#| Name: lab1_question2_part2.sh
#|
#| Written by: Andrew Abdulaziz - September 2024
#|
#| Purpose: Output the ips and their count of a log file for users searching 404 pages into a txt document then creating a second
#| txt document outputting the 10 most common ips into a separate txt file
#|
#| usage: ./lab1_question2_part2.sh #name_of_logfile
#|
#| Description of parameters:
#| $1 - the name of the log file to view ips of
#|
#| Subroutines/libraries required:
#| none
#|------------------------------------------------------------------

grep -e " 404 " $1 | awk '{print $1}' | sort | uniq -c | sort -h -r  > all_logged_ips.txt

# grep returns 404 pages of a log file. awk parses out the ips. uniq removes duplicate ips and shows the count for each ip. Sort sorts the ips in order by count
# this will be outputted to the all_logged_ips.txt file

cat all_logged_ips.txt | head -n 10 > top_10_ips.txt

# cat outpouts the contents of the all_logged_ips.txt file. head -n 10 specifies that we only need the highest 10. This will be outputted to the top_10_ips.txt file
