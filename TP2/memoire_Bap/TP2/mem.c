// PRE-PROCESSING

#include "mem.h"
#include "common.h"
#include <stdio.h>
#include <limits.h>
#include <string.h>

// Constantes utiles

#define DEADBEEF 0xdeadbeef // "Viande morte" pour les débordements mémoire
#define FADEDOFF 0xfaded0ff // Zones "disparues" pour les zones mémoire libérées
#define  BADSEED 0x13ad5eed // Zones non initialisées après un malloc 

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

// Définition du type entete_zone

typedef struct fb {
    size_t     size ; // taille  de la zone
    int        free ; // booléen indiquant si la zone est libre(1) ou occupée(0)
    struct fb *nxfr ; // adresse de la prochaine zone libre (NeXt FRee)
} entete_zone ;

// Définition du type entete_listes qui
// permet de parcourir les différents types de listes

typedef struct entete_listes {
    entete_zone         *lzl ; // liste des zones libres
    entete_zone         *lz  ; // liste des zones
    mem_fit_function_t  *fit_function ; // fonction de recherche utilisée
} entete_listes ;

int calculer_padding (size_t size) {
    int reste = size % sizeof(void*) ;
    return reste || !size ? sizeof(void*) - reste : reste ;
}

entete_zone* next (entete_zone *zone) {
    return ((char*)zone + zone->size + sizeof *zone) < (char*)FIN ?
    (entete_zone*)((char*)zone + zone->size + sizeof *zone) : NULL ;   
}

char* get_bloc (entete_zone *zone) {
    return (char*)zone + OBO ;
}

entete_zone** get_zone (entete_zone *zone) {
    return ((entete_zone**)((int*)LAST - 1) - zone->free) ;
}

int est_libre (entete_zone *zone) {
    return *get_zone(zone) == zone ;
}

void supp_zone_libre (entete_zone *zone) {
    *get_zone(zone) = *get_zone(zone) == *FIN ?
    NULL : *FIN ;
    NBM-- ;
}    

// Mise à jour des champs de la zone libre qui devient une zone occupée
char* maj_zone_occupee (entete_zone *zone, int tbloc) {
    zone->size = tbloc ;
    OCC(zone) ;
    for (int i = 0 ; i < sizeof(entete_zone*) % sizeof(int) ; i++)
        *((int*)(get_bloc(zone) + zone->size) + i) = DEADBEEF ;
    for (int i = 0 ; i < zone->size % sizeof(int) ; i++)
        *((int*)get_bloc(zone) + i) = BADSEED ;
    LZL = LZL == zone ? next(zone) : NULL ;
    return get_bloc(zone) ;
}   

void aff_zone (entete_zone *ez) {
    printf("Adresse : %p\nTaille de la zone : %d\nLibre ? %s\n\
Adresse de la zone suivante : %p\n", ez, (unsigned int) ez->size,
ez->free ? "vrai" : "faux", next(ez));
    if (est_libre(ez))
        printf("Adresse de la prochaine zone libre : %p\n", ez->nxfr) ;
}

void mem_init(char* mem, size_t taille) {
    
    NBM = 0 ;
    // pointeur vers entete de la zone libre
    entete_zone  *ezl_P =
    (entete_zone*)(mem + sizeof(entete_listes)) ;
    // structure  d'entête  de la zone libre
    entete_zone   ezl_S =
    {taille - sizeof(entete_listes) - sizeof ezl_S - sizeof ezl_P,++NBM, NULL};
    // entete des listes des  zones mémoires
    entete_listes elz   = {ezl_P, ezl_P, NULL} ;
    *FIN = ezl_P ;
    
    memcpy((entete_listes*)mem, &elz, sizeof elz) ;
    memcpy(ezl_P, &ezl_S, sizeof ezl_S) ;
    mem_fit(mem_fit_best) ;
    
}

void* mem_alloc(size_t size) {
    
    if (size < 0)
        return NULL ;

    entete_zone *ezo_P = (entete_zone*) FIT(LZL, size) ;

    if (!ezo_P)
        return NULL ;
    
    // Création de la nouvelle zone libre
    int padding = calculer_padding(size) ;        // Alignement
    int tbloc = size + padding ;                  // Taille Bloc
    int offset = sizeof *ezo_P + tbloc ;          // Décalage prochaine ZL
    int place_restante = ezo_P->size - tbloc ;    // Place Restante
    
    if (place_restante >= sizeof(entete_zone)) {
        entete_zone *new_ezl_P // Pointeur vers Nouvelle Entete Zone libre
        = (entete_zone*)((char*)ezo_P + offset) ;
        entete_zone  new_ezl_S // Structure entete de la nouvelle zone libre
        = {ezo_P->size-size-sizeof new_ezl_S - padding, ++NBM, ezo_P->nxfr} ;
        *FIN = new_ezl_P ;
        
        // Insertion de la nouvelle zone libre
        memcpy(new_ezl_P, &new_ezl_S, sizeof new_ezl_S) ;    
    }
    
    else {
        for (int i = 0 ; i < place_restante % sizeof(int) ; i++)
            *((int*)(get_bloc(ezo_P) + tbloc) + i) = DEADBEEF ;
    }

    return maj_zone_occupee(ezo_P, tbloc) ;
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
            printf("L Z L\n") ;
            // Fusion avec les zones libres précédente et suivante
            supp_zone_libre(next(zal)) ;
            supp_zone_libre(zal) ;
            AP->size +=
            zal->size + next(zal)->size + 2*sizeof(entete_zone);
            AP->nxfr  = next(zal)->nxfr ;
        }
        else {
            printf("L Z O/N\n") ;
            // Fusion avec la zone libre précédente
            supp_zone_libre(zal) ;
            AP->size += zal->size + sizeof(entete_zone) ;
        }
        LZL = AP < LZL ? AP : LZL ;
    }
    // Sinon s'il n'y a pas de zone mémoire libre précédente...
    else {
        if (next(zal) && est_libre(next(zal))) {
            printf("O/N Z L\n") ;
            // Fusion avec la zone libre suivante
            supp_zone_libre(next(zal)) ;
            zal->size += next(zal)->size + sizeof(entete_zone) ;
            LIB(zal) ;
            zal->nxfr  = next(zal) ? next(zal)->nxfr : NULL ;
        }
        else {
            printf("O/N Z O/N\n") ;
            // Libération de la zone
            LIB(zal) ;
            zal->nxfr = LZL ;
            LZL = zal ;
        }
        LZL = zal < LZL ? zal : LZL ;
    }
    
    aff_zone(zal) ;
}

size_t mem_get_size(void *zone) {

    entete_zone *ez = (entete_zone*)((char*)zone - OBO) ;
    return ez->size + next(ez) ? sizeof(entete_zone) + next(ez)->size : 0 ;
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
        
    entete_zone *ezlc = zone ; // Entete Zone Libre Courante
    
    while (ezlc) {
        if (ezlc->size >= size)
            return ezlc ;
        ezlc = ezlc->nxfr ;
    }
    return NULL ;
}

struct fb* mem_fit_best(struct fb* zone, size_t size) {

    size_t best = UINT_MAX ;
    entete_zone *zone_trouvee = NULL ;
        
    entete_zone *ezlc = zone ; // Entete Zone Libre Courante
    
    while (ezlc) {
        if (ezlc->size >= size && ezlc->size - size < best) {
            best = size ;
            zone_trouvee = ezlc ;
        }
        ezlc = ezlc->nxfr ;
    }
    return zone_trouvee ;
}

struct fb* mem_fit_worst(struct fb* zone, size_t size) {

    size_t worst = 0 ;
    entete_zone *zone_trouvee = NULL ;
        
    entete_zone *ezlc = zone ; // Entete Zone Libre Courante
    
    while (ezlc) {
        if (ezlc->size >= size && ezlc->size - size >= worst) {
            worst = size ;
            zone_trouvee = ezlc ;
        }
        ezlc = ezlc->nxfr ;
    }
    return zone_trouvee ;
}

/*int main (void) {
    
    NBM = 5 ;
    printf("Adresse NBM : %p == %p, Val NBM : %d\n", (entete_zone*)NBM - NBM, FIN, NBM) ;
    printf("Taille du tas : %d\n", (char*)FIN - (char*)DEB - 3) ;
    char * p1 = malloc(1) ;
    printf("Taille de *p1 : %d, %d\n", sizeof *p1, sizeof ((int *)p1)) ;
    strcpy(p1, "AAA") ;
    printf("Chaine : %s, %#X\n", p1, *(unsigned int*)p1) ;
    
    free(p1) ;
    
    
    return 0 ;
    
}*/
