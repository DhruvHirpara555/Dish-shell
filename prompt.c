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
        perror("uname");
        exit(1);
    }
    pw = getpwuid(uid);
    if (pw == NULL)
    {
        perror("getpwuid");
        exit(1);
    }


    char *cwd = getcwd(NULL, 0);
    home_dir = (char*) malloc(sizeof(char) * strlen(cwd));
    strcpy(home_dir, cwd);
    home_len = strlen(home_dir);
    free(cwd);
}




void prompt() {
    static int toclear = 1;
    if (toclear)
    {
        char clear[] = "\e[1;1H\e[2J";
        write(STDOUT_FILENO,clear,strlen(clear));
        toclear = 0;
    }

    printf("%s@%s",pw->pw_name,un.nodename);

    char *dir = getcwd(NULL, 0);
    unsigned int len = strlen(dir);

    if(len < home_len){
        printf("%s>",dir);
    }
    else{
        for(unsigned int i=0; i<home_len; i++){
            if(dir[i] != home_dir[i]){
                printf("%s>",dir);
                break;
            }
        }
        printf("~%s>",dir+home_len);
    }
    free(dir);

}
