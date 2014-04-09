#include "matrix.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glfw.h>
using namespace std;

// Returns cotangent of angle i
// NOTE: i is expected to be in radians
float cotan(float i) { 
	return(1.0f / tan(i));
}

// constructor
// initialzes matrix to 4x4 matrix containing zeros.
Matrix::Matrix() {
	for (int i = 0; i < 4; i ++) {
		for (int j = 0; j < 4; j++) {
			matrix[i][j] = 0.0f;
		}
	}
}

// returns normalized matrix
Matrix Matrix::normalMatrix() const {
    Matrix normal = identity();

    float det_recip = 1.0f / (matrix[0][0] * matrix[1][1] * matrix[2][2] +
                              matrix[1][0] * matrix[2][1] * matrix[0][2] +
                              matrix[2][0] * matrix[0][1] * matrix[1][2] -
                              matrix[0][0] * matrix[2][1] * matrix[1][2] -
                              matrix[2][0] * matrix[1][1] * matrix[0][2] -
                              matrix[1][0] * matrix[0][1] * matrix[2][2]);

    //cout << "det_recip: " << det_recip << endl;
    normal.matrix[0][0] = det_recip
      * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]);
    normal.matrix[0][1] = det_recip
      * (matrix[1][2] * matrix[2][0] - matrix[1][0] * matrix[2][2]);
    normal.matrix[0][2] = det_recip
      * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
    normal.matrix[1][0] = det_recip
      * (matrix[0][2] * matrix[2][1] - matrix[0][1] * matrix[2][2]);
    normal.matrix[1][1] = det_recip
      * (matrix[0][0] * matrix[2][2] - matrix[0][2] * matrix[2][0]);
    normal.matrix[1][2] = det_recip
      * (matrix[0][1] * matrix[2][0] - matrix[0][0] * matrix[2][1]);
    normal.matrix[2][0] = det_recip
      * (matrix[0][1] * matrix[1][2] - matrix[0][2] * matrix[1][1]);
    normal.matrix[2][1] = det_recip
      * (matrix[0][2] * matrix[1][0] - matrix[0][0] * matrix[1][2]);
    normal.matrix[2][2] = det_recip
      * (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]);
    return normal;
}

// constructor
// takes two dimensional of size 4 with each internal array having size 4
Matrix::Matrix(float matrixArray[4][4] ) {
	for (int i = 0; i < 4; i ++) {
		for (int j = 0; j < 4; j++) {
			matrix[i][j] = matrixArray[i][j];
		}
	}
}

// Returns the identity matrix
Matrix Matrix::identity() {
	Matrix identity2;

	for (int i = 0; i < 4; i ++) {
		for (int j = 0; j < 4; j ++) {
			if (i == j) {
				identity2.matrix[i][j] = 1.0f;
			}
		}
	}

	return identity2;
}

// multiplies two matrices together
// l is the matrix of the left side of the multiplication
// r is the matrix on the right side of the muliplication
Matrix Matrix::multiply(const Matrix l, const Matrix r) {
	Matrix result;

	for (int row = 0; row < NUM_ROWS; row ++) {
		for (int col = 0; col < NUM_COLS; col ++) {
			result.matrix[row][col] = 0.0f;
			for (int index = 0; index < NUM_ROWS; index ++) {
				result.matrix[row][col] += l.matrix[row][index] * r.matrix[index][col];
			}
		}
	}
	return result;
}

// overrides * operator
// x is matrix on the left side of the multiplication
// y is matrix on the right side of the multiplication
Matrix operator* (const Matrix& x, const Matrix& y) {
    return Matrix::multiply(x,y);
}

// returns ortho matrix
Matrix Matrix::ortho(float left, float right, float bottom, float top, float nearVal, float farVal) {
	Matrix test;

	test.matrix[0][0] = 2.0/(right - left);
	test.matrix[1][1] = 2.0/(top - bottom);
	test.matrix[2][2] = -2.0/(farVal - nearVal);
	test.matrix[0][3] = - (float)(right + left) / (right - left);
	test.matrix[1][3] = - (float) (top + bottom)/ (top - bottom);
	test.matrix[2][3] = - (float) (farVal + nearVal) / (farVal - nearVal);
	test.matrix[3][3] = 1;

	return test;
}

Matrix Matrix::perspective(float fovy, float aspect, float zNear, float zFar) {
	float f = cotan(fovy/2.0f);

	Matrix returnMatrix;

	returnMatrix.matrix[0][0] = f/ (float) aspect;
	returnMatrix.matrix[1][1] = f;
	returnMatrix.matrix[2][2] = (zFar + zNear) / (zNear - zFar);
	returnMatrix.matrix[2][3] = (2 * zFar * zNear) / (zNear - zFar);
	returnMatrix.matrix[3][2] = -1;

	return returnMatrix;
}

// scales the matrix
// NOTE: Needs to be completed
Matrix Matrix::scale(float x, float y, float z) {
	Matrix scaleMatrix;

	scaleMatrix.matrix[0][0] = x;
	scaleMatrix.matrix[1][1] = y;
	scaleMatrix.matrix[2][2] = z;
	scaleMatrix.matrix[3][3] = 1;

	return scaleMatrix;
}

// overrides the << operator
ostream& operator<<(ostream& out, const Matrix& temp){
	std::streamsize p = out.precision();
	out.precision(5);
	//out.setw(5);
	for (int i = 0; i < 4; i ++) {
		for (int j = 0; j < 4; j++) {
			out << temp.matrix[i][j] << " ";
			if (j == 3) {
				out << endl;
			}
		}
	}
	out.precision(p);
	return out;
}

// returns rotation matrix for x direction
Matrix Matrix::rotateX(float angle) {
	float rx[4][4]  = {{1,          0,           0, 0},
					   {0, cos(angle), -sin(angle), 0},
					   {0, sin(angle),  cos(angle), 0},
					   {0,          0,           0, 1} };
	return Matrix(rx);
}

// returns rotation matrix for y direction
Matrix Matrix::rotateY(float angle) {
	float ry[4][4]  = {{cos(angle), 0, -sin(angle), 0},
					  {0,           1, 0,           0},
					  {sin(angle),  0, cos(angle),  0},
					  {0,           0, 0,           1} };
	return Matrix(ry);
}

// returns rotation matrix for z direction
Matrix Matrix::rotateZ(float angle) {
	float rz[4][4]  = {{cos(angle), -sin(angle), 0, 0},
					  {sin(angle),  cos(angle),  0, 0},
					  {0,           0,           1, 0},
					  {0,           0,           0, 1} };
	return Matrix(rz);
}

// returns translation matrix for x, y, and z directions.
Matrix Matrix::translate(float x, float y, float z) {
	float result[4][4] = {{1, 0, 0, x},
						  {0, 1, 0, y},
						  {0, 0, 1, z},
						  {0, 0, 0, 1}};
	return Matrix(result);
}

Matrix Matrix::quaternionRotate(float x, float y, float z, float w) {
	float result[4][4] = {{-1.0f * (y*y + z*z), 2.0f * (x*y - w*z),  2.0f * (w*y + x*z)},
						  {2.0f * (x*y + w*z),  -1.0f * (x*x + z*z), 2.0f * (y*z - w*x)},
						  {2.0f * (x*z - w*y),  2.0f * (y*z + w*x),  -1.0f * (x*x + y*y)}};
	return Matrix(result);
}

// returns one dimensional array representation of matrix
// Note: general solution to access elements is row * 4 + col
float * Matrix::data() {
	float * returnArray = new float[4 * 4];

	for (int i = 0; i < 4; i ++) {
		for (int j = 0; j < 4; j ++) {
			returnArray[i * 4 + j] = matrix[i][j];
		}
	}

	return returnArray;
}