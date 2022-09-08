#include "inputs.h"
#include "headers.h"

char** parse_input(char* command, int* argc)
{
    unsigned int len = strlen(command);
    char* comm = (char*) malloc(sizeof(char) * (len+1));
    strcpy(comm, command);
    unsigned int j = 0;


    for (; j < len ; j++)
    {
        if (comm[j] == ' ' || comm[j] == '\t')
        {
            comm[j] = '\0';
        }
    }
    long long int last = -1;
    for (long long int  i = 0; i <= len; i++)
    {
        if (comm[i] == '\0' )
        {
            if((i-last) > 1)
            {
                (*argc)++;
            }
            last = i;
        }

    }

    char **argv = (char**) malloc(sizeof(char*) * (*argc+1));
    last =-1;
    unsigned int carg = 0;
    for (long long int i = 0; i < len; i++)
    {
        if (comm[i] == '\0')
        {
            last = i;
            continue;
        }
        if((i-last) == 1){
            if (strlen(comm+i) > 0)
            {
                argv[carg++]  = (char*) malloc(sizeof(char) * (strlen(comm+i)+1));
                strcpy(argv[carg-1], comm+i);
            }
        }
    }
    argv[carg] = NULL;
    free(comm);
    return argv;
}



char* get_input(){
    char *line = NULL;

    unsigned long len = 0;
    len = getline(&line, &len, stdin);
    if(len > 0){
        line[len-1] = '\0';
    }
    return line;
}