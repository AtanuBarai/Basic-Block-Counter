#include <stdio.h>

int adder(int limit)
{
    int num = 0;
    for(int i = 1; i <= limit; i++)
    {
        num += i;
    }
    for(int i = 1; i <= limit; i++)
    {
        num += i;
    }
    return num;
}

int multiplier(int limit)
{
    int num = 1;
    for(int i = 1; i <= limit; i++)
    {
        num *= i;
    }
    return num;
}


int main(int argc, const char** argv)
{
    printf("Adder result: %d\n", adder(10));
    printf("Multiplier result: %d\n", multiplier(10));
    return 0;
}
