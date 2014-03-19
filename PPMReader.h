#ifndef INCLUDED_PPMReader
#define INCLUDED_PPMReader

#include <iostream>
#include <fstream>
#include <string>

class PPMReader {
private:

	std::string file;

public:

	PPMReader(std::string file);

	~PPMReader();

	void setFile(std::string file);

	std::string getFile();

	unsigned char * read();
};

#endif