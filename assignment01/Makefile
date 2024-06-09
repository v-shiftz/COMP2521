# COMP2521 - Assignment 1

# !!! DO NOT MODIFY THIS FILE !!!

CC = clang
CFLAGS = -Wall -Wvla -Werror -g

########################################################################

.PHONY: asan msan nosan

asan: CFLAGS += -fsanitize=address,leak,undefined
asan: all

msan: CFLAGS += -fsanitize=memory,undefined -fsanitize-memory-track-origins
msan: all

nosan: all

########################################################################

.PHONY: all
all: testSet

testSet: testSet.c Set.c
	$(CC) $(CFLAGS) -o testSet testSet.c Set.c

########################################################################

.PHONY: clean
clean:
	rm -f *.o testSet

