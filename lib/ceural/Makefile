CFLAGS+=-g -pedantic -Wall -Werror -DDEBUG #-O2 -fomit-frame-pointer
LDFLAGS+=-lm -llag -Wl,-R../../obj


ceural: main.c ceural.c activations.c mnist.c convert.c
	cd ../lag && $(MAKE) lag.so
	${CC} ${CFLAGS} -I../lag/include -L../../obj -o $@ $^ ${LDFLAGS}

ceural.so: main.c ceural.c activations.c mnist.c convert.c
	cd ../lag && $(MAKE) lag.so
	${CC} ${CFLAGS} -I../lag/include -L../../obj -fPIC -shared -o ../../obj/lib$@ $^ ${LDFLAGS}



ceural_test: ceural
	valgrind --leak-check=full ./ceural

parse_test: parse_test.c ceural.c activations.c mnist.c convert.c ../../obj/lag.so
	cd ../lag && $(MAKE) lag.so
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}
	valgrind --leak-check=full ./parse_test


activations_test: activations_test.c activations.c ../lag/debug.c ../../obj/lag.o
	cd ../lag && $(MAKE) lag.o
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}


clean:
	rm -f activations_test
	rm -f ceural
	rm -f ../../obj/libceural.so