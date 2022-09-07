#include "history.h"
#include "headers.h"
#include "prompt.h"
int count;
int last_executed;

char *hist_path = NULL;
char *history[20] ;

void init_history()
{
;
    hist_path = (char*) malloc(sizeof(char) * (strlen(home_dir) + 14));
    sprintf(hist_path, "%s/.dish_hist", home_dir);
    count =0;
    last_executed = 0;
    FILE *fp = fopen(hist_path, "r");
    if (fp == NULL)
    {
        fp = fopen(hist_path, "w");
        fclose(fp);
        return;
    }
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (line[strlen(line) - 1] == '\n')
        {
            line[strlen(line) - 1] = '\0';
        }
        history[count] = (char*) malloc(sizeof(char) * (strlen(line) + 1));
        strcpy(history[count], line);
        count++;
    }
    last_executed = count-1;
    if(fclose(fp) == -1){
        printf("historr : Error closing file\n");
    }
    return;
}

void write_history(char *cmd)
{
    int curr = count % 20;
    if (cmd == NULL)
    {
        return;
    }
    if(cmd[strlen(cmd)-1] == '\n'){
        cmd[strlen(cmd)-1] = '\0';
    }
    if(count == 0){
        history[0] = (char*) malloc(sizeof(char) * (strlen(cmd) + 1));
        strcpy(history[0], cmd);
        last_executed  = count;
        count++;
        return;
    }
    if (strcmp(history[last_executed], cmd) == 0)
    {
        return;
    }
    else{
        if (history[curr] != NULL)
        {
            free(history[curr]);
        }
        history[curr] = (char*) malloc(sizeof(char) * (strlen(cmd) + 1));
        strcpy(history[curr], cmd);
        last_executed = curr;
        count++;
    }
    return;
}

void print_history(int num)
{
    if (num <= 0)
    {
        printf("history: %d: numeric argument required\n", num);
        return;
    }
    if(count < 20){
        if(count == 0){
            printf("history : No history\n");
            return;
        }

        if (num > count)
        {
            num = count;
        }

    }
    if(num > 20){
        printf("history: 20 values to show\n");
        num = 20;
    }
    // int i = count%20;
    // int j = last_executed;
    // int k = 0;
    if(last_executed +1 < num){

        for(int i = 20 - (num - last_executed-1); i < 20; i++){
            printf("[%d] : %s\n", i+num-20-last_executed, history[i]);
        }
    // while (1)
        for(int i = 0; i <= last_executed; i++){
            printf("[%d] : %s\n", i+1 +(num-last_executed-1), history[i]);
        }
    // {
    //     if(j<0){
    //         j = 19;
    //     }
    //     printf("[%d]: %s\n",k+1, history[j]);

    //     k++;
    //     if(k == num){
    //         break;
    //     }
    //     j--;

    // }
    }
    else{
        for(int i = last_executed - num+1; i <= last_executed; i++){
            printf("[%d] : %s\n", i+num-last_executed, history[i]);
        }
    }

    return;


}
// You must overwrite the oldest commands if
// more than the set number of commands are entered.
void write_history_to_file()
{
    FILE *fp = fopen(hist_path, "w");
    if (fp == NULL)
    {
        printf("history: Error opening file\n");
    }
    int sz = count < 20 ? count : 20;
    int i = count % 20;

    // for (int j = 0; j < sz; j++)
    // {
    //     int inx = (i + j) % 20;
    //     fprintf(fp, "%s\n", history[inx]);
    // }
    if(count >=20){
        for(int j =i;j<20;j++){
            fprintf(fp, "%s\n", history[j]);
        }
        for(int j =0;j<i;j++){
            fprintf(fp, "%s\n", history[j]);
        }
    }
    else{
        for(int j =0;j<count;j++){
            fprintf(fp, "%s\n", history[j]);
        }
    }
    if(fclose(fp) == -1){
        printf(RED "historr" RESET " : Error closing file\n");
    }
    return;

}

void exec_history(int argc,char* argv[]){
    if(argc == 1){
        print_history(20);
    }
    else if(argc == 2){
        int num = atoi(argv[1]);
        print_history(num);
    }
    else{
        printf(RED "history" RESET ": Too many arguments\n");
    }
    return;
}



