#include "process.h"
#include<signal.h>
#include<sys/wait.h>
#include "prompt.h"

struct process* process_list = NULL;

int process_count = 0;



void init_process_list()
{
    process_list = NULL;
    process_count = 0;

}

void process_add(char *name, pid_t pid)
{
    struct process *new_process = (struct process*) malloc(sizeof(struct process));
    new_process->pid = pid;
    new_process->name = (char*) malloc(sizeof(char) * (strlen(name)+1));
    strcpy(new_process->name, name);

    // adding process like in stack
    new_process->next = process_list;
    process_list = new_process;
    process_count++;
}

void process_remove(pid_t pid)
{
    struct process *temp = process_list;
    struct process *prev = NULL;
    while(temp != NULL)
    {
        if(temp->pid == pid)
        {
            if(prev == NULL)
            {
                process_list = temp->next;
            }
            else
            {
                prev->next = temp->next;
            }
            free(temp->name);
            free(temp);
            process_count--;
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}
char* get_process_name(pid_t pid)
{
    struct process *temp = process_list;
    while(temp != NULL)
    {
        if(temp->pid == pid)
        {
            return temp->name;
        }
        temp = temp->next;
    }
    return NULL;
}

void process_ch_handler(int sig)
{
    pid_t cpid;
    int wstatus;
    while((cpid = waitpid(-1, &wstatus, WNOHANG | WUNTRACED)) > 0)
    {
        if(WIFEXITED(wstatus))
        {
            // printf("\n%s with pid %d exited normally\n", get_process_name(cpid), cpid);
            fprintf(stderr, "\n%s with pid %d exited normally\n", get_process_name(cpid), cpid);
            process_remove(cpid);
        }
        else if(WIFSIGNALED(wstatus))
        {
            // printf("\n%s with pid %d exited abnormally\n", get_process_name(cpid), cpid);
            fprintf(stderr, "\n%s with pid %d exited abnormally\n", get_process_name(cpid), cpid);
            process_remove(cpid);
        }
        else if(WIFSTOPPED(wstatus))
        {
            // printf("\n%s with pid %d stopped\n", get_process_name(cpid), cpid);
            fprintf(stderr, "\n%s with pid %d stopped\n", get_process_name(cpid), cpid);

        }
        // prompt();
        // give /n to input buffer from here
        prompt();
        fflush(stderr);
        fflush(stdout);
        // fflush(stdout);


    }
}