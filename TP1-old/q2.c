#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char **argv) {

	pid_t pid ; int i , n ;

	n = atoi(argv[1]) ;
	pid = fork() ;
	
	if (pid != 0) printf("Je suis le processus %d, mon pere est %d\n", getpid(), getppid()) ;

	for (i=0;i < n - 1;i++) {

		switch(pid) {

			case -1 : exit(1) ;
	
			case 0 : printf("Je suis le processus %d, mon pere est %d\n", getpid(), getppid()) ; pid = fork(); break ;
	
			default : waitpid(pid, NULL, 0) ;
	
		}

	}
	
	
	return 0 ; 

}
