#include "autocomplete.h"
#include "headers.h"
#include "prompt.h"
#include <dirent.h>
#include <sys/stat.h>

void autocomplete(char *key,int * i){
    char *dupl = strdup(key);
    char* saveptr;
    char *token = strtok_r(dupl," ",&saveptr);
    char *last = NULL;
    while(token!=NULL){
        last = token;
        token = strtok_r(NULL," ",&saveptr);
    }

    char *path;
    char* tocom;
    if(last == NULL){
        path = strdup(".");
        tocom = "";
    }
    else{
        // path = last;
        tocom = strrchr(last,'/');
        if(tocom == NULL){
            path = strdup(".");
            tocom = last;
        }
        else{
            tocom++;
            path = strndup(last,strlen(last)-strlen(tocom));


        }
    }
    char *newpath = (char *) malloc(sizeof(char) * (strlen(path) + home_len) + 4);
    if(path[0] == '~'){
        strcpy(newpath,home_dir);
        sprintf(newpath,"%s%s",newpath,path+1);
    }
    else{
        strcpy(newpath,path);
    }

    // int len = strlen(newpath);
    // char* tocom = strrchr(newpath,'/');
    // if(tocom == NULL){
    //     tocom = newpath;
    //     newpath = ".";
    // }
    // else if(strlen(tocom) == 1){
    //     tocom = "";
    // }
    // else{
    //     tocom++;
    // }
    int tocomlen = strlen(tocom);
    // struct dirent **namelist;
    struct dirent *entry;
    DIR *dir  = opendir(newpath);
    if(dir == NULL){
        return;
    }
    int n = 0;
    while((entry = readdir(dir))!=NULL){
        n++;
    }
    closedir(dir);
    // namelist = (struct dirent **)malloc(n*sizeof(struct dirent *));
    dir = opendir(newpath);
    if (dir == NULL)
    {
        return;
    }
    int k =0;
    char** toprint = (char**)malloc(n*sizeof(char*));

    while((entry = readdir(dir))!=NULL){
        if(strcmp(entry->d_name,".")==0){
            continue;
        }

         if(strncmp(tocom,entry->d_name,tocomlen) == 0){
            char* npath = (char *) malloc(sizeof(char) * (strlen(newpath) + strlen(entry->d_name) + 2));
            sprintf(npath, "%s/%s", newpath, entry->d_name);
            struct stat nst;
            if (stat(npath, &nst) == -1){
                perror(RED "ls : stat" RESET);
                exit(1);
                return;
                // continue
            }
            toprint[k] = (char *) malloc(sizeof(char) * (strlen(entry->d_name) + 2));
            if(S_ISDIR(nst.st_mode)){
                sprintf(toprint[k], "%s/", entry->d_name);
            }
            else{
                sprintf(toprint[k], "%s", entry->d_name);
            }
            free(npath);
            k++;
        }

    }
    // for(int j=0;j<n;j++){
    //     if(strncmp(tocom,namelist[j]->d_name,tocomlen) == 0){
    //         char* npath = (char *) malloc(sizeof(char) * (strlen(newpath) + strlen(namelist[j]->d_name) + 2));
    //         sprintf(npath, "%s/%s", newpath, namelist[j]->d_name);
    //         struct stat nst;
    //         if (stat(npath, &nst) == -1){
    //             perror(RED "ls : stat" RESET);
    //             exit(1);
    //             return;
    //             // continue
    //         }
    //         toprint[k] = (char *) malloc(sizeof(char) * (strlen(namelist[j]->d_name) + 2));
    //         if(S_ISDIR(nst.st_mode)){
    //             sprintf(toprint[k], "%s/", namelist[j]->d_name);
    //         }
    //         else{
    //             sprintf(toprint[k], "%s", namelist[j]->d_name);
    //         }
    //         free(npath);
    //         k++;
    //     }
    // }
    // free(namelist);
    closedir(dir);

    if(k ==0){
        return;
    }
    else if(k==1){
        for(int l=0;l<strlen(toprint[0]) - tocomlen;l++){
            printf("%c",toprint[0][tocomlen+l]);
            key[*i] = toprint[0][tocomlen+l];
            (*i)++;
        }
    }
    else{
        int minlen = strlen(toprint[0]);
        char* common = (char*)malloc(sizeof(char)*(minlen+1));
        strcpy(common,toprint[0]);
        printf("\n");
        for(int j=0;j<k;j++){
            for(int l=0;l<minlen;l++){
                if(toprint[j][l] != common[l]){
                    common[l] = '\0';
                    minlen = l;
                    break;
                }
            }
            printf("%s\n",toprint[j]);
        }
        printf("\n");
        prompt();
        printf("%s",key);
        for(int l=0;l<strlen(common) - tocomlen;l++){
            printf("%c",common[tocomlen+l]);
            key[*i] = common[tocomlen+l];
            (*i)++;
        }

    }
    free(path);

    return;



}

