#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    pid_t pid;

    pid = fork();
    switch (pid) {
      case -1:
        fprintf(stderr, "Erreur de fork\n");
        exit(1);
      case 0:
        printf("Je suis le fils, mon pid est %d, celui de mon pere %d\n",
               getpid(), getppid());
        break;
      default:
        printf("Je suis le pere, mon pid est %d, celui de mon pere %d\n",
               getpid(), getppid());
    }
    return 0;
}
