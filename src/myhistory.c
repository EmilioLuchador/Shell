/*
Name: Aleksander Rodriguez
Section: 3600 003
Date: 11/10/2023
Description: this program will store every command typed in the shell into an array.
If "myhistory" is typed on the command line then the histroy of the last 20 commands will be printed
in the terminal of the shell. If "myhistory -c" is typed then the myhistory will be cleared. If 
"myhistory -e <number typed>" the number typed will run the command the number is associated with in
the myhistory. An int is returned, this is mainly used for "myhistory -e <number typed>".
*/

#include "major2.h"
int myhistory(char *input, int *count, char **history, int* hit20)
{
   //printf("%d: we made it here\n", *count);
   //(*count)++;
   
   // this will store the command in a array
   if( *count < 20 )
    {
       //printf("%d: we made it here\n", *argv);
		history[*count] = malloc(512 * sizeof(char));
		strcpy(history[*count], input);
        (*count)++;
    }
    else
    {
        *hit20 = *count;
        *count = 0;
        history[*count] = malloc(512 * sizeof(char));
        strcpy(history[*count], input);
        (*count)++;
    }

    // check for "myhistory" command
    int value = strcmp(input, "myhistory");
    //printf("1st value: %d\n", value);
    if(value == 0 || value == 116)
    {
        for (int x = 0; x < *count || x < *hit20; x++)
        {
            if(history[x] != NULL){
            printf("%d: %s", x+1 , history[x]);
            printf("\n");
            }
            else 
            return 34;
            
        }
    }

    // checks for "myhistory -c" command
    value = strcmp(input, "myhistory -c");
    if(value == 0)
    {
        // this it technically not deleting all entries but works as if it did
        *count = 0;
        *hit20 = 0;
        return 35;
    }

    // will need this once we hit 20+ commands because count is set to 0 and following if statements
    // need count to be 20 if there are already 20+ commands
    int tempC = *count;
     if (*hit20 == 20){
        *count = 20;
    }
   // printf("%d\n", *count);

    //checking for "myhistory -e #"
    char *string1 = "myhistory -e";
    char last = input[strlen(input)-1];
    char last1 = input[strlen(input)-2];
    //printf("last: %c%c \n", last1,last);

    // this will help determine if the last number on the
    // command line is a single digit or a double digit
    if(last1 == ' ')
    {
        size_t len = strlen(string1);

        //the following will format the str2 to then compare to orginal output
        char *str2 = malloc(len + 1 + 1 + 1);
        strcpy(str2, string1);
        str2[len] = ' ';
        str2[len + 1] = last;
        str2[len + 2] = '\0';
        
      
        // the follwing if will determine if a myhistory -e # was written to the command line
        value = strcmp(str2, input);
        if(value == 0 )
        {
            int num = last - '0';
            //printf("count: %d \n", *count);
            if( num > 20 || num > *count )
            {
                printf("Number given in argument does not exist\n");
                *count = tempC;
                return 33;
            }
            else
            {
                //printf("command: %s\n", history[num-1]);
                input = history[num-1];
                //printf("input: %s\n", input);
                *count = tempC;

                return num;
            }
        }
        else{
            *count = tempC;
            return 33;
        }


    }
    else if (last1 >='0' || last1 <= '9')
    {

        size_t len = strlen(string1);

        //the following will format the str2 to then compare to orginal output
        char *str2 = malloc(len + 1 + 1 + 1+ 1);
        strcpy(str2, string1);
        str2[len] = ' ';
        str2[len + 1] = last1;
        str2[len + 2] = last;
        str2[len + 3] = '\0';

        // the follwing if will determine if a myhistory -e # was written to the command line
        value = strcmp(str2, input);
        if(value == 0)
        {
            char *n= malloc(3);
            n[0]= last1;
            n[1]= last;
            n[2] = '\0';
            int num = atoi(n);
            //printf("count1: %d \n", *count);
            if( num > 20 || num > *count)
            {
                printf("Number given in argument does not exist\n");
                *count = tempC;
                return 33;
            }
            else
            {
                //printf("command: %s\n", history[num-1]);
                input = history[num-1];
                //printf("input: %s\n", input);
                *count = tempC;
                return num;
            }
        }
        else 
        {
            *count = tempC;
            return 33;
        }

    }
  
    
   *count = tempC;
    //printf("\n");
    return 33;
}
