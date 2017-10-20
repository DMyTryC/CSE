#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>






pthread_t *threads_writer;
pthread_t *threads_reader;


typedef struct struct_priority_reader{
  pthread_mutex_t mutex_variable;
  int nb_readers;

  //Semaphore utilisé pour les lecteurs prioritaires
  sem_t sem_writers;

}struct_priority_reader;



void init_priority_reader(struct_priority_reader *struct_priority)
{
  sem_init(&(struct_priority->sem_writers), 0, 1);
  struct_priority->mutex_variable=PTHREAD_MUTEX_INITIALIZER;
  struct_priority->nb_readers=0;
}



void writer_lock(struct_priority_reader *struct_priority)
{
  sem_wait(struct_priority->sem_writers);
}

void write_msg()
{
  printf("%s\n","J'écris le fichier" );
  sleep(1);
}

void writer_unlock(struct_priority_reader *struct_priority)
{
  sem_post(struct_priority->sem_writers);
}

void reader_lock(struct_priority_reader *struct_priority)
{
  pthread_mutex_lock(struct_priority->mutex_variable);
  struct_priority->nb_readers++;
  if(struct_priority->nb_readers==1)
    sem_wait(struct_priority->sem_writers);

  pthread_mutex_unlock(struct_priority->mutex_variable);
}

void read_msg()
{
  printf("%s\n","Je suis le lecteur" );
  sleep(1);
}

void reader_unlock(struct_priority_reader *struct_priority)
{
    pthread_mutex_lock(struct_priority->mutex_variable);
    struct_priority->nb_readers--;
      if(struct_priority->nb_readers==0)
        sem_post(struct_priority->sem_writers);
    pthread_mutex_unlock(struct_priority->mutex_variable);
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
  FILE *erase = fopen("database.txt", "w");
  fprintf(erase,"\n%s\n","Debut de fichier");
  fclose(erase);


  int nb_reader;
  int nb_writer;

  if(argc<2)
  {
    printf("%s","Merci d'entrer le nombre de thread lecteur puis le nombre" );
    printf("%s\n"," de thread écrivain" );
    return 0;
  }



  nb_reader=(int)strtol(argv[1], (char **)NULL, 10);
  nb_writer=(int)strtol(argv[2], (char **)NULL, 10);

  int idx_reader = 0;
  int idx_writer = 0;


  threads_reader = malloc(nb_reader*sizeof(pthread_t));
  threads_writer = malloc(nb_writer*sizeof(pthread_t));

  //Initialisation de notre semaphore de lecture avec nb_reader jetons

  struct_priority_reader* struct_reader_writer;
  struct_reader_writer = malloc(sizeof(struct_priority_reader));

  init_priority_reader(struct_reader_writer);


  while(idx_reader!=nb_reader && idx_writer!=nb_writer )
  {
    if(idx_reader!=nb_reader)
    {
      pthread_create(&threads_reader[idx_reader],NULL,thread_read_priority_reader,NULL);
      idx_reader++;
    }
    if(idx_writer!=nb_writer)
    {
      pthread_create(&threads_writer[idx_writer],NULL,thread_write_priority_reader,struct_reader_writer);
      idx_writer++;
    }
  }

  idx_reader = 0;
  idx_writer = 0;

  while(idx_reader!=nb_reader && idx_writer!=nb_writer)
  {
    if(idx_reader!=nb_reader)
    {
      pthread_join(threads_reader[idx_reader],NULL);
      idx_reader++;
    }

    if(idx_writer!=nb_writer)
    {
      pthread_join(threads_writer[idx_writer],NULL);
      idx_writer++;
    }
  }

  return 0;
}
