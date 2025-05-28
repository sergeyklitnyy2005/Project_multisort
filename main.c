#include "file_sorter.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define _POSIX_C_SOURCE 199309L

double get_time_sec() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        printf("Usage: %s <input_file> <output_file> <temp_dir> <max_numbers_in_memory>\n", argv[0]);
        return 1;
    }

    const char* input_file = argv[1];
    const char* output_file = argv[2];
    const char* temp_dir = argv[3];
    int max_numbers_in_memory = atoi(argv[4]);

    int thread_count;
    printf("Enter number of threads to use: ");
    if (scanf("%d", &thread_count) != 1 || thread_count <= 0) {
        printf("Invalid thread count.\n");
        return 1;
    }

    printf("Starting external sort with %d threads...\n", thread_count);

    double start_time = get_time_sec();

    file_sorter_run(input_file, output_file, max_numbers_in_memory, temp_dir, thread_count);

    double end_time = get_time_sec();

    printf("Sorting completed in %.2f seconds.\n", end_time - start_time);

    return 0;
}
