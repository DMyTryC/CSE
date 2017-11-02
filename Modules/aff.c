#include "aff.h"

// Ecrit dans le flux f l'entier pointé par e converti en pointeur de int

void aff_i (void *e, FILE *f) { fprintf(f, "%d", *(int*)e) ; }

// Ecrit dans le flux f l'entier pointé par e converti en pointeur de long int

void aff_li (void *e, FILE *f) { fprintf(f, "%ld", *(long int*)e) ; }

// Ecrit dans le flux f le caractère pointé par e converti en pointeur de char non signé

void aff_u (void *e, FILE *f) { fprintf(f, "%u", *(unsigned*)e) ; }

// Ecrit dans le flux f la chaîne de caractères pointée par e converti en pointeur de chaîne de caractère

void aff_s (void *e, FILE *f) { fprintf(f, "%s", (char *)e) ; }
