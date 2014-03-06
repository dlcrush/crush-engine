#ifndef INCLUDED_LIGHT
#define INCLUDED_LIGHT

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <string>
#include <vector>

using namespace std;

class Light {
private:
	static bool instantiated;
	GLfloat light_x, light_y, light_z;
	GLfloat light_r, light_g, light_b;
	GLfloat attenuation;
	static GLuint light_id, light_color_id, attenuation_amount_id;
	static GLuint program_id;
public:

	Light(GLuint program_id);

	void set_position(GLfloat x, GLfloat y, GLfloat z);

	void set_color(GLfloat r, GLfloat g, GLfloat b);

	void set_attenuation(GLfloat factor);

	void activate(GLuint program_id);

};

#endif