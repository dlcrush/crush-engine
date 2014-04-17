#ifndef INCLUDED_clone
#define INCLUDED_clone

#include "matrix.h"
#include "model.h"
#include "camera.h"
#include <vector>
#include "vector.h"

class Clone {
private:
	Model * model;
	Camera * camera;
	Matrix model_matrix;
	Matrix translate_matrix;
	Matrix rotate_matrix;
	Matrix scale_matrix;
	std::vector<Vector> bounding_box_normals;
public:
	Clone(Model * model, Camera * clone);

	~Clone();

	void draw(Matrix projection_matrix);

	void translate(float x, float y, float z);

	void rotate(float x, float y, float z);

	void scale(float x, float y, float z);
};

#endif