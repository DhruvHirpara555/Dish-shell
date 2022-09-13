#include "headers.h"
#include "execute.h"
#include "builtins.h"
#include "prompt.h"
#include "process.h"
#include <signal.h>
#include <sys/wait.h>
#include "pinfo.h"
#include "history.h"
#include "discover.h"

void execute(int argc,char* argv[],int bp)
{

    if(strcmp(argv[0],"exit") == 0)
    {
        write_history_to_file();
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

    else if (strcmp(argv[0],"ls") == 0)
    {
        exec_ls(argc,argv);
    }

    else if (strcmp(argv[0],"pinfo") == 0)
    {
        exec_pinfo(argc,argv);
    }
    else if (strcmp(argv[0],"history") == 0)
    {
        exec_history(argc,argv);
    }
    else if(strcmp(argv[0],"discover") ==0){
        exec_discover(argc,argv);
    }
    else
    {
        execute_system(argc,argv,bp);
        // printf("%s: command not found\n",argv[0]);
    }

}


void execute_system(int argc, char *argv[],int bp)
{
    pid_t pid = fork();
    time_t start = time(NULL);
    if (pid == 0)
    {
        // printf("Child process\n");
        // child process should have default signal handlers
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);

        for (int i = 0; i < argc; i++)
        {
            if(argv[i][0] == '~')
            {
                char* newpath = (char*)malloc(sizeof(char)*(strlen(argv[i])+home_len));
                sprintf(newpath,"%s%s",home_dir,argv[i]+1);
                argv[i] = newpath;
            }
        }
        setpgid(0,0); // set process group id to its own pid this makes new process group
        // making new process group for child process. This way, parent just continues to run  ---  waitpid is  not called in parent

        int exe_status;
        if ((exe_status = execvp(argv[0], argv)) < 0)
        {
            printf(RED "dish" RESET" : command not found: %s\n",argv[0]);
            exit(0);
        }
        exit(exe_status);
    }
    else
    {

        if(bp ==0){
            // since child should be foreground process, parent should wait for child to finish
            // ignore input output of parent
            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);
             // put child in foreground
            tcsetpgrp(STDIN_FILENO, pid);
            //wait
            int p_status;
            waitpid(pid,&p_status,WUNTRACED);
            // printf("Parent process\n");
            // printf("Child process exited with status %d\n",p_status);
            time_t end = time(NULL);
            last_time += end - start;

            // restore parent to foreground
            tcsetpgrp(STDIN_FILENO, getpid());
            // restore default signal handlers
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);


        }
        else
        {
            // printf("Background process\n");
            // add process to process list
            process_add(argv[0],pid);
            printf("[%d] %d\n",process_count,pid);
        }

    }

}