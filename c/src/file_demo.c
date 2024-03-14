#include <fcntl.h>
#include <stdio.h>

/**
 *  ┌─────────────┬───────────────────────────────┐
 *  │fopen() mode │ open() flags                  │
 *  ├─────────────┼───────────────────────────────┤
 *  │     r       │ O_RDONLY                      │
 *  ├─────────────┼───────────────────────────────┤
 *  │     w       │ O_WRONLY | O_CREAT | O_TRUNC  │
 *  ├─────────────┼───────────────────────────────┤
 *  │     a       │ O_WRONLY | O_CREAT | O_APPEND │
 *  ├─────────────┼───────────────────────────────┤
 *  │     r+      │ O_RDWR                        │
 *  ├─────────────┼───────────────────────────────┤
 *  │     w+      │ O_RDWR | O_CREAT | O_TRUNC    │
 *  ├─────────────┼───────────────────────────────┤
 *  │     a+      │ O_RDWR | O_CREAT | O_APPEND   │
 *  └─────────────┴───────────────────────────────┘
 */

int fread_demo(char *filename)
{
    FILE *file;
    char buf[64];
    if ((file = fopen(filename, "r")) == NULL) {
        perror("fopen");
        return -1;
    }
    while (!feof(file) && !ferror(file)) {
        int rn = fread(buf, sizeof(char), sizeof(buf) - 1, file);
        buf[rn] = '\0';
        printf("%s", buf);
    }
    fclose(file);
    return 0;
}

int fwrite_demo(char *filename, char *cont, size_t cont_size)
{
    if (cont == NULL)
        return -1;
    FILE *file;
    if ((file = fopen(filename, "a")) == NULL) {
        perror("fopen");
        return -1;
    }
    fwrite(cont, sizeof(char), cont_size, file);
    fclose(file);
    return 0;
}
