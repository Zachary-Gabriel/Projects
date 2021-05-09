SRCL 	:= $(shell find . -type f -name "test*.c" -print0 | xargs -0 )
SRCA	= $(subst ./,,$(SRCL))
APPS	= $(subst .c,,$(SRCA))


all: $(APPS)

include ../Makefile.inc

clean:
	$(QUIET) rm -rf $(APPS) *.dSYM output .deps