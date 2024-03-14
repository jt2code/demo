#include <stdarg.h>
#include <stdio.h>

void print_strings(char *str, ...)
{
    va_list ap;
    va_start(ap, str);
    for (char *s = str; s != NULL; s = va_arg(ap, char *))
        printf("%s\n", s);
    va_end(ap);
}

void myprintf(char *fmt, ...)
{
    int d;
    char c, *s;
    va_list ap;
    va_start(ap, fmt);
    for (int i = 0; fmt[i] != '\0'; i++) {
        switch (fmt[i]) {
        case 'c':
            c = (char)va_arg(ap, int);
            printf("(fmt[%d] <char>: %c)\n", i, c);
            break;
        case 'd':
            d = va_arg(ap, int);
            printf("(fmt[%d] <int>: %d)\n", i, d);
            break;
        case 's':
            s = va_arg(ap, char *);
            printf("(fmt[%d] <char*>: %s)\n", i, s);
        }
    }
    va_end(ap);
}

int main(void)
{
    myprintf("ccsdd", 'x', 'y', "i'm fine", 321, 456);

    /* arguments must be NULL terminated */
    print_strings("abc", "def", "ghi", "jkl", NULL);
    return 0;
}
