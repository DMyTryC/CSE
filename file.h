#ifndef __FILE_H__
#define __FILE_H__

typedef struct file {

	unsigned l ;
  unsigned
  void* array ;

} *file ;

// Retourne une file vide

file file_vide (void) ;

// Retourne la longueur de la file

unsigned int longueur_file (file f) ;

// Retourne vrai ssi la file f ne contient aucun élément

int est_file_vide (file f) ;

// Retourne l'élément de tête de la file f

void * tete (file f) ;

// Défile la file f puis retourne l'adresse de son nouvel élément de tête

Tete defiler (file f) ;

// Retourne vrai ssi l'élément e de longueur taille a été enfilé avec succès en queue de la file f

bool enfiler (file f, void *e, unsigned long int taille) ;

// Affiche les éléments e de la file f dans le flux fich selon la fonction d'affichage a avec ou sans saut de ligne

void aff_file (file f, FILE *fich, elt e, aff a, bool sdl) ;

// Libère l'espace mémoire occupée par la file f et son constructeur

void detruire_file (file f) ;

#endif
