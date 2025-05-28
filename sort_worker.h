#ifndef SORT_WORKER_H
#define SORT_WORKER_H

#include "thread_safe_queue.h"
#include "sorted_chunk_manager.h"

typedef struct {
    ThreadSafeQueue* input_queue;
    SortedChunkManager* manager;
    char* temp_dir;
    int thread_id;
} SortWorkerArgs;

void* sort_worker_run(void* args);

#endif
