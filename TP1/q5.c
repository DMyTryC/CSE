// PRE-PROCESSING

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>

/* Constantes utiles pour l'utilisation des tubes */

#define SORTIE_TUBE 0
#define ENTREE_TUBE 1

// PROGRAMME

int main(int argc, char **argv){

	pid_t pid ; // Pid du processus en cours d'exécution
	int tube[2] ; // Tube pour la communication père/fils
	int fd ; // Descripteur de fichier
	char *nom_fich = argv[1] ; // Référence vers le chemin du fichier passé en 1er argument sur la ligne de commande
	char buf[65536] ; // Buffer
	
	/* Arrêt du programme en cas d'absence d'arguments */
	
	if(argc < 2) {
		
		fprintf(stderr, "usage : %s chemin_fich\n", *argv) ;
		exit(3) ;
		
	}
	
	/* Création du tube et traitement des erreurs */
	
	if(pipe(tube) == -1) {
		
		fprintf(stderr, "Erreur à la création de tube\n") ;
		exit(1);
	
	}

	pid = fork() ; // Création d'un nouveau processus

	switch (pid) {

		/* Traitement des erreurs du fork */
		
		case -1 :
			
			fprintf(stderr, "Erreur de fork\n") ; exit(2) ;
			
		/* Chargement en mémoire du fichier transféré depuis le père par le fils */
	
		case 0 :
			
			close(tube[ENTREE_TUBE]) ; // Fermeture du descripteur inutilisé
			
			/* Boucle de lecture (du tube) / écriture (vers la sortie standard) bufferisée des données transférées depuis le père par le fils */
			
			for(int n = 0 ; (n = read(tube[SORTIE_TUBE], buf, sizeof buf)) > 0 ;)
				
				write(STDOUT_FILENO, buf, n) ;
				
			close(tube[SORTIE_TUBE]) ; // Fermeture du descripteur devenu inutile
			
			exit(0) ;
			
		/* Transfert des données du fichier au fils par le père */
	
		default :
		
			/* Ouverture en lecture du fichier et gestion des erreurs */
			
			if ((fd = open(nom_fich, O_RDONLY, NULL)) == -1) {
			
				 perror("open ") ;
				 exit(2) ;
			
			}
			
			close(tube[SORTIE_TUBE]) ; // Fermeture du descripteur inutilisé
			
			/* Boucle de lecture (du fichier) / écriture (dans le tube) bufferisée des données transférées depuis le père par le fils */
			
			for(int n = 0 ; (n = read(fd, buf, sizeof buf)) > 0 ;)
				
				write(tube[ENTREE_TUBE], buf, n) ;
			
			close(tube[ENTREE_TUBE]) ; // Fermeture du descripteur devenu inutile
			close(fd) ; // Fermeture du descripteur du fichier de données
	
	}
	
	return 0 ; 

}
