#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char **argv){

	pid_t pid ; int i , n ; 

	n = atoi(argv[1]) ;
	pid = fork() ;

	for (i=0;i < n;i++) {

		switch(pid) {

			case -1 : exit(1) ;
	
			case 0 : printf("processus pid %d node %d val = %ld\n", getpid(), i, random()) ; pid = fork(); break ;
	
			default : wait(NULL) ;
	
		}

	}
	
	
	return 0 ; 

}
