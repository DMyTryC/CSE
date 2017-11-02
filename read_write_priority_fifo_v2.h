#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>

//Couleurs
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KNRM  "\x1B[0m"

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
  pthread_mutex_t mutex_distrib;
  pthread_cond_t  cond_wakeup_writers;
  pthread_cond_t  cond_wakeup_readers;

  int identifiant_global;
  int distrib_ticket;

  int nb_reader;
  int nb_thread;
  int nb_iterations;

}struct_priority_fifo;

// Structure pour l'information des threads :
// identifiant : un identifiant pour la position dans la liste fifo
typedef struct struct_info_thread{
  int identifiant;
  int id_ticket;
}struct_info_thread;

int   hasard(int inf, int sup);
void  dodo(int scale);
int   in_reader_block(int idx);

//With type 1 = reader and 0 = writer
//With struct_personal, the struct to initialize
void  func_distrib_tickets(int type, struct_info_thread* struct_personal);

void wakeup_last();

void* thread_writer(void* data);
void writer_lock(struct_info_thread *struct_personal);
void write_msg(struct_info_thread *struct_personal);
void writer_unlock(struct_info_thread *struct_personal);

void* thread_reader(void* data);
void reader_lock(struct_info_thread *struct_personal);
void read_msg(struct_info_thread *struct_personal);
void reader_unlock(struct_info_thread *struct_personal);

void init_priority_fifo(int nb_thread, int nb_iterations);
void destruct_priority_fifo();
