#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(void){
	char *token, *username, *password;
char line[] = "--LOGIN Jaeven password";
char *search = " ";

/*This */
// Token will point to "SEVERAL".
token = strtok(line, search);
 printf("%s\n",token);
// Token will point to "WORDS".
username = strtok(NULL, search);
printf("%s\n",username);

password = strtok(NULL,search);
printf("%s\n",password);
}
