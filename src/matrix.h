#ifndef INCLUDED_MATRIX
#define INCLUDED_MATRIX

#include <iostream>
#include <GL/glew.h>
#include <GL/glfw.h>
#define NUM_ROWS 4
#define NUM_COLS 4
#define SIZE 16
using namespace std;

float cot(float i);

class Matrix {
private:
	float matrix[NUM_ROWS][NUM_COLS];	

public:

	Matrix();

	Matrix(float matrixArray[4][4]);

	static Matrix identity();

	Matrix normalMatrix() const;

	static Matrix rotateX(float angle);

	static Matrix rotateY(float angle);

	static Matrix rotateZ(float angle);

	void scale(float x, float y, float z);

	static Matrix multiply(Matrix l, Matrix r);

	static Matrix translate(float x, float y, float z);

	static Matrix ortho(float left, float right, float bottom, float top, float nearVal, float farVal);

	static Matrix perspective(float fovy, float aspect, float zNear, float zFar);

	static Matrix quaternionRotate(float x, float y, float z, float w);

	float * data();

	friend ostream& operator<<(ostream& out, const Matrix& temp);

	friend Matrix operator* (const Matrix& x, const Matrix& y);

};
#endif