// NrrdObject.h
//
// The header file for the NrrdObject.

#include<string.h>
#include<stdlib.h>

class NrrdObject {
	public:
		NrrdObject();
		~NrrdObject();
		void LoadType(char* desc);
		void LoadDimension(char* desc);
		void LoadSizes(char* desc);
		void LoadSpacing(char* desc);
		void LoadDataFileName(char* desc);
		void LoadEndian(char* desc);
		void LoadEncoding(char* desc);
		int type;
		int typeSize;
		int dimension;
		int* sizes;
		double* spacings;
		char* dataFileName;
		int endian;
		int encoding;
	private:
};
