#include "headers/clone.h"
#include "headers/matrix.h"
#include <vector>
#include "headers/vector.h"

// constructor
// model and camera pointers should point to two previously
// declared models and cameras.
Clone::Clone(Model * model, Camera * camera) {
	this->model = model;
	this->camera = camera;
	model_matrix = Matrix::identity();
	translate_matrix = Matrix::identity();
	rotate_matrix = Matrix::identity();
	scale_matrix = Matrix::identity();
	GLfloat minx, miny, minz;
	GLfloat maxx, maxy, maxz;
	minx = model->get_min_x();
	miny = model->get_min_y();
	minz = model->get_min_z();
	maxx = model->get_max_x();
	maxy = model->get_max_y();
	maxz = model->get_max_z();
	Vector * temp_vector;
	temp_vector = new Vector(minx, miny, minz);
	vectors.push_back(*temp_vector);
	temp_vector = new Vector(minx, miny, maxz);
	vectors.push_back(*temp_vector);
	temp_vector = new Vector(maxx, miny, maxz);
	vectors.push_back(*temp_vector);
	temp_vector = new Vector(maxx, miny, minz);
	vectors.push_back(*temp_vector);
	temp_vector = new Vector(maxx, maxy, minz);
	vectors.push_back(*temp_vector);
	temp_vector = new Vector(minx, maxy, minz);
	vectors.push_back(*temp_vector);
	temp_vector = new Vector(minx, maxy, maxz);
	vectors.push_back(*temp_vector);
	temp_vector = new Vector(maxx, maxy, maxz);
	vectors.push_back(*temp_vector);

	// for (int i = 0; i < 8; i ++) {
	// 	cout << vertices.at(i) << endl;
	// }
	// cout << endl;

	Vector vector1(2.0f, 8.0f, 4.0f);
	float data[4][4] = {{1.000,2.000, 3.000, 4.000}, {4.000,3.000, 2.000, 1.000}, {3.000, 2.000, 4.000, 1.000}, {2.000, 4.000, 1.000, 3.000}};
	Matrix matrix1(data);
	//cout << vector1 * matrix1 << endl;

}

// constructor
// Doesn't have a purpose right now.
Clone::~Clone() {
	// there is nothing to do
	
}

vector<Vector> Clone::getVectors() {
	vector<Vector> updatedVectors = vectors;

	for (int i = 0; i < updatedVectors.size(); i ++) {
		//Matrix temp_matrix = Matrix::identity() * translate_matrix * rotate_matrix * scale_matrix;
		Matrix view_matrix = camera->getViewMatrix();
		Matrix model_view_matrix = model_matrix * view_matrix;
		//cout << model_view_matrix << endl;
		updatedVectors.at(i) = updatedVectors.at(i) * model_view_matrix;
	}

	return updatedVectors;
}

// Draws clone
// projection_matrix is the 4x4 projection matrix for the window.
void Clone::draw(Matrix projection_matrix) {
	model_matrix = translate_matrix * rotate_matrix * scale_matrix;
	//cout << model_matrix << endl;
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
	rotate_matrix = Matrix::rotateX(x) * Matrix::rotateY(y) * Matrix::rotateZ(z);
	//model_matrix = model_matrix * Matrix::rotateX(x) * Matrix::rotateY(y) * Matrix::rotateZ(z);
}

// scales clone
void Clone::scale(float x, float y, float z) {
	scale_matrix = Matrix::scale(x,y,z);
}