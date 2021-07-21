CFLAGS+=-g -pedantic -Wall -Werror -DDEBUG
LDFLAGS+=-lm


ceural: main.c ceural.c activations.c ../../obj/lag.o
	cd ../lag && $(MAKE) lag.o
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

ceural_test: ceural
	valgrind ./ceural

clean:
	rm -f ceural