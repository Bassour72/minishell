// Memory tracking system for debugging leaks
// Add these to a debug.h header file

#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Memory tracking structures
typedef struct s_mem_block {
    void *ptr;
    size_t size;
    char *file;
    int line;
    char *func;
    struct s_mem_block *next;
} t_mem_block;

// Global memory tracking variables
extern t_mem_block *g_memory_blocks;
extern int g_total_allocations;
extern size_t g_total_memory;

// Memory tracking functions
void *debug_malloc(size_t size, const char *file, int line, const char *func);
void debug_free(void *ptr, const char *file, int line, const char *func);
void print_memory_leaks();
void track_memory_allocation(void *ptr, size_t size, const char *file, int line, const char *func);
void untrack_memory_allocation(void *ptr, const char *file, int line, const char *func);

// // Redefine malloc and free to use debug versions
 #define malloc(size) debug_malloc(size, __FILE__, __LINE__, __func__)
 #define free(ptr) debug_free(ptr, __FILE__, __LINE__, __func__)

#endif /* DEBUG_H */