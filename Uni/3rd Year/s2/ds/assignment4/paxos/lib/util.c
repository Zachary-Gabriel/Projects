#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>

#include "lp_utils.h"
#include "paxos_config.h"


void print_cmdline_args(int argc, char const *argv[]) {

    printf("Executing: %s\n", argv[0]);
    int i;
    for(i = 1; i < argc; i++) {
        printf("argv[%d] = \"%s\"\n", i, argv[i]);
    }
    
}

void clean_exit(int sig) {
	UNUSED_ARG(sig);
	printf("Caught exit signal\n");
	exit(0);
}

void enable_ctrl_c_handler() {
	signal(SIGINT, clean_exit);
	
}
void validate_acceptor_args_or_quit(int argc, char const *argv[]) {
    // Right number of arguments?
    if(argc != 3) {
        printf("Error: wrong number of arguments (%d)\n", (argc-1));
        goto pusage;
    }
    
    // First arg is a number greater than zero?
    int i = atoi(argv[1]);
    if(i == 0) {
        printf("Error: %d is not valid acceptor ID\n", i);
        goto pusage;
    }

    // Second arg is existing file?
    struct stat conf_stats;
    i = stat(argv[2], &conf_stats);
    if(i < 0) {
        perror("Opening configuration file");
        goto pusage;
    } else if ((conf_stats.st_mode & S_IFREG) == 0) {
        printf("Error: %s is not a regular file (type:%lu)\n", argv[2], (long unsigned int)conf_stats.st_mode);
        goto pusage;
    }
    
    LOG_MSG(DEBUG, ("Arguments are valid\n"));
    return;
    
pusage:
    printf("Error: invalid arguments\n");
    printf("Usage: %s acceptor_numeric_id path_to_config_file\n", argv[0]);
    exit(1);
}

// typedef struct ballot_alias_t {
//     uint16_t seqnum;
//     uint8_t mcaster_id;
//     uint8_t mcaster_incarnation;
// } __attribute__((packed)) ballot_alias;

ballot_t 
__attribute__ ((warn_unused_result, __const__)) 
make_first_ballot(acceptor_id_t acc_id, unsigned incarnation) {
    ballot_t b = 0;
    b += 1 << 16;
    b += acc_id << 8;
    b += incarnation;
    return b;
}

// ballot_t raise_ballot(ballot_t current_b, ballot_t ballot_to_beat) {
//     while (current_b < ballot_to_beat) {
//         current_b = increment_ballot(current_b);
//     }
//     return current_b;
// }

ballot_t 
__attribute__ ((warn_unused_result, __const__))
raise_ballot(ballot_t current_b, ballot_t ballot_to_beat) {
    ballot_t new_b = (current_b & 0xFFFF);
    uint16_t new_seq = ((ballot_to_beat>>16) & 0xFFFF);
    new_seq += 1;
    new_b += (new_seq<<16);
    return new_b;
}

ballot_t 
__attribute__ ((warn_unused_result, __const__))
increment_ballot(ballot_t b) {
    ballot_t new_b = (b & 0xFFFF);
    uint16_t new_seqnum = ((b>>16) & 0xFFFF);
    new_seqnum += 1;
    new_b += (new_seqnum<<16);
    return new_b;
}

bool 
__attribute__ ((warn_unused_result, __const__))
is_my_ballot(ballot_t b, acceptor_id_t acc_id, unsigned incarnation) {
    uint16_t identificator = (acc_id<<8) + incarnation;
    return (b & 0xFFFF) == identificator;
}


void
clear_cmd_key(command_id * cmd_key) {
    cmd_key->mcaster_id = 0;
    cmd_key->mcaster_incarnation = 0;
    cmd_key->cmd_seqnum = 0;
}

char * print_cmd_key(command_id * cmd_key) {
	static char str[32];
	sprintf(str, "{%u:%u:%u}", cmd_key->mcaster_id, cmd_key->mcaster_incarnation, cmd_key->cmd_seqnum);
	return str;
}
