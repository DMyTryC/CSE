#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#define NB_PHILO 10
#define NB_PORTION 10

typedef struct {
    bool disponible ;
    pthread_mutex_t m ;
} baguette ;

typedef struct {
    int num ;
    baguette * bg ;
    baguette * bd ;
    int riz ;   
} philosophe ;

philosophe philosophes[NB_PHILO] ;
baguette baguettes[NB_PHILO] ;
pthread_mutex_t m_global = PTHREAD_MUTEX_INITIALIZER ;
pthread_cond_t c_global = PTHREAD_COND_INITIALIZER ;

// Retourne un entier pseudo-aléatoire compris entre inf et sup

int hasard(int inf, int sup) {
   return inf +
   (int)((double)(sup - inf + 1) * rand() / ((double)RAND_MAX + 1));
}

void init_baguettes (baguette baguettes[NB_PHILO]) {
    for (int i = 0 ; i < NB_PHILO ; i++) {
        baguettes[i].disponible = true ;
        pthread_mutex_init(&baguettes[i].m, NULL) ;
    }
}

void init_philosophes (philosophe pilosophes[NB_PHILO], baguette baguettes[NB_PHILO]) {
    for (int i = 0 ; i < NB_PHILO ; i++) {
        philosophes[i].num = i+1 ;
        philosophes[i].bg = &baguettes[i] ;
        philosophes[i].bd = &baguettes[(i+1)%NB_PHILO] ;
        philosophes[i].riz = 0 ;
    }
}

void detruire_baguettes (baguette baguettes[NB_PHILO]) {
    for (int i = 0 ; i < NB_PHILO ; i++)
        pthread_mutex_destroy(&baguettes[i].m) ;
        
    pthread_mutex_destroy(&m_global) ;
    pthread_cond_destroy(&c_global) ;
}

bool pret_a_manger (philosophe p) {
    bool b ;
    pthread_mutex_lock(&p.bg->m) ;
    pthread_mutex_lock(&p.bd->m) ;
    b = p.bg->disponible && p.bd->disponible ;
    pthread_mutex_unlock(&p.bd->m) ;
    pthread_mutex_unlock(&p.bg->m) ;
    return b ;
}

void manger (philosophe* p) {
    pthread_mutex_lock(&p->bg->m) ;
    pthread_mutex_lock(&p->bd->m) ;
    p->bg->disponible = false ;
    p->bd->disponible = false ;
    pthread_mutex_unlock(&p->bd->m) ;
    pthread_mutex_unlock(&p->bg->m) ;
    sleep(1) ;
    p->riz++ ;
    pthread_mutex_lock(&p->bd->m) ;
    pthread_mutex_lock(&p->bg->m) ;
    p->bg->disponible = true ;
    p->bd->disponible = true ;
    pthread_mutex_unlock(&p->bd->m) ;
    pthread_mutex_unlock(&p->bg->m) ;
}

int penser (void) {
    int t ;
    sleep(t = hasard(0,3)) ;
    return t ;
}

bool fin_repas (philosophe p) {   
    return p.riz == NB_PORTION ;
}
    

void* fonction_philosophe (void *p_P) {
    
    philosophe p = *(philosophe*) p_P ;
    
    while (!fin_repas(p)) {
        pthread_mutex_lock(&m_global);
        while (!pret_a_manger(p))
            pthread_cond_wait(&c_global, &m_global) ;
   
        manger(&p) ;
        printf("Le philosophe %d a mangé %d fois\n", p.num, p.riz) ;
        pthread_cond_signal(&c_global) ;
        pthread_mutex_unlock(&m_global);
        printf("Le philosophe %d a pensé pendant %d seconde(s)\n",
        p.num, penser()) ;
    }
    
    
    pthread_exit(0) ;
}

int main (int argc, char **argv) {

    unsigned nb_threads = NB_PHILO ;    
        
    pthread_t tids[NB_PHILO] ;
    init_baguettes (baguettes) ;
    init_philosophes (philosophes, baguettes) ;
    srand(time(NULL)) ;
    
    for (int i = 0 ; i < nb_threads ; i++)
        pthread_create(&tids[i], NULL, fonction_philosophe, &philosophes[i]) ;
         
    for (int i = 0 ; i < nb_threads ; i++)
        pthread_join(tids[i], NULL) ;
    
    return EXIT_SUCCESS ;   
}

