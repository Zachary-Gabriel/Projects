run_linux:
	javac -cp ".:lib/core.jar" -d bin src/*/*.java
	cd bin; java -cp ".:../lib/core.jar" GUI.GUI; cd ..

clean:	
	rm -r bin/Game
	rm -r bin/Pieces
	rm -r bin/Tests
	rm -r bin/GUI