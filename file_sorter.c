#include "file_sorter.h"
#include "thread_safe_queue.h"
#include "sort_worker.h"
#include "sorted_chunk_manager.h"
#include "merger.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

// Инициализация ChunkReader, открываем файл в бинарном режиме
void chunk_reader_init(ChunkReader* reader, const char* filename, int max_numbers) {
    reader->file = fopen(filename, "rb");
    if (!reader->file) {
        perror("Failed to open input file");
        exit(EXIT_FAILURE);
    }
    reader->max_numbers = max_numbers;
    reader->buffer = malloc(sizeof(int) * max_numbers);
    reader->buffer_size = 0;
}

// Возвращаем 1, если есть данные для чтения, 0 — если достигнут конец файла
int chunk_reader_has_next(ChunkReader* reader) {
    if (feof(reader->file)) return 0;
    // Проверяем, не дошли ли до конца файла после следующего чтения
    int pos = ftell(reader->file);
    int dummy;
    size_t read = fread(&dummy, sizeof(int), 1, reader->file);
    if (read == 0) {
        return 0;
    }
    // Вернём указатель обратно
    fseek(reader->file, pos, SEEK_SET);
    return 1;
}

// Читаем до max_numbers чисел из бинарного файла, возвращаем количество прочитанных
int chunk_reader_read_next(ChunkReader* reader, int* buffer) {
    return fread(buffer, sizeof(int), reader->max_numbers, reader->file);
}

// Очистка ресурсов
void chunk_reader_free(ChunkReader* reader) {
    if (reader->file) fclose(reader->file);
    free(reader->buffer);
}

void file_sorter_run(const char* input_file, const char* output_file, int max_numbers, const char* temp_dir, int thread_count) {
    struct timeval start, end;
    gettimeofday(&start, NULL);

    ChunkReader reader;
    chunk_reader_init(&reader, input_file, max_numbers);

    ThreadSafeQueue queue;
    queue_init(&queue);

    SortedChunkManager manager;
    scm_init(&manager);

    pthread_t* threads = malloc(sizeof(pthread_t) * thread_count);
    SortWorkerArgs* args = malloc(sizeof(SortWorkerArgs) * thread_count);

    for (int i = 0; i < thread_count; ++i) {
        args[i].input_queue = &queue;
        args[i].manager = &manager;
        args[i].temp_dir = (char*)temp_dir;
        args[i].thread_id = i;
        pthread_create(&threads[i], NULL, sort_worker_run, &args[i]);
    }

    int chunk_index = 0;
    while (chunk_reader_has_next(&reader)) {
        int* chunk = malloc(sizeof(int) * (max_numbers + 1));
        int count = chunk_reader_read_next(&reader, chunk + 1);
        if (count == 0) {
            free(chunk);
            break;
        }
        chunk[0] = count; // store count in first element
        ChunkJob* job = malloc(sizeof(ChunkJob));
        job->data = chunk;
        job->size = count;
        job->index = chunk_index++;
        queue_push(&queue, job);
    }

    queue_close(&queue);

    for (int i = 0; i < thread_count; ++i)
        pthread_join(threads[i], NULL);

    // Получить список отсортированных чанков и передать в мерджер
    size_t num_chunks;
    char** chunk_files = scm_get_all_chunks(&manager, &num_chunks);
    merge_chunks((const char**)chunk_files, (int)num_chunks, output_file);

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
    printf("Total time: %.2f ms\n", elapsed);

    free(threads);
    free(args);
    chunk_reader_free(&reader);
    scm_destroy(&manager);
    queue_destroy(&queue);
}
