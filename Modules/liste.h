#ifndef __LISTE_H__
#define __LISTE_H__

#include "pile.h"

/******************************************************************************/
/*                          DESCRIPTION DU MODULE                             */
/******************************************************************************/
/*                                                                            */
/*                 Manipulation de listes simplement chaînées                 */
/*                       avec ajout en tête et en queue                       */
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
/*                             NOM DE LA CLASSE                               */
/******************************************************************************/
/*                                                                            */
                         typedef struct liste_t *liste_t ;             
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                INTERFACE                                   */
/******************************************************************************/
/*                                                                            */
/*        Prototypes des fonctions que l'utilisateur peut utiliser            */ 
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

liste_t liste_vide (unsigned taille, cle_t cle, cmp_t cmp, aff_t aff) ;

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

liste_t nouvelle_liste_vide (liste_t l) ;

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

unsigned longueur_liste (liste_t l) ;

// Pré-Condition :
// La liste l doit avoir été initialisée

// Post-Condition :
// Comportement indéterminé si l n'a pas été initialisée

/******************************************************************************/

// EST_LISTE_VIDE

// l : liste initialisée
// Renvoie vrai ssi la liste l ne contient aucun élément actuellement

bool est_liste_vide (liste_t l) ;

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

void* ajouter_en_tete (liste_t l, void* elt) ;

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

void* ajouter_en_queue (liste_t l, void* elt) ;

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

liste_t concat_liste (liste_t l1, liste_t l2) ;

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

void* ieme_element (liste_t l, unsigned i) ;

// Pré-Condition :
// La liste l doit avoir été initialisée

// Post-Condition :
// Comportement indéterminé si l n'a pas été initialisée

// Remarque :
// Le pointeur renvoyé doit être "casté" selon le type attendu
// pour être exploitable. Comportement indéterminé sinon.
// Le pointeur renvoyé doit être "casté" selon le type attendu
// pour être exploitable. Comportement indéterminé sinon.

/******************************************************************************/

// RECHERCHER_ELT (coût en O(n) avec n le nombre d'éléments de la liste)

//   l : liste initialisée
// cle : adresse de la clé de l'élément à rechercher dans la liste
// Renvoie l'adresse du premier élément
// dont la clé correspond à celle donnée en argument
// Renvoie NULL sinon

void* rechercher_elt (liste_t l, void* cle) ;

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

unsigned position_elt (liste_t l, void* cle) ;

// Pré-Condition :
// La liste l doit avoir été initialisée
// La clé doit être une variable scalaire de même type que le type de clé
// des éléments de la liste définie à l'initialisation de la liste

// Post-Condition :
// Comportement indéterminé si la Pré-Condition n'est pas respectée

// Remarque : Si les clés ne sont pas uniques
// (Par exemple, dans le cas d'une séquence d'éléments scalaires)
// C'est la position du premier élément correspondant rencontré qui est renvoyé

/******************************************************************************/
/*                       SUPPRESSION D'ELEMENTS                               */
/******************************************************************************/

// DECAPITER

// l : liste initialisée
// Renvoie vrai ssi l'élément de tête de la liste a pu être supprimé
// Renvoie faux ssi la liste l est vide

bool decapiter (liste_t l) ;

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

bool supprimer_ieme (liste_t l, unsigned i) ;

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

bool supprimer_elt (liste_t l, void* elt) ;

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

void vider_liste (liste_t l) ;

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

//void aff_liste (liste_t l, FILE *fich, bool sdl) ;

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

liste_t renverser (liste_t l) ;

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

liste_t tri_insertion (liste_t l, oprel_t oprel) ;

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

liste_t tri_fusion (liste_t l, oprel_t oprel) ;

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

liste_t detruire_liste (liste_t l) ;

// Pré-Condition :
// La liste l doit avoir été initialisée

// Post-Condition :
// Comportement indéterminé si l n'a pas été initialisée
// Libère l'espace mémoire occupé par la liste l qui ne peut plus être utilisée

/******************************************************************************/

#endif
