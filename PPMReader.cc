#include "PPMReader.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

PPMReader::PPMReader(string file) {
	this->file = file;
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
	unsigned char * result = new unsigned char[10];
	return result;
}