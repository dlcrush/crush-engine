#ifndef INCLUDED_vector
#define INCLUDED_vector

#include <iostream>

class Vector {
private:
	float data[3];
public:
	Vector();

	Vector(float * data);

	void setData(float * data);

	float * getData();

	static float dot(Vector vector1, Vector vector2);

	static Vector cross(Vector vector1, Vector vector2);

	friend std::ostream& operator<<(std::ostream& out, const Vector& temp);
};

#endif