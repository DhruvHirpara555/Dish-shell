#ifndef __HISTORY_H__
#define __HISTORY_H__

void init_history();
void exec_history(int argc, char *argv[]);
void write_history(char *cmd);
void read_history();
void write_history_to_file();

extern char* history[20];
extern int count;
extern int last_executed;
extern char* hist_path;


#endif