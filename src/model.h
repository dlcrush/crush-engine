#ifndef INCLUDED_MODEL
#define INCLUDED_MODEL

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <string>
#include <vector>
#include "material.h"
#include "matrix.h"

// Copies values in source vector to destination array
// NOTE: destination needs to be allocated space before function call
// NOTE: i should be a positive integer (or 0) and i < mapping.size()
void copyVectorToArray(std::vector<GLfloat> source, GLfloat * destination, std::vector<int> mapping, int i);

// Copies values in source vector to destination array
// NOTE: destination needs to be allocated space before function call
// NOTE: begin should be a positive integer (or 0) and end should be a
// positive integer
// NOTE: begin <= end
void copyVectorToArray(std::vector<GLfloat> source, GLfloat * destination, int begin, int end);

// Splits string into two parts. First part is face value.
// Second value is normal value.
void split(string input, string & face, string & normal, string & texture);

// loads Material file
// PRE: mtlFile is defined, materials has been allocated space
// POST: materials contains all the materials from the mtlFile
void loadMtlFile(string mtlFile, vector<Material> & materials,
  unsigned char * & texture);

class Model {

private:

	/* Will hold binding to vertices and color arrays */
	std::vector<GLuint> vertex_buffer_id, normal_buffer_id;
	/* Holds number of vertices in the model */
	int number_of_vertices;
	std::vector<GLuint> vertex_id, normal_id;
	std::vector<int> materialIDs;
	std::vector<int> material_vertex_map;
	std::vector<Material> materials;
	std::vector<int> sizes;
	GLuint texture_id;
	GLuint tex_coord_id;
	GLuint tex_coord_buffer_id;
	int tex_width;
	int tex_height;
	int texture_size;
	GLuint ambient_id, diffuse_id, specular_id, specular_coefficient_id,
		texture_sampler_id, model_view_projection_matrix_id, model_view_matrix_id,
		normal_matrix_id;
	GLuint program_id;

public:
	
	// PRE:
	// POST:
	Model(GLuint program_id);

	// PRE:
	// POST: All glBuffers have been deleted.
	~Model();

	void readOBJFile(ifstream & inputFile, vector<GLfloat> &points, 
  vector<GLfloat> &vn, vector<GLfloat> &vt, vector<GLfloat> &vertices, 
  vector<GLfloat> &normals, vector<Material> &materials, 
  vector<GLfloat> &textures, unsigned char * & texture,
  vector<int> & materialIDs, vector<int> & material_vertex_map);

	// PRE: vertices has been defined, points has been defined,
	// normals has been defined, vn has been defined, face is defined and 
	// an integer, normal is defined and an integer.
	// POST: the vertex for the given face has been added to the vertices
	// vector.
	void addFaceVertex(vector<GLfloat> & vertices, 
  vector<GLfloat> points, vector<GLfloat> & normals, 
  vector<GLfloat> & vn, vector<GLfloat> & textures,
  vector<GLfloat> & vt, int face, int normal, 
  int texture);

	// PRE: objFileName is defined and contains the filename of a valid 
	// .obj file.
	// POST: The vertices for the obj file have been loaded and binded to 
	// vertex_buffer_id.
	void load(std::string objFileName);

	// PRE: This is defined. vertex_buffer_id is a valid buffer binded to an
	// an array of vertices. color_buffer_id is a valid buffer binded to an
	// array of colors.
	// POST: The model has been drawn to the screen. 
	void draw(Matrix model_view_projection_matrix, Matrix model_view_matrix, Matrix normal_matrix);

	// Deletes the buffers from memory
	void clear();
};
#endif