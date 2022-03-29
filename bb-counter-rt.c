#include <stdio.h>
// #include <vector.h>
int count = 0;

void bbCounter(char * name) {
    count++;
    printf("bbCounter called from %s, count = %d\n", name, count);
}
