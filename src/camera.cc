#include "headers/camera.h"
#include "headers/matrix.h"

// constructor
Camera::Camera() {
	view_matrix = Matrix::identity();
	translate_matrix = Matrix::identity();
	rotate_matrix = Matrix::identity();
}

// destructor
Camera::~Camera() {
	// nothing to do
}

// Translates camera by specified amounts in x, y, and z directions
void Camera::translate(float x, float y, float z) {
	translate_matrix = Matrix::identity() * Matrix::translate(x,y,z);
	//view_matrix = view_matrix * Matrix::translate(x,y,z);	
}

// Rotates camera by specified amounts in x, y, and z directions
void Camera::rotate(float x, float y, float z) {
	rotate_matrix = Matrix::identity() * Matrix::rotateX(x) * Matrix::rotateY(y) * Matrix::rotateZ(z);;
	//view_matrix = view_matrix * Matrix::rotateX(x) * Matrix::rotateY(y) * Matrix::rotateZ(z);
}

// Returns the view_matrix for the camera
Matrix Camera::getViewMatrix() {
	view_matrix = translate_matrix * rotate_matrix;
	return view_matrix;
}