#include "PPMReader.h"
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

PPMReader::~PPMReader() {
	file = "";
}

void PPMReader::setFile(string file) {
	this->file = file;
}

string PPMReader::getFile() {
	return file;
}

// WARNING: This will only work if PPM file has no header information.
// NOTE: Method used here is pretty stupid as the insert method for
// vector isn't really efficient.
void PPMReader::read(unsigned char * & result, int & tex_size) {
	// TODO
	ifstream inputFile(file); // open fstream
	bool done = ! inputFile.good(); // check to see if fstream is good
	while (! done) {
		unsigned char temp;
		inputFile >> temp; // read in next character
		data.insert(data.begin(), temp); // insert at beginning to flip data
		done = ! inputFile.good(); // check to see if fstream is still good
	}
	tex_size = data.size();
	result = new unsigned char[data.size()];
	copy(data.begin(), data.end(), result);
}