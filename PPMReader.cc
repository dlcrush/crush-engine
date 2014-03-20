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

unsigned char * PPMReader::read() {
	// TODO
	ifstream inputFile(file);
	cout << "file: " << file << endl;
	bool done = ! inputFile.good();
	cout << "done: " << done << endl;
	while (! done) {
		unsigned char temp;
		inputFile >> temp;
		data.insert(data.begin(), temp);
		cout << (int) temp << endl;
		done = ! inputFile.good();
	}
	unsigned char * result = new unsigned char[data.size()];
	copy(data.begin(), data.end(), result);
	return result;
}