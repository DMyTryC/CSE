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

// Structure pour l'information des threads :
// identifiant : un identifiant pour la position dans la liste fifo
typedef struct struct_info_thread{
  int identifiant;
}struct_info_thread;

// Structure modelisant le cas de first-in-first-out :
// mutex_variable : un mutex pour un acces protege, utilise pour changer le nombre des readers actuel
// cond_wakeup_writers : une condition pour pouvoir reveiller les writers en cas qu'ils peuvent ecrire
// cond_wakeup_readers : une condition pour pouvoir reveiller les readers en cas qu'ils peuvent lire
// identifiant_global : un identifiant pour la position dans la liste fifo
// nb_reader : le nombre des lecteurs actuels 
// last_reader : le reader precedent dans la liste fifo
// info_thread : une structure pour l'information de chaque thread
typedef struct struct_priority_fifo{
  pthread_mutex_t mutex_variable;
  pthread_cond_t  cond_wakeup_writers;
  pthread_cond_t  cond_wakeup_readers;

  int identifiant_global;
  int nb_reader;
  int last_reader;
  struct_info_thread * info_thread;

}struct_priority_fifo;

// Une pointeur vers une structure fifo
struct_priority_fifo *struct_priority;


// FONCTIONS DU PROGRAMME

// Intialisation de la structure pour fifo
// Allocation un espace memoire pour la structure
// Initialisation des conditions et du mutex
// Initialisation du dernier reader a 100
// Initialisation du nombre des readers actuels a 0
// Initialisation de la position globale dans la liste a 0
void init_priority_fifo()
{
  struct_priority = malloc(sizeof(struct_priority_fifo));
  pthread_cond_init(&(struct_priority->cond_wakeup_readers),NULL);
  pthread_cond_init(&(struct_priority->cond_wakeup_writers),NULL);
  pthread_mutex_init(&(struct_priority->mutex_variable),NULL);
  struct_priority->last_reader=100;
  struct_priority->nb_reader = 0;
  struct_priority->identifiant_global=0;
}

// Fonction random
int hasard(int inf, int sup) {
   return inf +
   (int)((double)(sup - inf + 1) * rand() / ((double)RAND_MAX + 1));
}

//Fonction permettant de savoir si un thread écrivain peut lire si une lecture est déjà
//En cours afin de permettre un accès parallèle au fichier
/*int keep_reading(int id_global, int idx)
{
  printf("%s%d%s%d","Keep Reading ",idx, "avec i =",id_global );
  if(id_global==idx)
    return 0;
  while(id_global!=idx)
  {
    if(array_of_role_per_thread[id_global])
    {
        printf("%s%d\n"," Retour : ",0);
        return 0;
    }
    id_global++;
  }
  printf("%s%d\n"," Retour : ",1);
  return 1;
}
DEPRECATED
*/

// Bloquage du writer pour attendre son tour ou que le nombre des lecteurs actuels est egal a 0 :
// Verification qu'une des deux conditions soit correcte
// Dans ce cas on reveille tous les readers
// Et on attend d'etre reveille par un reader pour reverifier les conditions
void writer_lock(struct_info_thread *struct_personal)
{
  pthread_mutex_lock(&(struct_priority->mutex_variable));
  int id = struct_personal->identifiant;
  while(id!=struct_priority->identifiant_global||struct_priority->nb_reader>0)
  {
    pthread_cond_broadcast(&(struct_priority->cond_wakeup_readers));
    pthread_cond_wait(&(struct_priority->cond_wakeup_readers),
    &(struct_priority->mutex_variable));
  }
  pthread_mutex_unlock(&(struct_priority->mutex_variable));
}

// Ecriture du message pour un writer
void write_msg(struct_info_thread *struct_personal)
{
  printf("%s%d\n", "Je suis ecrivain mon identifiant est: ",struct_personal->identifiant);
  sleep(1);
}

// Avancement de la liste pour un writer :
// Incrementation de la position dans la liste
// On reveille tous les readers
void writer_unlock(struct_info_thread *struct_personal)
{
  pthread_mutex_lock(&(struct_priority->mutex_variable));
  struct_priority->identifiant_global++;
  pthread_mutex_unlock(&(struct_priority->mutex_variable));
  pthread_cond_broadcast(&(struct_priority->cond_wakeup_readers));
}

// Bloquage du reader pour attendre son tour :
// Verification que la condition soit vraie et dans ce cas
// On attend d'etre reveille par un writer et on re-teste
// En cas contraire, on incremente le nombre des readers et on incremente la position
void reader_lock(struct_info_thread *struct_personal)
{
  pthread_mutex_lock(&(struct_priority->mutex_variable));
  int id = struct_personal->identifiant;

  while(struct_priority->identifiant_global!=id)
    {
        pthread_cond_wait(&(struct_priority->cond_wakeup_readers),
        &(struct_priority->mutex_variable));
    }

  struct_priority->nb_reader ++;
  struct_priority->identifiant_global++;
  pthread_mutex_unlock(&(struct_priority->mutex_variable));
}

// Ecriture du message par un reader
void read_msg(struct_info_thread *struct_personal)
{
    printf("%s%d\n", "Je suis lecteur mon identifiant est: ",struct_personal->identifiant);
    sleep(1);
}

// Avancement de la liste pour un reader :
// Changement du dernier lecteur pour dire qu'on etait le dernier
// Decrementation du nombre des lecteurs actuels
void reader_unlock(struct_info_thread *struct_personal)
{
  pthread_mutex_lock(&(struct_priority->mutex_variable));
  struct_priority->last_reader =  struct_personal->identifiant;
  struct_priority->nb_reader --;
  pthread_mutex_unlock(&(struct_priority->mutex_variable));
  pthread_cond_broadcast(&(struct_priority->cond_wakeup_readers));
}

// Le fonctionnement d'un thread reader :
// Bloquage d'un reader pour attendre son tour
// Ecriture du message quand c'est son tour 
// Avancement de la liste d'attente
void* thread_reader(void* data)
{
  struct_info_thread* struct_personal;
  struct_personal = (struct_info_thread*) data;
  reader_lock(struct_personal);
  read_msg(struct_personal);
  reader_unlock(struct_personal);

  return NULL;
}

// Le fonctionnement d'un thread writer :
// Bloquage d'un writer pour attendre son tour
// Ecriture du message quand c'est son tour 
// Avancement de la liste d'attente
void* thread_writer(void* data)
{
  struct_info_thread* struct_personal;
  struct_personal = (struct_info_thread*) data;
  writer_lock(struct_personal);
  write_msg(struct_personal);
  writer_unlock(struct_personal);

  return NULL;
}


// PROGRAMME PRINCIPALE

int main(int argc, char **argv)
{

  srand(time(NULL)) ; // Intialisation de la seed pour les randoms
  int nb_thread = 0; 
  int idx_thread= 0;

  // Verification du nombre d'arguments 
  if(argc<1)
  {
    printf("%s","Merci d'entrer le nombre de thread" );
    return 0;
  }

  // Initialisation
  init_priority_fifo();

  nb_thread=(int)strtol(argv[1], (char  **)NULL, 10);
  printf("%d\n",nb_thread );
  threads = malloc(nb_thread*sizeof(pthread_t));

  // Creation des threads
  while(idx_thread!=nb_thread)
  {
    struct_info_thread *info_thread=malloc(sizeof(struct_info_thread));
    if(hasard(0,1))
    {
      info_thread->identifiant=idx_thread;
      pthread_create(&threads[idx_thread],NULL,thread_reader,info_thread);
      printf("%s%d\n", "[INIT]Je suis lecteur mon identifiant est: ",(idx_thread));
    }
    else
    {
      info_thread->identifiant=idx_thread;
      pthread_create(&threads[idx_thread],NULL,thread_writer,info_thread);
      printf("%s%d\n", "[INIT]Je suis ecrivain mon identifiant est: ",(idx_thread));
    }
    idx_thread++;
  }

  idx_thread = 0;

  // Fin de lecture
  while(idx_thread!=nb_thread)
  {
      pthread_join(threads[idx_thread],NULL);
      idx_thread++;
  }

  return 0;
}
