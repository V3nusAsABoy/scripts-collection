/*
#======================================================================
| Signal thing
|
| Name: lab5.c
|
| Written by: Andrew Abdulaziz - November 2024
|
| Purpose: create infinite loop tracking ^C and executing after the 5th. If a ^\ signal is receieved, it shows the ^C count and terminates.
|
| usage: ./lab5
|
|------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile int counter = 0; // counter for ^C signals

void sig_usr( int signo ) // function handling SIGNIT and SIGQUIT
{
	if( signo == SIGINT ){ // handles SIGINT
		counter++; // increments counter
		printf("This is the %d time you pressed ctrl-c \n", counter); // displays that this is the nth time ^C has been used
		if(counter == 5){ // if ^C has been used 5 times, it prints a message showing the max has been reached and the SIGINT signal goes back to default
			printf("Max has been reached. Program will terminate on next ^C \n");
			signal(SIGINT,SIG_DFL);
		}
	}

	if( signo == SIGQUIT ){ // handles SIGQUIT
		int ppid = getpid(); // gets ppid
		if(fork() == 0){ // child
			printf("I am the child hear me roar\n"); // message from child
			kill(ppid, SIGUSR1); // sends SIGUSR1 to parrent
			exit(0); // terminates
		}
		else // parent
		{
			wait((int*)0); // waits for child to terminate
			exit(0); // terminates
		}
	}
	
	if ( signo == SIGUSR1 ) // handles SIGUSR1
		printf("You pressed ^C %d times\nThe child sent a signal so you must be bored. See ya!\n", counter); // displays message showing final ^C count
}

int main() // main function
{
	if( signal( SIGINT,sig_usr ) == SIG_ERR ){ // handles SIGINT
		printf( "Cannot catch ^C SIGINT\n" );
		return 1;
	}
	if( signal( SIGQUIT,sig_usr ) == SIG_ERR ){ // handles SIGQUIT
		printf("Cannot catch ^\ SIGQUIT\n");
		return 1;
	}
	if( signal( SIGUSR1,sig_usr ) == SIG_ERR) // handles SIGUSR1
        {
                printf("Cannot catch SIGUSR1\n");
                return 1;
        }
	while(1) // infinite loop waiting for signals
	{
		printf( "Waiting for another signal... \n" );
		pause(); // pauses until signal has been received
	}
	return 0;
}
