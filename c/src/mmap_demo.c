#include "../include/utils.h"
#include <fcntl.h>
#include <strings.h>
#include <sys/mman.h>

#define _DEMO_MAP_SIZE 64
#define _DEMO_MAP_SHARED_FILE "/tmp/myshmmap"

void anonymous_mmap_example(void)
{
    void *maddr;
    maddr = mmap(NULL, _DEMO_MAP_SIZE, PROT_READ | PROT_WRITE,
                 MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    ASSERT(maddr != MAP_FAILED, "mmap");

    const char buf[] = "helloworld";
    for (int i = 0; i < sizeof(buf); i++)
        *(((char *)maddr) + i) = buf[i];

    for (int i = 0; i < sizeof(buf); i++)
        printf("[%d] %p %c\n", i, &((char *)maddr)[i], ((char *)maddr)[i]);

    munmap(maddr, _DEMO_MAP_SIZE);
}

void shared_mmap_example(void)
{
    int fd;
    void *maddr;
    fd = open(_DEMO_MAP_SHARED_FILE, O_CREAT | O_EXCL | O_RDWR, 0666);
    if (fd != -1) {
        /* if create new file */
        ASSERT(ftruncate(fd, _DEMO_MAP_SIZE) != -1, "truncate");
    } else if (errno == EEXIST) {
        fd = open(_DEMO_MAP_SHARED_FILE, O_RDWR, 0666);
        ASSERT(fd != -1, "open");
    } else {
        FATAL("open");
    }

    maddr =
        mmap(NULL, _DEMO_MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    ASSERT(maddr != MAP_FAILED, "mmap");

    /* write to file */
    char buf[] = "helloworld";
    for (int i = 0; i < sizeof(buf); i++)
        *(((char *)maddr) + i) = buf[i];

    /* msync(maddr, _DEMO_MAP_SIZE, MS_SYNC); */
    munmap(maddr, _DEMO_MAP_SIZE);
    /* clean buf */
    bzero(buf, sizeof(buf));
    ASSERT(read(fd, buf, sizeof(buf)) != -1, "read");
    printf("read file: %s\n", buf);
    /* close file */
    close(fd);
}

int main(void)
{
    anonymous_mmap_example();
    shared_mmap_example();
    return 0;
}