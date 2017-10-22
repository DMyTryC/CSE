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
    
    void *adr[4] ;
    int bloc = TB ;
    
    mem_init(get_memory_adr(), get_memory_size());
    printf("\n\n") ;
    
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

