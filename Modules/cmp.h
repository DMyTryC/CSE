#ifndef __CMP_H__
#define __CMP_H__

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define NBOP 6

typedef enum {INF, INFEGAL, EGAL, SUPEGAL, SUP, DIFF} oprel_t ;
typedef void* (*cle_t)(void*) ;
typedef bool  (*cmp_t)(unsigned, void*, void*) ;

// Une variable scalaire est sa propre clé
// La fonction sca retourne la clé d'un scalaire, c-à-d lui-même

void* sca (void *e) ;

// Echange les valeurs de a et b de "taille" octets

void echangerValeurs (void* a, void* b, unsigned taille) ;

// Echange les adresses de a et b

void echangerAdresses (void** a, void** b) ;

// Renvoie le résultat du prédicat a oprel b avec a et b des entiers de int

bool fcmp_i (oprel_t oprel, void* a, void* b) ;

// Renvoie le résultat du prédicat a oprel b avec a et b des entiers de type unsigned (int)

bool fcmp_u (oprel_t oprel, void* a, void* b) ;

// Renvoie le résultat du prédicat a oprel b avec a et b des entiers de type long int

bool fcmp_li (oprel_t oprel, void* a, void* b) ;

// Renvoie le résultat du prédicat a oprel b avec a et b des chaînes de caractères

bool fcmp_s (oprel_t oprel, void* a, void* b) ;

#endif
