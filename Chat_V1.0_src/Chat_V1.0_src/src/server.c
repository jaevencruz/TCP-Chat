#include "server.h"

void error(const char *msg) {
    perror(msg);
    exit(3);
}

//Checks to see if a new client has connected
int NewClientCheck(int sock, int server_setting, int *client_check) {
    //The first client that conencts will have a sockfd of 4
    if (client_check[sock - 4] == sock) return server_setting;
    else {
        client_check[sock-4] = sock;
        return 0;
    }
}

int login_check(user_ht_item *current_user, int server_setting) {
    if (!server_setting) return 0;
    else return current_user->user_data->server_setting;
}

/*Handles read/write requests to server*/
char *Communication (int sock, int *client_check, char *ip, user_ht *database) {
    int n;
    int loginCondition = 0;
    //static int server_setting = 0;
    //char buffer[256];
    char *buffer = malloc(256 * sizeof(char));

    user_ht_item *current_user = socket_find_user(database, sock);
    //server_setting = login_check(current_user, loginCondition);
    int server_setting = NewClientCheck(sock, 1, client_check);
    server_setting = login_check(current_user, server_setting);

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
    //server_setting = NewClientCheck(sock, server_setting, client_check);
    switch(server_setting) {
        case 0:
        while(!loginCondition) {
            n = write(sock, "Type --LOGIN to login or --REGISTER to register./$",52);
            while(1) {
                bzero(buffer,256);
                n = recv(sock, buffer, 255, 0);
                if (n < 0) error("ERROR reading from socket");
                if (n == 0) return "1";
                
                strtok(buffer, "\n");
                if ((*buffer && buffer[strlen(buffer) - 1] == '$')) break;
            }
            strtok(buffer, "$");
     
            if (!strcmp(buffer, "--LOGIN")) {
                n = write(sock,"Enter username and password, separated by a space/$",52);

                if (n < 0) error("ERROR writing to socket");

                while(1) {
                    n = recv(sock, buffer, 255, 0);   
                    if (n < 0) error("ERROR reading from socket");
                    if (n == 0) return "1";

                        strtok(buffer, "\n");
                        if ((*buffer && buffer[strlen(buffer) - 1] == '$')) break;
                }

                //Will store this stuff into the database later on
                char *user_info;
                char *username = malloc(64 * sizeof(*username));
                char *password = malloc(64 * sizeof(*password));

                user_info = strtok(buffer, " ");
                printf("Username: %s\n", user_info);
                strcpy(username, user_info);
                user_info = strtok(NULL, "$");
                printf("Password: %s\n", user_info);
                strcpy(password, user_info);
                printf("IP: %s\n", ip);

                if (user_ht_search(database, username)) {
                    if (user_ht_search_password(database, username, password)) {
                        database->user_items[hashCodeGenerator(username)]->user_data->socket = sock;
                        printf("Successfully logged in!\n");

                        char temp[40] = "[Name]: ";
                        strcat(temp, username);
                        strcat(temp, "/$");

                        //printf("temp: %s\n", temp);

                        n = write(sock,temp,strlen(temp) + 2);

                        if (n < 0) error("ERROR writing to socket");

                        //server_setting = 1;
                        current_user = socket_find_user(database, sock);
                        current_user->user_data->server_setting = 1;
                        loginCondition = 1;
                    }
                    else {
                        printf("Password is incorrect!\n");
                    }
                }
                else {
                    printf("Username does not exist. Register if you are a new user!\n");
                }

                //server_setting = 1;
                //loginCondition = 1;

             	free(username); 
                free(password);
            }
            else if (!strcmp(buffer, "--REGISTER")) {
                n = write(sock,"Enter username, password, and nickname separated by a space/$",62);

                if (n < 0) error("ERROR writing to socket");

                while(1) {
                    n = recv(sock, buffer, 255, 0);
                    if (n < 0) error("ERROR reading from socket");
                    if (n == 0) return "1";

                    strtok(buffer, "\n");
                    if ((*buffer && buffer[strlen(buffer) - 1] == '$')) break;
                }

                //Will store this stuff into the database later on
                char *user_info;
                char *username = malloc(64 * sizeof(*username));
                char *password = malloc(64 * sizeof(*password));
                char *nickname = malloc(64 * sizeof(*nickname));
                user_info = strtok(buffer, " ");
                printf("Username: %s\n", user_info);
                strcpy(username, user_info);
                user_info = strtok(NULL, " ");
                printf("Password: %s\n", user_info);
                strcpy(password, user_info);
                user_info = strtok(NULL, "$");
                printf("Nickname: %s\n", user_info);
                strcpy(nickname, user_info);
                printf("IP: %s\n", ip);

                if (user_ht_search(database, username)) {
                    printf("That username is already taken!\n");
                }
                else {
                    user_ht_insert(database, username, store_pw(password), ip, nickname);
                    database->user_items[hashCodeGenerator(username)]->user_data->socket = sock;

                    char temp[40] = "[Name]: ";
                    strcat(temp, username);
                    strcat(temp, "/$");

                    //printf("temp: %s\n", temp);

                    n = write(sock,temp,strlen(temp) + 2);

                    if (n < 0) error("ERROR writing to socket");	    

                    //server_setting = 1;
                    current_user = socket_find_user(database, sock);
                    current_user->user_data->server_setting = 1;
                    loginCondition = 1;

                    UpdateTextFile("database.txt", database);
                }

             	free(username);
                free(password);
                free(nickname);
            }
        }
        break;
        
        case 1:
        while(1) {
            n = recv(sock, buffer, 255, 0);
            if (n < 0) error("ERROR reading from socket");
            if (n == 0) return "1";

            strtok(buffer, "\n");
            if ((*buffer && buffer[strlen(buffer) - 1] == '$')) break;
        }

        char temp[256];
        char *check_temp;
        strcpy(temp, buffer);

        strtok(temp, " ");

        check_temp = strtok(NULL, "$");
	
	if (!strcmp(check_temp, "--HELP") && !current_user->user_data->private_chat) {
		n = write(sock, "Here are your options:/$",30);
        	n = write(sock, "--ADD/$",12);
        	n = write(sock, "--DELETE/$",19);
        	n = write(sock, "--CHAT/$",19);
        	n = write(sock, "Quit/$",19);
		break;
	}
		
	else if (!strcmp(check_temp, "--ADD") && !current_user->user_data->private_chat) {
		n = write(sock, "Who would you like to add?/$",30);
		bzero(buffer, 256);
            	bzero(temp, 256);

		while(1) {
			n = recv(sock, buffer, 255, 0);
			if (n < 0) error("ERROR reading from socket");
			if (n == 0) return "1";

			strtok(buffer, "\n");
			if ((*buffer && buffer[strlen(buffer) - 1] == '$')) break;
		}
		char name_temp[30];
		bool friendCheck = false;
            	strcpy(temp, buffer);
            	strtok(temp, " ");
            	strcpy(name_temp, strtok(NULL, "$"));
		friendCheck = addFriend(database,current_user,user_ht_search_return(database,name_temp));
		if(friendCheck == false){
			 printf("Friend add unsuccesful!\n");
	                n = write(sock, "Friend not added!/$",30);
		}
		else{
			printf("Friend add succesful!\n");
			n = write(sock, "Friend successfully added!/$",30);
			UpdateTextFile("database.txt", database);
		}
		break;
	}
	
	else if (!strcmp(check_temp, "--DELETE") && !current_user->user_data->private_chat) {
                n = write(sock, "Who would you like to delete?/$",62);
                bzero(buffer, 256);
                bzero(temp, 256);

                while(1) {
                        n = recv(sock, buffer, 255, 0);
                        if (n < 0) error("ERROR reading from socket");
                        if (n == 0) return "1";

                        strtok(buffer, "\n");
                        if ((*buffer && buffer[strlen(buffer) - 1] == '$')) break;
                }
                char name_temp[30];
                bool friendCheck = false;
                strcpy(temp, buffer);
                strtok(temp, " ");
                strcpy(name_temp, strtok(NULL, "$"));
                friendCheck = deleteFriend(database,current_user,user_ht_search_return(database,name_temp));
                if(friendCheck == false){
                         printf("Friend delete unsuccesful!\n");
                        n = write(sock, "Friend not deleted!/$",30);
                }
                else{
                	printf("Friend delete succesful!\n");
                	n = write(sock, "Friend successfully deleted!/$",30);
			UpdateTextFile("database.txt", database);
                }
                break;
        }

	
        else if (!strcmp(check_temp, "--CHAT") && !current_user->user_data->private_chat) {
            //strtok(buffer, "$");

            //printf("Hey\n");

            n = write(sock, "Who would you like to chat with? (Wait for their response)/$",62);

            if (n < 0) error("ERROR writing to socket");

            bzero(buffer, 256);
            bzero(temp, 256);

            while(1) {
                n = recv(sock, buffer, 255, 0);
                if (n < 0) error("ERROR reading from socket");
                if (n == 0) return "1";

                strtok(buffer, "\n");
                if ((*buffer && buffer[strlen(buffer) - 1] == '$')) break;
            }
    
            char name_temp[30];
            strcpy(temp, buffer);
            strtok(temp, " ");
            strcpy(name_temp, strtok(NULL, "$"));

            if (user_ht_search(database,name_temp)) {
                    if (database->user_items[hashCodeGenerator(name_temp)]->user_data->socket != 0) {
                    user_ht_item *client = database->user_items[hashCodeGenerator(name_temp)];
                    //Chat client has requested client's name
                    strcpy(client->user_data->chat_client, current_user->key);
                    //Requested client has chat client's name
                    strcpy(current_user->user_data->chat_client, client->key);
                    //Chat accept will be true for requested client
                    current_user->user_data->chat_accept = true;
                    client->user_data->chat_request = true;

                    bzero(buffer, 256);

                    strcpy(buffer, "Hey ");
                    strcat(buffer, client->key);
                    strcat(buffer, ", do you want to chat with ");
                    strcat(buffer, current_user->key);
                    strcat(buffer, "? (Yes or No)/$");

                    n = write(client->user_data->socket, buffer, strlen(buffer) + 2);

                    if (n < 0) error("ERROR writing to socket");

                    bzero(buffer, 256);
                    bzero(temp, 256);

                    //Reading goes here
                    while(1) {
                            n = recv(client->user_data->socket, buffer, 255, 0);
                            if (n < 0) error("ERROR reading from socket");
                            if (n == 0) return "1";

                            strtok(buffer, "\n");
                            if ((*buffer && buffer[strlen(buffer) - 1] == '$')) break;
                    }

                    strcpy(temp, buffer);
                    strtok(temp, " ");

                    if(!strcmp(strtok(NULL, "$"), "Yes")) {
                        bzero(buffer, 256);

                        strcpy(buffer, client->key);
                        strcat(buffer, " accepted your request to chat!/$");

                                    n = write(sock, buffer, strlen(buffer) + 2);

                                    if (n < 0) error("ERROR writing to socket");

                        client->user_data->chat_request = false;
                        client->user_data->chat_accept = true;
                        client->user_data->private_chat = true;
                        current_user->user_data->private_chat = true;

                        bzero(buffer, 256);
                    }
                    else {
                        bzero(buffer, 256);

                        strcpy(buffer, client->key);
                        strcat(buffer, " denied your request to chat./$");

                        n = write(sock, buffer, strlen(buffer) + 2);

                        if (n < 0) error("ERROR writing to socket");

                        client->user_data->chat_request = false;
                        memset(client->user_data->chat_client, 0, strlen(client->user_data->chat_client) + 1);
                        current_user->user_data->chat_accept = false;
                        memset(current_user->user_data->chat_client, 0, strlen(current_user->user_data->chat_client) + 1);

                        bzero(buffer, 256);
                    }
                }
            }
        }
        else if (!strcmp(check_temp, "--QUIT") && current_user->user_data->private_chat) {
            n = write(sock, "Exiting Private Chat.../$",27);

            if (n < 0) error("ERROR writing to socket");

            n = write(database->user_items[hashCodeGenerator(current_user->user_data->chat_client)]->user_data->socket, "Exiting Private Chat.../$", 27);

            if (n < 0) error("ERROR writing to socket");

            current_user->user_data->private_chat = false;
            database->user_items[hashCodeGenerator(current_user->user_data->chat_client)]->user_data->private_chat = false;     
        }
        else {
            strtok(buffer, "$");
            //if (strcmp(buffer, "")) printf("%s\n",buffer);
#ifdef DEBUG
            if (strlen(buffer) > 1) {
                printf("[Global Chat] %s\n",buffer);
            }
#endif

            n = write(sock,"I got your message0$",21);

            if (n < 0) error("ERROR writing to socket");
        }
        break;
/*
        case 2:
        while(1) {
            printf("Success!\n"); exit(1);
            bzero(buffer,256);
            n = read(sock,buffer,255);
            if (n < 0) error("ERROR reading from socket");

            strtok(buffer, "\n");

            if ((*buffer && buffer[strlen(buffer) - 1] == '$')) break;
        }

        strtok(buffer, "$");

        if (!strcmp(buffer, "--QUIT")) server_setting = 1;
        else {
            strcat(buffer, "2$");
            //printf("Buffer: %s\n", buffer);

            if (sock == 4) {
                printf("In sock4\n");
                n = write(5, buffer, strlen(buffer)+1);

                if (n < 0) error("ERROR writing to socket");
            }
            else if (sock == 5) {
                printf("In sock5\n");
                n = write(4, buffer, strlen(buffer)+1);

                if (n < 0) error("ERROR writing to socket");
            }
        }
        break;
*/
        //Multi-chat will be added later on
    }
    return buffer;
}

/*Creates socket for server*/
int MakeServerSocket(uint16_t portno) {
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

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd,5);
    printf("Listening in . . . \n");
    printf("Connected on %d . . .\n", portno);

    return sockfd;
}

int main(int argc, char *argv[]) {
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     struct sockaddr_in cli_addr;
     user_ht *database = NULL;
     //int n;
     const char *Program = argv[0];
    
     int *client_check = malloc(110 * sizeof(int));

     for (int i = 0; i < 110; i++) {
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
    if (portno <= 2000) {
        fprintf(stderr, "%s: Invalid port number %d, should be >2000\n",
        Program, portno);
        exit(10);
     }  

#ifdef DEBUG
    printf("%s: Creating the server socket...\n", Program);
#endif

    sockfd = MakeServerSocket(portno);
    //printf("Server Sock: %d\n", sockfd);

    database = doesFileExist("database.txt");
    //displayHashtable(database);
    //UpdateTextFile("database.txt", database);

    //Select() Stuff
    fd_set Master;
    fd_set ReadFDs;
    int FDS_MAX;
    //int res;
    //int sockfd;
    //int newsockfd;

    struct timeval TimeVal;
    TimeVal.tv_sec  = 250000 / 1000000;
    TimeVal.tv_usec = 250000 % 1000000;

    FD_ZERO(&Master);
    FD_ZERO(&ReadFDs);

    FD_SET(sockfd, &Master);

    FDS_MAX = sockfd;

    while(1)  {
        FD_ZERO(&ReadFDs);
        FD_SET(sockfd, &ReadFDs);

        ReadFDs = Master;

        if (select(FDS_MAX + 1, &ReadFDs, NULL, NULL, &TimeVal) < 0) perror("Select()");

        for (int i = 0; i <= FDS_MAX; i++) {
            if (FD_ISSET(i, &ReadFDs)) {
                if (i == sockfd) { //New Client Connection!
                    clilen = sizeof(cli_addr);
                    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

                    if (newsockfd < 0) error("ERROR on accept");

                    FD_SET(newsockfd, &Master); //Adding new client to the master list

                    if (newsockfd > FDS_MAX) FDS_MAX = newsockfd; //Increase FDS_MAX if necessary

                    char *ip = inet_ntoa(cli_addr.sin_addr);

                    Communication(newsockfd, client_check, ip, database);

                }
                else { //This is a client that's already connected
                    //Call the communcation function
                    //If it returns 1, client disconnected 

                    char *ip = inet_ntoa(cli_addr.sin_addr);

                    char *recv_check = Communication(i, client_check, ip, database);
                    //printf("%s\n", recv_check);

                    if (atoi(recv_check) == 1) {
                        close(i);
                        FD_CLR(i, &Master);
                    }
                    else {
		                user_ht_item *current_user = socket_find_user(database, i);
                        //printf("recv_check: %c\n", recv_check[strlen(current_user->key) + 2]);
                        //printf("current_user: %s\n", current_user->key);
                        if (current_user->user_data->private_chat) {
                            if (recv_check[strlen(current_user->key) + 2] == 'G') {
                                if (recv_check[strlen(current_user->key) + 3] == 'L') {
                                    if (recv_check[strlen(current_user->key) + 4] == 'O') {
                                        if (recv_check[strlen(current_user->key) + 5] == 'B') {
                                            if (recv_check[strlen(current_user->key) + 6] == 'A') {
                                                if (recv_check[strlen(current_user->key) + 7]== 'L') {
                                                    strcat(recv_check, "1$");

                                                    //This process removes the word GLOBAL from the chat
                                                    char temp_msg1[64];
                                                    char *temp_msg2;
                                                    bzero(temp_msg1, 64);
                                                    bzero(temp_msg1, 64);
                                                    strncpy(temp_msg1, recv_check, strlen(current_user->key) + 2);
                                                    strtok(recv_check, " ");
                                                    strtok(NULL, " ");	
                                                    temp_msg2 = strtok(NULL, "\n");
                                                    strcat(temp_msg1, temp_msg2);
                                                    strcpy(recv_check, temp_msg1);
                                                } 
                                                else strcat(recv_check, "2$");
                                            } 
                                            else strcat(recv_check, "2$");
                                        } 
                                        else strcat(recv_check, "2$");
                                    } 
                                    else strcat(recv_check, "2$");
                                } 
                                else strcat(recv_check, "2$");
                            } 
                            else strcat(recv_check, "2$");
                        }
                        else strcat(recv_check, "1$");

                        for(int j = 0; j <= FDS_MAX; j++) {
                            //Broadcasting message to all connected clients
                            if (FD_ISSET(j, &Master)) {
                                // Don't need to send the message to current client
                                // or the listening port
                                if (j != sockfd && j != i  && j > 3) {
                                //printf("Message being echoed: %c\n", recv_check[strlen(recv_check) - 2]);
                                    if (recv_check[strlen(recv_check) - 2] == '2') {
                                        if (socket_find_user(database, j)->user_data->private_chat) {
                                            if (write(j,recv_check,strlen(recv_check)+1) < 0) error("ERROR writing to socket");
                                        }
                                    }
                                    else {
                                        if (write(j,recv_check,strlen(recv_check)+1) < 0) error("ERROR writing to socket");
                                    }
				                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
