/*******************************************************
* PART I.3: Binary and Interpolation search algorithms *
*******************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <chrono> // C++ library to measure the running time of each algorithm using the clock with the highest resolution available  
#include <unistd.h>

#define MAX_SIZE 3240 // Max size of the array stored the daily transactions


struct dailyStockData 
{
    char Date[11];
    float Open, High, Low, Close;
    int Volume, OpenInt;
};
typedef struct dailyStockData dataItem;

int (*searchAlgPtr)(dataItem arr[], int l, int r, char x[]); // Pointer to functions implementing binarySearch and interpolationSearch algorithms

int steps; // Number of steps made by algorithm


// Declaration of functions
int readFile(dataItem arr[], int argc, char *argv[]);

int binarySearch(dataItem arr[], int l, int r, char x[]);
int interpolationSearch(dataItem arr[], int l, int r, char x[]);

unsigned long val(char s[]);

int main(int argc, char *argv[])
{
    dataItem S[MAX_SIZE];

    if (argc >= 2)
	{
	    if (strcmp(argv[1], "binarySearch") == 0)
		searchAlgPtr = &binarySearch;
	    else if (strcmp(argv[1], "interpolationSearch") == 0)
		searchAlgPtr = &interpolationSearch;
	    else
	    {
		printf("Invalid algorithm specified. Please use 'binarySearch' or 'interpolationSearch'.\n");
		return 1;
	    }
	}
	else
	{
	    printf("Please specify the sorting algorithm to use as a command line argument. Options: 'binarySearch', 'interpolationSearch'.\n");
	    return 1;
	}
    
    int N = readFile(S, argc, argv); // N is the number of daily transactions read from file

    char x[11];
    int pos; // Position of search key in the array or -1 if not found

    printf("Give the date to search for (yyyy-mm-dd): ");
    scanf("%s", x);

    typedef std :: chrono :: high_resolution_clock clock;
    // Start measuring running time
    auto startTime = clock :: now(); 
    
    pos = (*searchAlgPtr)(S, 0, N-1, x);  
	
    // Stop measuring running time and calculate the elapsed time
    auto endTime = clock :: now();
    auto elapsedTime = std :: chrono :: duration_cast<std :: chrono :: nanoseconds>(endTime - startTime).count();
		
    if (searchAlgPtr == &binarySearch) 
	printf("\n\n[BINARY SEARCH]\n");
    if (searchAlgPtr == &interpolationSearch)
	printf("\n\n[INTERPOLATION SEARCH]\n");

    if (pos == -1)
        printf("\nDate %s not found.\nNumber of steps: %d\n", x, steps);
    else
        printf("\nDate %s found at array position %d. Volume: %d\nNumber of steps: %d\n", x, pos, S[pos].Volume, steps);
    printf("Running time measured: %g seconds\n", (double)elapsedTime*1e-9);
	
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


// Binary search algorithm 
int binarySearch(dataItem arr[], int l, int r, char x[])
{
    int m;

    steps = 0;

    while (l <= r)
    {
        steps++;
        m = (l+r)/2; // probe index

        if (strcmp(arr[m].Date, x) == 0)
            return m; // Search successful
        else if (strcmp(arr[m].Date, x) < 0)
                 l = m+1;
             else
                 r = m-1;
    }

    return -1; // Search unsuccessful
}


// Helping function to convert a Date field to its integer value
unsigned long val(char s[]) 
{
    int i, j, k = 0;
    char d[strlen(s)-2];

    i = strlen(s);

    for (j=0; j<i; j++)
    {
        if (isdigit(s[j]))
        {
            d[k] = s[j];
            k++;
        }
    }	
    
    d[k] = '\0';
    return (unsigned long)atoi(d);
}


// Interpolation search algorithm
int interpolationSearch(dataItem arr[], int l, int r, char x[])
{
    int m;

    steps = 1;

    while (strcmp(arr[r].Date, x) >= 0 && strcmp(x, arr[l].Date) > 0)
    {
        steps++;
        m = l + (r-l) * (val(x) - val(arr[l].Date)) / (val(arr[r].Date) - val(arr[l].Date)); // probe index

        if (strcmp(arr[m].Date, x) < 0)
            l = m+1;
        else if (strcmp(arr[m].Date, x) > 0)
                 r = m-1;
             else
                 l = m;
    }

    if (strcmp(x, arr[l].Date) == 0)
        return l; // Search successful
    else
        return -1; // Search unsuccessful
}

