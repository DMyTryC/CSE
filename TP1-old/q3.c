#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char **argv) {

	pid_t pid ; int i , n ;

	n = atoi(argv[1]) ;
	pid = fork() ;

	for (i=0 ; i < n ; i++) {

		switch(pid) {

			case -1 : exit(1) ;

			case 0 : printf("processus pid %d node %d val = %d\n", getpid(), i, rand()) ; pid = fork(); break ;

			default : waitpid(pid, NULL, 0) ;

		}

	}


	return 0 ;

}
