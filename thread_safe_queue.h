#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <pthread.h>

typedef struct ChunkJob {
    int* data;
    int size;
    int index;
    struct ChunkJob* next;
} ChunkJob;

typedef struct {
    ChunkJob* head;
    ChunkJob* tail;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int closed;
} ThreadSafeQueue;

void queue_init(ThreadSafeQueue* queue);
void queue_destroy(ThreadSafeQueue* queue);
void queue_push(ThreadSafeQueue* queue, ChunkJob* job);
ChunkJob* queue_pop(ThreadSafeQueue* queue);
void queue_close(ThreadSafeQueue* queue);

#endif
