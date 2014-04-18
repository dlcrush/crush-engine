#include "headers/vector.h"
#include <iostream>
#include "headers/matrix.h"

using namespace std;

Vector::Vector() {
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
}

Vector::Vector(float * data) {
	this->data[0] = data[0];
	this->data[1] = data[1];
	this->data[2] = data[2];
	this->data[3] = data[3];
}

Vector::Vector(float x, float y, float z) {
	this->data[0] = x;
	this->data[1] = y;
	this->data[2] = z;
	this->data[3] = 0;
}

void Vector::setData(float * data) {
	this->data[0] = data[0];
	this->data[1] = data[1];
	this->data[2] = data[2];
	this->data[3] = data[3];
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

// It is assumed that Matrix is a 4x4 matrix
Vector operator* (const Vector& v, const Matrix& m) {
	Vector result;
	result.data[0] = m.matrix[0][0] * v.data[0] + m.matrix[0][1] * v.data[1] + m.matrix[0][2] * v.data[2];
	result.data[1] = m.matrix[1][0] * v.data[0] + m.matrix[1][1] * v.data[1] + m.matrix[1][2] * v.data[2];
	result.data[2] = m.matrix[2][0] * v.data[0] + m.matrix[2][1] * v.data[1] + m.matrix[2][2] * v.data[2];
	result.data[3] = m.matrix[3][0] * v.data[0] + m.matrix[3][1] * v.data[1] + m.matrix[3][2] * v.data[2];
	return result;
}

