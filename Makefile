# Debug mode:
# DEBUG     = yes

CC        = g++
LIB       = libxift.a

CPPFLAGS  = -Wall -ansi -pedantic
ifeq '$(DEBUG)' 'yes'
CPPFLAGS += -g -O0
else
CPPFLAGS += -O3
endif

SRC_DIR   = src
SRC       = $(wildcard $(SRC_DIR)/*.cpp)
OBJ       = $(SRC:.cpp=.o)


default: $(LIB)

ifneq "clean" "$(MAKECMDGOALS)"
-include deps.mk
endif

deps.mk: $(SRC)
	$(CC) -MM $^ > $@

%.o: %.cpp %.hpp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(LIB): $(OBJ)
	ar rcs $@ $^

clean:
	rm -f $(OBJ) $(LIB) deps.mk
