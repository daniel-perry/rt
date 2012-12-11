// NrrdObject.h

#include<teem/nrrd.h>

class NrrdWrapper{
	public:
		NrrdWrapper(Nrrd* nin);
		~NrrdWrapper();
		unsigned char getValue(int x, int y, int z);
	private:
		Nrrd* nin;
		int element_size;
		int size0;
		int size1;
		int size2;
};
