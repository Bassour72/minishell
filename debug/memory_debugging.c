// // Implementation of memory tracking system
// // Save this as debug.c

// #include "../include/debug.h"

// // Initialize global variables
// t_mem_block *g_memory_blocks = NULL;
// int g_total_allocations = 0;
// size_t g_total_memory = 0;

// // Custom malloc that tracks allocations
// void *debug_malloc(size_t size, const char *file, int line, const char *func) {
//     void *ptr = malloc(size);
//     if (ptr != NULL) {
//         track_memory_allocation(ptr, size, file, line, func);
//     }
//     return ptr;
// }

// // Custom free that tracks deallocations
// void debug_free(void *ptr, const char *file, int line, const char *func) {
//     if (ptr != NULL) {
//         untrack_memory_allocation(ptr, file, line, func);
//         free(ptr);
//     }
// }

// // Track a memory allocation
// void track_memory_allocation(void *ptr, size_t size, const char *file, int line, const char *func) {
//     // Create new memory block tracking info
//     t_mem_block *new_block = (t_mem_block *)malloc(sizeof(t_mem_block));
//     if (!new_block) {
//         fprintf(stderr, "Failed to allocate memory for tracking block\n");
//         return;
//     }
    
//     // Fill in tracking info
//     new_block->ptr = ptr;
//     new_block->size = size;
//     new_block->file = strdup(file);
//     new_block->line = line;
//     new_block->func = strdup(func);
    
//     // Add to linked list of tracked blocks
//     new_block->next = g_memory_blocks;
//     g_memory_blocks = new_block;
    
//     // Update statistics
//     g_total_allocations++;
//     g_total_memory += size;
    
//     printf("[MEM] Allocated %zu bytes at %p (%s:%d in %s)\n", 
//            size, ptr, file, line, func);
// }

// // Remove a memory allocation from tracking
// void untrack_memory_allocation(void *ptr, const char *file, int line, const char *func) {
//     t_mem_block *curr = g_memory_blocks;
//     t_mem_block *prev = NULL;
    
//     // Find the block in our tracking list
//     while (curr != NULL) {
//         if (curr->ptr == ptr) {
//             // Update statistics
//             g_total_memory -= curr->size;
            
//             printf("[MEM] Freed %zu bytes at %p (originally from %s:%d in %s)\n", 
//                    curr->size, ptr, curr->file, curr->line, curr->func);
            
//             // Remove from linked list
//             if (prev == NULL) {
//                 g_memory_blocks = curr->next;
//             } else {
//                 prev->next = curr->next;
//             }
            
//             // Free tracking info
//             free(curr->file);
//             free(curr->func);
//             free(curr);
//             return;
//         }
        
//         prev = curr;
//         curr = curr->next;
//     }
    
//     // If we get here, we're trying to free memory we didn't allocate
//     fprintf(stderr, "[ERROR] Attempting to free untracked memory at %p (%s:%d in %s)\n", 
//             ptr, file, line, func);
// }

// // Print all current memory leaks
// void print_memory_leaks() {
//     printf("\n===== MEMORY LEAK REPORT =====\n");
    
//     if (g_memory_blocks == NULL) {
//         printf("No memory leaks detected! 🎉\n");
//         return;
//     }
    
//     t_mem_block *curr = g_memory_blocks;
//     int leak_count = 0;
//     size_t total_leaked = 0;
    
//     while (curr != NULL) {
//         printf("LEAK: %zu bytes at %p, allocated in %s:%d (%s)\n", 
//                curr->size, curr->ptr, curr->file, curr->line, curr->func);
        
//         leak_count++;
//         total_leaked += curr->size;
//         curr = curr->next;
//     }
    
//     printf("\nTotal: %d leaks, %zu bytes\n", leak_count, total_leaked);
//     printf("==============================\n");
// }

// Implementation of memory tracking system
// Save this as debug.c

#include "../include/debug.h"

// Initialize global variables
t_mem_block *g_memory_blocks = NULL;
int g_total_allocations = 0;
size_t g_total_memory = 0;
static int g_next_id = 1;

// Custom malloc that tracks allocations
void *debug_malloc(size_t size, const char *file, int line, const char *func) {
    void *ptr = malloc(size);
    if (ptr != NULL) {
        track_memory_allocation(ptr, size, file, line, func);
    }
    return ptr;
}

// Custom free that tracks deallocations
void debug_free(void *ptr, const char *file, int line, const char *func) {
    if (ptr != NULL) {
        if (untrack_memory_allocation(ptr, file, line, func)) {
            free(ptr);
        }
    }
}

// Track a memory allocation
void track_memory_allocation(void *ptr, size_t size, const char *file, int line, const char *func) {
    // Create new memory block tracking info
    t_mem_block *new_block = (t_mem_block *)malloc(sizeof(t_mem_block));
    if (!new_block) {
        fprintf(stderr, "[ERROR] Failed to allocate memory for tracking block\n");
        return;
    }
    
    // Fill in tracking info
    new_block->ptr = ptr;
    new_block->size = size;
    new_block->file = strdup(file);
    new_block->line = line;
    new_block->func = strdup(func);
    new_block->id = g_next_id++;
    
    // Add to linked list of tracked blocks
    new_block->next = g_memory_blocks;
    g_memory_blocks = new_block;
    
    // Update statistics
    g_total_allocations++;
    g_total_memory += size;
    
    printf("[MEM] Allocated %zu bytes at %p (ID: %d, %s:%d in %s)\n", size, ptr, new_block->id, file, line, func);
}

// Remove a memory allocation from tracking
int untrack_memory_allocation(void *ptr, const char *file, int line, const char *func) {
    t_mem_block *curr = g_memory_blocks;
    t_mem_block *prev = NULL;
    
    // Find the block in our tracking list
    while (curr != NULL) {
        if (curr->ptr == ptr) {
            // Update statistics
            g_total_memory -= curr->size;
            
            printf("[MEM] Freed %zu bytes at %p (ID: %d, originally from %s:%d in %s)\n", 
                   curr->size, ptr, curr->id, curr->file, curr->line, curr->func);
            
            // Remove from linked list
            if (prev == NULL) {
                g_memory_blocks = curr->next;
            } else {
                prev->next = curr->next;
            }
            
            // Free tracking info
            free(curr->file);
            free(curr->func);
            free(curr);
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }
    
    // If we get here, we're trying to free memory we didn't allocate
    fprintf(stderr, "[ERROR] Attempting to free untracked memory at %p (%s:%d in %s)\n", 
            ptr, file, line, func);
    print_memory_leaks(); // Print all leaks to help debug
    return 0;
}

// Print all current memory leaks
void print_memory_leaks() {
    printf("\n===== MEMORY LEAK REPORT =====\n");
    
    if (g_memory_blocks == NULL) {
        printf("No memory leaks detected! 🎉\n");
        return;
    }
    
    t_mem_block *curr = g_memory_blocks;
    int leak_count = 0;
    size_t total_leaked = 0;
    
    while (curr != NULL) {
        printf("LEAK: %zu bytes at %p (ID: %d, allocated in %s:%d (%s))\n", 
               curr->size, curr->ptr, curr->id, curr->file, curr->line, curr->func);
        
        leak_count++;
        total_leaked += curr->size;
        curr = curr->next;
    }
    
    printf("\nTotal: %d leaks, %zu bytes\n", leak_count, total_leaked);
    printf("==============================\n");
}