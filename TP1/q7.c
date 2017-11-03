// PRE-PROCESSING

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <limits.h>

// Constantes utiles pour l'utilisation des tubes

#define ST 0 // Sortie du tube
#define ET 1 // Entrée du tube
#define PROCMIN 2 // Nombre minimal de processus autorisé
#define PROCMAX 1000 // Nombre maximal de processus autorisé

// PROGRAMME

int main(int argc, char **argv){

    pid_t pid ; // Pid du processus en cours d'exécution
    int nb_proc = argc >= 2 ? atoi(argv[1]) : 0 ; // Nombre de processus
    long int entier = LONG_MIN , ent_alea ; // Entier qui sera transféré
    int num_proc_ent_max = 1 ; // Numéro du processus au plus grand entier
    pid_t pid_ent_max ; // Pid du processus qui a généré le plus grand entier
    pid_t pid_initial ; // Pid du processus initial

    // On ignore les signaux SIGPIPE afin de pouvoir traiter les erreurs
    // liées aux tubes
    signal(SIGPIPE, SIG_IGN) ;

    // Arrêt du programme en cas d'absence/non-conformité de l'argument
    if(nb_proc < PROCMIN || nb_proc > PROCMAX) {
        fprintf(stderr, "usage : %s nb_proc >= 2\n", *argv) ;
        exit(3) ;
    }

    // Tableau de tubes (pour former l'anneau, il faut n tubes pour n processus)
    int tubes[nb_proc][2] ;
    srandom(time(NULL)) ; // Initialisation de la graine
    long int r, w ; // variables pour stocker les résultats de read/write
    int itc = 0 ; // indice du tube courant dans la boucle
    // booléen qui va permettre de nullifier l'exécution du processus initial
    // dans la boucle
    int proc_init_exec_bloc = 1 ;

    // Création des tubes et traitement des erreurs
    for (; itc < nb_proc ; itc++)
        if(pipe(tubes[itc]) == -1) {
            fprintf(stderr, "Erreur à la création de tube\n") ;
            exit(1);
        }

    pid = pid_initial = fork() ;

    // Création des processus restants et transfert
    // du plus grand entier généré aléatoirement
    for (itc = 0 ; itc < nb_proc ; itc++) {
        // Génération pseudo-aléatoire de l'entier
        ent_alea = random () ;
        // On chosit le max entre l'ancien et le nouvel entier
        entier = entier < ent_alea ? ent_alea : entier ;
        // Mise à jour du numéro du processus qui a généré le lus grand entier
        num_proc_ent_max = entier == ent_alea ? itc + 1 : num_proc_ent_max ;
        // Mise à jour du pid qui a généré le plus grand entier
        pid_ent_max = entier == ent_alea ? getpid() : pid_ent_max ;

        switch (pid) {
    
            case -1 :
            fprintf(stderr, "Erreur de fork\n") ;
            exit(2) ;

            case 0 :            
            // Si le fils n'est pas le dernier processus, on lit l'entier
            if (itc < nb_proc - 1) {
                close(tubes[itc][ET]) ;
                r = read(tubes[itc][ST], &entier, sizeof entier) ;
                if (r == -1) perror("Read fils ") ;
                printf("Je suis le processus Fils n°%d de pid %d, entier reçu  \
   : %10ld, Bytes reçus  : %ld\n", itc + 1, getpid(), entier, r) ;
                close(tubes[itc][ST]) ;
                pid = fork() ;
            }
            else
                exit(0) ; // Sinon on le tue
            break ;
            
            default :
            // Cette condition oblige le processus initial maintenu en vie
            // à n'exécuter ce bloc d'instructions qu'une seule fois
            // pendant la boucle
            if (pid != pid_initial || proc_init_exec_bloc) {
                close(tubes[itc][ST]) ;
                w = write(tubes[itc][ET], &entier, sizeof entier) ;
                if (w == -1) perror("Write père ") ;
                printf("Je suis le processus Père n°%d de pid %d, entier généré\
   : %10ld, entier transféré : %ld, Bytes écrits : %ld\n",
                 itc, getpid(), ent_alea, entier, w) ;
                close(tubes[itc][ET]) ;
                wait(NULL) ;
                proc_init_exec_bloc = 0 ;
            }
            // Terminaison du père sauf le père initial
            if (pid != pid_initial)
                exit(0) ;
        }
    }

    // Réception de l'entier par le processus initial envoyé par le dernier fils
    if (pid == pid_initial) {
        close(tubes[nb_proc-1][ET]) ;
        r = read(tubes[nb_proc-1][ST], &entier, sizeof entier) ;
        if (r == -1) perror("Read père initial ") ;
        printf("Je suis le processus Init n°0 de pid %d, entier reçu   \
  : %10ld, Bytes reçus  : %ld\n", getpid(), entier, r) ;
        close(tubes[nb_proc-1][ST]) ;
        printf("Le processus n°%d de pid %d a généré le plus grand entier : \
%10ld\n", num_proc_ent_max, pid_ent_max, entier) ;
    }

    return 0 ;

}
