#ifndef CHUNK_READER_H
#define CHUNK_READER_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    FILE* file;
    size_t chunk_size;
    size_t total_read;
} ChunkReader;

ChunkReader* create_chunk_reader(const char* filename, size_t chunk_size);
void destroy_chunk_reader(ChunkReader* reader);
bool has_next_chunk(ChunkReader* reader);
size_t read_next_chunk(ChunkReader* reader, int32_t* buffer);

#endif
