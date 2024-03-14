#include "../include/utils.h"
#include <string.h>
#include <sys/wait.h>

/* output purple content */
void print_subproc_output(FILE *file)
{
    char buf[256];
    puts("\033[35m");
    while (!feof(file)) {
        bzero(buf, sizeof(buf));
        if (fread(buf, sizeof(buf) - 1, 1, file) == -1) {
            perror("fread");
            goto END;
        }
        printf("%s", buf);
    }
END:
    puts("\033[0m");
}

int main(void)
{
    FILE *file;
    file = popen("ps u 2>&1 | head -n 5", "r");
    ASSERT(file != NULL, "popen");
    print_subproc_output(file);
    /* must be used last */
    printf("exit status: %d\n", WEXITSTATUS(pclose(file)));
    return 0;
}