#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

/*
#======================================================================
| File copier - server
|
| Name: server.c
|
| Written by: Andrew Abdulaziz - December 2024
|
| Purpose: Receives file from client and copies the contents of a file to it
|
| usage: ./server
|
|------------------------------------------------------------------
*/

void catcher(int sig);
int newfd;

// packet structure to send in pipe
struct packet
{
        int nbytes; // number of bytes in packet
        char payload[1024]; // buffer contining file contents
        int file; // file descriptor
	char *name[64]; // name of file
}

main()
{
	int socketd, outfile; // socket and fd for copy of file to write to
	struct sockaddr_in srv; // server
	struct sockaddr_in cli; // client
	int cli_len = sizeof(cli); // length of client
	int not_done = 1;
	ssize_t nread; // number read
	struct packet p; // packet to receive
	void catcher(int sig)
        {
                signal(SIGPIPE, catcher);
                close(newfd);
                exit(0);
        }
	signal(SIGPIPE, catcher);
	if((socketd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket call failed");
		exit(1);
	}

	srv.sin_family = AF_INET;
	srv.sin_port = htons(59573); // sets port to 59753
	srv.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(socketd, (struct sockaddr *) &srv, sizeof(srv)) < 0) { // bind socket
		perror("bind call failed");
		exit(1);
	}

	if(listen(socketd, 5) < 0) { // listen to socket
		perror("listen call failed");
		exit(1);
	}

	while (not_done)
	{
		newfd = accept(socketd, (struct sockaddr*) &cli, &cli_len); // accept message received on socket
		if(newfd < 0)
		{
			perror("accept call failed");
			not_done = 0;
		}
		if ( fork() == 0) // new process to send new file name through socket
		{
			while(recv(newfd, &p, sizeof(p), 0) > 0)
			{
				char filename[128]; // name of copy file
				strcpy(filename, "copyof");
				strcat(filename, p.name); // sets naame to copyof + file naame
				outfile = open(filename,O_WRONLY | O_CREAT, 0644);
				write(outfile, p.payload, p.nbytes); // creates file copy
				close(outfile);
				send(newfd, &filename, sizeof(filename), 0); // sends copy file name to client
			}
		close(newfd);
		exit(0);
		}
		else
		{
			close(newfd);
		}
	}
}
