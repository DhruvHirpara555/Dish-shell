#ifndef __HISTORY_H__
#define __HISTORY_H__

void init_history();
void exec_history(int argc, char *argv[]);
void write_history(char *cmd);
void read_history();
void write_history_to_file();


#endif