// NrrdReader.cpp
// A function that reads in a nrrd file

#include<stdio.h>
#include<cstring>
#include "NrrdObject.h"

void nrrdReader(FILE* fp);
void loadValue(char* field, char* desc, NrrdObject* nb);

int main(int argc, char** argv){
	FILE* fp;

	fp = fopen(argv[1], "r");
	if (!fp){
		printf("Could not open file\n");
	}
	nrrdReader(fp);
	fclose(fp);
	return 0;
}

void nrrdReader(FILE* fp){
	char line_buffer[BUFSIZ];
	char field[25];
	char desc[BUFSIZ];
	int line_number = 0;
	int in_desc = 0;
	int in_value = 0;
	int i,j,k;

	NrrdObject b;

	while(fgets(line_buffer, BUFSIZ, fp)){
		line_number++;
		if(line_number != 1){
			j = 0;
			k = 0;
			for(i = 0; i < BUFSIZ; i++){
				if(line_buffer[i] == '#'){
					i = BUFSIZ;
				} else if(line_buffer[i] == ':'){
					i++;
					if(line_buffer[i] == ' '){
						in_desc = 1;
					}else{
						in_value = 1;
					}
					field[j] = '\0';
				}else if(!in_desc && !in_value){
					field[j] = line_buffer[i];
					j++;
				}else{
					if(line_buffer[i] == '\n' || line_buffer[i] == '\r'){
						desc[k] = '\0';
						i = BUFSIZ;
					}else{
						desc[k] = line_buffer[i];
						k++;
					}
				}
			}
			loadValue(field, desc, &b);
		}
		in_desc = 0;
		in_value = 0;
	}
}

void loadValue(char* field, char* desc, NrrdObject* nb){
	int i;
	if(strcmp(field, "type") == 0){
		nb->LoadType(desc);
	}else if(strcmp(field, "dimension") == 0){
		nb->LoadDimension(desc);
	}else if(strcmp(field, "sizes") == 0){
		nb->LoadSizes(desc);
	}else if(strcmp(field, "spacings") == 0){
		nb->LoadSpacing(desc);
	}else if(strcmp(field, "data file") == 0){
		nb->dataFileName = desc;
	}else if(strcmp(field, "endian") == 0){
		nb->LoadEndian(desc);
	}else if(strcmp(field, "encoding") == 0){
		nb->LoadEncoding(desc);
		printf("encoding: %d\n", nb->encoding);
	}
}
