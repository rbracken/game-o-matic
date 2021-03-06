

#include "Drawing.h"



#define CHECK_NULLPTR_ERROR(x) \
	do { \
		if (!x){ \
			return DRW_NULLPTR_ERROR; \
		} \
	} while(0)

int Drawing_Init(Drawing* drawing, float* x, float* y, int N, int type, double outline, double fill, bool transparent_fill){


	drawing->N = N;
	drawing->type = type;
	drawing->outline = outline;
	drawing->fill = fill;
	drawing->transparent = transparent_fill;

	drawing->x = (float*) malloc(N * sizeof(float));
	drawing->y = (float*) malloc(N * sizeof(float));

	int i;	
	for (i = 0; i < drawing->N; i++){
		drawing->x[i] = x[i];
		drawing->y[i] = y[i];
	}
	return 0;
}

Drawing* Drawing_Initialize(float* x, float* y, int N, int type, double outline, double fill, bool transparent_fill){

	Drawing* drawing = (Drawing*) malloc(sizeof(Drawing));


	drawing->N = N;
	drawing->type = type;
	drawing->outline = outline;
	drawing->fill = fill;
	drawing->transparent = transparent_fill;

	drawing->x = (float*) malloc(N * sizeof(float));
	drawing->y = (float*) malloc(N * sizeof(float));

	int i;	
	for (i = 0; i < drawing->N; i++){
		drawing->x[i] = x[i];
		drawing->y[i] = y[i];
	}

	return drawing; 
}

void Drawing_Term(Drawing* drawing){

	drawing->N = 0;

	free(drawing->x);
	free(drawing->y);
	drawing->x = NULL;
	drawing->y = NULL;

}

void Drawing_Terminate(Drawing** drawing){

	free((*drawing)->x);
	free((*drawing)->y);

	free(*drawing);
	*drawing = NULL;
}


int Drawing_GetVertices(Drawing* drawing, int x, int y, float ratio, int* x_list, int* y_list){

	CHECK_NULLPTR_ERROR(drawing);
	CHECK_NULLPTR_ERROR(x_list);
	CHECK_NULLPTR_ERROR(y_list);


	int i;
	for (i = 0; i < drawing->N; i++){
		x_list[i] = x + lround(ratio * drawing->x[i]);
		y_list[i] = y + lround(ratio * drawing->y[i]);
	}


	return 0;
}


int Drawing_Rotate(Drawing* drawing, float angle){

	CHECK_NULLPTR_ERROR(drawing);
	
	int i;
	float x, y;
	for (i = 0; i < drawing->N; i++){
		x = drawing->x[i];
		y = drawing->y[i];
		drawing->x[i] = cos(angle)*x - sin(angle)*y;
		drawing->y[i] = sin(angle)*x + cos(angle)*y;
	}

	return 0;

}
int Drawing_Scale(Drawing* drawing, float x_factor, float y_factor){

	CHECK_NULLPTR_ERROR(drawing);

	int i;
	for (i = 0; i < drawing->N; i++){
		drawing->x[i] *= x_factor;
		drawing->y[i] *= y_factor;
	}

	return 0;
}
int Drawing_ShearHrzn(Drawing* drawing, float factor){

	CHECK_NULLPTR_ERROR(drawing);

	int i;
	float x, y;
	for (i = 0; i < drawing->N; i++){
		x = drawing->x[i];
		y = drawing->y[i];
		drawing->x[i] = x + factor * y;
		drawing->y[i] = y;
	}

}
int Drawing_ShearVert(Drawing* drawing, float factor){

	CHECK_NULLPTR_ERROR(drawing);
	
	int i;
	float x, y;
	for (i = 0; i < drawing->N; i++){
		x = drawing->x[i];
		y = drawing->y[i];
		drawing->x[i] = x;
		drawing->y[i] = y + factor * x;
	}
}

int Drawing_Reflect(Drawing* drawing, float lx, float ly){

	CHECK_NULLPTR_ERROR(drawing);

	int i;
	float x, y;
	for (i = 0; i < drawing->N; i++){
		x = drawing->x[i];
		y = drawing->y[i];
		drawing->x[i] = x * (lx*lx-ly*ly) + y * (2.0f*lx*ly);
		drawing->y[i] = x * (2.0f*lx*ly) + y * (ly*ly-lx*lx);
	}
	return 0;
}



