#include "liste.h"

/******************************************************************************/
/*                          DESCRIPTION DU MODULE                             */
/******************************************************************************/
/*                                                                            */
/*                 Manipulation de listes simplement chaînées                 */
/*                       avec ajout en tête et en queue                       */
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

// Maillon d'une liste                             

typedef struct cellule {
    void* elt ;               // élément
    struct cellule* succ ;    // pointeur sur le maillon suivant
} cellule ;

// Entete de la liste

typedef struct liste_t {
    unsigned taille ;         // taille en octets d'un élément
	unsigned longueur ;       // nombre de maillons de la liste
	cellule* queue ;          // pointeur sur le dernier maillon de la liste
	cellule* tete ;           // pointeur sur le premier maillon de la liste
	cle_t cle ;               // fonction qui retourne la clé de l'élément
	cmp_t cmp ;               // fonction de comparaison de deux clés
	aff_t aff ;               // fonction d'affichage d'un élément
} *liste_t ;

/******************************************************************************/

/******************************************************************************/
/*                               IMPLANTATION                                 */
/******************************************************************************/
/*                                                                            */
/*        Définitions des fonctions que l'utilisateur peut utiliser           */ 
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                              CONSTRUCTEURS                                 */
/******************************************************************************/

// LISTE_VIDE

// taille : taille en octets d'un élément de la liste (défini le type)
//    cle : fonction qui renvoie la clé de l'élément
//    cmp : fonction qui associe un ensemble d'opérateurs relationnels
//          à deux opérandes de même type
//          Les opérateurs sont : INF, INFEGAL, EGAL, SUPEGAL, SUP et DIFF
//    aff : fonction d'affichage d'un élément
// Renvoie un pointeur sur une nouvelle liste initialisée et vide
// ou
// Renvoie un pointeur NULL si la création de la liste a échoué

liste_t liste_vide (unsigned taille, cle_t cle, cmp_t cmp, aff_t aff) {
	liste_t l = malloc (sizeof *l) ;
	if (l) {
	    l->taille = taille ;
	    l->longueur = 0 ;
	    l->queue = NULL ;
	    l->tete =  NULL ;
	    l->cle = cle ;
	    l->cmp = cmp ;
	    l->aff = aff ;
	}
	return l ;
}

// Pré-Condition :
// L'utilisateur ne peut stocker que des éléments de même type

// Post-Condition :
// Une liste vide a été créée en mémoire
// s'il existe assez de mémoire pour l'initialiser

/******************************************************************************/

// NOUVELLE_LISTE_VIDE

// l : liste initialisée
// Renvoie un pointeur sur une nouvelle liste initialisée et vide
// ou
// Renvoie un pointeur NULL si la création de la liste a échoué

liste_t nouvelle_liste_vide (liste_t l) {
	liste_t nl = malloc (sizeof *nl) ;
	if (nl) {
	    nl->taille = l->taille ;
	    nl->longueur = 0 ;
	    nl->queue = NULL ;
	    nl->tete =  NULL ;
	    nl->cle = l->cle ;
	    nl->cmp = l->cmp ;
	    nl->aff = l->aff ;
	}
	return nl ;
}

// Pré-Condition :
// l doit avoir été initialisée
// L'utilisateur ne peut stocker que des éléments de même type

// Post-Condition :
// Comportement indéterminé si l n'a pas été initialisée
// Une liste vide a été créée en mémoire de même type que l
// s'il existe assez de mémoire pour l'initialiser

/******************************************************************************/
/*                               ACCESSEURS                                   */
/******************************************************************************/

// LONGUEUR_LISTE

// l : liste initialisée
// Renvoie le nombre d'éléments courant que contient la liste

unsigned longueur_liste (liste_t l) {
    return l->longueur ;
}

// Pré-Condition :
// La liste l doit avoir été initialisée

// Post-Condition :
// Comportement indéterminé si l n'a pas été initialisée

/******************************************************************************/

// EST_LISTE_VIDE

// l : liste initialisée
// Renvoie vrai ssi la liste l ne contient aucun élément actuellement

bool est_liste_vide (liste_t l) {
	return !l->tete ;
}

// Pré-Condition :
// La liste l doit avoir été initialisée

// Post-Condition :
// Comportement indéterminé si l n'a pas été initialisée

/******************************************************************************/
/*                           AJOUT D'ELEMENTS                                 */
/******************************************************************************/

// AJOUTER_EN_TETE

//   l : liste initialisée
// elt : adresse de l'élément à ajouter dans la liste
// Renvoie l'adresse de l'élément s'il a été ajouté avec succès. NULL sinon.

void* ajouter_en_tete (liste_t l, void* elt) {
	void* res = NULL ;
	cellule* C = malloc(sizeof *C) ;
	if (C) {
	    C->elt = malloc (l->taille) ;
	    if (C->elt) {
		    res = memcpy(C->elt, elt, l->taille) ;
		    C->succ = l->tete ;
		    l->tete = C ;
		    l->longueur++ ;
		    l->queue = res && l->longueur == 1 ? l->tete : l->queue ;
		}
		else
		    free(C) ;
	}
	return res ? C->elt : NULL ;
}

// Pré-Condition :
// La liste l doit avoir été initialisée, elt doit être du même type que celui
// défini à l'initialisation

// Post-Condition :
// Comportement indéterminé si la Pré-Condition n'est pas respectée
// une copie de elt a été ajouté en tête de liste

/******************************************************************************/

// AJOUTER_EN_QUEUE

//   l : liste initialisée
// elt : adresse de l'élément à ajouter dans la liste
// Renvoie l'adresse de l'élément s'il a été ajouté avec succès. NULL sinon.

void* ajouter_en_queue (liste_t l, void* elt) {
    void* creer_liste_singleton (liste_t singleton, void* s) {
        void* res = NULL ;
	    cellule* C = malloc(sizeof *C) ;
	    if (C) {
	        C->elt = malloc (singleton->taille) ;
	        if (C->elt) {
		        res = memcpy(C->elt, s, singleton->taille) ;
		        C->succ = NULL ;
		        singleton->tete = C ;
		        singleton->queue = C ;
		        singleton->longueur++ ;
		    }
		    else
		        free(C) ;
	    }
	    return res ? C->elt : NULL ;
    }
	void* res =  NULL ;
	if (l->tete) {
        cellule* C = malloc(sizeof *C) ;
        if (C) {
            C->elt = malloc (l->taille) ;
            if (C->elt) {
	            res = memcpy(C->elt, elt, l->taille) ;
	            C->succ = NULL ;
	            l->queue->succ = C ;
	            l->queue = C ;
	            l->longueur++ ;
	        }
	        else
	            free(C) ;
        }
        return res ? C->elt : NULL ;
    }
    else
        return creer_liste_singleton(l, elt) ;
}

// Pré-Condition :
// La liste l doit avoir été initialisée, elt doit être du même type que celui
// défini à l'initialisation

// Post-Condition :
// Comportement indéterminé si la Pré-Condition n'est pas respectée
// une copie de elt a été ajouté en queue de liste

/******************************************************************************/

// CONCAT_LISTE

// l1 : liste initialisée
// l2 : liste initialisée
// Renvoie un pointeur NULL indiquant que la liste concaténée n'existe plus

liste_t concat_liste (liste_t l1, liste_t l2) {
    if (l2->tete) {
        if (l1->queue)
            l1->queue->succ = l2->tete ;
        else
            l1->tete = l2->tete ;
        l1->queue = l2->queue ;
        l1->longueur += l2->longueur ;
    }
    free(l2) ;
    return NULL ;
}

// Pré-Condition :
// Les listes l1 et l2 doivent avoir été initialisées
// l1 et l2 doivent être de même type

// Post-Condition :
// Comportement indéterminé si la Pré-Condition n'est pas respectée
// Les maillons de l2 ont été chaînés en queue de l1 puis l2 a été détruite

/******************************************************************************/
/*                       RECHERCHE D'ELEMENTS                                 */
/******************************************************************************/

// IEME_ELEMENT (coût en O(n) avec n le nombre d'éléments de la liste)

// l : liste initialisée
// i : indice de l'élément recherché
// Renvoie l'adresse de l'élément trouvé à la ième position
// Renvoie NULL si i < 0 || i >= longueur_liste(l)

void* ieme_element (liste_t l, unsigned i) {
	if (i < 0 || i >= l->longueur)
	    return NULL ;
	cellule* AC = l->tete ;
	unsigned indice = 0 ;
	while (AC && indice != i) {
	    indice++ ;
		AC = AC->succ ;
    }		
	return AC->elt ;
}

// Pré-Condition :
// La liste l doit avoir été initialisée

// Post-Condition :
// Comportement indéterminé si l n'a pas été initialisée

// Remarque :
// Le pointeur renvoyé doit être "casté" selon le type attendu
// pour être exploitable. Comportement indéterminé sinon.

/******************************************************************************/

// RECHERCHER_ELT (coût en O(n) avec n le nombre d'éléments de la liste)

//   l : liste initialisée
// cle : adresse de la clé de l'élément à rechercher dans la liste
// Renvoie l'adresse du premier élément
// dont la clé correspond à celle donnée en argument
// Renvoie NULL sinon

void* rechercher_elt (liste_t l, void* cle) {
	cellule* AC = l->tete ;
	while (AC && l->cmp(DIFF, l->cle(AC->elt), cle))
		AC = AC->succ ;
	return AC ? AC->elt : NULL ;
}

// Pré-Condition :
// La liste l doit avoir été initialisée
// La clé doit être une variable scalaire de même type que le type de clé
// des éléments de la liste définie à l'initialisation de la liste

// Post-Condition :
// Comportement indéterminé si la Pré-Condition n'est pas respectée

// Remarque : Si les clés ne sont pas uniques
// (Par exemple, dans le cas d'une séquence d'éléments scalaires)
// C'est le premier élément correspondant rencontré qui est renvoyé
// Le pointeur renvoyé doit être "casté" selon le type attendu
// pour être exploitable. Comportement indéterminé sinon.

/******************************************************************************/

// POSITION_ELT (coût en O(n) avec n le nombre d'éléments de la liste)

//   l : liste initialisée
// cle : adresse de la clé de l'élément à rechercher dans la liste
// Renvoie la position (indice) du premier élément
// dont la clé correspond à celle donnée en argument
// Renvoie la longueur de la liste sinon

unsigned position_elt (liste_t l, void* cle) {
	cellule* AC = l->tete ;
	unsigned pos = 0 ;
	while (AC && l->cmp(DIFF, l->cle(AC->elt), cle)) {
		AC = AC->succ ;
		pos++ ;
    }
	return pos ;
}

// Pré-Condition :
// La liste l doit avoir été initialisée
// La clé doit être une variable scalaire de même type que le type de clé
// des éléments de la liste définie à l'initialisation de la liste

// Post-Condition :
// Comportement indéterminé si la Pré-Condition n'est pas respectée

// Remarque : Si les clés ne sont pas uniques
// (Par exemple, dans le cas d'une séquence d'éléments scalaires)
// C'est la position du premier élément correspondant rencontré qui est renvoyé
// Le pointeur renvoyé doit être "casté" selon le type attendu
// pour être exploitable. Comportement indéterminé sinon.

/******************************************************************************/
/*                       SUPPRESSION D'ELEMENTS                               */
/******************************************************************************/

static bool supprimer_cellule (cellule* AP) {
    void supprimer (cellule* C) {
	    cellule* TMP = C->succ ;
	    C->succ = TMP->succ ;
	    free (TMP->elt) ;
	    free (TMP) ;
    }
    if (AP->succ) {
        supprimer(AP) ;
        return true ;
    }
    else
        return false ;
}

/******************************************************************************/

// DECAPITER

// l : liste initialisée
// Renvoie vrai ssi l'élément de tête de la liste a pu être supprimé
// Renvoie faux ssi la liste l est vide

bool decapiter (liste_t l) {
    if (l->tete) {
	    cellule* TMP = l->tete ;
	    l->tete = l->tete->succ ;
	    free (TMP->elt) ;
	    free (TMP) ;
	    l->longueur-- ;
	    return true ;
	}
	else
	    return false ;
}

// Pré-Condition :
// La liste l doit avoir été initialisée

// Post-Condition :
// Comportement indéterminé si l n'a pas été initialisée
// En cas de succès, l'élément de tête de l a été supprimée et
// l'élément suivant est le nouvel élément de tête de la liste

// Remarque : cette fonction équivaut à la fonction défiler d'une file
// si tous les éléments de la liste ont été ajoutés en queue

/******************************************************************************/

// SUPPRIMER_IEME (coût en O(n) avec n le nombre d'éléments de la liste)

// l : liste initialisée
// i : indice de l'élément à supprimer
// Renvoie vrai ssi l'élément d'indice i a pu être supprimé
// Renvoie faux ssi si i < 0 || i >= longueur_liste(l)

bool supprimer_ieme (liste_t l, unsigned i) {
    if (i < 0 || i >= l->longueur)
	    return false ;
	cellule C = {NULL, l->tete}, *AP = &C, *AC = l->tete ;
    for (int j = 0 ; AC && j < i ; j++) {
        AP = AC ;
        AC = AC->succ ;
    }
    bool res = supprimer_cellule(AP) ;
    l->tete = C.succ ;
    l->longueur-- ;
    return res ;
}

// Pré-Condition :
// La liste l doit avoir été initialisée

// Post-Condition :
// Comportement indéterminé si l n'a pas été initialisée
// En cas de succès, l'élément d'indice i a été supprimé de la liste l

/******************************************************************************/

// SUPPRIMER_ELT (coût en O(n) avec n le nombre d'éléments de la liste)

//   l : liste initialisée
// cle : adresse de la clé de l'élément à supprimer dans la liste
// Renvoie vrai ssi la fonction a pu supprimer un élément
// dont la clé correspond à celle donnée en argument
// Renvoie faux sinon

bool supprimer_elt (liste_t l, void* elt) {
    cellule C = {NULL, l->tete}, *AP = &C, *AC = l->tete ;
	while (AC && l->cmp(DIFF, l->cle(AC->elt), l->cle(elt))) {
        AP = AC ;
		AC = AC->succ ;
    }
    bool res = supprimer_cellule(AP) ;
    l->tete = C.succ ;
    l->longueur-- ;
	return res ;
}

// Pré-Condition :
// La liste l doit avoir été initialisée
// La clé doit être une variable scalaire de même type que le type de clé
// des éléments de la liste définie à l'initialisation de la liste

// Post-Condition :
// Comportement indéterminé si la Pré-Condition n'est pas respectée

// Remarque : Si les clés ne sont pas uniques
// (Par exemple, dans le cas d'une séquence d'éléments scalaires)
// C'est le premier élément correspondant rencontré qui est supprimé

/******************************************************************************/

// VIDER_LISTE (coût en O(n) avec n le nombre d'éléments de la liste)

// l : liste initialisée

void vider_liste (liste_t l) {
    while (decapiter(l)) ;
}

// Pré-Condition :
// La liste l doit avoir été initialisée

// Post-Condition :
// Comportement indéterminé si l n'a pas été initialisée
// Supprime tous les éléments de la liste

/******************************************************************************/
/*                              AFFICHAGE                                     */
/******************************************************************************/

// AFF_LISTE (coût en O(n) avec n le nombre d'éléments de la liste)

// l : liste initialisée
// fich : descripteur de fichier ouvert en écriture
// sdl : booléen qui indique si on fait un saut de ligne à la fin de l'affichage

void aff_liste (liste_t l, FILE* f, bool sdl) {
	fprintf(f, "[") ;
	if (l->tete) {
		cellule* AC = l->tete ;
		l->aff(AC->elt, f) ;
		AC = AC->succ ;
		while (AC) {
			fprintf(f, "; ") ;
		    l->aff(AC->elt, f) ;
			AC = AC->succ ;
		}
	}
	fprintf(f, "]%s", sdl ? "\n" : "") ;
}

// Pré-Condition :
// La liste l doit avoir été initialisée, fich doit être ouvert en écriture,
// L'élement affiché par aff doit être de même type que les éléments contenus
// dans la liste l
// Post-Condition :
// Comportement indéterminé si la Pré-Condition n'est pas respectée
// Affiche le contenu de l dans fich selon la fonction d'affichage aff puis
// saute une ligne ssi sdl == vrai

/******************************************************************************/
/*                              ALTERATEURS                                   */
/******************************************************************************/

// RENVERSER (coût en O(n) avec n le nombre d'éléments de la liste)

// l : liste initialisée
// Renvoie un pointeur sur une nouvelle liste

liste_t renverser (liste_t l) {
    void* deplacer_queue (liste_t l1, liste_t l2) {
        void* tmp = l2->tete->succ ;
        l2->tete->succ = l1->tete ;
        l1->tete  = l2->tete ;
        return tmp ;
    }
    liste_t nl = nouvelle_liste_vide (l) ;
    if (!nl)
        return detruire_liste(l) ;
    if (l->tete) {
        nl->queue = l->tete ;
        l->tete = deplacer_queue (nl, l) ;
    }
    while (l->tete)
        l->tete = deplacer_queue (nl, l) ;
    l = detruire_liste(l) ;
    return nl ;
}

// Pré-Condition :
// La liste l doit avoir été initialisée

// Post-Condition :
// Comportement indéterminé si l n'a pas été intialisée
// Une nouvelle liste nl a été créée et contient les maillons de l
// dans l'ordre inverse puis l a été détruite

/******************************************************************************/

// TRI_INSERTION (coût en O(n*n) avec n le nombre d'éléments de la liste)

//     l : liste initialisée
// oprel : opérateur relationnel définissant la relation d'ordre des éléments
//         Les opérateurs sont : INF, INFEGAL, EGAL, SUPEGAL, SUP et DIFF
// Renvoie un pointeur sur une nouvelle liste

liste_t tri_insertion (liste_t l, oprel_t oprel) {
    void insertion (liste_t nl, cellule* E) {
        cellule C = {NULL, nl->tete}, *AP = &C, *AC = nl->tete ;
        while (AC && nl->cmp(oprel, nl->cle(AC->elt), nl->cle(E->elt))) {
            AP = AC ;
            AC = AC->succ ;
        }
        E->succ = AC ;
        AP->succ = E ;
        nl->tete = C.succ ;
    }
    liste_t nl = nouvelle_liste_vide (l) ;
    if (!nl)
        return detruire_liste(l) ;
    nl->longueur = l->longueur ;
    void* tmp ;
    while (l->tete) {
        tmp = l->tete->succ ;
        insertion (nl, l->tete) ;
        l->tete = tmp ;
    }
    l = detruire_liste(l) ;
    return nl ;
}

// Pré-Condition :
// La liste l doit avoir été initialisée
// oprel appartient à {INF, INFEGAL, EGAL, SUPEGAL, SUP, DIFF}

// Post-Condition :
// Comportement indéterminé si la Pré-Condition n'est pas respectée
// Une nouvelle liste nl a été créée et contient les éléments triés de l
// suivant l'algorithme de tri par insertion
// et la relation d'ordre donnée par oprel

/******************************************************************************/

// TRI_FUSION (coût en O(n*log2(n)) avec n le nombre d'éléments de la liste)

//     l : liste initialisée
// oprel : opérateur relationnel définissant la relation d'ordre des éléments
//         Les opérateurs sont : INF, INFEGAL, EGAL, SUPEGAL, SUP et DIFF
// Renvoie un pointeur sur une nouvelle liste

liste_t tri_fusion (liste_t l, oprel_t oprel) {
    typedef struct {
        liste_t l1 ;
        liste_t l2 ;
    } deuxlistes ;
    void deplacer_tete (liste_t l1, liste_t l2) {
        l1->tete  = l2->tete ;
        l1->queue = l2->tete ;
        l2->tete  = l2->tete->succ ;
    }
    void deplacer_cellule (liste_t l1, liste_t l2) {
        l1->queue->succ = l2->tete ;
        l1->queue       = l2->tete ;
        l2->tete        = l2->tete->succ ;
    }
    deuxlistes* division (unsigned n, liste_t ld) {
        deuxlistes* dl = malloc(sizeof *dl) ;
        liste_t l1 = nouvelle_liste_vide(ld) ;
        liste_t l2 = nouvelle_liste_vide(ld) ;
        if (!dl || !l1 || !l2) {
            if (dl) {
                free(dl) ;
                if (l1) {
                    l1 =  detruire_liste(l1) ;
                    if (l2)
                        l2 = detruire_liste(l2) ;
                }
            }
            return NULL ;
        }
        l1->longueur = n ;
        ld->longueur -= n ;
        if (n)
            deplacer_tete(l1, ld) ;
        for (int i = 1 ; i < n ; i++)
            deplacer_cellule (l1, ld) ;
        l1->queue->succ = NULL ;
        ld = concat_liste(l2, ld) ;
        dl->l1 = l1 ;
        dl->l2 = l2 ;
        return dl ;
    }
    liste_t fusion (liste_t l1, liste_t l2) {
        void concat_puis_detruire (liste_t lst1, liste_t lst2, liste_t lst3) { 
            lst2 = concat_liste(lst1, lst2) ;
            lst1->longueur += lst3->longueur ;
            lst3 = detruire_liste(lst3) ;
        }
        if (!l1->tete) {
            l1 = detruire_liste(l1) ;
            return l2 ;
        }
        if (!l2->tete) {
            l2 = detruire_liste(l2) ;
            return l1 ;
        }
        liste_t l3 = nouvelle_liste_vide(l1) ;
        if (!l3)
            return NULL ;
        if (l3->cmp(oprel, l1->cle(l1->tete->elt), l2->cle(l2->tete->elt)))
            deplacer_tete(l3, l1) ;
        else
            deplacer_tete(l3, l2) ;
        while (l1->tete && l2->tete)
            if (l3->cmp(oprel, l1->cle(l1->tete->elt), l2->cle(l2->tete->elt)))
                deplacer_cellule (l3, l1) ;
            else
                deplacer_cellule (l3, l2) ;
        l3->queue->succ = NULL ;
        if (!l1->tete)
            concat_puis_detruire(l3, l2, l1) ;
        else
            concat_puis_detruire(l3, l1, l2) ;
        return l3 ;
    }
    // VERSION RECURSIVE
    /*if (l->longueur <= 1)
        return l ;
    deuxlistes* dl ;
    if ((dl = division(l->longueur/2, l))) {
        //printf("dl->l1 : ") ; aff_liste(dl->l1, stdout, true) ;
        //printf("dl->l2 : ") ; aff_liste(dl->l2, stdout, true) ;
        liste_t l1 = tri_fusion(dl->l1, oprel) ;
        liste_t l2 = tri_fusion(dl->l2, oprel) ;
        //printf("dl->l1 : ") ; aff_liste(l1, stdout, true) ;
        //printf("dl->l2 : ") ; aff_liste(l2, stdout, true) ;
        free(dl) ;
        return fusion(l1, l2) ;
    }
    return detruire_liste(l) ;*/
    // VERSION ITERATIVE
    if (l->longueur <= 1)
        return l ;
    pile_t p ;
    {
        extern double log2(double) ;
        extern double floor(double) ;
        double lp = log2(l->longueur) ;
        p = pile_vide((floor(lp) == lp ? lp + 1 : lp + 2), sizeof(l), NULL) ;
    }
    liste_t nl = nouvelle_liste_vide(l) ;
    if (!p)
        return detruire_liste(l) ;
    if (!nl) {
        p = detruire_pile(p) ;
        return detruire_liste(l) ;
    }
    empiler(p, &l) ;
    while (!est_pile_vide(p)) {
        liste_t s = *(liste_t*)sommet(p) ;
        depiler(p) ;
        if (s->longueur > 1) {
            deuxlistes* dl = division(s->longueur/2, s) ;
            empiler(p, &dl->l1) ;
            empiler(p, &dl->l2) ;
            free(dl) ;
        }
        else
            nl = fusion(nl, s) ;
    }
    p = detruire_pile(p) ;
    return nl ;
}

// Pré-Condition :
// La liste l doit avoir été initialisée
// oprel appartient à {INF, INFEGAL, EGAL, SUPEGAL, SUP, DIFF}

// Post-Condition :
// Comportement indéterminé si la Pré-Condition n'est pas respectée
// Une nouvelle liste nl a été créée et contient les éléments triés de l
// suivant l'algorithme de tri fusion et la relation d'ordre donnée par oprel

/******************************************************************************/
/*                                DESTRUCTEUR                                 */
/******************************************************************************/

// DETRUIRE_LISTE

// l : liste initialisée
// Renvoie un pointeur sur liste NULL

liste_t detruire_liste (liste_t l) {
    vider_liste(l) ;
	free(l) ;
	return NULL ;
}

// Pré-Condition :
// La liste l doit avoir été initialisée

// Post-Condition :
// Comportement indéterminé si l n'a pas été initialisée
// Libère l'espace mémoire occupé par la liste l qui ne peut plus être utilisée

/******************************************************************************/

/******************************************************************************/
/*                        TESTS UNITAIRES DES FONCTIONS                       */
/******************************************************************************/

/*int main (void) {
    
    #include "hasard.h"

   	liste_t
	l1 = liste_vide(sizeof(int), sca, fcmp_i, aff_i),
	l2 = nouvelle_liste_vide(l1),
	l3 = liste_vide(16, sca, fcmp_s, aff_s),
	l4 = nouvelle_liste_vide(l1) ;
	printf("l1 : ") ; aff_liste(l1, stdout, true) ;
	printf("l2 : ") ; aff_liste(l2, stdout, true) ;
	printf("l3 : ") ; aff_liste(l3, stdout, true) ;
	printf("l4 : ") ; aff_liste(l4, stdout, true) ;
	int val ;
	for (int i = 0 ; i < 10 ; i++) {
		ajouter_en_tete(l1, &i) ;
		ajouter_en_queue(l2, &i) ;
		val = hasard(0,20) ;
		ajouter_en_tete(l4, &val) ;	
	}
	
	printf("longueur l1 = %u\n", longueur_liste(l1)) ;
	printf("l1 : ") ; aff_liste(l1, stdout, true) ;
	printf("l2 : ") ; aff_liste(l2, stdout, true) ;
	printf("l4 : ") ; aff_liste(l4, stdout, true) ;
	int cle = 0 ;
	printf("elt 1 = %d\n", *(int*)rechercher_elt(l1, &cle)) ;
    printf("elt d'indice 9 = %d\n", *(int*)ieme_element(l1, 9)) ;
    printf("position de l'élément 0 = %u\n", position_elt(l1, &cle)) ;
    cle = 42 ;
    printf("position de l'élément 42 = %u\n", position_elt(l1, &cle)) ;
   	
    l2 = concat_liste(l1, l2) ;
    printf("l1 : ") ; aff_liste(l1, stdout, true) ;
  	printf("longueur l1 = %u\n", longueur_liste(l1)) ;
  		
    
    l4 = tri_insertion(l4, SUP) ;
    printf("l1 : ") ; aff_liste(l1, stdout, true) ;
    printf("l4 : ") ; aff_liste(l4, stdout, true) ;
	printf("longueur l1 = %u\n", longueur_liste(l1)) ;
	printf("longueur l4 = %u\n", longueur_liste(l4)) ;
	l2 = nouvelle_liste_vide(l1) ;
  	l1 = concat_liste(l2, l1) ;
    printf("l2 : ") ; aff_liste(l2, stdout, true) ;
    l2 = tri_fusion(l2, INF) ;
    printf("l2 : ") ; aff_liste(l2, stdout, true) ;
    l2 = detruire_liste(l2) ;
	l4 = detruire_liste(l4) ;
	
	//ajouter_en_tete(l3, "Chaine1") ;
	ajouter_en_tete(l3, "Chainees") ;
    ajouter_en_tete(l3, "chaineDeux") ;
    ajouter_en_tete(l3, "Chaine5") ;
    ajouter_en_tete(l3, "Chaine3") ;
    //ajouter_en_queue(l3, "") ;
    printf("l3 : ") ; aff_liste(l3, stdout, true) ;
    l3 = tri_fusion(l3, INF) ;
    printf("l3 : ") ; aff_liste(l3, stdout, true) ;
    l3 = renverser(l3) ;
    printf("l3 : ") ; aff_liste(l3, stdout, true) ;
    l3 = renverser(l3) ;
    printf("l3 : ") ; aff_liste(l3, stdout, true) ;
    l3 = tri_insertion(l3, SUP) ;
    printf("l3 : ") ; aff_liste(l3, stdout, true) ;
    vider_liste(l3) ;
    l3 = detruire_liste(l3) ;
	
	return EXIT_SUCCESS ;	
}*/
