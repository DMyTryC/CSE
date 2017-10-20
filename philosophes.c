#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <semaphore.h>

// DONNEES DU PROGRAMME

// Structure modélisant une baguette :
// Un booléen pour la disponibilité de la baguette
// Un mutex pour un accès protégé au booléen de la baguette

typedef struct {
    bool disponible ;
    pthread_mutex_t m ;
} baguette ;

// Structure modélisant un philosophe :
// Un numéro unique pour le distinguer
// Une référence vers sa baguette gauche
// Une référence vers sa baguette droite
// Le nombre de portions de riz qu'il a englouties actuellement

typedef struct {
    unsigned num ;
    baguette * bg ;
    baguette * bd ;
    unsigned riz ;
} philosophe ;

// Définition du type de la routine que l'on passera aux threads (= philosophes)
// On va en définir deux :
// La 1ère utilise une condition, la seconde utilise des sémaphores

typedef void (fonction_philosophe_t)(philosophe *p);

void fonction_philosophe_cond (philosophe *p) ;
void fonction_philosophe_sem (philosophe *p) ;

// Variables globales partagées en lecture par les threads

unsigned nb_philosophes ; // Nombre de philosophes prenant part au dîner
unsigned nb_portions ; // Nombre de portions qu'ils doivent manger
fonction_philosophe_t *fp ; // Adresse de la routine passée aux threads
philosophe* philosophes ; // Ensemble des philosophes prenant part au dîner
baguette* baguettes ; // Emsemble de baguettes disposées sur la table

// Artefacts utilisés pour l'accès exclusif aux baguettes

// Un mutex et une condition pour fonction_philosophe_cond
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER ;
pthread_cond_t c = PTHREAD_COND_INITIALIZER ;

// Un sémaphore pour fonction_philosophe_sem
sem_t semaphore ;

// FONCTIONS DU PROGRAMME

// Retourne un entier pseudo-aléatoire compris entre inf et sup

int hasard(int inf, int sup) {
   return inf +
   (int)((double)(sup - inf + 1) * rand() / ((double)RAND_MAX + 1));
}

// Initialisation des baguettes :
// Elles sont toutes disponibles dès le départ
// Initialisation du mutex associé

void init_baguettes (baguette* baguettes) {
    for (int i = 0 ; i < nb_philosophes ; i++) {
        baguettes[i].disponible = true ;
        pthread_mutex_init(&baguettes[i].m, NULL) ;
    }
}

// Initialisation des philosophes :
// Ils ont un numéro unique de 1 à nb_philosophes
// On leur associe deux baguettes placées de chaque côté du philosophe
// Et au départ, ils n'ont mangé aucune portion de leur riz

void init_philosophes (philosophe* pilosophes, baguette* baguettes) {
    for (int i = 0 ; i < nb_philosophes ; i++) {
        philosophes[i].num = i+1 ;
        philosophes[i].bg = &baguettes[i] ;
        philosophes[i].bd = &baguettes[(i+1)%nb_philosophes] ;
        philosophes[i].riz = 0 ;
    }
}

// Vérification de la disponibilité des baguettes d'un philosophe :
// On verrouille l'accès aux baguettes et si elles sont disponibles
// (= bg/bd -> disponible == true)
// Le philosophe s'en saisit et s'apprête à manger une portion de son riz
// (= bg/bd -> disponible == false)
// On déverrouille les accès

bool pret_a_manger (philosophe* p) {
    bool b ;
    pthread_mutex_lock(&p->bg->m) ;
    pthread_mutex_lock(&p->bd->m) ;
    if((b = p->bg->disponible && p->bd->disponible)) {
        p->bg->disponible = false ;
        p->bd->disponible = false ;
    }
    pthread_mutex_unlock(&p->bd->m) ;
    pthread_mutex_unlock(&p->bg->m) ;
    return b  ;
}

// Phase de dégustation du riz :
// Baguettes en main, le philosophe se goinfre (= sleep(1) et p->riz--)
// On verrouille l'accès aux baguettes pour préserver la cohérence des données
// Le philosophe ainsi repu (pour le moment) repose ses baguettes qui sont
// de nouveau disponibles (= bg/bd -> disponible == true)
// On déverrouille les accès

void manger (philosophe* p) {
    sleep(1) ;
    p->riz++ ;
    pthread_mutex_lock(&p->bg->m) ;
    pthread_mutex_lock(&p->bd->m) ;
    p->bg->disponible = true ;
    p->bd->disponible = true ;
    pthread_mutex_unlock(&p->bd->m) ;
    pthread_mutex_unlock(&p->bg->m) ;
}

// Phase de méditation :
// Ainsi le philosophe, le ventre plein, se perd dans ses pensées
// (A moins qu'il ne fasse une sieste, allez savoir !)
// (= sleep(t = hasard(0,3)) ;
// On retourne le temps (en seconde) pendant lequel le philosophe était inactif

int penser (void) {
    int t ;
    sleep(t = hasard(0,3)) ;
    return t ;
}

// Vérification de la fin durepas d'un philosophe :
// Son repas prend fin quand il a avalé toutes ses portions de riz
// (= p->riz == nb_portions)

bool fin_repas (philosophe* p) {
    return p->riz == nb_portions ;
}

// Routine du philosophe :
// C'est-à-dire manger puis penser (ou dormir) ou inversement
// (= Tant que non fin du repas faire routine)

void* fonction_philosophe (void *p_P) {
    philosophe* p = (philosophe*) p_P ;
    while (!fin_repas(p)) {
        fp(p) ;
        printf("Le philosophe %d a pensé pendant %d seconde(s)\n",
        p->num, penser()) ;
    }
    pthread_exit(0) ;
}

// Routine du philosophe utilisant une condition :
// Pour éviter l'interblocage, on utilise un mutex global qui fait office
// de moniteur. Pas d'interblocage ni de famine obtenu lors des tests a priori

void fonction_philosophe_cond (philosophe *p) {
    pthread_mutex_lock(&m) ;
    while (!pret_a_manger(p))
        pthread_cond_wait(&c, &m) ;
    manger(p) ;
    pthread_cond_signal(&c) ;
    pthread_mutex_unlock(&m);
    printf("Le philosophe %d a mangé %d fois\n", p->num, p->riz) ;
}

// Routine du philosophe utilisant un sémaphore :
// Pour éviter l'interblocage, Pas plus de nb_philosophes - 1 peuvent vérifier
// en même temps que leurs baguettes sont disponibles
// Pas d'interblocage ni de famine obtenu lors des tests a priori

void fonction_philosophe_sem (philosophe *p)  {
    if (pret_a_manger(p)) {
        sem_wait(&semaphore) ;
        manger(p) ;
        sem_post(&semaphore) ;
        printf("Le philosophe %d a mangé %d fois\n", p->num, p->riz) ;
    }
}

// PROGRAMME PRINCIPAL

int main (int argc, char **argv) {

    pthread_t* tids ; // Adresse du tableau des tids

    // Vérification du nombre d'arguments
    if (argc != 4) {
        fprintf(stderr, "Usage : %s nb_philosophes_>_1 nb_portions_>_0\
 \"cond\"_ou_\"sem\"\n", argv[0]) ;
        exit(1) ;
    }

    {
        // Vérification de la sémantique des arguments
        unsigned arg1 ; unsigned arg2 ;
        if  (!(arg1 = atoi(argv[1])) || !(arg2 = atoi(argv[2])) ||
            (strcmp(argv[3], "cond") && strcmp(argv[3], "sem"))) {
            fprintf(stderr, "Usage : %s nb_philosophes_>_1 nb_portions_>_0\
 \"cond\"_ou_\"sem\"\n", argv[0]) ;
            exit(1) ;
        }

        // Vérification qu'au moins deux philosophes ont été conviés au dîner
        if (arg1 <= 1) {
            fprintf(stderr, "Usage : %s nb_philosophes_>_1 nb_portions_>_0\
 \"cond\"_ou_\"sem\"\n", argv[0]) ;
            exit(1) ;
        }

        nb_philosophes = arg1 ;
        nb_portions = arg2 ;

        // Initialisation de la routine choisie et du sémaphore si c'est ce
        // dernier qui a été choisi

        if (strcmp(argv[3], "cond")) {
            fp = fonction_philosophe_sem ;
            sem_init(&semaphore, 0, nb_philosophes-1) ;
        }
        else
            fp = fonction_philosophe_cond ;
    }

    // Allocation mémoire des différentes structures de données
    tids = malloc (nb_philosophes*sizeof(pthread_t)) ;
    baguettes = malloc (nb_philosophes*sizeof *baguettes) ;
    philosophes = malloc (nb_philosophes*sizeof *philosophes) ;

    if (!tids || !baguettes || !philosophes) {
        fprintf(stderr, "Erreur : Mémoire insuffisante\n") ;
        exit(1) ;
    }

    // Initialisation des baguettes et des philosophes
    init_baguettes (baguettes) ;
    init_philosophes (philosophes, baguettes) ;
    srand(time(NULL)) ;

    // Début du dîner des philosophes
    for (int i = 0 ; i < nb_philosophes ; i++)
        pthread_create(&tids[i], NULL, fonction_philosophe, &philosophes[i]) ;

    // Fin du dîner des philosophes
    for (int i = 0 ; i < nb_philosophes ; i++)
        pthread_join(tids[i], NULL) ;

    // Libération de la mémoire
    free(tids) ;
    free(baguettes) ;
    free(philosophes) ;
    return EXIT_SUCCESS ;
}

