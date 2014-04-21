#ifndef INCLUDED_MATERIAL
#define INCLUDED_MATERIAL

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <string>
#include <vector>

using namespace std;

class Material {

private:
	string name;
	GLfloat Ka_r, Ka_g, Ka_b;
	GLfloat Kd_r, Kd_g, Kd_b;
	GLfloat Ks_r, Ks_g, Ks_b;
	GLfloat Ns;
	bool hasTexture;
	unsigned char * texture;
	GLfloat tex_height;
	GLfloat tex_width;
	GLfloat transparency;
public:

	// Constructor
	Material();

	// Destructor
	~Material();

	// Clears the material's properties
	void clear();

	// updates the transparency value
	void set_transparency(GLfloat transparency);

	// updates the name of the material
	void set_name(string mtlName);

	// updates the Ka property of the material
	// NOTE: 0.0 <= r,g,b <= 1.0
	void set_Ka(GLfloat r, GLfloat g, GLfloat b);

	// updates the Kd property of the material
	// NOTE: 0.0 <= r,g,b <= 1.0
	void set_Kd(GLfloat r, GLfloat g, GLfloat b);

	// updates the Ks property of the material
	// NOTE: 0.0 <= r,g,b <= 1.0
	void set_Ks(GLfloat r, GLfloat g, GLfloat b);

	// updates the Ns coefficient of the material
	// NOTE: 0.0 <= coefficient <= 1.0
	void set_Ns(GLfloat coefficient);

	void set_texture(unsigned char * texture);

	void set_has_texture(bool has_texture);

	void set_tex_height(GLfloat height);

	void set_tex_width(GLfloat width);

	GLfloat get_tex_height();

	GLfloat get_tex_width();

	GLfloat get_transparency();

	// returns the name of the material
	string get_name();

	// returns a pointer to the Ka array of the material
	GLfloat * get_Ka();

	// returns a pointer to the Kd array of the material
	GLfloat * get_Kd();

	// returns a pointer to the Ks array of the material
	GLfloat * get_Ks();

	// returns the Ns property of the material
	GLfloat get_Ns();

	bool has_texture();

	unsigned char * get_texture();

};

#endif