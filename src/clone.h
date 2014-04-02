#ifndef INCLUDED_clone
#define INCLUDED_clone

#include "matrix.h"
#include "model.h"
#include "camera.h"

class Clone {
private:
	Model * model;
	Camera * camera;
	Matrix model_matrix;
public:
	Clone(Model * model, Camera * clone);

	~Clone();

	void draw(Matrix projection_matrix);

	void translate(float x, float y, float z);

	void rotate(float x, float y, float z);

	void scale(float x, float y, float z);
};

#endif