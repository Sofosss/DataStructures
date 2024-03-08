/************************************************************************
* PART II.D: Consolidation of Binary Search Tree and Hashing operations *
************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define M 11  // Hash table size


// Data structures 

FILE *fp; // Pointer to data file


// Binary search tree data structures

struct dateVolume // Data record stored in binary tree node / bucket list node
{  
    char Date[11];
    int Volume;
};
typedef struct dateVolume dataItem;

struct binaryTreeNode // Binary Search Tree node implemented as AVL tree node
{  
    dataItem data; 
    struct binaryTreeNode *left;
    struct binaryTreeNode *right;
    int height;
};
typedef struct binaryTreeNode btNode;

btNode *root = NULL; // Root of the tree initially empty

int (*cmpPtr)(dataItem, dataItem); // Pointer to compare functions


// Hashing with chained lists data structures 

struct listNode // Bucket list node
{
    dataItem data;
    struct listNode *next;
};
typedef struct listNode lNode;

lNode *hashTable[M] = {NULL}; // Hash table of M buckets initially empty



/*******************************************************************/



// Declaration of functions

void mainMenu();


// Binary search tree functions

void binaryTreeByDateMenu();
void binaryTreeByVolumeMenu();
void binaryTreeMenu();
void openFile(int argc, char *argv[]);
void readFileToBinTree();

btNode *createbtNode(dataItem x);
int btNodeHeight(btNode *r);
int btNodeBalance(btNode *n);
btNode *rotateL(btNode *r);
btNode *rotateR(btNode *r);

btNode *searchBinTree(btNode *r, char x[11]);
btNode *insertToBinTree(btNode *r, dataItem x);
btNode *deleteFromBinTree(btNode *r, char x[]);
void reportBinTree(btNode *r, int x);
btNode *minValuebtNode(btNode *r);
btNode *maxValuebtNode(btNode *r);

int max(int a, int b);
void inorderBinTree(btNode *r);
void printBinTree(btNode *r, int k);

int cmpDate(dataItem a, dataItem b);
int cmpVolumeDate(dataItem a, dataItem b);


// Hashing with chained lists functions

void hashingMenu();
void readFileToHashTable();

lNode *createlNode(dataItem x);
int hashValue(char x[11]);

void insertToHashTable(dataItem x);
lNode *searchHashTable(char x[11]);
void deleteFromHashTable(char x[11]);

void displayHashTable();



/*******************************************************************/



int main(int argc, char *argv[])
{
    openFile(argc, argv); // Open inputa data file before any other action
	mainMenu();
	return 0;  
}


// Main user menu
void mainMenu()
{
	int selection;

    while (1)
	{
        printf("1. Read file to a Binary Search Tree");
        printf("\n2. Read file to a Hash Table");
          
		printf("\n\nEnter your choice (1 - 2): ");
        scanf("%d",&selection);

        switch (selection)
        {
            case 1 :
                    binaryTreeMenu();
                    return;

            case 2 :        
                    hashingMenu();
                    return;

            default :        
                    printf("\nWrong option, try again ...\n\n\n");
                    break;
        }
    }
}



/*******************************************************************/



// BINARY SEARCH TREE FUNCTIONS

// Binary tree menu
void binaryTreeMenu()
{
    int selection;

    while (1)
	{
          printf("\n\n1. Read file to a Binary Search Tree by Date");
          printf("\n2. Read file to a Binary Search Tree by Volume\n");
          
		  printf("\nEnter your choice (1 - 2): ");
          scanf("%d",&selection);

          switch (selection)
          {
                case 1 :
                        binaryTreeByDateMenu();
                        return;

                case 2 :        
                        binaryTreeByVolumeMenu();
                        return;

                default :        
                        printf("\nWrong option, try again ...\n");
                        break;
          }
     }
}


// Binary tree by Date menu: Read file by Date and display user menu
void binaryTreeByDateMenu()
{
    cmpPtr = &cmpDate; // The key of each data record is the field Date
	readFileToBinTree();

    int selection;
    char x[11];
    dataItem d;
    btNode *r;

    while (1)    
  	{
          printf("\n\n1. Inorder traversal of BST");
          printf("\n2. Search volume for a given date");
          printf("\n3. Modify volume for a given date");
		  printf("\n4. Delete BST node of a given date");
          printf("\n5. Exit\n");

          printf("\nEnter your choice (1 - 5): ");
          scanf("%d",&selection);
          
          switch (selection)
          {
                case 1 :
                        printf("\n\nDate         Volume\tHeight\n");
                        printf("------------------------------");
                        inorderBinTree(root);

                        printf("\n\n");system("pause");
						printf("\n\n\nTree structure:\n");
                        printf("---------------\n\n"); 
                        printBinTree(root, 1);
                        break;

                case 2 :
                        printf("\n\nGive the date (yyyy-mm-dd): ");
                        scanf("%s", x);

                        if (!root)
							printf("\nTree is empty\n\n");
						else
						{
							r = searchBinTree(root, x);
                        	if (!r)
                            	printf("\nThis date does not exist in the tree\n");
                        	else
                            	printf("\nVolume for the given date is: %d\n", r->data.Volume);
                        }
						break;

                case 3 :
                        printf("\n\nGive the date (yyyy-mm-dd): ");
                        scanf("%s", x);

                        if (!root)
							printf("\nTree is empty\n\n");
						else
						{	
							r = searchBinTree(root, x);
                        	if (!r)
                            	printf("\nThis date does not exist in the tree");
                        	else
                        	{
								printf("\nCurrent record: %s | %d", r->data.Date, r->data.Volume);
                            	printf("\n\nGive the new volume (>= 0): ");
                            	scanf("%d", &r->data.Volume);
                            	printf("\nVolume modified\n");
                        	}
                    	}
                        break;

                case 4 :
                        printf("\n\nGive the date (yyyy-mm-dd): ");
                        scanf("%s", x);

                        if (!root)
							printf("\nTree is empty\n\n");
						else
						{	
							r = searchBinTree(root, x);
                        	if (!r)
                            	printf("\nThis date does not exist in the tree");
                        	else
                        	{
								root = deleteFromBinTree(root, x);
                            	r->height = btNodeHeight(r);
                            	printf("\n\nDate found and deleted\n");
                        	}
                    	}
                        break;
                        
                case 5 :        
                        return;
                        
                default :        
                        printf("\nWrong option, try again ...\n");
                        break;
          }
     }
}


// Binary tree by Volume menu: Read file by Volume and display user menu
void binaryTreeByVolumeMenu()
{
    cmpPtr = &cmpVolumeDate; // The key of each data record is the pair (Volume, Date)
	readFileToBinTree();
	
    int selection, v;

    while (1)
	{
        printf("\n\n1. Find date(s) with MIN volume");
        printf("\n2. Find date(s) with MAX volume");
        
		printf("\n\nEnter your choice (1 - 2): ");
        scanf("%d",&selection);

        switch (selection)
        {
            case 1 :
                    /* // This code is used for verification: Display tree nodes info in inorder and the tree structure  
					printf("\n\nDate         Volume\tHeight\tBalance\n");
                    printf("------------------------------");
                    inorderBinTree(root);
                    
					printf("\n\n");system("pause");
					printf("\n\n\nTree structure:\n");
                    printf("---------------\n\n"); 
                    printBinTree(root, 1); */
                        
                    if (!root)
                        printf("\n\nTree is empty\n\n");
                    else
                    {
                        v = minValuebtNode(root)->data.Volume;
						printf("\nDates with MIN volume: ");
                        reportBinTree(root, v);
                        printf("\nMIN volume: %d\n\n", v);
                    }
                    return;

            case 2 :        
                    if (!root)
                        printf("\n\nTree is empty\n\n");
                    else
                    {
                        v = maxValuebtNode(root)->data.Volume;
						printf("\nDates with MAX volume: ");
                        reportBinTree(root, v);
                        printf("\nMAX volume: %d\n\n", v);
                    }
                    return;

            default :        
                    printf("\nWrong option, try again ...\n");
                    break;
        }
    }
}


// Open the input data file
void openFile(int argc, char *argv[])
{
    char fileName[20]; // Input data file name
	
	if (argc >= 2) // Data filename passed as command line argument
	    strcpy(fileName, argv[1]);
	else
	{
		printf("Give the stock data filename: "); // Data filename asked by user
        scanf("%s", fileName);  
        printf("\n\n");
    } 

	fp = fopen(fileName, "r");
	if (!fp)
	{
		printf( "ERROR: Can't open file\n");
		exit (1);
	}
}


// Read the file and store data records to a binary tree implemented as an AVL tree 
void readFileToBinTree()
{
	char line[80];  
	dataItem dt;
    float a, b, c, d;
    int e;

	fgets(line, 80, fp); // Get the first line

	while (fgets(line, 80, fp))
    {
        sscanf(line, "%10s,%f,%f,%f,%f,%d,%d", dt.Date, &a, &b, &c, &d, &dt.Volume, &e);  // a, b, c, d and e are dummy variables
        root = insertToBinTree(root, dt);
        root->height = btNodeHeight(root); // Update height of root node
    }

    fclose(fp);
}


// Allocate memory to a new tree node n and set left and right pointers to NULL and height to 0
btNode *createbtNode(dataItem x) 
{
    btNode *n = (btNode *) malloc(sizeof(btNode));
    if (!n)
    {
        printf("\nERROR: Memory failure\n\n");
        exit(1);
    }
    n->data = x;
	n->left = n->right = NULL;    
    n->height = 0;

    return n;
}


// Inorder traversal of the tree rooted at r
void inorderBinTree(btNode *r)
{
    if (r)
    {
      	inorderBinTree(r->left);
        printf("\n%s   %d\t%d\t%d", r->data.Date, r->data.Volume, r->height, btNodeBalance(r));
        inorderBinTree(r->right);
    }
}


// Search for a given date x in the tree rooted at r
btNode *searchBinTree(btNode *r, char x[11]) 
{
    if (!r)
        return NULL; // NULL tree
    else if (strcmp(x, r->data.Date) < 0) // If x is smaller than r's Date, then it lies in r's left subtree
			 return (searchBinTree(r->left, x));
         else if (strcmp(x, r->data.Date) > 0) // If x is greater than r's Date, then it lies in r's right subtree
           		  return (searchBinTree(r->right, x));
              else // If x is the same as r's Date, then search successful
           		  return r;
}


// Rotate right subtree rooted at T1
btNode *rotateR(btNode *T1)
{
    btNode *T2 = T1->left;
    btNode *T3 = T2->right;
 
    // Perform rotation
    T2->right = T1;
    T1->left = T3;
 
    // Update heights
    T1->height = btNodeHeight(T1);
    T2->height = btNodeHeight(T2);

    // Return new root
    return T2;
}
 

// Rotate left subtree rooted at T1
btNode *rotateL(btNode *T1)
{
    btNode *T2 = T1->right;
    btNode *T3 = T2->left;
 
    // Perform rotation
    T2->left = T1;
    T1->right = T3;
 
    //  Update heights
    T1->height = btNodeHeight(T1);
    T2->height = btNodeHeight(T2);
    
	// Return new root
    return T2;
}


// Get balance of node r
int btNodeBalance(btNode *r)
{
    if (!r)
        return 0;
    return btNodeHeight(r->left) - btNodeHeight(r->right);
}
 

// Recursive function to insert a data record x in the subtree rooted at r. It returns the new root of the subtree 
btNode *insertToBinTree(btNode *r, dataItem x)
{
    // 1. Perform the standard insertion to binary tree
    if (!r) 
        return createbtNode(x);
 
    if ((*cmpPtr)(x, r->data) < 0) // The key of each data record is the Date field (Exercise II.A) or the pair (Volume, Date) (Exercise II.B) 
        r->left = insertToBinTree(r->left, x); // Insert x to r's left subtree
    else if (strcmp(x.Date, r->data.Date) > 0) 
        	  r->right = insertToBinTree(r->right, x);  // Insert x to r's right subtree
         else
         	  return r; // Duplicates are not allowed in an AVL tree. Actually, as the Date value of each data record is unique, we have no duplicates 
    
    // 2. Update height of ancestor node r
    r->height = btNodeHeight(r);
 
    // 3. Get balance of ancestor node r to check whether the node became unbalanced  
    int balance = btNodeBalance(r);
 
    // If the node becomes unbalanced, then there are 4 Cases
 
    // Left Left Case - Right rotation
    if (balance > 1 && (*cmpPtr)(x, r->left->data) < 0)
        return rotateR(r);
 
    // Right Left Case - Double rotation: Rotate right and then Rotate left
    if (balance < -1 && (*cmpPtr)(x, r->right->data) < 0)    
	{
        r->right = rotateR(r->right);
        return rotateL(r);
    }
	
	// Left Right Case - Double rotation: Rotate left and then Rotate right
    if (balance > 1 && (*cmpPtr)(x, r->left->data) > 0) 
    {
        r->left = rotateL(r->left);
        return rotateR(r);
    }
	
	// Right Right Case - Left rotation
    if (balance < -1 && (*cmpPtr)(x, r->right->data) > 0) 
        return rotateL(r);
 
    // Return the (unchanged) node pointer if node stays balanced  
    return r;
}


// Given a non-empty binary tree rooted at r, return the node with minimum value stored in the tree
btNode *minValuebtNode(btNode *r)
{
    btNode *p = r;
  
    // Loop down to find the leftmost node  
    while (p->left)
        p = p->left;
  
    return p;
}
  

/* // Given a non-empty binary tree rooted at r, return the node with minimum value stored in the tree
btNode *minValuebtNode(btNode *r)
{
    if (r->left)
   		return minValuebtNode(r->left);
    else 
	    return r;
} */


// Given a non-empty binary tree rooted at r, return the node with maximun value stored in the tree
btNode *maxValuebtNode(btNode *r)
{
    btNode *p = r;
  
    // Loop down to find the rightmost node 
    while (p->right)
        p = p->right;
  
    return p;
}
  

/* // Given a non-empty binary tree rooted at r, return the node with maximum value stored in the tree
btNode *maxValuebtNode(btNode *r)
{
    if (r->right)
   		return maxValuebtNode(r->right);
    else 
	    return r;
} */


// Recursive function to delete a node with given date from subtree rooted at r. It returns the root of the modified subtree 
btNode *deleteFromBinTree(btNode *r, char x[])
{
    // 1. Perform the standard deletion from binary tree
  
    if (!r)
        return r; // NULL tree
  
    // If x is smaller than r's Date, then it lies in r's left subtree
    if (strcmp(x, r->data.Date) < 0)
        r->left = deleteFromBinTree(r->left, x);
  
       	// If x is greater than r's Date, then it lies in r's right subtree
       	else if (strcmp(x, r->data.Date) > 0)
        	    r->right = deleteFromBinTree(r->right, x);
  
            // If x is the same as r's Date, then this is the node to be deleted
    	    else
    		{
        	    // Node with only one son or a leaf
        	    if(!r->left || !r->right)
        	    {
                    btNode *t = r->left ? r->left : r->right;
  
                    // Leaf case
            	    if (!t)
            	    {
                        t = r;
                        r = NULL;
                    }
                    else // One son case
                        *r = *t; // Copy the contents of the non-empty son
            	    free(t);
            	}
                else
        	   	{
                    // Node with two sons: Get the inorder successor (node with minimum value in the right subtree)
            	    btNode *t = minValuebtNode(r->right);
  
            	    // Copy the inorder successor's data to this node
            	    r->data = t->data;
  
            	    // Delete the inorder successor
            	    r->right = deleteFromBinTree(r->right, t->data.Date);
                }
            }
  
    // If the tree had only one node, return NULL
    if (!r)
    	return r;
  
    // 2. Update height of current node r  
    r->height = btNodeHeight(r);
  
    // 3. Get balance of node r in order to check whether the node became unbalanced 
    int balance = btNodeBalance(r);
  
    // If the node becomes unbalanced, then there are 4 Cases
  
    // Left Left Case - Right rotation
    if (balance > 1 && btNodeBalance(r->left) >= 0)
        return rotateR(r);
  
    // Right Left Case - Double rotation: Rotate right and then Rotate left
    if (balance < -1 && btNodeBalance(r->right) > 0)
    {
        r->right = rotateR(r->right);
        return rotateL(r);
    }
	
	// Left Right Case - Double rotation: Rotate left and then Rotate right 
    if (balance > 1 && btNodeBalance(r->left) < 0)
    {
        r->left =  rotateL(r->left);
        return rotateR(r);
    }
  
    // Right Right Case - Left rotation
    if (balance < -1 && btNodeBalance(r->right) <= 0)
        return rotateL(r);

    // Return the (unchanged) node pointer if node stays balanced  
	return r;
}
  

// Print tree structure 
void printBinTree(btNode *r, int k)
{
	if (r)
	{
		printBinTree(r->right, k+1);
		for (int i = 0; i < k; i++)
            printf("    ");
		printf("%d (%d)\n", r->data.Volume, btNodeBalance(r));
		printBinTree(r->left, k+1);
	}
}


// Utility function
int max(int a, int b)
{
    return a >= b ? a : b;
}


// Compute the height of the tree rooted at r from height info stored in the roots of left and right subtrees
int btNodeHeight(btNode *r) 
{
    if (!r)
        return -1;
	if (r->left == r->right) // A leaf
	    return 0;
	if (r->left && !r->right) // r has only left son
        return 1 + r->left->height; 
    if (!r->left && r->right) // r has only right son
        return 1 + r->right->height;
    return 1 + max(r->left->height, r->right->height); // r has two sons
} 


// Utility function to compare Date fields
int cmpDate(dataItem a, dataItem b)
{
	return strcmp(a.Date, b.Date); 
} 


// Utility function to compare pairs (Volume, Date)
int cmpVolumeDate(dataItem a, dataItem b)
{
	if (a.Volume > b.Volume)
		return 1;
	if (a.Volume < b.Volume)	
	    return -1;
  	if (strcmp(a.Date, b.Date) > 0)
	    return 1;
	if (strcmp(a.Date, b.Date) < 0)
		return -1;
	return 0;	 
} 


// Report the date(s) with Volume value == x
void reportBinTree(btNode *r, int x) 
{
     if (r)
	     if (r->data.Volume == x) // x can be stored in many tree nodes
         {
             printf("%s ", r->data.Date);
             reportBinTree(r->left, x);
             reportBinTree(r->right, x);
         }
         else if (x < r->data.Volume)
                  reportBinTree(r->left, x);
              else
                  reportBinTree(r->right, x);
}



/*******************************************************************/



// HASHING WITH CHAINED LISTS FUNCTIONS 

// Hashing menu: Read file to a hash table and display user menu
void hashingMenu()
{
    readFileToHashTable();

    int selection;
    char x[11];
    lNode *n;
	dataItem d;


    while (1)
    {
        printf("\n\n1. Search volume for a given date");
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
                        printf("\nThis date does not exist in Hash table\n");
                    else
                        printf("\nVolume for the given date is: %d\n", n->data.Volume);
                    break;

            case 2 :
                    printf("\n\nGive the date (yyyy-mm-dd): ");
                    scanf("%s", x);

                    n = searchHashTable(x);
                    if (!n)
                        printf("\nThis date does not exist in Hash table\n");
                    else
                    {
						printf("\nCurrent record: %s | %d", n->data.Date, n->data.Volume);						
						printf("\n\nGive the NEW volume (>= 0): ");
                        scanf("%d", &n->data.Volume);
                        printf("\nVolume modified\n");                           
                    }
                    break;

            case 3 :
                    printf("\n\nGive the date (yyyy-mm-dd): ");
                    scanf("%s", x);

                    n = searchHashTable(x);
                    if (!n)
                        printf("\nThis date does not exist in Hash table\n");
                    else
                    {
                        deleteFromHashTable(x);
                        printf("\nDate found and deleted\n");
                    }
                    break;
                        
            case 4 :
                    displayHashTable();
					return;
                        
            default :
                    printf("\nWrong option, try again ...\n");
                    break;
        }
    } 
}


// Read the file and store data records to a Hash table with chained linked lists
void readFileToHashTable()
{
	char line[80];  
    lNode *n;
	dataItem dt;
    float a, b, c, d;
    int e;

	fgets(line, 80, fp); // Get the first line

	while (fgets(line, 80, fp))
    {
        sscanf(line, "%10s,%f,%f,%f,%f,%d,%d", dt.Date, &a, &b, &c, &d, &dt.Volume, &e); // a, b, c, d and e are dummy variables
		insertToHashTable(dt);
    }

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

	p = n; // n points to current list node and p to previous list node
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
		else // Ìïve to next list node
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

