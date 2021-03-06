// PRE-PROCESSING

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <limits.h>

/* Constantes utiles pour l'utilisation des tubes */

#define SORTIE_TUBE 0
#define ENTREE_TUBE 1

// Défintion d'une structure qui va référencer les informations du processus qui aura généré le plus grand entier
// Cette structure sera ensuite transférée dans l'anneau pour prévenir tous les processus de l'identité du gagnant

typedef struct info_proc {
	
	long int *ref_entier ; // Référence vers le plus grand entier généré
	int *ref_numero ; // Référence vers le numéro du gagnant
	pid_t *ref_pid ; // Référence vers le pid du gagnant
	
} info_proc ;
	

// PROGRAMME

int main(int argc, char **argv){

	pid_t pid ; // Pid du processus en cours d'exécution
	int nb_processus = argc >= 2 ? atoi(argv[1]) : 0 ; // nombre de processus à générer demandés par l'utilisateur 
	int (*tubes)[2] ; // Pointeur de tubes pour la communication entre les différents processus
	long int entier = LONG_MIN ; // Entier qui sera transféré de processus en processus
	int numero_processus_au_plus_grand_entier = 1 ; // Numéro du processus qui a généré le plus grand entier
	pid_t pid_au_plus_grand_entier ; // Indice du processus qui a généré le plus grand entier
	info_proc gagnant = {NULL,NULL,NULL} ; // Informations sur le processus gagnant
	
	signal(SIGPIPE, SIG_IGN) ; // On ignore les signaux SIGPIPE afin de pouvoir traiter les erreurs liées aux tubes
	
	/* Arrêt du programme en cas d'absence/non-conformité de l'argument */
	
	if(nb_processus < 2) {
		
		fprintf(stderr, "usage : %s nb_processus >= 2\n", *argv) ;
		exit(3) ;
		
	}
	
	int tubes[nb_processus][2] ; // Tableau de tubes pour la communication entre les différents processus (pour former un anneau, il nous faut autant de tubes que de processus)
	
	/* Initialisation de la graine */ 
	
	srandom(time(NULL));
	
	/* Création des tubes pour faire communiquer les processus et traitement des erreurs */
		
	long int r, w ; // variables pour stocker les résultats des fonctions read/write
	int indice_tube_courant = 0 ;
	int loop = 1 ; // booléen qui va nous permettre de nullifier l'exécution du processus initial dans la boucle
	
	for (; indice_tube_courant < nb_processus ; indice_tube_courant++) // Création des tubes
		if(pipe(tubes[indice_tube_courant]) == -1) {
			fprintf(stderr, "Erreur à la création de tube\n") ;
			free(tubes) ;
			exit(1);
	}
	
	pid = pid_initial = fork() ; // Création du premier processus de la chaîne
	
	/* Création des processus restants avant de transférer l'entier généré aléatoirement */
	
	for (indice_tube_courant = 0 ; indice_tube_courant < nb_processus ; indice_tube_courant++) {
	
		entier_aleatoire = random () ; // Génération pseudo-aléatoire de l'entier
		entier = entier < entier_aleatoire ? entier_aleatoire : entier ; // On chosit le max entre l'ancien et le nouvel entier */
		numero_processus_au_plus_grand_entier = entier == entier_aleatoire ? indice_tube_courant + 1 : numero_processus_au_plus_grand_entier ; // Mise à jour du numéro du processus qui a généré le lus grand entier
		pid_au_plus_grand_entier = entier == entier_aleatoire ? getpid() : pid_au_plus_grand_entier ; // Mise à jour du pid qui a généré le plus grand entier
	
		switch (pid) {
	
			/* Traitement des erreurs du fork */
	
			case -1 :
		
				fprintf(stderr, "Erreur de fork\n") ; exit(2) ;
				
			/* Réception de l'entier par le fils */
		
			case 0 :
			
				if (indice_tube_courant < nb_processus - 1) { // Si le fils n'est pas le dernier processus, on réceptionne l'entier
				
					close(tubes[indice_tube_courant][ENTREE_TUBE]) ; // Fermeture du descripteur inutilisé
					r = read(tubes[indice_tube_courant][SORTIE_TUBE], &entier, sizeof entier) ; // Lecture de l'entier dans le tube
					if (r == -1) perror("Read fils ") ;
					printf("Je suis le processus Fils n°%d de pid %d, entier reçu   : %10ld, Bytes reçus  : %ld\n", indice_tube_courant + 1, getpid(), entier, r) ;
					close(tubes[indice_tube_courant][SORTIE_TUBE]) ; // Fermeture du descripteur devenu inutile
			
					pid = fork() ; // Génération du prochain fils
					
				}
				
				else exit(0) ; // Sinon on le tue
				
				break ;
				
			/* Envoi de l'entier par le père */
			
			default :
				
				if (pid != pid_initial || loop) { // Cette condition oblige le processus initial maintenu en vie à n'exécuter ce bloc d'instructions qu'une seule fois
				
					close(tubes[indice_tube_courant][SORTIE_TUBE]) ; // Fermeture du descripteur inutilisé
					w = write(tubes[indice_tube_courant][ENTREE_TUBE], &entier, sizeof entier) ; // Ecriture de l'entier dans le tube
					if (w == -1) perror("Write père ") ;
					printf("Je suis le processus Père n°%d de pid %d, entier généré : %10ld, entier transféré : %ld, Bytes écrits : %ld\n", indice_tube_courant, getpid(), entier_aleatoire, entier, w) ;
					close(tubes[indice_tube_courant][ENTREE_TUBE]) ; // Fermeture du descripteur devenu inutile
					wait(NULL) ; // Attente de la terminaison du fils
					loop = 0 ;
					
				}
				
				if (pid != pid_initial) exit(0) ; // Terminaison du père sauf le père initial
				
		}
		
	}
		
	/* Formation de l'anneau et envoi des informations du gagnant par le dernier fils au processus initial */

	gagnant.ref_entier = &entier , gagnant.ref_numero = &numero_processus_au_plus_grand_entier , gagnant.ref_pid = &pid_au_plus_grand_entier ; // Référencement des infos

	switch (pid) {

		int tube[2] ;

		/* Traitement des erreurs du fork */

		case -1 :
	
			fprintf(stderr, "Erreur de fork\n") ; exit(2) ;

		/* Envoi des infos au processus initial par le fils */
	
		case 0 :

			if(pipe(tube) == -1) {
				fprintf(stderr, "Erreur à la création du tube\n") ;
				exit(1);
			}

			close(tube[SORTIE_TUBE]) ; // Fermeture du descripteur inutilisé
			write(tube[ENTREE_TUBE], &gagnant, sizeof gagnant) ; // Ecriture de l'entier dans le tube
			close(tube[ENTREE_TUBE]) ; // Fermeture du descripteur devenu inutile
		
		default :
	
			close(tube[ENTREE_TUBE]) ; // Fermeture du descripteur inutilisé
			read(tube[SORTIE_TUBE], &gagnant, sizeof gagnant) ; // Lecture de l'entier dans le tube
			close(tube[SORTIE_TUBE]) ; // Fermeture du descripteur devenu inutile
		
			printf("Le gagnant est : entier %ld, pid %d, numéro %d\n", *(gagnant.ref_entier), *(gagnant.ref_pid), *(gagnant.ref_numero)) ;
		
	}

	pid = fork() ; // Création du premier processus de la chaîne

	/* Création des processus restants et des tubes avant de transférer l'entier généré aléatoirement */

	for (indice_tube_courant = 1 ; indice_tube_courant < nb_processus ; indice_tube_courant++) {

		switch (pid) {

			/* Traitement des erreurs du fork */

			case -1 :
	
				fprintf(stderr, "Erreur de fork\n") ; exit(2) ;
			
			/* Réception des infos par le fils */
	
			case 0 :
			
				close(tubes[indice_tube_courant][ENTREE_TUBE]) ; // Fermeture du descripteur inutilisé
				read(tubes[indice_tube_courant][SORTIE_TUBE], &gagnant, sizeof gagnant) ; // Lecture des infos dans le tube
				printf("Le gagnant est : entier %ld, pid %d, numéro %d\n", *(gagnant.ref_entier), *(gagnant.ref_pid), *(gagnant.ref_numero)) ;
				close(tubes[indice_tube_courant][SORTIE_TUBE]) ; // Fermeture du descripteur devenu inutile
				pid = fork() ; // Génération du prochain fils
			
				break ;
			
			/* Envoi des infos par le père */
		
			default :
			
				wait(NULL) ; // Attente de la terminaison du fils
		
				if(pipe(tubes[indice_tube_courant]) == -1) {
					fprintf(stderr, "Erreur à la création de tube\n") ;
					free(tubes) ;
					exit(1);
				}
			
				close(tubes[indice_tube_courant][SORTIE_TUBE]) ; // Fermeture du descripteur inutilisé
				write(tubes[indice_tube_courant][ENTREE_TUBE], &gagnant, sizeof gagnant) ; // Ecriture des infos dans le tube
				close(tubes[indice_tube_courant][ENTREE_TUBE]) ; // Fermeture du descripteur devenu inutile
			
		}
	
	}

	return 0 ; 

}
