release:
	cd ./src && $(MAKE) main
	cp ./obj/libceural.so ./build/usr/lib/
	cp ./obj/liblag.so ./build/usr/lib/
	cp ./src/main ./build/usr/bin/crecog
	#cd ./build/usr/bin && ln -s .wrapper crecog
	#cp main.wrapper ./build/usr/bin/main
	#chmod +x ./build/usr/bin/main
	chrpath -r '../lib' ./build/usr/bin/crecog

	LD_LIBRARY_PATH=.:"$LD_LIBRARY_PATH"
	export LD_LIBRARY_PATH
	linuxdeploy --appdir ./build --executable ./build/usr/bin/crecog --output appimage

	


clean:
	cd ./lib/ceural && $(MAKE) clean
	cd ./lib/lag && $(MAKE) clean
	cd ./src && $(MAKE) clean
	rm -f ./build/usr/lib/*
	rm -f ./build/usr/bin/*
