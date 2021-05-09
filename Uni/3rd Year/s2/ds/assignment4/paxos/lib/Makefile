SRCL 	:= $(shell find . -type f -name "[A-Za-z]**.c" -print0 | xargs -0 )
SRCS	= $(subst ./,,$(SRCL))
OBJS	= $(subst .c,.o,$(SRCS))

all: $(OBJS)
	
include ../Makefile.inc

clean: 
	$(QUIET) rm -rf *.o .deps