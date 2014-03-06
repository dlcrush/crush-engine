all: shader

shader: shader.o model.o material.o light.o window.h
	g++ shader.o model.o material.o light.o -o shader -framework OpenGL -lglfw -lGLEW
shader.o: shader.cc model.h window.h light.h
	g++ -c shader.cc -framework OpenGL -lglfw -lGLEW
model.o: model.cc model.h
	g++ -c model.cc -framework OpenGL -lglfw -lGLEW
material.o: material.cc material.h
	g++ -c material.cc -framework OpenGL -lglfw -lGLEW
light.o: light.cc light.h
	g++ -c light.cc -framework OpenGL -lglfw -lGLEW
