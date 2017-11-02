#ifndef __AFF_H__
#define __AFF_H__

#include <stdio.h>

typedef void (*aff_t)(void*, FILE*) ;

// Ecrit dans le flux f l'entier pointé par e castée en pointeur de int

void aff_i (void *e, FILE *f) ;

// Ecrit dans le flux f l'entier pointé par e castée en pointeur de unsigned (int)

void aff_u (void *e, FILE *f) ;

// Ecrit dans le flux f l'entier pointé par e castée en pointeur de long int

void aff_li (void *e, FILE *f) ;

// Ecrit dans le flux f la chaîne de caractères pointée par e converti en pointeur de chaîne de caractère

void aff_s (void *e, FILE *f) ;

#endif
