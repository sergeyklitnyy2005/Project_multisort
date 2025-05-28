#include "sort_worker.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

static int cmp_int(const void* a, const void* b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return (ia > ib) - (ia < ib);
}

void* sort_worker_run(void* arg) {
    SortWorkerArgs* args = (SortWorkerArgs*)arg;
    while (1) {
        ChunkJob* job = queue_pop(args->input_queue);
        if (!job) break;

        qsort(job->data, job->size, sizeof(int), cmp_int);

        char filename[512];
        snprintf(filename, sizeof(filename), "%s/chunk_%d_%d.bin", args->temp_dir, args->thread_id, job->index);

        FILE* f = fopen(filename, "wb");
        if (!f) {
            perror("Failed to open chunk file for writing");
            free(job->data);
            free(job);
            continue;
        }

        fwrite(job->data, sizeof(int), job->size, f);
        fclose(f);

        scm_add_chunk(args->manager, strdup(filename));

        free(job->data);
        free(job);
    }
    return NULL;
}
