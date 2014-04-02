#include "clone.h"
#include "matrix.h"

Clone::Clone(Model * model, Camera * camera) {
	this->model = model;
	this->camera = camera;
	model_matrix = Matrix::identity();
}

Clone::~Clone() {
	//delete[] model;
}

void Clone::draw(Matrix projection_matrix) {
	model->draw(projection_matrix, camera->getViewMatrix(), model_matrix);
}

void Clone::translate(float x, float y, float z) {
	model_matrix = model_matrix * Matrix::translate(x,y,z);
}

void Clone::rotate(float x, float y, float z) {
	model_matrix = model_matrix * Matrix::rotateX(x) * Matrix::rotateY(y) * Matrix::rotateZ(z);
}

void Clone::scale(float x, float y, float z) {
	model_matrix.scale(x,y,z);
}