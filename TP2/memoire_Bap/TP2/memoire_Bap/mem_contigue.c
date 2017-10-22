#include "mem.h"

typedef struct fb {
    size_t size ;
    struct fb *next ;
} fb ;


void * mem_struct ;
void * mem_used ; /* memoire pour les blocs utilises */

void mem_init(char* mem, size_t taille) {

    mem_struct = mem ;
    mem_used = mem + taille ;
    
}

void* mem_alloc(size_t size) {

    int reste = size % sizeof (int) ;
    int final_size = size + sizeof (int) - reste ;

    if (mem_struct + sizeof final_size > mem_used - final_size)
        return NULL ;
   
    *mem_struct = final_size ; // allocation mémoire pour copier la variable taille du bloc
    mem_struct += sizeof final_size ; // déplacement du pointeur taille de bloc
    *mem_struct =  mem_used ; // allocation mémoire pour copier la variable adresse du bloc
    mem_struct += sizeof final_size ; // déplacement du pointeur variable adresse du bloc
    
    mem_used -= final_size ; // déplacement du pointeur vers variable adresse du bloc
   
    return mem_used ; 

}


void mem_free(void* zone) {

    

    


size_t mem_get_size(void *);
