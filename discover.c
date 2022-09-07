#include "discover.h"
#include "headers.h"
#include <sys/stat.h>
#include <dirent.h>
#include "prompt.h"

void exec_discover(int argc,char* argv[])
{
    char* target_dir = ".";
    int flag_d = 0;
    int flag_f = 0;
    char* file_name = NULL;

    for (int i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-'){
            for(int j = 1; j < strlen(argv[i]); j++){
                if(argv[i][j] == 'd'){
                    flag_d = 1;
                }
                else if(argv[i][j] == 'f'){
                    flag_f = 1;
                }
                else{
                    printf("discover: Invalid flag\n");
                    return;
                }
            }
        }
        else if(argv[i][0] == '~' && (argv[i][1] =='\0' || argv[i][1] == '/')){
            target_dir = (char*)malloc(sizeof(char)*(strlen(home_dir)+strlen(argv[i])));
            strcpy(target_dir,home_dir);
            strcat(target_dir,argv[i]+1);

        }
        else if(argv[i][0] == '"' && argv[i][strlen(argv[i])-1] == '"'){
            file_name = (char*)malloc(sizeof(char)*(strlen(argv[i])-1));
            strncpy(file_name,argv[i]+1,strlen(argv[i])-2);
            file_name[strlen(argv[i])-2] = '\0';
            // printf("\n%s\n\n",file_name);
        }
        else{
            target_dir = (char*)malloc(sizeof(char)*(strlen(argv[i])+1));
            strcpy(target_dir,argv[i]);
        }
    }
    if(flag_d == 0 && flag_f == 0){
        flag_d = 1;
        flag_f = 1;
    }
    if (flag_d==1 && file_name==NULL)
    {
        printf("%s\n",target_dir);
    }
    recurse_search(target_dir,file_name,flag_d,flag_f);


    return;


}

void recurse_search(char* target_dir,char* file_name,int flag_d,int flag_f)
{

    struct dirent **namelist;
    int n;
    n = scandir(target_dir, &namelist, NULL, alphasort);
    if (n < 0)
    {
        perror(RED "discover :" RESET);
        return;
    }


    for(int i=0;i<n;i++)
    {
        // if(strcmp(namelist[i]->d_name,".")==0 || strcmp(namelist[i]->d_name,"..")==0)
        // {
        //     continue;
        // }
        if(namelist[i]->d_name[0] == '.')
        {
            continue;
        }
        char* path = (char*)malloc(sizeof(char)*(strlen(target_dir)+strlen(namelist[i]->d_name)+2));
        strcpy(path,target_dir);
        strcat(path,"/");
        strcat(path,namelist[i]->d_name);
        struct stat file_stat;
        stat(path,&file_stat);
        if(S_ISDIR(file_stat.st_mode))
        {
            if(flag_d == 1)
            {
                if(file_name == NULL)
                {
                    printf("%s\n",path);
                }
                else if(strcmp(namelist[i]->d_name,file_name)==0)
                {
                    printf("%s\n",path);
                }
            }
            recurse_search(path,file_name,flag_d,flag_f);
        }
        else
        {
            if(flag_f == 1)
            {
                if(file_name == NULL)
                {
                    printf("%s\n",path);
                }
                else if(strcmp(namelist[i]->d_name,file_name)==0)
                {
                    printf("%s\n",path);
                }
            }
        }

    }

    return;
}