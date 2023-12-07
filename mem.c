/* On inclut l'interface publique */
#include "mem.h"
/* ainsi que les détails d'implémentation locaux */
#include "common.h"
#include <stdio.h>
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

struct fb *precedent(struct fb *fb){
    struct fb *ptr_fb = get_header()->first_fb;
    if (ptr_fb == fb) {
        return NULL;
    }
    while (ptr_fb->next != fb) {
        ptr_fb = ptr_fb->next;
    }
    return ptr_fb;
}

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
    ptr_h->first_fb = ptr_fb;
    


    /* On enregistre une fonction de recherche par défaut */
    mem_fit(&mem_fit_first);
}

void mem_show(void (*print)(void *, size_t, int)) {
    unsigned long i = (unsigned long)memory_addr+sizeof(struct all_h);
    void *addr=(void*)(get_header()+1);
    struct fb *fb_next = (get_header())->first_fb;

    while (addr<memory_addr+(get_header())->mem_size) {
        size_t t = *(size_t*)i;

        if ( (void*)fb_next == addr ) {
            print((void*)i,t,1);
            fb_next=fb_next->next;
            addr=addr + ((struct fb*)addr)->size;
        }
        else {
            print((void*)i,t,0);
            addr=addr + ((struct ub*)addr)->size;
        };

        i+=t;
    }
}

void mem_fit(mem_fit_function_t *f) {
    get_header()->fit = f;
}

void *mem_alloc(size_t taille) {

    if (taille < sizeof(struct fb)) {
        taille = sizeof(struct fb);
    }
    __attribute__((
        unused)) /* juste pour que gcc compile ce squelette avec -Werror */
    struct fb *fb = get_header()->fit(get_header()->first_fb, taille);
    if (fb == NULL) {
        return NULL;
    }
    struct fb *fb_prev = precedent(fb);
    struct ub *ub = (struct ub*)fb;
    

    if (fb->size==taille+sizeof(struct ub)){
        if (fb_prev == NULL)
            get_header()->first_fb = fb->next;
        else
            fb_prev->next = fb->next;

    }
    else {

        struct fb *fb_new = (void*)fb+ sizeof(struct ub) + taille;
        fb_new->size = fb->size - (sizeof(struct ub) + taille);

        if (fb_prev != NULL){
            fb_prev->next = fb_new;
            fb_new->next = fb->next;
        }
        else {
            get_header()->first_fb = fb_new;
            fb_new->next = fb->next;    
        }
    
    }

    ub->size = taille+sizeof(struct ub);
    return (void*)(ub+1);
}

int is_fb(void *ptr){
    struct fb *fbf = get_header()->first_fb;
    while (fbf<=ptr) {
        if (fbf==ptr) {
            return 1;
        };
        fbf=fbf->next;
    }
    return 0;
}

void *prev_fb(void *ptr){
    void *ptr_fb = (void *)get_header()->first_fb;
    
}

void mem_free(void *mem) {
    if (is_fb((void*(prev_fb(mem)))) and is_fb(((void*)fb+ sizeof(struct ub) + taille))){

    }
}


struct fb *mem_fit_first(struct fb *list, size_t size) {
    struct fb *ptr_fb = list;
    while (ptr_fb != NULL) {
        if (ptr_fb->size >= size+sizeof(struct ub)) {
            return ptr_fb;
        }
        ptr_fb = ptr_fb->next;
    }
    return NULL;
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
