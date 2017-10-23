#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>






pthread_t *threads;


typedef struct struct_priority_reader{
  pthread_mutex_t mutex_variable;
  int nb_readers;

  //Semaphore utilisé pour les lecteurs prioritaires
  sem_t sem_writers;

}struct_priority_reader;

int hasard(int inf, int sup) {
   return inf +
   (int)((double)(sup - inf + 1) * rand() / ((double)RAND_MAX + 1));
}



void init_priority_reader(struct_priority_reader *struct_priority)
{
  sem_init(&(struct_priority->sem_writers), 0, 1);
  pthread_mutex_init(&(struct_priority->mutex_variable),NULL);
  struct_priority->nb_readers=0;
}



void writer_lock(struct_priority_reader *struct_priority)
{
  sem_wait(&(struct_priority->sem_writers));
}

void write_msg()
{
  printf("%s\n","J'écris le fichier" );
  sleep(1);
}

void writer_unlock(struct_priority_reader *struct_priority)
{
  sem_post(&(struct_priority->sem_writers));
}

void reader_lock(struct_priority_reader *struct_priority)
{
  pthread_mutex_lock(&(struct_priority->mutex_variable));
  struct_priority->nb_readers++;
  if(struct_priority->nb_readers==1)
    sem_wait(&(struct_priority->sem_writers));

  pthread_mutex_unlock(&(struct_priority->mutex_variable));
}

void read_msg()
{
  printf("%s\n","Je suis le lecteur" );
  sleep(1);
}

void reader_unlock(struct_priority_reader *struct_priority)
{
    pthread_mutex_lock(&(struct_priority->mutex_variable));
    struct_priority->nb_readers--;
      if(struct_priority->nb_readers==0)
        sem_post(&(struct_priority->sem_writers));
    pthread_mutex_unlock(&(struct_priority->mutex_variable));
}


void* thread_reader(void* data)
{
  struct_priority_reader* struct_priority;
  struct_priority = (struct_priority_reader*) data;
  reader_lock(struct_priority);
  read_msg();
  reader_unlock(struct_priority);
  return NULL;
}


void* thread_writer(void* data)
{
  struct_priority_reader* struct_priority;
  struct_priority = (struct_priority_reader*) data;
  writer_lock(struct_priority);
  write_msg();
  writer_unlock(struct_priority);
  return NULL;
}





int main(int argc, char **argv)
{
  srand(time(NULL)) ;
  int nb_thread = 0;
  int idx_thread= 0;


  if(argc<1)
  {
    printf("%s","Merci d'entrer le nombre de thread" );
    return 0;
  }


  nb_thread=(int)strtol(argv[1], (char **)NULL, 10);

  threads= malloc(nb_thread*sizeof(pthread_t));

  //Initialisation de notre semaphore de lecture avec nb_reader jetons

  struct_priority_reader* struct_reader_writer;
  struct_reader_writer = malloc(sizeof(struct_priority_reader));

  init_priority_reader(struct_reader_writer);


  int nb_reader=0;
  int nb_writer=0;

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

  while(idx_thread!=nb_thread)
  {
    pthread_join(threads[idx_thread],NULL);

    idx_thread++;
  }

  return 0;
}
