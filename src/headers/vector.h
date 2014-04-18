#ifndef INCLUDED_vector
#define INCLUDED_vector

#include <iostream>
#include "matrix.h"

class Vector {
private:
	float data[4];
public:
	Vector();

	Vector(float * data);

	Vector(float x, float y, float z);

	void setData(float * data);

	float * getData();

	static float dot(Vector vector1, Vector vector2);

	static Vector cross(Vector vector1, Vector vector2);

	friend std::ostream& operator<<(std::ostream& out, const Vector& temp);

	friend Vector operator* (const Vector& v, const Matrix& m);
};

#endif