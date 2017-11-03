// PRE-PROCESSING

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

// Constantes utiles pour l'utilisation des tubes

#define ST 0 // Sortie du tube
#define ET 1 // Entrée du tube
#define PROCMIN 2 // Nombre minimal de processus autorisé
#define PROCMAX 1000 // Nombre maximal de processus autorisé

// PROGRAMME

int main(int argc, char **argv){

    pid_t pid ; // Pid du processus en cours d'exécution
    int nb_proc = argc >= 2 ? atoi(argv[1]) : 0 ; // nombre de processus
    int entier ; // Entier qui sera transféré de processus en processus
    
    // On ignore les signaux SIGPIPE afin de pouvoir traiter les erreurs
    // liées aux tubes
    signal(SIGPIPE, SIG_IGN) ;

    // Arrêt du programme en cas d'absence/non-conformité de l'argument
    if(nb_proc < PROCMIN || nb_proc > PROCMAX) {
        fprintf(stderr, "usage : %s nb_proc >= 2\n", *argv) ;
        exit(3) ;
    }

    // Tableau de tubes (on a besoin de n-1 tubes pour n processus)
    int tubes[nb_proc-1][2] ;
    // Génération pseudo-aléatoire d'un entier
    srandom(time(NULL));
    entier = random() ;
    int itc = 0 ; // indice du tube courant dans la boucle

    // Création du premier tube
    if(pipe(tubes[itc]) == -1) {
        fprintf(stderr, "Erreur à la création de tube\n") ;
        exit(1);
    }

    pid = fork() ;

    // Création des processus et tubes restants avant de transférer l'entier
    for ( ; itc < nb_proc - 1; itc++) {
        if(itc + 1 < nb_proc && pipe(tubes[itc + 1]) == -1) {
            fprintf(stderr, "Erreur à la création de tube\n") ;
            exit(1);
        }
        switch (pid) {

            // variables pour stocker les résultats des fonctions read/write
            long int r, w ;

            case -1 :
            fprintf(stderr, "Erreur de fork\n") ;
            exit(2) ;

            case 0 :
            close(tubes[itc][ET]) ;
            r = read(tubes[itc][ ST], &entier, sizeof entier) ;
            if (r == -1) perror("Read fils ") ;
            printf("Je suis le processus Fils n°%d de pid %d : entier reçu\
      : %10d, Bytes reçus  : %ld\n", itc + 1, getpid(), entier, r) ;
            close(tubes[itc][ ST]) ;
            pid = fork() ;
            break ;

            default :
            close(tubes[itc][ ST]) ;
            w = write(tubes[itc][ET], &entier, sizeof entier) ;
            if (w == -1) perror("Write père ") ;
            printf("Je suis le processus Père n°%d de pid %d : entier transféré\
 : %10d, Bytes écrits : %ld\n", itc, getpid(), entier, w) ;
            close(tubes[itc][ET]) ;
            wait(NULL) ;
            exit(0) ;

        }
    }

    return 0 ;

}
