#include "clone.h"

Clone::Clone(Model * model) {
	this->model = model;
}

Clone::~Clone() {
	//delete[] model;
}

void Clone::draw() {
	model->draw();
}

