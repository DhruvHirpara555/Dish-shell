#include "jobbuiltins.h"
#include "process.h"
#include "headers.h"
#include <sys/wait.h>

int cmpprocess(const void* a,const void* b)
{
    struct process* p1 = (struct process*)a;
    struct process* p2 = (struct process*)b;
    return strcmp(p1->name,p2->name);
}


void exec_jobs(int argc, char* argv[])
{
    int flag_r = 0;
    int flag_s = 0;

    for(int i=1; i< argc; i++){
        if(strcmp(argv[i],"-r") == 0){
            flag_r = 1;
        }
        else if(strcmp(argv[i],"-s") == 0){
            flag_s = 1;
        }
        else{
            printf(RED"jobs : "RESET"Invalid argument\n");
            return;
        }
    }
    if(flag_r && flag_s){
        printf(RED"jobs : "RESET"Invalid argument\n");
        return;
    }
    if(flag_r == 0 && flag_s == 0){
        flag_r = 1;
        flag_s = 1;
    }

    struct process list[process_count];
    int i = 0;
    struct process *temp = process_list;
    while(temp != NULL)
    {
        list[i++] = *temp;
        temp = temp->next;
    }

    qsort(list,process_count,sizeof(struct process),cmpprocess);

    for ( i = 0; i < process_count; i++)
    {
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        char *proc_path = (char *)malloc(sizeof(char) * 128);

        sprintf(proc_path, "/proc/%d/stat", list[i].pid);
        FILE *fp = fopen(proc_path, "r");
        if (fp == NULL)
        {
            perror(RED "fopen" RESET);
            return;
        }

        if ((read = getline(&line, &len, fp)) != -1)
        {
            char *token = strtok(line, " ");
            int j = 0;
            while (token != NULL)
            {
                if (j == 2)
                {
                    char *status = token;
                    if(flag_r && strcmp(status,"R") == 0){
                        printf("[%d] Running %s [%d]\n", get_process_no(list[i].pid), list[i].name, list[i].pid);
                    }
                    else if(flag_r && strcmp(status,"S") == 0){
                        printf("[%d] Running %s [%d]\n", get_process_no(list[i].pid),  list[i].name, list[i].pid);
                    }
                    else if(flag_s && strcmp(status,"T") == 0){
                        printf("[%d] Stopped %s [%d]\n", get_process_no(list[i].pid),  list[i].name, list[i].pid);
                    }
                    else if(flag_s && flag_r && strcmp(status,"Z") == 0){
                        printf("[%d] Zombie %s [%d]\n", get_process_no(list[i].pid),  list[i].name, list[i].pid);
                    }
                    break;
                }

                token = strtok(NULL, " ");
                j++;
            }
        }
        fclose(fp);
        free(proc_path);
    }
}

void exec_bg(int argc, char* argv[]){
    if(argc != 2){
        printf(RED"bg : "RESET"Invalid argument\n");
        return;
    }
    int job_no = atoi(argv[1]);
    if(job_no > process_count || job_no < 1){
        printf(RED"bg : "RESET"Invalid job number\n");
        return;
    }
    struct process *temp = process_list;
    int i = process_count;
    while(temp != NULL)
    {
        if(i == job_no){
            break;
        }
        temp = temp->next;
        i--;
    }
    if(temp == NULL){
        printf(RED"bg : "RESET"Invalid job number\n");
        return;
    }
    if(kill(temp->pid,SIGCONT) == -1){
        perror(RED"kill"RESET);
        return;
    }
    // temp->status = 1;
}

void exec_fg(int argc, char* argv[]){
    if(argc != 2){
        printf(RED"fg : "RESET"Invalid argument\n");
        return;
    }
    int job_no = atoi(argv[1]);
    if(job_no > process_count || job_no < 1){
        printf(RED"fg : "RESET"Invalid job number\n");
        return;
    }
    struct process *temp = process_list;
    int i = process_count;
    while(temp != NULL)
    {
        if(i == job_no){
            break;
        }
        temp = temp->next;
        i--;
    }
    if(temp == NULL){
        printf(RED"fg : "RESET"Invalid job number\n");
        return;
    }
    if(kill(temp->pid,SIGCONT) == -1){
        perror(RED"kill"RESET);
        return;
    }
    pid_t pid = temp->pid;
    char* name = strdup(temp->name);
    process_remove(temp->pid);

    // temp->status = 1;
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
        // put child in foreground
    tcsetpgrp(STDIN_FILENO, pid);
    //wait
    int p_status;
    waitpid(pid,&p_status,WUNTRACED);
    if(WIFSTOPPED(p_status)){
        // printf("Process stopped\n");
        process_add(name,pid);
        printf("[%d] %d suspended %s\n",process_count,pid,name);
    }
    // printf("Parent process\n");
    // printf("Child process exited with status %d\n",p_status);
    // restore parent to foreground
    tcsetpgrp(STDIN_FILENO, getpid());
    // restore default signal handlers
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    free(name);
}

void exec_sig(int argc,char *argv[])
{
    if(argc != 3){
        printf(RED"kill : "RESET"Invalid argument\n");
        return;
    }
    int job_no = atoi(argv[1]);
    if(job_no > process_count || job_no < 1){
        printf(RED"kill : "RESET"Invalid job number\n");
        return;
    }
    // struct process *temp = process_list;
    // int i = process_count;
    // while(temp != NULL)
    // {
    //     if(i == job_no){
    //         break;
    //     }
    //     temp = temp->next;
    //     i--;
    // }
    pid_t pid = get_process_pid(job_no);
    if(pid == -1){
        printf(RED"kill : "RESET"Invalid job number\n");
        return;
    }
    int sig = atoi(argv[2]);
    if(kill(pid,sig) == -1){
        perror(RED"kill"RESET);
        return;
    }
}