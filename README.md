# MEMORY ALLOCATOR

**Authors:**  
Matthieu Muller  
Arthur Mermet

### Memory Allocator

#### Choices for Implementation

- **mem_free**: For the implementation of free and to make our code simpler and clearer, we first release the block by chaining it to other free blocks, then call the `fusion_fb` function to merge adjacent free zones.

- **struct ub** and **struct fb**: These structures represent the occupied and free partitions of memory. For `fb`, we implement the size and the next free block to easily traverse this list. For `ub`, however, to save memory, we only implement its size.

- **mem_alloc**: To avoid any issues when freeing zones, we only allocate sizes greater than the heaviest structure (`fb`) (increasing it if necessary when the size parameter is too small). Additionally, all allocations, like in the standard C library's malloc, are aligned to a 16-byte padding. Finally, to allow the user to write directly to this zone, the function returns a pointer to the beginning of the allocated zone.

- **mem_init**: We initialize the memory and choose a specific function among `mem_fit_first`, `mem_fit_best`, and `mem_fit_worst`, each representing a different way to manage which free zone memory is allocated to. By default, we use `mem_fit_best`.

---

### Usage

- **mem_init(mem_addr, size_zone)**  
  Initializes the memory system of `size_zone` at the address `mem_addr`.

- **mem_alloc(size)**  
  Returns the address of a zone where data of up to `size` can be written.

- **mem_free(address)**  
  Frees the memory block at the `address` if it was occupied; does nothing otherwise. The passed address must be one returned by `mem_alloc`.

- **mem_show(print)**  
  Displays free and occupied memory zones on the screen using the provided `print` function.

---

### Tests Performed

**test_alloc.c**:  
- First, allocation of 10 zones of random sizes without overflow.  
- Memory reset, followed by an allocation of size zero.  
- Memory reset, followed by an allocation larger than the available size.  
  For the last two tests, the memory shown should be a single free zone the size of the memory.

**Test free**:  
- Allocation of several zones of random sizes, storing the addresses returned by `mem_alloc`.  
- Freeing every other zone in alternating fashion, then freeing the remaining zones, ensuring in the end that only one free block covering the entire memory remains.  
- We also tested freeing a free zone and then freeing a random address in our memory system using `memshell`.

---

### Makefile Commands

- **make**  
  Compiles `memshell.c` and all tests without running them.

- **make test**  
  Compiles and runs all the tests.

- **make clean**  
  Deletes compiled files.

- **make test_ls**  
  Tests our implementation using the `ls` shell function instead of the default library.

---

### Implementation Limits

- The 16-byte padding can cause significant memory waste if all allocated zones have a size congruent to 1 modulo 16. In this case, out of the 16 bytes allocated, only one would be used.

- Additionally, while our implementation allows for several ways to select allocation zones, it does not let the user choose this without modifying the code.
