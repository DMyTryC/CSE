#include "mem.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define NB_TESTS 10
#define TB 50 // Taille Bloc
#define NBA 4 // NomBre d'Allocations

void afficher_zone(void *adresse, size_t taille, int free)
{
  printf("Zone %s, Adresse : %lx, Taille : %lu\n", free?"libre":"occupee",
         (unsigned long) adresse, (unsigned long) taille);
}

void test1(void) {
    
    mem_init(get_memory_adr(), get_memory_size());
    printf("\n\n") ;
    
    /* Test de chaque type de liberation de memoire */
    int bloc = TB ;
    void *adr[4];

    // Reinitialisation de memoire
    printf("TEST REINITIALISATION DE MEMOIRE -------\n\n");
    mem_init(get_memory_adr(), get_memory_size());
    mem_show(afficher_zone);
    printf("\n\n");
    printf("FIN TEST REINITIALISATION DE MEMOIRE -------\n\n");

    // Allocation de taille plus grande que le max ou plus petite que 0
    printf("TEST ALLOCATION TAILLE >max, -1 -------\n\n");
    printf("Alloc > max :\n");
    mem_alloc(1111111);
    mem_show(afficher_zone);
    printf("\n\n");
    printf("Alloc -1 :\n");
    mem_alloc(-1);
    mem_show(afficher_zone);
    printf("\n\n");
    printf("FIN TEST ALLOCATION TAILLE >max, -1 -------\n\n");
    mem_init(get_memory_adr(), get_memory_size());
    
    // Alloc 0
    printf("TEST D'ALLOC 0 -------\n\n");
    mem_alloc(0);
    mem_show(afficher_zone);
    printf("\n\n");
    mem_alloc(55);
    mem_alloc(0);
    mem_show(afficher_zone);
    printf("\n\n");
    printf("FIN TEST D'ALLOC 0 -------\n\n");
    mem_init(get_memory_adr(), get_memory_size());

    // Test de suppression non-lineaire    
    printf("TEST DE SUPPRESSION NON-LINEAIRE\n\n") ;
    for (int i = 0 ; i < NBA ; i++) {
        adr[i] = mem_alloc(bloc *= 2) ;
        mem_show(afficher_zone);
        printf("\n\n") ;
    }

    mem_free(adr[1]) ;
    mem_show(afficher_zone);
    printf("\n\n") ;
    mem_free(adr[0]) ;
    mem_show(afficher_zone);
    printf("\n\n") ;
    mem_free(adr[2]) ;
    mem_show(afficher_zone);
    printf("\n\n") ;
    mem_free(adr[3]) ;
    mem_show(afficher_zone);
    printf("\n\n") ; 
    printf("FIN TEST DE SUPPRESSION NON-LINEAIRE -------\n\n");

    // LZL
    printf("TEST DE LIBERATION : LZL -------\n\n");
    for (int i = 0 ; i < 3 ; i++) {
        adr[i] = mem_alloc(bloc *= 2) ;
        mem_show(afficher_zone);
        printf("\n\n") ;
    }
    mem_free(adr[0]) ;
    mem_show(afficher_zone);
    printf("\n\n") ;
    mem_free(adr[2]) ;
    mem_show(afficher_zone);
    printf("\n\n") ;
    mem_free(adr[1]) ;
    mem_show(afficher_zone);
    printf("\n\n") ;
    //Libre
    bloc = TB;
    printf("FIN TEST : LZL -------\n\n");

    // OZL
    printf("TEST DE LIBERATION : OZL -------\n\n");
    for (int i = 0 ; i < 3 ; i++) {
        adr[i] = mem_alloc(bloc *= 2) ;
        mem_show(afficher_zone);
        printf("\n\n") ;
    }
    mem_free(adr[2]) ;
    mem_show(afficher_zone);
    printf("\n\n") ;
    //Libre
    bloc = TB;
    printf("FIN TEST : OZL -------\n\n");
    mem_init(get_memory_adr(), get_memory_size());

    // LZO
    printf("TEST DE LIBERATION : LZO -------\n\n");
    for (int i = 0 ; i < 3 ; i++) {
        adr[i] = mem_alloc(bloc *= 2) ;
        mem_show(afficher_zone);
        printf("\n\n") ;
    }
    mem_free(adr[0]) ;
    mem_show(afficher_zone);
    printf("\n\n") ;
    mem_free(adr[1]) ;
    mem_show(afficher_zone);
    printf("\n\n") ;
    //Libre
    bloc = TB;
    printf("FIN TEST : LZO -------\n\n");
    mem_init(get_memory_adr(), get_memory_size());

    // OZO
    printf("TEST DE LIBERATION : OZO -------\n\n");
    for (int i = 0 ; i < 3 ; i++) {
        adr[i] = mem_alloc(bloc *= 2) ;
        mem_show(afficher_zone);
        printf("\n\n") ;
    }
    mem_free(adr[1]) ;
    mem_show(afficher_zone);
    printf("\n\n") ;
    //Libre
    bloc = TB;
    printf("FIN TEST : OZO -------\n\n");
    mem_init(get_memory_adr(), get_memory_size());

    // ZO
    printf("TEST DE LIBERATION : ZO -------\n\n");
    for (int i = 0 ; i < 2 ; i++) {
        adr[i] = mem_alloc(bloc *= 2);
        mem_show(afficher_zone);
        printf("\n\n");
    }
    mem_free(adr[0]) ;
    mem_show(afficher_zone);
    printf("\n\n") ;
    //Libre
    bloc = TB;
    printf("FIN TEST : ZO -------\n\n");
    mem_init(get_memory_adr(), get_memory_size());

    // ZL
    printf("TEST DE LIBERATION : ZL -------\n\n");
    for (int i = 0 ; i < 2 ; i++) {
        adr[i] = mem_alloc(bloc *= 2);
        mem_show(afficher_zone);
        printf("\n\n");
    }
    mem_free(adr[1]) ;
    mem_show(afficher_zone);
    printf("\n\n") ;
    //Libre
    bloc = TB;
    printf("FIN TEST : ZL -------\n\n");
    mem_init(get_memory_adr(), get_memory_size());

    // Liberation/Realloc de la meme zone
    printf("TEST REALLOC DE LA MEME ZONE APRES SUPPR -------\n\n");
    adr[0] = mem_alloc(53);
    mem_show(afficher_zone);
    printf("\n\n");
    mem_alloc(154);
    mem_show(afficher_zone);
    printf("\n\n");
    mem_free(adr[0]);
    mem_show(afficher_zone);
    printf("\n\n");
    mem_alloc(53);
    mem_show(afficher_zone);
    printf("\n\n");
    printf("FIN TEST REALLOC DE LA MEME ZONE APRES SUPPR -------\n\n");
    mem_init(get_memory_adr(), get_memory_size());

    // Test chevauchement des zones, la possibilite d'avoir cree une zone libre
    // apres la reallocation qui sera trop petite
    printf("TEST ZONE TROP PETITE -------\n\n");
    adr[0] = mem_alloc(8);
    mem_show(afficher_zone);
    printf("\n\n");
    adr[1] = mem_alloc(16);
    mem_show(afficher_zone);
    printf("\n\n");
    mem_free(adr[0]);
    mem_show(afficher_zone);
    printf("\n\n");
    adr[0] = mem_alloc(0);
    mem_show(afficher_zone);
    printf("\n\n");
    printf("FIN TEST ZONE TROP PETITE -------\n\n");
    mem_init(get_memory_adr(), get_memory_size());

    // Test de propriete de best fit pour la premiere zone libre suffisamment 
    // grande avec le plus petit residu
    printf("TEST PROPRIETE BEST FIT -------\n\n");
    {
        void *p4, *p5, *p6 ;
        adr[0] = mem_alloc(32);
        mem_show(afficher_zone);
        printf("\n\n");
        adr[1] = mem_alloc(24);
        mem_show(afficher_zone);
        printf("\n\n");
        adr[2] = mem_alloc(16);
        mem_show(afficher_zone);
        printf("\n\n");
        adr[3] = mem_alloc(24);
        mem_show(afficher_zone);
        printf("\n\n");
        p4 = mem_alloc(8);
        mem_show(afficher_zone);
        printf("\n\n");
        p5 = mem_alloc(24);
        mem_show(afficher_zone);
        printf("\n\n");
        mem_free(adr[0]) ;
        mem_show(afficher_zone);
        printf("\n\n");
        mem_free(adr[2]) ;
        mem_show(afficher_zone);
        printf("\n\n");
        mem_free(p4) ;
        mem_show(afficher_zone);
        printf("\n\n");
        p6 = mem_alloc(0) ;
        mem_show(afficher_zone);
        printf("\n\n");
        mem_free(p5) ;
        mem_show(afficher_zone);
        printf("\n\n");
        mem_free(adr[1]) ;
        mem_show(afficher_zone);
        printf("\n\n");
        mem_free(adr[3]) ;
        mem_show(afficher_zone);
        printf("\n\n");
        mem_free(p6) ;
        mem_show(afficher_zone);
        printf("\n\n");
    }
    printf("FIN TEST PROPRIETE BEST FIT -------\n\n");
    mem_init(get_memory_adr(), get_memory_size());
    
    // Test de propriete de worst fit pour la premiere zone libre suffisamment 
    // grande avec le plus petit residu
    printf("TEST PROPRIETE WORST FIT -------\n\n");
    {
        void *p4, *p5, *p6 ;
        adr[0] = mem_alloc(8);
        mem_show(afficher_zone);
        printf("\n\n");
        adr[1] = mem_alloc(24);
        mem_show(afficher_zone);
        printf("\n\n");
        adr[2] = mem_alloc(16);
        mem_show(afficher_zone);
        printf("\n\n");
        adr[3] = mem_alloc(24);
        mem_show(afficher_zone);
        printf("\n\n");
        p4 = mem_alloc(32);
        mem_show(afficher_zone);
        printf("\n\n");
        p5 = mem_alloc(24);
        mem_show(afficher_zone);
        printf("\n\n");
        mem_free(adr[0]) ;
        mem_show(afficher_zone);
        printf("\n\n");
        mem_free(adr[2]) ;
        mem_show(afficher_zone);
        printf("\n\n");
        mem_free(p4) ;
        mem_show(afficher_zone);
        printf("\n\n");
        p6 = mem_alloc(0) ;
        mem_show(afficher_zone);
        printf("\n\n");
        mem_free(p5) ;
        mem_show(afficher_zone);
        printf("\n\n");
        mem_free(adr[1]) ;
        mem_show(afficher_zone);
        printf("\n\n");
        mem_free(adr[3]) ;
        mem_show(afficher_zone);
        printf("\n\n");
        mem_free(p6) ;
        mem_show(afficher_zone);
        printf("\n\n");
    }
    printf("FIN TEST PROPRIETE WORST FIT -------\n\n");
    mem_init(get_memory_adr(), get_memory_size());
    
}

static void *alloc_max(size_t estimate) {
	void *result;
	static size_t last = 0;

	while ((result = mem_alloc(estimate)) == NULL) {
		estimate--;
	}
	debug("Alloced %zu bytes at %p\n", estimate, result);
	if (last) {
		// Idempotence test
		assert(estimate == last);
	} else
		last = estimate;
	return result;
}

int main(int argc, char *argv[]) {
	fprintf(stderr, "Test réalisant de multiples fois une initialisation suivie d'une alloc max.\n"
			"Définir DEBUG à la compilation pour avoir une sortie un peu plus verbeuse."
 		"\n");
	for (int i=0; i<NB_TESTS; i++) {
		mem_init(get_memory_adr(), get_memory_size());
		alloc_max(get_memory_size());
	}
    
    test1() ;
	// TEST OK
	return 0;
}


//O O O O L
//O L O O O L O O O O L

