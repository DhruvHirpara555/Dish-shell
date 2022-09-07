#include "pinfo.h"
#include "headers.h"
#include "prompt.h"
int check_foreground(pid_t pid)
{
    // pid_t ppid = getpid();

    pid_t grp_id = getpgid(pid);
    if(grp_id == -1){
        perror(RED "getpgid" RESET);
        return -1;
    }
    if(grp_id == pid){
        return 1;
    }
    return 0;
}

void print_pinfo(pid_t pid){
    // pid = getpid();
    // pid_t grp_id = getpgid(pid);
    printf("pid -- %d\n", pid);
    int ffg = check_foreground(pid);
    if(ffg == -1){
        return;
    }
    char* fg = ffg  ? "+" : "";


    char* proc_path = (char*) malloc(sizeof(char) * 64);
    sprintf(proc_path, "/proc/%d/stat", pid);
    FILE* fp = fopen(proc_path, "r");
    if (fp == NULL)
    {
        perror(RED "fopen" RESET);
        return;
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    if ((read = getline(&line, &len, fp)) != -1)
    {
        char* token = strtok(line, " ");
        int i = 0;
        while (token != NULL)
        {
            if (i == 2)
            {
                printf("Process Status -- %s%s\n", token, fg);
            }
            else if (i == 22)
            {
                printf("memory -- %s\n", token);
                break;
            }
            token = strtok(NULL, " ");
            i++;
        }
    }
    fclose(fp);


    sprintf(proc_path, "/proc/%d/exe", pid);
    char* exe_path = (char*)malloc(257);
    read = readlink(proc_path, exe_path, 256);
    if (read == -1)
    {
        perror(RED "readlink" RESET);
        return;
    }
    exe_path[read] = '\0';
    int ff = 0;
    if(strlen(exe_path) >= home_len){
        for (size_t i = 0; i < home_len; i++)
        {
            if(exe_path[i] != home_dir[i]){
                ff = 1;
                break;
            }
        }
        if(ff==0){
            exe_path = exe_path + home_len-1;
            exe_path[0] = '~';

        }

    }
    printf("Executable Path -- %s\n", exe_path);
    free(line);
    // free(exe_path);
    free(proc_path);

    return;



}
void exec_pinfo(int argc,char* argv[])
{
    pid_t pid;
    if (argc == 1)
    {
        pid = getpid();
    }
    else if (argc == 2)
    {
        pid = atoi(argv[1]);
    }
    else
    {
        printf(RED "pinfo" RESET ": Too many arguments\n");
        return;
    }
    print_pinfo(pid);

}