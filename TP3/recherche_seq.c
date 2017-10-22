#include <stdio.h>
#include <stdlib.h>

int taille;
int * tab;
int el_a_trouver = 3;

void init(int t, char ** arguments){
	taille = t;
	tab = malloc(t*sizeof(int));
	int i = 0;
	for (; i<taille ; i++){
		tab[i] = atoi(arguments[i]);
	}
}

int main(int argc, char ** argv){
	init (argc, argv);

	int i = 0;
	while (i < taille){
		if (tab[i] == el_a_trouver){
			printf("TROUVE\n");	
			return 1;
		}
		i++;
	}
	free(tab);
	return -1;
}
