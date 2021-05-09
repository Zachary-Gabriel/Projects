#include "lp_delivery_repeat.h"
#include "lp_utils.h"

struct learner_mngr_t {
    char ip_str;
    int port;
    handle_learner_request_cb cb;
};


learner_mngr *
learner_mngr_TCP_init(
    char* addr_str,       /*Listen address (optional)*/
    int port,             /*Listen port port*/
    handle_learner_request_cb cb) /*Callback for packet received*/
{
	
	// TODO implement
	struct learner_mngr_t * lm = calloc(1, sizeof(struct learner_mngr_t));
	UNUSED_ARG(addr_str);
	UNUSED_ARG(port);
	UNUSED_ARG(cb);
	return lm;
}
