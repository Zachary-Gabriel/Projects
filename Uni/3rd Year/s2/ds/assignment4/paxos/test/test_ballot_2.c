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

#define PROC_1 1, 0
#define PROC_2 1, 1
#define PROC_3 2, 0

int main (int argc, char const *argv[]) {
    UNUSED_ARG(argc);
    UNUSED_ARG(argv);
    
    // ballot_to_binary(0);
    // ballot_to_binary(1);
    // ballot_to_binary(2);
    // ballot_to_binary(1234);

    unsigned int i;
    ballot_t x, y, z;

    x = make_first_ballot(PROC_1);
    assert(is_my_ballot(x, PROC_1));
    
    y = make_first_ballot(PROC_2);
    assert(is_my_ballot(y, PROC_2));
    
    z = make_first_ballot(PROC_3);
    assert(is_my_ballot(z, PROC_3));


    assert(!is_my_ballot(x, PROC_2));
    assert(!is_my_ballot(z, PROC_2));
    assert(!is_my_ballot(y, PROC_1));
    
    for(i = 0; i < 10; i++) {
        x = increment_ballot(x);
        assert(is_my_ballot(x, PROC_1));
    }
    assert(!is_my_ballot(x, PROC_2));

    y = raise_ballot(y, x);
    assert(is_my_ballot(y, PROC_2));   

    printf("Test successful\n");
    return 0;
}
