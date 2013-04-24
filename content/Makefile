
#
# makefile
#

# which c plus plus compiler
CPP     = ccache g++

# which c compiler
CC      = ccache cc

# options for compiler
CFLAGS  = -Wall -Werror

# is debug? (for development)
CDEBUG  = -g

# libraries for linker
LINKS   = -lpthread

# objects
OBJS    = student.o \
		  room.o \
		  grade.o \
          course.o \
		  classmanager.o \
		  teacher.o \
		  class.o \
          main.o

# binary
BIN	    = test

# where are include files kept
INC	    = -I. -I..

# library file (.so or .a) directory
LIB	    = -L.

# rock..
all:$(BIN)

# how to compiling programs
$(BIN):$(OBJS)
	$(CPP) $(CFLAGS) $(CDEBUG) $(LIB) -o $@ $(OBJS) $(LINKS)
%.o:%.cpp
	$(CPP) $(CFLAGS) $(CDEBUG) $(INC) -o $@ -c $<
%.o:%.c
	$(CC)  $(CFLAGS) $(CDEBUG) $(INC) -o $@ -c $<

.PHONY: clean

clean:
	-rm -rf *.o test
