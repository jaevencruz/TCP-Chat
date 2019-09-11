#include "hash.h"

/*This function creates the hashtable*/
user_ht *create_ht(void) {
    /*Jaeven's note: Do not name the line b*/
    user_ht *hashtable = malloc(sizeof(user_ht));
    if (! hashtable){
                perror("Out of memory! Aborting . . .");
                exit(10);
        }
    hashtable->size = 110;
    hashtable->curr_amt = 0;
    hashtable->user_items = malloc(hashtable->size * sizeof(user_ht_item));
    memset(hashtable->user_items, 0, (hashtable->size * sizeof(user_ht_item)));
    #if DEBUG
    printf("Created a hash table!\n");
    #endif
    return hashtable;
}

user_ht_item *create_ht_item(const char *key, const char *value, char *ip, char *nickname) {
    user_ht_item *item = malloc(sizeof(user_ht_item));
    if (! item){
                perror("Out of memory! Aborting . . .");
                exit(10);
        }
    item->key = malloc(strlen(key) + 1);
    item->value = malloc((64 + 1)*sizeof(char));

    strcpy(item->key, key);
    strcpy(item->value, value);

    item->link = NULL;

    //This function will need to take in ip, nickname
    item->user_data = create_user_data(ip, nickname);

    strcpy(item->user_data->ip, ip);
    strcpy(item->user_data->nickname, nickname);
    item->user_data->socket = 0;
    item->user_data->server_setting = 0;
    item->user_data->chat_request = false;
    item->user_data->chat_accept = false;
    //item->user_data->chat_client = NULL;
    item->user_data->chess_request = false;
    item->user_data->chess_accept = false;
    item->user_data->private_chat = false;
    
    return item;
}

user_info *create_user_data(char *ip, char *nickname) {
    //Allocating memory for the struct inside the item just created above
    user_info *user_data = malloc(sizeof(user_info));
    if (! user_data){
                perror("Out of memory! Aborting . . .");
                exit(10);
        }
    user_data->ip = (char*) malloc(16*sizeof(char));
    user_data->nickname = (char*) malloc(21*sizeof(char));
    user_data->chat_client = (char*) malloc(30*sizeof(char));


    //The max amount of users in the hash table is 110, so the max amount
    //of friends a user can have is 109.
    user_data->friends = malloc(109 * sizeof(int));

    //item->user_data->status = malloc(sizeof(int));
    //item->user_data->play_game = malloc(sizeof(bool));
    //item->user_data->in_game = malloc(sizeof(bool));

    //memset(user_data->ip, 0, (strlen(ip) + 1));
    //memset(user_data->nickname, 0, (strlen(nickname) + 1));
    //memset(user_data->ip, 0, 16);
    //memset(user_data->ip, 0, 21);
    //memset(user_data->friends, -1, (109 * sizeof(int)));


     for (int i = 0; i < 110; i++)
     {
         user_data->friends[i] = - 1;
     }

    return user_data;
}

void del_ht(user_ht *hashtable) {
	assert(hashtable);
	user_ht_item *temp, *temp2;
	for(int i = 0; i < hashtable->size; i++) {
		if(hashtable->user_items[i] != NULL){
			if(hashtable->user_items[i]->link != NULL){
				temp = hashtable->user_items[i];
				while(temp != NULL){
					temp2 = temp->link;
					del_ht_item(temp);
					temp = temp2;
				}	
			}
			else{
				del_ht_item(hashtable->user_items[i]);
			}
		}
	}
	free(hashtable->user_items);
	free(hashtable);
	hashtable = NULL;
	#if DEBUG
	printf("Destroyed a hash table!\n");
	#endif
}

void del_ht_item(user_ht_item *item) {
	if (item != NULL) {
		free(item->key);
		free(item->value);
		free(item->user_data->nickname);
		free(item->user_data->ip);
		free(item->user_data->friends);
		free(item->user_data);
		//free(item->user_data->status);
		//free(item->user_data->play_game);
		//free(item->user_data->in_game);
		item->link = NULL;
	    
		free(item);
		item = NULL;
    }
}

void user_ht_insert(user_ht *user_ht, const char *key, const char *value, char *ip, char *nickname) {
    user_ht_item *item = create_ht_item(key, value, ip, nickname);
    int hash = hashCodeGenerator(key);
    /*If no items in the hashtable index, simply add the item there*/
    if (user_ht->user_items[hash] == NULL) {
	user_ht->user_items[hash] = item;
	user_ht->curr_amt++;
    }
    /*If there is an existing item*/
    else {
    	user_ht_item *curr, *next;
  	curr = user_ht->user_items[hash];
	if(curr->link == NULL){
		curr->link = item;
	}
	else{
		while(curr != NULL){
			next = curr->link;
			if(next == NULL){
				curr->link = item;
				break;
			}
			curr = next;
		}
	}	     
	user_ht->curr_amt++; 
    }
	//user_ht->curr_amt++;
    
    #if DEBUG
	/*printf("User inserted into hash table!");*/
    #endif
}

/*hashCodeGenerator sums up the Ascii values of the string and multiplies it by 13*/
/*The multiplier of 13 is to prevent collision in most cases*/
/*It then mods that number by the size of the hashtable (hardcoded to 110) to get the hashcode*/
int hashCodeGenerator(const char *string){
	int sum = 0;
	for(int i = 0; i<strlen(string); i++){
		sum = sum + string[i]*13;
		//sum = sum + string[i];

	}
	return sum % 110;
}


bool user_ht_search(user_ht *hashtable, const char *key) {
	int hashCode = hashCodeGenerator(key);
	#ifdef DEBUG
	printf("HashCode: %d\n", hashCode);
	#endif

	user_ht_item *temp, *temp2;

	/*Checks if hashtable index is NULL, if it is, return that the user does not exist*/
	if(!hashtable->user_items[hashCode]){
		#ifdef DEBUG
		printf("%s is not a registered user!\n",key);
		#endif
		return false;
	}	
	else{
                        if(hashtable->user_items[hashCode]->link){
                                temp = hashtable->user_items[hashCode];
                                while(temp != NULL){
                                        temp2 = temp->link;
					if(strcmp(temp->key,key)==0){
						#ifdef DEBUG
						printf("%s is a registered user!\n",key);
						#endif
						return true;
					}
                                        temp = temp2;
                                }
				#ifdef DEBUG
				printf("%s is not a registered user!\n",key);
				#endif
                        }
                        else{
				if(strcmp(hashtable->user_items[hashCode]->key,key) == 0){
					#ifdef DEBUG
					printf("%s is a registered user!\n",key);
					#endif
					return true;
				}
				else{
					#ifdef DEBUG
					printf("%s is not a registered user",key);
					#endif
					return false;
				}
                        }
        }
	
	return false;
}

user_ht_item *user_ht_search_return(user_ht *hashtable, const char *key) {
        int hashCode = hashCodeGenerator(key);
        #ifdef DEBUG
        printf("HashCode: %d\n", hashCode);
        #endif

        user_ht_item *temp, *temp2;

        /*Checks if hashtable index is NULL, if it is, return that the user does not exist*/
        if(!hashtable->user_items[hashCode]){
                #ifdef DEBUG
                printf("%s is not a registered user!\n",key);
                #endif
                return NULL;
        }
        else{
                        if(hashtable->user_items[hashCode]->link){
                                temp = hashtable->user_items[hashCode];
                                while(temp != NULL){
                                        temp2 = temp->link;
                                        if(strcmp(temp->key,key)==0){
                                                #ifdef DEBUG
                                                printf("%s is a registered user!\n",key);
                                                #endif
                                                return temp;
                                        }
                                        temp = temp2;
                                }
                                #ifdef DEBUG
                                printf("%s is not a registered user!\n",key);
                                #endif
                        }
                        else{
                                if(strcmp(hashtable->user_items[hashCode]->key,key) == 0){
                                        #ifdef DEBUG
                                        printf("%s is a registered user!\n",key);
                                        #endif
                                        return hashtable->user_items[hashCode];
                                }
                                else{
                                        #ifdef DEBUG
                                        printf("%s is not a registered user",key);
                                        #endif
                                        return NULL;
                                }
                        }
        }

        return NULL;
}


user_ht_item *socket_find_user(user_ht *database, int socket)
{
    user_ht_item *temp, *temp2;
    for(int i = 0; i < 110; i++)
    {
        if(!database->user_items[i]){
                continue;
        }
        else if(database->user_items[i])
        {
            if(database->user_items[i]->user_data->socket == socket)
            {
                #ifdef DEBUG
                printf("Socket found\n");
                #endif
                return database->user_items[i];
            }
            else if(database->user_items[i]->link){
                                temp = database->user_items[i];
                                while(temp != NULL){
                                        temp2 = temp->link;
                                        if(temp->user_data->socket==socket){
                                                #ifdef DEBUG
                                                printf("%s is a registered user!\n",temp->key);
                                                printf("Socket found\n");
                                                #endif
                                                return temp;
                                        }
                                        temp = temp2;
                                }
                                #ifdef DEBUG
                                printf("This is not a registered user!\n");
                                #endif
            }

        }
    }
    return NULL;
}

/*
void user_set_online(user_ht *hashtable, const char *key) {
        int hashCode = hashCodeGenerator(key);
        #ifdef DEBUG
        printf("HashCode: %d\n", hashCode);
        #endif

        user_ht_item *temp, *temp2;

        //Checks if hashtable index is NULL, if it is, return that the user does not exist
        if(!hashtable->user_items[hashCode]){
                #ifdef DEBUG
                printf("%s is not a registered user!\n",key);
                #endif
        }
        else{
			temp = malloc(sizeof(hashtable->user_items[hashCode]));
                        if(hashtable->user_items[hashCode]->link){
                                temp = hashtable->user_items[hashCode];
                                while(temp != NULL){
                                        temp2 = temp->link;
                                        if(strcmp(temp->key,key)==0){
                                                #ifdef DEBUG
                                                printf("%s is a registered user!\n",key);
                                                #endif
                                                temp->user_data->status = 1;
                                        }
                                        temp = temp2;
                                }
                                #ifdef DEBUG
                                printf("%s is not a registered user!\n",key);
                                #endif
                        }
                        else{
                                if(strcmp(hashtable->user_items[hashCode]->key,key) == 0){
                                        #ifdef DEBUG
                                        printf("%s is a registered user!\n",key);
                                        #endif
                                        temp->user_data->status = 1;
                                }
                                else{
                                        #ifdef DEBUG
                                        printf("%s is not a registered user",key);
                                        #endif
                                     
                                }
                        }
	free(temp);
        }

}

void user_set_offline(user_ht *hashtable, const char *key) {
        int hashCode = hashCodeGenerator(key);
        #ifdef DEBUG
        printf("HashCode: %d\n", hashCode);
        #endif

        user_ht_item *temp, *temp2;
        //Checks if hashtable index is NULL, if it is, return that the user does not exist
        if(!hashtable->user_items[hashCode]){
                #ifdef DEBUG
                printf("%s is not a registered user!\n",key);
                #endif
        }
        else{
			temp = malloc(sizeof(hashtable->user_items[hashCode]));
                        if(hashtable->user_items[hashCode]->link){
                                temp = hashtable->user_items[hashCode];
                                while(temp != NULL){
                                        temp2 = temp->link;
                                        if(strcmp(temp->key,key)==0){
                                                #ifdef DEBUG
                                                printf("%s is a registered user!\n",key);
                                                #endif
                                                temp->user_data->status = 0;
                                        }
                                        temp = temp2;
                                }
                                #ifdef DEBUG
                                printf("%s is not a registered user!\n",key);
                                #endif
                        }
                        else{
                                if(strcmp(hashtable->user_items[hashCode]->key,key) == 0){
                                        #ifdef DEBUG
                                        printf("%s is a registered user!\n",key);
                                        #endif
                                        temp->user_data->status = 0;
                                }
                                else{
                                        #ifdef DEBUG
                                        printf("%s is not a registered user",key);
                                        #endif

                                }
                        }
		free(temp);
        }

}
*/

bool user_ht_search_password(user_ht *hashtable, const char *key, const char *value) {
        int hashCode = hashCodeGenerator(key);
        #ifdef DEBUG
        printf("HashCode: %d\n", hashCode);
        #endif

        user_ht_item *temp, *temp2;

        /*Checks if hashtable index is NULL, if it is, return that the user does not exist*/
        if(!hashtable->user_items[hashCode]){
                printf("%s is not a registered user!\n",key);
                
        }
        else{
                        if(hashtable->user_items[hashCode]->link){
                                temp = hashtable->user_items[hashCode];
                                while(temp != NULL){
                                        temp2 = temp->link;
                                        if(strcmp(temp->key,key)==0){
						if(strcmp(temp->value,store_pw(value))==0){
							#ifdef DEBUG					
							printf("Password Accepted.\n");
							#endif
							return true;
						}
                                                else{
							#ifdef DEBUG
							printf("Incorrect Password.\n");
							#endif
							return false;
						}
                                        }
                                        temp = temp2;
                                }
                                printf("%s is not a registered user!\n",key);
                        }
                        else{
                                if(strcmp(hashtable->user_items[hashCode]->key,key) == 0){
					if(strcmp(hashtable->user_items[hashCode]->value,store_pw(value))==0){
						#ifdef DEBUG
						printf("Login Successful.\n");
						#endif
						return true;
					}
					else{
						#ifdef DEBUG
						printf("Incorrect Password.\n");
						#endif
						return false;
					} 
                                }
                                else{
                                        printf("%s is not a registered user",key);
                                }
                        }
        }

        return false;
}

char *store_pw(const char *pw)
{
    char *factor = malloc((64+1)*sizeof(char));
    char *salt = "{1hVj2EDtZwLN{#j\"p^>IqyxfvrXL'Y3JhA]PdqIw;rCT@t%:0t+yw)\"i(1T,IQ\0";
    strcpy(factor,"{1hVj2EDtZwLN{#j\"p^>IqyxfvrXL'Y3JhA]PdqIw;rCT@t%:0t+yw)\"i(1T,IQ\0");
    int counter = 0;
    int secret = 0;
    
    //int len = strlen(salt);
    
    #ifdef DEBUG
    /*printf("len: %d\n",len);*/
    printf("Factor: %s\n", factor);
    #endif

    for (int i = 0; i < strlen(pw); i++)
    {
        for (int j = 0; j < strlen(salt); j++)
        {
            if (pw[i] == salt[j]) 
            {
                factor[(i + j) % 63] = pw[i];
                
                #ifdef DEBUG
                printf("pw[i]: %c\n", pw[i]); 
                printf("factor index: %d\n", (i + j) % 63);
                #endif
            }
            else 
            {
                if (counter < 24)
                {
                    secret = ((strlen(pw))*(232 + counter) % 127);
                    if (secret < 33) secret += 33;
                    if (secret == 44) secret += 1;
                    
                    factor[((22*i) + (7*j)) % 63] = secret;

                    #ifdef DEBUG
                    printf("Num: %d\n", secret);
                    #endif

                    counter++;
                }
            }
        }
    }
    
    int check = 0;
    
    for (int i = 0; i < strlen(factor); i++)
    {
        while(factor[i] == '$' || factor[i] == ',')
        {
            factor[i] = ((44*factor[i] + 532 + check) % 127);
            check++;
        }
    }
    
    #ifdef DEBUG
    printf("%s\n", factor);
    #endif
    
    return factor;
}

void displayHashtable(user_ht *hashtable){
	assert(hashtable);
	user_ht_item *temp, *temp2;
 	int i, hashtableLen = hashtable->size;
	printf("********************************");
	printf("********************************");
	printf("********************************");
	printf("HASHTABLE");
	printf("********************************");
	printf("********************************");
	printf("********************************\n");
	for(i = 0; i < hashtableLen; i++){
		if(hashtable->user_items[i] != NULL){
			if(hashtable->user_items[i]->link){
				temp = hashtable->user_items[i];
                        	while(temp != NULL){
                                	temp2 = temp->link;
                                	printf("Key:%-30s Value: %-64s \t IP: %-15s \t Nickname: %-20s \t\t Hashcode:%-3d\n",
                        		temp->key,
                        		temp->value,
                        		temp->user_data->ip,
                        		temp->user_data->nickname,
					hashCodeGenerator(temp->key));
                                	temp = temp2;
                        	}				
                        }

			else{
				printf("Key:%-30s Value: %-64s \t IP: %-15s \t Nickname: %-20s \t\t Hashcode:%-3d\n",
				hashtable->user_items[i]->key,
				hashtable->user_items[i]->value,
				hashtable->user_items[i]->user_data->ip,
				hashtable->user_items[i]->user_data->nickname,
				hashCodeGenerator(hashtable->user_items[i]->key));
				
				/*printf("Key: %s Value: %s IP: %s Nickname: %s\n", 
				user_ht->user_items[i]->key, 
				user_ht->user_items[i]->value, 
				user_ht->user_items[i]->user_data->ip, 
				user_ht->user_items[i]->user_data->nickname);*/
			}
		}		
	}	
}

//Test this later to make sure creating/destroying hash table works

/*
    int main(void) {
    char *key = "Zachary";
    char *value = "gitbread";
    char *ip = "10.0.0.1";
    char *nickname = "Zach";
*/

    //char *tempKey, *tempValue, *tempNickname, *ptr;
    //char delim[] = " ";
    //char temp[256];
    //int n = 0, loopBreaker = 0;

/*
     user_ht *user_ht = create_ht();	
*/

     //printf("Created a hash table!\n");
     
/*
    printf("Welcome to Hashcode Inserter!\n");
*/

/*
    while(loopBreaker != 1){
	printf("Enter 0 to quit, Enter 1 to insert into a hashtable\n");
	scanf("%d",&n);
	printf("\n");
	switch (n){
		case 1:
			printf("Enter key,value, and nickname separated by spaces.");	
			scanf("%s",temp);
			printf("\n");
			tempKey = strtok(temp,delim);
			ptr = strtok(NULL,delim);
			tempValue = ptr;
			tempNickname = strtok(temp,delim);
			user_ht_insert(user_ht, tempKey, tempValue, ip, tempNickname);
		case 0:
			printf("Program Terminated.\n");
			loopBreaker =1;
			break;
		default:
			printf("Invalid Choice.\n");
			continue;
	
	}
    }
*/


/*
    user_ht_insert(user_ht, key, value, ip, nickname);
    user_ht_insert(user_ht, "Jaeven", "password", "10.0.0.1", "Jay");
    user_ht_insert(user_ht, "Robert", "password", "10.0.0.1", "Bob");
    user_ht_insert(user_ht, "Alex", "password", "10.0.0.1", "Alex");
    user_ht_insert(user_ht, "Dirk", "password", "10.0.0.1", "Drew");

*/  

 
    /*printf("User inserted into hash table!\n");
    printf("Key: %s Value: %s IP: %s Nickname: %s\n", 
	user_ht->user_items[36]->key, user_ht->user_items[36]->value,
	user_ht->user_items[36]->user_data->ip, 
	user_ht->user_items[36]->user_data->nickname);*/

/*

    displayHashtable(user_ht);


*/

    /*char *test = user_ht_search(user_ht, key);
    printf("Test: %s\n", test);*/

/*

   user_ht_search(user_ht, "Alex");
    user_ht_search(user_ht, "Dirk");
    user_ht_search_password(user_ht, "Dirk","password");
    user_ht_search_password(user_ht, "Jaeven","Radiohead");

    del_ht(user_ht);
    printf("Destroyed a hash table!\n");

}
*/
