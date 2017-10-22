#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int i;

    for (i=0; i<3; i++)
        fork();
    printf("Je suis le processus %d, mon pere est %d\n", getpid(), getppid());
    return 0;
}
