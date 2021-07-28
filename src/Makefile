main: main.c ../obj/ceural.so ../obj/lag.so
	cd ../lib/ceural && $(MAKE) ceural.so
	gcc -g `pkg-config gtk+-3.0 --cflags` $^ -o $@ `pkg-config gtk+-3.0 --libs`


clean:
	rm -f main