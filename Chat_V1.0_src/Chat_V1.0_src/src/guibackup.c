#include "gui.h"

const char *Program	= NULL;

struct sockaddr_in
	ServerAddress;	/* server address we connect with */

void StartWindow (LogInfo *log) {
    /* Containers */
    GtkWidget *table;
    GtkWidget *frame;

    /* Labels */
    GtkWidget *login;
    GtkWidget *chatnchess;
    GtkWidget *username;
    GtkWidget *password;

    /* Buttons */
    GtkWidget *signin;
    GtkWidget *createnew;

    PangoFontDescription *pfd; 

    log->window[0] = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(log->window[0]), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(log->window[0]), "ChatNChess");
    gtk_container_set_border_width(GTK_CONTAINER(log->window[0]), 15);

    table = gtk_table_new(8, 4, FALSE);
    gtk_container_add(GTK_CONTAINER(log->window[0]), table);

    frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_ETCHED_IN);

    gtk_table_attach(GTK_TABLE(table), frame, 0, 4, 3, 8, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 1, 1);

    pfd = pango_font_description_new();
    pango_font_description_set_size(pfd, 20 * PANGO_SCALE);
    
    /* CHATNCHESS */
    chatnchess = gtk_label_new("Chat N Chess");
    gtk_widget_modify_font(chatnchess, pfd);

    gtk_table_attach(GTK_TABLE(table), chatnchess, 0, 4, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    /* LOGIN USERNAME AND PASSWORD */
    login = gtk_label_new("Login");
    gtk_widget_modify_font(login, pfd); 
    username = gtk_label_new("Username:");
    password = gtk_label_new("Password:");
    
    gtk_table_attach(GTK_TABLE(table), login, 0, 4, 3, 4, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), username, 1, 2, 4, 5, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), password, 1, 2, 5, 6, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    
    /* USER INPUT ENTRIES */
    log->entry1 = gtk_entry_new();
    log->entry2 = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(log->entry2), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(log->entry2), '*');

    gtk_table_attach(GTK_TABLE(table), log->entry1, 2, 3, 4, 5, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), log->entry2, 2, 3, 5, 6, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    /* SIGN IN AND CREATE NEW ACCOUNT */
    signin = gtk_button_new_with_label("Sign In");
    createnew = gtk_button_new_with_label("Create New Account");
    
    gtk_table_attach(GTK_TABLE(table), signin, 1, 3, 6, 7, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), createnew, 1, 3, 7, 8, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    
    gtk_widget_show_all(log->window[0]);

    /*Combine the string of "USERNAME" and "sent you a friend request" below*/
/*    int DialResp = FriendRequest(log->window[0], "  NAME sent you a friend request!  ");
    g_print("%d\n", DialResp);
    char UserResp = open_dialog (log->window[0]);
    g_print("%s\n", &UserResp);
*/
    g_signal_connect(signin, "clicked", G_CALLBACK(GetLogin), (gpointer)log);
    g_signal_connect(createnew, "clicked", G_CALLBACK(CreateAccount), (gpointer)log);

    g_signal_connect(log->window[0], "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

gboolean Delete (GtkWidget *widget, GtkWidget *event, gpointer data) {
    g_free(data);
    gtk_main_quit();
    return FALSE;
}

void GetLogin (GtkWidget *widget, gpointer data) {
    LogInfo *log = (LogInfo*)data;
    log->string1 = gtk_entry_get_text(GTK_ENTRY(log->entry1));
    log->string2 = gtk_entry_get_text(GTK_ENTRY(log->entry2));
    printf("Login pressed\n");
#ifdef DEBUG
    g_print("Username: %s\nPassword: %s\n", log->string1, log->string2);
#endif
   
    char *login = Login(log->string1, log->string2);
#ifdef DEBUG
    g_print("Login protocol: %s\n", login);
#endif
/*    
    char *friends[FRIENDMAX];
    char *token = strtok(login, " ");
    if (!strcmp(token, "--SUCCESS")) {
        int i;
        for (i = 0; token != NULL; ++i) {
            friends[i] = token;
            token = strtok(NULL, " ");
        }
        FriendsList(log, friends);
    }
*/
    gtk_widget_destroy(log->window[0]);
}

char *Login (const char *username, const char *password) {
    static int info = 1;
    static char protocol[BUFFSIZE];

    if (!info) return protocol;

    strcpy(protocol, "--LOGIN ");

/*
    char RecvBuf[BUFFSIZE]; 
    static char *Response;
*/

#ifdef DEBUG
    printf("%s: Login protocol starting...\n", Program);
#endif
    strcat(protocol, username); 
    strcat(protocol, " "); 
    strcat(protocol, password); 
#ifdef DEBUG
    printf("%s: Login protocol done...\n", Program);
#endif

#ifdef DEBUG
    printf("%s: Login callback starting...\n", Program);
#endif
/*
    Response = Talk2Server(protocol, RecvBuf);
*/
#ifdef DEBUG
    printf("%s: Login callback done.\n", Program);
#endif
/*
    return (Response);
*/
    info = 0;
  
    return protocol;
}

char *Talk2Server(const char *Message, char *RecvBuf) {
    int n;
    int SocketFD;

    SocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (SocketFD < 0)
        FatalError("socket creation failed");
    
#ifdef DEBUG
    printf("%s: Connecting to the server at port %d...\n", Program, ntohs(ServerAddress.sin_port));
#endif

    if (connect(SocketFD, (struct sockaddr*)&ServerAddress, sizeof(struct sockaddr_in)) < 0)
        FatalError("connecting to server failed");

#ifdef DEBUG
    printf("%s: Sending message '%s'...\n", Program, Message);
#endif

    n = write(SocketFD, Message, strlen(Message));
    
    if (n < 0)
        FatalError("writing to socket failed");

#ifdef DEBUG
    printf("%s: Waiting for response...\n", Program);
#endif

    n = read(SocketFD, RecvBuf, BUFFSIZE-1);

    if (n < 0) 
        FatalError("reading from socket failed");

    RecvBuf[n] = 0;

#ifdef DEBUG
    printf("%s: Received response: %s\n", Program, RecvBuf);
    printf("%s: Closing the connection...\n", Program);
#endif

    close(SocketFD);
    return(RecvBuf);
}

void FatalError(const char *ErrorMsg) {
    fputs(Program, stderr);
    fputs(": ", stderr);
    perror(ErrorMsg);
    fputs(Program, stderr);
    fputs(": Exiting!\n", stderr);
    exit(20);
} 

void CreateAccount (GtkWidget *widget, gpointer data) {
    /* Containers */
    GtkWidget *table;
    GtkWidget *frame;

    /* Labels */
    GtkWidget *title;
    GtkWidget *username;
    GtkWidget *password;
    GtkWidget *confirm;

    /* Buttons */
    GtkWidget *signin;
    GtkWidget *createnew;

    PangoFontDescription *pfd;
    
    LogInfo *log = (LogInfo*)data;

    gtk_widget_hide(log->window[0]);
    log->window[1] = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(log->window[1]), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(log->window[1]), "ChatNChess");
    gtk_container_set_border_width(GTK_CONTAINER(log->window[1]), 15);

    table = gtk_table_new(8, 4, FALSE);
    gtk_container_add(GTK_CONTAINER(log->window[1]), table);

    frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_ETCHED_IN);

    gtk_table_attach(GTK_TABLE(table), frame, 0, 4, 0, 8, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 1, 1);

    pfd = pango_font_description_new();
    pango_font_description_set_size(pfd, 20 * PANGO_SCALE);
    
    /* Title */
    title = gtk_label_new("Create Account");
    gtk_widget_modify_font(title, pfd);

    gtk_table_attach(GTK_TABLE(table), title, 0, 4, 0, 1, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    /* LOGIN USERNAME AND PASSWORD */
    username = gtk_label_new("Username:");
    password = gtk_label_new("Password:");
    confirm = gtk_label_new("Confirm Password:");
    
    gtk_table_attach(GTK_TABLE(table), username, 1, 2, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), password, 1, 2, 2, 3, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), confirm, 1, 2, 3, 4, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    
    /* USER INPUT ENTRIES */
    log->entry1 = gtk_entry_new();
    log->entry2 = gtk_entry_new();
    log->entry3 = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(log->entry2), FALSE);
    gtk_entry_set_visibility(GTK_ENTRY(log->entry3), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(log->entry2), '*');
    gtk_entry_set_invisible_char(GTK_ENTRY(log->entry3), '*');

    gtk_table_attach(GTK_TABLE(table), log->entry1, 2, 3, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), log->entry2, 2, 3, 2, 3, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), log->entry3, 2, 3, 3, 4, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    /* SIGN IN AND CREATE NEW ACCOUNT */
    signin = gtk_button_new_with_label("Return To Sign In");
    createnew = gtk_button_new_with_label("Create New Account");
    
    gtk_table_attach(GTK_TABLE(table), signin, 1, 3, 7, 8, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), createnew, 1, 3, 6, 7, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    
    gtk_widget_show_all(log->window[1]);

    g_signal_connect(signin, "clicked", G_CALLBACK(ShowWindow), (gpointer)log);
    g_signal_connect(createnew, "clicked", G_CALLBACK(GetRegister), (gpointer)log);

    g_signal_connect(log->window[1], "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

void GetRegister (GtkWidget *widget, gpointer data) {
    LogInfo *log = (LogInfo*)data;
    log->string1 = gtk_entry_get_text(GTK_ENTRY(log->entry1));
    log->string2 = gtk_entry_get_text(GTK_ENTRY(log->entry2));
    log->string3 = gtk_entry_get_text(GTK_ENTRY(log->entry3));

    printf("Register pressed\n");
   
#ifndef DEBUG
    g_print("Username: %s\nPassword: %s\nConfirm: %s\n", log->string1, log->string2, log->string3);
#endif

    if (!strcmp(log->string2, log->string3)) {
        g_print("Passwords do not match!\n");
    }
    else {
        char *message = Register(log->string1, log->string2);
        if (strcmp(message, "SUCCESS"))
            ShowWindow(widget, (gpointer)log);
    }
}

char *Register (const char *username, const char *password) {
    char RecvBuf[BUFFSIZE]; 
    char protocol[BUFFSIZE] = "--Register ";
    static char *Response;

    strcat(protocol, username); 
    strcat(protocol, " "); 
    strcat(protocol, password); 

#ifdef DEBUG
    printf("%s: Register callback starting...\n", Program);
#endif

    Response = Talk2Server(protocol, RecvBuf);

#ifdef DEBUG
    printf("%s: Register callback done.\n", Program);
#endif

    return (Response);
}

void ShowWindow (GtkWidget *widget, gpointer data) {
    LogInfo *log = (LogInfo*)data;
    gtk_widget_show(log->window[0]);
    gtk_widget_hide(log->window[1]);
}

void RemoveFriend (GtkButton *button, GtkNotebook *notebook) {
    gint page = gtk_notebook_get_current_page(notebook);
    gtk_notebook_remove_page(notebook, page);
    gtk_widget_queue_draw(GTK_WIDGET(notebook));
}

void FriendsList (LogInfo *log, char *friends[FRIENDMAX]) {
    GtkWidget *button;
    GtkWidget *table;
    GtkWidget *frame;
    GtkWidget *label;
    GtkWidget *box;
    int i;
    char bufferf[32];
    char bufferl[32];

    ChatWindow *chat = g_malloc(sizeof(*chat));
   
    gtk_widget_hide(log->window[0]);
    
    chat->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(chat->window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(chat->window), "ChatNChess");
    
    g_signal_connect(chat->window, "delete-event", G_CALLBACK(Delete), NULL);

    gtk_container_set_border_width (GTK_CONTAINER(chat->window), 10);
    
    table = gtk_table_new(2, 3, FALSE);
    gtk_container_add (GTK_CONTAINER (chat->window), table);

    /* Create a new notebook, place the position of the tabs */
    chat->notebook = gtk_notebook_new ();
    gtk_notebook_set_tab_pos (GTK_NOTEBOOK (chat->notebook), GTK_POS_TOP);
    gtk_table_attach_defaults (GTK_TABLE (table), chat->notebook, 0, 3, 0, 1);
    gtk_widget_show (chat->notebook);

    /* Let's append a bunch of pages to the notebook */
    for (i = 0; i < 3; i++) {
        sprintf(bufferf, "Append Frame %d", i + 1);
        sprintf(bufferl, "Page %d", i + 1);

        frame = gtk_frame_new (NULL);
        gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
        gtk_widget_set_size_request (frame, 480, 320);
        gtk_widget_show (frame);

        chat->textview = gtk_text_view_new ();
        gtk_text_view_set_editable(GTK_TEXT_VIEW (chat->textview), FALSE);
        gtk_container_add (GTK_CONTAINER (frame), chat->textview);
        gtk_widget_show (chat->textview);

        label = gtk_label_new (bufferl);
        gtk_notebook_append_page (GTK_NOTEBOOK (chat->notebook), frame, label);
    } 

    /* Set what page to start at (page 4) */
    gtk_notebook_set_current_page (GTK_NOTEBOOK (chat->notebook), 0);

    sprintf (bufferl, "Home Page");

    frame = gtk_frame_new (NULL);
    gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
    gtk_widget_set_size_request (frame, 480, 320);
    gtk_widget_show (frame);

    box = gtk_vbox_new (TRUE, 1);
    gtk_container_add (GTK_CONTAINER (frame), box);
    gtk_widget_show (box);

    button = gtk_button_new_with_label ("Add Friend");
    g_signal_connect_swapped (button, "clicked", G_CALLBACK (Delete), chat);
    gtk_container_add (GTK_CONTAINER (box), button);
    gtk_widget_show (button);

    button = gtk_button_new_with_label ("Remove Friend");
    g_signal_connect (button, "clicked", G_CALLBACK (RemoveFriend), chat->notebook);
    gtk_container_add (GTK_CONTAINER (box), button);
    gtk_widget_show (button);

    button = gtk_button_new_with_label ("Close");
    g_signal_connect_swapped (button, "clicked", G_CALLBACK (Delete), chat);
    gtk_container_add (GTK_CONTAINER (box), button);
    gtk_widget_show (button);

    label = gtk_label_new (bufferl);
    gtk_notebook_prepend_page (GTK_NOTEBOOK (chat->notebook), frame, label);

    /* Create a button and an entry */
    chat->entry = gtk_entry_new();
    /*gtk_table_attach_defaults (GTK_TABLE (table), chat->entry, 0, 5, 9, 10);*/
    gtk_table_attach(GTK_TABLE(table), chat->entry, 0, 2, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_widget_show (chat->entry);

    button = gtk_button_new_with_label ("Send");
    g_signal_connect (button, "clicked", G_CALLBACK (Message), chat);
    /*gtk_table_attach_defaults (GTK_TABLE (table), button, 5, 6, 9, 10);*/
    gtk_table_attach(GTK_TABLE(table), button, 2, 3, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_widget_show (button);

    gtk_widget_show (table);
    gtk_widget_show (chat->window);
}

void Message (GtkWidget *widget, gpointer data) {
    ChatWindow *chat = (ChatWindow*)data;
    chat->message = gtk_entry_get_text(GTK_ENTRY(chat->entry));

#ifndef DEBUG
    g_print("Message: %s\n", chat->message);
#endif

    gtk_text_view_set_editable(GTK_TEXT_VIEW (chat->textview), TRUE);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(chat->textview));

    gtk_text_buffer_set_text(buffer, chat->message, -1);
}

void Friend_List (LogInfo *log)
{
    /* Containers */
    GtkWidget *table;
    GtkWidget *frame;

    /* Labels */
    GtkWidget *friend_list;

    /* Buttons */
    GtkWidget *add_friends;
    GtkWidget *delete_friends;

    PangoFontDescription *pfd;

    log->window[0] = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(log->window[0]), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(log->window[0]), "Friends list");
    gtk_container_set_border_width(GTK_CONTAINER(log->window[0]), 15);

    table = gtk_table_new(9, 8, FALSE);
    gtk_container_add(GTK_CONTAINER(log->window[0]), table);

    frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_ETCHED_IN);

    gtk_table_attach(GTK_TABLE(table), frame, 0, 4, 3, 8, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 1, 1);

    pfd = pango_font_description_new();
    pango_font_description_set_size(pfd, 20 * PANGO_SCALE);

    /* Friends List */

    friend_list = gtk_label_new("Firends List");
    gtk_widget_modify_font(friend_list, pfd);

    gtk_table_attach(GTK_TABLE(table), friend_list, 0, 4, 3, 4, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    /* ADD and DELETE FRIENDS */
    add_friends = gtk_button_new_with_label("Add friends");
    delete_friends = gtk_button_new_with_label("Delete friends");
    
    gtk_table_attach(GTK_TABLE(table), add_friends, 1, 7, 1, 4, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), delete_friends, 1, 7, 5, 8, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    
    gtk_widget_show_all(log->window[0]);
    g_signal_connect(log->window[0], "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

void ServerWindow() {
    GtkWidget *window;

    /* Containers */
    GtkWidget *table;
    GtkWidget *frame;

    /* Labels */
    GtkWidget *server;

    /* Buttons */
    GtkWidget *quit;

    /* Text Buffer */
    GtkWidget *buffer;

    PangoFontDescription *pfd;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), "Server");
    gtk_container_set_border_width(GTK_CONTAINER(window), 15);

    table = gtk_table_new(8, 4, FALSE);
    gtk_container_add(GTK_CONTAINER(window), table);

    frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_ETCHED_IN);

    gtk_table_attach(GTK_TABLE(table), frame, 0, 4, 3, 8, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 1, 1);

    pfd = pango_font_description_new();
    pango_font_description_set_size(pfd, 20 * PANGO_SCALE);

    quit = gtk_button_new_with_label(" Quit ");

    /* SERVER */
    server = gtk_label_new("Server Window");
    gtk_widget_modify_font(server, pfd);

    /* TEXT VIEWER */
    buffer = gtk_text_buffer_new(NULL);

    /* Attaching all components to table */
    gtk_table_attach(GTK_TABLE(table), buffer, 0, 4, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    gtk_table_attach(GTK_TABLE(table), server, 0, 4, 2, 3, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    gtk_table_attach(GTK_TABLE(table), quit, 0, 4, 3, 4, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    gtk_widget_show_all(window);

    g_signal_connect(quit, "clicked", G_CALLBACK(gtk_widget_destroy), NULL);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

/*
int main(int argc, char *argv[]) {
    LogInfo *log = g_malloc(sizeof(*log));
    Program = argv[0];
    
#ifdef DEBUG
    printf("%s: Starting...\n", Program);
#endif
    gtk_init(&argc, &argv);
    StartWindow(log);
    gtk_main();
    g_free(log);
#ifdef DEBUG
    printf("%s: Exiting...\n", Program);
#endif
    return 0;
}
*//*
int FriendRequest (GtkWidget *parent, gchar *message) {
    GtkWidget *dialog, *label, *content_area;
    GtkDialogFlags flags;
    
    int response = 2;
    flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_dialog_new_with_buttons("New Friend Request",
                                         parent, flags,
                                         "_ACCEPT", GTK_RESPONSE_YES,
                                         "_DECLINE", GTK_RESPONSE_NO, NULL);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG (dialog));
    label = gtk_label_new (message);
    gtk_container_add (GTK_CONTAINER (content_area), label);
    gtk_widget_show_all (dialog);
    switch(gtk_dialog_run(dialog))
    {
        case GTK_RESPONSE_YES:
            response = 1;
            break;
        case GTK_RESPONSE_NO:
            response = 0;
            break;
    }
    gtk_widget_destroy(dialog);
    return response;
}
char open_dialog (GtkWidget *widget)
{
    GtkWidget *dialog, *grid, *label, *content_area, *textbox;
    GtkDialogFlags flags;
    
    char response[] = "";
    flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_dialog_new_with_buttons("Add New Friend",
                                         widget, flags,
                                         "_Search", GTK_RESPONSE_YES,
                                         "_Cancel", GTK_RESPONSE_CANCEL, NULL);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG (dialog));
    grid = gtk_table_new(2, 2, TRUE);
    gtk_container_add (GTK_CONTAINER (content_area), grid);
    label = gtk_label_new ("Enter username: ");
    gtk_table_attach_defaults(GTK_TABLE(grid), label, 0, 0, 1, 1);
    textbox = gtk_entry_new();
    gtk_entry_set_text(textbox, "<Value>");
    gtk_table_attach_defaults(GTK_TABLE(grid), textbox, 1, 0, 1, 1);
    gtk_widget_show_all (dialog);
    switch(gtk_dialog_run(dialog))
    {
        case GTK_RESPONSE_YES:
            response = "yes";
            break;
        case GTK_RESPONSE_CANCEL:
            break;
    }
    
    gtk_widget_destroy(dialog);
    return response;
}
*//*
int main(int argc, char *argv[]) {
    int PortNo;
    struct hostent *Server;
    LogInfo *log = g_malloc(sizeof(*log));
    Program = argv[0];
    
#ifdef DEBUG
    printf("%s: Starting...\n", argv[0]);
#endif
    if (argc < 3) {   
        fprintf(stderr, "Usage: %s hostname port\n", Program);
	    exit(10);
    }
    Server = gethostbyname(argv[1]);
    if (Server == NULL) {   
        fprintf(stderr, "%s: no such host named '%s'\n", Program, argv[1]);
        exit(10);
    }
    PortNo = atoi(argv[2]);
    if (PortNo <= 2000) {   
        fprintf(stderr, "%s: invalid port number %d, should be >2000\n",
		Program, PortNo);
        exit(10);
    }
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(PortNo);
    ServerAddress.sin_addr = *(struct in_addr*)Server->h_addr_list[0];
    gtk_init(&argc, &argv);
    StartWindow(log);
    gtk_main();
    g_free(log);
#ifdef DEBUG
    printf("%s: Exiting...\n", Program);
#endif
    return 0;
}
*/
