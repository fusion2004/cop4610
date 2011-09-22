CFLAGS = -std=gnu99 -O2 -Wall -Werror -ggdb -pthread

all: part1.c
	${CC} ${CFLAGS} -o part1 $^
	${CC} ${CFLAGS} -DPTHREAD_SYNC -o part1-sync $^

clean::
	-rm *~ *.o *.dep part1 part1-sync
