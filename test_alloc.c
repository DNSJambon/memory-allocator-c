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
    fprintf(stderr, "\n-------------------TEST ALLOC------------------------\nTest réalisant de multiples fois une initialisation "
                    "suivie d'une alloc max.\n"
                    "Définir DEBUG à la compilation pour avoir une sortie un "
                    "peu plus verbeuse."
                    "\n\n");
    mem_init(get_memory_adr(), get_memory_size());
    srand(clock());
    for (int i = 0; i < NB_TESTS; i++) {
        debug("Allocating memory\n");
        mem_alloc(rand()%8192);
    }
    mem_show(afficher_zone);
    debug("PASSED\n \n");

    debug("On test ensuite une allocation NULL\n");
    mem_init(get_memory_adr(), get_memory_size());
    mem_alloc(0);
    mem_show(afficher_zone);
    debug("PASSED\n \n");

    debug("On test enfin une allocation Superieure \n");
    mem_init(get_memory_adr(), get_memory_size());
    mem_alloc((get_memory_size())+10);
    mem_show(afficher_zone);
    debug("PASSED\n");

    // TEST OK
    return 0;
}
