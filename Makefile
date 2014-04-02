all: engine

engine: engine.o model.o material.o light.o src/window.h PPMReader.o functions.o matrix.o clone.o camera.o
	g++ engine.o model.o material.o light.o PPMReader.o functions.o matrix.o clone.o camera.o -o engine -framework OpenGL -lglfw -lGLEW
engine.o: src/engine.cc src/model.h src/window.h src/light.h src/functions.h
	g++ -c src/engine.cc -framework OpenGL -lglfw -lGLEW
model.o: src/model.cc src/model.h PPMReader.o
	g++ -c src/model.cc -framework OpenGL -lglfw -lGLEW
material.o: src/material.cc src/material.h
	g++ -c src/material.cc -framework OpenGL -lglfw -lGLEW
light.o: src/light.cc src/light.h
	g++ -c src/light.cc -framework OpenGL -lglfw -lGLEW
PPMReader.o: src/PPMReader.cc src/PPMReader.h
	g++ -c src/PPMReader.cc -framework OpenGL -lglfw -lGLEW
functions.o: src/functions.cc src/functions.h
	g++ -c src/functions.cc -framework OpenGL -lglfw -lGLEW
matrix.o: src/matrix.cpp src/matrix.h
	g++ -c src/matrix.cpp -framework OpenGL -lglfw -lGLEW
clone.o: src/clone.cc src/clone.h src/matrix.h src/camera.h
	g++ -c src/clone.cc -framework OpenGL -lglfw -lGLEW
camera.o: src/camera.cc src/camera.h src/matrix.h
	g++ -c src/camera.cc -framework OpenGL -lglfw -lGLEW
clean:
	rm *.o
	rm engine
	rm *~
