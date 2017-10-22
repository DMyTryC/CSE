#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h> 

#define NBE_MAX 10000
#define LIGNE   1024

typedef struct vecteur {
    int  deb  ; // Indice du début de la recherche
    int  fin  ; // Indice de fin   de la recherche
} vecteur ;

static int* tab  ; // Adresse du vecteur d'éléments
static pthread_t* tids ; // Adresse du tableau des tids 
static int  val  ; // Valeur recherchée
static unsigned int nb_threads = 0 ; // Nombre de threads
static int indice_min = NBE_MAX ; // ^^^^^^^
static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;; // ^^^^^^^
//static int val_trouvee = 0 ;

void vider_buffer (void) {
    int c ;
    while ((c = getchar()) != '\n' && c != EOF) ;
}

int hasard(int inf, int sup) {
   return inf +
   (int)((double)(sup - inf + 1) * rand() / ((double)RAND_MAX + 1));
}

int lire_donnees (int** tab, int nbe) {
    
    char  ligne[LIGNE] ;
    char* token ;
    int i = 0 ;
    *tab = malloc(NBE_MAX * sizeof(int)) ;
    
    if (!*tab)
        return i ;
        
    if (nbe) {
        srand(time(NULL)) ;
        for (int i = 0 ; i < nbe ; i++)
            (*tab)[i] = hasard(0, NBE_MAX/2) ;
        i = nbe ;
    }
            
    else
        while (i < NBE_MAX && fgets(ligne, LIGNE-1, stdin)) {
            token = strtok(ligne, " \n") ;
            while (i < NBE_MAX && token) {
                int e = atoi(token) ;
                if (e || token[0] == '0') {
                    (*tab)[i++] = e ;
                }
                token = strtok(NULL, " \n") ;
            }
        }
    
    printf("[") ;
    if (i)
        printf("%d", (*tab)[0]) ;
    for (int j = 1 ; j < i ; j++) printf(", %d", (*tab)[j]) ;
    printf("]\n") ;
    
    return i ;
}

void *search (void *arg){
    vecteur   vect = *(vecteur *) arg ;
    int       i ;
    //pthread_t tid ;
    //tid = pthread_self () ;
    int *ret = malloc(sizeof *ret) ;

    for (i = vect.deb ; i <= vect.fin ; i++){
        pthread_mutex_lock(&m);
        if (i < indice_min){
            if (tab[i] == val){
                printf("indice = %d\n", i) ;
                indice_min = i; // On met a jour l'indice de la val min
                *ret = indice_min ;
                pthread_mutex_unlock(&m);
                pthread_exit(ret); // On a trouve la valeur, une prochaine recherche est inutile
            }
        }
        else 
            pthread_exit(0); // L'indice est trop grand, donc, c'est inutile de chercher, on a trouve un indice plus petit
        pthread_mutex_unlock(&m);
        /*if (!val_trouvee && tab[i] == val) {
            printf("indice = %d\n", i) ;
            val_trouvee = 1 ;
            *ret = i ;
            pthread_exit(ret) ;
        }
        else if (val_trouvee)
            pthread_exit(0) ;*/

             
    }
    
    *ret = -1 ;
    pthread_exit(ret) ; 

}

int main (int argc, char **argv){

    int i, nbe ;
    if (argc < 3) {
        fprintf(stderr, "Usage : %s valeur_recherchée nb_threads\n", argv[0]) ;
        exit(1) ;
    }
    
    {
        int arg1 ; unsigned int arg2 ;
        if ((!(arg1 = atoi(argv[1])) && argv[1][0] != '0') || 
            (!(arg2 = atoi(argv[2])))) {
            fprintf(stderr, "Usage : %s valeur_recherchée nb_threads\n", argv[0]) ;
            exit(1) ;
        }
    
        val = arg1 ;
        nb_threads = arg2 ;
        nbe = argc > 3 ? atoi(argv[3]) : 0 ;
        if (nbe > NBE_MAX) {
            fprintf(stderr, "Le nombre d'entiers est limité à %d\n", NBE_MAX) ;
            exit(1) ;
        } 
        
    }
    
    if (!(i = lire_donnees(&tab, nbe))) {
        fprintf(stderr, "Aucune donnée lue\n") ;
        if (tab)
            free(tab) ;
        exit(2) ;
    }
    
    printf("val = %d\n", val) ;
    printf("nb_threads = %u\n", nb_threads) ;
    
    tids = malloc (nb_threads*sizeof(pthread_t)) ;
    vecteur* vect = malloc (nb_threads*sizeof(vecteur)) ;
    
    int j = 0 ;
    int deb = 0 ;
    int l = i/nb_threads ;
    int fin = i/nb_threads ;
    
    for (; j < nb_threads - 1 ; j++) {
         vect[j].deb = deb ;
         vect[j].fin = fin ;
         deb = fin + 1;
         fin = deb + l - 1 ;
         pthread_create(&tids[j], NULL, search, &vect[j]) ;
    }
    
    vect[j].deb = deb ;
    vect[j].fin = i-1 ;
    pthread_create(&tids[j], NULL, search, &vect[j]) ;
    
    int * res ;

    // Wait until every thread end
    for (j = 0; j < nb_threads; j++){
        pthread_join (tids[j], (void**)&res) ;
        if (res != 0) {
            if (*res != -1)
                printf("Le thread %lu a trouvé le valeur recherchée %d à\
 l'indice %d\n", tids[j], val, *(int*)res) ;
            else
                printf("Le thread %lu a terminé sa recherche sans avoir\
 trouvé la valeur %d\n", tids[j], val) ;
        }
    free (res) ;
    }

    free (tab)  ;
    free (tids) ;
    free (vect) ;

    return EXIT_SUCCESS;
}
