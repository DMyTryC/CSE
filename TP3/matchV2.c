#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct chant {
    int nb_chant ;
    char* paroles ;  
} chant ;

void *supporter (void *arg){
  chant     chant_S = *(chant *) arg ;
  int       i ;
  int       pid ;
  pthread_t tid ;  
  pid = getpid () ;
  tid = pthread_self () ;
  srand ((int) tid) ;

  for (i = 0; i < chant_S.nb_chant; i++){
    printf ("Processus %d Thread %x : %s \n",
    pid, (unsigned int) tid, chant_S.paroles) ;
    usleep (rand() / RAND_MAX * 1000000.) ;
  }
  return (void *) tid ;
}

int main (int argc, char **argv){

  int teamFrance ;
  int teamEngland ;
  int i ;
  int nb_threads = 0 ;
  pthread_t *tids ;

  if (argc != 5){
    fprintf(stderr, "usage : %s teamFrance nb_chant_teamFrance\
 teamEngland nb_chant_teamEngland\n", argv[0]) ;
    exit (-1) ;
  }
  
  chant
  francais = {atoi(argv[2]), "Allons enfants de la patrie..."},
  anglais = {atoi(argv[4]), "Swing low, sweet chariot..."} ;

  teamFrance = atoi (argv[1]) ;
  teamEngland  = atoi (argv[3]) ;
  nb_threads = teamFrance + teamEngland ; 
  tids = malloc (nb_threads*sizeof(pthread_t)) ;

  /* Create the threads for teamFrance */
  for (i = 0 ; i < teamFrance; i++){
    pthread_create (&tids[i], NULL, supporter, &francais) ;
  }
  /* Create the other threads (ie. teamEngland) */
  for ( ; i < nb_threads; i++){
    pthread_create (&tids[i], NULL, supporter, &anglais) ;
  }  

  /* Wait until every thread ened */ 
  for (i = 0; i < nb_threads; i++){
    pthread_join (tids[i], NULL) ;
  }
  
  free (tids) ;
  return EXIT_SUCCESS;
}
