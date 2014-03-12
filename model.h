#ifndef INCLUDED_MODEL
#define INCLUDED_MODEL

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <string>
#include <vector>
#include "material.h"

void copyVectorToArray(std::vector<GLfloat> source, GLfloat * destination, std::vector<int> mapping, int i);
void copyVectorToArray(std::vector<GLfloat> source, GLfloat * destination, int begin, int end);

class Model {

private:

	/* Will hold binding to vertices and color arrays */
	//GLuint vertex_buffer_id, normal_buffer_id;
	std::vector<GLuint> vertex_buffer_id, normal_buffer_id;
	/* Holds number of vertices in the model */
	int number_of_vertices;
	//GLuint vertex_id, normal_id;
	std::vector<GLuint> vertex_id, normal_id;
	std::vector<int> materialIDs;
	std::vector<int> material_vertex_map;
	std::vector<Material> materials;

public:
	
	// PRE:
	// POST: vertex_buffer_id has been allocated space,
	// color_buffer_id has been allocated space,
	// number_of_vertices has been allocated space.
	Model();

	// PRE: This is defined.
	// POST: Any existing buffers created by the model have been deleted.
	~Model();

	// PRE: vertices has been defined, points has been defined,
	// colors has been defined, and face is defined and an integer.
	// POST: the vertex for the given face has been added to the vertices
	// vector.
	void addFaceVertex(std::vector<GLfloat> & vertices, std::vector<GLfloat> points, std::vector<GLfloat> & normals, std::vector<GLfloat> & vn, int face, int normal);

	// PRE: objFileName is defined and contains the filename of a valid .obj file.
	// This has been defined.
	// POST: The vertices for the obj file have been loaded and binded to 
	// vertex_buffer_id. Also, color (same for all) has also been binded to 
	// color_buffer_id.
	void load(std::string objFileName, GLuint program_id);

	// PRE: This is defined. vertex_buffer_id is a valid buffer binded to an
	// an array of vertices. color_buffer_id is a valid buffer binded to an
	// array of colors.
	// POST: The model has been drawn to the screen. 
	void draw(GLuint program_id);

	void clear();
};
#endif