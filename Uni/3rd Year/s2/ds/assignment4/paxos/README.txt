Contents:
1. About libPaxos and RingPaxos
2. Warning <- Very important!!!
3. Compiling
4. Configuring and launching
5. Example applications

Last update: 23 April 2010

-----------------------------------------------------------------------------
***      About libPaxos and RingPaxos                                     ***
-----------------------------------------------------------------------------

 - What is libPaxos?
libPaxos is a collection of open-source Paxos implementations.
Comments, suggestions are welcome on our mailing list available at
http://libpaxos.sourceforge.net

Paxos is a popular algorithm that can be used to solve consensus and 
implement Atomic Broadcast.
Atomic Broadcast is a powerful primitive for building fault-tolerant 
distributed systems. It ensures that some set of processes will receive a 
sequence of commands in the same order, despite failures and message loss.

The purpose of libPaxos is providing ready-to-use Atomic Broadcast
implementations for building dependable distributed systems.

At the moment, all the projects in libPaxos were developed at the University
of Lugano (http://www.inf.usi.ch).

 - What is RingPaxos?
RingPaxos is the latest addition to libPaxos, this particular implementation
targets high-throughput delivery. During our tests we managed to deliver up
to 900Mbit/s of data to each broadcast subscriber ("learners" in Paxos terms).

For more informations about the RingPaxos algorithm, you can read:
  "RingPaxos: A High-Throughput Atomic Broadcast Algorithm"
The paper will appear in the Proceedings of the 40th International Conference
on Dependable Systems and Networks (DSN-2010).
( http://scholar.google.com/scholar?hl=en&q=ringpaxos ) 


-----------------------------------------------------------------------------
***                           WARNING                                     ***
*** If you plan to this RingPaxos implementation, please read carefully!! ***
-----------------------------------------------------------------------------

The current implementation of RingPaxos is NOT complete, in particular the
"recovery" part is still under development.

The protocol is implemented completely, meaning that safety properties are
granted: two learners will never deliver messages in a different order.

However Liveness is not guaranteed. There is no failure detection
mechanism with consequent reconfiguration/recovery.
For example, if an acceptor crashes, the broadcast will stop.

We are currently working to fill-in the missing pieces. Although this is 
nothing exceptionally difficult, it may require some time...

You can check TODO.txt for a detailed list of the features that we intend to
implement in the near future.


-----------------------------------------------------------------------------
***      Compiling                                                        ***
-----------------------------------------------------------------------------

To build RingPaxos, run 'make' in the main directory.

Notice that you will need libevent (1.4.*) already built on your system.
http://www.monkey.org/~provos/libevent/
If the library is stored in a directory different than ~/libevent, you need 
to update the value of the LEV_DIR variable in Makefile.inc

To make sure everything went fine, try executing 'make run_test'.

Some compile-time variables (like verbosity and IP MTU) can be configured
by editing include/paxos_config.h before building.


-----------------------------------------------------------------------------
***      Configuring and launching                                        ***
-----------------------------------------------------------------------------

Processes in RingPaxos use a configuration file to learn about the topology
and set protocol parameters. 
See example_config.cfg for a commented example configuration file.

One of the mandatory sections in the configuration file, is the list of 
Paxos acceptors. 
When started, each acceptor needs to know who he is in that list.

The current acceptor (found in ring-paxos/acceptor after building) is for 
example launched as
$ ./ring-paxos/acceptor 1 ./example_config.cfg


To launch an example execution, 
$ cd ring-paxos/scripts/xterm_run_debug/
$ ./run_slow

This launches 2 learners, 3 acceptors and 1 proposer on localhost.

-----------------------------------------------------------------------------
***      Example applications                                             ***
-----------------------------------------------------------------------------

At the moment, the protocol is implemented almost entirely in the acceptors.
Any process that wants to send values through RingPaxos, can do so by 
submitting a correctly formatted message to the first acceptor in the ring.

For examples, refer to ring-paxos/client_proposer.c (an example of process
submitting values through RingPaxos) and ring-paxos/client_proposer_learner.c 
(an example of process submitting values and also delivering them).
ring-paxos/client_learner.c is instead just a learner, which will deliver
an ordered set of values.

For other questions and problem, don't hesitate to contact us through the
libPaxos mailing list, available at http://libpaxos.sourceforge.net
