// PRE-PROCESSING

#include "mem.h"
#include "common.h"
#include <stdio.h>
#include <string.h>

// Constantes utiles

#define DEADBEEF 0xdeadbeef // "Viande morte" pour les débordements mémoire
#define FADEDOFF 0xfaded0ff // Zones "disparues" pour les zones mémoire libérées
#define  BADSEED 0x13ad5eed // Zones non initialisées après un malloc 

#define HEAP get_memory_adr()                        // Adresse du Tas
#define SIZE get_memory_size()                       // Taille du Tas
#define LZL  ((entete_listes*)HEAP)->lzl             // Liste Zones Libres
#define LZ   ((entete_listes*)HEAP)->lz              // Liste Zones
#define FIT  ((entete_listes*)HEAP)->fit_function    // Fonction de recherche
#define DEB  ((entete_listes*)HEAP + 1)              // Debut Plage d'Allocation
#define NBM  (int*)((entete_zone*)(HEAP + SIZE) - 1) // Nombre de Blocs Mémoire
#define FIN  (entete_zone*)(HEAP + SIZE) - *NBM - 1  // Fin Plage d'Allocation
#define LIB 1                                        // free == true
#define OCC 0                                        // free == false

// Définition du type entete_zone

typedef struct fb {
    size_t     size ; // taille  de la zone
    struct fb *next ; // adresse de la prochaine zone  contigüe
    struct fb *prev ; // adresse de la zone précédente contigüe
    struct fb *nxfr ; // adresse de la prochaine zone libre (NeXt FRee)
    int        free ; // booléen indiquant si la zone est libre(1) ou occupée(0)
} entete_zone ;

// Renommage du type entete_zone afin de spécifier le type de liste chaînée

typedef entete_zone entete_zone_libre ;
typedef entete_zone entete_zone_occupee ;

// Définition du type entete_listes qui
// permet de parcourir les différents types de listes

typedef struct entete_listes {
    entete_zone_libre   *lzl ; // liste des zones libres
    entete_zone         *lz  ; // liste des zones
    mem_fit_function_t  *fit_function ; // fonction de recherche utilisée
} entete_listes ;

int calculer_padding (size_t size) {
    int reste = size % sizeof(int) ;
    return reste ? sizeof(int) - reste : reste ;
}

void aff_zone (entete_zone *ez) {
    printf("taille de la zone : %d\n\
adresse de la zone suivante : %p\nadresse de la zone précédente : %p\n\
adresse de la prochaine zone libre : %p\nlibre ? %s\n",
(int) ez->size, ez->next, ez->prev, ez->nxfr, ez->free ? "vrai" : "faux");
}

void aff_zone (entete_zone *) ;

void mem_init(char* mem, size_t taille) {

    // pointeur vers entete de la zone libre
    entete_zone_libre *ezl_P =
    (entete_zone_libre*)(mem + sizeof(entete_listes)) ;
    // structure  d'entête  de la zone libre
    entete_zone_libre  ezl_S =
    {taille - sizeof(entete_listes) - sizeof ezl_S, NULL, NULL, NULL, LIB} ;
    // entete des listes des  zones mémoires
    entete_listes      elz   = {ezl_P, ezl_P, NULL} ;
    *NBM = 0 ;
    
    memcpy((entete_listes*)mem, &elz, sizeof elz) ;
    memcpy(ezl_P, &ezl_S, sizeof ezl_S) ;
    mem_fit(mem_fit_first) ;
    
}

void* mem_alloc(size_t size) {

    entete_zone_occupee *ezo_P = (entete_zone*) FIT(LZL, size) ;

    if (!ezo_P)
        return NULL ;
    
    // Création de la nouvelle zone libre
    int offset = sizeof *ezo_P + size ; // Décalage prochaine ZL
    entete_zone_libre *new_ezl_P // Pointeur vers Nouvelle Entete Zone libre
    = (entete_zone_libre*)((char*)ezo_P + offset) ;
    entete_zone_libre  new_ezl_S // Structure entete de la nouvelle zone libre
    = {ezo_P->size - size - sizeof new_ezl_S,
    ezo_P->next, ezo_P, ezo_P->nxfr, LIB} ;

    // Mise à jour de LZL s'il pointait sur ezo_P
    LZL == ezo_P ? LZL = new_ezl_P : NULL ;

    // Mise à jour des champs de la zone libre qui devient une zone occupée
    ezo_P->size = size ;
    ezo_P->nxfr = new_ezl_P ;
    ezo_P->next = new_ezl_P ;
    if (ezo_P->prev)
        ezo_P->prev->nxfr = new_ezl_P ;
    ezo_P->free = OCC ;

    // Insertion de la nouvelle zone libre
    memcpy(new_ezl_P, &new_ezl_S, sizeof new_ezl_S) ;

    return (entete_zone_occupee*)((char*)ezo_P + sizeof *ezo_P) ;
}

void mem_free(void* zone) {

    entete_zone *zal = (entete_zone*)zone - 1 ; // Zone A Libérer

    if (!zal->free) {
        int padding = calculer_padding(zal->size) ;
        if (zal->prev) {
            if (zal->prev->free && zal->next->free) {
                printf("L Z L\n") ;
                // Fusion avec les zones libres précédente et suivante
                zal->prev->size +=
                zal->size + padding + zal->next->size + 2*sizeof(entete_zone);
                zal->prev->nxfr  = zal->next->nxfr ;
                zal->prev->next  = zal->next->next ;
            }
            else if (zal->next->free) {
                printf("O Z L\n") ;
                // Fusion avec la zone libre suivante
                zal->size += zal->next->size + padding + sizeof(entete_zone) ;
                zal->nxfr  = zal->next->nxfr ;
                zal->next  = zal->next->next ;
                zal->prev->nxfr = zal ;
                if(zal->next)
                    zal->next->prev = zal ;
                zal->free  = LIB ;

            }
            else if (zal->prev->free) {
                printf("L Z O\n") ;
                // Fusion avec la zone libre précédente
                zal->prev->size += zal->size + padding + sizeof(entete_zone) ;
                zal->prev->next  = zal->next ;
                zal->next->prev  = zal->prev ;
            }
            else {
                printf("O Z O\n") ;
                // Libération de la zone
                zal->size += padding ;
                zal->prev->nxfr = zal ;
                zal->free = LIB ;
           }
        }
        // Sinon s'il n'y a pas de zone mémoire précédente...
        else {
            if (zal->next->free) {
                printf("Z L\n") ;
                // Fusion avec la zone libre suivante
                zal->size += padding + zal->next->size + sizeof(entete_zone) ;
                zal->nxfr  = zal->next->nxfr ;
                zal->next  = zal->next->next ;
                zal->next->prev = zal ;

            }
            else {
                printf("Z O\n") ;
                // Libération de la zone
                zal->size += padding ;
            }
            zal->free = LIB ;
            //printf("zal libre : %s\n", zal->free ? "true" : "false") ;
        }
        // Mise à jour de LZL s'il n'y a pas de zone précédente
        zal < LZL ? LZL = zal : LZL ;
    }
}

size_t mem_get_size(void *zone) {

    entete_zone_occupee *ezo = (entete_zone_occupee*)zone - 1 ;
    int padding = calculer_padding(ezo->size) ;
    return ezo->size + padding
    + ezo->next ? sizeof(entete_zone_libre) + ezo->next->size : 0 ;
}

void mem_show(void (*print)(void *adresse, size_t size, int free)) {

    entete_zone *ezc = LZ ; // Entete Zone Courante

    while (ezc) {
        print(ezc, ezc->size, ezc->free) ;
        ezc = ezc->next ;
    }
}

void mem_fit(mem_fit_function_t* fit_function) {
    FIT = fit_function ;
}

struct fb* mem_fit_first(struct fb* zone, size_t size) {

    entete_zone_libre *ezlc = zone ; // Entete Zone Libre Courante

    while (ezlc) {
        if (ezlc->size >= size + sizeof(entete_zone_libre) + sizeof(int))
            return ezlc ;
        ezlc = ezlc->nxfr ;
    }
    return NULL ;
}

int main (void) {
    
    *NBM = 5 ;
    printf("Adresse NBM : %p == %p, Val NBM : %d\n", (entete_zone*)NBM - *NBM, FIN, *NBM) ;
    printf("Taille du tas : %d\n", (char*)FIN - (char*)DEB - 3) ;
    char * p1 = malloc(1) ;
    printf("Taille de *p1 : %d, %d\n", sizeof *p1, sizeof ((int *)p1)) ;
    strcpy(p1, "AAA") ;
    printf("Chaine : %s, %#X\n", p1, *(unsigned int*)p1) ;
    
    free(p1) ;
    
    
    return 0 ;
    
}
