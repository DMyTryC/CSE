#ifndef __PILE_H__
#define __PILE_H__

#include "aff.h"
#include "cmp.h"

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
/*                             NOM DE LA CLASSE                               */
/******************************************************************************/
/*                                                                            */
                         typedef struct pile_t *pile_t ;             
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
/*                               CONSTRUCTEURS                                */
/******************************************************************************/

// PILE_VIDE

// taille : nombre d'éléments maximal que peut contenir la pile
//    elt : taille en octets d'un élément de la pile (défini le type)
//    aff : fonction d'affichage d'un élément
// Renvoie un pointeur sur une nouvelle pile initialisée et vide
// ou
// Renvoie un pointeur NULL si la création a échoué

pile_t pile_vide (unsigned taille, unsigned elt, aff_t aff) ;

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

pile_t nouvelle_pile_vide (pile_t p) ;

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

unsigned longueur_pile (pile_t p) ;

// Pré-Condition :
// La pile p doit avoir été initialisée

// Post-Condition :
// Comportement indéterminé si p n'a pas été initialisée

/******************************************************************************/

// EST_PILE_VIDE

// p : pile initialisée
// Renvoie vrai ssi la pile p ne contient aucun élément actuellement

bool est_pile_vide (pile_t p) ;

// Pré-Condition :
// La pile p doit avoir été initialisée

// Post-Condition :
// Comportement indéterminé si p n'a pas été initialisée

/******************************************************************************/

// EST_PILE_PLEINE

// p : pile initialisée
// Renvoie vrai ssi la pile p ne peut plus contenir de nouveaux éléments

bool est_pile_pleine (pile_t p) ;

// Pré-Condition :
// La pile p doit avoir été initialisée

// Post-Condition :
// Comportement indéterminé si p n'a pas été initialisée

/******************************************************************************/

// SOMMET

// p : pile initialisée
// Renvoie un pointeur sur l'élément en sommet de la pile p

void* sommet (pile_t p) ;

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

void* pied (pile_t f) ;

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

void* empiler (pile_t p, void* elt) ;

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

void depiler (pile_t p) ;

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

void aff_pile (pile_t f, FILE *fich, bool sdl) ;

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

pile_t detruire_pile (pile_t p) ;

// Pré-Condition :
// La pile p doit avoir été initialisée

// Post-Condition :
// Comportement indéterminé si p n'a pas été initialisée
// Libère l'espace mémoire occupé par la pile p qui ne peut plus être utilisée

/******************************************************************************/

#endif
