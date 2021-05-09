#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lp_utils.h"
#include "lp_queue.h"
#include "forced_assert.h"

char * strings[] = {
    "1sdadasdasaasdasd",
    "2jnasd;jnasd;"
    };

int main (int argc, char const *argv[]) {
    UNUSED_ARG(argc);
    UNUSED_ARG(argv);

    //Create queue
    lpqueue * q = queue_init(3);
    assert(q != NULL);

    bool success;
    void * data;
    size_t size;

    //Add element
	printf("app1\n");
    success = queue_append(q, strings[0], strlen(strings[0]), true);
    assert(success == true);

    //Remove it
	printf("dis1\n");
    success = queue_discard_last(q);
    assert(success == true);
    
    //Queue should be empty
	printf("pop1\n");
    success = queue_pop(q, &data, &size);
    assert(success == VALUES_QUEUE_EMPTY);
    
    // fill the queue
	printf("app2\n");
    success = queue_append(q, strings[0], strlen(strings[0]), true);
    assert(success == true);

	printf("app3\n");
    success = queue_append(q, strings[0], strlen(strings[0]), true);
    assert(success == true);

	printf("app3\n");
    success = queue_append(q, strings[1], strlen(strings[1]), true);
    assert(success == true);
    
    //Element should not fit
	printf("app4\n");
    success = queue_append(q, strings[0], strlen(strings[0]), true);
    assert(success == false);

    //Drop last appended, and replace it
	printf("pre1\n");
    success = queue_prepend(q, strings[0], strlen(strings[0]), true);
    assert(success == true);
    
    //Queue should contains 3 times string 0 now
    unsigned int i;
    for(i = 0; i < 3; i++) {
        printf("%u\n", i);
        success = queue_pop(q, &data, &size);
        assert(success == true);
        
        //Check that they are ok
        assert(size = strlen(strings[0]));
        assert(memcmp(data, strings[0], size) == 0);
        free(data);
    }

    //Queue should be empty
	printf("pop2\n");
    success = queue_pop(q, &data, &size);
    assert(success == VALUES_QUEUE_EMPTY);

    printf("\nTEST SUCCESSFUL!\n");
    return 0;
}
