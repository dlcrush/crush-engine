#ifndef INCLUDED_clone
#define INCLUDED_clone

#include "matrix.h"
#include "model.h"

class Clone {
private:
	Model model;
	Matrix model_view_projection_matrix;
	Matrix model_view_matrix;
	Matrix normal_matrix;
public:
	Clone(Model * model);

	~Clone();

	void draw();

	void translate(float x, float y, float z);

	void rotate(float x, float y, float z);

	void scale(float x, float y, float z);
};

#endif