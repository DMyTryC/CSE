#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char **argv){

	pid_t pid ; int tube[2] ; FILE * fichier ; char message[256] ;

	if(pipe(tube) == -1) {
		fprintf(stderr, "Erreur de creation de tube\n") ;
		exit(1);
	}

	pid = fork() ;

	switch(pid) {

		case -1 : printf("Erreur a la creation du fork\n") ; exit(2) ;

		case 0 :

			close(tube[1]) ;
			while (read(tube[0], message, sizeof(message)) > 0){
				printf("Message bien recu fils : %s", message) ;
			}
			close(tube[0]) ;
			break ;

		default :

			fichier = fopen(argv[1], "r") ;
			close(tube[0]) ;

			while(fgets(message, 256, fichier)) {
				write(tube[1], message, sizeof(message)) ; printf("Ligne envoyee : %s", message) ;
			}

			close(tube[1]) ;
	}

	return 0 ;

}
