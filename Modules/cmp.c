#include "cmp.h"

void* sca (void* elt) { return elt ; }

// Echange les valeurs de a et b

void echangerValeurs (void *a, void *b, unsigned taille) {
	void *tmp = malloc (taille) ;
	memcpy(tmp, a, taille) ;
    memcpy(a, b, taille) ;
    memcpy(b, tmp, taille) ;
    free(tmp) ;
}

// Echange les adresses de a et b

void echangerAdresses (void** a, void** b) {
	void* tmp ;
    tmp =  *a ;
     *a =  *b ;
     *b = tmp ;
}

// Fonctions pour le type int

typedef bool (*cmp_i)(int,int) ;
static  bool inf_i (int a, int b) { return a < b ; }
static  bool inf_egal_i (int a, int b) { return a <= b ; }
static  bool egal_i (int a, int b) { return a == b ; }
static  bool sup_egal_i (int a, int b) { return a >= b ; }
static  bool sup_i (int a, int b) { return a > b ; }
static  bool diff_i (int a, int b) { return a != b ; }
cmp_i tabcmp_i[NBOP] = {inf_i, inf_egal_i, egal_i, sup_egal_i, sup_i, diff_i} ;

// Initialise un opérateur de comparaison de variables de type int

bool fcmp_i (oprel_t oprel, void* a, void* b) {
    return tabcmp_i[oprel](*(int*)a, *(int*)b) ;
}

// Fonctions pour le type unsigned (int)

typedef bool (*cmp_u)(unsigned,unsigned) ;
static  bool inf_u (unsigned a, unsigned b) { return a < b ; }
static  bool inf_egal_u (unsigned a, unsigned b) { return a <= b ; }
static  bool egal_u (unsigned a, unsigned b) { return a == b ; }
static  bool sup_egal_u (unsigned a, unsigned b) { return a >= b ; }
static  bool sup_u (unsigned a, unsigned b) { return a > b ; }
static  bool diff_u (unsigned a, unsigned b) { return a != b ; }
cmp_u tabcmp_u[NBOP] = {inf_u, inf_egal_u, egal_u, sup_egal_u, sup_u, diff_u} ;

// Initialise un opérateur de comparaison de variables de type unsigned (int)

bool fcmp_u (oprel_t oprel, void* a, void* b) {
    return tabcmp_u[oprel](*(unsigned*)a, *(unsigned*)b) ;
}

// Fonctions pour le type long int

typedef bool (*cmp_li)(long int,long int) ;
static  bool inf_li (long int a, long int b) { return a < b ; }
static  bool inf_egal_li (long int a, long int b) { return a <= b ; }
static  bool egal_li (long int a, long int b) { return a == b ; }
static  bool sup_egal_li (long int a, long int b) { return a >= b ; }
static  bool sup_li (long int a, long int b) { return a > b ; }
static  bool diff_li (long int a, long int b) { return a != b ; }
cmp_li tabcmp_li[NBOP] =
{inf_li, inf_egal_li, egal_li, sup_egal_li, sup_li, diff_li} ;

// Initialise un opérateur de comparaison de variables de type long int

bool fcmp_li (oprel_t oprel, void* a, void* b) {
    return tabcmp_li[oprel](*(long int*)a, *(long int*)b) ;
}

// Fonctions pour le type chaîne de caractères

typedef bool (*cmp_s)(char*,char*) ;
static  bool inf_s (char* a, char* b) { return strcmp(a,b) < 0 ; }
static  bool inf_egal_s (char* a, char* b) { return strcmp(a,b) <= 0 ; }
static  bool egal_s (char* a, char* b) { return !strcmp(a,b) ; }
static  bool sup_egal_s (char* a, char* b) { return strcmp(a,b) >= 0 ; }
static  bool sup_s (char* a, char* b) { return strcmp(a,b) > 0 ; }
static  bool diff_s (char* a, char* b) { return strcmp(a,b) ; }
cmp_s tabcmp_s[NBOP] = {inf_s, inf_egal_s, egal_s, sup_egal_s, sup_s, diff_s} ;

// Initialise un opérateur de comparaison de variables de type chaîne de caractères

bool fcmp_s (oprel_t oprel, void* a, void* b) {
    return tabcmp_s[oprel]((char*)a, (char*)b) ;
}
