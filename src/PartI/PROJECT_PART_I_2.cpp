/*************************************************
* PART I.2: Heapsort and Countingsort algorithms *
**************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <chrono> // C++ library to measure the running time of each algorithm using the clock with the highest resolution available  

#define MAX_SIZE 3240 // Max size of the array stored the daily transactions


// Data record read from file
struct dailyStockData 
{
	char Date[11];
	float Open, High, Low, Close;
	int Volume, OpenInt;
};
typedef struct dailyStockData dataItem;

void (*sortAlgPtr)(dataItem arr[], int arrSize); // Pointer to functions implementing heapSort and countingSort algorithms

int comps = 0; // Number of comparisons made by heapSort


// Declaration of functions

int readFile(dataItem arr[], int argc, char *argv[]);

void heapSort(dataItem arr[], int arrSize);
void shiftDown(dataItem arr[], int i, int k);

void countingSort(dataItem arr[], int arrSize);
void countingSortExtended(dataItem arr[], int arrSize);

int cmpCloseDate(dataItem a, dataItem b);
void swap(dataItem *a, dataItem *b);
void printArray(dataItem arr[], int arrSize);



int main(int argc, char *argv[])
{
	dataItem S[MAX_SIZE];
    int N = readFile(S, argc, argv); // N is the number of daily transactions read from file

	sortAlgPtr = &heapSort; // sortAlgPtr = &countingSort; sortAlgPtr = &countingSortExtended;
	
	typedef std :: chrono :: high_resolution_clock clock;
    // Start measuring running time
    auto startTime = clock :: now(); 
	
	(*sortAlgPtr)(S, N);  
	
	// Stop measuring running time and calculate the elapsed time
    auto endTime = clock :: now();
    auto elapsedTime = std :: chrono :: duration_cast<std :: chrono :: nanoseconds>(endTime - startTime).count();
		
	if (*sortAlgPtr == heapSort) 
		printf("[HEAPSORT] ");
	if (*sortAlgPtr == countingSort)
		printf("[COUNTINGSORT] ");
	if (*sortAlgPtr == countingSortExtended) 
		printf("[COUNTINGSORT EXTENDED] ");
	
	printf("SORTED ARRAY:\n\n");
	printArray(S, N);
	
	if (*sortAlgPtr == heapSort)
		printf("\n\nNumber of comparisons: %d\n", comps);
	else
	    printf("\n");
	printf("Running time measured: %lg seconds\n", (double)elapsedTime * 1e-9);
	
	return 0;
}


// Open the file, read data records, store them to array arr and return the number of records read
int readFile(dataItem arr[], int argc, char *argv[])  
{
    FILE *fp; 
	char fileName[20], line[80];  
    int numLines = 0; // Number of lines read from file
    dataItem dt;
 
    if (argc >= 2)  // Data filename passed as command line argument
	    strcpy(fileName, argv[1]);
	else
	{
		printf("Give the stock data filename: "); // Data filename asked by user
        scanf("%s", fileName); printf("\n\n");
    } 
    
	fp = fopen(fileName, "r");
	if (!fp)
	{
		printf( "\nERROR: can't open file\n");
		exit(1);
	}
    
	fgets(line, 80, fp); // Get the first line

	while (fgets(line, 80, fp))
    {
        sscanf(line, "%10s,%f,%f,%f,%f,%d,%d", dt.Date,  &dt.Open, &dt.High, &dt.Low, &dt.Close, &dt.Volume, &dt.OpenInt);
        arr[numLines] = dt;
        numLines++;
    }

    fclose(fp);
    return numLines; // Number of daily transactions read 
}


// Print array data records 
void printArray(dataItem arr[], int arrSize)
{
    printf("Date       : \tOpen\tHigh  \tLow   \tClose \tVolume\n");
    printf("--------------------------------------------------------\n");
    for (int i = 0; i < arrSize; i++)
    {
        printf("%s : \t%.3f\t%.3f\t%.3f\t%.3f\t%d\n", arr[i].Date,  arr[i].Open, arr[i].High, arr[i].Low, arr[i].Close, arr[i].Volume);
    }
}


/* // Print Close field of array data records - It is used for the screenshots  
void printArray(dataItem arr[], int arrSize)
{
    for (int i = 0; i < arrSize; i++)
        printf("%.3f | ", arr[i].Close);
} */


// Utility function to swap the contents of two data records
void swap(dataItem *a, dataItem *b)
{
    dataItem temp;

    temp = *a;
    *a = *b;
    *b = temp;
}


// Heapsort algorithm 
void heapSort(dataItem arr[], int arrSize)
{
	// Build the heap
   	for (int i = arrSize/2; i > 0; i--)
     	shiftDown(arr, i, arrSize);

	// Move the largest element from root to rightmost leaf of the bottom level and rebuild the heap
	for (int i = arrSize; i > 1; i--)
	{
 	 	swap(&arr[i-1], &arr[0]);
      	shiftDown(arr, 1, i-1);
	}
}


// Shift down the root element in order to satisfy the heap property
void shiftDown(dataItem arr[], int root, int last)
{
 	 int j, k;
 	 dataItem v;

 	 v = arr[root-1];
 	 k = root;

     while (k <= last/2)
 	 {
         j = 2*k;
         if (j < last)
         {
             comps++;
             if (cmpCloseDate(arr[j], arr[j-1]) > 0)  
                j++;
         }
         comps++;
         if (cmpCloseDate(v, arr[j-1]) >= 0)  
             break;
         arr[k-1] = arr[j-1];
         k = j;
  	}
  	arr[k-1] = v;
}


// Utility function to compare pairs (Close, Date)
int cmpCloseDate(dataItem a, dataItem b)
{
	if (a.Close > b.Close)
		return 1;
	if (a.Close < b.Close)	
	    return -1;
  	if (strcmp(a.Date, b.Date) > 0)
	    return 1;
	if (strcmp(a.Date, b.Date) < 0)
		return -1;
	return 0;	 
}


// Counting sort algorithm on non negative integer values  
void countingSort(dataItem arr[], int arrSize)
{
    int i, j, v, max, min;
	max = arr[0].Close;
	min = max;

    for (i = 1; i < arrSize; i++) // Compute max, min of the values (int) value of Close field  
    {
        v = arr[i].Close;		
		if (v > max)
        	max = v;
		if (v < min)
        	min = v;
    }

    // C[i] finally counts the int values smaller than or equal to i+min
	int C[max-min+1] = {0}; // Initialize array C to 0

    for (j = 0; j < arrSize; j++)
    {
    	v = arr[j].Close;
		C[v-min]++;
    }

    for (i = 1; i < max-min+1; i++)
        C[i] = C[i] + C[i-1];

    dataItem B[arrSize]; // B stores the sorted data records
    for (j = arrSize-1; j >= 0; j--)
    {
    	v = arr[j].Close;
		B[C[v-min]-1] = arr[j];
     	C[v-min]--;

    }

  	for (i = 0; i < arrSize; i++)
    	arr[i] = B[i];  // Move temp array B to array arr
}


// Counting sort algorithm on non negative integer values extended
void countingSortExtended(dataItem arr[], int arrSize)
{
    int i, j, v, UpMax, UpMin;
	
	UpMax = 1000 * arr[0].Close;
	UpMin = UpMax;

    for (i = 1; i < arrSize; i++) // Compute UpMax, UpMin of the values 1.000 * value of Close field  
    {
        v = 1000 * arr[i].Close;
		if (v > UpMax)
        	UpMax = v;
        if (v < UpMin)
        	UpMin = v;
    }

	// C[i] finally counts the number of int values smaller than or equal to i+UpMin 
    int C[UpMax-UpMin+1] = {0}; // Initialize C array to 0

    for (j = 0; j < arrSize; j++)
    {
        v = 1000 * arr[j].Close;
        C[v-UpMin]++;
    }

    for (i = 1; i < UpMax-UpMin+1; i++)
        C[i] = C[i] + C[i-1];

    dataItem B[arrSize];
	for (j = arrSize-1; j >= 0; j--)
    {
    	v = 1000 * arr[j].Close;
    	B[C[v-UpMin]-1] = arr[j];
     	C[v-UpMin]--;
  	}

  	for (i = 0; i < arrSize; i++)
        arr[i] = B[i];  // Move B to arr
}
