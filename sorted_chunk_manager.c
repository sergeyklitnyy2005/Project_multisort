#include "sorted_chunk_manager.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define INITIAL_CAPACITY 16

void scm_init(SortedChunkManager* manager) {
    manager->chunk_files = malloc(sizeof(char*) * INITIAL_CAPACITY);
    manager->capacity = INITIAL_CAPACITY;
    manager->count = 0;
}

void scm_add_chunk(SortedChunkManager* manager, char* chunk_file) {
    if (manager->count == manager->capacity) {
        manager->capacity *= 2;
        manager->chunk_files = realloc(manager->chunk_files, sizeof(char*) * manager->capacity);
    }
    manager->chunk_files[manager->count++] = chunk_file;
}

char** scm_get_all_chunks(SortedChunkManager* manager, size_t* out_count) {
    *out_count = manager->count;
    return manager->chunk_files;
}

void scm_destroy(SortedChunkManager* manager) {
    for (int i = 0; i < manager->count; i++) {
        free(manager->chunk_files[i]);
    }
    free(manager->chunk_files);
    manager->chunk_files = NULL;
    manager->count = 0;
    manager->capacity = 0;
}
