


#ifndef __FONT_H__
#define __FONT_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "Utils.h"

#define FNT_ALLOC_ERROR (-1)
#define FNT_NULLPTR_ERROR (-2)
#define FNT_RANGE_ERROR (-3)


typedef struct __Font {
	int width;
	int height;
	// TODO: bool doesn't use less memory
	bool*** bits;
} Font;

// Use the parser to generate the file. Use "default.bcm" for testing
int Font_Init(Font* font, char* filename);
void Font_Term(Font* font);

Font* Font_Initialize(char* filename);
void Font_Terminate(Font** font);

int Font_Resize(Font* font, int factor);


#endif


