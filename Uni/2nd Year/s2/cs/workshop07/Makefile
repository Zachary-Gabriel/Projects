include Makefile-extras

ifeq (x$(CS_ARCH),x)
    $(error Environment variable CS_ARCH is not set - check the Makefiles!!!)
endif

# C++ 11 just in case
CXXFLAGS=--std=c++11 -I. -Iincludes

# must specify make target
all: test

.PHONY: parser translator
# compile but do not test
notest: parser translator

# testing student code
test: test-parser test-parser-sim test-translator test-parser-translator

test-parser: parser
	@bash bin/run-tests parser

test-parser-sim: parser
	@bash bin/run-tests parser-sim

test-translator: translator
	@bash bin/run-tests translator

test-parser-translator: parser translator
	@bash bin/run-tests parser-translator

# testing "working" code
test-working: test-working-parser test-working-translator

test-working-parser:
	@bash bin/run-tests working-parser

test-working-translator:
	@bash bin/run-tests working-translator

# add missing expected test outputs
test-add: test-add-Pxml test-add-sim

test-add-Pxml:
	@bash bin/run-tests add-Pxml quiet

test-add-sim:
	@bash bin/run-tests add-sim quiet


# regenerate all expected test outputs
test-new: test-new-Pxml test-new-sim

test-new-Pxml:
	@bash bin/run-tests new-Pxml quiet

test-new-sim:
	@bash bin/run-tests new-sim quiet


clean:
	rm -f lib/*/parser lib/*/translator

parser: lib/$(CS_ARCH)/parser

lib/$(CS_ARCH)/parser: parser.cpp lib/$(CS_ARCH)/lib.a
	${CXX} ${CXXFLAGS} -o $@ $^

translator: lib/$(CS_ARCH)/translator

lib/$(CS_ARCH)/translator: translator.cpp lib/$(CS_ARCH)/lib.a
	${CXX} ${CXXFLAGS} -o $@ $^
