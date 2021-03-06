

#include "Bmp.h"



int ReadBmpHeader(FILE* fp, int* width, int* height){
	if (ReadShort(fp) != 0x424D){
		printf("BMP: File is not a bitmap\n");
		return -1;
	}

	int file_size = ReadIntLE(fp);
	ReadIntLE(fp); // reserved
	int offset = ReadIntLE(fp);
	int header_size = ReadIntLE(fp);
	*width = ReadIntLE(fp);
	*height = ReadIntLE(fp);

	if (ReadShortLE(fp) != 1){
		printf("BMP: More than one plane in the bitmap\n");
		return -1;
	}
	if (ReadShortLE(fp) != 1){
		printf("BMP: Only monochrome bitmap supported\n");
		return -1;
	}
	if (ReadIntLE(fp) != 0){
		printf("BMP: File compression not supported\n");
		return -1;
	}

	int i;
	// go to data
	for (i = 0x22; i < offset; i++){
		fgetc(fp);
	}

	return 0;

}

void ReadBmpData(FILE* fp, int width, int height, bool** bits){

	int i, j;
	int y = 0;
	int x = 0;
	char c;
	for (i = 0; i < width * height / 8; i++){
		c = ReadIntLE(fp);
		
		bool b[8];
		for (j = 0; j < 8; j++){
			b[i] = ((c >> (8-j)) & 0x1);

			bits[x][height - y - 1] = b[i];
			x++;
			if (x %(width) == 0){
				y++;
				x -= width;
			}
		}
		
	}


}


int Bmp_Init(Bmp* bmp, char* filename){
	
	FILE* fp = fopen(filename, "rb");
	
	int err = ReadBmpHeader(fp, &(bmp->width), &(bmp->height));
	if (err != 0){
		fclose(fp);
		return err;
	}

	bmp->pixels = (bool**) malloc(bmp->width * sizeof(bool*));
	int i, j;	
	for (i = 0; i < bmp->width; i++){
		bmp->pixels[i] = (bool*) malloc(bmp->height * sizeof(bool));
	}

	ReadBmpData(fp, bmp->width, bmp->height, bmp->pixels);

	fclose(fp);

	
	

	return 0;
}

Bmp* Bmp_Initialize(char* filename){
	Bmp* bmp = (Bmp*) malloc(sizeof(Bmp));

	FILE* fp = fopen(filename, "rb");
	int err = ReadBmpHeader(fp, &(bmp->width), &(bmp->height));
	if (err != 0){
		fclose(fp);
		free(bmp);
		return NULL;
	}
	
	bmp->pixels = (bool**) malloc(bmp->width * sizeof(bool*));
	int i, j;	
	for (i = 0; i < bmp->width; i++){
		bmp->pixels[i] = (bool*) malloc(bmp->height * sizeof(bool));
	}

	ReadBmpData(fp, bmp->width, bmp->height, bmp->pixels);

	fclose(fp);

	

	
	return bmp;

}

void Bmp_Term(Bmp* bmp){
	int i;
	for(i = 0; i < bmp->width; i++){
		free(bmp->pixels[i]);
	}
	free(bmp->pixels);
	bmp->pixels = NULL;

}
void Bmp_Terminate(Bmp** bmp){

	int i;
	for(i = 0; i < (*bmp)->width; i++){
		free((*bmp)->pixels[i]);
	}
	free((*bmp)->pixels);


	free(*bmp);
	*bmp = NULL;
}

