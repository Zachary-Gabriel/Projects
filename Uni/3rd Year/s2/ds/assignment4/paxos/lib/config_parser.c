#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <strings.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/sysctl.h>

#include "paxos_config.h"
#include "lp_utils.h"
#include "lp_os_dependent.h"
#include "lp_config_parser.h"
#include "lp_config_parser_macros.h"

struct acceptor_info_t {
    acceptor_id_t id;
    char ip_addr[16];
    int inbound_port;
    int learners_port;
};

struct config_mngr_t {
	
    conf_change_callback cb;
    acceptor_id_t self_acceptor_id;

    int acceptors_count;
    
    struct timeval p1_interval;
    struct timeval p2_interval;
	struct timeval delivery_check_interval;
	struct timeval mcaster_clock_interval;
	int default_autoflush_interval;
	struct timeval retransmit_request_interval;
	
    int socket_buffers_size;
    
    char mcast_addr[16];
    int mcast_port;
    
    unsigned quorum_size;
	int working_set_size;
	int max_active_instances;
	int preexecution_window_size;
	int max_p2_open_per_iteration;

	int max_client_values_queue_size;
    
    acceptor_info acc_infos[MAX_ACCEPTORS];
};

static int validate_config(config_mngr * cm);
static config_mngr * open_and_parse(const char * config_path, config_mngr * cm);

/**** INSTANCE ****/
// This is a 'singleton' instance of the configuration manager
static config_mngr * singleton_config_manager = NULL;
config_mngr * lpconfig_get_manager_instance() { return singleton_config_manager; }

void lpconfig_destroy() {
    if(singleton_config_manager != NULL) {
        free(lpconfig_get_manager_instance());
    }
    singleton_config_manager = NULL;
}

CONF_GETTER(self_acceptor_id, acceptor_id_t);
CONF_GETTER(acceptors_count, int);

CONF_GETTER_P(p1_interval, struct timeval *);
CONF_GETTER_P(p2_interval, struct timeval *);
CONF_GETTER_P(delivery_check_interval, struct timeval *);
CONF_GETTER_P(mcaster_clock_interval, struct timeval *);
CONF_GETTER(default_autoflush_interval, int);
CONF_GETTER_P(retransmit_request_interval, struct timeval *);

CONF_GETTER(socket_buffers_size, int);


CONF_GETTER(mcast_addr, char *);
CONF_GETTER(mcast_port, int);

CONF_GETTER(quorum_size, unsigned);

CONF_GETTER(working_set_size, int);
CONF_GETTER(max_active_instances, int);
CONF_GETTER(preexecution_window_size, int);

CONF_GETTER(max_p2_open_per_iteration, int);
CONF_GETTER(max_client_values_queue_size, int);


acceptor_info * lpconfig_get_acceptor_info(acceptor_id_t acceptor) {
    config_mngr * cm = lpconfig_get_manager_instance();
    assert(acceptor <= MAX_ACCEPTORS);    
    acceptor_info * ai = &cm->acc_infos[acceptor];
    assert(ai->id <= MAX_ACCEPTORS);
    return ai;
}

char * lpconfig_get_self_ip_addr() {
    return (lpconfig_get_acceptor_info(lpconfig_get_self_acceptor_id()))->ip_addr;
}

char * lpconfig_get_ring_inbound_addr() {
    return lpconfig_get_self_ip_addr();
}

int lpconfig_get_ring_inbound_port() {
    config_mngr * cm = lpconfig_get_manager_instance();
    acceptor_info * my_info = &cm->acc_infos[lpconfig_get_self_acceptor_id()];
    return my_info->inbound_port;  
}

char * lpconfig_get_learners_inbound_addr() {
    return lpconfig_get_self_ip_addr();
}

int lpconfig_get_learners_inbound_port() {
    config_mngr * cm = lpconfig_get_manager_instance();
    acceptor_info * my_info = &cm->acc_infos[lpconfig_get_self_acceptor_id()];
    return my_info->learners_port;  
}

char * lpconfig_get_ip_addr_of(acceptor_id_t acceptor) {
    return (lpconfig_get_acceptor_info(acceptor))->ip_addr;
}

int lpconfig_get_learners_port_of(acceptor_id_t acceptor) {
    return (lpconfig_get_acceptor_info(acceptor))->learners_port;
}

int lpconfig_get_ring_port_of(acceptor_id_t acceptor) {
    return (lpconfig_get_acceptor_info(acceptor))->inbound_port;
}

/**** PUBLIC ****/

int
config_mngr_init(
    const char * config_path,        /*Path to config file*/
    acceptor_id_t acceptor_id, /*Numeric ID of this acceptor*/
    conf_change_callback cb    /*Callback for config update*/
    )
    {
        // Current singleton instance should be null!
        assert(singleton_config_manager == NULL);

        // config_mngr struct stores current values
        config_mngr * cm = calloc(1, sizeof(config_mngr));
        assert(cm != NULL);

        // Store callback triggered on config. change
        if (cb == NULL) 
            LOG_MSG(WARNING, ("Warning: Callback for configuration change is null\n"));
        
        cm->cb = cb;
        cm->self_acceptor_id = acceptor_id;
        
        //Parse line by line
        cm = open_and_parse(config_path, cm);
        if(cm == NULL) {
            printf("Error: could not parse configuration file\n");
            return -1;
        }
        LOG_MSG(DEBUG, ("Configuration loaded, validating\n"));

        //Save instance
        singleton_config_manager = cm;
        
        //Check that all required fields are set 
        // to meaningful values
        
        if(validate_config(cm) != 0) {
            printf("Error: configuration file is invalid\n");
            singleton_config_manager = NULL;
            return -1;
        }

        LOG_MSG(DEBUG, ("Configuration is valid\n"));
        return 0;
    }

/**** HELPERS ****/
static bool starts_with(char * match, char * buffer) {
    int size = strlen(match);
    return (strncmp(match, buffer, size) == 0);
}

static bool add_acceptor_info(
    config_mngr * cm, 
    acceptor_id_t acc_id, 
    char * tmp_ip,
    int port_in, 
    int port_lea
    ) 
{
    //Check ID is a valid number
    if(acc_id <= 0 || acc_id > MAX_ACCEPTORS) {
        printf("Invalid acceptor ID: %d\n", acc_id);
        return false;
    }
    
    // Check IP is of a valid size
    if(strlen(tmp_ip) > 15) {
        printf("Invalid acceptor address: %s\n", tmp_ip);
        return false;
    }

    //Copy fields in the CM array of infos
    acceptor_info * ai = &cm->acc_infos[acc_id];
    ai->id = (acceptor_id_t)acc_id;
    ai->inbound_port = port_in;
    ai->learners_port = port_lea;
    memcpy(ai->ip_addr, tmp_ip, 16);
    
    cm->acceptors_count += 1;
    
    return true;
}

static config_mngr * open_and_parse(const char * config_path, config_mngr * cm) {
    int max_line_length = 1024;
    char LINE_BUFFER[max_line_length];
    char IGNOREBUFFER[max_line_length];
    
    //Open cfg file
	LOG_MSG(DEBUG, ("Opening:%s\n", config_path));
    FILE* fp;
    fp = fopen(config_path,"r");
	if (fp == NULL) {
		perror("fopen");
        printf("Error: could not load configuration file\n");
        free(cm);
        return NULL;
	}

    int LINE_NUMBER = 0;
	while(fgets(LINE_BUFFER, max_line_length, fp) != NULL) {
        LINE_NUMBER++;

        //Comment line        
		IGNORE_LINE_START_WITH("#");
		IGNORE_LINE_START_WITH("//");
		IGNORE_LINE_START_WITH("\n");
        
		PARSE_TIMEVAL(p1_interval);
		
		PARSE_TIMEVAL(p2_interval);
		
		PARSE_TIMEVAL(mcaster_clock_interval);

		PARSE_TIMEVAL(delivery_check_interval);
				
		PARSE_TIMEVAL(retransmit_request_interval);

		PARSE_INTEGER(default_autoflush_interval);

		PARSE_INTEGER(socket_buffers_size);
        
		PARSE_INTEGER(quorum_size);

		PARSE_INTEGER(working_set_size);
		
		PARSE_INTEGER(max_active_instances);
		
		PARSE_INTEGER(preexecution_window_size);
		
		PARSE_INTEGER(max_p2_open_per_iteration);

		PARSE_INTEGER(max_client_values_queue_size);
		
		// Multicast info line
		if(starts_with("multicast", LINE_BUFFER)) {
            // IP PORT
            sscanf(LINE_BUFFER, "%s %s %d", IGNOREBUFFER, cm->mcast_addr, &cm->mcast_port);
            LOG_MSG(INFO, ("Multicast address is %s:%d\n", 
                cm->mcast_addr, cm->mcast_port));
            continue;
        }

        // An acceptor info line
        if(starts_with("acceptor", LINE_BUFFER)) {
            acceptor_id_t acc_id = 0;
            char tmp_ip[16];
            int port1 = 0;
            int port2 = 0;
            // ID RING_IP RING_PORT LEARNERS_PORT
            sscanf(LINE_BUFFER, "%s %hho %s %d %d", IGNOREBUFFER, &acc_id, tmp_ip, &port1, &port2);
            LOG_MSG(INFO, ("Acceptor %u -> %s:%d %s:%d \n", 
                acc_id, tmp_ip, port1, tmp_ip, port2));
            if(add_acceptor_info(cm, acc_id, tmp_ip, port1, port2)) {
                continue;
            } else {
                goto ERROR_LABEL;
            }
        }
		
        //Line didn't match anything!
        goto ERROR_LABEL;

	}
    fclose(fp);
    return cm;
	
ERROR_LABEL:
    printf("Configuration parse error at line %d\n", LINE_NUMBER);
    printf("VVV\n%s^^^\n", LINE_BUFFER);
    printf("Error: could not load configuration file\n");

    fclose(fp);
    free(cm);
    return NULL;
}

static int validate_socket_buffer_size() {

	int value;
	
	//Read sockets check
	value = get_os_max_rbuf_size();
	if(value < 0) {
		printf("Error reading OS max socket read buffer size\n");
		return -1;
	}
	LOG_MSG(INFO, ("Max socket READ buffer size: %d\n", value));
	
	if(lpconfig_get_socket_buffers_size() > value) {
		printf("Error: the size of system socket buffers in the config file is larger than");
		printf(" the maximum allowed on this system.\n You can increase the maximum");
		printf(" using sysctl with key: %s\n", OS_MAX_SOCKET_RBUF_SIZE_STRING);
		return -1;
	}

	//Write sockets check
	value = get_os_max_wbuf_size();
	if(value < 0) {
		printf("Error reading OS max socket write buffer size\n");
		return -1;
	}
	LOG_MSG(INFO, ("Max socket WRITE buffer size: %d\n", value));
	
	if(lpconfig_get_socket_buffers_size() > value) {
		printf("Error: the size of system socket buffers in the config file is larger than");
		printf(" the maximum allowed on this system.\n You can increase the maximum");
		printf(" using sysctl with key: %s\n", OS_MAX_SOCKET_WBUF_SIZE_STRING);
		return -1;
	}
	
	return 0;
}

static int validate_config(config_mngr * cm) {

	//Multicaster address/port
	VALIDATE_ADDRESS(lpconfig_get_mcast_addr());
	VALIDATE_PORTNUMBER(lpconfig_get_mcast_port());

	// Multicaster time variables
	VALIDATE_TIMEVAL_NONZERO(lpconfig_get_p1_interval());
	VALIDATE_TIMEVAL_NONZERO(lpconfig_get_p2_interval());
	VALIDATE_TIMEVAL_NONZERO_OR_DEFAULT(lpconfig_get_mcaster_clock_interval(), 0, 100000);
	VALIDATE_TIMEVAL_GREATER_EQUAL_THAN(lpconfig_get_p2_interval(), lpconfig_get_mcaster_clock_interval());
	VALIDATE_TIMEVAL_GREATER_EQUAL_THAN(lpconfig_get_p1_interval(), lpconfig_get_mcaster_clock_interval());
	
	// Learners intervals
	VALIDATE_TIMEVAL_NONZERO_OR_DEFAULT(lpconfig_get_delivery_check_interval(), 1, 0);
	VALIDATE_TIMEVAL_NONZERO_OR_DEFAULT(lpconfig_get_retransmit_request_interval(), 1, 0);
	VALIDATE_TIMEVAL_GREATER_EQUAL_THAN(lpconfig_get_retransmit_request_interval(), lpconfig_get_delivery_check_interval());
		
	// Acceptors number and quorum size
	VALIDATE_INT_NONZERO(lpconfig_get_quorum_size());	
	VALIDATE_INT_COMPARISON(cm->acceptors_count, (int)lpconfig_get_quorum_size(), >=);

	// Network flush
	VALIDATE_INT_NONZERO_OR_DEFAULT(cm->default_autoflush_interval, 50);

    // Paxos-related parameters
	VALIDATE_INT_NONZERO_OR_DEFAULT(cm->working_set_size, 100);
	VALIDATE_INT_NONZERO_OR_DEFAULT(cm->max_active_instances, 25);
	VALIDATE_INT_NONZERO_OR_DEFAULT(cm->preexecution_window_size, 50);
	VALIDATE_INT_NONZERO_OR_DEFAULT(cm->max_p2_open_per_iteration, 10);
	VALIDATE_INT_NONZERO_OR_DEFAULT(cm->max_client_values_queue_size, 100);
	
	
	// Validate other params
	
	//Check default size of OS socket buffers
    if(lpconfig_get_socket_buffers_size() <= 0) {
		LOG_MSG(WARNING, ("WARNING: socket_buffers_size is not set in configuration!\n"));
		LOG_MSG(WARNING, ("Default value will be used\n"));
	} else if (validate_socket_buffer_size() == -1) {
        goto VALIDATE_ERROR_LABEL;		
	}
	
	//Make sure Working set size is big enough
	VALIDATE_INT_COMPARISON(lpconfig_get_working_set_size(), lpconfig_get_max_active_instances(), >);
	if (lpconfig_get_max_active_instances()*2 >= lpconfig_get_working_set_size()) {
		LOG_MSG(WARNING, ("WARNING: max_active_instances is too big w.r.t working_set_size\n"));
	}	
	if(lpconfig_get_working_set_size() <= 
		(lpconfig_get_preexecution_window_size() + lpconfig_get_max_active_instances())) {
	        printf("Error: working_set_size is smaller than preexecution_window_size + max_active_instances\n");
	        printf(" (You should increase the former (%d) or increase one of the latter (%d+%d))\n", 
				lpconfig_get_working_set_size(), lpconfig_get_preexecution_window_size(), 
				lpconfig_get_max_active_instances());
			goto VALIDATE_ERROR_LABEL;		
	}
	
	//Iterate over acceptor infos
    int i, count=0;
    acceptor_info * ai;
    for(i = 1; i < MAX_ACCEPTORS; i++) {
        ai = lpconfig_get_acceptor_info(i);
        
        if(ai->id == 0) {
            //This slot is empty, don't check it
            LOG_MSG(DEBUG, ("Skipped acceptor info slot %d\n", i))
            continue;
        }
        assert(i == ai->id);
        
        count++;
        
        //Check ip address validity
		VALIDATE_ADDRESS(lpconfig_get_ip_addr_of(i));
        
        //Port numbers below 1024 require superuser privileges
		VALIDATE_PORTNUMBER(lpconfig_get_learners_port_of(i));
		VALIDATE_PORTNUMBER(lpconfig_get_ring_port_of(i));
		LOG_MSG(DEBUG, ("Acceptor info slot %d is valid\n", i));
	}

    //Make sure there are no acceptors with the same ID
	VALIDATE_INT_COMPARISON(lpconfig_get_acceptors_count(), count, ==);
    
    //Validation successful
    return 0;
    
VALIDATE_ERROR_LABEL:
        free(cm);
        return -1;
}

uint8_t lpconfig_get_incarnation_number() {
    // TODO implement
    return 0;
}
