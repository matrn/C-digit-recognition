CFLAGS+=-g #-pedantic -Wall -Werror -DDEBUG
LDFLAGS+=-L../obj -lceural -llag -Wl,-R../obj
GTKFLAGS+=`pkg-config gtk+-3.0 --cflags`
GTKLIBS+=`pkg-config gtk+-3.0 --libs`


main: main.c gui.c nn.c 
	cd ../lib/ceural && $(MAKE) ceural.so
	${CC} ${CFLAGS} ${GTKFLAGS} -I../lib/ceural/include -I../lib/lag/include $^ -o $@ ${GTKLIBS} ${LDFLAGS}


clean:
	rm -f main