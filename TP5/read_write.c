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


typedef struct writer_reader_struct_priority_writer{
  pthread_mutex_t mutex_variable      = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t  cond_wakeup_writers = PTHREAD_COND_INITIALIZER;
  pthread_cond_t  cond_wakeup_readers = PTHREAD_COND_INITIALIZER;

  int nb_readers;
  int nb_potential_writers;
  int current_writing;

}writer_reader_struct_priority_writer;


typedef struct writer_reader_struct_priority_reader{
  pthread_mutex_t mutex_variable      = PTHREAD_MUTEX_INITIALIZER;
  int nb_readers;

  //Semaphore utilisé pour les lecteurs prioritaires
  sem_t sem_writers;

}writer_reader_struct_priority_reader;


void read_data()
{
  printf("%s\n","DEBUT DE LECTURE" );
  int c;
  FILE *database_read;
  database_read = fopen("database.txt", "r");
  sleep(1);
  if (database_read)
  {
    while ((c = getc(database_read)) != EOF)
        putchar(c);
    fclose(database_read);
    printf("%s\n","FIN DE LECTURE" );
  }
}




void write_data(void* data)
{
  char *data_char;
  data_char = (char*)data;
  FILE *database_write;
  database_write = fopen("database.txt", "a+");
  fseek(database_write, 0, SEEK_END);
  fprintf(database_write,"\n%s\n",data_char);
  fclose(database_write);
}

void* thread_read_priority_reader()
{
  pthread_mutex_lock(&mutex_variable);
  nb_current_reader++;
  if(nb_current_reader==1)
    sem_wait(&sem_writers);

  pthread_mutex_unlock(&mutex_variable);

  read_data();
  sleep(1);

  pthread_mutex_lock(&mutex_variable);
  nb_current_reader--;
    if(nb_current_reader==0)
      sem_post(&sem_writers);
  pthread_mutex_unlock(&mutex_variable);

  return NULL;
}


void* thread_write_priority_reader(void* data)
{
  sem_wait(&sem_writers);
  write_data(data);
  sleep(1);
  sem_post(&sem_writers);

  return NULL;
}

void* thread_read_priority_writer()
{

}

void* thread_write_priority_writer()
{

}




int main(int argc, char **argv)
{


  FILE *erase = fopen("database.txt", "w");
  fprintf(erase,"\n%s\n","Debut de fichier");
  fclose(erase);


  int nb_reader;
  int nb_writer;

  if(argc<3)
  {
    printf("%s","Merci d'entrer le nombre de thread lecteur puis le nombre" );
    printf("%s\n"," de thread écrivain puis : \n * 0 pour priorité lecteur" );
    printf("%s\n"," * 1 pour priorité ecrivains " );
    printf("%s\n"," * 2 pour FIFO " );
    return 0;
  }



  nb_reader=(int)strtol(argv[1], (char **)NULL, 10);
  nb_writer=(int)strtol(argv[2], (char **)NULL, 10);
  int choice =(int)strtol(argv[3], (char **)NULL, 10);

  int idx_reader = 0;
  int idx_writer = 0;


  threads_reader = malloc(nb_reader*sizeof(pthread_t));
  threads_writer = malloc(nb_writer*sizeof(pthread_t));

  //Initialisation de notre semaphore de lecture avec nb_reader jetons

  switch (choice) {
    case 0:
      init_priority_reader();
      break;
    case 1:
      init_priority_writer();
      break;
    case 2:
      init_priority_fifo();
      break;
    default:
      printf("%s\n","WTF man?" );
      return 0;
  }

  sem_init(&sem_readers, 0, nb_reader);

  //Initialisation de notre semaphore d'écriture avec 1 jeton
  sem_init(&sem_writers, 0, 1);

  while(idx_reader!=nb_reader && idx_writer!=nb_writer )
  {
    if(idx_reader!=nb_reader)
    {
      pthread_create(&threads_reader[idx_reader],NULL,thread_read_priority_reader,NULL);
      idx_reader++;
    }
    if(idx_writer!=nb_writer)
    {
      char* message;
      message = "Ecriture du thread ";
      char convert_idx[5];
      sprintf(convert_idx, "%d", idx_writer);
      char * message_final = (char *) malloc(1 + strlen(message)+ strlen(convert_idx) );
      strcpy(message_final, message);
      strcat(message_final, convert_idx);
      pthread_create(&threads_writer[idx_writer],NULL,thread_write_priority_reader,message_final);
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

void init_priority_writer()
{

}

void init_priority_reader()
{

}

void init_priority_fifo()
{

}
