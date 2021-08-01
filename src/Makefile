CFLAGS+=-g #-pedantic -Wall -Werror -DDEBUG
LDFLAGS+=-L../obj -lceural -llag -Wl,-R../obj

main: main.c gui.c nn.c 
	cd ../lib/ceural && $(MAKE) ceural.so
	${CC} ${CFLAGS} -I../lib/ceural/include -I../lib/lag/include `pkg-config gtk+-3.0 --cflags` $^ -o $@ `pkg-config gtk+-3.0 --libs` ${LDFLAGS}


clean:
	rm -f main