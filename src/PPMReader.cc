#include "headers/PPMReader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// WARNING: This expects a PPM file with no header. To use correctly,
// you will need to delete the header information from the PPM file.
PPMReader::PPMReader(string file2) {
	file = file2;
}

// constructor
PPMReader::~PPMReader() {
	file = "";
}

// Sets the file to the specified file.
// NOTE: It is assumed that file is a valid file. Otherwise,
// very bad things will happen.
void PPMReader::setFile(string file) {
	this->file = file;
}

// Returns the name of the file
string PPMReader::getFile() {
	return file;
}

// 
void PPMReader::readHeader(ifstream & inputFile, int & width, int & height) {
	char * crap = new char[500]; // good variable name!
	inputFile.getline(crap, 500);
	inputFile.getline(crap, 500);
	inputFile >> width;
	inputFile >> height;
	inputFile.getline(crap, 500);
	inputFile.getline(crap, 500);
}

// WARNING: This will only work if PPM file 4 line header with line 3 containing the width and
// height information for the texture.
// NOTE: Method used here is pretty stupid as the insert method for
// vector is extremely inefficient. However, I'm too lazy to fix it right now.
void PPMReader::read(unsigned char * & result, int & tex_size, int & tex_width, int & tex_height) {
	// TODO
	ifstream inputFile(file); // open fstream
	bool done = ! inputFile.good(); // check to see if fstream is good
	readHeader(inputFile, tex_width, tex_height);
	cout << tex_width << endl;
	cout << tex_height << endl;
	tex_size = tex_width * tex_height * 3;
	result = new unsigned char[tex_size];
	// for (int i = tex_size - 1; i >= 0; i --) {
	// 	result[i] = inputFile.get();
	// }
	for (int i = 0; i < tex_size; i ++) {
		result[i] = inputFile.get();
	}

	cout << tex_size << endl;
}