#include "pile.h"

/******************************************************************************/
/*                          DESCRIPTION DU MODULE                             */
/******************************************************************************/
/*                                                                            */
/*                           Manipulation de piles                            */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                PARADIGMES                                  */
/******************************************************************************/
/*                                                                            */
/*       Les paradigmes de programmation utilisés sont les suivants           */ 
/*                                                                            */
/******************************************************************************/

// Orienté Objet : + Facilite l'encapsulation des données
//                 + Réduit le nombre d'arguments des fonctions
//                 = Sécurité renforcée car masquage des attributs
//                 = Interface + simple et intuitive pour l'utilisateur 

// Fonctionnel   : + Toutes les fonctions ont une valeur de retour
//                   (Evite l'utilisation d'un paramètre entrée/résultat)
//                 + Passage des paramètres par valeur
//                   (L'utilisateur ne fera jamais de passage par adresses)
//                 = Réduit les risques de bogue pour l'utilisateur car
//                   les modifications des variables sont explicites
//                   (encapsulées dans la valeur de retour)
//                 = Plus de confort pour le programmeur car le passage des
//                   paramètres par valeur les rendent plus simple à utiliser
// Contrats :      = Pré-Condition et Post-Condition toujours spécifiées
//                 = La vérification des arguments est à la charge
//                   de l'utilisateur car le comportement des fonctions est
//                   indéterminé quand les arguments ne respectent pas la Pré-C
//                 = Simplifie la tâche du programmeur

/******************************************************************************/

/******************************************************************************/
/*                          DEFINITION DE LA CLASSE                           */
/******************************************************************************/

typedef struct pile_t {
    unsigned taille ;     // longueur maximale de la pile
    unsigned longueur ;   // longueur courante de la pile
    unsigned elt ;        // taille en octets d'un élément de la pile
       void* array ;      // adresse du tableau d'éléments
       aff_t aff ;        // fonction d'affichage d'un élément 
} *pile_t ;

/******************************************************************************/

/******************************************************************************/
/*                               IMPLANTATION                                 */
/******************************************************************************/
/*                                                                            */
/*        Définitions des fonctions que l'utilisateur peut utiliser           */ 
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                               CONSTRUCTEURS                                */
/******************************************************************************/

// PILE_VIDE

// taille : nombre d'éléments maximal que peut contenir la pile
//    elt : taille en octets d'un élément de la pile (défini le type)
//    aff : fonction d'affichage d'un élément
// Renvoie un pointeur sur une nouvelle pile initialisée et vide
// ou
// Renvoie un pointeur NULL si la création a échoué

pile_t pile_vide (unsigned taille, unsigned elt, aff_t aff) {
    pile_t p = malloc(sizeof *p) ;
    if (p) {
        p->array = malloc (elt * taille) ;
        if (p->array) {
            p->taille = taille ;
            p->elt = elt ;
            p->longueur = 0 ;
            p->aff = aff ;
        }
        else {
            free (p) ;
            p = NULL ;
        }
    }
    return p ;
}

// Pré-Condition :
// L'utilisateur ne peut stocker que des éléments de même type

// Post-Condition :
// Une pile vide de taille "taille" * "elt" octets a été créée en mémoire
// s'il existe assez de mémoire pour contenir la pile

/******************************************************************************/

// NOUVELLE_PILE_VIDE

// p : pile initialisée
// Renvoie un pointeur sur une nouvelle pile initialisée et vide
// ou
// Renvoie un pointeur NULL si la création a échoué

pile_t nouvelle_pile_vide (pile_t p) {
    pile_t np = malloc(sizeof *p) ;
    if (np) {
        np->array = malloc (p->elt * p->taille) ;
        if (np->array) {
            np->taille = p->taille ;
            np->elt = p->elt ;
            np->longueur = 0 ;
            np->aff = p->aff ;
        }
        else {
            free (np) ;
            np = NULL ;
        }
    }
    return np ;
}

// Pré-Condition :
// f doit avoir été initialisée
// L'utilisateur ne peut stocker que des éléments de même type

// Post-Condition :
// Comportement indéterminé si p n'a pas été initialisée
// Une pile vide a été créée en mémoire de même type que p
// s'il existe assez de mémoire pour l'initialiser

/******************************************************************************/
/*                               ACCESSEURS                                   */
/******************************************************************************/

// LONGUEUR_PILE

// p : pile initialisée
// Renvoie le nombre d'éléments courant que contient la pile

unsigned longueur_pile (pile_t p) {
    return p->longueur  ;  
}

// Pré-Condition :
// La pile p doit avoir été initialisée

// Post-Condition :
// Comportement indéterminé si p n'a pas été initialisée

/******************************************************************************/

// EST_PILE_VIDE

// p : pile initialisée
// Renvoie vrai ssi la pile p ne contient aucun élément actuellement

bool est_pile_vide (pile_t p) {
    return !p->longueur ;  
}

// Pré-Condition :
// La pile p doit avoir été initialisée

// Post-Condition :
// Comportement indéterminé si p n'a pas été initialisée

/******************************************************************************/

// EST_PILE_PLEINE

// p : pile initialisée
// Renvoie vrai ssi la pile p ne peut plus contenir de nouveaux éléments

bool est_pile_pleine (pile_t p) {
    return p->longueur == p->taille ;
}

// Pré-Condition :
// La pile p doit avoir été initialisée

// Post-Condition :
// Comportement indéterminé si p n'a pas été initialisée

/******************************************************************************/

// SOMMET

// p : pile initialisée
// Renvoie un pointeur sur l'élément en sommet de la pile p

void* sommet (pile_t p) {
    return p->array + p->elt * (p->longueur-1) ;
}

// Pré-Condition :
// La pile p doit avoir été initialisée et contenir au moins un élément

// Post-Condition :
// Comportement indéterminé si p n'a pas été initialisée ou est vide

// Remarque :
// Le pointeur renvoyé doit être "casté" selon le type attendu
// pour être exploitable. Comportement indéterminé sinon.

/******************************************************************************/

// PIED

// f : pile initialisée
// Renvoie un pointeur sur l'élément au pied de la pile p

void* pied (pile_t f) {
    return f->array ;
}

// Pré-Condition :
// La pile p doit avoir été initialisée et contenir au moins un élément

// Post-Condition :
// Comportement indéterminé si p n'a pas été initialisée ou est vide

// Remarque :
// Le pointeur renvoyé doit être "casté" selon le type attendu
// pour être exploitable. Comportement indéterminé sinon.

/******************************************************************************/
/*                           AJOUT D'ELEMENTS                                 */
/******************************************************************************/

// EMPILER

// p : pile initialisée
// Renvoie un pointeur non NULL ssi l'élément elt a pu être empilé

void* empiler (pile_t p, void* elt) {
    void* res = memcpy(p->array + p->elt * p->longueur, elt, p->elt) ;
    p->longueur++ ;
    return res ;
}

// Pré-Condition :
// La pile p doit avoir été initialisée et ne pas être pleine

// Post-Condition :
// Comportement indéterminé si p n'a pas été initialisée ou est pleine
// ou si elt n'est pas de même type que les éléments attendus par p
// Une copie de elt a été ajouté en sommet de pile en cas de succès

/******************************************************************************/
/*                       SUPPRESSION D'ELEMENTS                               */
/******************************************************************************/

// DEPILER

// p : pile initialisée

void depiler (pile_t p) {
    p->longueur-- ;
}

// Pré-Condition :
// La pile p doit avoir été initialisée et contenir au moins un élément

// Post-Condition :
// Comportement indéterminé si p n'a pas été initialisée ou est vide

// Remarque :
// Le sommet de la pile est désormais l'élément précédemment empilé

/******************************************************************************/
/*                              AFFICHAGE                                     */
/******************************************************************************/

// AFF_PILE (coût en O(n) avec n le nombre d'éléments de la pile)

// p : pile initialisée
// fich : descripteur de fichier ouvert en écriture
// sdl : booléen qui indique si on fait un saut de ligne à la fin de l'affichage

void aff_pile (pile_t p, FILE *fich, bool sdl) {
    int i = 0 ;
    fprintf(fich, "[") ;
    if (p->longueur) {
        p->aff(p->array + p->elt * i, fich) ;
        i++ ;
    }
    for ( ; i < p->longueur ; i++) {
        fprintf(fich, "; ") ;
        p->aff(p->array + p->elt * i, fich) ;
    }
    fprintf(fich, "]%s", sdl ? "\n" : "") ;
}

// Pré-Condition :
// La pile p doit avoir été initialisée, fich doit être ouvert en écriture

// Post-Condition :
// Comportement indéterminé si la Pré-Condition n'est pas respectée
// Affiche le contenu de p dans fich puis saute une ligne ssi sdl == vrai

/******************************************************************************/
/*                                DESTRUCTEUR                                 */
/******************************************************************************/

// DETRUIRE_PILE

// p : pile initialisée
// Renvoie un pointeur sur pile NULL

pile_t detruire_pile (pile_t p) {
    free(p->array) ;
    free(p) ;
    return NULL ;
}

// Pré-Condition :
// La pile p doit avoir été initialisée

// Post-Condition :
// Comportement indéterminé si p n'a pas été initialisée
// Libère l'espace mémoire occupé par la pile p qui ne peut plus être utilisée

/******************************************************************************/

/******************************************************************************/
/*                        TESTS UNITAIRES DES FONCTIONS                       */
/******************************************************************************/

/*int main (void) {
    
    #include "hasard.h"

    int val ;
    const char Bap[16] = "Baptiste" ;
    pile_t p = pile_vide(10, sizeof(int), aff_i) ;
    pile_t p2 = nouvelle_pile_vide(p) ;
    pile_t ps = pile_vide(10, sizeof Bap, aff_s) ;
    
    aff_pile(p, stdout, true) ;
    aff_pile(p2, stdout, true) ;
    aff_pile(ps, stdout, true) ;
    
    for (int j = 0 ; j < 3 ; j++) {
        for (int i = 0 ; i < 10 ; i++) {
            val = hasard(0,9) ;
            empiler(p, &val) ;
            empiler(p2, &val) ;
            empiler(ps, (void *)Bap) ;
            aff_pile(p, stdout, true) ;
            aff_pile(p2, stdout, true) ;
            aff_pile(ps, stdout, true) ;
        }    
        
        if (!est_pile_vide(p))
            do {
                printf("longueur = %d\n", longueur_pile(p)) ;
                printf("sommet = %d\n", !est_pile_vide(p) ? *(int*)sommet(p) : -1) ;
                printf("pied = %d\n", !est_pile_vide(p) ? *(int*)pied(p) : -1) ;
                printf("sommet = %s\n", !est_pile_vide(ps)?(char*)sommet(ps):NULL) ;
                printf("pied = %s\n", !est_pile_vide(ps)?(char*)pied(ps) : NULL) ;
                aff_pile(p, stdout, true) ;
                aff_pile(p2, stdout, true) ;
                aff_pile(ps, stdout, true) ;
            } while(depiler(p), depiler(p2), depiler(ps), !est_pile_vide(p)) ;
        aff_pile(p, stdout, true) ;
        aff_pile(p2, stdout, true) ;
        aff_pile(ps, stdout, true) ;
    }
    p = detruire_pile(p) ;
    p2 = detruire_pile(p2) ;
    ps = detruire_pile(ps) ;
    return EXIT_SUCCESS ;
}*/
