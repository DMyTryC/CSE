// PRE-PROCESSING

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>

// Constantes utiles pour l'utilisation des tubes

#define ST 0 // Sortie du tube
#define ET 1 // Entrée du tube
#define MAXBUF 65536 // Taille maximale du buffer

// PROGRAMME

int main(int argc, char **argv){

    pid_t pid ; // Pid du processus en cours d'exécution
    int tube[2] ; // Tube pour la communication père/fils
    int fd ; // Descripteur de fichier
    char *nom_fich = argv[1] ; // Référence vers le chemin du fichier
    char buf[MAXBUF] ; // Buffer

    // Arrêt du programme en cas d'absence d'arguments
    if(argc < 2) {
        fprintf(stderr, "usage : %s chemin_fich\n", *argv) ;
        exit(3) ;
    }

    // Création du tube et traitement des erreurs
    if(pipe(tube) == -1) {
        fprintf(stderr, "Erreur à la création de tube\n") ;
        exit(1);
    }

    pid = fork() ;

    switch (pid) {

        case -1 :
        fprintf(stderr, "Erreur de fork\n") ; exit(2) ;

        case 0 :
        close(tube[ET]) ;
        // Boucle de lecture/écriture bufferisée (du tube vers stdout)
        for(int n = 0 ; (n = read(tube[ST], buf, sizeof buf)) > 0 ;)
            write(STDOUT_FILENO, buf, n) ;
        close(tube[ST]) ;
        exit(0) ;

        default :
        // Ouverture en lecture du fichier et gestion des erreurs
        if ((fd = open(nom_fich, O_RDONLY, NULL)) == -1) {
            perror("open ") ;
            exit(2) ;
        }
        close(tube[ST]) ;
        // Boucle de lecture/écriture bufferisée (du fichier dans le tube)
        for(int n = 0 ; (n = read(fd, buf, sizeof buf)) > 0 ;)
            write(tube[ET], buf, n) ;
        close(tube[ET]) ;
        close(fd) ;
        wait(NULL) ;
    }

    return 0 ;

}
