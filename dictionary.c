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
int static stupid_search(char **dict, char *word){
    //printf("here\n");
    //printf("%ld",sizeof(dict)/sizeof(dict[0]));
    for (int i = 0; i < 58810; ++i){
       //printf("%s\n",dict[465]);
        //printf("%d", i);
        if (strcmp(word,dict[i]) == 0){
            return 1;
        }
        //
    }
    return 0;
}

int static valid_word(char *word){
    FILE *file;
    int i = 0;
    file = fopen("dict.txt","r");
    char line[100];
    char **dict = calloc(58110, sizeof(char *));
    printf("HI I AM HERE1\n");
    /*
    for (int i = 0; i < 58810; i++){
        fgets(dict[i],10,file);
    }
    */
    
    while (fgets(line,sizeof(line),file)){
        strtok(line,"\n");
        dict[i] = line;
        printf("%s \n",dict[i]);
        i++;
    }
    
    for (int i = 0; i < 58810; ++i){
       printf("%s\n",dict[i]);
        //
    }
    printf("HI I AM HERE2\n");
    return stupid_search(dict,word);
}

int main(){
    printf("%d\n",valid_word("mole"));
    return 0;
}
