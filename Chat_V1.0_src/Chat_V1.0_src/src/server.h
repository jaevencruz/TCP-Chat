#ifndef SERVER
#define SERVER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>
#include "database.h"
#include "hash.h"
/*#include "chess_src/ChessPieceHierarchy.h"
#include "chess_src/ChessRules.h"
#include "chess_src/GameBoard.h"
#include "chess_src/ChessTools.h"*/




void error(const char *msg);
int MakeServerSocket(uint16_t portno);
char *Communication(int sock, int *client_check, char *ip, user_ht *database);
int NewClientCheck(int sock, int server_setting, int *client_check);
int login_check(user_ht_item *current_user, int loginCondition);
#endif
