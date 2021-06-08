run_linux:
	javac -cp ".:lib/core.jar" -d bin src/*/*.java
	cd bin; java -cp ".:../lib/core.jar" gui_package.GUI; cd ..

clean:
	rm bin/*.class
	rm bin/gui_package/*.class
	rm -r bin/gui_package