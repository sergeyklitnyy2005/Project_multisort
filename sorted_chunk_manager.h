#ifndef SORTED_CHUNK_MANAGER_H
#define SORTED_CHUNK_MANAGER_H
#include <stddef.h>

typedef struct {
    char** chunk_files;
    int capacity;
    int count;
} SortedChunkManager;

void scm_init(SortedChunkManager* manager);
void scm_add_chunk(SortedChunkManager* manager, char* chunk_file);
char** scm_get_all_chunks(SortedChunkManager* manager, size_t* out_count);
void scm_destroy(SortedChunkManager* manager);

#endif
