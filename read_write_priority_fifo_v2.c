#include "read_write_priority_fifo_v2.h"


pthread_t *threads;

/*
Variable globale à la totalité de nos threads, permettant d'accéder
aux mutex etc (voir .h)
*/
struct_priority_fifo *struct_priority;

//Tableau d'entier permettant de savoir le rôle de chaque ticket
int *array_of_role_per_ticket;


void init_priority_fifo(int nb_thread, int nb_iterations)
{
  struct_priority = malloc(sizeof(struct_priority_fifo));
  pthread_cond_init(&(struct_priority->cond_wakeup_readers),NULL);
  pthread_cond_init(&(struct_priority->cond_wakeup_writers),NULL);
  pthread_mutex_init(&(struct_priority->mutex_variable),NULL);
  pthread_mutex_init(&(struct_priority->mutex_distrib),NULL);

  struct_priority->identifiant_global=0;
  struct_priority->distrib_ticket=0;

  struct_priority->nb_reader = 0;
  struct_priority->nb_thread = nb_thread;
  struct_priority->nb_iterations = nb_iterations;
}

void destruct_priority_fifo()
{
  free(threads);
  free(array_of_role_per_ticket);
  pthread_mutex_destroy(&(struct_priority->mutex_variable));
  pthread_mutex_destroy(&(struct_priority->mutex_distrib));
  pthread_cond_destroy(&(struct_priority->cond_wakeup_readers));
  pthread_cond_destroy(&(struct_priority->cond_wakeup_writers));
  free(struct_priority);
}

// FONCTIONS UTILITAIRES //

//Fonction renvoyant 1 ou 0
int hasard(int inf, int sup) {
   return inf +
   (int)((double)(sup - inf + 1) * rand() / ((double)RAND_MAX + 1));
}

//Sleep. Source: test_lecteurs_redacteurs.c
void dodo(int scale) {
    usleep((random()%1000000)*scale);
}

/*
Fonction permettant de savoir si un thread ayant le ticket idx se trouve
Dans le bloc de lecteur courant, si c'est le cas, on renvoi 1, sinon, 0
*/
int in_reader_block(int id_ticket)
{
  int id_global = struct_priority->identifiant_global;
  while(id_global!=id_ticket)
  {
    if(!array_of_role_per_ticket[id_global])
        return 0;
    id_global++;
  }
  return 1;
}

void func_distrib_tickets(int type, struct_info_thread* struct_personal)
{
  pthread_mutex_lock(&(struct_priority->mutex_distrib));
  int ticket = struct_priority->distrib_ticket;
  struct_priority->distrib_ticket++;
  struct_personal->id_ticket=ticket;
  array_of_role_per_ticket[ticket]=type;
  if(!type)
    printf("%d%s%d%s\n", struct_personal->identifiant,KRED"?"KNRM,ticket," ");
  else
    printf("%d%s%d%s\n", struct_personal->identifiant,KGRN"?"KNRM,ticket," ");
  pthread_mutex_unlock(&(struct_priority->mutex_distrib));
}


//Fonction utilisée pour réveiller le dernier thread
void wakeup_last()
{
  /*
  Si le dernier thread est un écrivain, et que le thread avant lui est aussi
  Un écrivain il faudra provoquer cette fonction pour réveiller ce dernier écrivain,
  sinon il ne sera jamais réveillé et attendra éternellement
  */

  /*
  De même dans le cas où le dernier thread est un écrivain, et que le thread
  ayant acquéri le verrou avant lui est un lecteur. L'écrivain ne sera aussi
  jamais réveillé et attendra éternellement.
  */
  if(struct_priority->identifiant_global+1==
    (struct_priority->nb_thread*struct_priority->nb_iterations))
    pthread_cond_signal(&(struct_priority->cond_wakeup_readers));
}

//FONCTIONS EN LIEN AVEC ECRIVAIN//

void* thread_writer(void* data)
{
  struct_info_thread* struct_personal;
  struct_personal = (struct_info_thread*) data;
  for (int i = 0; i < struct_priority->nb_iterations; i++)
  {
    func_distrib_tickets(0,struct_personal);
    writer_lock(struct_personal);
    write_msg(struct_personal);
    writer_unlock(struct_personal);
  }
  free(data);
  return NULL;
}


void writer_lock(struct_info_thread *struct_personal)
{
  //Acquisition du verrou
  pthread_mutex_lock(&(struct_priority->mutex_variable));
  int id = struct_personal->id_ticket;
  /*
  Tant que le ticket appelé ne correspond pas à notre ticket
  Ou qu'une lecture est en cours
  */
  while(id!=struct_priority->identifiant_global||struct_priority->nb_reader>0)
  {
    /*
    Si l'on se trouve dans cette boucle, cela veut dire qu'une lecture est en
    cours ou que ce n'est pas encore mon tour.
    Je réveil tous mes tickets en attente
    */

    pthread_cond_broadcast(&(struct_priority->cond_wakeup_readers));

    //Puis j'attend que l'on me réveil
    pthread_cond_wait(&(struct_priority->cond_wakeup_readers),
    &(struct_priority->mutex_variable));
  }
  pthread_mutex_unlock(&(struct_priority->mutex_variable));
}

// Action d'ecriture effectuée par un écrivain
void write_msg(struct_info_thread *struct_personal)
{
  printf("%s%d%s%d%s\n", KRED,struct_personal->identifiant, "-",
  struct_personal->id_ticket,KNRM " ");
  sleep(1);
}


void writer_unlock(struct_info_thread *struct_personal)
{
  pthread_mutex_lock(&(struct_priority->mutex_variable));
  /*
  Incrémentation du ticket fait lors de la libération du verrou car
  l'écriture est exclusive entre les lecteurs et les écrivains
  */
  struct_priority->identifiant_global++;

  wakeup_last();
  pthread_mutex_unlock(&(struct_priority->mutex_variable));
}

//FIN DES FONCTIONS EN LIEN AVEC ECRIVAIN//



//FONCTIONS EN LIEN AVEC LECTEURS//

//Fonction threadé pour nos lecteurs
void* thread_reader(void* data)
{
  //Récupération de la structure liée à chaque lecteur
  struct_info_thread* struct_personal;
  struct_personal = (struct_info_thread*) data;
  for (int i = 0; i < struct_priority->nb_iterations; i++)
  {
    //Fonction donnant un ticket à chaque passage
    //Premier argument 1 ou 0, 1 pour lecteur, 0 pour écrivain
    func_distrib_tickets(1,struct_personal);

    reader_lock(struct_personal);
    read_msg(struct_personal);
    reader_unlock(struct_personal);
  }
  free(data);
  return NULL;
}


//Fonction de lock pour nos lecteurs
void reader_lock(struct_info_thread *struct_personal)
{
  //Acquisition du mutex
  pthread_mutex_lock(&(struct_priority->mutex_variable));
  int id = struct_personal->id_ticket;

  /*
  Tant que nous ne sommes pas le ticket appelé, ou que l'on est pas dans un
  bloc de lecteur..
  */
  while(!in_reader_block(id)||struct_priority->identifiant_global!=id)
  {
      /*
      Réveil de tous nos tickets en attente.
      Broadcast car on ne sait pas si le ticket ayant le numéro du ticket
      appelé est le premier à attendre cette condition.
      */
      pthread_cond_broadcast(&(struct_priority->cond_wakeup_readers));

      //Puis attente que l'on nous réveille
      pthread_cond_wait(&(struct_priority->cond_wakeup_readers),
      &(struct_priority->mutex_variable));
  }

  //Si c'est notre tour, incrémentation du nombre de lecteur courant
  struct_priority->nb_reader ++;
  /*
  Incrémentation du ticket appelé lors de l'acquisition de notre verrou
  Car l'on veut autoriser la possibilité que les lecteurs d'un même bloc lisent
  en même temps.
  */
  struct_priority->identifiant_global++;

  wakeup_last();
  pthread_mutex_unlock(&(struct_priority->mutex_variable));
}

// Action de lecture par un lecteur
void read_msg(struct_info_thread *struct_personal)
{
    printf("%s%d%s%d%s\n", KGRN,struct_personal->identifiant,
    "*", struct_personal->id_ticket,KNRM " ");
    sleep(1);
}


void reader_unlock(struct_info_thread *struct_personal)
{
  //Acquisition du verrou
  pthread_mutex_lock(&(struct_priority->mutex_variable));

  //Décrémentation du nombre de reader courant
  struct_priority->nb_reader --;


  wakeup_last();
  pthread_mutex_unlock(&(struct_priority->mutex_variable));
}

//FIN DES FONCTIONS EN LIEN AVEC LECTEURS//


// PROGRAMME PRINCIPAL

int main(int argc, char **argv)
{

  // Intialisation de la seed pour les randoms
  srand(time(NULL)) ;

  int nb_thread,idx_thread,nb_lecteurs,nb_redacteurs,nb_iterations;

  // Verification du nombre d'arguments
  if (argc < 4) {
      fprintf(stderr, "Utilisation: %s nb_lecteurs nb_redacteurs "
                      "nb_iterations\n", argv[0]);
      exit(1);
  }

  printf("%s\n","Symbole : " );
  printf(KRED "-" KNRM );
  printf("%s\n","  : Ecriture" );
  printf(KGRN "*" KNRM );
  printf("%s\n","  : Lecture" );
  printf("%s\n\n","?  : Prise de ticket" );
  printf("%s\n","Notations: " );
  printf(KRED "Ecriture -> [Identifiant Thread Ecrivain]-[Identifiant Ticket]\n");
  printf(KGRN "Lecture  -> [Identifiant Thread Lecteur]*[Identifiant Ticket]\n");
  printf(KNRM"Prise Ticket Lecteur  -> [Identifiant Thread Lecteur]"KGRN"?"KNRM"[Identifiant Ticket]\n");
  printf("Prise Ticket Ecrivain -> [Identifiant Thread Ecrivain]"KRED"?"KNRM"[Identifiant Ticket]\n\n");

  nb_lecteurs=   atoi(argv[1]);
  nb_redacteurs= atoi(argv[2]);
  nb_iterations= atoi(argv[3]);
  nb_thread = nb_lecteurs + nb_redacteurs;

  init_priority_fifo(nb_thread,nb_iterations);

  threads = malloc(nb_thread*sizeof(pthread_t));
  array_of_role_per_ticket = malloc((nb_thread*nb_iterations)*sizeof(int));

  // Creation des threads
  for (idx_thread = 0; idx_thread < nb_lecteurs; idx_thread++) {
    struct_info_thread *info_thread=malloc(sizeof(struct_info_thread));
    info_thread->identifiant=idx_thread;
    pthread_create(&threads[idx_thread],NULL,thread_reader,info_thread);
  }
  for (int idx = idx_thread; idx < (nb_redacteurs+idx_thread); idx++) {
    struct_info_thread *info_thread=malloc(sizeof(struct_info_thread));
    info_thread->identifiant=idx;
    pthread_create(&threads[idx],NULL,thread_writer,info_thread);
  }

  for (idx_thread = 0; idx_thread < nb_thread; idx_thread++)
    pthread_join(threads[idx_thread],NULL);

  destruct_priority_fifo();

  return 0;
}
