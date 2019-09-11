//Using select statement
#include "gui.h"
#include "hash.h"
#include "database.h"

#define h_addr h_addr_list[0]

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    LogInfo *log = g_malloc(sizeof(*log));
    printf("Type Quit to close connection\n");

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    log->portno = atoi(argv[2]); 

    log->server = gethostbyname(argv[1]);
    if (log->server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &(log->serv_addr), sizeof(log->serv_addr));
    log->serv_addr.sin_family = AF_INET;
    bcopy((char *)log->server->h_addr,
        (char *)&(log->serv_addr.sin_addr.s_addr),
        log->server->h_length);
    log->serv_addr.sin_port = htons(log->portno);
    log->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    /* GUI */
    gtk_init(&argc, &argv);
    StartWindow(log);
    gtk_main();
#ifdef DEBUG
	g_print("Ending GUI...\n");
#endif
    /* End GUI */

    g_free(log);
    return 0;
}

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
    log->usernameEntry = gtk_entry_new();
    log->passwordEntry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(log->passwordEntry), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(log->passwordEntry), '*');

    gtk_table_attach(GTK_TABLE(table), log->usernameEntry, 2, 3, 4, 5, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), log->passwordEntry, 2, 3, 5, 6, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

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
    log->username = gtk_entry_get_text(GTK_ENTRY(log->usernameEntry));
    log->password = gtk_entry_get_text(GTK_ENTRY(log->passwordEntry));
#ifndef DEBUG
    g_print("Username: %s\nPassword: %s\n", log->username, log->password);
#endif

    user_ht *database = doesFileExist("database.txt");
    if (user_ht_search(database, log->username)) {
        if (user_ht_search_password(database, log->username, log->password)) {
            Login(log);
#ifndef DEBUG
            g_print("Login protocol: %s\n", log->protocol);
#endif
            char *friends[FRIENDMAX];
            FriendsList(log, friends);
        }
        else { 
            g_print("Password is incorrect!\n");
        }
    }
    else {
        g_print("Username does not exist. Register if you are a new user!\n");
    }

/*    
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
}

void Login (LogInfo *log) {
#ifdef DEBUG
    g_print("%s: Login protocol starting...\n", Program);
#endif
    log->flag = "--Login"; 
    int size = strlen(log->username) + strlen(log->password) + 2;
    log->protocol = malloc(size);
    strcpy(log->protocol, log->username);
    strcat(log->protocol, " "); 
    strcat(log->protocol, log->password);
    
    char buffer[256];
    bzero(buffer,256);
    strcpy(buffer, log->flag);
    strtok(buffer, "\n");
    strcat(buffer, "$");
#ifdef DEBUG
    g_print("%s: Login protocol done...\n", Program);
#endif  
}

void CreateAccount (GtkWidget *widget, gpointer data) {
    /* Containers */
    GtkWidget *table;
    GtkWidget *frame;

    /* Labels */
    GtkWidget *title;
    GtkWidget *username;
    GtkWidget *password;
    GtkWidget *nickname;

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
    nickname = gtk_label_new("Nickname:");
    username = gtk_label_new("Username:");
    password = gtk_label_new("Password:");

    gtk_table_attach(GTK_TABLE(table), nickname, 1, 2, 3, 4, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), username, 1, 2, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), password, 1, 2, 2, 3, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    
    /* USER INPUT ENTRIES */
    log->usernameEntry = gtk_entry_new();
    log->passwordEntry = gtk_entry_new();
    log->nicknameEntry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(log->passwordEntry), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(log->passwordEntry), '*');

    gtk_table_attach(GTK_TABLE(table), log->usernameEntry, 2, 3, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), log->passwordEntry, 2, 3, 2, 3, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), log->nicknameEntry, 2, 3, 3, 4, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

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
    log->username = gtk_entry_get_text(GTK_ENTRY(log->usernameEntry));
    log->password = gtk_entry_get_text(GTK_ENTRY(log->passwordEntry));
    log->nickname = gtk_entry_get_text(GTK_ENTRY(log->nicknameEntry));
   
#ifndef DEBUG
    g_print("Username: %s\nPassword: %s\nNickname: %s\n", log->username, log->password, log->nickname);
#endif

    Register(log);
}

void Register (LogInfo *log) {
    log->flag = "--REGISTER"; 
    int size = strlen(log->username) + strlen(log->password) + strlen(log->nickname) + 2;
    log->protocol = malloc(size);
    strcpy(log->protocol, log->username);
    strcat(log->protocol, " "); 
    strcat(log->protocol, log->password); 
    strcat(log->protocol, " "); 
    strcat(log->protocol, log->nickname);
    
    char buffer[256];
    bzero(buffer,256);
    strcpy(buffer, log->flag);
    strtok(buffer, "\n");
    strcat(buffer, "$");
    
    ShowWindow(log->window[0], log);
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
    
    /* Set what page to start at (page 0) */
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
    
    button = gtk_button_new_with_label ("Sign Out");
    g_signal_connect (button, "clicked", G_CALLBACK (ShowWindow), log);
    gtk_container_add (GTK_CONTAINER (box), button);
    gtk_widget_show (button);

    button = gtk_button_new_with_label ("Close");
    g_signal_connect_swapped (button, "clicked", G_CALLBACK (Delete), chat);
    gtk_container_add (GTK_CONTAINER (box), button);
    gtk_widget_show (button);

    label = gtk_label_new (bufferl);
    gtk_notebook_prepend_page (GTK_NOTEBOOK (chat->notebook), frame, label);
    
    /* Let's append a bunch of pages to the notebook */
    int i;
    for (i = 0; i < 7; i++) {
        if (i == 0) sprintf(bufferl, "Zach");
        else if (i == 1) sprintf(bufferl, "Jaeven");
        else if (i == 2) sprintf(bufferl, "Jared");
        else if (i == 3) sprintf(bufferl, "AJ");
        else if (i == 4) sprintf(bufferl, "Robert");
        else if (i == 5) sprintf(bufferl, "Liyuan");
        else sprintf(bufferl, "QV");

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
    
    int n;
    int gui_enabled = 1;

    char buffer[256];
    

    if (log->sockfd < 0) error("ERROR opening socket");

    if (connect(log->sockfd,(struct sockaddr *) &(log->serv_addr), sizeof(log->serv_addr)) < 0)
        error("ERROR connecting");
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
            FD_SET(log->sockfd, &ReadFDs);

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
                if (FD_ISSET(log->sockfd, &ReadFDs)) {
                //printf("In read\n");
                    while(1)
                    {
                        //bzero(buffer, 256);
                        n = read(log->sockfd, buffer, 255);

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
        bzero(buffer,256);
        strcpy(buffer, log->flag);
        strtok(buffer, "\n");
        strcat(buffer, "$");
        n = write(log->sockfd, buffer, strlen(buffer));

        while(1)
        {
            //printf("Please enter the message: ");
            bzero(buffer,256);
            //fgets(buffer,255,stdin);
	   
	    if (gui_enabled)
        { 
            strcpy(buffer, log->protocol);
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
			n = write(log->sockfd, "--REGISTER$", 13);
			if (n < 0) error("ERROR writing to socket");
			sleep(1);

			info = strtok(NULL, "$");

			n = write(log->sockfd, info, strlen(info));
			if (n < 0) error("ERROR writing to socket");
		    }
		    else
		    {
			n = write(log->sockfd, "--LOGIN$", 10);
			if (n < 0) error("ERROR writing to socket");
			sleep(1);
			info = strtok(NULL, "$");

			//printf("Info: %s\n", info);

			strcat(info, "$");

			n = write(log->sockfd, info, strlen(info));
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

            n = write(log->sockfd, buffer, strlen(buffer));
            if (n < 0) error("ERROR writing to socket");

            bzero(buffer,256);
        }
    }
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
/*
void ServerWindow() {
    GtkWidget *window;
    
    // Containers 
    GtkWidget *table;
    GtkWidget *frame;

    // Labels 
    GtkWidget *server;

    // Buttons 
    GtkWidget *quit;

    // Text Buffer 
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

    // SERVER 
    server = gtk_label_new("Server Window");
    gtk_widget_modify_font(server, pfd);

    // TEXT VIEWER
    buffer = gtk_text_buffer_new(NULL);

    // Attaching all components to table
    gtk_table_attach(GTK_TABLE(table), buffer, 0, 4, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    gtk_table_attach(GTK_TABLE(table), server, 0, 4, 2, 3, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    gtk_table_attach(GTK_TABLE(table), quit, 0, 4, 3, 4, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    gtk_widget_show_all(window);

    g_signal_connect(quit, "clicked", G_CALLBACK(gtk_widget_destroy), NULL);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}
*/
