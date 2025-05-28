//
// Created by ksv on 28.05.2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <output_file> <count>\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];
    int count = atoi(argv[2]);

    FILE* file = fopen(filename, "wb");
    if (!file) {
        perror("fopen");
        return 1;
    }

    srand(time(NULL));
    fwrite(&count, sizeof(int), 1, file);
    for (int i = 0; i < count; ++i) {
        int num = rand();
        fwrite(&num, sizeof(int), 1, file);
    }

    fclose(file);
    printf("Generated %d integers to %s\n", count, filename);
    return 0;
}
