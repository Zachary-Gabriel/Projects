#ifndef LP_CONFIG_PARSER_H_FKVUY8M6
#define LP_CONFIG_PARSER_H_FKVUY8M6

#include <sys/time.h>

#include "paxos_config.h"

struct acceptor_info_t;
typedef struct acceptor_info_t acceptor_info;
struct config_mngr_t;
typedef struct config_mngr_t config_mngr;

//Prototype for callback invoked when the configuration file changes
//This is a stub, this functionality is still not completely implemented
typedef void (* conf_change_callback)(config_mngr *, config_mngr *);

//Initializes the configuration manager, responsible of parsing the configuration file
int 
config_mngr_init(
        const char * config_path,        /*Path to config file*/
        acceptor_id_t acceptor_id, /*Numeric ID of this acceptor*/
        conf_change_callback cb    /*Callback for config update*/
        );

// Destroys the current configuration manager
void lpconfig_destroy();

struct timeval * lpconfig_get_p1_interval();
struct timeval * lpconfig_get_p2_interval();
struct timeval * lpconfig_get_delivery_check_interval();
struct timeval * lpconfig_get_mcaster_clock_interval();
struct timeval * lpconfig_get_retransmit_request_interval();

//The following accessors are used to request the values
// parsed from the configuration file
int lpconfig_get_default_autoflush_interval();

int lpconfig_get_socket_buffers_size();

char * lpconfig_get_mcast_addr();
int lpconfig_get_mcast_port();

char * lpconfig_get_ring_inbound_addr();
int lpconfig_get_ring_inbound_port();

char * lpconfig_get_learners_inbound_addr();
int lpconfig_get_learners_inbound_port();
acceptor_id_t lpconfig_get_self_acceptor_id();
int lpconfig_get_acceptors_count();

char * lpconfig_get_ip_addr_of(acceptor_id_t acceptor);
int lpconfig_get_learners_port_of(acceptor_id_t acceptor);
int lpconfig_get_ring_port_of(acceptor_id_t acceptor);

uint8_t lpconfig_get_incarnation_number();

unsigned lpconfig_get_quorum_size();

int lpconfig_get_working_set_size();
int lpconfig_get_max_active_instances();
int lpconfig_get_preexecution_window_size();
int lpconfig_get_max_p2_open_per_iteration();

int lpconfig_get_max_client_values_queue_size();

#endif /* end of include guard: LP_CONFIG_PARSER_H_FKVUY8M6 */
