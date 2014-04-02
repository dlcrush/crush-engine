#include "clone.h"
#include "matrix.h"

// constructor
// model and camera pointers should point to two previously
// declared models and cameras.
Clone::Clone(Model * model, Camera * camera) {
	this->model = model;
	this->camera = camera;
	model_matrix = Matrix::identity();
	translate_matrix = Matrix::identity();
	rotate_matrix = Matrix::identity();
}

// constructor
// Doesn't have a purpose right now.
Clone::~Clone() {
	// there is nothing to do
}

// Draws clone
// projection_matrix is the 4x4 projection matrix for the window.
void Clone::draw(Matrix projection_matrix) {
	model_matrix = translate_matrix * rotate_matrix;
	model->draw(projection_matrix, camera->getViewMatrix(), model_matrix);
}

// translates clone
// x, y, and z are the values to translate in the respective directions.
void Clone::translate(float x, float y, float z) {
	translate_matrix = Matrix::identity() * Matrix::translate(x,y,z);
	//model_matrix = model_matrix * Matrix::translate(x,y,z);
}

// rotates clone
// x, y, and z are the values to rotate in the respective directions.
void Clone::rotate(float x, float y, float z) {
	rotate_matrix = Matrix::identity() * Matrix::rotateX(x) * Matrix::rotateY(y) * Matrix::rotateZ(z);
	//model_matrix = model_matrix * Matrix::rotateX(x) * Matrix::rotateY(y) * Matrix::rotateZ(z);
}

// scales clone
// NOTE: Matrix::scale needs to be completed. Won't work as is.
void Clone::scale(float x, float y, float z) {
	model_matrix.scale(x,y,z);
}