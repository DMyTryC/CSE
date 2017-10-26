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

// Structure modelisant la situation pour la priorite des readers :
// mutex_variable : un mutex pour un acces protege, utilise pour changer le nombre des readers actuel
// nb_readers : le nombre des lecteurs actuels
// sem_writers : semaphore utilisé pour les lecteurs prioritaires
typedef struct struct_priority_reader{
  pthread_mutex_t mutex_variable;
  int nb_readers;

  sem_t sem_writers;

}struct_priority_reader;


// FONCTIONS DU PROGRAMME

// Intialisation de la structure pour la priorite reader :
// Initialisation du semaphore avec un seul token
// Initialisation du mutex
// Initialisation du nombre des readers actuels a 0
void init_priority_reader(struct_priority_reader *struct_priority)
{
  sem_init(&(struct_priority->sem_writers), 0, 1);
  pthread_mutex_init(&(struct_priority->mutex_variable),NULL);
  struct_priority->nb_readers=0;
}

// Fonction random
int hasard(int inf, int sup) {
   return inf +
   (int)((double)(sup - inf + 1) * rand() / ((double)RAND_MAX + 1));
}

// Bloquage du writer 
void writer_lock(struct_priority_reader *struct_priority)
{
  sem_wait(&(struct_priority->sem_writers));
}

// Ecriture du message pour writer
void write_msg()
{
  printf("%s\n","J'écris le fichier" );
  sleep(1);
}

// Debloquage du writer 
void writer_unlock(struct_priority_reader *struct_priority)
{
  sem_post(&(struct_priority->sem_writers));
}

// Bloquage du reader :
// Incrementation du nombre des readers
// Verification si c'est le premier reader
// Si c'est le cas, on prend le token
void reader_lock(struct_priority_reader *struct_priority)
{
  pthread_mutex_lock(&(struct_priority->mutex_variable));
  struct_priority->nb_readers++;
  if(struct_priority->nb_readers==1)
    sem_wait(&(struct_priority->sem_writers));

  pthread_mutex_unlock(&(struct_priority->mutex_variable));
}

// Ecriture du message pour reader
void read_msg()
{
  printf("%s\n","Je suis le lecteur" );
  sleep(1);
}

// Debloquage du reader :
// Decrementation du nombre des readers
// Verification si c'est le dernier reader
// Si c'est le cas, on depose le token
void reader_unlock(struct_priority_reader *struct_priority)
{
    pthread_mutex_lock(&(struct_priority->mutex_variable));
    struct_priority->nb_readers--;
    if(struct_priority->nb_readers==0)
      sem_post(&(struct_priority->sem_writers));
    pthread_mutex_unlock(&(struct_priority->mutex_variable));
}

// Le thread pour le reader
void* thread_reader(void* data)
{
  struct_priority_reader* struct_priority;
  struct_priority = (struct_priority_reader*) data;
  reader_lock(struct_priority);
  read_msg();
  reader_unlock(struct_priority);
  return NULL;
}

// Le thread pour le writer 
void* thread_writer(void* data)
{
  struct_priority_reader* struct_priority;
  struct_priority = (struct_priority_reader*) data;
  writer_lock(struct_priority);
  write_msg();
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

  nb_thread=(int)strtol(argv[1], (char **)NULL, 10);
  threads= malloc(nb_thread*sizeof(pthread_t));

  // Initialisation du semaphore de lecture avec nb_reader jetons
  struct_priority_reader* struct_reader_writer;
  struct_reader_writer = malloc(sizeof(struct_priority_reader));

  // Initialisation
  init_priority_reader(struct_reader_writer);

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
