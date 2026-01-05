/*
#======================================================================
| Reversing a file
|
| Name: lab2.sh
|
| Written by: Andrew Abdulaziz - October 2024
|
| Purpose: reverse the contents of a file in an output file
|
| usage: ./main
|
| Description of parameters:
| fileName - name of file in which the contents will be reversed
| fileName2 - name of output file containing reversed content
|
| Subroutines/libraries required:
| none
|------------------------------------------------------------------
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    // pointers of input and input file. fd1 = input, fd2 = output.
    FILE *fd1, *fd2;
    // name of input file
    char fileName[512];
    // name of output file
    char fileName2[512];
    // asks user for name of input file and stores it in fileName pointer
    printf("Type in the name of the file you want to reverse the contents of: ");
    scanf("%s", &fileName);
    // asks user for name of output file and stores it in fileName2 pointer
    printf("Type in the name of the output file containing the reversed data: ");
    scanf("%s", &fileName2);
    // opens the input file to be read from
    fd1 = open(fileName, O_RDONLY, 0);
    // opens the output file to be written to. Creates it if it doesn't exist
    fd2 = open(fileName2, O_WRONLY | O_CREAT, 0644);
    // returns size of file
    int size = lseek(fd1, 0, SEEK_END);
    // for loop that creates a character c, lseeks to the end of the file minus i, reads from input then writes to output
    for(int i = 0; i < size+1; i++){
	char c;
    	lseek(fd1, -i, SEEK_END);
    	read(fd1, &c, 1);
    	write(fd2, &c, 1);
    }
    exit(0);
}
