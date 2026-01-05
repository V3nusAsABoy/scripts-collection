/*
#======================================================================
| Poor man's -ls
|
| Name: lab3.sh
|
| Written by: Andrew Abdulaziz - October 2024
|
| Purpose: display the file information of files in a given directory
|
| usage: ./main (optional dash parameters with given value) (optional name of directory)
|
| Description of parameters:
| -o - this specifies that you only want to display files that have been last modified older than a given number of dayw. Example usage: ./main -o 10 will only show files last modified  older than 10 days ago
| -y - this specifies that you only want to display files that have been last modified earlier than a given number of days. Example usage: ./main -y 10 will only show files last modified earlier than 10 days ago
| -l - this specifies that you only want to display files larger than a given file size. Example usage: ./main -l 100 will only display files larger than 100 bytes
| -s - this specifies that you only want to display files smaller than a given file size. Example usage: ./main -s 100 will only display files smaller than 100 bytes
| directory name - this is an optional parameter you can put at the end of the command line which will specify which directory you want to show the contents of. Example usage: ./main ${HOME} will display contents of home directory
|
|------------------------------------------------------------------
*/

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <stdbool.h>

const int days(const char* path){ // method that calculates the number of days a file has been last modified since. the parameter is the path of the file
	struct stat sfile;
	stat(path, &sfile);

	time_t rightNow = time(NULL); // returns current time

	double ageInSeconds = difftime(rightNow, sfile.st_mtime); // calculates difference in seconds between the current time and the time a file was last modified

	int ageInDays = ageInSeconds/(86400); // translates difference in seconds to difference in days by multiplying by 86400

	return ageInDays;
}


int permissions(const char* path){ // prints the permission links of a file. the parameter is the path of the file
	struct stat sfile;
	stat(path, &sfile);

	printf( (S_ISDIR(sfile.st_mode)) ? "d" : "-");
    	printf( (sfile.st_mode & S_IRUSR) ? "r" : "-");
    	printf( (sfile.st_mode & S_IWUSR) ? "w" : "-");
    	printf( (sfile.st_mode & S_IXUSR) ? "x" : "-");
    	printf( (sfile.st_mode & S_IRGRP) ? "r" : "-");
    	printf( (sfile.st_mode & S_IWGRP) ? "w" : "-");
    	printf( (sfile.st_mode & S_IXGRP) ? "x" : "-");
    	printf( (sfile.st_mode & S_IROTH) ? "r" : "-");
    	printf( (sfile.st_mode & S_IWOTH) ? "w" : "-");
    	printf( (sfile.st_mode & S_IXOTH) ? "x" : "-");
	printf(" ");
}

const char* filetype(const char* path){ // prints the type of the file. it checks if its a regular, directory, etc then prints a 3 letter string specifying it. the parameter is the path of the file
	struct stat sfile;
	stat(path, &sfile);

	if(S_ISREG(sfile.st_mode) != 0){
		return "REG";
    	} else if (S_ISDIR(sfile.st_mode) != 0) {
        	return "DIR";
    	} else if (S_ISCHR(sfile.st_mode) != 0) {
        	return "CHR";
    	} else if (S_ISBLK(sfile.st_mode) != 0) {
        	return "BLK";
    	} else if (S_ISFIFO(sfile.st_mode) != 0) {
        	return "FIFO";
    	} else if (S_ISLNK(sfile.st_mode) != 0) {
        	return "LNK";
    	} else if (S_ISSOCK(sfile.st_mode) != 0) {
        	return "SOCK";
    	} else {
        	return "UNKNOWN";
    	}
}

int output(const char* path) // prints the inode, type, permission links, uid, gid, last modification time, and name of a file. parameter is the file path
{
	struct stat sfile;
	stat (path, &sfile);
	printf("%lu ", sfile.st_ino);
        printf("%s ", filetype(path));
        permissions(path);
        printf("  %d ", sfile.st_uid);
        printf("     %d ", sfile.st_gid);
        char modDate[50];
        strftime(modDate, 50, "%Y-%m-%d %H:%M:%S", localtime(&sfile.st_mtime));
        printf("   %s ", modDate);
        printf("%d ", (long long)sfile.st_size);
}

int main(int argc, char **argv[]) // main argument parameters are the command line argument count and the command line arguments
{
	int min = -1;
	int max = -1;
	int minDays = -1;
	int maxDays = -1;
	for(int i = 1; i < argc; i++){ // for loop to get the options and adjust accordingly
		if(strcmp(argv[i], "-o") == 0){ // if the user types -o, it sets minDays to the int value following it. Otherwise minDays is -1
			minDays = atoi(argv[i+1]);
		} else if(strcmp(argv[i], "-y") == 0){ // if the user types -y, it sets maxDays to the int value following it. Otherwise maxDays is -1
			maxDays = atoi(argv[i+1]);
		} else if(strcmp(argv[i], "-l") == 0){ // if the user types -l, it sets min to the int value following it. Otherwise min is -1
			min = atoi(argv[i+1]);
		} else if(strcmp(argv[i], "-s") == 0){ // if the user types -s, it sets max to the int value following it. Otherwise max is -1
			max = atoi(argv[i+1]);
		}
	}
	DIR* dir = opendir(argv[argc-1]); // opens directory inputted by user
	char dirname[4096] = "."; // sets directory name to "." by default
	struct stat sfile;
	if(dir == NULL){ // if the directory is null, the directory will be set to "."
		dir = opendir(".");
	} else	{ // if directory is not null, it changes the dirname file to the directory inputted by the user
		strncpy(dirname, argv[argc-1], sizeof(dirname) - 1);;
	}

	struct dirent* dp;
	dp = readdir(dir); // reads directory
	printf("inode    Type Permission   UID	    GID	    Modification date   Size   Filename \n"); // prints beginning row
	while((dp = readdir(dir)) != NULL){ // reads directory until empty
		char path[4096];
		snprintf(path, sizeof(path), "%s/%s", dirname, dp->d_name); // adds full path of a file to path variable
		stat(path, &sfile);
		if(minDays != -1){ // if the -o parameter was used, it checks if the file is older than the value given. otherwise the loop continues
			if(days(path) < minDays){
				continue;
			}
		}
		if(maxDays != -1) { // if the -y parameter was used, it checks if the file is younger than the value given. otherwise the loop continues
			if(days(path) > maxDays){
				continue;
			}
		}
		if(min != -1){ // if the -l parameter was used, it checks if the file is larger than the value given. otherwise the loop continues
			if(sfile.st_size <  min){
				continue;
			}
		}
		if(max != -1){ // if the -s parameter was used, it checks if the file is smaller than the value given. otherwise the loop continues
			if(sfile.st_size > max){
				continue;
			}
		}
		output(path); // outputs the contents of the file
		printf("   %s \n", dp->d_name); // outputs name of the file
	}

	closedir(dir); // closes directory
	return 0;
}
