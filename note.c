#include <stdlib.h>

void newNoteArrey(int arrayLength);

void initializeNote(int arrayLength, int timing, int length, int kind);
void initializeY(int arrayLength);

void setY(int arrayLength, int y);

int getY(int arrayLength);
int getOldY(int arrayLength);
int getTiming(int arrayLength);
int getLength(int arrayLength);
int getKind(int arrayLength);

void downY(int arrayLength);

struct _note{
	
	int y;
	int old_y;
	int timing;
	int length;
	int kind;
	
};

struct _note *array;

void newNoteArrey(int arrayLength){
	
	int i;
	
	array = (struct _note *)malloc(arrayLength * sizeof(struct _note));
	
	for(i = 0; i < arrayLength; i++){
		initializeY(i);
	}
}

void initializeNote(int arrayLength, int timing, int length, int kind){
	array[arrayLength].timing = timing;
	array[arrayLength].length = length;
	array[arrayLength].kind = kind;
}

void initializeY(int arrayLength){
	array[arrayLength].y = 0;
	array[arrayLength].old_y = 0;
}

void setY(int arrayLength, int y){
	array[arrayLength].y = y;
}

int getY(int arrayLength){
	return array[arrayLength].y;
}

int getOldY(int arrayLength){
	return array[arrayLength].old_y;
}

int getTiming(int arrayLength){
	return array[arrayLength].timing;
}

int getLength(int arrayLength){
	return array[arrayLength].length;
}

int getKind(int arrayLength){
	if(arrayLength == 0)
		return 0;
	return array[arrayLength].kind;
}

void downY(int arrayLength){
	array[arrayLength].old_y = array[arrayLength].y;
	array[arrayLength].y++;
}