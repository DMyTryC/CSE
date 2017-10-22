// PRE-PROCESSING

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

/* Constantes utiles pour l'utilisation des tubes */

#define SORTIE_TUBE 0
#define ENTREE_TUBE 1

// PROGRAMME

int main(int argc, char **argv){

	pid_t pid ; // Pid du processus en cours d'exécution
	int nb_processus = argc >= 2 ? atoi(argv[1]) : 0 ; // nombre de processus à générer demandés par l'utilisateur 
	int (*tubes)[2] ; // Pointeur de tubes pour la communication entre les différents processus
	int entier ; // Entier qui sera transféré de processus en processus
	
	/* Arrêt du programme en cas d'absence/non-conformité de l'argument */
	
	if(nb_processus < 1) {
		
		fprintf(stderr, "usage : %s nb_processus > 0\n", *argv) ;
		exit(3) ;
		
	}
	
	/* Génération pseudo-aléatoire d'un entier */ 
	
	srandom(time(NULL));
    entier = random() ;
	
	/* Création des tubes pour faire communiquer les processus (si nb_processus > 1) et traitement des erreurs */
	
	if (nb_processus != 1) {
	
		int indice_tube_courant = 0 ;
	
		tubes = malloc (sizeof *tubes * nb_processus - 1) ; // Allocation mémoire des tubes
		
		if(!tubes) {
			
			fprintf(stderr, "Erreur lors de l'appel à malloc\n") ;
			exit(4) ;
			
		}
		
		pid = fork() ; // Création du premier processus de la chaîne
		
		/* Création des processus restants et des tubes avant de transférer l'entier */
		
		for ( ; indice_tube_courant < nb_processus ; indice_tube_courant++) {
		
			switch (pid) {
		
				/* Traitement des erreurs du fork */
		
				case -1 :
			
					fprintf(stderr, "Erreur de fork\n") ; exit(2) ;
					
				/* Réception de l'entier par le fils */
			
				case 0 :
			
					close(tubes[indice_tube_courant][ENTREE_TUBE]) ; // Fermeture du descripteur inutilisé
					read(tubes[indice_tube_courant][SORTIE_TUBE], &entier, sizeof entier) ; // Lecture de l'entier dans le tube
					printf("Je suis le processus n°%d de pid %d : entier transféré = %d\n", indice_tube_courant + 1, getpid(), entier) ;
					close(tubes[indice_tube_courant][SORTIE_TUBE]) ; // Fermeture du descripteur devenu inutile
					pid = fork() ; // Génération du prochain fils
				
					break ;
					
				/* Envoi de l'entier par le père */
				
				default :
					
					wait(NULL) ; // Attente de la terminaison du fils
				
					if(pipe(tubes[indice_tube_courant]) == -1) {
						fprintf(stderr, "Erreur à la création de tube\n") ;
						free(tubes) ;
						exit(1);
					}
				
					close(tubes[indice_tube_courant][SORTIE_TUBE]) ; // Fermeture du descripteur inutilisé
					write(tubes[indice_tube_courant][ENTREE_TUBE], &entier, sizeof entier) ; // Ecriture de l'entier dans le tube
					close(tubes[indice_tube_courant][ENTREE_TUBE]) ; // Fermeture du descripteur devenu inutile
					
			}
			
		}
		
		free(tubes) ; // Libération de la mémoire allouée aux tubes
		
	}
	
	else printf("entier = %d\n", entier) ;
	
	return 0 ; 

}
