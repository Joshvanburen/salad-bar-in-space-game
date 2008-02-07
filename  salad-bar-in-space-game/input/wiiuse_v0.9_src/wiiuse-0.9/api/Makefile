# Minimal Makefile
#
# By: Michael Laforest <paralizer -AT- users -DOT- sourceforge -DOT- net>
#
# $Header$

#
# Change this to your GCC version.
#
CC = gcc
CCX = g++

####################################################
#
# You should not need to edit below this line.
#
####################################################

#
# Universal cflags
#
CFLAGS = -Wall -pipe

#
# Object flags
#
FLAGS = $(CFLAGS)

#
# Debug object flags
#
DFLAGS = $(CFLAGS) -g

#
# Linking flags
#
LDFLAGS = -ldl

#
# Target binaries (always created as BIN)
#
BIN = wiiuse-example
DBIN = wiiuse-debug

#
# Inclusion paths.
#
INCLUDES = -I. -I../include/

#
# Find all source files.
#
C_SRC_FILES = $(wildcard *.c)
CPP_SRC_FILES = $(wildcard *.cpp)

SRC_FILES = $(C_SRC_FILES) $(CPP_SRC_FILES)

#
# Generate a list of object files
#
C_OBJS = $(C_SRC_FILES:%.c=%.o)
CPP_OBJS = $(CPP_SRC_FILES:%.cpp=%.o)

OBJS = $(C_OBJS) $(CPP_OBJS)

DOBJS = $(OBJS:%.o=d_%.o)

#
# Need this for the 'clean' target.
#
EXISTING_OBJS = $(wildcard *.o)


###############################
#
# Build targets.
#
###############################

Release:
	@echo
	@echo ------
	@echo - Building wiiuse example ...
	@echo ------
	make $(BIN)

	@echo
	@echo --
	@echo -- Build complete.
	@echo --
	@echo

Debug: $(DBIN)

release: Release
debug: Debug

#
# If they exist, remove the files:
#   *.o
#
clean:
	@if [ ! -z "$(EXISTING_OBJS)" ]; \
	then \
		rm *.o &> /dev/null ; \
		echo "Cleaned files."; \
	else \
		echo "No files to clean."; \
	fi

distclean:
	@make clean
	@if [ -e "$(BIN)" ]; \
	then \
		rm $(BIN) &> /dev/null ; \
	fi

$(BIN): $(OBJS)
	$(CCX) $(FLAGS) $(LDFLAGS) $(OBJS) -o $(BIN)

$(DBIN): $(DOBJS)
	$(CCX) $(DFLAGS) $(LDFLAGS) $(DOBJS) -o $(BIN)

%.o: %.c
	$(CC) $(FLAGS) $(INCLUDES) -c $<

%.o: %.cpp
	$(CCX) $(FLAGS) $(INCLUDES) -c $<

d_%.o: %.c
	$(CC) $(DFLAGS) $(INCLUDES) -c $< -o $@

d_%.o: %.cpp
	$(CCX) $(DFLAGS) $(INCLUDES) -c $< -o $@
