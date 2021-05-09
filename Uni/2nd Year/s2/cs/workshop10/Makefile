include Makefile-extras

# C++ 11 just in case
CXXFLAGS=--std=c++11 -I. -Iincludes

# must specify make target
all: test 

notest: parser

.PHONY: parser

# testing student code
test: test-parser 

test-parser: parser
	@bash bin/run-tests parser

# testing "working" code
test-working: test-working-parser

test-working-parser:
	@bash bin/run-tests working-parser

# add missing expected test outputs
test-add: test-add-Pxml

test-add-Pxml:
	@bash bin/run-tests add-Pxml quiet

# regenerate all expected test outputs
test-new: test-new-Pxml

test-new-Pxml:
	@bash bin/run-tests new-Pxml quiet

clean:
	rm -f lib/*/parser

parser: lib/$(CS_ARCH)/parser

lib/$(CS_ARCH)/parser: parser.cpp lib/$(CS_ARCH)/lib.a
	${CXX} ${CXXFLAGS} -o $@ $^
