#include "headers.h"
#include "execute.h"
#include "builtins.h"

void execute(int argc,char* argv[])
{
    if(strcmp(argv[0],"exit") == 0)
    {
        exit(0);
    }
    else if(strcmp(argv[0],"cd") == 0)
    {
        exec_cd(argc,argv);
    }
    else if (strcmp(argv[0],"pwd") == 0)
    {
        exec_pwd(argc,argv);
    }
    else if (strcmp(argv[0],"echo") == 0)
    {
        exec_echo(argc,argv);
    }
    else if (strcmp(argv[0],"exit") == 0)
    {
        exec_exit(argc,argv);
    }
    else
    {
        printf("%s: command not found\n",argv[0]);
    }



}