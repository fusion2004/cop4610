CFLAGS = -std=gnu99 -O2 -Wall -ggdb

all: testsyscall.c
	${CC} ${CFLAGS} -o testsyscall $^

clean::
	-rm -f *~ *.o *.dep testsyscall
