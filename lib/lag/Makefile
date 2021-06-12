CFLAGS+=-g -pedantic -Wall -Werror -DDEBUG

matrix: matrix_test.c matrix.c
	${CC} ${CFLAGS} -o $@ $^

matrix_test: matrix
	valgrind ./matrix

clean:
	rm -f matrix