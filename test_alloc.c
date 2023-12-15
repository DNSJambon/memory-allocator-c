#include "common.h"
#include "mem.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NB_TESTS 10

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
    mem_init(get_memory_adr(), get_memory_size());
    fprintf(stderr, "Test réalisant de multiples fois une allocation.\n"
                    "Définir DEBUG à la compilation pour avoir une sortie un "
                    "peu plus verbeuse."
                    "\n");
    for (int i = 0; i < NB_TESTS; i++) {
        debug("allocating memory\n");
        srand(clock());
        size_t r=rand();
        mem_alloc(r%800);
    }
    mem_show(afficher_zone);
    // TEST OK
    return 0;
}

