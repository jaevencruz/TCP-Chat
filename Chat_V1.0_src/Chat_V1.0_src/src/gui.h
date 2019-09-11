#ifndef GUI_H
#define GUI_H

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
#include <gtk/gtk.h>
#include <assert.h>
#include <stdbool.h>

#define BUFFSIZE 256
#define FRIENDMAX 10

typedef struct ChatInfo {
    GtkWidget *window;
    GtkWidget *notebook;
    GtkWidget *entry;
    GtkWidget *textview;
    gchar *message;
    gint page;
} ChatInfo;

typedef struct LoginInfo {
    GtkWidget *window[2];
    GtkWidget *usernameEntry;
    GtkWidget *passwordEntry;
    GtkWidget *nicknameEntry;
    const gchar *username;
    const gchar *password;
    const gchar *nickname;
    gchar *flag;
    gchar *protocol;
    bool shutdown;
    ChatInfo *chat; 
} LogInfo;

void StartWindow (LogInfo *log);
void GetLogin (GtkWidget *widget, gpointer data);
void Login (LogInfo *log);
void CreateAccount (GtkWidget *widget, gpointer data);
void GetRegister (GtkWidget *widget, gpointer data);
void Register (LogInfo *log);
void ShowWindow (GtkWidget *widget, gpointer data);
void RemoveFriend (GtkButton *button, GtkNotebook *notebook);
gboolean Delete (GtkWidget *widget, GtkWidget *event, gpointer data);
void FriendsList (LogInfo *log, char *friends[FRIENDMAX]);
void GetPage (GtkWidget *widget, gpointer data);
void Message (GtkWidget *widget, gpointer data);
void UpdateChat (LogInfo *log, char *message);
void ChatWindow (LogInfo *log, char *friends[FRIENDMAX]);
void Chess (GtkWidget *widget, gpointer data);
void Chat (GtkWidget *widget, gpointer data);
void Add (GtkWidget *widget, gpointer data);
void Remove (GtkWidget *widget, gpointer data);
int FriendRequest (GtkWidget *parent, gchar *message);
char open_dialog (GtkWidget *widget);
void ServerWindow();

//Shared Memory
void* create_shared_memory(size_t size);

#endif
