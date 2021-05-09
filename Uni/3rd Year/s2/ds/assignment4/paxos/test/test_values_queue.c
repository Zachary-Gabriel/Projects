#include <string.h>
#include <stdio.h>
#include "forced_assert.h"
#include <stdlib.h>
#include <stdbool.h>

#include "lp_utils.h"
#include "lp_queue.h"

char * strings[] = {
    "1sdadasdasaasdasd",
    "2jnasd;jnasd;",
    "3falsdjfbnasodfuasdf",
    "4asdf;kjansdfajnsdfajnsdf",
    "5asdf;kjnasd;fkjansdfjansdfai4afj",
    "6a;sjvnas;dfjnasjnaorivoaeirnvafv`",
    "7a;fjnafjansdansdjfna",
    "8asd;jandfjana[alfdsknv;aodut",
    "9a;jnas;fjan",
    "10jsvlknsdsnfrvoiropi"
    };

int main (int argc, char const *argv[]) {
    UNUSED_ARG(argc);
    UNUSED_ARG(argv);

    //Create queue
    lpqueue * q = queue_init(10);
    assert(q != NULL);
    
    //Add 10 elements (fill the queue)
    bool success;
    unsigned int i;
    printf("\nEnqueue... ");
    for(i = 0; i < 10; i++) {
        printf("%u, ", i);
        success = queue_append(q, strings[i], strlen(strings[i]), true);
        assert(success == true);
    }
    
    //Should be full
    success = queue_append(q, strings[1], strlen(strings[1]), true);
    assert(success == false);
    
    //Pop 10 elements
    void * data;
    size_t size;
    printf("\nDequeue... ");
    for(i = 0; i < 10; i++) {
        printf("%u, ", i);
        success = queue_pop(q, &data, &size);
        assert(success == true);
        
        //Check that they are ok
        assert(size = strlen(strings[i]));
        assert(memcmp(data, strings[i], size) == 0);
        free(data);
    }
    
    //Should now be empty
    success = queue_pop(q, &data, &size);
    assert(success == VALUES_QUEUE_EMPTY);
    
    printf("\nTEST SUCCESSFUL!\n");
    return 0;
}
