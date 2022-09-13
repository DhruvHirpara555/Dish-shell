#include "exe.h"
#include "headers.h"
#include "execute.h"
#include <fcntl.h>



void exe_redi_pip(int argc,char* argv[],int bp){
    char** darrgv = (char**)malloc(sizeof(char*)*argc);
    memset(darrgv,0,sizeof(char*)*argc);

    ORIG_STDIN = dup(STDIN_FILENO);
    if(ORIG_STDIN < 0)
    {
        perror(RED "dup" RESET);
        exit(1);
    }
    ORIG_STDOUT = dup(STDOUT_FILENO);
    if(ORIG_STDOUT < 0)
    {
        perror(RED "dup" RESET);
        exit(1);
    }
    int darrgc = 0;
    for(int i=0;i<argc;i++){
        if(strcmp(argv[i],"<") == 0){
            if(i+1>=argc){
                printf(RED "dish : " RESET "expected file name after <");
                return;
            }
            int fd = open(argv[i+1],O_RDONLY);
            i++;
            if(fd < 0)
            {
                perror(RED"open" RESET);
                return;
            }
            if(dup2(fd,STDIN_FILENO) < 0)
            {
                perror(RED "dup2" RESET);
                return;
            }
        }
        else if(strcmp(argv[i],">") == 0){
            if(i+1>=argc){
                printf(RED "dish : " RESET "expected file name after >");
                return;
            }
            int fd = open(argv[i+1],O_WRONLY | O_CREAT | O_TRUNC, 0644);
            i++;
            if(fd < 0)
            {
                perror(RED"open" RESET);
                return;
            }
            if(dup2(fd,STDOUT_FILENO) < 0)
            {
                perror(RED "dup2" RESET);
                return;
            }
        }
        else if(strcmp(argv[i],">>") == 0){
            if(i+1>=argc){
                printf(RED "dish : " RESET "expected file name after >>");
                return;
            }
            int fd = open(argv[i+1],O_WRONLY | O_CREAT , 0644);
            i++;
            if(fd < 0)
            {
                perror(RED"open" RESET);
                return;

            }
            lseek(fd,0,SEEK_END);
            if(dup2(fd,STDOUT_FILENO) < 0)
            {
                perror(RED "dup2" RESET);
                return;
            }
        }
        else if(strcmp(argv[i],"|") == 0){
            if(i+1>=argc){
                printf(RED "dish : " RESET "expected command after |");
                return;
            }
            int pipe_fd[2];
            if(pipe(pipe_fd) < 0)
            {
                perror(RED "pipe" RESET);
                return;
            }
            if(dup2(pipe_fd[1],STDOUT_FILENO) < 0)
            {
                perror(RED "dup2" RESET);
                return;
            }
            darrgv[darrgc] = NULL;

            execute(darrgc,darrgv,bp);
            close(pipe_fd[1]);

            if(dup2(pipe_fd[0],STDIN_FILENO) < 0)
            {
                perror(RED "dup2" RESET);
                return;
            }

            darrgc = 0;
            memset(darrgv,0,sizeof(char*)*argc);

            if(dup2(ORIG_STDOUT,STDOUT_FILENO) < 0)
            {
                perror(RED "dup2" RESET);
                return;
            }


        }
        else{
            darrgv[darrgc] = argv[i];
            darrgc++;
        }

    }
    darrgv[darrgc] = NULL;
    if(darrgc == 0){
        return;
    }
    execute(darrgc,darrgv,bp);
    if(dup2(ORIG_STDIN,STDIN_FILENO) < 0){
        perror(RED "dup2" RESET);
        return;
    }

    if(dup2(ORIG_STDOUT,STDOUT_FILENO) < 0){
        perror(RED "dup2" RESET);
        return;
    }

    close(ORIG_STDIN);
    close(ORIG_STDOUT);
}