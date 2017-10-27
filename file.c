#include "file.h"

// Retourne une file vide

file file_vide (void) {
	
	file f = malloc(sizeof *f) ;
	
	if (f) {
		
		f->t = malloc (sizeof *(f->t)) ;
		f->q = malloc (sizeof *(f->q)) ;
		
		if (f->t && f->q) {
		
			*(f->t) = liste_vide() ;
			*(f->q) = liste_vide() ;
			f->l = 0 ;
		
		}
		
		else {
			
			if (f->t) free (f->t) ;
			if (f->q) free (f->q) ;
			free (f) ; f = NULL ;
			
		}
		
	}
	
	return f ;
	
}

// Retourne le pointeur de l'adresse de la tête de la file f

TETE T (file f) {

	return f->t ;
	
}

// Retourne le pointeur de l'adresse de la queue de la file f

QUEUE Q (file f) {

	return f->q ;
	
}

// Retourne la longueur de la file

unsigned int longueur_file (file f) {
	
	return f->l ;
	
}

// Retourne vrai ssi la file f ne contient aucun élément

int est_file_vide (file f) {
	
	return est_liste_vide(*T(f)) ;
	
}

// Retourne l'élément de tête de la file f

void * tete (file f) {
	
	return acceder(*T(f)) ;
	
}

// Défile la file f puis retourne l'adresse de son nouvel élément de tête

Tete defiler (file f) {

	Tete t = liste_vide() ;

	t = supprimer (T(f)) ;
	
	if (! est_liste_vide(t)) f->l-- ;
	
	return t ;
	
}

// Retourne vrai ssi l'élément e de longueur taille a été enfilé avec succès en queue de la file f

bool enfiler (file f, void *e, unsigned long int taille) {
	
	bool ok = false ;
	
	ok = ajouter_en_queue (T(f), e, taille, Q(f)) ;
	
	if (ok) f->l++ ;
	
	return ok ;
	
}

// Affiche les éléments e de la file f dans le flux fich selon la fonction d'affichage a avec ou sans saut de ligne

void aff_file (file f, FILE *fich, elt e, aff a, bool sdl) {

	aff_liste (*T(f), fich, e, a, sdl) ;
		
}

// Libère l'espace mémoire occupée par la file f et son constructeur

void detruire_file (file f) {

	if (f) {
	
		detruire_liste(T(f)) ;
		free(T(f)) ;
		free(Q(f)) ;
		free(f) ;
		
	}
		
}

/*

int main (void) {
	
	#include "hasard.h"

	file f = file_vide() ;
	int val ; bool ok = true ;
	
	aff_file(f, stdout, aff_scalaire, aff_i, true) ;
	
	for (int i = 0 ; i < 10 && ok ; i++) {
		
		val = hasard(0,9) ;
		enfiler(f, &val, sizeof val) ;
		aff_file(f, stdout, aff_scalaire, aff_i, true) ;
		
	}	
	
	do {
		
		printf("\nlongueur = %d\n", longueur_file(f)) ;
		printf("\ntete = %d\n", *(int *)tete(f)) ;
		aff_file(f, stdout, aff_scalaire, aff_i, true) ;
		
	} while(defiler(f)) ;
	
	aff_file(f, stdout, aff_scalaire, aff_i, true) ;
	
	detruire_file(f) ;
	
	return 0 ;

}

*/
	 
