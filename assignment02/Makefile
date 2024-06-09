# COMP2521 - Assignment 2

# List all your supporting .c files here. Do NOT include .h files in this list.
# Example: SUPPORTING_FILES = hello.c world.c

SUPPORTING_FILES = Queue.c matrix.c Graph.c Pq.c

########################################################################
# !!! DO NOT MODIFY ANYTHING BELOW THIS LINE !!!

CC = clang
CFLAGS = -Wall -Wvla -Werror -gdwarf-4

########################################################################

.PHONY: asan msan nosan

asan: CFLAGS += -fsanitize=address,leak,undefined
asan: all

msan: CFLAGS += -fsanitize=memory,undefined -fsanitize-memory-track-origins
msan: all

nosan: all

########################################################################

.PHONY: all
all: testWall testClimber

testWall: testWall.c wallReader.c Wall.c $(SUPPORTING_FILES)
	$(CC) $(CFLAGS) -o testWall testWall.c wallReader.c Wall.c $(SUPPORTING_FILES) -lm

testClimber: testClimber.c climber.c wallReader.c Wall.c $(SUPPORTING_FILES)
	$(CC) $(CFLAGS) -o testClimber testClimber.c climber.c wallReader.c Wall.c $(SUPPORTING_FILES) -lm

########################################################################

.PHONY: clean
clean:
	rm -f testWall testClimber

