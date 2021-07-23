CFLAGS+=-g -pedantic -Wall -Werror -DDEBUG #-O2 -fomit-frame-pointer
LDFLAGS+=-lm


ceural: main.c ceural.c activations.c mnist.c ../../obj/lag.o
	cd ../lag && $(MAKE) lag.o
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

ceural_test: ceural
	valgrind --leak-check=full ./ceural

clean:
	rm -f ceural