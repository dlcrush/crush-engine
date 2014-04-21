#include "headers/material.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <string>
#include <vector>

using namespace std;

// Constructor
Material::Material() {
	// do nothing
	hasTexture = false;
	tex_height = 0.0f;
	tex_width = 0.0f;
	transparency = 1.0f;
}

// Destructor
Material::~Material() {
	clear();
}

// clears the material properties
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
	transparency = 1.0f;
}

// updates the material transparency
void Material::set_transparency(GLfloat transparency) {
	this->transparency = transparency;
}

// updates the name of the material
void Material::set_name(string mtlName){
	name = mtlName;
}

// updates the Ka property of the material
// NOTE: 0.0 <= r,g,b <= 1.0
void Material::set_Ka(GLfloat r, GLfloat g, GLfloat b){
	Ka_r = r;
	Ka_g = g;
	Ka_b = b;
}

// updates the Kd property of the material
// NOTE: 0.0 <= r,g,b <= 1.0
void Material::set_Kd(GLfloat r, GLfloat g, GLfloat b){
	Kd_r = r;
	Kd_g = g;
	Kd_b = b;
}

// updates the Ks property of the material
// NOTE: 0.0 <= r,g,b <= 1.0
void Material::set_Ks(GLfloat r, GLfloat g, GLfloat b){
	Ks_r = r;
	Ks_g = g;
	Ks_b = b;
}

// updates the Ns coefficient of the material
// NOTE: 0.0 <= coefficient <= 1.0
void Material::set_Ns(GLfloat coefficient){
	Ns = coefficient;
}

void Material::set_has_texture(bool has_texture) {
	hasTexture = has_texture;
}

void Material::set_texture(unsigned char * texture) {
	this->texture = texture;
}

void Material::set_tex_height(GLfloat height) {
	tex_height = height;
}

void Material::set_tex_width(GLfloat width) {
	tex_width = width;
}

GLfloat Material::get_tex_height() {
	return tex_height;
}

GLfloat Material::get_tex_width() {
	return tex_width;
}

GLfloat Material::get_transparency() {
	return transparency;
}

// returns the name of the material
string Material::get_name(){
	return name;
}

// returns a pointer to the Ka array of the material
GLfloat * Material::get_Ka(){
	GLfloat * Ka = new GLfloat[3];
	Ka[0] = Ka_r;
	Ka[1] = Ka_g; 
	Ka[2] = Ka_b;

	return Ka;
}

// returns a pointer to the Kd array of the material
GLfloat * Material::get_Kd(){
	GLfloat * Kd = new GLfloat[3];
	Kd[0] = Kd_r;
	Kd[1] = Kd_g; 
	Kd[2] = Kd_b;

	return Kd;
}

// returns a pointer to the Ks array of the material
GLfloat * Material::get_Ks(){
	GLfloat * Ks = new GLfloat[3];
	Ks[0] = Ks_r;
	Ks[1] = Ks_g; 
	Ks[2] = Ks_b;

	return Ks;
}

// returns the Ns property of the material
GLfloat Material::get_Ns(){
	return Ns;
}

bool Material::has_texture() {
	return hasTexture;
}

// Returns the texture array of the material
// NOTE: user is responsible for verifying that the
// material has a texture before using texture array.
unsigned char * Material::get_texture() {
	return texture;
}