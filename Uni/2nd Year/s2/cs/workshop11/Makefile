include Makefile-extras

# C++ 11 just in case
CXXFLAGS=--std=c++11 -I. -Iincludes

# must specify make target
all: test

notest: translator

.PHONY: translator

# testing student code
test: test-translator

test-translator: translator
	@bash bin/run-tests translator

# testing "working" code
test-working: test-working-translator

test-working-translator:
	@bash bin/run-tests working-translator

# add missing expected test outputs
test-add: test-add-Pxml test-add-Pxml-vm

test-add-Pxml:
	@bash bin/run-tests add-Pxml quiet

test-add-Pxml-vm:
	@bash bin/run-tests add-Pxml-vm quiet

# regenerate all expected test outputs
test-new: test-new-Pxml test-new-Pxml-vm

test-new-Pxml:
	@bash bin/run-tests new-Pxml quiet

test-new-Pxml-vm:
	@bash bin/run-tests new-Pxml-vm quiet

clean:
	rm -f lib/*/translator

translator: lib/$(CS_ARCH)/translator

lib/$(CS_ARCH)/translator: translator.cpp lib/$(CS_ARCH)/lib.a
	${CXX} ${CXXFLAGS} -o $@ $^
