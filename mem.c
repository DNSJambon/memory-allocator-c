/* On inclut l'interface publique */
#include "mem.h"
/* ainsi que les détails d'implémentation locaux */
#include "common.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

/* Définition de l'alignement recherché
 * Avec gcc, on peut utiliser __BIGGEST_ALIGNMENT__
 * sinon, on utilise 16 qui conviendra aux plateformes qu'on cible
 */
#ifdef __BIGGEST_ALIGNMENT__
#define ALIGNMENT __BIGGEST_ALIGNMENT__
#else
#define ALIGNMENT 16
#endif

/* structure placée au début de la zone de l'allocateur

   Elle contient toutes les variables globales nécessaires au
   fonctionnement de l'allocateur

   Elle peut bien évidemment être complétée
*/
struct all_h {
    size_t mem_size;
    mem_fit_function_t *fit;
    struct fb *first_fb;
};

/* La seule variable globale autorisée
 * On trouve à cette adresse le début de la zone à gérer
 * (et une structure 'struct allocator_header)
 */
static void *memory_addr;

static inline void *get_system_memory_addr() {
    return memory_addr;
}

static inline struct all_h *get_header() {
    struct all_h *h;
    h = get_system_memory_addr();
    return h;
}

static inline size_t get_system_memory_size() {
    return get_header()->mem_size;
}

struct fb {
    // Taille, entête compris
    size_t size;
    struct fb *next;
};

struct ub {
   size_t size;
};

//
void mem_init(void *mem, size_t taille) {
    memory_addr = mem;
    struct all_h *ptr_h = (struct all_h*)mem;

    /* On vérifie qu'on a bien enregistré les infos et qu'on
     * sera capable de les récupérer par la suite
     */
    assert(mem == get_system_memory_addr());
    
    ptr_h->mem_size = taille;
    assert(taille == get_system_memory_size());

    //initialisation de la liste chainée de blocs libres
    struct fb *ptr_fb = (struct fb*)(mem + sizeof(struct all_h));
    ptr_fb->size = taille - sizeof(struct all_h);
    ptr_fb->next = NULL;
    ptr_fb->free=1;
    ptr_h->first_fb = ptr_fb;
    


    /* On enregistre une fonction de recherche par défaut */
    mem_fit(&mem_fit_first);
}

void mem_show(void (*print)(void *, size_t, int)) {
    unsigned long i = (unsigned long)memory_addr+sizeof(struct all_h);
    void *addr=void*(get_header()+1);
    struct fb *fb_next = (get_header())->first_fb;
    while (addr<memory_addr+(get_header())->first_fb->mem_size) {
        size_t t = *(size_t*)i;
        if ( (void*)fb_next == addr ) {
            print((void*)i,t,1);
            fb_next=fb_next->next;
            addr=adr + ((struct ub*)addr)->size
        }
        else {
            print((void*)i,t,0);
            addr=addr + ((struct ub*)addr)->size
        };
        i+=t;
    }
}

void mem_fit(mem_fit_function_t *f) {
    get_header()->fit = f;
}

void *mem_alloc(size_t taille) {
    /* ... */
    __attribute__((
        unused)) /* juste pour que gcc compile ce squelette avec -Werror */
    struct fb *fb = get_header()->fit(/*...*/ NULL, /*...*/ 0);
    /* ... */
    return NULL;
}


void mem_free(void *mem) {
}

struct fb *mem_fit_first(struct fb *list, size_t size) {
    struct fb *tmp= list;
    while (tmp->mem_size<size) {
        tmp=tmp->next;
    };
    return tmp;
}

/* Fonction à faire dans un second temps
 * - utilisée par realloc() dans malloc_stub.c
 * - nécessaire pour remplacer l'allocateur de la libc
 * - donc nécessaire pour 'make test_ls'
 * Lire malloc_stub.c pour comprendre son utilisation
 * (ou en discuter avec l'enseignant)
 */
size_t mem_get_size(void *zone) {
    /* zone est une adresse qui a été retournée par mem_alloc() */

    /* la valeur retournée doit être la taille maximale que
     * l'utilisateur peut utiliser dans cette zone */
    return 0;
}

/* Fonctions facultatives
 * autres stratégies d'allocation
 */
struct fb *mem_fit_best(struct fb *list, size_t size) {
    return NULL;
}

struct fb *mem_fit_worst(struct fb *list, size_t size) {
    return NULL;
}
