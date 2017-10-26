#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>


// DONNEES DU PROGRAMME

// threads : Adresse vers les threads (les writers et lecteurs)
pthread_t *threads;

// Structure modelisant la situation pour la priorite des writers :
// mutex_variable : un mutex pour un acces protege, utilise pour changer le nombre des readers actuel
// cond_wakeup_writers : une condition pour pouvoir reveiller les writers en cas qu'ils peuvent ecrire
// cond_wakeup_readers : une condition pour pouvoir reveiller les readers en cas qu'ils peuvent lire
// nb_readers : le nombre des lecteurs actuels
// nb_potential_writers : variable pour bloquer la lecture
// current_writing : variable pour savoir si un writer est en train d'ecrire
typedef struct struct_priority_writer{
  pthread_mutex_t mutex_variable;
  pthread_cond_t  cond_wakeup_writers;
  pthread_cond_t  cond_wakeup_readers;

  int nb_readers;
  int nb_potential_writers;
  int current_writing;

}struct_priority_writer;


// FONCTIONS DU PROGRAMME

// Intialisation de la structure pour la priorite reader :
// Initialisation des conditions et du mutex
// Initialisation du nombre des readers actuels a 0
// Initialisation de la variable pour bloquer les lecteurs
// Initialisation de la variable pour savoir si un writer est en train d'ecrire
void init_priority_writer(struct_priority_writer *struct_reader_writer)
{
  pthread_cond_init(&(struct_reader_writer->cond_wakeup_readers),NULL);
  pthread_cond_init(&(struct_reader_writer->cond_wakeup_writers),NULL);
  pthread_mutex_init(&(struct_reader_writer->mutex_variable),NULL);
  struct_reader_writer->nb_readers=0;
  struct_reader_writer->nb_potential_writers=0;
  struct_reader_writer->current_writing=0;
}

// Fonction random
int hasard(int inf, int sup) {
   return inf +
   (int)((double)(sup - inf + 1) * rand() / ((double)RAND_MAX + 1));
}

// Verrouillage du writer :
// Verification si le nombre des lecteurs est plus grand que 0 ou si quelqu'un est en train d'ecrire
// Si c'est le cas, on attend d'etre reveille
// Sinon, on va dire qu'on est en train d'ecrire
void writer_lock(struct_priority_writer *struct_priority)
{
  pthread_mutex_lock(&(struct_priority->mutex_variable));
  struct_priority->nb_potential_writers++;
  while(struct_priority->nb_readers>0||struct_priority->current_writing==1)
    pthread_cond_wait(&(struct_priority->cond_wakeup_writers),&(struct_priority->mutex_variable));
  struct_priority->current_writing=1;
  pthread_mutex_unlock(&(struct_priority->mutex_variable));
}

// Ecriture du message pour writer
void write_msg()
{
  printf("%s\n","Je suis l'écrivain." );
  sleep(1);
}

// Debloquage du writer :
// Decrementation du nombre de writers potentiels
// Si le nombre de writers potentiels est egal a 0
// On reveille tous les readers
// Sinon, on reveille un writer
// On met a jour l'ecriture courante
void writer_unlock(struct_priority_writer *struct_priority)
{
  pthread_mutex_lock(&(struct_priority->mutex_variable));
  struct_priority->nb_potential_writers--;
  if(struct_priority->nb_potential_writers==0)
    pthread_cond_broadcast(&(struct_priority->cond_wakeup_readers));
  else
    pthread_cond_signal(&(struct_priority->cond_wakeup_writers));
  struct_priority->current_writing=0;
  pthread_mutex_unlock(&(struct_priority->mutex_variable));

}

// Bloquage du reader :
// Si le nombre de writers actuels est > 0
// On attend le signal pour lire
// Sinon, on augmente le nombre de lecteurs
void reader_lock(struct_priority_writer *struct_priority)
{
  pthread_mutex_lock(&(struct_priority->mutex_variable));
  if(struct_priority->nb_potential_writers>0)
    pthread_cond_wait(&(struct_priority->cond_wakeup_readers),&(struct_priority->mutex_variable));
  struct_priority->nb_readers++;
  pthread_mutex_unlock(&(struct_priority->mutex_variable));
}

// Ecriture du message pour reader
void read_msg()
{
  printf("%s\n","Je suis le lecteur." );
  sleep(0.5);
}

// Debloquage du reader :
// On met a jour le nombre de readers
// Si le nombre de writers potentiels est egal a 0
// On reveille un writer
void reader_unlock(struct_priority_writer *struct_priority)
{
  pthread_mutex_lock(&(struct_priority->mutex_variable));
  struct_priority->nb_readers--;
  if(struct_priority->nb_potential_writers==0)
    pthread_cond_signal(&(struct_priority->cond_wakeup_writers));
  pthread_mutex_unlock(&(struct_priority->mutex_variable));
}

// Le thread pour le reader
void* thread_reader(void* data)
{
  struct_priority_writer* struct_priority;
  struct_priority = (struct_priority_writer*) data;
  reader_lock(struct_priority);
  read_msg(struct_priority);
  reader_unlock(struct_priority);

  return NULL;
}

// Le thread pour le writer
void* thread_writer(void* data)
{
  struct_priority_writer* struct_priority;
  struct_priority = (struct_priority_writer*) data;
  writer_lock(struct_priority);
  write_msg(struct_priority);
  writer_unlock(struct_priority);

  return NULL;
}


// PROGRAMME PRINCIPAL

int main(int argc, char **argv)
{

  srand(time(NULL)) ;
  int nb_thread = 0;
  int idx_thread= 0;

  // Verification du nombre d'arguments 
  if(argc<1)
  {
    printf("%s","Merci d'entrer le nombre de thread" );
    return 0;
  }

  struct_priority_writer *struct_reader_writer;
  struct_reader_writer= malloc(sizeof(struct_priority_writer));

  // Initialisation
  init_priority_writer( struct_reader_writer);

  nb_thread=(int)strtol(argv[1], (char **)NULL, 10);
  threads= malloc(nb_thread*sizeof(pthread_t));

  int nb_reader=0;
  int nb_writer=0;

  // Creation des threads
  while(idx_thread!=nb_thread)
  {
    if(hasard(0,1))
    {
      pthread_create(&threads[idx_thread],NULL,thread_reader,struct_reader_writer);
      nb_reader++;
    }
    else
    {
      pthread_create(&threads[idx_thread],NULL,thread_writer,struct_reader_writer);
      nb_writer++;
    }

    idx_thread++;
  }
  printf("%d%s\n",nb_reader," lecteurs créés." );
  printf("%d%s\n",nb_writer," ecrivains créés." );
  idx_thread = 0;

  // Fin de lecture
  while(idx_thread!=nb_thread)
  {
    pthread_join(threads[idx_thread],NULL);
    idx_thread++;
  }

  return 0;
}
