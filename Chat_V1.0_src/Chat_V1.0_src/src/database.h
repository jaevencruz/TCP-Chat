#ifndef DATABASE
#define DATABASE

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

//Shows all the information of an item given its key.
bool show_ht_item(user_ht *user_ht, const char *key);

//Allows the server to change the password of a user
bool new_pw_ht_item(user_ht *user_ht, const char* key, char* new_pw);

//Checks if the text file for the database exists.
bool database_txt_exist(const char *filename);

//Creates a blank database to store in a .txt file
user_ht *new_database(const char *filename);

//Reads the .txt file and creates the database
user_ht *create_database(const char *filename);

//Saves the database to the .txt file
bool save_database(user_ht *user_ht, const char *filename);

//Handles the database
bool database_main(const char *filename);

//Database test
bool databasetest();

//Functions to read from .txt file
user_ht *get_db(FILE *filename, user_ht *database);
user_ht *get_user_total(FILE *filename, user_ht *database);
user_ht *get_user_info(FILE *filename, user_ht *database);
user_ht *get_friends(FILE *filename, user_ht *datbase, int *key_list);

user_ht *doesFileExist(char *filename);
void UpdateTextFile(char *filename, user_ht *database);

/*Uses parameters database, user that is adding the friend, and the uFriend that is getting added*/
bool addFriend(user_ht *database, user_ht_item *user, user_ht_item *uFriend);
bool deleteFriend(user_ht *database, user_ht_item *user, user_ht_item *uFriend);
#endif
