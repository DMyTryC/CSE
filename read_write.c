#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>


// DONNEES DU PROGRAMME

// threads_writers : Adresse vers les threads des writers
// threads_reader : Adresse vers les threads des readers
pthread_t *threads_writer;
pthread_t *threads_reader;

// Structure modelisant la situation pour la priorite des writers :
// mutex_variable : un mutex pour un acces protege, utilise pour changer le nombre des readers actuel
// cond_wakeup_writers : une condition pour pouvoir reveiller les writers en cas qu'ils peuvent ecrire
// cond_wakeup_readers : une condition pour pouvoir reveiller les readers en cas qu'ils peuvent lire
// nb_readers : le nombre des lecteurs actuels
// nb_potential_writers : variable pour bloquer la lecture
// current_writing : variable pour savoir si un writer est en train d'ecrire
typedef struct writer_reader_struct_priority_writer{
  pthread_mutex_t mutex_variable      = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t  cond_wakeup_writers = PTHREAD_COND_INITIALIZER;
  pthread_cond_t  cond_wakeup_readers = PTHREAD_COND_INITIALIZER;

  int nb_readers;
  int nb_potential_writers;
  int current_writing;

}writer_reader_struct_priority_writer;

// Structure modelisant la situation pour la priorite des readers :
// mutex_variable : un mutex pour un acces protege, utilise pour changer le nombre des readers actuel
// nb_readers : le nombre des lecteurs actuels
// sem_writers : un semaphore pour gerer les lecteurs prioritaires
typedef struct writer_reader_struct_priority_reader{
  pthread_mutex_t mutex_variable      = PTHREAD_MUTEX_INITIALIZER;
  int nb_readers;

  sem_t sem_writers;  
}writer_reader_struct_priority_reader;

// FONCTIONS DU PROGRAMME

// La lecture d'un fichier de donnees :
// On ouvre un fichier "database.txt" en mode lecture
// On lit le fichier en faisant des putchar(c) pour chaque caractere
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

// L'ecriture d'un fichier de donnees :
// On ouvre un fichier "database.txt" en mode ecriture
// On fprintf ce qu'on a lit dans le fichier
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

// Lecture avec priorite du reader :
// On incremente le nombre des lecteurs actuels
// On lit le fichier
// On decremente le nombre des lecteurs actuels 
//   en envoyant un signal aux writers si le nombre des lecteurs est egal a 0
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

// Ecriture avec priorite du reader :
// On attend 
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
