#include "camera.h"
#include "matrix.h"

Camera::Camera() {
	view_matrix = Matrix::identity();
}

Camera::~Camera() {

}

void Camera::translate(float x, float y, float z) {
	view_matrix = view_matrix * Matrix::translate(x,y,z);	
}

void Camera::rotate(float x, float y, float z) {
	view_matrix = view_matrix * Matrix::rotateX(x) * Matrix::rotateY(y) * Matrix::rotateZ(z);
}

Matrix Camera::getViewMatrix() {
	return view_matrix;
}