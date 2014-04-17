#include "vector.h"
#include <iostream>

using namespace std;

Vector::Vector() {
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
}

Vector::Vector(float * data) {
	this->data[0] = data[0];
	this->data[1] = data[1];
	this->data[2] = data[2];
}

void Vector::setData(float * data) {
	this->data[0] = data[0];
	this->data[1] = data[1];
	this->data[2] = data[2];
}

float * Vector::getData() {
	return data;
}

float Vector::dot(Vector vector1, Vector vector2) {
	float result = 0.0f;
	for (int i = 0; i < 3; i ++) {
		result += vector1.data[i] * vector2.data[i];
	}
	return result;
}

Vector Vector::cross(Vector vector1, Vector vector2) {
	Vector result;
	result.data[0] = vector1.data[1] * vector2.data[2] - vector1.data[2] * vector2.data[1];
	result.data[1] = vector1.data[2] * vector2.data[0] - vector1.data[0] * vector2.data[2];
	result.data[2] = vector1.data[0] * vector2.data[1] - vector1.data[1] * vector2.data[0];
	return result;
}

ostream& operator<<(ostream& out, const Vector& temp){
	for (int i = 0; i < 3; i++) {
		out << temp.data[i] << endl;
	}
	return out;
}