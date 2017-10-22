#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {

	pid_t pid ; int tube[2][2] ; char *message = "\"message\"";

	for (int i=0; i < 2 ; i++) {
		if(pipe(tube[i]) == -1) {
			fprintf(stderr, "erreur de création de tube\n") ;
			exit(1);
		}
	}

	pid = fork() ;

	switch(pid) {

		case -1 : printf("Erreur a la creation du fork\n") ; exit(2) ;

		case 0 :

			close(tube[0][1]) ; close(tube[1][0]) ; read(tube[0][0], message, sizeof(*message)) ; printf("Le fils a bien recu le message : %s \n", message) ; close(tube[0][0]) ;
			write(tube[1][1], message, sizeof(*message)) ; printf("Message renvoyé\n") ;
			break ;

		default :
			close(tube[0][0]) ; close(tube[1][1]) ; write(tube[0][1], message, sizeof(*message)) ; printf("Message envoyé\n") ; waitpid(pid, NULL, 0) ;
			read(tube[1][0], message, sizeof(*message)) ; printf("Le pere a bien recu le message : %s \n", message) ; close(tube[1][0]) ;

	}

	return 0 ;

}
