#include "headers.h"
#include "builtins.h"
#include "prompt.h"
#include <sys/stat.h>
#include <dirent.h>
#include <grp.h>
#include <time.h>
#include <ctype.h>

char* lastused = NULL;
// char* home_dir;
void exec_cd(int argc, char *argv[])
{
    char* cwd = getcwd(NULL, 0);
    if (argc == 1){
        // chdir(home_dir);
        if (chdir(home_dir) == -1){
            perror(RED "chdir" RESET);
            // exit(1);
        }
    }
    else if (argc == 2){
        char* newpath = (char *) malloc(sizeof(char) * (strlen(argv[1]) + home_len));

        if(argv[1][0] == '~' && (argv[1][1] == '\0' || argv[1][1] == '/')){
            strcpy(newpath, home_dir);
            sprintf(newpath, "%s%s", newpath, argv[1]+1);
            // chdir(newpath);
            if (chdir(newpath) == -1){
                perror(RED "cd" RESET);
                // exit(1);
            }
        }
        else if (strcmp("-",argv[1]) == 0){
            if (lastused == NULL){
                lastused = home_dir;
            }

            if (chdir(lastused) == -1){
                perror(RED "cd" RESET);
                // exit(1);
            }
        }
        else{
            // chdir(argv[1]);
            if (chdir(argv[1]) == -1){
                perror(RED "cd" RESET);
                // exit(1);
            }
        }
        free(newpath);

    }
    else{
        printf(RED "cd" RESET ": too many arguments\n");
    }
    if(lastused != NULL){
        free(lastused);
    }
    lastused = strdup(cwd);
    free(cwd);
}

void exec_pwd(int argc, char *argv[])
{
    if (argc != 1)
    {
        printf(RED"pwd" RESET ": too many arguments\n");
        return;
    }

    char* cwd = getcwd(NULL, 0);
    write(STDOUT_FILENO,cwd,strlen(cwd));
    write(STDOUT_FILENO,"\n",1);
    free(cwd);
}

void exec_exit(int argc, char *argv[])
{
    if (argc == 1){
        exit(0);
    }
    else{
        printf(RED "exit" RESET ": too many arguments\n");
    }
}

void exec_echo(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++){
        write(STDOUT_FILENO,argv[i],strlen(argv[i]));
        write(STDOUT_FILENO," ",1);
    }
    write(STDOUT_FILENO,"\n",1);
}


int cmp_str(const void *a, const void *b)
{
    // return strcmp(*(char **)a, *(char **)b);
    char *s1 = strdup(*(char **)a);
    char *s2 = strdup(*(char **)b);
    // compare the base name of the file
    char *itr = strtok(s1, "/");
    char *base1 = itr;
    while (itr != NULL){
        if(strlen(itr) > 0){
            base1 = itr;
        }
        itr = strtok(NULL, "/");
    }
    char *itr2 = strtok(s2, "/");
    char *base2 = itr2;
    while (itr2 != NULL){
        if(strlen(itr2) > 0){
            base2 = itr2;
        }
        itr2 = strtok(NULL, "/");
    }
    free(s1);
    free(s2);
    return strcasecmp(base1, base2);
}

void exec_ls(int argc, char *argv[])
{
    int flag_a = 0;
    int flag_l = 0;
    char **path_array = (char **) malloc(sizeof(char *) * argc);
    int path_count = 0;
    for (int i = 1; i < argc; i++){
        if (argv[i][0] == '-'){
            for (unsigned int j = 1; j < strlen(argv[i]); j++){
                if (argv[i][j] == 'a'){
                    flag_a = 1;
                }
                else if (argv[i][j] == 'l'){
                    flag_l = 1;
                }
                else{
                    printf("ls: invalid option -- '%c\n'",argv[i][j]);
                    return;
                }

            }
        }
        else{
            path_array[path_count] = argv[i];
            path_count++;
        }
    }
    if (path_count == 0){
        path_array[path_count] = ".";
        path_count++;
    }

    qsort(path_array, path_count, sizeof(char *), cmp_str);
    // print all the files in the path_array
    int path_done[path_count];
    for(int i = 0;i< path_count;i++){
        path_done[i] = lsutil_printfile(path_array[i],flag_a,flag_l);
    }
    // print all the directories in the path_array
    for (int  i = 0; i < path_count; i++)
    {
        if(path_done[i]){
            continue;
        }
        if (path_count > 1){
            printf("%s:\n",path_array[i]);
        }
        lsutil_printdir(path_array[i],flag_a,flag_l);

    }
    free(path_array);

    return;

}
int filter_hidden(const struct dirent *dir)
{
    if (dir->d_name[0] == '.'){
        return 0;
    }
    return 1;
}



char* permi_file(struct stat file_stat)
{
    char* permi = (char *) malloc(sizeof(char) * 11);
    permi[0] = (S_ISDIR(file_stat.st_mode)) ? 'd' : '-';
    permi[1] = (file_stat.st_mode & S_IRUSR) ? 'r' : '-';
    permi[2] = (file_stat.st_mode & S_IWUSR) ? 'w' : '-';
    permi[3] = (file_stat.st_mode & S_IXUSR) ? 'x' : '-';
    permi[4] = (file_stat.st_mode & S_IRGRP) ? 'r' : '-';
    permi[5] = (file_stat.st_mode & S_IWGRP) ? 'w' : '-';
    permi[6] = (file_stat.st_mode & S_IXGRP) ? 'x' : '-';
    permi[7] = (file_stat.st_mode & S_IROTH) ? 'r' : '-';
    permi[8] = (file_stat.st_mode & S_IWOTH) ? 'w' : '-';
    permi[9] = (file_stat.st_mode & S_IXOTH) ? 'x' : '-';
    permi[10] = '\0';
    return permi;
}

int cmpfunc (const void * a, const void * b)
{
//    return ( strcmp(*(char **)a, *(char **)b) );
    struct dirent *dir1 = *(struct dirent **)a;
    struct dirent *dir2 = *(struct dirent **)b;
    char *str_dir1 = dir1->d_name;
    char *str_dir2 = dir2->d_name;
    if(str_dir1[0] == '.'){
        str_dir1++;
    }
    if(str_dir2[0] == '.'){
        str_dir2++;
    }
    return strcasecmp(str_dir1, str_dir2);
    // return ( strcmp(str_dir1, str_dir2) );
    // return ( strcmp(dir1->d_name, dir2->d_name) );
}
int lsutil_printfile(char *path, int flag_a,int flag_l)
{
    char* cwd = getcwd(NULL, 0);
    char* newpath = (char *) malloc(sizeof(char) * (strlen(path) + home_len));

    if (path[0] == '~' && (path[1] == '\0' || path[1] == '/')){
        strcpy(newpath, home_dir);
        sprintf(newpath, "%s%s", newpath, path+1);

        // path = newpath;
    }
    else{
        strcpy(newpath, path);
    }
    struct stat st;
    if (stat(newpath, &st) == -1){
        perror(RED "stat" RESET);
        return 0;
        // exit(1);
    }
    if(!S_ISDIR(st.st_mode)){
        // struct stat st;
        // if (stat(newpath, &st) == -1){
        //     perror(RED "stat" RESET);
        //     // exit(1);
        //     return 1;
        // }

        char* duplicate = strdup(newpath);


        // char* filename = strtok(duplicate, "/");

        // while (strtok(NULL, "/") != NULL){
        //     filename = strtok(NULL, "/");
        // }
        if(st.st_mode & S_IXUSR){
            printf(GREEN);
        }
        else{
            printf(WHITE);
        }
        if (flag_l == 1){

            char* permi = permi_file(st);
            char* time = ctime(&st.st_mtime);
            time[strlen(time)-1] = '\0';
            printf("%s %ld %s %s %ld %s %s", permi, st.st_nlink, getpwuid(st.st_uid)->pw_name, getgrgid(st.st_gid)->gr_name, st.st_size, time, path);
            free(permi);
            // free(time);
        }
        else{
            printf("%s", path);
        }
        printf(RESET);
        printf("\n\n");

        free(duplicate);
        free(newpath);
        free(cwd);
        return 1;
    }
    free(newpath);
    free(cwd);
    return 0;

}

void lsutil_printdir(char *path, int flag_a, int flag_l)
{
    char* cwd = getcwd(NULL, 0);
    char* newpath = (char *) malloc(sizeof(char) * (strlen(path) + home_len));
    if (path[0] == '~' && (path[1] == '\0' || path[1] == '/')){
        strcpy(newpath, home_dir);
        sprintf(newpath, "%s%s", newpath, path+1);

        // path = newpath;
    }
    else{
        strcpy(newpath, path);
    }
    struct stat st;
    if (stat(newpath, &st) == -1){
        perror(RED "stat" RESET);
        return;
        // exit(1);
    }

    // DIR *dir = opendir(path);
    if(!S_ISDIR(st.st_mode)){

        free(newpath);
        free(cwd);
        return;
    }

    struct dirent **namelist;
    struct dirent *dir;
    DIR *d;
    int n = 0;
    d = opendir(newpath);
    if (d == NULL){
        perror(RED "opendir" RESET);
        return;
        // exit(1);
    }
    while ((dir = readdir(d)) != NULL){
        if(flag_a == 0 && dir->d_name[0] == '.'){
            continue;
        }
        n++;
    }
    closedir(d);
    namelist = (struct dirent **) malloc(sizeof(struct dirent *) * n);
    d = opendir(newpath);
    if (d == NULL){
        perror(RED "opendir" RESET);
        return;
        // exit(1);
    }
    int i = 0;
    while ((dir = readdir(d)) != NULL){
        if(flag_a == 0 && dir->d_name[0] == '.'){
            continue;
        }
        namelist[i] = dir;
        i++;
    }


    qsort(namelist, n, sizeof(struct dirent *), cmpfunc);

    // if (flag_a == 1){
    //     n = scandir(newpath, &namelist, NULL, alphasort);
    // }
    // else{
    //     n = scandir(newpath, &namelist, filter_hidden, alphasort);
    // }
    // if (n == -1){
    //     perror(RED "ls" RESET);
    //     // exit(1);
    //     return;
    // }

    if(flag_l == 1){
        for(int i = 0; i < n; i++){
            // if (namelist[i]->d_name[0] == '.' && flag_a == 0){
            //     continue;
            // }
            char* npath = (char *) malloc(sizeof(char) * (strlen(newpath) + strlen(namelist[i]->d_name) + 2));
            sprintf(npath, "%s/%s", newpath, namelist[i]->d_name);
            struct stat nst;
            if (stat(npath, &nst) == -1){
                perror(RED "ls : stat" RESET);
                exit(1);
                return;
                // continue
            }
            char* permi = permi_file(nst);
            char* time = ctime(&nst.st_mtime);
            if(S_ISDIR(nst.st_mode)){
                printf(BLUE);
            }
            else if(nst.st_mode & S_IXUSR){
                printf(GREEN);
            }
            else{
                printf(WHITE);
            }
            time[strlen(time)-1] = '\0';
            printf("%s %ld %s %s %ld %s %s", permi, nst.st_nlink, getpwuid(nst.st_uid)->pw_name, getgrgid(nst.st_gid)->gr_name, nst.st_size, time, namelist[i]->d_name);
            free(permi);
            printf(RESET);
            // free(time);
            printf("\n");
            free(npath);
        }
    }
    else{


        for(int i = 0; i < n; i++){
            char* npath = (char *) malloc(sizeof(char) * (strlen(newpath) + strlen(namelist[i]->d_name) + 2));
            sprintf(npath, "%s/%s", newpath, namelist[i]->d_name);
            struct stat nst;
            if (stat(npath, &nst) == -1){
                perror(RED "ls : stat" RESET);
                exit(1);
                return;
                // continue
            }
            if(S_ISDIR(nst.st_mode)){
                printf(BLUE);
            }
            else if(nst.st_mode & S_IXUSR){
                printf(GREEN);
            }
            else{
                printf(WHITE);
            }
            printf("%s\n", namelist[i]->d_name);
            printf(RESET);
            free(npath);
        }
        printf("\n");
    }
    free(newpath);
    closedir(d);
    free(namelist);




}