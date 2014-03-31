#ifndef INCLUDED_PPMReader
#define INCLUDED_PPMReader

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class PPMReader {

private:

	std::string file;
	std::vector<unsigned char> data;

public:

	PPMReader(std::string file2);

	~PPMReader();

	void setFile(std::string file);

	std::string getFile();

	void read(unsigned char * & result, int & tex_size);
	
};

#endif