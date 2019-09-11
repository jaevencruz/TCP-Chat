//Using select statement
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include "gui.h"
#define h_addr h_addr_list[0]

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    int gui_enabled = 1;

    char buffer[256];
  
    printf("Type Quit to close connection\n");

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    /* GUI */
    LogInfo *log = g_malloc(sizeof(*log));
    gtk_init(&argc, &argv);
    StartWindow(log);
    gtk_main();
#ifdef DEBUG
	g_print("Ending GUI...\n");
#endif
    /* End GUI */

    //Going to use fork to read and write at the same time

    if (fork() == 0) { //This is where we read constantly if possible
        //Select() Stuff
        fd_set ReadFDs;     /* socket file descriptors ready to read from */
        struct timeval TimeVal;
        int res;

        TimeVal.tv_sec  = 250000 / 1000000;
        TimeVal.tv_usec = 250000 % 1000000;

        while(1) {

            FD_ZERO(&ReadFDs);
            FD_SET(sockfd, &ReadFDs);

            // select(int numfds,  fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
            //numfds is the highest file descriptor plus one -- FD_SETSIZE is a macro set to 1024 
            //We are trying to check if we can read and write to ONE socket -- the socket connected to the server
            res = select(FD_SETSIZE, &ReadFDs, NULL, NULL, &TimeVal);

            if (res < 0) {
                perror("Select() Error");
            }
            else if (res == 0) {
                //There's nothing to read
            }
            else {
                if (FD_ISSET(sockfd, &ReadFDs)) {
                //printf("In read\n");
                    while(1)
                    {
                        //bzero(buffer, 256);
                        n = read(sockfd, buffer, 255);

                        if (n < 0) error("ERROR reading from socket");
            
                        //strtok(buffer, "\n");

                        if ((*buffer &&  buffer[strlen(buffer) - 1] == '$')) break;
                    }
            char *msg;
            msg = strtok(buffer, "$");
	    #ifdef DEBUG
	    printf("Msg: %s\n", msg);
	    #endif
            strcpy(buffer, msg);

                    if (strlen(buffer) > 1) 
                    {
                        if (buffer[strlen(buffer) - 1] == '1')
                        {
                            strtok(buffer, "1");
                            printf("[Global Chat]: %s\n", buffer);
                        }
                        else if (buffer[strlen(buffer) - 1] == '2')
                        {
                            strtok(buffer, "2");
                            printf("[User]: %s\n", buffer);
                        }
			else if (buffer[strlen(buffer) - 1] == '/')
			{
			    strtok(buffer, "/");
			    printf("[Server]: %s\n", buffer);
			}
                    }
                }
            }
        }
    }
    else { //This is where we will constantly write if possible

	sleep(0.01);

        while(1)
        {
            //printf("Please enter the message: ");
            bzero(buffer,256);
            //fgets(buffer,255,stdin);
            //printf("String1: %s\n", log->string1);
	    //printf("String2: %s\n", log->string2);
	   
	    if (gui_enabled)
	    {
 
	        char *login = Login(log->string1, log->string2);
                strcpy(buffer, Login(log->string1, log->string2));
                strtok(buffer, "\n");
                strcat(buffer, "$");

	        //printf("Buffer: %s\n", buffer);

		char *option;
		char *info;

		option = strtok(buffer, " ");
		
		if((!strcmp(option, "--LOGIN")) || (!strcmp(option, "--REGISTER")))
		{
		    if(!strcmp(option, "--REGISTER"))
		    {
			n = write(sockfd, "--REGISTER$", 13);
			if (n < 0) error("ERROR writing to socket");
			sleep(1);

			info = strtok(NULL, "$");

			n = write(sockfd, info, strlen(info));
			if (n < 0) error("ERROR writing to socket");
		    }
		    else
		    {
			n = write(sockfd, "--LOGIN$", 10);
			if (n < 0) error("ERROR writing to socket");
			sleep(1);
			info = strtok(NULL, "$");

			//printf("Info: %s\n", info);

			strcat(info, "$");

			n = write(sockfd, info, strlen(info));
			if (n < 0) error("ERROR writing to socket");
		    }
		}
		gui_enabled = 0;
	    }

            bzero(buffer,256);
            fgets(buffer,255,stdin);
	    strtok(buffer, "\n");
	    strcat(buffer, "$");

            #ifdef DEBUG
            printf("Buffer: %s\n", buffer);
            #endif

            if (!strcmp(buffer, "quit") || !strcmp(buffer, "Quit")) break;
            //exit(0); //Plan to do something else here

            n = write(sockfd, buffer, strlen(buffer));
            if (n < 0) error("ERROR writing to socket");

            bzero(buffer,256);
        }
    }
    g_free(log);
    close(sockfd);
    return 0;
}
