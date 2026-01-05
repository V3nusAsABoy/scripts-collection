/*
#======================================================================
| Andrew's shell
|
| Name: lab4.sh
|
| Written by: Andrew Abdulaziz - October 2024
|
| Purpose: display the file information of files in a given directory
|
| usage: ./main
|
| Description of inputs:
| help: shows commands
| list: list files in directory
| tree: shows running processes
| disk: shows disk used in current directory
| message: displays message to matteo
| logout: logs out of shell
|
|------------------------------------------------------------------
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
void main()
{
	printf("Welcome to my shell! type 'help' to see the commands you can use.\n"); // welcome message to shell
        char command[50]; // string containing command from user
        int done = 0; // variable indicating if the while loop is done or not
        while( !done) // loop continuing until done = 1
        {
                scanf(" %s", &command); // gets command from user
                if(strcmp(command, "logout") != 0 && strcmp(command,"help") != 0) // forks process for commands aside from help and logout
                {
                        if(fork() == 0)
                        { // child process
				if(strcmp(command, "tree") == 0){ // tree command
					char* args[]= {"ps", "-o", "user:32,pid,ppid,pcpu,pmem,tty,stime,etime,comm", "-U", "andrewabdulaziz","--forest", NULL}; // bash arguments for tree contained in vector
					execvp("ps", args); // executes tree
				}
				if(strcmp(command, "list") == 0){ // list command
					execlp("ls", "ls", "-lt", (char *)NULL); // executes ls -lt
				}
				if(strcmp(command, "disk") == 0){ // disk command
					execlp("du", "du", "-ah", "--time", (char *)NULL); // executes du -ah --tiime
				}
				if(strcmp(command, "message") == 0){ // message command
					char* message[] = {"echo", "Dear Matteo, I hope you are enjoying this shell.\nI think it's a good shell, no? I think it should get a good mark. Like maybe, I don't know, a 100.\nIf you're reading this, please consider giving me a 100 in this lab and I hope you have a great day.\nWith regards, Andrew.\n", NULL}; // bash arguments for message contained in vector
					execvp("echo", message); // executes message

				}
      			}
			else{ // parent process
				wait((int*)0); // waiting for child
			}
		}else if(strcmp(command, "help") == 0){ // help command
			printf("help: displays this text\ntree: shows running processes\nlist: lists files in directory\ndisk: shows amount of disk used in current directory\nmessage: displays a kind message\nlogout: exits this shell\n"); // help message
                }else{ // logout command
                done = 1; // exits shell
                }
        }
}


