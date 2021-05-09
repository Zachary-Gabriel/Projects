#include <stdio.h>
#include "forced_assert.h"
#include <string.h>

#include "lp_utils.h"

// To speed up ballot generation and comparison, some dirty trick is used
// It may not work on machines with little(?) endian

#define PRINT_BALLOTS { \
        printf("[line:%d]\n", __LINE__); \
        ballot_to_binary(x, "x"); \
        ballot_to_binary(y, "y"); \
}

// typedef struct ballot_alias_t {
//     uint16_t seqnum;
//     uint8_t mcaster_id;
//     uint8_t mcaster_incarnation;
// } __attribute__((packed)) ballot_alias;

void ballot_to_binary(ballot_t b, char * name) {
// printf("%u\n", b);
    char str[64] = {'\0'};
    char * bal_bytes = (char*)&b;
    
    unsigned int i;
    str[0] = '\0';
    for(i = (sizeof(ballot_t) - 1); ; i--) {
        unsigned char c = bal_bytes[i];
        unsigned z;
        for (z = 256; z > 0; z >>= 1) {
            strcat(str, ((c & z) == z) ? "1" : "0");
        }
        if(i == 0) break;
        strcat(str, "|");
        // for(z = 8; z > 0; z--) {
        //     strcat(str, ((c & (1>>z)) == (1>>z)) ? "1" : "0");
        //     // str[z] = (c & (1>>z)) ? '1' : '0';
        // }
        
        // str[8] = '\0';
    }
    printf("%s: %s (%u)\n", name, str, b);
}


int main (int argc, char const *argv[]) {
    UNUSED_ARG(argc);
    UNUSED_ARG(argv);
    
    // ballot_to_binary(0);
    // ballot_to_binary(1);
    // ballot_to_binary(2);
    // ballot_to_binary(1234);

    unsigned int i;
    ballot_t x, y;

    x = make_first_ballot(0, 0);
    ballot_to_binary(x, "x");
    assert(x == 65536);
    
    x = make_first_ballot(1, 1);
    ballot_to_binary(x,"x");
    assert(x == (65536 + (1<<8) + 1));

    y = make_first_ballot(2, 1);
    ballot_to_binary(y, "y");
    assert(y == (65536 + (2<<8) + 1));
    
    PRINT_BALLOTS;
    assert(y > x);
        
    printf("Incrementing X\n");
    x = increment_ballot(x);
    PRINT_BALLOTS;
    assert(x > y);

    printf("Incrementing Y\n");
    y = increment_ballot(y);
    PRINT_BALLOTS;
    assert(y > x);
    
    for(i = 0; i < 100; i++) {
        y = increment_ballot(y);
        x = increment_ballot(x);
    }
    PRINT_BALLOTS;
    assert(y > x);    
    
    for(i = 0; i < 10; i++) {
        y = increment_ballot(y);
    }
    assert(y > x);
    x = raise_ballot(x, y);
    PRINT_BALLOTS;
    assert(x > y);
    
    y = raise_ballot(y, x);
    PRINT_BALLOTS;
    assert(y > x);
    
    for(i = 0; i < 10; i++) {
        x = increment_ballot(x);
    }
    assert(x > y);
    y = raise_ballot(y, x);
    PRINT_BALLOTS;
    assert(y > x);    

    printf("Test successful\n");
    return 0;
}
