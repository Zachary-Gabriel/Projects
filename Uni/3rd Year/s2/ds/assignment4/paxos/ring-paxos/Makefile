SRCS		= acceptor.c client_learner.c client_proposer.c client_proposer_learner.c
APPS		= $(subst .c,,$(SRCS))

all: $(APPS)

include ../Makefile.inc

clean: 
	$(QUIET) rm -rf $(APPS) *.dSYM .deps