#include <stdio.h>

void bbCounter(char * name) {
    static int count = 0;
    count++;

    printf("bbCounter called from %s, count = %d\n", name, count);
}
