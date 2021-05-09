#include <stdlib.h>

// This part is stubbed, no functionality is really implemented at the moment

struct learner_mngr_t;
typedef struct learner_mngr_t learner_mngr;

//Handling of learners requests (i.e. when there is a gap)
typedef void(*handle_learner_request_cb)(void*, size_t);

learner_mngr *
learner_mngr_TCP_init(
    char* addr_str,       /*Listen address (optional)*/
    int port,             /*Listen port port*/
    handle_learner_request_cb cb /*Callback for packet received*/
    );
