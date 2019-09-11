#ifndef HASH
#define HASH

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/**
 * Defining a struct that will contain all the information of each user.
 * The password will be stored in a char. (This is for simplicitiy --
 * we could generate a certain value based on their password instead
 * of actually storing the password). An int array will store all the 
 * user's friends. It will contain the key of each person the user is
 * friends with. The status determines whether the user is online,
 * offline, or away. Elements in the struct can be added/changed/removed.
**/

/**Jaeven's Note - this stores all stuff**/
typedef struct user_info{
    char *nickname;
    char *ip;
    char *chat_client;
    int *friends;
    int server_setting;
    int socket;
    bool private_chat;
    bool chat_request;
    bool chat_accept;
    //The following are for playing the chess game
    bool chess_request;
    bool chess_accept;
} user_info;

/**
 * Defining each item in the hash table as a struct with a key-value pair.
 * The struct will also contain a pointer to another struct holding all
 * the necessary data for the user. The key-value pair is just used to
 * locate/create/delete a specific user. There will also be a link to
 * the next entry if there happens to be a collision in the hash table.
 * The key and value must match just in case two users end up with the
 * same hash value and the same password (having the same usernames will
 * not be allowed). The password (encrypted one)  will be stored in value
**/

typedef struct user_ht_item{
    char *key;
    char *value;
    struct user_ht_item *link;
    user_info *user_data;
} user_ht_item;

/**
 * Defining the hash table. This will keep a current track of the size
 * of the hash table and the current amount of items. The total size
 * of the hash table will be hardcoded -- say 110 to include all
 * EECS 22L students, TA's and Q.V.
**/

typedef struct user_ht{
    int size;
    int curr_amt;

    //This double pointer is basically a list containg each 
    //hashtable item. The "first pointer" points to the 
    //list and the "second pointer" points to the address 
    //of a specific item in the hashtable. 
    //Accessing an item in the list: user_ht->user_items[index]
    user_ht_item **user_items;
} user_ht;

/**
 * The following functions handle basic processes of the hash table
**/

//Creates a new hash table (memory allocation)
user_ht *create_ht(void);

//Creates a new item in the hash table (memory allocation)
user_ht_item *create_ht_item(const char *key, const char *value, char *ip, char *nickname);

//Creates a user data struct for the new item above(memory allocation)
user_info *create_user_data(char *ip, char *nickname);

//Deletes a hash table (memory deallocation)
void del_ht(user_ht *user_ht);

//Deletes an item in the hash table (memory deallocation)
void del_ht_item(user_ht_item *item);

//Searches the hash table for an item given its key
bool user_ht_search(user_ht *user_ht, const char *key);

/*Returns a user profile*/
user_ht_item *user_ht_search_return(user_ht *hashtable, const char *key);

/*user_ht_search_password checks if given password is correct*/
bool user_ht_search_password(user_ht *user_ht, const char *key, const char *value);

//Inserts an item into the hash table and returns the key of the
//newly added item.
void user_ht_insert(user_ht *user_ht, const char *key, const char *value, char *ip, char *nickname);

//Deletes an item from the hash table and returns the value of
//the recently deleted item.
char *user_ht_delete(user_ht *user_ht, const char *key);

//Generates hash code
int hashCodeGenerator(const char *string);

/*Displays the Hash table*/
void displayHashtable(user_ht *hashtable);

/*Sets users to offline*/
void user_set_offline(user_ht *hashtable, const char *key);

/*Sets users to online*/
void user_set_online(user_ht *hashtable, const char *key);  
/**
 * The folllowing functions will be used by the uers
**/

//Allows the user to send a request to another user to be their friend
//Note: If Person A sends a friend request to Person B, Person A will
//be friends with Person B but Person B will not be friends with Person A
bool friend_req(user_ht *user_ht, const char *userkey, const char *friendkey);

//Allows the user to delete a friend on their friends list
bool friend_del(user_ht *user_ht, const char *userkey, const char *friendkey);

//Show the user's friend list
bool friend_list(user_ht* user_ht, const char *key);

//Allows the user to request to play a chess game with a certain user
bool play_chess(user_ht *user_ht, const char *userkey, const char *friendkey);

//Find user by their socket number
user_ht_item *socket_find_user(user_ht *database, int socket);

//Password encryption
char *store_pw(const char *pw);

/*EOF*/

#endif
