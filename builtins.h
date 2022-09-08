#ifndef __BUILTINS_H__
#define __BUILTINS_H__

void exec_cd(int argc, char *argv[]);
void exec_pwd(int argc, char *argv[]);
void exec_exit(int argc, char *argv[]);
void exec_echo(int argc, char *argv[]);
void exec_ls(int argc, char *argv[]);
int lsutil_printfile(char *path, int flag_a, int flag_l);
void lsutil_printdir(char *path, int flag_a, int flag_l);

#endif // __BUILTINS_H__