#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 512
#define COUNT 1000000000

void clocktest() 
{
    struct timespec res;
    clock_getres(CLOCK_MONOTONIC_RAW, &res);
    
    if (res.tv_sec == 0 && res.tv_nsec <= 1000) 
        return;

    fprintf (stderr, "Clock resolution of %llu ns is too low\n", (uint64_t)res.tv_sec * 1000000000 + res.tv_nsec);
    exit (1);
}

uint64_t gettime() 
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC_RAW, &now);
    return ((uint64_t)(now.tv_sec))*1000000000 + now.tv_nsec;
}

void printtime(uint64_t time) 
{
    time += 500;
    printf("%llu.%06llu\n", time/1000000000, (time/1000)%1000000);
}

uint64_t prg() 
{
    static uint64_t state = 0xdeadbeef;
    uint64_t x = state;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    state = x;
    return x;
}

void main(int ac, char **av) {
    clocktest();

    if (ac != 2) 
    {
        fprintf (stderr, "Usage: %s <skip>\n", av[0]);
        exit (1);
    }
    
    int skip = atoi (av[1]);
    
    if (skip < 1 || skip > 8192) {
        fprintf (stderr, "Offset out of range (1-8192)\n");
        exit(1);
    }
    
    uint64_t *array = (uint64_t *) malloc (SIZE*sizeof (uint64_t) * skip);

    for (int i = 0; i < SIZE; i++)
        array[i * skip] = prg();

    uint64_t start = gettime();
    uint64_t s = 0;

    for (int i = 0; i < COUNT; i++) 
    {
        s ^= array[(s % SIZE) * skip];
    }
    
    uint64_t end = gettime();
    if (s != 0)
        printtime(end - start);
}
