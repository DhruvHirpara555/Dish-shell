#include "signalhandling.h"
#include "headers.h"
#include "prompt.h"
void sigint_handler(int sig) {
    printf("\n");
    prompt();
    fflush(stdout);
    return;
}

void sigtstop_handler(int sig)
{
    printf("\n");
    prompt();
    fflush(stdout);
    return;
}
