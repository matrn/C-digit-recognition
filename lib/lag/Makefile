CFLAGS+=-g -pedantic -Wall -Werror -DDEBUG
LDFLAGS+=-lm

lag.o: matrix.c
	echo "lag.o"
	${CC} ${CFLAGS} -o ../../obj/$@ -c $^ ${LDFLAGS}

lag.so: matrix.c debug.c
	${CC} ${CFLAGS} -fPIC -shared -o ../../obj/lib$@ $^ ${LDFLAGS}

matrix: matrix_test.c matrix.c debug.c
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}


matrix_test: matrix
	valgrind ./matrix

clean:
	rm -f matrix
	rm -f ../../obj/lag.so