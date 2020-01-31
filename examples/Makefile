# Uncomment following to enable debug mode:
# DEBUG     = yes

CC        = g++
EXAMPLES  = filter error

CPPFLAGS  = -Wall -ansi -pedantic
ifeq '$(DEBUG)' 'yes'
CPPFLAGS += -g -O0
else
CPPFLAGS += -O3
endif

INCLUDE   = -I../src
LFLAGS    = -L.. -lxift

XENOPHAGE = ../libxift.a


default: $(EXAMPLES)

$(XENOPHAGE):
	cd ..; $(MAKE)

%: %.cpp $(XENOPHAGE)
	$(CC) $(CPPFLAGS) $(INCLUDE) $< $(LFLAGS) -o $@

clean:
	rm -f $(EXAMPLES)
