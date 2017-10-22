// PRE-PROCESSING

#include "mem.h"
#include "common.h"
#include <stdio.h>
#include <limits.h>
#include <string.h>

// Algo de recherche utilisé (mem_fit_first ou mem_fit_best ou mem_fit_worst) 

#define SEARCH mem_fit_first

// Marqueurs pour vérifier les lectures/écritures en mémoire interdites 

#define DEADBEEF 0xdeadbeef // "Viande morte" pour les débordements mémoire
#define FADEDOFF 0xfaded0ff // Zones "disparues" pour les zones mémoire libérées
#define  BADSEED 0x13ad5eed // Zones non initialisées après un malloc

// Constantes utiles

#define HEAP get_memory_adr()                        // Adresse du Tas
#define SIZE get_memory_size()                       // Taille du Tas
#define LAST (HEAP + SIZE)                           // Dernière Adresse du Tas
#define LZL  ((entete_listes*)HEAP)->lzl             // Liste Zones Libres
#define LZ   ((entete_listes*)HEAP)->lz              // Liste Zones
#define FIT  ((entete_listes*)HEAP)->fit_function    // Fonction de recherche
#define DEB  ((entete_listes*)HEAP + 1)              // Debut Plage d'Allocation
#define NBM  (*((int*)LAST - 1))                     // Nombre de Blocs Mémoire
#define FIN  ((entete_zone**)((int*)LAST - 1) - NBM) // Fin Plage d'Allocation
#define OBO  (sizeof(size_t) + sizeof(int))          // Offset Bloc Occupé
#define TZA  ((char*)LAST - (char*)FIN)              // Taille Zone Adresses
#define LIB(zone) *get_zone(zone) = zone
#define OCC(zone) *get_zone(zone) = NULL ;

// DEFINITION DU TYPE FB

// Définition du type entete_zone

typedef struct fb {
    size_t     size ; // taille  de la zone
    int        free ; // booléen indiquant si la zone est libre(1) ou occupée(0)
    struct fb *nxfr ; // adresse de la prochaine zone libre (NeXt FRee)
} entete_zone ;

// DEFINITION DU TYPE ENTETE_LISTES

// Définition du type entete_listes qui
// permet de parcourir les différents types de listes

typedef struct entete_listes {
    entete_zone         *lzl ; // liste des zones libres
    entete_zone         *lz  ; // liste des zones
    mem_fit_function_t  *fit_function ; // fonction de recherche utilisée
} entete_listes ;

// DEFINITION DE FONCTIONS UTILES A L'IMPLANTATION DE L'ALLOCATEUR

int calculer_padding (size_t size) {
    int reste = size % sizeof(void*) ;
    printf("reste : %d\n", reste) ;
    return reste || !size ? sizeof(void*) - reste : reste ;
}

// Retourne l'adresse de l'entête du bloc contigü suivant

entete_zone* next (entete_zone *zone) {
    return ((char*)zone + zone->size + sizeof *zone) < (char*)FIN ?
    (entete_zone*)((char*)zone + zone->size + sizeof *zone) : NULL ;}

// Retourne l'adresse du début du bloc mémoire occupé
// à partir de l'adresse d'une entête d'un bloc occupé

char* get_bloc (entete_zone *zone) {
    return (char*)zone + OBO ;
}

// Retourne l'adresse de la frame qui stocke l'information
// de l'état (libre ou occupé) d'un bloc mémoire

entete_zone** get_zone (entete_zone *zone) {
    return ((entete_zone**)((int*)LAST - 1) - zone->free) ;
}

// Retourne Vrai ssi zone est une entête de zone libre

int est_libre (entete_zone *zone) {
    return *get_zone(zone) == zone ;
}

// Détruit la frame associée au bloc qui n'existe plus

void supp_zone_libre (entete_zone *zone) {
    *get_zone(zone) = *get_zone(zone) == *FIN ? NULL : *FIN ;
    NBM-- ;
}

// Affiche les champs de l'entête d'une zone mémoire pour le débogage

void aff_zone (entete_zone *ez) {
    printf("Adresse : %p\nTaille de la zone : %d\nLibre ? %s\n\
Adresse de la zone suivante : %p\n", ez, (unsigned int) ez->size,
est_libre(ez) ? "vrai" : "faux", next(ez));
    if (est_libre(ez))
        printf("Adresse de la prochaine zone libre : %p\n", ez->nxfr) ;
}

// Marque une zone mémoire afin d'en interdire l'accès

void marquer_zone (char* zone, size_t size, unsigned int marqueur) {
    for (int i = 0 ; i < size % sizeof(int) ; i++)
        *((int*)zone + i) = marqueur ;
}

// Mise à jour des champs d'une zone libre qui devient une zone occupée

char* maj_zone_occupee (entete_zone *zone, int tbloc) {
    OCC(zone) ;
    marquer_zone(get_bloc(zone) + zone->size, sizeof(entete_zone*), DEADBEEF) ;
    marquer_zone(get_bloc(zone),  zone->size, BADSEED) ;
    aff_zone(zone) ;
    return get_bloc(zone) ;
}

// Crée une nouvelle zone libre (si possible) après le bloc mémoire alloué

entete_zone* creer_nouvelle_zone_libre (entete_zone* prec, entete_zone* zone,
size_t size) {
    if (!zone)
        return NULL ;
    int padding = calculer_padding(size) ;        // Alignement
    int tbloc = size + padding ;                  // Taille Bloc
    int offset = sizeof *zone + tbloc ;           // Décalage prochaine ZL
    int place_restante = zone->size - tbloc ;     // Place Restante
    if (place_restante >= sizeof(entete_zone)) {
        entete_zone *new_ezl_P // Pointeur vers Nouvelle Entete Zone libre
        = (entete_zone*)((char*)zone + offset) ;
        entete_zone  new_ezl_S // Structure entete de la nouvelle zone libre
        = {zone->size - tbloc - sizeof new_ezl_S, ++NBM, zone->nxfr} ;
        *FIN = new_ezl_P ;
        if(zone <= LZL) {
             new_ezl_P->nxfr = LZL ;
             LZL = new_ezl_P ;
        }
        else if (prec)
            prec->nxfr = new_ezl_P ;
        zone->size = tbloc ;
        zone->nxfr = new_ezl_P ;
        // Insertion de la nouvelle zone libre
        memcpy(new_ezl_P, &new_ezl_S, sizeof new_ezl_S) ;
    }
    else {
        marquer_zone(get_bloc(zone) + tbloc, place_restante, DEADBEEF) ;
        if (zone <= LZL) {
            LZL = LZL->nxfr ;
        }
        else if (prec)
            prec->nxfr = zone->nxfr ;
    }
    return zone ;
}

// Permet à l'utilisateur de choisir l'algo de recherche
// (Fonction non utilisée)

void choisir_fit_function (void) {
    
    char c ;
    printf("\nfonction de recherche ? ");
    scanf(" %c", &c) ;
    
    switch (c) {
        case 'b'  : case 'B' : mem_fit(mem_fit_best)  ; break ;
        case 'w'  : case 'W' : mem_fit(mem_fit_worst) ; break ;
        default :          mem_fit(mem_fit_first) ;
    }
    printf("\n") ;
}

// FONCTIONS DE L'ALLOCATEUR MEMOIRE

void mem_init(char* mem, size_t taille) {

    NBM = 0 ;
    size_t taille_zone_initiale = taille
    - sizeof(entete_listes) - sizeof(entete_zone) - sizeof(entete_zone*) ;
    // pointeur vers entete de la zone libre
    entete_zone  *ezl_P =
    (entete_zone*)(mem + sizeof(entete_listes)) ;
    // structure  d'entête  de la zone libre
    entete_zone   ezl_S = {taille_zone_initiale , ++NBM, NULL} ;
    // entete des listes des  zones mémoires
    entete_listes elz   = {ezl_P, ezl_P, NULL} ;
    *FIN = ezl_P ;
    memcpy((entete_listes*)mem, &elz, sizeof elz) ;
    memcpy(ezl_P, &ezl_S, sizeof ezl_S) ;
    mem_fit(SEARCH) ;
}

void* mem_alloc(size_t size) {
    
    if (size < 0)
        return NULL ;

    entete_zone *ezo_P = (entete_zone*) FIT(LZL, size) ;

    if (!ezo_P)
        return NULL ;
    
    return maj_zone_occupee(ezo_P, ezo_P->size) ;
}

void mem_free(void* zone) {

    entete_zone *AC = LZL, *AP = NULL,         // Adresse Courante
    *zal = (entete_zone*)((char*)zone - OBO) ; // Zone A Libérer
    if(zone < (void*)DEB || zone >= (void*)FIN) {
        fprintf(stderr, "Zone mémoire interdite\n") ;
        mem_init(HEAP, SIZE) ;
        exit(1) ;
    }
    if (est_libre(zal)) {
        fprintf(stderr, "La zone mémoire d'adresse %p a déja été libérée\n",
        zone) ;
        mem_init(HEAP, SIZE) ;
        exit(2) ;
    }
    while (AC && AC < zal) {
        AP = AC ;
        AC = AC->nxfr ;
    }
    if (AP && next(AP) == zal) {
        if (next(zal) && est_libre(next(zal))) {
            //printf("L Z L\n") ;
            // Fusion avec les zones libres précédente et suivante
            supp_zone_libre(next(zal)) ;
            supp_zone_libre(zal) ;
            AP->size +=
            zal->size + next(zal)->size + 2*sizeof(entete_zone);
            AP->nxfr  = next(zal)->nxfr ;
        }
        else {
            //printf("L Z O/N\n") ;
            // Fusion avec la zone libre précédente
            supp_zone_libre(zal) ;
            AP->size += zal->size + sizeof(entete_zone) ;
        }
        marquer_zone(get_bloc(AP), AP->size, FADEDOFF) ;
        LZL = AP < LZL ? AP : LZL ;
    }
    // Sinon s'il n'y a pas de zone mémoire libre précédente...
    else {
        if (next(zal) && est_libre(next(zal))) {
            //printf("O/N Z L\n") ;
            // Fusion avec la zone libre suivante
            supp_zone_libre(next(zal)) ;
            zal->size += next(zal)->size + sizeof(entete_zone) ;
            LIB(zal) ;
            if (AP) {
                zal->nxfr = AP->nxfr->nxfr ;
                AP->nxfr = zal ;
            }   
            else {
                zal->nxfr = LZL->nxfr ;
                LZL = zal ;
            }
        }
        else {
            //printf("O/N Z O/N\n") ;
            // Libération de la zone
            LIB(zal) ;
            if (AP) {
                zal->nxfr = AP->nxfr ;
                AP->nxfr = zal ;
            }   
            else {
                zal->nxfr = LZL ;
                LZL = zal ;
            }
        }
        marquer_zone(get_bloc(zal), zal->size, FADEDOFF) ;
    }
    
    aff_zone(zal) ;
}

size_t mem_get_size(void *zone) {
    entete_zone *ez = (entete_zone*)((char*)zone - OBO) ;
    return est_libre(next(ez)) ?
    sizeof(entete_zone) + next(ez)->size : 0 ;
}

void mem_show(void (*print)(void *adresse, size_t size, int free)) {

    entete_zone *ezc = LZ ; // Entete Zone Courante

    while (ezc) {
        print(ezc, ezc->size - (!next(ezc) && est_libre(ezc) ? TZA : 0),
        est_libre(ezc)) ;
        ezc = next(ezc) ;
    }
}

void mem_fit(mem_fit_function_t* fit_function) {
    FIT = fit_function ;
}

struct fb* mem_fit_first(struct fb* zone, size_t size) {
    entete_zone *AP = NULL, *ezlc = zone ; // Entete Zone Libre Courante
    while (ezlc) {
        if (ezlc->size >= size) {
            return creer_nouvelle_zone_libre(AP, ezlc, size) ;
        }
        AP = ezlc ;
        ezlc = ezlc->nxfr ;
    }
    return NULL ;
}

struct fb* mem_fit_best(struct fb* zone, size_t size) {

    size_t best = UINT_MAX ;
    entete_zone *zone_trouvee = NULL ;
    entete_zone P = {0,0,zone}, *AP = &P, *AC ; // Entete Zone Libre Courante
    while (AP->nxfr) {
        if (AP->nxfr->size >= size && AP->nxfr->size - size <= best) {
            best = AP->nxfr->size - size ;
            zone_trouvee = AP->nxfr ;
            AC = AP ;
        }
        AP = AP->nxfr ;
    }
    return creer_nouvelle_zone_libre(AC, zone_trouvee, size) ;
}

struct fb* mem_fit_worst(struct fb* zone, size_t size) {

    size_t worst = 0 ;
    entete_zone *zone_trouvee = NULL ;
    entete_zone P = {0,0,zone}, *AP = &P, *AC ; // Entete Zone Libre Courante
    while (AP->nxfr) {
        if (AP->nxfr->size >= size && AP->nxfr->size - size >= worst) {
            worst = AP->nxfr->size - size ;
            zone_trouvee = AP->nxfr ;
            AC = AP ;
        }
        AP = AP->nxfr ;
    }
    return creer_nouvelle_zone_libre(AC, zone_trouvee, size) ;
}
