#include "prompt.h"
#include "headers.h"
// #include <stdlib.h>
struct passwd *pw;
struct utsname un;
unsigned int home_len;

#define MAX_PATH_LEN 1024
char* home_dir = NULL;


void init_shell()
{
    uid_t uid = geteuid();

    if (uname(&un) == -1)
    {
        perror(RED "uname" RESET);
        exit(1);
    }
    pw = getpwuid(uid);
    if (pw == NULL)
    {
        perror(RED "getpwuid" RESET);
        exit(1);
    }


    char *cwd = getcwd(NULL, 0);
    home_dir = (char*) malloc(sizeof(char) * strlen(cwd));
    strcpy(home_dir, cwd);
    home_len = strlen(home_dir);
    char clear[] = "\e[1;1H\e[2J";
    // write(STDOUT_FILENO,clear,strlen(clear));
    free(cwd);
}




void prompt() {
    // static int toclear = 1;
    // if (toclear)
    // {
    //
    //     toclear = 0;
    // }

    printf(CYAN "<%s@%s:" RESET,pw->pw_name,un.nodename);

    char *dir = getcwd(NULL, 0);
    unsigned int len = strlen(dir);

    if(len < home_len){
        printf(YELLOW "%s" RESET,dir);
        // time taken greater than 2 seconds
        if(last_time > 2){
            printf(" %s",RED);
            printf("took %d",last_time);
            printf("s%s",RESET);
        }
        printf(">");
    }
    else{
        for(unsigned int i=0; i<home_len; i++){
            if(dir[i] != home_dir[i]){
                printf(YELLOW "%s" RESET,dir);
                free(dir);
                if(last_time > 2){
                    printf(" %s",RED);
                    printf("took %d",last_time);
                    printf("s%s",RESET);
                    printf(">");
                }
                return;
            }
        }
        printf(YELLOW "~%s" RESET,dir+home_len);
        if(last_time > 2){
            printf(" %s",RED);
            printf("took %d",last_time);
            printf("s%s",RESET);
        }
        printf(">");
    }
    free(dir);

}
