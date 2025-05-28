//
// Created by ksv on 28.05.2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool is_sorted(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("fopen");
        return false;
    }

    int prev, curr;
    if (fread(&prev, sizeof(int), 1, file) != 1) {
        fclose(file);
        return true; // empty file is considered sorted
    }

    while (fread(&curr, sizeof(int), 1, file) == 1) {
        if (curr < prev) {
            fclose(file);
            return false;
        }
        prev = curr;
    }

    fclose(file);
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <sorted_file>\n", argv[0]);
        return 1;
    }

    if (is_sorted(argv[1])) {
        printf("File %s is sorted.\n", argv[1]);
    } else {
        printf("File %s is NOT sorted.\n", argv[1]);
    }

    return 0;
}
