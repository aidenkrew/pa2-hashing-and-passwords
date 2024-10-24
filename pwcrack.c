#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <openssl/sha.h>

uint8_t hex_to_byte(unsigned char h1, unsigned char h2){
   int i = 0;
    if (h1 >= '0' && h1 <= '9'){
        i = h1 - '0';
        i = i * 16;
    }
    else{
        i = h1 - 'a' + 10;
        i = i * 16;
    }
    if (h2 >= '0' && h2 <= '9'){
        i += h2 - '0';
    }
    else{
        i += h2 - 'a' + 10;
    }
    return i;
}

void hexstr_to_hash(char hexstr[], unsigned char hash[32]){
   int j = 0;
    for (int i = 0; i < 32; i++)
    {
	    
    hash[i] = hex_to_byte( hexstr[j], hexstr[j+1]);
    j+=2;
    }
}


int8_t check_password(char password[], unsigned char given_hash[32]){
unsigned char hash[32];
SHA256(password, strlen(password), hash);

for (int i =0; i < 32; i++){
    if (given_hash[i] != hash[i]){
        return 0;
    }
}

return 1;

}


int8_t crack_password(char password[], unsigned char given_hash[]){

  int j = 0;

    if(check_password(password, given_hash) == 0){
        char test[strlen(password)];
        strcpy(test, password);
        for (int i = 0; i < strlen(password); i++){
            j=0;
            if(test[i] >= 'a' && test[i] <= 'z'){ //uppercase test password
                test[i] = test[i] - 32;
                j = 1;
	  }
	    else if(test[i] >= 'A' && test[i] <= 'Z'){//lowercase test password
		j = 2;
		test[i] = test[i] + 32;
	  }	


            if(check_password(test, given_hash) == 1){
		if(j == 1)    
                password[i] = password[i] - 32; //uppercase actual password 
		else
		password[i] = password[i] + 32;//lowercase actual password letter
                return 1;
            }
	    else if(j == 1)         //No match, return test to original password
                    test[i] = test[i] + 32;
       
            
	    else if(j == 2)
		    test[i] = test[i] - 32;
            
        	}


 
       return 0;
    }
return 1;
}
    



int main(int argc, char *argv[]){



char hash_as_hexstr[64]; // SHA256 hash for "password"

strcpy (hash_as_hexstr, argv[1]);
unsigned char given_hash[32];
hexstr_to_hash(hash_as_hexstr, given_hash);


char password[129];
char correct_password[129];
int8_t match = 0;


while(fgets(password,sizeof(password), stdin) != NULL){
password[strlen(password) - 1] = 0;
if(crack_password(password, given_hash) == 1){
match = 1;
strcpy(correct_password, password);
}

}

if(match == 1)

	printf("Found password: SHA256(%s) = %s\n", correct_password, hash_as_hexstr);
else
	printf("Did not find a matching password\n");
    return 0;

}
