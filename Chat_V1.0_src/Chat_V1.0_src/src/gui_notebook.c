#include <gtk/gtk.h>
#include <stdio.h>

typedef struct LoginInfo {
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry3;
} LogInfo;

void StartWindow (GtkWidget *widget, LogInfo *log, gpointer data);
void GetEntry (GtkWidget *widget, gpointer data);
void CreateAccount (GtkWidget *widget, gpointer data);
void CreateNotebook (GtkWidget *widget, gpointer data);


int main(int argc, char *argv[]) {
    GtkWidget *widget = NULL;
    gpointer data = NULL;
    LogInfo *log = g_malloc(sizeof(*log));

    gtk_init(&argc, &argv);
    StartWindow(widget, log, data);
    gtk_main();

    g_free(log);

    return 0;
}

void StartWindow (GtkWidget *widget, LogInfo *log, gpointer data) {

    GtkWidget *window;
    GtkWidget *table;
    GtkWidget *frame;

    GtkWidget *login;
    GtkWidget *chatnchess;
    GtkWidget *username;
    GtkWidget *password;

    GtkWidget *signin;
    GtkWidget *createnew;
    GtkWidget *notebook;

    PangoFontDescription *pfd;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), "ChatNChess");
    gtk_container_set_border_width(GTK_CONTAINER(window), 15);

    table = gtk_table_new(5, 4, FALSE);
    gtk_container_add(GTK_CONTAINER(window), table);

    frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_ETCHED_IN);

    gtk_table_attach(GTK_TABLE(table), frame, 0, 4, 3, 10, 
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 1, 1);

    pfd = pango_font_description_new();
    pango_font_description_set_size(pfd, 20 * PANGO_SCALE);
    
    /* CHATNCHESS */
    chatnchess = gtk_label_new("Chat N Chess");
    gtk_widget_modify_font(chatnchess, pfd);

    gtk_table_attach(GTK_TABLE(table), chatnchess, 0, 4, 1, 2,
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    /* LOGIN USERNAME AND PASSWORD */
    login = gtk_label_new("Login");
    gtk_widget_modify_font(login, pfd); 
    username = gtk_label_new("Username:");
    password = gtk_label_new("Password:");
    
    gtk_table_attach(GTK_TABLE(table), login, 0, 4, 3, 4, 
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), username, 1, 2, 4, 5, 
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), password, 1, 2, 5, 6, 
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    
    /* USER INPUT ENTRIES */
    log->entry1 = gtk_entry_new();
    log->entry2 = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(log->entry2), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(log->entry2), '*');

    gtk_table_attach(GTK_TABLE(table), log->entry1, 2, 3, 4, 5, 
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), log->entry2, 2, 3, 5, 6, 
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), log->entry3, 2, 3, 5, 6,
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    /* SIGN IN AND CREATE NEW ACCOUNT */
    signin = gtk_button_new_with_label("Sign In");
    createnew = gtk_button_new_with_label("Create New Account");
    notebook = gtk_button_new_with_label("Open a notebook");


    gtk_table_attach(GTK_TABLE(table), signin, 1, 3, 6, 7,
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), createnew, 1, 3, 7, 8,
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), notebook, 1, 3, 8, 9,
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
	
    
    gtk_widget_show_all(window);

    g_signal_connect(signin, "clicked", G_CALLBACK(GetEntry), (gpointer)log);
    g_signal_connect(createnew, "clicked", G_CALLBACK(CreateAccount), (gpointer)log);
    g_signal_connect(notebook, "clicked", G_CALLBACK(GetEntry), (gpointer)log);

    g_signal_connect(window, "destroy",
    G_CALLBACK(gtk_main_quit), NULL);
}

void GetEntry (GtkWidget *widget, gpointer data) 
{
    LogInfo *log = (LogInfo*)data;
    const gchar *string1 = gtk_entry_get_text(GTK_ENTRY(log->entry1));
    const gchar *string2 = gtk_entry_get_text(GTK_ENTRY(log->entry2));
    const gchar *string3 = gtk_entry_get_text(GTK_ENTRY(log->entry3));

    
#ifndef DEBUG
 	   g_print("%s\n%s\n%s\n", string1, string2, string3);
            /*gtk_entry_get_text(GTK_ENTRY(log->entry1)), 
            gtk_entry_get_text(GTK_ENTRY(log->entry2)));*/ 
#endif
}

void CreateAccount (GtkWidget *widget, gpointer data) {
    /* Containers */
    GtkWidget *window;
    GtkWidget *table;
    GtkWidget *frame;
    /* Labels */
    GtkWidget *title;
    /* Entries */
    GtkWidget *username;
    GtkWidget *password;
    GtkWidget *confirm;
    /* Buttons */
    GtkWidget *signin;
    GtkWidget *createnew;
    /*notebook*/
    


    PangoFontDescription *pfd;
    
    LogInfo *log = (LogInfo*)data;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), "ChatNChess");
    gtk_container_set_border_width(GTK_CONTAINER(window), 15);

    table = gtk_table_new(5, 4, FALSE);
    gtk_container_add(GTK_CONTAINER(window), table);

    frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_ETCHED_IN);

    gtk_table_attach(GTK_TABLE(table), frame, 0, 4, 3, 8, 
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 1, 1);

    pfd = pango_font_description_new();
    pango_font_description_set_size(pfd, 20 * PANGO_SCALE);
    
    /* Title */
    title = gtk_label_new("Create Account");
    gtk_widget_modify_font(title, pfd);

    gtk_table_attach(GTK_TABLE(table), title, 0, 4, 1, 2,
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    /* LOGIN USERNAME AND PASSWORD */
    username = gtk_label_new("Username:");
    password = gtk_label_new("Password:");
    confirm = gtk_label_new("Confirm Password:");
    
    gtk_table_attach(GTK_TABLE(table), username, 1, 2, 4, 5, 
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), password, 1, 2, 5, 6, 
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), confirm, 0, 4, 3, 4, 
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    
    /* USER INPUT ENTRIES */
    log->entry1 = gtk_entry_new();
    log->entry2 = gtk_entry_new();
    log->entry3 = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(log->entry2), FALSE);
    gtk_entry_set_visibility(GTK_ENTRY(log->entry3), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(log->entry2), '*');
    gtk_entry_set_invisible_char(GTK_ENTRY(log->entry3), '*');

    gtk_table_attach(GTK_TABLE(table), log->entry1, 2, 3, 4, 5, 
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), log->entry2, 2, 3, 5, 6, 
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), log->entry3, 2, 3, 5, 6, 
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    /* SIGN IN AND CREATE NEW ACCOUNT */
    signin = gtk_button_new_with_label("Return To Sign In");
    createnew = gtk_button_new_with_label("Create New Account");
    
    gtk_table_attach(GTK_TABLE(table), signin, 1, 3, 6, 7,
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), createnew, 1, 3, 7, 8,
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    
    gtk_widget_show_all(window);

    g_signal_connect(signin, "clicked", G_CALLBACK(StartWindow), (gpointer)log);
    g_signal_connect(createnew, "clicked", G_CALLBACK(GetEntry), (gpointer)log);

    g_signal_connect(window, "destroy",
    G_CALLBACK(gtk_main_quit), NULL);

}

void CreateNotebook (GtkWidget *widget, gpointer data)
{
 
    /* Containers */
    GtkWidget *window;
    GtkWidget *table;
    GtkWidget *frame;
    GtkWidget *notebook;
    /* titles */
    GtkWidget *title;
    /* Entries */
    GtkWidget *label;
    /* Buttons */
    GtkWidget *button;
    GtkWidget *signin;
    int i;
    char bufferf[32];
    char bufferl[32];

    PangoFontDescription *pfd;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), "Notebook");
    gtk_container_set_border_width(GTK_CONTAINER(window), 15);

    /*create the table*/
    table = gtk_table_new(10,4, FALSE);
    gtk_container_add(GTK_CONTAINER(window), table);
    
    /*create a new notebook*/
    notebook = gtk_notebook_new ();
    gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook), GTK_POS_TOP);
    gtk_table_attach_defaults (GTK_TABLE (table), notebook, 0, 6, 0, 1);
    /*gtk_widget_show (notebook);*/
    
    for (i = 0; i < 5; i++) 	
    {
	sprintf(bufferf, "Append Frame %d", i + 1);
	sprintf(bufferl, "Page %d", i + 1);
	
	frame = gtk_frame_new (bufferf);
	gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
	gtk_widget_set_size_request (frame, 100, 75);
	gtk_widget_show (frame);
	
	label = gtk_label_new (bufferf);
	gtk_container_add (GTK_CONTAINER (frame), label);
	gtk_widget_show (label);
	
	label = gtk_label_new (bufferl);
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
    }
    frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_ETCHED_IN);

    gtk_table_attach(GTK_TABLE(table), frame, 0, 4, 3, 8, 
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 1, 1);

    pfd = pango_font_description_new();
    pango_font_description_set_size(pfd, 20 * PANGO_SCALE);
    
    /* Title */
    title = gtk_label_new("Prepend frame");
    gtk_widget_modify_font(title, pfd);

    gtk_table_attach(GTK_TABLE(table), title, 0, 4, 1, 2,
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

        /* Now let's add a page to a specific spot */
    button = gtk_check_button_new_with_label ("Check me please!");
    gtk_widget_set_size_request (button, 100, 75);
    gtk_widget_show (button);
   
    label = gtk_label_new ("Add page");
    gtk_notebook_insert_page (GTK_NOTEBOOK (frame), button, label, 2);
    
    /* Now finally let's prepend pages to the notebook */
    for (i = 0; i < 5; i++) 
    {
	sprintf (bufferf, "Prepend Frame %d", i + 1);
	sprintf (bufferl, "PPage %d", i + 1);
	
	frame = gtk_frame_new (bufferf);
	gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
	gtk_widget_set_size_request (frame, 100, 75);
	gtk_widget_show (frame);
	
	label = gtk_label_new (bufferf);
	gtk_container_add (GTK_CONTAINER (frame), label);
	gtk_widget_show (label);
	
	label = gtk_label_new (bufferl);
	gtk_notebook_prepend_page (GTK_NOTEBOOK (notebook), frame, label);
    }
    
    /* USER INPUT ENTRIES */
    LogInfo *log = g_malloc(sizeof(*log));
    log->entry1 = gtk_entry_new();
    log->entry2 = gtk_entry_new();
    log->entry3 = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(log->entry2), FALSE);
    gtk_entry_set_visibility(GTK_ENTRY(log->entry3), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(log->entry2), '*');
    gtk_entry_set_invisible_char(GTK_ENTRY(log->entry3), '*');

    gtk_table_attach(GTK_TABLE(table), log->entry1, 2, 3, 4, 5, 
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), log->entry2, 2, 3, 5, 6, 
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), log->entry3, 2, 3, 5, 6, 
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    /* Set what page to start at (page 4) */
    gtk_notebook_set_current_page (GTK_NOTEBOOK (notebook), 3);

    /* Create a bunch of buttons */
    signin = gtk_button_new_with_label("Return To Sign In");
    /*button = gtk_button_new_with_label("Page");*/
    
    /*gtk_table_attach(GTK_TABLE(table), button, 1, 3, 6, 7,
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);*/
    gtk_table_attach(GTK_TABLE(table), signin, 1, 3, 7, 8,
    GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    gtk_widget_show_all(window);

    g_signal_connect(signin, "clicked", G_CALLBACK(GetEntry), (gpointer)log);
    /*g_signal_connect(createnew, "clicked", G_CALLBACK(Get), NULL);*/

    g_signal_connect(window, "destroy",
    G_CALLBACK(gtk_main_quit), NULL);

    g_free(log);

    button = gtk_button_new_with_label ("next page");
    g_signal_connect_swapped (button, "clicked",G_CALLBACK (gtk_notebook_next_page),
			      notebook);
    gtk_table_attach_defaults (GTK_TABLE (table), button, 1, 2, 1, 2);
    gtk_widget_show (button);
    
    gtk_widget_show_all(window);

    
}

/*    
    gtk_widget_show_all(window);

    g_signal_connect(signin, "clicked", G_CALLBACK(StartWindow), (gpointer)log);
    g_signal_connect(createnew, "clicked", G_CALLBACK(GetEntry), (gpointer)log);

    g_signal_connect(window, "destroy",
    G_CALLBACK(gtk_main_quit), NULL);

}*/


/*static void rotate_book( GtkButton   *button, GtkNotebook *notebook )
{
    gtk_notebook_set_tab_pos (notebook, (notebook->tab_pos + 1) % 4);
}*/

/* Add/Remove the page tabs and the borders */
/*static void tabsborder_book( GtkButton   *button, GtkNotebook *notebook )
{
    gint tval = FALSE;
    gint bval = FALSE;
    if (notebook->show_tabs == 0)
	    tval = TRUE; 
    if (notebook->show_border == 0)
	    bval = TRUE;
    
    gtk_notebook_set_show_tabs (notebook, tval);
    gtk_notebook_set_show_border (notebook, bval);
}*/

/* Remove a page from the notebook */
/*static void remove_book( GtkButton   *button,
                         GtkNotebook *notebook )
{
    gint page;
    
    page = gtk_notebook_get_current_page (notebook);
    gtk_notebook_remove_page (notebook, page);
    
    gtk_widget_queue_draw (GTK_WIDGET (notebook));
}*/

/*static gboolean delete( GtkWidget *widget,
                        GtkWidget *event,
                        gpointer   data )
{
    gtk_main_quit ();
    return FALSE;
}*/
