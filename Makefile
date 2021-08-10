LIB_ORIGIN="${PWD}/build/usr/lib"

install_tools:
	sudo apt install libgtk-3-dev
	sudo apt install patchelf
	sudo apt install libopenblas-dev

	#sudo apt install doxygen
	#sudo apt install graphviz
	#sudo npm install moxygen -g


release:
	cd ./src && $(MAKE) main
	cp ./obj/libceural.so ./build/usr/lib/
	cp ./obj/liblag.so ./build/usr/lib/
	cp ./src/main ./build/usr/bin/crecog

	patchelf --set-rpath "${LIB_ORIGIN}" ./build/usr/bin/crecog
	patchelf --set-rpath "${LIB_ORIGIN}" ./build/usr/lib/libceural.so

	linuxdeploy --appdir ./build --executable ./build/usr/bin/crecog --output appimage


clean:
	cd ./lib/ceural && $(MAKE) clean
	cd ./lib/lag && $(MAKE) clean
	cd ./src && $(MAKE) clean
	
	rm -f ./build/usr/lib/*
	rm -f ./build/usr/bin/*
	rm -f ./build/AppRun
	rm -f C_digit_recognition-*.AppImage 
