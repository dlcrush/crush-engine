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

	// constructor
	// NOTE: program_id should be defined
	Light(GLuint program_id);

	// updates position of light
	// NOTE: 0.0 <= x,y,z <= 1.0
	void set_position(GLfloat x, GLfloat y, GLfloat z);

	// updates color of light
	// NOTE: 0.0 <= r,g,b <= 1.0
	void set_color(GLfloat r, GLfloat g, GLfloat b);

	// updates attenuation amount of light
	// NOTE factor >= 0.0
	void set_attenuation(GLfloat factor);

	// activates the light. Only one light can be activated at a time.
	// If a light is already activated, this call replaces the current
	// active light.
	void activate();

};

#endif