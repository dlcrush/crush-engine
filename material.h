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
public:

	Material();

	~Material();

	void clear();

	void set_name(string mtlName);

	void set_Ka(GLfloat r, GLfloat g, GLfloat b);

	void set_Kd(GLfloat r, GLfloat g, GLfloat b);

	void set_Ks(GLfloat r, GLfloat g, GLfloat b);

	void set_Ns(GLfloat coefficient);

	string get_name();

	GLfloat * get_Ka();

	GLfloat * get_Kd();

	GLfloat * get_Ks();

	GLfloat get_Ns();

};

#endif