#include "inputs.h"
#include "headers.h"
#include <termios.h>
#include <ctype.h>
#include "history.h"
#include "prompt.h"

struct termios orig_termios;

void disableRawMode() {
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) {
        perror("tcsetattr");
        exit(1);
    }
}

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
        perror("tcgetattr");
        exit(1);
    }
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        perror("tcsetattr");
        exit(1);
    }
}

char* rawinput(){
    setbuf(stdout, NULL);
    enableRawMode();

    char * line = NULL;
    line = (char *)malloc(sizeof(char)*257);
    memset (line, '\0', 257);
    char c;
    int i = 0;
    int last_com = last_executed;

    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (iscntrl(c))
        {
            if (c == 10)
            {
                printf("%c", c);
                break;
            }
            else if(c==27){
                // escape detected
                char seq[3];
                if(read(STDIN_FILENO,seq,2)==2){
                    // printf("%s",seq);
                    if(seq[0]==91){
                        if(seq[1]==65){
                            // up arrow

                            if(last_com>=0){
                                printf("\r");
                                prompt();
                                for(int j=0;j<i;j++){
                                    printf(" ");
                                }
                                printf("\r");
                                prompt();
                                printf("%s",history[last_com]);
                                strcpy(line,history[last_com]);
                                i = strlen(line);
                                last_com--;
                            }
                            else if(count > 20 && last_com + 20 > last_executed){
                                printf("\r");
                                prompt();
                                for(int j=0;j<i;j++){
                                    printf(" ");
                                }
                                printf("\r");
                                prompt();
                                printf("%s",history[last_com+20]);
                                strcpy(line,history[last_com+20]);
                                i = strlen(line);
                                last_com--;

                            }
                        }
                        else if(seq[1]==66){
                            // down arrow
                            printf("\r");
                            prompt();
                            for(int j=0;j<i;j++){
                                printf(" ");
                            }
                            printf("\r");
                            prompt();
                            if(last_com < 0){
                                printf("%s",history[(last_com+22)%20]);
                                strcpy(line,history[(last_com+22)%20]);
                                i = strlen(line);
                                last_com++;
                            }
                            else if(last_com<last_executed-1){
                                printf("%s",history[last_com+2]);
                                strcpy(line,history[last_com+2]);
                                i = strlen(line);
                                last_com++;
                            }
                            else if(last_com==last_executed-1){
                                memset(line,'\0',257);
                                i = 0;
                                last_com++;
                            }
                        }
                        else if(seq[1]==67){
                            // right arrow
                        }
                        else if(seq[1]==68){
                            // left arrow
                        }
                    }
                }
            }
            else if (c==127)
            {
                if(i>0){
                    if(line[i-1]=='\t'){
                        for(int j=0;j<4;j++){
                            printf("\b \b");
                        }
                    }
                    line[--i]='\0';
                    printf("\b \b");
                }

            }
            else if(c==9){
                for(int j=0;j<3;j++){
                    printf(" ");
                }
                line[i++]='\t';
            }
            else if(c==4) {
                exit(0);
            }
            else {
                printf("%d\n",c);
            }



        }
        else{
            line[i++]=c;
            printf("%c",c);
        }
    }
    disableRawMode();
    return line;





}

char** parse_input(char* command, int* argc)
{
    unsigned int len = strlen(command);
    char* comm = (char*) malloc(sizeof(char) * (len+1));
    strcpy(comm, command);
    unsigned int j = 0;


    for (; j < len ; j++)
    {
        if (comm[j] == ' ' || comm[j] == '\t')
        {
            comm[j] = '\0';
        }
    }
    long long int last = -1;
    for (long long int  i = 0; i <= len; i++)
    {
        if (comm[i] == '\0' )
        {
            if((i-last) > 1)
            {
                (*argc)++;
            }
            last = i;
        }

    }

    char **argv = (char**) malloc(sizeof(char*) * (*argc+1));
    last =-1;
    unsigned int carg = 0;
    for (long long int i = 0; i < len; i++)
    {
        if (comm[i] == '\0')
        {
            last = i;
            continue;
        }
        if((i-last) == 1){
            if (strlen(comm+i) > 0)
            {
                argv[carg++]  = (char*) malloc(sizeof(char) * (strlen(comm+i)+1));
                strcpy(argv[carg-1], comm+i);
            }
        }
    }
    argv[carg] = NULL;
    free(comm);
    return argv;
}



char* get_input(){
    char *line = NULL;

    unsigned long len = 0;
    len = getline(&line, &len, stdin);
    if(len > 0){
        line[len-1] = '\0';
    }
    return line;
}