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


typedef struct struct_priority_writer{
  pthread_mutex_t mutex_variable;
  pthread_cond_t  cond_wakeup_writers;
  pthread_cond_t  cond_wakeup_readers;

  int nb_readers;
  int nb_potential_writers;
  int current_writing;

}struct_priority_writer;

void init_priority_writer(struct_priority_writer *struct_reader_writer)
{
  pthread_cond_init(&(struct_reader_writer->cond_wakeup_readers),NULL);
  pthread_cond_init(&(struct_reader_writer->cond_wakeup_writers),NULL);
  pthread_mutex_init(&(struct_reader_writer->mutex_variable),NULL);
  struct_reader_writer->nb_readers=0;
  struct_reader_writer->nb_potential_writers=0;
  struct_reader_writer->current_writing=0;
}


void writer_lock(struct_priority_writer *struct_priority)
{
  //On verrouille pour accéder aux variables globales
  pthread_mutex_lock(struct_priority->mutex_variable);
  struct_priority->nb_potential_writers++;
  while(struct_priority->struct_reader_writer>0)
    pthread_cond_wait(struct_priority->cond_wakeup_writers)
  pthread_mutex_unlock(struct_priority->mutex_variable);
}

void write_msg()
{
  printf("%s\n","J'écris le fichier" );
  sleep(1);
}

void writer_unlock(struct_priority_writer *struct_priority)
{

}

void reader_lock(struct_priority_writer *struct_priority)
{

}

void read_msg()
{
  printf("%s\n","Je suis le lecteur" );
  sleep(1);
}

void reader_unlock(struct_priority_writer *struct_priority)
{

}

void* thread_reader(void* data)
{
  struct_priority_writer* struct_priority;
  struct_priority = (struct_priority_writer*) data;
  reader_lock(struct_priority);
  read_msg(struct_priority);
  reader_unlock(struct_priority);

  return NULL;
}


void* thread_writer(void* data)
{
  struct_priority_writer* struct_priority;
  struct_priority = (struct_priority_writer*) data;
  reader_lock(struct_priority);
  read_msg(struct_priority);
  reader_unlock(struct_priority);

  return NULL;
}



int main(int argc, char **argv)
{


  int nb_reader;
  int nb_writer;

  if(argc<2)
  {
    printf("%s","Merci d'entrer le nombre de thread lecteur puis le nombre" );
    printf("%s\n"," de thread écrivain" );
    return 0;
  }

  struct_priority_writer *struct_reader_writer;
  struct_reader_writer= malloc(sizeof(struct_reader_writer));

  init_priority_writer( struct_reader_writer);

  nb_reader=(int)strtol(argv[1], (char **)NULL, 10);
  nb_writer=(int)strtol(argv[2], (char **)NULL, 10);

  int idx_reader = 0;
  int idx_writer = 0;


  threads_reader = malloc(nb_reader*sizeof(pthread_t));
  threads_writer = malloc(nb_writer*sizeof(pthread_t));


  while(idx_reader!=nb_reader && idx_writer!=nb_writer )
  {
    if(idx_reader!=nb_reader)
    {
      pthread_create(&threads_reader[idx_reader],NULL,thread_reader,struct_reader_writer);
      idx_reader++;
    }
    if(idx_writer!=nb_writer)
    {
      pthread_create(&threads_writer[idx_writer],NULL,thread_writer,struct_reader_writer);
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
