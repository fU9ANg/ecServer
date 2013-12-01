
#
# makefile
#

# set src, dst directory and flags for protobuf
PROTOC  = protoc
PBFLAGS = --cpp_out
PBSRC   = ./protos
PBDST   = ./message

# which c plus plus compiler
CPP     = ccache g++

# which c compiler
CC      = ccache cc

# options for compiler
CFLAGS  = -Wall -Werror

# flags of buildhouse
BHFLAGS	= 
#-D_BUILD_HOUSE_GMAE

# is debug? (for development)
CDEBUG  = -g -DDEBUG -D_RBV_TEST -D_BUILDHOUSE_LONGLONG

# macro
CMACRO	= 
#-D_OLD_MAKEHOUSE_GAME

# objects
OBJS    = 	main.o \
    		Config.o \
    		SendTask.o \
		    database.o \
			memcached.o \
			mongodb.o \
		    Evloop.o \
		    Sock.o \
		    Single.o \
		    ThreadPool.o \
		    RecvTask.o \
			AuthTask.o \
		    handler/HandleCartoon.o \
		    handler/HandleExercise.o \
		    handler/HandleClassRoom.o \
		    handler/HandleSprite.o \
		    handler/HandleBuildHouse.o \
		    handler/HandlePuzzle.o \
		    handler/HandleMessage.o \
		    handler/HandleRainbowValley.o \
			handler/task/BHSyncTask.o \
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
			content/DataTool.o \
			message/protos/proto.pb.o

# binary
BIN	    = server

# where are include files kept
INC	    = -I. -I./includes -I./handler -I./content -I./handler/task -I./message

# where are static library
#LIBA	= /usr/lib64/libmongoclient.a
LIBA	= /usr/local/lib/libmongoclient.a

# for Linker
#LINK        = libs/libev.a libs/libglog.a libs/liblua52.so libs/libmysqlcppconn.so libs/libmemcached.so
LINK        = -lev -lglog -lmysqlcppconn -llua5.2 -lmemcached -lprotobuf -lpthread \
              -lboost_system -lboost_thread -lboost_program_options -lboost_filesystem -lrt
# rock..
all: clean precompile_protobuf $(BIN)

# pre-compile protobuf
precompile_protobuf:
	$(PROTOC) $(PBFLAGS)=$(PBDST) $(PBSRC)/*

# how to compiling programs
$(BIN):$(OBJS)
#	$(CPP) $(CFLAGS) $(CDEBUG) -o $@ $(OBJS)
	$(CPP) $(CFLAGS) $(CDEBUG) $(BHFLAGS) $(CMACRO) -o $@ $(OBJS) $(LIBA) $(LINK)
%.o:%.cpp
	$(CPP) $(CFLAGS) $(CDEBUG) $(BHFLAGS) $(CMACRO) $(INC) -o $@ -c $<
%.o:%.cc
	$(CPP) $(CFLAGS) $(CDEBUG) $(BHFLAGS) $(CMACRO) $(INC) -o $@ -c $<
%.o:%.c
	$(CC)  $(CFLAGS) $(CDEBUG) $(BHFLAGS) $(CMACRO) $(INC) -o $@ -c $<

.PHONY: clean

clean:
	-rm -rf $(OBJS) $(BIN) *~ logs .lock message/*
