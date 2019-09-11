#include <stdio.h>
#include <stdlib.h>

#include "database.h"

/*int main()
{

   databasetest();

   user_ht *database = new_database("database.txt");
  

   displayHashtable(database);


   #ifdef DEBUG
  // if(doesFileExist("databasetest.txt")) printf("Hey I'm here\n");
   //if(!doesFileExist("databasetest.txt")) printf("KIA\n");
   #endif

   addFriend(database, user_ht_search_return(database,"Jaeven"), user_ht_search_return(database,"Liyuan"));
   printf("Added Liyuan\n");
   addFriend(database, user_ht_search_return(database,"Jaeven"), user_ht_search_return(database,"Liyuan"));
   printf("Added Liyuan again\n");
   addFriend(database, user_ht_search_return(database,"Jaeven"), user_ht_search_return(database,"StanTheMan"));		
   deleteFriend(database, user_ht_search_return(database,"Jaeven"), user_ht_search_return(database,"AJ"));
   deleteFriend(database, user_ht_search_return(database,"Jaeven"), user_ht_search_return(database,"AJ"));
   UpdateTextFile("databasetest.txt", database);
   
   return 0;
}*/

user_ht *new_database(const char *filename) {
    user_ht *database = create_ht();
    //Read from file to get all information

    FILE *file = NULL;

    if((file = fopen(filename, "r")) == NULL)
    {
        perror("Failed opening file");
        exit(5);
    }

    database = get_db(file, database);
    database = get_user_total(file, database);
    database = get_user_info(file, database);
    database = get_user_info(file, database);
    database = get_user_info(file, database);
    database = get_user_info(file, database);

    //IP's will not be kept in the database since they are dynamic

    //The database is now created from the text file
    fclose(file);
    return database;
}

user_ht *get_db(FILE *filename, user_ht *database) {
    char *keyword = malloc((30+1)*sizeof(char));
    char *db = malloc((30+1)*sizeof(char));
    
    //Input: DATABASE:No-Clue-Crew-Database
    fscanf(filename, "%s", keyword);
    keyword = strtok (keyword, ":");
    #ifdef DEBUG
    printf ("%s\n",keyword);
    #endif
    db = strtok (NULL, "\n");
    if (strcmp(db, "No-Clue-Crew-User-Database")) exit(0);
    #ifdef DEBUG
    printf("%s\n", db);
    #endif

    return database;
}


user_ht *get_user_total(FILE *filename, user_ht *database) {
    //Input: USERTOTAL:6
    char *keyword = malloc((11+1)*sizeof(char));
    char *total = malloc((11+1)*sizeof(char));
    
    fscanf(filename, "%s", keyword);
    keyword = strtok (keyword, ":");
    #ifdef DEBUG
    printf ("%s\n",keyword);
    #endif
    total = strtok (NULL, "\n");
    #ifdef DEBUG
    printf("%d\n", atoi(total));
    #endif
    //database->curr_amt = atoi(total);

    return database;
}

user_ht *get_user_info(FILE *filename, user_ht *database) {
        
    //Input: USERS:Zach,AJ,Jaeven,Jared,Robert,Liyuan,
    /*Input: PASSWORDS:_1hVj2EgtZtLN{oj"p^>IwyxfvrX!'r3JhA)PdqIw;GCd@r%tOt+tw)"Wi1T.IQ, 
    		        g1hVj2Emt22LN{sj"p^>IyyxfvrX!'Y3JhA'PdqIw;UCT@t%:[t+yw)"a(1T.IQ,
    			s1hVj2EvtZwLN{yj"p^>I|yxfvrX!'Y3JhA$PdqIw;jCT@t%:mt+yw)"p(1T.IQ,
    			p1hVj22stZwLN{vj"p^>IyyxfvrX|'Y3J3A!PdqIw;$CT@t%:jt+yw)"m(1T.IQ,
    			g1hVj2EjtZwLN{wj"pj>IyyxfvrX!'Y3JhA'PdqIw;UCw@t%:[t+yw)"aw1T.IQ,
    			c1hVj2Ejt22LN{qj"p^>IxyxfvrX!'Y3JhA(PdqIw;NCT@t%:Ut+yw)"\(1T.IQ,
    */
    //Input: NICKNAME:Zachary,Benedict,J,Ja,Rob,Lee,
    
    char *keyword = malloc((999+1)*sizeof(char));
    char *friends = malloc((999+1)*sizeof(char));
    int counter = 0;
    static int function_count = 1;
    static int key_list[110] = {0};
    if (function_count == 1) {
	memset(key_list, -1, (110 * sizeof(int)));
    }
    if (function_count == 4) {
	return get_friends(filename, database, key_list);
    }


    fscanf(filename, "%s", keyword);
    keyword = strtok (keyword, ":");
    #ifdef DEBUG
    printf ("%s\n",keyword);
    #endif
    friends = strtok (NULL, ",");
    
    switch(function_count) {
        case 1:
	    user_ht_insert(database, friends, "password", "192.168.111.111", "nickname");
	    strcpy(database->user_items[hashCodeGenerator(friends)]->key, friends);
	    key_list[counter] = hashCodeGenerator(friends);
	    break;
        case 2:
	    strcpy(database->user_items[key_list[counter]]->value, friends);
	    break;
        case 3:
	    strcpy(database->user_items[key_list[counter]]->user_data->nickname, friends);
	    break;
    }

        while(friends != NULL)
        {
	    counter++;
	    #ifdef DEBUG
            printf("%s\n", friends);
	    #endif
            friends = strtok (NULL, ",");
	    if (friends == NULL) break;
	    switch(function_count) {
                case 1:
		    user_ht_insert(database, friends, "password", "192.168.111.111", "nickname");
                    strcpy(database->user_items[hashCodeGenerator(friends)]->key, friends);
		    key_list[counter] = hashCodeGenerator(friends);
            	    break;
        	case 2:
                    strcpy(database->user_items[key_list[counter]]->value, friends);
            	    break;
        	case 3:
                    strcpy(database->user_items[key_list[counter]]->user_data->nickname, friends);
                    break;
	    }
        }
        //if (!counter) printf("%s\n", friends);

    function_count += 1;

    return database;
}

user_ht *get_friends(FILE *filename, user_ht *database, int *key_list) {
    //Input: FRIENDS:AJ-Jaeven-Jared,Jaeven-Zach-Robert,Liyuan-AJ,Jared-Robert-Zach,    
        
    char *keyword = malloc((999+1)*sizeof(char));
    char *user = malloc((999+1)*sizeof(char));
 
   //char *user_split = malloc((999+1)*sizeof(char));
    char user_split[30];
    const char *temp[110];
    int counter = 0;
    
    fscanf(filename, "%s", keyword);
    keyword = strtok (keyword, ":");
    #ifdef DEBUG
    printf ("%s\n",keyword);
    #endif
    user = strtok (NULL, ",");
    temp[counter] = user;
    counter ++;

    while(user != NULL)
    {
	#ifdef DEBUG
        printf("%s\n", user);
	#endif
        user = strtok (NULL, ",");
        temp[counter] = user;
        if (user !=NULL) counter++;
    }
    
    int j = 0;
    for (int i =  0; i < counter; i++) {
        strcpy(user_split, temp[i]);
        if(strcmp(user_split, "-1") == 0){
            continue;
        }
        user = strtok (user_split, "-");
        database->user_items[key_list[i]]->user_data->friends[j] = hashCodeGenerator(user);       

        while(user != NULL)
        {
	    j++;
	    #ifdef DEBUG
            printf("%s\n", user);
	    #endif
            user = strtok (NULL, "-");
	    if (user == NULL) break;
	    database->user_items[key_list[i]]->user_data->friends[j] = hashCodeGenerator(user);
        }
	j = 0;
    }

    return database;
}    

bool databasetest()
{

FILE *File = fopen("database.txt", "w");

if (File == NULL) exit(0);

const char *Keyword_1 = "DATABASE:";
const char *Keyword_2 = "USERTOTAL:";
const char *Keyword_3 = "USERS:";
const char *Keyword_4 = "PASSWORDS:";
const char *Keyword_5 = "NICKNAMES:";
const char *Keyword_6 = "FRIENDS:";

char *db = "No-Clue-Crew-User-Database";
int user_total = 6;
char *user1 = "Zach";
char *user2 = "AJ";
char *user3 = "Jaeven";
char *user4 = "Jared";
char *user5 = "Robert";
char *user6 = "Liyuan";
char *user1_pw = "_1hVj2EgtZtLN{oj\"p^>IwyxfvrX!'r3JhA)PdqIw;GCd@r%tOt+tw)\"Wi1T.IQ";
char *user2_pw = "g1hVj2Emt22LN{sj\"p^>IyyxfvrX!'Y3JhA'PdqIw;UCT@t%:[t+yw)\"a(1T.IQ";
char *user3_pw = "s1hVj2EvtZwLN{yj\"p^>I|yxfvrX!'Y3JhA$PdqIw;jCT@t%:mt+yw)\"p(1T.IQ";
char *user4_pw = "p1hVj22stZwLN{vj\"p^>IyyxfvrX|'Y3J3A!PdqIw;$CT@t%:jt+yw)\"m(1T.IQ";
char *user5_pw = "g1hVj2EjtZwLN{wj\"pj>IyyxfvrX!'Y3JhA'PdqIw;UCw@t%:[t+yw)\"aw1T.IQ";
char *user6_pw = "c1hVj2Ejt22LN{qj\"p^>IxyxfvrX!'Y3JhA(PdqIw;NCT@t%:Ut+yw)\"\\(1T.IQ";


fprintf(File, "%s", Keyword_1);
fprintf(File, "%s\n", db);

fprintf(File, "%s", Keyword_2);
fprintf(File, "%d\n", user_total);

fprintf(File, "%s", Keyword_3);
fprintf(File, "%s", user1);
fprintf(File, ",");
fprintf(File, "%s", user2);
fprintf(File, ",");
fprintf(File, "%s", user3);
fprintf(File, ",");
fprintf(File, "%s", user4);
fprintf(File, ",");
fprintf(File, "%s", user5);
fprintf(File, ",");
fprintf(File, "%s", user6);
fprintf(File, ",\n");

fprintf(File, "%s", Keyword_4);
fprintf(File, "%s", user1_pw);
fprintf(File, ",");
fprintf(File, "%s", user2_pw);
fprintf(File, ",");
fprintf(File, "%s", user3_pw);
fprintf(File, ",");
fprintf(File, "%s", user4_pw);
fprintf(File, ",");
fprintf(File, "%s", user5_pw);
fprintf(File, ",");
fprintf(File, "%s", user6_pw);
fprintf(File, ",\n");

fprintf(File, "%s", Keyword_5);
//Same idea as shown above for getting nicknames
char *user1_nickname = "Zachary";
char *user2_nickname = "Benedict";
char *user3_nickname = "J";
char *user4_nickname = "Ja";
char *user5_nickname = "Rob";
char *user6_nickname = "Lee";

fprintf(File, "%s", user1_nickname);
fprintf(File, ",");
fprintf(File, "%s", user2_nickname);
fprintf(File, ",");
fprintf(File, "%s", user3_nickname);
fprintf(File, ",");
fprintf(File, "%s", user4_nickname);
fprintf(File, ",");
fprintf(File, "%s", user5_nickname);
fprintf(File, ",");
fprintf(File, "%s", user6_nickname);
fprintf(File, ",\n");



fprintf(File, "%s", Keyword_6);
//With the friends int array, run the hash functions and find the key
//The key value will be the user's name
//Find the length of the int array
//Example:
//Assume length of int array is  3 in this case.
//Assume hashtable->user_ht_items[0]->Key = AJ 
//	 hashtable->user_ht_items[1]->Key = Jaeven
//	 hashtable->user_ht_items[2]->Key = Jared
//for (int i = 0; i < (len - 1); i++)
//hashfunc(friends[i])
//char *temp1 = hashtable->user_ht_items[i]->Key;
//char *temp2 = hashtable->user_ht_items[i + 1]->Key;
//strcat(temp1,"-")
//strcat(temp1, temp2);
//This would create: AJ-Jaeven-Jared

//The order of the hardcoded values below could be different
//when the actual functions are used to the info
char *user1_friends = "AJ-Jaeven-Jared";
char *user2_friends = "Jaeven-Zach-Robert";
char *user3_friends = "Apples-Oranges";
char *user4_friends = "QV-Kavianpour";
char *user5_friends = "Liyuan-AJ";
char *user6_friends = "Jared-Robert-Zach";

fprintf(File, "%s", user1_friends);
fprintf(File, ",");
fprintf(File, "%s", user2_friends);
fprintf(File, ",");
fprintf(File, "%s", user3_friends);
fprintf(File, ",");
fprintf(File, "%s", user4_friends);
fprintf(File, ",");
fprintf(File, "%s", user5_friends);
fprintf(File, ",");
fprintf(File, "%s", user6_friends);
fprintf(File, ",\n");


fclose(File);

return true;
}


//Checks if a .txt file exists. Returns true if file exists, false if not.
user_ht *doesFileExist(char *filename){
    FILE *file = fopen(filename, "r");
    if(file == NULL) 
    {
	fclose(file);
	user_ht *database = create_ht();
	return database;
    }
    else 
    {
	fclose(file);
	user_ht *database = new_database(filename);
	return database;
    }
}

//Stores contents of user database into .txt file.
void UpdateTextFile(char *filename, user_ht *database){
	const char *Keyword_1 = "DATABASE:";
	const char *Keyword_2 = "USERTOTAL:";
	const char *Keyword_3 = "USERS:";
	const char *Keyword_4 = "PASSWORDS:";
	const char *Keyword_5 = "NICKNAMES:";
	const char *Keyword_6 = "FRIENDS:";
	char *dbase = "No-Clue-Crew-User-Database";
	
	//FILE *file = fopen(filename, "w");	/*Opens the text file for writing*/
	FILE *file = fopen(filename, "w");
	
	//writes the DATABASE title to the text file
	fprintf(file, "%s", Keyword_1);
	fprintf(file, "%s\n", dbase);
	
	//writes the USERTOTAL to the text file
	fprintf(file, "%s", Keyword_2);
	fprintf(file, "%d\n", (database->curr_amt));
	
	//writes the USERS of the database to the text file
	fprintf(file, "%s", Keyword_3);
	int width = database->size;
	for(int x = 0; x < width; x++){
	    if(database->user_items[x]){
		    fprintf(file, "%s,", database->user_items[x]->key);
		    //user_ht_item *next = database->user_items[x]->link;
		    if (database->user_items[x]->link){
		      user_ht_item *next = database->user_items[x]->link;
              while(next){
			      fprintf(file, "%s,", next->key);
			      next = next->link;
		  	  }
	       } 
	    }
	}

	fprintf(file, "%s\n", "");
	
	//writes the user PASSWORDS in the database to the text file
	fprintf(file, "%s", Keyword_4);
	for(int x = 0; x < width; x++){
	    if(database->user_items[x]){
		    fprintf(file, "%s,", database->user_items[x]->value);
		    if (database->user_items[x]->link){
		      user_ht_item *next = database->user_items[x]->link;
              while(next){
			      fprintf(file, "%s,", next->value);
			      next = next->link;
		  	  }
	       } 
	    }
	}
	fprintf(file, "%s\n", "");

	//writes the user Nicknames in the database to the text file
	fprintf(file, "%s", Keyword_5);
    for(int x = 0; x < width; x++){
      if(database->user_items[x]){
          fprintf(file, "%s,", database->user_items[x]->user_data->nickname);
          //user_ht_item *next = database->user_items[x]->user_data->link;
          if (database->user_items[x]->link){
              user_ht_item *next = database->user_items[x]->link;
              while(next){
                  fprintf(file, "%s,", next->user_data->nickname);
                  next = next->link;
              }      
          }
      }
    }
	fprintf(file, "%s\n", "");
	
	//writes the user FRIENDS in the database to the text file
	fprintf(file, "%s", Keyword_6);
	char *name = malloc(30 * sizeof(*name));

	//printf("Width: %d\n", width);
	
	int n = 0, m = 0;
	
	for(int x = 0; x < width; x++){
		if(database->user_items[x]){
			if(database->user_items[x]->user_data->friends){
				while(database->user_items[x]->user_data->friends[n] != -1){
				    n++;
				}
                #ifdef DEBUG
                    printf("Friends Counter: %d\n", n);
                #endif

                if(n == 0){
                    fprintf(file, "%s,", "-1");
                }      

	            else if(n != 0){
                    for(int i=0; i < n; i++){
                        #ifdef DEBUG
                            printf("Friend: %s\n", database->user_items[database->user_items[x]->user_data->friends[i]]->key);
                        #endif
                        strcpy(name, database->user_items[database->user_items[x]->user_data->friends[i]]->key);
                        if(!(i == (n-1)))
                            fprintf(file, "%s-", name);
                        else
                            fprintf(file, "%s", name);
                    
                    }
                    fprintf(file, "%c", ',');
                    n = 0;
                }
            }
		
            if(database->user_items[x]->link){
		        user_ht_item *next = database->user_items[x]->link;
			    while(next){
			        while(next->user_data->friends[m] != -1){
				        m++;
				    }
                    if( m == 0){
                        fprintf(file, "%s,", "-1");
                    }
                    else if(m != 0){
                        for(int j = 0; j < m; j++){
                            strcpy(name, database->user_items[next->user_data->friends[j]]->key);
                            if(!(j == (m-1)))
                                fprintf(file, "%s-", name);
                            else
                                fprintf(file, "%s", name);
                        }
                        fprintf(file, "%c", ',');
                        next = next->link;
                    }
                }   
            }     
		}
	}
	fprintf(file, "%s\n", "");
	
	//close file after writing is complete
	fclose(file);

}

bool addFriend(user_ht *database, user_ht_item *user, user_ht_item *uFriend){
	user_ht_item *temp;
	if(user == NULL || uFriend == NULL){
		#ifdef DEBUG
                printf("User or Friend is null.\n");
                #endif
                return false;	
	}
	/*Ends function if either user or friend does not exist in database*/
	if(hashCodeGenerator(user->key) == hashCodeGenerator(uFriend->key)){
		#ifdef DEBUG
                printf("You cannot add yourself.\n");
                #endif
		return false;
	}
	else if(user_ht_search(database, uFriend->key) != true || user_ht_search(database,user->key) != true){
		#ifdef DEBUG
		printf("User does not exist and cannot be added.\n");
		#endif
		return false;		
	}
	else{
		temp = user_ht_search_return(database,user->key);
		for(int i = 0; i < sizeof(temp->user_data->friends); i++){
			if(temp->user_data->friends[i] == hashCodeGenerator(uFriend->key)){
                                #ifdef DEBUG
                                printf("This person is already your friend.\n");
                                #endif
				return false;
                        }
			/*If no friends, add friend to the first index of Friend array*/
			else if(temp->user_data->friends[i] == -1){
				temp->user_data->friends[i] = hashCodeGenerator(uFriend->key);
				return true;
			}
			
			/*If there are existing friends, place new friend in first possible empty space*/
			else if(!temp->user_data->friends[i]){
				temp->user_data->friends[i] = hashCodeGenerator(uFriend->key);
                                return true;
			}
		}	
	}
		#ifdef DEBUG
                printf("Friend's list full. No new friends can be added\n");
                #endif	
	return false;
}

bool deleteFriend(user_ht *database, user_ht_item *user, user_ht_item *uFriend){
	user_ht_item *temp;
	int lastFriendHashcode = 0;
	if(user == NULL || uFriend == NULL){
                #ifdef DEBUG
                printf("User or Friend is null.\n");
                #endif
                return false;
        }
        /*Ends function if either user or friend does not exist in database*/
        if(user_ht_search(database, uFriend->key) != true || user_ht_search(database,user->key) != true){
                #ifdef DEBUG
                printf("User does not exist and cannot be deleted.\n");
                #endif
                return false;
        }
        else{
		temp = user_ht_search_return(database,user->key);	
		if(temp->user_data->friends[0] == -1){
			#ifdef DEBUG
                	printf("Friend's list is empty and nobody can be deleted.\n");
                	#endif
			return false;
		}
		else if (temp->user_data->friends[0] == hashCodeGenerator(uFriend->key)){
			temp->user_data->friends[0] = -1;
			return true;
		}
		/*Work backwards from friends list to find latest non-null entry*/
                for(int i = (sizeof(temp->user_data->friends)-1); i > 0; i--){
         		if(temp->user_data->friends[i] != -1){
				lastFriendHashcode = temp->user_data->friends[i];
				temp->user_data->friends[i] = -1;
				break;
			}             
                }
		for(int i = 0; i < sizeof(temp->user_data->friends); i++){
                	if(temp->user_data->friends[i] == hashCodeGenerator(uFriend->key)){
				temp->user_data->friends[i] = lastFriendHashcode;
				return true;	
			}       
                        
                }
        }
                #ifdef DEBUG
                printf("This user is not your friend and cannot be deleted.\n");
                #endif
	return false;
}
