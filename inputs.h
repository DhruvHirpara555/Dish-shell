#ifndef __INPUTS_H__
#define __INPUTS_H__


void disableRawMode();
void enableRawMode();
char* rawinput();
char* get_input();
char** parse_input(char* comm, int* argc);

#endif