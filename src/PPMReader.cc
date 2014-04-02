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

void PPMReader::readHeader(ifstream & inputFile, int & width, int & height) {
	char * crap = new char[500];
	inputFile.getline(crap, 500);
	//cout << crap << endl;
	inputFile.getline(crap, 500);
	//cout << crap << endl;
	inputFile >> width;
	inputFile >> height;
	inputFile.getline(crap, 500);
	//cout << crap << endl;
	inputFile.getline(crap, 500);
	//cout << crap << endl;
}

// WARNING: This will only work if PPM file 4 line header with line 3 containing the width and
// height information for the texture.
// NOTE: Method used here is pretty stupid as the insert method for
// vector isn't really efficient.
void PPMReader::read(unsigned char * & result, int & tex_size, int & tex_width, int & tex_height) {
	// TODO
	ifstream inputFile(file); // open fstream
	bool done = ! inputFile.good(); // check to see if fstream is good
	int width, height;
	readHeader(inputFile, width, height);
	cout << tex_width << endl;
	cout << tex_height << endl;
	while (! done) {
		unsigned char temp;
		//inputFile >> temp;
		temp = inputFile.get();
		//inputFile >> temp; // read in next character
		done = ! inputFile.good(); // check to see if fstream is still good
		if (! done) {
			//cout << temp << endl;
			//data.insert(data.begin(), (int) temp); // insert at beginning to flip data
			data.push_back((int) temp);
		}
		//done = inputFile.eof(); // check to see if fstream is still good
	}
	tex_size = data.size();
	cout << tex_size << endl;
	result = new unsigned char[data.size()];
	copy(data.begin(), data.end(), result);
}