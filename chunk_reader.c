#include "chunk_reader.h"
#include <stdlib.h>

ChunkReader* create_chunk_reader(const char* filename, size_t chunk_size) {
    ChunkReader* reader = (ChunkReader*)malloc(sizeof(ChunkReader));
    if (!reader) return NULL;
    reader->file = fopen(filename, "rb");
    if (!reader->file) {
        free(reader);
        return NULL;
    }
    reader->chunk_size = chunk_size;
    reader->total_read = 0;
    return reader;
}

void destroy_chunk_reader(ChunkReader* reader) {
    if (!reader) return;
    if (reader->file) fclose(reader->file);
    free(reader);
}

bool has_next_chunk(ChunkReader* reader) {
    if (!reader || !reader->file) return false;
    long current = ftell(reader->file);
    fseek(reader->file, 0, SEEK_END);
    long end = ftell(reader->file);
    fseek(reader->file, current, SEEK_SET);
    return current < end;
}

size_t read_next_chunk(ChunkReader* reader, int32_t* buffer) {
    if (!reader || !reader->file || !buffer) return 0;
    size_t read_count = fread(buffer, sizeof(int32_t), reader->chunk_size, reader->file);
    reader->total_read += read_count;
    return read_count;
}
