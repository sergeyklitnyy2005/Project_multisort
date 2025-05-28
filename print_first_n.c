//
// Created by ksv on 28.05.2025.
//

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <filename> <count>\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];
    int count = atoi(argv[2]);

    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("fopen");
        return 1;
    }

    int value;
    fread(&value, sizeof(int), 1, file);
    for (int i = 0; i < count && fread(&value, sizeof(int), 1, file) == 1; ++i) {
        printf("%d ", value);
    }
    printf("\n");
    fclose(file);
    return 0;
}
