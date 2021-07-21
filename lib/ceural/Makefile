CFLAGS+=-g -pedantic -Wall -Werror -DDEBUG

ceural: main.c ceural.c ../../obj/lag.o
	${CC} ${CFLAGS} -o $@ $^

ceural_test: ceural
	valgrind ./ceural

clean:
	rm -f ceural