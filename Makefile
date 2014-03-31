all: engine

engine: engine.o model.o material.o light.o window.h PPMReader.o functions.o matrix.o
	g++ engine.o model.o material.o light.o PPMReader.o functions.o matrix.o -o engine -framework OpenGL -lglfw -lGLEW
engine.o: engine.cc model.h window.h light.h functions.h
	g++ -c engine.cc -framework OpenGL -lglfw -lGLEW
model.o: model.cc model.h PPMReader.o
	g++ -c model.cc -framework OpenGL -lglfw -lGLEW
material.o: material.cc material.h
	g++ -c material.cc -framework OpenGL -lglfw -lGLEW
light.o: light.cc light.h
	g++ -c light.cc -framework OpenGL -lglfw -lGLEW
PPMReader.o: PPMReader.cc PPMReader.h
	g++ -c PPMReader.cc -framework OpenGL -lglfw -lGLEW
functions.o: functions.cc functions.h
	g++ -c functions.cc -framework OpenGL -lglfw -lGLEW
matrix.o: matrix.cpp matrix.h
	g++ -c matrix.cpp -framework OpenGL -lglfw -lGLEW
clean:
	rm *.o
	rm engine
	rm *~
