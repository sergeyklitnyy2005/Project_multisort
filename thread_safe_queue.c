#include "thread_safe_queue.h"
#include <stdlib.h>

void queue_init(ThreadSafeQueue* queue) {
    queue->head = NULL;
    queue->tail = NULL;
    queue->closed = 0;
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);
}

void queue_destroy(ThreadSafeQueue* queue) {
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->cond);
}

void queue_push(ThreadSafeQueue* queue, ChunkJob* job) {
    job->next = NULL;
    pthread_mutex_lock(&queue->mutex);
    if (queue->closed) {
        pthread_mutex_unlock(&queue->mutex);
        return;
    }
    if (queue->tail == NULL) {
        queue->head = job;
        queue->tail = job;
    } else {
        queue->tail->next = job;
        queue->tail = job;
    }
    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

ChunkJob* queue_pop(ThreadSafeQueue* queue) {
    pthread_mutex_lock(&queue->mutex);
    while (queue->head == NULL && !queue->closed) {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }
    if (queue->head == NULL && queue->closed) {
        pthread_mutex_unlock(&queue->mutex);
        return NULL;
    }
    ChunkJob* job = queue->head;
    queue->head = job->next;
    if (queue->head == NULL)
        queue->tail = NULL;
    pthread_mutex_unlock(&queue->mutex);
    return job;
}

void queue_close(ThreadSafeQueue* queue) {
    pthread_mutex_lock(&queue->mutex);
    queue->closed = 1;
    pthread_cond_broadcast(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}
