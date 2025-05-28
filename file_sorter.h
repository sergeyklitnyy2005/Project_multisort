#ifndef FILE_SORTER_H
#define FILE_SORTER_H
#include "thread_safe_queue.h"

#include <stdio.h>

typedef struct {
    FILE* file;
    int* buffer;
    int buffer_size;
    int max_numbers;
} ChunkReader;



void chunk_reader_init(ChunkReader* reader, const char* filename, int max_numbers);
int chunk_reader_has_next(ChunkReader* reader);
int chunk_reader_read_next(ChunkReader* reader, int* buffer);
void chunk_reader_free(ChunkReader* reader);

void file_sorter_run(const char* input_file, const char* output_file, int max_numbers, const char* temp_dir, int thread_count);

#endif
