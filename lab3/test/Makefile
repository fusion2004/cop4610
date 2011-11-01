CFLAGS = -std=gnu99 -O2 -Wall -Werror -ggdb

all: testsyscall.c
	${CC} ${CFLAGS} -o testsyscall $^

clean::
	-rm -f *~ *.o *.dep testsyscall
