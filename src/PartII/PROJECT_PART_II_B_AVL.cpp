/******************************************************************
* PART II.B: Binary Search Tree by Volume implemented as AVL tree *
******************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct dateVolume // Data record stored in binary tree node
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


// Declaration of functions

void binaryTreeByVolumeMenu(int argc, char *argv[]);
void readFileToBinTree(int argc, char *argv[]);

btNode *createbtNode(dataItem x);
int btNodeHeight(btNode *r);
int btNodeBalance(btNode *r);
btNode *rotateL(btNode *r);
btNode *rotateR(btNode *r);

btNode *insertToBinTree(btNode *r, dataItem x);
void reportBinTree(btNode *r, int x);
btNode *minValuebtNode(btNode *r);
btNode *maxValuebtNode(btNode *r);

int max(int a, int b);
void inorderBinTree(btNode *r);
void printBinTree(btNode *r, int k);
int cmpVolumeDate(dataItem a, dataItem b);



int main(int argc, char *argv[])
{
    binaryTreeByVolumeMenu(argc, argv);
	return 0;  
}


// Binary tree by Volume menu: Read file by Volume and display user menu
void binaryTreeByVolumeMenu(int argc, char *argv[])
{
    readFileToBinTree(argc, argv);

    int selection, v;

    while (1)
	{
        printf("1. Find date(s) with MIN volume");
        printf("\n2. Find date(s) with MAX volume\n");
          
		printf("\nEnter your choice (1 - 2): ");
        scanf("%d",&selection);

        switch (selection)
        {
            case 1 :
                    /* // This code is used for verification: Display tree nodes info in inorder and the tree structure  
					printf("\n\nDate         Volume\tHeight\tBalance\n");
                    printf("---------------------------------------");
                    inorderBinTree(root);
                        
                    printf("\n\n");system("pause");
					printf("\n\n\nTree structure:\n");
                    printf("---------------\n\n"); 
                    printBinTree(root, 1);
                    printf("\n\n"); */
                        
                    if (!root)
                        printf("\n\nTree is empty\n\n");
                    else
                    {
                        v = minValuebtNode(root)->data.Volume;
						printf("\nDates with MIN volume: ");
                        reportBinTree(root, v);
                        printf("\nMIN volume: %d\n", v);
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
                        printf("\nMAX volume: %d\n", v);
                    }
                    return;

            default :        
                    printf("\nWrong option, try again ...\n\n\n");
                    break;
        }
    }
}


// Open the file, read data records and store them to a binary tree implemented as an AVL tree 
void readFileToBinTree(int argc, char *argv[])
{
    FILE *fp;
    char fileName[20], line[80];  
    dataItem dt;
    float a, b, c, d;
    int e;

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
		printf( "ERROR: Can't open file\n\n");
		exit (1);
	}
	
	fgets(line, 80, fp); // Get the first line

	while (fgets(line, 80, fp))
    {
        sscanf(line, "%10s,%f,%f,%f,%f,%d,%d", dt.Date, &a, &b, &c, &d, &dt.Volume, &e);
        root = insertToBinTree(root, dt);
        root->height = btNodeHeight(root);  // Update height of root node
    }

    fclose(fp);
}


// Allocate memory to a new tree node n and set  left and right pointers to NULL and height to 0
btNode *createbtNode(dataItem x) 
{
     btNode *n = (btNode *) malloc(sizeof(btNode));
     if (!n)
     {
          printf("\nERROR: Memory failure\n\n");
          exit (1);
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
 
    // Update heights
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
 
    if (cmpVolumeDate(x, r->data) < 0) // The key of each data record is the pair (Volume, Date)   
        r->left = insertToBinTree(r->left, x); // Insert x to r's left subtree
    else if (strcmp(x.Date, r->data.Date) > 0)
        	  r->right = insertToBinTree(r->right, x); // Insert x to r's right subtree
         else
         	  return r; // Duplicates are not allowed in an AVL tree. Actually, as the Date value of each data record is unique, we have no duplicates  
				 
    // 2. Update height of ancestor node r
    r->height = btNodeHeight(r);
 
    // 3. Get balance of ancestor node r to check whether the node became unbalanced  
    int balance = btNodeBalance(r);
 
    // If the node becomes unbalanced, then there are 4 Cases
 
    // Left Left Case - Right rotation
    if (balance > 1 && cmpVolumeDate(x, r->left->data) < 0)
        return rotateR(r);
 
    // Right Left Case - Double rotation: Rotate right and then rotate left
    if (balance < -1 && cmpVolumeDate(x, r->right->data) < 0)    
	{
        r->right = rotateR(r->right);
        return rotateL(r);
    }
 
    // Left Right Case - Double rotation: Rotate left and then rotate right
    if (balance > 1 && cmpVolumeDate(x, r->left->data) > 0) 
    {
        r->left = rotateL(r->left);
        return rotateR(r);
    }
  
    // Right Right Case - Left rotation
    if (balance < -1 && cmpVolumeDate(x, r->right->data) > 0) 
        return rotateL(r);
		
	// Return the (unchanged) node pointer if node stays balanced  
    return r;
}


// Report the date(s) with Volume value == x  
void reportBinTree(btNode *r, int x) 
{
    if (r)
    {
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


// Compare pairs (Date, Volume)
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
