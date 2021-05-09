#include <stdlib.h>
#include <assert.h>

#include "lp_utils.h"
#include "lp_topology.h"

// Description of the current topology
struct topology_info_t {
    void* placeholder;
};

struct topolo_mngr_t {
    topology_info * current_topology;
    topo_change_callback cb;
};

/*** HELPERS ***/

static struct topolo_mngr_t * singleton_topology_mngr = NULL;

topology_info * build_topology_info() {
	struct topology_info_t * ti = calloc(1, sizeof(struct topology_info_t));
    return ti;
    
}
/*** PUBLIC ***/

int topology_mngr_init(
        topo_change_callback on_topology_change /*Callback for ring update*/
    )
    {
		assert(singleton_topology_mngr == NULL); //Should not be instantiated multiple times!
		
        // topolo_mngr struct handles topology updates
        singleton_topology_mngr = calloc(1, sizeof(topolo_mngr));
        assert(singleton_topology_mngr != NULL);
        
        //Save callback for topology update
        singleton_topology_mngr->cb = on_topology_change;

        //Build default topology_info from config
        singleton_topology_mngr->current_topology = build_topology_info();
        assert(singleton_topology_mngr->current_topology != NULL);

        return 0;
    }

acceptor_id_t lptopo_get_successor_id() {
    acceptor_id_t successor = lpconfig_get_self_acceptor_id() + 1;
    if(successor > lpconfig_get_acceptors_count()) {
        assert (successor == lpconfig_get_acceptors_count() + 1);
        successor = 1;
    }
    return successor;
}

int lptopo_get_successor_port() {
    //TODO low_priority

    acceptor_id_t successor = lptopo_get_successor_id();
    return lpconfig_get_ring_port_of(successor);
}

char * lptopo_get_successor_addr() {
    //TODO low_priority

    acceptor_id_t successor = lptopo_get_successor_id();
    return lpconfig_get_ip_addr_of(successor);
}

char * lptopo_get_leader_addr() {
    //TODO low_priority

    return lpconfig_get_ip_addr_of(1);
}

int lptopo_get_leader_ring_port() {
    //TODO low_priority

    return lpconfig_get_ring_port_of(1);

}

int lptopo_get_leader_clients_port() {
    //TODO low_priority

    return lpconfig_get_learners_port_of(1);
}
