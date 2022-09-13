#include "prompt.h"
#include "headers.h"
#include "inputs.h"
#include "execute.h"
#include "process.h"
#include "signalhandling.h"
#include "history.h"
#include "exe.h"
time_t last_time = 0;
int ORIG_STDIN;
int ORIG_STDOUT;
int main()
{

    init_shell();
    init_process_list();
    init_history();
    signal(SIGINT, sigint_handler);
    signal(SIGTSTP, sigtstop_handler);

    signal(SIGCHLD,process_ch_handler);

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

    while (1)
    {
        prompt();
        last_time = 0;
        char* line;
        line = rawinput();
        // printf("%s",line);
        if (strcmp(line,"") == 0)
        {
            continue;
        }


        char* copy = (char*)malloc(strlen(line) + 1);
        strcpy(copy, line);
        char* paste = (char*)malloc(strlen(line) + 1);
        strcpy(paste, line);
        char* saveptr;
        char* com = strtok_r(paste, ";&\n", &saveptr);
        int i = strlen(com);
        while (com != NULL)
        {
            int bp=0;
            if(copy[i] == '&')
            {
                // printf("Background process %s\n", com);
                bp = 1;

            }
            // else
            // {
            //     // execute(com, 0);
            //     printf("Foreground process %s\n", com);
            // }

            int argc = 0;
            char** argv = parse_input(com, &argc);
            if (argc > 0)
            {
                exe_redi_pip(argc, argv, bp);
            }
            // printf("\n");
            // for (int i = 0; i < argc; i++)
            // {
            //     printf("%s ", argv[i]);
            // }
            // printf("\n");

            for(int i=0;i<argc;i++)
            {
                if(argv[i] == NULL)
                {
                    continue;
                }
                free(argv[i]);
            }
            free(argv);

            com = strtok_r(NULL, ";&\n", &saveptr);
            if (com==NULL)
            {
                break;
            }

            i+=strlen(com)+1;
        }
        free(copy);
        free(paste);
        write_history(line);

    }
}

/*
comm1 ; comm2 ; comm3

comm1 == cd       Downloads

argc = 2
argv[0] = cd
argv[1] = Downloads
*/