CFLAGS+=-g -pedantic -Wall -Werror

matrix: matrix_test.c matrix.c
	${CC} ${CFLAGS} -o $@ $^

matrix_test: matrix
	./matrix