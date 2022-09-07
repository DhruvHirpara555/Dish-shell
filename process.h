#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "headers.h"

struct process
{
    char *name;
    pid_t pid;
    struct process *next;
};

extern struct process *process_list;
extern int process_count;

void init_process_list();
void process_add(char *name, pid_t pid);
void process_remove(pid_t pid);
void process_ch_handler(int sig);
char* get_process_name(pid_t pid);

#endif // __PROCESS_H__
