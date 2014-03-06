#include "material.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <string>
#include <vector>

using namespace std;

Material::Material(){

}

Material::~Material(){

}

void Material::clear() {
	name = "";
	Ka_r = 0.0f;
	Ka_g = 0.0f;
	Ka_b = 0.0f;
	Kd_r = 0.0f;
	Kd_g = 0.0f;
	Kd_b = 0.0f;
	Ks_r = 0.0f;
	Ks_g = 0.0f;
	Ks_b = 0.0f;
	Ns = 0.0f;
}

void Material::set_name(string mtlName){
	name = mtlName;
}

void Material::set_Ka(GLfloat r, GLfloat g, GLfloat b){
	Ka_r = r;
	Ka_g = g;
	Ka_b = b;
}

void Material::set_Kd(GLfloat r, GLfloat g, GLfloat b){
	Kd_r = r;
	Kd_g = g;
	Kd_b = b;
}

void Material::set_Ks(GLfloat r, GLfloat g, GLfloat b){
	Ks_r = r;
	Ks_g = g;
	Ks_b = b;
}

void Material::set_Ns(GLfloat coefficient){
	Ns = coefficient;
}

string Material::get_name(){
	return name;
}

GLfloat * Material::get_Ka(){
	GLfloat * Ka = new GLfloat[3];
	Ka[0] = Ka_r;
	Ka[1] = Ka_g; 
	Ka[2] = Ka_b;

	return Ka;
}

GLfloat * Material::get_Kd(){
	GLfloat * Kd = new GLfloat[3];
	Kd[0] = Kd_r;
	Kd[1] = Kd_g; 
	Kd[2] = Kd_b;

	return Kd;
}

GLfloat * Material::get_Ks(){
	GLfloat * Ks = new GLfloat[3];
	Ks[0] = Ks_r;
	Ks[1] = Ks_g; 
	Ks[2] = Ks_b;

	return Ks;
}

GLfloat Material::get_Ns(){
	return Ns;
}