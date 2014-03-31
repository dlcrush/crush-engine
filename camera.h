#ifndef INCLUDED_CAMERA
#define INCLUDED_CAMERA

#include "matrix.h"

class Camera {
private:

public:
	Camera();

	~Camera();

	void translate(float x, float y, float z);

	void rotate(float x, float y, float z);
};

#endif