#ifndef __FILE_H__
#define __FILE_H__

#include "aff.h"
#include "cmp.h"

/******************************************************************************/
/*                          DESCRIPTION DU MODULE                             */
/******************************************************************************/
/*                                                                            */
/*                           Manipulation de files                            */
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
                         typedef struct file_t *file_t ;             
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

// FILE_VIDE

// taille : nombre d'éléments maximal que peut contenir la file
//    elt : taille en octets d'un élément de la file (défini le type)
//    aff : fonction d'affichage d'un élément
// Renvoie un pointeur sur une nouvelle file initialisée et vide
// ou
// Renvoie un pointeur NULL si la création a échoué

file_t file_vide (unsigned taille, unsigned elt, aff_t aff) ;

// Pré-Condition :
// L'utilisateur ne peut stocker que des éléments de même type

// Post-Condition :
// Une file vide de taille "taille" * "elt" octets a été créée en mémoire
// s'il existe assez de mémoire pour contenir la file

/******************************************************************************/

// NOUVELLE_FILE_VIDE

// f : file initialisée
// Renvoie un pointeur sur une nouvelle file initialisée et vide
// ou
// Renvoie un pointeur NULL si la création a échoué

file_t nouvelle_file_vide (file_t f) ;

// Pré-Condition :
// f doit avoir été initialisée
// L'utilisateur ne peut stocker que des éléments de même type

// Post-Condition :
// Comportement indéterminé si f n'a pas été initialisée
// Une file vide a été créée en mémoire de même type que f
// s'il existe assez de mémoire pour l'initialiser

/******************************************************************************/
/*                               ACCESSEURS                                   */
/******************************************************************************/

// LONGUEUR_FILE

// f : file initialisée
// Renvoie le nombre d'éléments courant que contient la file

unsigned int longueur_file (file_t f) ;

// Pré-Condition :
// La file f doit avoir été initialisée

// Post-Condition :
// Comportement indéterminé si f n'a pas été initialisée

/******************************************************************************/

// EST_FILE_VIDE

// f : file initialisée
// Renvoie vrai ssi la file f ne contient aucun élément actuellement

bool est_file_vide (file_t f) ;

// Pré-Condition :
// La file f doit avoir été initialisée

// Post-Condition :
// Comportement indéterminé si f n'a pas été initialisée

/******************************************************************************/

// EST_FILE_PLEINE

// f : file initialisée
// Renvoie vrai ssi la file f ne peut plus contenir de nouveaux éléments

bool est_file_pleine (file_t f) ;

// Pré-Condition :
// La file f doit avoir été initialisée

// Post-Condition :
// Comportement indéterminé si f n'a pas été initialisée

/******************************************************************************/

// TETE

// f : file initialisée
// Renvoie un pointeur sur l'élément de tête de la file f

void* tete (file_t f) ;

// Pré-Condition :
// La file f doit avoir été initialisée et contenir au moins un élément

// Post-Condition :
// Comportement indéterminé si f n'a pas été initialisée ou est vide

// Remarque :
// Le pointeur renvoyé doit être "casté" selon le type attendu
// pour être exploitable. Comportement indéterminé sinon.

/******************************************************************************/

// QUEUE

// f : file initialisée
// Renvoie un pointeur sur l'élément de queue de la file f

void* queue (file_t f) ;

// Pré-Condition :
// La file f doit avoir été initialisée et contenir au moins un élément

// Post-Condition :
// Comportement indéterminé si f n'a pas été initialisée ou est vide

// Remarque :
// Le pointeur renvoyé doit être "casté" selon le type attendu
// pour être exploitable. Comportement indéterminé sinon.

/******************************************************************************/
/*                           AJOUT D'ELEMENTS                                 */
/******************************************************************************/

// ENFILER

// f : file initialisée
// Renvoie un pointeur non NULL ssi l'élément elt a pu être enfilé

void* enfiler (file_t f, void* elt) ;

// Pré-Condition :
// La file f doit avoir été initialisée et ne pas être pleine

// Post-Condition :
// Comportement indéterminé si f n'a pas été initialisée ou est pleine
// ou si elt n'est pas de même type que les éléments attendus par f
// Une copie de elt a été ajouté en queue de file en cas de succès

/******************************************************************************/
/*                       SUPPRESSION D'ELEMENTS                               */
/******************************************************************************/

// DEFILER

// f : file initialisée

void defiler (file_t f) ;

// Pré-Condition :
// La file f doit avoir été initialisée et contenir au moins un élément

// Post-Condition :
// Comportement indéterminé si f n'a pas été initialisée ou est vide

// Remarque :
// La tête de la file est désormais l'élément suivant

/******************************************************************************/
/*                              AFFICHAGE                                     */
/******************************************************************************/

// AFF_FILE (coût en O(n) avec n le nombre d'éléments de la file)

// f : file initialisée
// fich : descripteur de fichier ouvert en écriture
// aff : fonction d'affichage d'un élément dans un fichier
// sdl : booléen qui indique si on fait un saut de ligne à la fin de l'affichage

void aff_file (file_t f, FILE *fich, bool sdl) ;

// Pré-Condition :
// La file f doit avoir été initialisée, fich doit être ouvert en écriture

// Post-Condition :
// Comportement indéterminé si la Pré-Condition n'est pas respectée
// Affiche le contenu de f dans fich puis saute une ligne ssi sdl == vrai

/******************************************************************************/
/*                                DESTRUCTEUR                                 */
/******************************************************************************/

// DETRUIRE_FILE

// f : file initialisée
// Renvoie un pointeur sur file NULL

file_t detruire_file (file_t f) ;

// Pré-Condition :
// La file f doit avoir été initialisée

// Post-Condition :
// Comportement indéterminé si f n'a pas été initialisée
// Libère l'espace mémoire occupé par la file f qui ne peut plus être utilisée

/******************************************************************************/

#endif
