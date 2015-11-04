#include "knn.hpp"


size_t getFileSize(FILE * input)
{
	size_t initial = ftell(input);

	fseek(input, 0, SEEK_END);
	size_t fileSize = ftell(input);

	fseek(input, initial, SEEK_SET);
	return fileSize;
}


void ** getDatabase(int * dataCount)
{
	FILE * input = fopen("database.dat", "r");
	if (input==NULL) {
		printf("Unnable to open file!\n");
		return NULL;
	}

	void ** database = NULL;
	*dataCount = 0;

	long fileSize = getFileSize(input);
	while (ftell(input) != fileSize){
		ulong auxTotal;
		if (fread(&auxTotal, sizeof(ulong), 1, input)!=1)
			printf("Reading Error!");

		database = (void**) realloc(database, (*dataCount + 1) * sizeof(void*));
		database[*dataCount] = malloc(sizeof(ulong) + (auxTotal+1)*sizeof(double) + sizeof(int));

		memcpy(database[*dataCount], &auxTotal, sizeof(ulong));
		if (fread(database[*dataCount]+sizeof(ulong), (sizeof(double)*auxTotal), 1, input)!=1 ||
		fread(database[*dataCount]+sizeof(ulong)+(sizeof(double)*auxTotal), sizeof(int), 1, input)!=1)
			printf("Reading Error!");
		memset(database[(*dataCount)++]+sizeof(ulong)+(sizeof(double)*auxTotal)+sizeof(int), 0, sizeof(double));
	}

	return database;
}



int compareVotes(void * A, void * B, ulong total)
{
	return 
	(*((Candidate**)A))->votes >
	(*((Candidate**)B))->votes;
}

int compareDist(void * A, void * B, ulong total)
{
	return
	*((double*)((*((void**)A)) + sizeof(ulong)+(sizeof(double)*total)+sizeof(int))) <
	*((double*)((*((void**)B)) + sizeof(ulong)+(sizeof(double)*total)+sizeof(int)));
}

void mergeSort(void * vect, size_t n, size_t size, int (*compare)(void *, void *, ulong), ulong total)
{
	size_t lowerHalf = n >> 1;
	if (lowerHalf > 1) mergeSort(vect, lowerHalf, size, compare, total);

	size_t upperHalf = n - lowerHalf;
	void * upperVect = vect + (size * lowerHalf);
	if (upperHalf > 1) mergeSort(upperVect, upperHalf, size, compare, total);

	void * auxVect = malloc (n * size);
	size_t lowerIndex, upperIndex, auxIndex;
	for  (lowerIndex = 0, upperIndex = 0, auxIndex = 0;
		(auxIndex < n) && (lowerIndex < lowerHalf) && (upperIndex < upperHalf);
		auxIndex++)
			memcpy (auxVect + (auxIndex * size),
				   compare(upperVect + (upperIndex * size), vect + (lowerIndex * size), total) ?
				   upperVect + (upperIndex++ * size) : vect + (lowerIndex++ * size),
				   size);

	memcpy (auxVect + (auxIndex * size),
		  (lowerIndex < lowerHalf) ? vect + (lowerIndex * size) : upperVect + (upperIndex * size),
		  (n-auxIndex) * size);

	memcpy (vect, auxVect, n * size);
	free(auxVect);
}



double getDist(double * A, double * B, ulong size)
{
	double sum = 0;
	for (ulong i=0; i<size; i++) sum+=(A[i]-B[i])*(A[i]-B[i]);

	return sqrt(sum);
}


void calcDists(void ** database, int dataCount, Histogram * histA)
{
	for (int i=0; i<dataCount; i++){
		double dist = getDist(histA->elements, (double*)(database[i]+sizeof(ulong)), histA->total);
		memcpy(database[i]+sizeof(ulong)+(sizeof(double)*(histA->total))+sizeof(int), &dist, sizeof(double));
	}

	mergeSort(database, dataCount, sizeof(void*), &compareDist, histA->total);
}



Candidate * election(void ** database, int dataCount, Histogram * hist, int k)
{
	Candidate ** elec = NULL;
	int elecCount = 0;
	for (int i=0; i<k; i++){
		int exists, j;
		for (j=0, exists=0; j<elecCount && exists==0 && j<dataCount; j++)
			if (*((int*)(database[i]+sizeof(ulong)+(sizeof(double)*(hist->total)))) ==
			elec[j]->value)
				exists = 1;
		
		if (exists) elec[j-1]->votes++;
		else {
			elec = (Candidate**) realloc(elec, (elecCount + 1) * sizeof(Candidate*));
			elec[elecCount] = (Candidate*) malloc(sizeof(Candidate));
			elec[elecCount]->value = *((int*)(database[i]+sizeof(ulong)+(sizeof(double)*(hist->total))));
			elec[elecCount++]->votes = 1;
		}
	}
	mergeSort(elec, elecCount, sizeof(Candidate*), &compareVotes, 0);

	Candidate * output = (Candidate*) malloc(sizeof(Candidate));
	memcpy(output, elec[0], sizeof(Candidate));

	for (int i=0; i<elecCount; i++) free(elec[i]);
	free(elec);

	return output;
}


int getClass(void ** database, int dataCount, Histogram * hist, int k)
{
	calcDists(database, dataCount, hist);

	Candidate * auxCand = election(database, dataCount, hist, k);
	int output = auxCand->value;

	printf("Classe:\t%d Centavos %s (%s)\t(%05d)\nProbabilidade:\t%.3lf\t(%d/%d)\n\n",
	auxCand->value%1000,
	(auxCand->value/1000)%10 ? "Nova" : "Antiga",
	auxCand->value/10000 ? "Coroa" : "Cara",
	auxCand->value, auxCand->votes/(double)k, auxCand->votes, k);
	free(auxCand);
	/*
	void ** auxDatabase = (void**) malloc(dataCount*sizeof(void*));
	for (int i=0; i<dataCount; i++){
		auxDatabase[i] = malloc(sizeof(ulong) + (hist->total+1)*sizeof(double) + sizeof(int));
		memcpy(auxDatabase[i], database[i], sizeof(ulong) + (hist->total+1)*sizeof(double) + sizeof(int));
		*((int*)(auxDatabase[i]+sizeof(ulong)+(sizeof(double)*(hist->total)))) =
		(*((int*)(auxDatabase[i]+sizeof(ulong)+(sizeof(double)*(hist->total)))))%1000;
	}

	auxCand = election(auxDatabase, dataCount, hist, k);

	printf("Valor:\t%d Centavos\nProbabilidade:\t%.3lf\t(%d/%d)\n",
	auxCand->value, auxCand->votes/(double)k, auxCand->votes, k);
	free(auxCand);
	*/
	return output;
}