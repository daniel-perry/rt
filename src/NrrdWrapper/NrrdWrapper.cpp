// NrrdWrapper.cpp

#include "NrrdWrapper.h"

NrrdWrapper::NrrdWrapper(Nrrd* n){
	nin = n;
	element_size = (int)nrrdElementSize(nin);
	size0 = nin->axis[0].size;
	size1 = nin->axis[1].size;
	size2 = nin->axis[2].size;
}

NrrdWrapper::~NrrdWrapper(){}
	
unsigned char NrrdWrapper::getValue(int x, int y, int z){
	unsigned char* values;
	unsigned char val;
	values = (unsigned char*)nin->data;

	val = values[x + size0*(y + z*size1)];
	return val;
}
