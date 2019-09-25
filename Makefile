build:
	@echo "=> Building project..."
	g++ -c raycast.cpp
	g++ -o Raycast raycast.o -lSDL2 -lSDL2_image

clean:
	rm ./*.o