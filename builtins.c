#include "headers.h"
#include "builtins.h"
#include "prompt.h"

char* lastused = NULL;
// char* home_dir;
void exec_cd(int argc, char *argv[])
{
    char* cwd = getcwd(NULL, 0);
    char* newpath = (char *) malloc(sizeof(char) * (strlen(argv[1]) + home_len));
    if (argc == 1){
        // chdir(home_dir);
        if (chdir(home_dir) == -1){
            perror("chdir");
            // exit(1);
        }
    }
    else if (argc == 2){
        if(argv[1][0] == '~' && (argv[1][1] == '\0' || argv[1][1] == '/')){
            strcpy(newpath, home_dir);
            sprintf(newpath, "%s%s", newpath, argv[1]+1);
            // chdir(newpath);
            if (chdir(newpath) == -1){
                perror("cd");
                // exit(1);
            }
        }
        else if (strcmp("-",argv[1]) == 0){
            if (lastused == NULL){
                lastused = home_dir;
            }

            if (chdir(lastused) == -1){
                perror("cd");
                // exit(1);
            }
        }
        else{
            // chdir(argv[1]);
            if (chdir(argv[1]) == -1){
                perror("cd");
                // exit(1);
            }
        }
    }
    else{
        printf("cd: too many arguments\n");
    }
    lastused = strdup(cwd);
    free(cwd);
    free(newpath);
}

void exec_pwd(int argc, char *argv[])
{
    if (argc != 1)
    {
        printf("pwd: too many arguments\n");
    }

    char* cwd = getcwd(NULL, 0);
    write(STDOUT_FILENO,cwd,strlen(cwd));
    write(STDOUT_FILENO,"\n",1);
    free(cwd);
}

void exec_exit(int argc, char *argv[])
{
    if (argc == 1){
        exit(0);
    }
    else{
        printf("exit: too many arguments\n");
    }
}

void exec_echo(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++){
        write(STDOUT_FILENO,argv[i],strlen(argv[i]));
        write(STDOUT_FILENO," ",1);
    }
    write(STDOUT_FILENO,"\n",1);
}