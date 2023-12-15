#include "common.h"
#include "mem.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void afficher_zone(void *adresse, size_t taille, int free) {
    printf("Zone %s, Adresse : %lu, Taille : %lu\n", free ? "libre" : "occupee",
           adresse - get_memory_adr(), (unsigned long)taille);
}

void afficher_zone_libre(void *adresse, size_t taille, int free) {
    if (free)
        afficher_zone(adresse, taille, 1);
}

void afficher_zone_occupee(void *adresse, size_t taille, int free) {
    if (!free)
        afficher_zone(adresse, taille, 0);
}

int main(int argc, char *argv[]) {
    fprintf(stderr, "Test réalisant des allocations et des libérations "
                    "afin de verifier la fusion  des blocs libres."
                    "et le bon fonctionnement de la fonction de mem_free"
                    "\n\n");

    debug("Initializing memory\n");
    mem_init(get_memory_adr(), get_memory_size());
    mem_show(afficher_zone);
    printf("\n");

    debug("Allocating 10 blocks of random sizes \n");
    srand(clock());
    void *ptr[10];
    for (int i = 0; i < 10; i++) {
        ptr[i] = mem_alloc(rand() % 800);
        assert(ptr[i] != NULL);
    }
    mem_show(afficher_zone);
    printf("\n");

    debug("Freeing 0, 2, 4, 6, 8th blocks\n");
    mem_free(ptr[0]);
    mem_free(ptr[2]);
    mem_free(ptr[4]);
    mem_free(ptr[6]);
    mem_free(ptr[8]);
    mem_show(afficher_zone);
    printf("\n");

    debug("freeing 1, 3, 5, 7, 9th blocks\n");
    mem_free(ptr[1]);
    mem_free(ptr[3]);
    mem_free(ptr[5]);
    mem_free(ptr[7]);
    mem_free(ptr[9]);
    mem_show(afficher_zone);

  
    // TEST OK
    return 0;
}