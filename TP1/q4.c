// PRE-PROCESSING

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

// Constantes utiles pour l'utilisation des tubes

#define ST 0 // Sortie du tube
#define ET 1 // Entrée du tube
#define NBT 2 // Nombre de tubes utilisés
#define PVF 0 // indice du tube Père Vers Fils
#define FVP 1 // indice du tube Père Vers Fils

// PROGRAMME

int main(int argc, char **argv){

    pid_t pid ; // Pid du processus en cours d'exécution
    int tube[NBT][2] ; // Tableau de tubes pour la communication père/fils
    char *message = argv[1] ; // Référence vers le message
    int lm = argc >= 2 ? strlen(message) + 1  : 0 ; // Longueur du message
    char message_recu[lm] ; // Tableau pour stocker le message reçu

    // Arrêt du programme en cas d'absence d'arguments
    if(!lm) {
        fprintf(stderr, "usage : %s message\n", *argv) ;
        exit(3) ;
    }

    // Création des tubes et traitement des erreurs
    for (int i = 0; i < NBT ; i++) {
        if(pipe(tube[i]) == -1) {
            fprintf(stderr, "Erreur à la création de tube\n") ;
            exit(1);
        }
    }

    pid = fork() ;

    switch (pid) {
        
        // Traitement des erreurs du fork
        case -1 :
        fprintf(stderr, "Erreur de fork\n") ; exit(2) ;
        
        // Réception et renvoi du message du père par le fils
        case 0 :
        close(tube[PVF][ET]) ; close(tube[FVP][ST]) ;
        read(tube[PVF][ST], message_recu, lm) ;
        printf("Le fils a reçu le message suivant  : %s \n", message) ;
        close(tube[PVF][ST]) ;
        write(tube[FVP][ET], message_recu, lm) ;
        printf("Message renvoyé :                    %s \n", message) ;
        close(tube[FVP][ET]) ;
        exit(0) ;
        break ;
        
        // Envoi du message du père au fils
        default :
        close(tube[PVF][ST]) ; close(tube[FVP][ET]) ;
        write(tube[PVF][ET], message, lm) ;
        printf("Message envoyé :                     %s \n", message) ;
        read(tube[FVP][ST], message, lm) ;
        printf("Le père a reçu le message suivant :  %s \n", message) ;
        close(tube[PVF][ET]) ; close(tube[FVP][ST]) ;
        wait(NULL) ;
    }

    return 0 ;
}
