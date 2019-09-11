//Using select statement
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/prctl.h>
#include <signal.h>
#include <stdbool.h>
#define h_addr h_addr_list[0]

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void* create_shared_memory(size_t size) {
    //Using this to share a string between
    //the parent process and the child
    //process of the fork().

    //PROT_READ & PROT_WRITE allows us
    //to read the string and write to it
    int prot = PROT_READ | PROT_WRITE;
    
    //MAP_SHARED allows the parent to see
    //the child's changes to the string
    //and vice versa -- If parent changes
    //the string to "Zach", the child process
    //will be able to see that.
    //MAP_ANONYMOUS allows only the child process
    //and the parent process to access this string
    int permissions = MAP_ANONYMOUS | MAP_SHARED;
    return mmap(NULL, size, prot, permissions, 0, 0);
}


char *getbuffer(char *name, char *buffer)
{
    //256 is buffer length, 30 is name length, 2 accounts
    //for the null characters. Can't use strlen on the
    //pointers to the char arrays.
    int len = 256 + 30 + 2;

    if (!strcmp(name, "USERNONE")) return buffer;
    else 
    {
	    char temp[len];
	    memset(temp, 0, len);
	    strcat(temp, name);
	    strcat(temp, ": ");
	    strcat(temp, buffer);
	    //strncpy(buffer, temp, len + 1);
	    strncpy(buffer,temp,256);
	    //printf("Temp: %s\n", temp);
	    return buffer;
    }
}	


int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

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
        fprintf(stderr,"ERROR, no such host\n");
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

    //Shared Memory
    void *shared_name = create_shared_memory(256);
    //memcpy(shared_name, 0, 8192);
    memcpy(shared_name, "USERNONE", strlen("USERNONE"));

    //Going to use fork to read and write at the same time

    if (fork() == 0) { //This is where we read constantly if possible
        //Select() Stuff
        fd_set ReadFDs;     /* socket file descriptors ready to read from */
        struct timeval TimeVal;
        int res;
	int private_start = 1;

        TimeVal.tv_sec  = 250000 / 1000000;
        TimeVal.tv_usec = 250000 % 1000000;

        while(1) {

	    prctl(PR_SET_PDEATHSIG, SIGKILL);

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
                        //bzero(buffer,256);
                        n = read(sockfd,buffer,255);

                        if (n < 0) error("ERROR reading from socket");
            
                        //strtok(buffer, "\n");

                        if ((*buffer &&  buffer[strlen(buffer) - 1] == '$')) break;
                    }
            char *msg;
	    char *name_check = malloc(40 * sizeof(*name_check));
            msg = strtok(buffer, "$");
	    strncpy(name_check, msg, 8);
	    //printf("name_check: %s\n", name_check);
	    #ifdef DEBUG
	    printf("Msg: %s\n", msg);
	    #endif
            strcpy(buffer, msg);
	    if (!strcmp(name_check, "[Name]: "))
	    {
		strtok(msg, " ");
		memcpy(shared_name, strtok(NULL, "/"), 256);
		//bzero(name_check, 40);
	    }
                    if (strlen(buffer) > 1) 
                    {
                        if (buffer[strlen(buffer) - 1] == '1')
                        {
                            strtok(buffer, "1");
                            printf("[Global Chat]: %s\n", buffer);
                        }
                        else if (buffer[strlen(buffer) - 1] == '2')
                        {
			    if (private_start)
			    {
				printf("Type --QUIT to exit Private Chat.\n");
				printf("Type GLOBAL followed by a space to chat in Global Chat while in a private chat.\n");
				private_start = 0;
			    }
                            strtok(buffer, "2");
                            printf("[User]: %s\n", buffer);
                        }
			else if (buffer[strlen(buffer) - 1] == '/')
			{
			    strtok(buffer, "/");
			    printf("[Server]: %s\n", buffer);
			}
                    }
		    free(name_check);
                }
            }
        }
    }
    else { //This is where we will constantly write if possible

	//char *name = malloc(30 * sizeof(*name));
	//strcpy(name, "USERNONE");

        while(1)
        {
            //printf("Please enter the message: ");
            bzero(buffer,256);
            fgets(buffer,255,stdin);
	    //printf("Buffer: %s\n", buffer);
	    strcpy(buffer, getbuffer(shared_name, buffer));
            strtok(buffer, "\n");
            strcat(buffer, "$");

            #ifdef DEBUG
            printf("Buffer: %s\n", buffer);
            #endif

            if (buffer[strlen(buffer) - strlen("Quit$")] == 'Q')
                if (buffer[strlen(buffer) - strlen("Quit$") + 1] == 'u')
            	    if (buffer[strlen(buffer) - strlen("Quit$") + 2] == 'i')
                	if (buffer[strlen(buffer) - strlen("Quit$") + 3] == 't')
                            if (buffer[strlen(buffer) - strlen("Quit$") + 4] == '$')
                            {
				close(sockfd);
				exit(0);
			    }

            n = write(sockfd,buffer,strlen(buffer));
            if (n < 0) error("ERROR writing to socket");

            bzero(buffer,256);
        }
    }
    close(sockfd);
    return 0;
}
