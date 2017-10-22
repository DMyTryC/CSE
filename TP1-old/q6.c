#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char **argv) {

	pid_t pid ; int i , n , random , recu , tube[2];

	n = atoi(argv[1]) ;
	pid = fork() ;
  random = random() ;

	for (i=0 ; i < n ; i++) {

		switch(pid) {

			case -1 : exit(1) ;

			case 0 : close(tube[1]) ; read(tube[0], &recu, sizeof(recu)) ; printf("processus pid %d node %d val = %d\n", getpid(), i, random) ; close(tube[0]); pid = fork(); break ;

			default :  close(tube[0]) ; write(tube[1], &random , sizeof(random)) ; close(tube[1]) ; waitpid(pid, NULL, 0) ;

		}

	}


	return 0 ;

}
