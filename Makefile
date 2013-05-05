
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

# macro
CMACRO	= 
#-D_OLD_MAKEHOUSE_GAME

# objects
OBJS    = main.o \
    Config.o \
    SendTask.o \
    database.o \
    Evloop.o \
    Sock.o \
    Single.o \
    ThreadPool.o \
    RecvTask.o \
    handler/HandleCartoon.o \
    handler/HandleExercise.o \
    handler/HandleClassRoom.o \
    handler/HandleSprite.o \
    handler/HandleBuildHouse.o \
    handler/HandlePuzzle.o \
    handler/HandleMessage.o \
    handler/HandleRainbowValley.o \
    ProcessManager.o \
    content/course.o \
    content/courseitem.o \
    content/teacher.o \
    content/room.o \
    content/grade.o \
    content/game.o \
    content/student.o \
    content/roommanager.o \
    content/LoginCheck.o \
    content/makehouse.o \
    content/puzzle.o \
	content/DataTool.o

# binary
BIN	    = server

# where are include files kept
INC	    = -I. -I./includes -I./handler -I./content

# for Linker
#LINK        = libs/libev.a libs/libglog.a libs/liblua52.so libs/libmysqlcppconn.so
LINK        = -lev -lglog -lmysqlcppconn -llua5.2
# rock..
all:$(BIN)

# how to compiling programs
$(BIN):$(OBJS)
#	$(CPP) $(CFLAGS) $(CDEBUG) -o $@ $(OBJS)
	$(CPP) $(CFLAGS) $(CDEBUG) $(CMACRO) -o $@ $(OBJS) $(LINK)
%.o:%.cpp
	$(CPP) $(CFLAGS) $(CDEBUG) $(CMACRO) $(INC) -o $@ -c $<
%.o:%.c
	$(CC)  $(CFLAGS) $(CDEBUG) $(CMACRO) $(INC) -o $@ -c $<

.PHONY: clean

clean:
	-rm -rf $(OBJS) $(BIN) *~
