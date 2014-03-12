#include "light.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

bool Light::instantiated = false;
GLuint Light::light_id = 0;
GLuint Light::light_color_id = 0;
GLuint Light::attenuation_amount_id = 0;
GLuint Light::program_id = 0;

// constructor
// NOTE: program_id should be defined
Light::Light(GLuint program_id) {

	if (! Light::instantiated) {
		Light::instantiated = false;
		Light::program_id = program_id;
		Light::light_id = glGetUniformLocation(Light::program_id, "light_position");
		Light::light_color_id = glGetUniformLocation(Light::program_id, "light_color");
		Light::attenuation_amount_id = glGetUniformLocation(Light::program_id, "attenuation_amount");
	}

	light_x = 1.0f;
	light_y = 1.0f;
	light_z = 1.0f;

	light_r = 1.0f;
	light_g = 1.0f;
	light_b = 1.0f;

	attenuation = 0.0f;
}

// updates position of light
// NOTE: 0.0 <= x,y,z <= 1.0
void Light::set_position(GLfloat x, GLfloat y, GLfloat z){
	light_x = x;
	light_y = y;
	light_z = z;
}

// updates color of light
// NOTE: 0.0 <= r,g,b <= 1.0
void Light::set_color(GLfloat r, GLfloat g, GLfloat b){
	light_r = r;
	light_g = g;
	light_b = b;
}

// updates attenuation amount of light
// NOTE factor >= 0.0
void Light::set_attenuation(GLfloat factor){
	attenuation = factor;
}

// activates the light. Only one light can be activated at a time.
// If a light is already activated, this call replaces the current
// active light.
void Light::activate() {
	light_id = glGetUniformLocation(Light::program_id, "light_position");
	light_color_id = glGetUniformLocation(Light::program_id, "light_color");
	attenuation_amount_id = glGetUniformLocation(Light::program_id, "attenuation_amount");
  	glUniform3f(Light::light_id, light_x, light_y, light_z);
	glUniform4f(Light::light_color_id, light_r, light_g, light_b, 1.0f);
	glUniform1f(Light::attenuation_amount_id, attenuation);
}