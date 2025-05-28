#include "merger.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    FILE* file;
    int value;
    int is_valid;
} ChunkReader;

void merge_chunks(const char** chunk_files, int num_chunks, const char* output_filename) {
    ChunkReader* readers = malloc(sizeof(ChunkReader) * num_chunks);
    for (int i = 0; i < num_chunks; i++) {
        readers[i].file = fopen(chunk_files[i], "rb");
        if (!readers[i].file) {
            perror("Failed to open chunk file");
            for (int j = 0; j < i; j++) fclose(readers[j].file);
            free(readers);
            return;
        }
        readers[i].is_valid = fread(&readers[i].value, sizeof(int), 1, readers[i].file) == 1;
    }

    FILE* output = fopen(output_filename, "wb");
    if (!output) {
        perror("Failed to open output file");
        for (int i = 0; i < num_chunks; i++) fclose(readers[i].file);
        free(readers);
        return;
    }
    int min_idx = -1;
    while (min_idx == -1) {
        int min_val = INT_MAX;
        min_idx = -1;
        for (int i = 0; i < num_chunks; i++) {
            if (readers[i].is_valid && readers[i].value < min_val) {
                min_val = readers[i].value;
                min_idx = i;
            }
        }

        fwrite(&min_val, sizeof(int), 1, output);

        readers[min_idx].is_valid = fread(&readers[min_idx].value, sizeof(int), 1, readers[min_idx].file) == 1;
    }

    for (int i = 0; i < num_chunks; i++) {
        fclose(readers[i].file);
    }
    fclose(output);
    free(readers);
}
