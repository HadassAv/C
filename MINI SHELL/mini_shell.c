#include <stdio.h>
#include <argz.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

char** makeArr(char* str)
{
    char** words;
    char* token;
    char temp[510];
    strcpy(temp, str);

    token = strtok(str, " ");
    int count = 1;

    while (token !=NULL)
    {
        token = strtok(NULL, " ");
        count++;
    }

    words = (char**)malloc(count*sizeof(char*));
    if (words == NULL)
    {
        printf("malloc failed\n");
        exit(1);
    }
    token = strtok(temp, " \n");
    int i = 0;

    while (token != NULL)
    {
        words[i] = (char*)malloc(strlen(token)+1*sizeof(char));
        if(words[i] == NULL)
        {
            printf("malloc failed\n");
            exit(1);
        }
        strcpy(words[i], token);
        token = strtok(NULL, " \n");
        i++;
    }
    words[count-1] = NULL;
    return words;
}

int main()
{
    char str[510];
    char hostname[1024];
    gethostname(hostname, 1024);
    char* log;
    if((log = getlogin() != NULL))
    {
        log = getlogin();
    }
    else
    {
        log = malloc(sizeof(char)*5);
        strcpy(log, "null");
    }
    int i = 0;
    char **a;

    while (1)
    {
        printf("%s@%s$ ", log, hostname);
        fgets(str, 510, stdin);
        if(strcmp(str, "done\n") == 0)
        {
            exit(0);
        }
        a = makeArr(str);
        pid_t x = fork();
        if (x == -1)
        {
            printf("%s: command not found", a[i]);
        }
        if (x == 0) //son
        {
            if(execvp(a[i], a) == -1 && execvp(a[i], a) !=NULL)
            {
                printf("%s: command not found\n", a[i]);
                exit(0);
            }
        }
        else //father
        {
            wait(NULL);
            int j = 0;
            while (a[j] != NULL)
            {
               free(a[j]);
               j++;
            }
         }
    }
    free(a); //free the array
    return 0;
}
