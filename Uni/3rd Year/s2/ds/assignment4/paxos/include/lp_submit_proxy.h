
// This object can be used by applications willing to submit values through Paxos

typedef struct submit_cmd_msg_t {
	size_t cmd_size;
	void * cmd_value[0];
} submit_cmd_msg;
#define SUBMIT_CMD_MSG_SIZE(M) (sizeof(submit_cmd_msg) + M->cmd_size)

typedef struct submit_proxy_t submit_proxy;


// Initialize a submit proxy
submit_proxy * submit_proxy_udp_init(const char * config_path);

// Submit a value to be delivered through atomic broadcast
void submit_command(submit_proxy * sp, submit_cmd_msg * scm);

// Normally the proxy tries to batch multiple values into a single packet
// this call forces to flush (send) the packet immediately
void submit_proxy_flush_now(submit_proxy * sp);
