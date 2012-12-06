// NrrdObject.cpp
//
// The NrrdObject class

#include "NrrdObject.h"

NrrdObject::NrrdObject(){
	type = 0;
	typeSize = 0;
	dimension = 0;
	sizes = NULL;
	spacings = NULL;
	dataFileName = NULL;
	endian = 0;
	encoding = 0;
}

NrrdObject::~NrrdObject(){
	delete [] sizes;
}

// different types
// 0 = signed int
// 1 = unsigned int
// 2 = floating point

void NrrdObject::LoadType(char* desc){
	if(strcmp(desc, "signed char") == 0){
		type = 0;
		typeSize = 1;
	} else if(strcmp(desc, "uchar") == 0 || strcmp(desc, "unsigned char") == 0){
		type = 1;
		typeSize = 1;
	} else if(strcmp(desc, "ushort") == 0 || strcmp(desc, "unsigned short") == 0){
		type = 1;
		typeSize = 2;
	}
}

void NrrdObject::LoadDimension(char* desc){
	dimension = atoi(desc);
}

void NrrdObject::LoadSizes(char* desc){
	int i = 0;
	char* num;
	sizes = new int[dimension];
	num = strtok(desc, " ");
	while(num != NULL){
		sizes[i] = atoi(num);
		i++;
		num = strtok(NULL, " ");
	}
}

void NrrdObject::LoadSpacing(char* desc){
	int i = 0;
	char* num;
	spacings = new double[dimension];
	num = strtok(desc, " ");
	while(num != NULL){
		spacings[i] = atoi(num);
		i++;
		num = strtok(NULL, " ");
	}
}

void NrrdObject::LoadDataFileName(char* desc){
}

// endian numbers
// 0 = "undefined"
// 1 = "little"
// 2 = "big"

void NrrdObject::LoadEndian(char* desc){
	if(strcmp(desc, "little") == 0)
		endian = 1;
	if(strcmp(desc, "big") == 0)
		endian = 2;
}

// encoding numbers
// 0 = undefined
// 1 = raw
// 2 = ascii, txt, text

void NrrdObject::LoadEncoding(char* desc){
	if(strcmp(desc, "raw") == 0)
		encoding = 1;
	if(strcmp(desc, "ascii") == 0 || strcmp(desc, "txt") == 0)
		encoding = 2;
}
