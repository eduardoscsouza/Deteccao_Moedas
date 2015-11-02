#include "utils.hpp"


int validPos(int i, int j, int height, int width)
{
	return
	(i>=0 && j>=0 && i<height && j<width);
}



char ** createMov(int neighType)
{
	char ** movement = (char**) malloc(neighType * sizeof(char*));
	for (int i=0; i<neighType; i++) movement[i]=(char*) malloc(2*sizeof(char));

	if (neighType==4){
		movement[0][0]=0; movement[0][1]=1;
		movement[1][0]=1; movement[1][1]=0;
		movement[2][0]=0; movement[2][1]=-1;
		movement[3][0]=-1;movement[3][1]=0;
	}
	else if(neighType==8){
		movement[0][0]=0; movement[0][1]=1;
		movement[1][0]=-1;movement[1][1]=1;
		movement[2][0]=-1;movement[2][1]=0;
		movement[3][0]=-1;movement[3][1]=-1;
		movement[4][0]=0; movement[4][1]=-1;
		movement[5][0]=1; movement[5][1]=-1;
		movement[6][0]=1; movement[6][1]=0;
		movement[7][0]=1; movement[7][1]=1;
	}

	return movement;
}


void freeMov(char ** movement, int neighType)
{
	for (int i=0; i<neighType; i++) free(movement[i]);
	free(movement);
}