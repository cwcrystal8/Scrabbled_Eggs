#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int static binary_search(char **dict, char *word){
	int lb, mid, ub;
    lb = 0;                             
    ub = 58810;                             
    do
    {
        mid = (lb + ub) / 2;             
        if ((strcmp(word,dict[mid]))<0){
        	ub = mid - 1;                         

        }       
        else if ((strcmp(word,dict[mid]))>0){
            lb = mid + 1;                          
        }
    } while ((strcmp(word,dict[mid])!=0) && lb <= ub);
    if ((strcmp(word,dict[mid])) == 0){
    	return 1;
    }          
    else{                                      
        return 0;
 	}
}

int static valid_word(char *word){
    FILE *file;

    file = fopen("dict.txt","r");
    char **dict = calloc(58110, sizeof(char *));
    printf("HI I AM HERE\n");
    for (int i = 0; i < 58810; i++){

        fgets(dict[i],10,file);
    }
    printf("HI I AM HERE\n");
    return binary_search(dict,word);
}

int main(){
    printf("%d\n",valid_word("crystal"));
    return 0;
}
