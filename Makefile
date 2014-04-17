all: engine

engine: engine.o model.o material.o light.o src/headers/window.h PPMReader.o functions.o matrix.o clone.o camera.o
	g++ engine.o model.o material.o light.o PPMReader.o functions.o matrix.o clone.o camera.o -o engine -framework OpenGL -lglfw -lGLEW
engine.o: src/engine.cc src/headers/model.h src/headers/window.h src/headers/light.h src/headers/functions.h
	g++ -c src/engine.cc -framework OpenGL -lglfw -lGLEW
model.o: src/model.cc src/headers/model.h PPMReader.o
	g++ -c src/model.cc -framework OpenGL -lglfw -lGLEW
material.o: src/material.cc src/headers/material.h
	g++ -c src/material.cc -framework OpenGL -lglfw -lGLEW
light.o: src/light.cc src/headers/light.h
	g++ -c src/light.cc -framework OpenGL -lglfw -lGLEW
PPMReader.o: src/PPMReader.cc src/headers/PPMReader.h
	g++ -c src/PPMReader.cc -framework OpenGL -lglfw -lGLEW
functions.o: src/functions.cc src/headers/functions.h
	g++ -c src/functions.cc -framework OpenGL -lglfw -lGLEW
matrix.o: src/matrix.cpp src/headers/matrix.h
	g++ -c src/matrix.cpp -framework OpenGL -lglfw -lGLEW
clone.o: src/clone.cc src/headers/clone.h src/headers/matrix.h src/headers/camera.h
	g++ -c src/clone.cc -framework OpenGL -lglfw -lGLEW
camera.o: src/camera.cc src/headers/camera.h src/headers/matrix.h
	g++ -c src/camera.cc -framework OpenGL -lglfw -lGLEW
vector.o: src/vector.cpp src/headers/vector.h
	g++ -c src/vector.cc -framework OpenGL -lglfw -lGLEW
clean:
	rm *.o
	rm engine
	rm *~
