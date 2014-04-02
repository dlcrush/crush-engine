#ifndef INCLUDED_CAMERA
#define INCLUDED_CAMERA

#include "matrix.h"

class Camera {
private:
	Matrix view_matrix;
	Matrix rotate_matrix;
	Matrix translate_matrix;
public:
	Camera();

	~Camera();

	void translate(float x, float y, float z);

	void rotate(float x, float y, float z);

	Matrix getViewMatrix();
};

#endif