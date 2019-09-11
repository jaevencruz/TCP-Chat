/* A simple server in the internet domain using TCP
   The port number is passed as an argument */

#include "server.h"

/*Function that closes the program when an error occurs*/
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

//Checks to see if a new client has connected
int NewClientCheck(int sock, int server_setting, int *client_check)
{
    //The first client that conencts will have a sockfd of 4
    //printf("Client_Check[0]: %d\n", client_check[0]);
    if (client_check[sock - 4] == sock) 
    {
/*
	for (int i = 0; i < 110; i++)
	{
	    printf("client_check[%d]: %d\n", i, client_check[i]);
	}
	printf("end\n");
*/
	
	return server_setting;
    }
    else
    {
        client_check[sock-4] = sock;
        return 0;
    }
}

/*Handles read/write requests to server*/
void Communication (int sock, int *client_check)
{
    int n;
    static int server_setting = 0;
    char buffer[256];

    #ifdef DEBUG
    printf("Sock: %d\n", sock);
    #endif  

    bzero(buffer, 256);

    /*Server Setting Breakdown
    // 0: Logging in
    // 1: Global Chat (Messages shown in server)
    // 2: 1-on-1 Private Chat
    // 3: Private Multi Chat
    */

    //printf("Server Setting: %d\n", server_setting);

    //printf("Test: %d\n", NewClientCheck(sock, server_setting, client_check));
    //switch(1)
    server_setting = NewClientCheck(sock, server_setting, client_check);
    switch(server_setting)
    {
        case 0:
        n = write(sock,"Enter username and password, separated by a space$",51);

        if (n < 0) error("ERROR writing to socket");

        while(1)
        {
            n = read(sock,buffer,255); //Hard coded fd for testing

            strtok(buffer, "\n");

            if ((*buffer && buffer[strlen(buffer) - 1] == '$')) break;
        }

        //Will store this stuff into the database later on
        char *user_info;
        user_info = strtok(buffer, " ");
        printf("Username: %s\n", user_info);
        user_info = strtok(NULL, "$");
        printf("Password: %s\n", user_info);

        server_setting = 1;
        break;

        case 1:
            while(1)
            {
                bzero(buffer,256);
                n = read(sock,buffer,255);
                if (n < 0) error("ERROR reading from socket");

                strtok(buffer, "\n");

                if ((*buffer && buffer[strlen(buffer) - 1] == '$')) break;
            }

            strtok(buffer, "$");

            if (!strcmp(buffer, "--CHAT")) server_setting = 2;
            else
            {
                //if (strcmp(buffer, "")) printf("%s\n",buffer);
                if (strlen(buffer) > 1) printf("[Global Chat] %s\n",buffer);

                n = write(sock,"I got your message1$",21);

                if (n < 0) error("ERROR writing to socket");
            }

            break;

        case 2:
            while(1)
            {
                bzero(buffer,256);

                n = read(sock,buffer,255);

                if (n < 0) error("ERROR reading from socket");

                strtok(buffer, "\n");

                if ((*buffer && buffer[strlen(buffer) - 1] == '$')) break;
            }

            strtok(buffer, "$");

            if (!strcmp(buffer, "--QUIT")) server_setting = 1;
            else
            {
                strcat(buffer, "2$");
		//printf("Buffer: %s\n", buffer);

                if (sock == 4)
                {
                    printf("In sock4\n");
                    n = write(5, buffer, strlen(buffer)+1);

                    if (n < 0) error("ERROR writing to socket");
                }
                if (sock == 5)
                {
                    printf("In sock5\n");
                    n = write(4, buffer, strlen(buffer)+1);

                    if (n < 0) error("ERROR writing to socket");
                }
            }

            break;

        //Multi-chat will be added later on

    }
}

/*Creates socket for server*/
int MakeServerSocket(uint16_t portno)
{
    struct sockaddr_in serv_addr;
    int sockfd;

    /*The socket function returns an integer. The function looks like socet(int domain, int type, int protocol)*/
    /*AF_INET is the network protocol. SOCK_STREAM makes the socket able to send data.  Setting protocol to 0 allows the socket to use its default protocol rules.*/


    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) error("Error opening socket. Socket creation failed");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr,
        sizeof(serv_addr)) < 0)
            error("ERROR on binding");

    listen(sockfd,5);
    printf("Listening in . . . \n");
    printf("Connected on %d . . .\n", portno);

    return sockfd;
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     struct sockaddr_in cli_addr;
     int n;
     const char *Program = argv[0];
    
     int *client_check = malloc(110 * sizeof(int));

     for (int i = 0; i < 110; i++)
     {
         client_check[i] = - 1;
     }
 
     #ifdef DEBUG
     printf("%s: Starting...\n", Program);
     #endif

    if (argc < 2) {
        fprintf(stderr,"No Port Provided. Usage: %s port\n", Program);
        exit(1);
    }
    portno = atoi(argv[1]);
    if (portno <= 2000)
    {
        fprintf(stderr, "%s: Invalid port number %d, should be >2000\n",
        Program, portno);
        exit(10);
     }  

    #ifdef DEBUG
    printf("%s: Creating the server socket...\n", Program);
    #endif

    sockfd = MakeServerSocket(portno);
    clilen = sizeof(cli_addr);

    while (1) 
    {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        //printf("IP address is: %s\n", inet_ntoa(cli_addr.sin_addr));
        //char *ip = inet_ntoa(cli_addr.sin_addr);
        //printf("Port is: %d\n", (int) ntohs(cli_addr.sin_port));

        if (newsockfd < 0) error("ERROR on accept");

        if (fork() == 0) 
        {
            #ifdef DEBUG
            printf("Child Process\n");
            #endif

            close(sockfd); //Fork Child is using newsockfd
            //strcat(ip, "$");

            //n = write(newsockfd, ip, strlen(ip) + 1);

            //if (n < 0) error("ERROR writing to socket");

            //n = write(newsockfd, "Enter Username and Password, separated by a space$",51);

            //if (n < 0) error("ERROR writing to socket");

            while(1) 
            {
                Communication(newsockfd, client_check);
            }
        }
        //Since sockfd is never closed in the loop, the sockfd
        //can be used to talk back to a specific client
    }
    close(sockfd);
    return 0;
}
