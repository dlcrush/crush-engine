#include "PPMReader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

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

void PPMReader::read(unsigned char * & result, int & tex_size) {
	// TODO
	ifstream inputFile(file);
	bool done = ! inputFile.good();
	while (! done) {
		unsigned char temp;
		inputFile >> temp;
		data.insert(data.begin(), temp);
		done = ! inputFile.good();
	}
	tex_size = data.size();
	result = new unsigned char[data.size()];
	copy(data.begin(), data.end(), result);
}