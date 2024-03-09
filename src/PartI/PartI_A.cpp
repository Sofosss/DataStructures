/***********************************************
* PART I.1: Mergesort and Quicksort algorithms *
************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include <chrono> // C++ library to measure the running time of each algorithm using the clock with the highest resolution available (nanoseconds) 
#include <unistd.h>

#define MAX_SIZE 3240 // Max size of the array stored the daily transactions

// Data record read from file
struct dailyStockData 
{
	char Date[11];
	float Open, High, Low, Close;
	int Volume, OpenInt;
};
typedef struct dailyStockData dataItem;

void (*sortAlgPtr)(dataItem arr[], int l, int r); // Pointer to functions implementing mergeSort and quickSort algorithms

int comps = 0; // Number of comparisons made by algorithm


// Declaration of functions

int readFile(dataItem arr[], int argc, char *argv[]);

void mergeSort(dataItem arr[], int l, int r);
void merge(dataItem arr[], int l, int m, int r);

int partition(dataItem arr[], int l, int r);
void quickSort(dataItem arr[], int l, int r);
void setPivotAtRandom(dataItem arr[], int l, int r);
void randomizedQuickSort(dataItem arr[], int l, int r);
void setPivotToMedOfThree(dataItem arr[], int l, int r);
void medOfThreeQuickSort(dataItem arr[], int l, int r);

int cmpOpenDate(dataItem a, dataItem b);
void swap(dataItem *a, dataItem *b);
void printArray(dataItem arr[], int arrSize);



int main(int argc, char *argv[])
{
    dataItem S[MAX_SIZE];

    if (argc >= 2)
    {
        if (strcmp(argv[1], "mergeSort") == 0)
            sortAlgPtr = &mergeSort;
        else if (strcmp(argv[1], "quickSort") == 0)
            sortAlgPtr = &quickSort;
        else if (strcmp(argv[1], "randomizedQuickSort") == 0)
            sortAlgPtr = &randomizedQuickSort;
        else if (strcmp(argv[1], "medOfThreeQuickSort") == 0)
            sortAlgPtr = &medOfThreeQuickSort;
        else
        {
            printf("Invalid algorithm specified. Please use 'mergeSort', 'quickSort', 'randomizedQuickSort', or 'medOfThreeQuickSort'.\n");
            return 1;
        }
    }
    else
    {
        printf("Please specify the sorting algorithm to use as a command line argument. Options: 'mergeSort', 'quickSort', 'randomizedQuickSort', 'medOfThreeQuickSort'\n");
        return 1;
    }
    
    int N = readFile(S, argc, argv); // N is the number of daily transactions read from the file
    srand(time(0)); // Use the current time as seed for the random generator used in randomized QuickSort
    S[N].Open = FLT_MAX;
    strcpy(S[N].Date, "2021-05-19"); // Right guard needed in standard and randomized quickSort

    typedef std::chrono::high_resolution_clock clock;
    // Start measuring running time
    auto startTime = clock::now();

    (*sortAlgPtr)(S, 0, N - 1);

    // Stop measuring running time and calculate the elapsed time
    auto endTime = clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

    if (sortAlgPtr == &mergeSort)
        printf("[MERGESORT] ");
    else if (sortAlgPtr == &quickSort)
        printf("[STANDARD QUICKSORT] ");
    else if (sortAlgPtr == &randomizedQuickSort)
        printf("[RANDOMIZED QUICKSORT] ");
    else if (sortAlgPtr == &medOfThreeQuickSort)
        printf("[MEDIAN OF THREE QUICKSORT] ");

    printf("SORTED ARRAY:\n\n");
    printArray(S, N);

    printf("\n\nNumber of comparisons: %d\n", comps);
    printf("Running time measured: %lg seconds\n", static_cast<double>(elapsedTime) * 1e-9);

    return 0;
}


// Open the file, read data records, store them to array arr and return the number of records read
int readFile(dataItem arr[], int argc, char *argv[]) 
{
    FILE *fp; 
    char *fileName;
    char line[80]; 
    int numLines = 0; // Number of lines read from file
    dataItem dt;

    if (argc >= 3)  // Data filename passed as a command line argument
        fileName = strdup(argv[2]);
    else
    {
        printf("Give the stock data filename: "); // Data filename asked by user
        scanf("%ms", &fileName);
        printf("\n\n");
    } 

    // Check if the file exists
    if (access(fileName, F_OK) == -1)
    {
        printf("\nERROR: File '%s' not found\n", fileName);
        free(fileName);
        exit(1);
    }

    fp = fopen(fileName, "r");
    if (!fp) // fp == NULL
    {
        printf("\nERROR: can't open file\n");
        free(fileName);
        exit(1);
    }

    fgets(line, 80, fp); // Get the first line

    while (fgets(line, 80, fp))
    {
        sscanf(line, "%10s,%f,%f,%f,%f,%d,%d", dt.Date,  &dt.Open, &dt.High, &dt.Low, &dt.Close, &dt.Volume, &dt.OpenInt);
        arr[numLines] = dt;
        numLines++;
    } 

    free(fileName);
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


/* // Print Open field of array data records - It is used for the screenshots  
void printArray(dataItem arr[], int arrSize)
{
    for (int i = 0; i < arrSize; i++)
        printf("%.3f | ", arr[i].Open);
} */


// Merge sorted subarrays arr[l..m], arr[m+1..r]
void merge(dataItem arr[], int l, int m, int r)
{
    int n1 = m - l + 1, n2 = r - m;

    // Create temp arrays
    dataItem L[n1], R[n2];  // Temp arrays 

    // Copy data from arr to temp arrays L and R  
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Indexes initialization
    int i = 0, j = 0, k = l;

    // Merge the subarrays
    while (i < n1 && j < n2)
    {
        comps++;
        if (cmpOpenDate(L[i], R[j]) <= 0)   
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) 
	{
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}


// Mergesort algorithm
void mergeSort(dataItem arr[], int l, int r)
{
	if (l < r)
    {
    	int m = (l+r-1)/2;
    	mergeSort (arr, l, m); // Call mergesort recursively for left subarray 
    	mergeSort(arr, m+1, r); // Call mergesort recursively for right subarray 
    	merge(arr, l, m, r); // Merge left and right subarrays
	}
}


// Utility function to compare pairs (Open, Date)
int cmpOpenDate(dataItem a, dataItem b)
{
	if (a.Open > b.Open)
		return 1;
	if (a.Open < b.Open)	
	    return -1;
  	if (strcmp(a.Date, b.Date) > 0)
	    return 1; 
	if (strcmp(a.Date, b.Date) < 0)
		return -1;
	return 0;	 
}


// Utility function to swap the contents of two data records
void swap(dataItem *a, dataItem *b)
{
    dataItem temp;

    temp = *a;
    *a = *b;
    *b = temp;
}


// Standard quicksort algorithm  
void quickSort(dataItem arr[], int l, int r)
{
    if (l < r)
    {
		int k = partition(arr, l, r);
        quickSort(arr, l, k-1);
        quickSort(arr, k+1, r);
    }
}


// Randomized quicksort algorithm  
void randomizedQuickSort(dataItem arr[], int l, int r)
{
    if (l < r)
    {
		setPivotAtRandom(arr, l, r);
		int k = partition(arr, l, r);
        randomizedQuickSort(arr, l, k-1);
        randomizedQuickSort(arr, k+1, r);
    }
}


// Set pivot at random to an element of arr[l..r] and move it to position l; 
void setPivotAtRandom(dataItem arr[], int l, int r)
{
	int j = rand()%(r-l+1)+l;
	
	if (l != j)
	 	swap(&arr[l], &arr[j]);
}


// Quicksort algorithm using the median of three as pivot
void medOfThreeQuickSort(dataItem arr[], int l, int r)
{
    if (l < r)
    {
        setPivotToMedOfThree(arr, l, r);
		int k = partition(arr, l, r);
        medOfThreeQuickSort(arr, l, k-1);
        medOfThreeQuickSort(arr, k+1, r);
    }
}


// Set pivot to median of arr[l], arr[(l+m)/2] and arr[r] and move it to position l; 
void setPivotToMedOfThree(dataItem arr[], int l, int r)
{
	if (r - l >= 2)
	{
		comps +=3;
		int m = (l+r)/2;
		
		if (cmpOpenDate(arr[l], arr[m]) < 0)
		    swap(&arr[l], &arr[m]);
		if (cmpOpenDate(arr[l], arr[r]) > 0)
		    swap(&arr[l], &arr[r]);
		if (cmpOpenDate(arr[l], arr[m]) < 0)
		    swap(&arr[l], &arr[m]);
	}
}


// Partition array arr[l..r] to left and right subarrays arr[l..j-1] and arr[j+1..r] with elements of left subarray <= arr[j] and elements of right subarray >= arr[j]
// Return index j
int partition(dataItem arr[], int l, int r)
{
 	int i, j;
 	dataItem x;

    // arr[l] is the pivot
    i = l;
    j = r + 1;

    while (i < j)
    {
		do
        {
            i++;
            comps++;
        } while (cmpOpenDate(arr[i], arr[l]) < 0);  

        do
        {
            j--;
            comps++;
        } while (cmpOpenDate(arr[j], arr[l]) > 0);  

        if (i < j)
            swap(&arr[i], &arr[j]);
    }

    if (l != j)
	  	swap(&arr[l], &arr[j]);

    return j;
}

