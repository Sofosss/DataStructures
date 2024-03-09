/************************************************* 
* PART II.C: Hashing with chained (linked) lists *
**************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define M 11 // Size of Hash table


struct dateVolume // Data record stored in Hash table Bucket list
{
    char Date[11];
    int Volume;
};
typedef struct dateVolume dataItem;

struct listNode // Bucket list node
{
    dataItem data;
    struct listNode *next;
};
typedef struct listNode lNode;

lNode *hashTable[M] = {NULL}; // Hash table of M buckets initially empty


// Declaration of functions

void HashingMenu(int argc, char *argv[]);
void readFileToHashTable(int argc, char *argv[]);

lNode *createlNode(dataItem x);
int hashValue(char x[11]);

void insertToHashTable(dataItem x);
lNode *searchHashTable(char x[11]);
void deleteFromHashTable(char x[11]);

void displayHashTable();



int main(int argc, char *argv[])
{
	HashingMenu(argc, argv);
    return 0;
}


// Hashing menu
void HashingMenu(int argc, char *argv[])
{
    readFileToHashTable(argc, argv);

    int selection;
    char x[11];
    lNode *n;
	dataItem d;


    while (1)
    {
        printf("1. Search volume for a given date");
        printf("\n2. Modify volume for a given date");
        printf("\n3. Delete data record of a given date");
        printf("\n4. Display Hash table contents and Exit\n");
		  
		printf("\nEnter your choice (1 - 4): ");
        scanf("%d",&selection);   

        switch (selection)
        {
            case 1 :
                    printf("\n\nGive the date (yyyy-mm-dd): ");
                    scanf("%s", x);

                    n = searchHashTable(x);
                	if (!n)
                        printf("\nThis date does not exist in Hash table\n\n\n");
                    else
                        printf("\nVolume for the given date is: %d\n\n\n", n->data.Volume);
                    break;

            case 2 :
                    printf("\n\nGive the date (yyyy-mm-dd): ");
                    scanf("%s", x);

                    n = searchHashTable(x);
                    if (!n)
                        printf("\nThis date does not exist in Hash table\n\n\n");
                    else
                    {
                        printf("\nCurrent record: %s | %d", n->data.Date, n->data.Volume);						
						printf("\n\nGive the NEW volume (>= 0): ");
                        scanf("%d", &n->data.Volume);
                        printf("\nVolume modified\n\n\n");
                    }
                    break;

            case 3 :
                    printf("\n\nGive the date (yyyy-mm-dd): ");
                    scanf("%s", x);

                    n = searchHashTable(x);
                    if (!n)
                        printf("\nThis date does not exist in Hash table\n\n\n");
                    else
                    {
                        deleteFromHashTable(x);
                        printf("\nDate found and deleted\n\n\n");
                    }
                    break;
                        
            case 4 :
                    displayHashTable();
					return;
                        
            default :
                	printf("\nWrong option, try again ...\n\n\n");
                    break;
        }
    } 
}


// Read the file and store data records to a Hash table using chaining with linked lists
void readFileToHashTable(int argc, char *argv[])
{
    FILE *fp;
	char *fileName; 
	char line[80];  
    lNode *n;
	dataItem dt;
    float a, b, c, d;
    int e;

    if (argc >= 2)  // Data filename passed as a command line argument
        fileName = strdup(argv[1]);
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
        sscanf(line, "%10s,%f,%f,%f,%f,%d,%d", dt.Date, &a, &b, &c, &d, &dt.Volume, &e); // a, b, c, d and e are dummy variables
		insertToHashTable(dt);
    }
    
	free(fileName);
    fclose(fp);
}
	

// Allocate memory to a new list node 	
lNode *createlNode(dataItem x) 
{
    lNode *n = (lNode *) malloc(sizeof(lNode));
    if (!n)
    {
        printf("\nERROR: Memory failure\n\n");
    exit (1);
     }
    n->data = x;
	n->next = NULL;

    return n;
}


// Compute hash value 
int hashValue(char s[11])
{
	int v = 0;
	
	for (int i = 0; i < strlen(s); i++)
		v = v + s[i];
	
	return v%M;
}


// Insert a data record to Hash table  
void insertToHashTable(dataItem x) 
{
	lNode *n = searchHashTable(x.Date);
	
	if (!n) // Duplicates are not allowed in the Hash table. Actually, as the Date value of each data record is unique, we have no duplicates  
	{
		int hashIndex = hashValue(x.Date); // hashIndex is the bucket number
		n = createlNode(x);
	
		// Add a new node to an empty bucket list  
		if (!hashTable[hashIndex])
			hashTable[hashIndex] = n;
		else
		{
			// Add a new node to bucket list  
			n->next = hashTable[hashIndex];
			// Update the head node of bucket list  
			hashTable[hashIndex] = n;
		}
	}
}


// Search the Hash table for a given date 
lNode *searchHashTable(char x[11]) 
{
	int hashIndex = hashValue(x);
	lNode *n;	    
		
	n = hashTable[hashIndex];
	if (!n)
	    return NULL; // Search unsuccessful
	
	while (n) 
	{
	    if (strcmp(n->data.Date, x) == 0) 
			return n; // Search successful
        else
			n = n->next;
	}
	return NULL; // Search unsuccessful
}


// Delete an existing data record from Hash table
void deleteFromHashTable(char x[11]) 
{
	// Locate the bucket using hash index  
	int hashIndex = hashValue(x);
	lNode *n, *p;
	
	// Get the head node from current bucket list  
	n = hashTable[hashIndex];

	p = n;  // n points to current list node and p to previous list node
	while (n) 
	{
		// Delete node with given date
		if (strcmp(n->data.Date, x) == 0)
		{
			if (n == hashTable[hashIndex]) // Delete the head node from bucket list
				hashTable[hashIndex] = n->next;
			else
				p->next = n->next;
			free(n);
			return;
		}
		else // ��ve to next list node
		{
			p = n;
			n = n->next;
		}
    }  
}


// Display Hash table contents
void displayHashTable()
{
	lNode *n;
	
	printf("\n\nHash Table contents:\n");
	printf("--------------------");
	
	for (int i = 0; i < M; i++)
	{
		printf("\n\n...............\n");
		printf("Bucket [%d]", i);	
		n = hashTable[i];
		
		if (!n)
		    printf(" -|");
		else
		{
			printf("  ->\t");
			while (n)
			{
				printf("%s, %d | ", n->data.Date, n->data.Volume);
				n = n->next;
			}
	    }
	}
	printf("\n\n");
}

	
	
