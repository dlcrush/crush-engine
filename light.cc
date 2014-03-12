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
	light_z = .0f;

	light_r = 1.0f;
	light_g = 1.0f;
	light_b = 1.0f;

	attenuation = 0.0f;
}

// updates position of light
void Light::set_position(GLfloat x, GLfloat y, GLfloat z){
	light_x = x;
	light_y = y;
	light_z = z;
}

// updates color of light
void Light::set_color(GLfloat r, GLfloat g, GLfloat b){
	light_r = r;
	light_g = g;
	light_b = b;
}

// updates attenuation amount of light
void Light::set_attenuation(GLfloat factor){
	attenuation = factor;
}

// makes the light the active light used
void Light::activate(GLuint program_id) {
	//cout << "program id: " << Light::program_id << endl;
	light_id = glGetUniformLocation(Light::program_id, "light_position");
	//cout << "light id: " << Light::light_id << endl;
	light_color_id = glGetUniformLocation(Light::program_id, "light_color");
	attenuation_amount_id = glGetUniformLocation(Light::program_id, "attenuation_amount");
	//cout << light_x << " " << light_y << " " << light_z << endl;
	//cout << "light_id: " << Light::light_id << endl;
  	//glUniform3f(Light::light_id, light_x, light_y, light_z);
  	glUniform3f(Light::light_id, light_x, light_y, light_z);
  	float * light_position = new float[4];
  	light_position[0] = -20.0f;
  	light_position[1] = -10.0f;
  	light_position[2] = -30.0f;
  	glGetUniformfv(Light::program_id, Light::light_id, light_position);
  	//cout << "light_position: " << light_position[0] << " " << light_position[1] << " " << light_position[2] << endl;
  	delete[] (light_position);
  	//cout << light_r << " " << light_g << " " << light_b << endl;
	glUniform4f(Light::light_color_id, light_r, light_g, light_b, 1.0f);
	glUniform1f(Light::attenuation_amount_id, attenuation);
}