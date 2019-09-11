#ifndef GUI_H
#define GUI_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <gtk/gtk.h>
#include <assert.h>

#define BUFFSIZE 256
#define FRIENDMAX 10

typedef struct LoginInfo {
    GtkWidget *window[2];
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry3;
    const gchar *string1;
    const gchar *string2;
    const gchar *string3;
} LogInfo;

typedef struct ChatWindow {
    GtkWidget *window;
    GtkWidget *notebook;
    GtkWidget *entry;
    GtkWidget *textview;
    const gchar *message;
} ChatWindow;

void StartWindow (LogInfo *log);
void GetLogin (GtkWidget *widget, gpointer data);
void FatalError (const char *ErrorMsg);
char *Talk2Server(const char *Message, char *RecvBuf);
char *Login (const char *username, const char *password);
void CreateAccount (GtkWidget *widget, gpointer data);
void GetRegister (GtkWidget *widget, gpointer data);
char *Register (const char *username, const char *password);
void ShowWindow (GtkWidget *widget, gpointer data);
void RemoveFriend (GtkButton *button, GtkNotebook *notebook);
gboolean Delete (GtkWidget *widget, GtkWidget *event, gpointer data);
void FriendsList (LogInfo *log, char *friends[FRIENDMAX]);
void Friend_List (LogInfo *log);
void Message (GtkWidget *widget, gpointer data);
int FriendRequest (GtkWidget *parent, gchar *message);
char open_dialog (GtkWidget *widget);
void ServerWindow();
#endif
