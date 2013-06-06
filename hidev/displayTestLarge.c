#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <wiringPi.h>
#include <pthread.h>
//#include <printScreen.h> //custom lodev library
/*Every other size or position value is based on the following. We might add or remove some as we write the program.*/
#define ARRAY_HEIGHT  8 //change these to the matrix size
#define ARRAY_WIDTH  8
#define TOP_MARGIN  0.0 /*The margins bound the playable space in the array -- they might hold things like score*/
#define BOTTOM_MARGIN  0.0
#define LEFT_MARGIN  0.0
#define RIGHT_MARGIN  0.0
#define DISTANCE_PER_REFRESH  1.0

#define BOTTOM_END  ARRAY_HEIGHT-BOTTOM_MARGIN-1
#define RIGHT_END  ARRAY_WIDTH-RIGHT_MARGIN-1

//#### PIN DECLARATIONS ####
#define	COL 0
#define ROW 1
#define LATCH 2
#define COLCLK 3
#define ROWCLK 4

//#### BEGINNING OF printScreen LIBRARY ####
//printScreen.c -- prints all required pixels onto a the screen. Scans in rows from top-to-bottom.

void yClock(void) {
	digitalWrite(COLCLK, HIGH);
	digitalWrite(COLCLK, LOW);
	return;
}

void xClock(void) {
	digitalWrite(ROWCLK, HIGH);
	digitalWrite(ROWCLK, LOW);
	return;
}

void outputToScreen(void) {
	digitalWrite(LATCH, HIGH); 
	digitalWrite(LATCH, LOW);
	return;
}

void flushAllRegisters(void) { //clears all data from shift registers (but doesn't show this on screen)
	int y = 0, x = 0; //x=#cols,y=#rows
	digitalWrite(COL, LOW); 
	digitalWrite(ROW, LOW); 
	for(x = 0; x <= ARRAY_WIDTH; x++) { //assuming array is strictly as wide or wider than tall
		yClock();
		xClock();
	}
	return;
}

void flushRowRegisters(void) { //clears all data from shift registers (but doesn't show this on screen)
	int x = 0; //x=#cols,y=#rows
	digitalWrite(ROW, LOW); //1 = "pin one" on Raspi --> x-"data" pin
	for(x = 0; x <= ARRAY_WIDTH; x++){
		xClock();
	}
	return;
}


void printScreen(bool (*matrix)[ARRAY_WIDTH]){//scans downward, across screen ONE FULL TIME.
	for(int y = ARRAY_HEIGHT-1; y >= 0; y--) { //making assumption of matrix form matrixPtr[x][y]
		for(int x = ARRAY_WIDTH;x >= 0; x--){
			digitalWrite(1, (matrix[y][x])); //1 = "pin one" on Raspi --> x-"data" pin
			xClock();
			//usleep(10000);
		}
		if(y==0){ 
				digitalWrite(COL, HIGH); //0 = "pin zero" on RasPi --> y-"data" pin
				yClock();
				digitalWrite(COL, LOW);
		}
		else yClock(); //shifts the data over to make sure the proper column is lit
		outputToScreen();
		usleep(520); // leaves the screen on for a while before the next line is lit = ~60fps
	}
	return;
}

void print_test(bool (*array)[ARRAY_WIDTH]){
    int i, j;
    for(i = 0; i < ARRAY_HEIGHT; i++){
        for(j = 0; j < ARRAY_WIDTH; j++){
            if(array[i][j] == true){
                printf(" #", array[i][j]);
            }
            else{
                printf("  ", array[i][j]);
            }
        }
        printf("\n");
    }
 return;
}


void *printScreenImplement(void *vptr_value){//matrixPtr points to a bool 8x8 2-d array.
	wiringPiSetup();
	for (int i = 0; i<=4; i++){
	pinMode(i, OUTPUT);
	}
	bool (*matrixPtr)[ARRAY_WIDTH] = (bool (*)[ARRAY_WIDTH]) vptr_value;
	flushAllRegisters(); 
	while(1) {
		//print_test(matrixPtr);
		printScreen(matrixPtr);
		//system("clear");
	}
}

int MainScreen(bool (*matrixPtr)[ARRAY_WIDTH]){//matrixPtr points to a bool 2-d array. Points containing true interpreted on, false is off.
	pthread_t tid;
	pthread_create(&tid, NULL, printScreenImplement, (void *) matrixPtr[ARRAY_WIDTH]);
	return tid;
}

//##### END OF printScreen LIBRARY ####

void cleanArray(int y, int x, bool array[ARRAY_HEIGHT][ARRAY_WIDTH]){
	array[y][x] = false;
	return;
}

void updateArray(int y, int x, bool array[ARRAY_HEIGHT][ARRAY_WIDTH]){
	array[y][x] = true;
	return;
}

int main (void){
/*initialization start*/
	wiringPiSetup();	//
	bool array[ARRAY_HEIGHT][ARRAY_WIDTH] = {false};
	bool (*arrayPtr)[ARRAY_WIDTH] = array;
	int x=0, y=0;
/*initialization end*/
	int tid=MainScreen(arrayPtr);
	while (1){
	cleanArray(x,y,array);
	if (y==ARRAY_HEIGHT-1 && x==ARRAY_WIDTH-1){x=0;y=0;}
	else if (y==ARRAY_HEIGHT-1 && x<ARRAY_WIDTH-1){y=0;x++;}
	else if (y<ARRAY_HEIGHT-1)y++;
	updateArray(x,y,array);
	//print_test(arrayPtr);
	usleep(35000);
	//system("clear");
	
		
	}
	return 0;
}
