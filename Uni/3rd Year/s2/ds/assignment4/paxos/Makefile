include ./Makefile.inc

all:  libevent_check $(LIBPAXOS) $(MODULES) ring-paxos
	@echo "Build completed"

again: 
	$(QUIET) $(MAKE) clean
	$(QUIET) $(MAKE) all

run_test:
	./scripts/local/run_all_tests.sh

.PHONY: $(MODULES) bdb_check libevent_check again run_test ring-paxos

$(MODULES):	
	$(QUIET) $(MAKE) --directory=$@ || exit 1;
	@echo "Done in $@/"

ring-paxos: $(LIBPAXOS)
	$(QUIET) $(MAKE) --directory=$@ || exit 1;
	@echo "Done in $@/"


$(LIBPAXOS): lib 
	$(QUIET) $(AR) $(ARFLAGS) $@ $(addsuffix /*.o, $^)
	@echo "Libpaxos.a done."

bdb_check:
	$(QUIET) if [ ! -e $(BDB_DIR) ]; then \
	echo "Error: Berkeley DB not found in $(BDB_DIR)"; \
	echo "You must build BDB and update the BDB_DIR variable in Makefile.inc before proceeding"; \
	exit 1; \
	fi
	@echo "BDB found"


libevent_check:
	$(QUIET) if [ ! -e $(LEV_DIR) ]; then \
	echo "Error: LibEvent not found in $(LEV_DIR)"; \
	echo "You must build LibEvent and update the LEV_DIR variable in Makefile.inc before proceeding"; \
	exit 1; \
	fi
	@echo "LibEvent found"

clean:
	@echo "Cleaning $(LIBPAXOS)"
	$(QUIET) rm -f $(LIBPAXOS)
	$(QUIET) for m in $(MODULES); do \
		echo "Cleaning $$m"; \
		$(MAKE) clean --directory=$$m; \
	done
	@echo "Cleaning ring-paxos"
	$(QUIET) $(MAKE) clean --directory=ring-paxos;
