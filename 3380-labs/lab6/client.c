#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

/*
#======================================================================
| File copier - client
|
| Name: client.c
|
| Written by: Andrew Abdulaziz - December 2024
|
| Purpose: Reads the contents of a file to a file descriptor and sends it to the server to create a copy
|
| usage: ./client (make sure server is running)
|
|------------------------------------------------------------------
*/

// packet structure to send in pipe
struct packet
{
        int nbytes; // number of bytes in packet
        char payload[1024];  // buffer contining file contents
        int file; // file descriptor
        char *name[64]; // name of file
}

main()
{
	int socketd;  // socket
	ssize_t maxread = 1024; // max bytes to read
	struct sockaddr_in srv; // server
	int more_data = 1;
	char *filename[64]; // name of file
	struct packet p; // packet
	if((socketd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket call failed");
		exit(1);
	}

	srv.sin_family = AF_INET;
	srv.sin_port = htons(59573); // connect to porrt 59573
	srv.sin_addr.s_addr = INADDR_ANY; // connect to any ip
	if(connect(socketd, (struct sockaddr*) &srv, sizeof(srv)) < 0)
	{
		perror("connect call failed"); exit(1);
	}
	while (more_data)
	{
		if(strcmp("\n",p.name) != 0)
		{
			printf("Type the name of the file you want to copy (or type '0' to quit): "); // ask for file name
			scanf(" %64s",p.name); // get file name
			fflush(stdin);
			if(strcmp("0",p.name) != 0)
			{
				if(strcmp("\n",p.name) != 0)
				{
					p.file = open(p.name, O_RDONLY); // open file to read
					p.nbytes = read(p.file, p.payload, maxread); // reads file
					close(p.file);
					send(socketd, &p, sizeof(p), 0); // sends file to server
					if(recv(socketd, &filename, sizeof(filename), 0) > 0) // receives name of copy file
						printf("File copied. Name of file is: %s.\n", filename); // prints name of copy file
					else
					{
						printf("Server has died\n");
						close(socketd);
						exit(1);
					}
				}
			}
			else
				more_data = 0;
		}
	}
	exit(0);
}
