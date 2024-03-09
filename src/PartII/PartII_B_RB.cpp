/************************************************************************
* PART II.B: Binary Search Tree by Volume implemented as RED-BLACK tree *
************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


struct dateVolume // Data record stored in a binary tree node
{  
    char Date[11];
    int Volume;
};
typedef struct dateVolume dataItem;

struct binaryTreeNode // Binary Search Tree node implemented as RED-BLACK tree node
{  
    dataItem data; 
    struct binaryTreeNode *left;
    struct binaryTreeNode *right;
    struct binaryTreeNode *parent;
    char color;
};
typedef struct binaryTreeNode btNode;

btNode *root = NULL; // Root of the tree initially empty


// Declaration of functions

void binaryTreeByVolumeMenu(int argc, char *argv[]);
void readFileToBinTree(int argc, char *argv[]);

btNode *createbtNode(dataItem x);
btNode *uncle(btNode *r);
int leftSon(btNode *r);

void rotateL(btNode *r);
void rotateR(btNode *r);

void swapColors(btNode *n1, btNode *n2) ;
void swapDataValues(btNode *n1, btNode *n2);

void fixRedRed(btNode *r); 

btNode *searchBinTree(btNode *r, dataItem x);
void insertToBinTree(dataItem x);
void reportBinTree(btNode *r, int x);
btNode *minValuebtNode(btNode *r);
btNode *maxValuebtNode(btNode *r);

void inorderBinTree(btNode *r);
void printbtNodeInfo(btNode *r);
void printBinTree(btNode *r, int k);
void btPathNodes(btNode *r, int *blackNodes, int *totalNodes);

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
					printf("\n\nDate         Volume\tColor\tBNodes\tTNodes\n");
                    printf("----------------------------------------------");
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
	char *fileName;
    char line[80];  
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

	int i = 0;
	while (fgets(line, 80, fp))
    {
        sscanf(line, "%10s,%f,%f,%f,%f,%d,%d", dt.Date, &a, &b, &c, &d, &dt.Volume, &e);  // a, b, c, d and e are dummy variables
        insertToBinTree(dt);
    }

    free(fileName);
    fclose(fp);
}


// Allocate memory to a new tree node n, set left, right and parent pointers to NULL and color it RED
btNode *createbtNode(dataItem x) 
{
    btNode *n = (btNode *) malloc(sizeof(btNode));
    if (!n)
    {
        printf("\nERROR: Memory failure\n\n");
        exit(1);
    }
    n->data = x;
	n->left = n->right = n->parent = NULL;
     
	// Node is created during insertion and its color is RED
    n->color = 'R';

    return n;
}


// Return pointer to uncle of node r
btNode *uncle(btNode *r) 
{
    // If there is no parent or grandParent, then return NULL
    if (!r->parent)
        return NULL;
    if (!r->parent->parent)
        return NULL;
 
    if (leftSon(r->parent))
    	// Uncle on the right
        return r->parent->parent->right;
    else
        // Uncle on the left
        return r->parent->parent->left;
}
 

// Check if node r is left son of its parent
int leftSon(btNode *r) 
{ 
	return r == r->parent->left;    
}
 

// Rotate right the subtree rooted at T1
void rotateR(btNode *T1)
{
    btNode *T2 = T1->left;
    btNode *T3 = T2->right;
 
    // Perform rotation
    T2->right = T1;
    T1->left = T3;
 
    // Update root and parent nodes
    if (T1 == root)
	{
        root = T2;
        root->parent = NULL;
    }
 	else
 	{
 		T2->parent = T1->parent;
 		if (leftSon(T1))
 		    T2->parent->left = T2;
 		else 
 		    T2->parent->right = T2;
    }
	T1->parent = T2;
 	if (T3)
	    T3->parent = T1;
}
 

// Rotate left the subtree rooted at T1
void rotateL(btNode *T1)
{
    btNode *T2 = T1->right;
    btNode *T3 = T2->left;
 
    // Perform rotation
    T2->left = T1; 
    T1->right = T3;
 
    // Update root and parent nodes
	if (T1 == root)
	{
        root = T2;
        root->parent = NULL;
    }
 	else
 	{
 		T2->parent = T1->parent;
 		if (leftSon(T1))
 		    T2->parent->left = T2;
 		else 
 		    T2->parent->right = T2;
    }
 	T1->parent = T2;
 	if (T3)
	    T3->parent = T1;
} 


// Utility function
void swapColors(btNode *n1, btNode *n2) 
{
    char t;
    
	t = n1->color;
    n1->color = n2->color;
    n2->color = t;
}


// Search for a given date x in a non-empty tree rooted at r
// If found return the node else return the last node while traversing (used in insert)
btNode *searchBinTree(btNode *r, dataItem x) 
{
    if (cmpVolumeDate(r->data, x) == 0) 
		return r;
	
	if (cmpVolumeDate(r->data, x) > 0) 
	{
        if (!r->left)
            return r;
        else 
			return searchBinTree(r->left, x);
    } 
	
	if (cmpVolumeDate(r->data, x) < 0) 
    {
        if (!r->right)
            return r; 
        else
        	return searchBinTree(r->right, x);
    }
    return nullptr;
}


// Insert a data record x in the tree 
void insertToBinTree(dataItem x) 
{    
    btNode *newNode = createbtNode(x);
	
	if (!root) 
	{  
		// New root becomes BLACK
		newNode->color = 'B';
        root = newNode;
    } 
	else 
	{
        btNode *t = searchBinTree(root, x);
 
        if (cmpVolumeDate(t->data, x) == 0)  
        	return; // Duplicates are not allowed in a RED-BLACK tree. Actually, as the Date value of each data record is unique, we have no duplicates 
 
      	// Connect new node to the right node
        newNode->parent = t;
 
        if (cmpVolumeDate(t->data, x) > 0)
        	t->left = newNode;
      	else
        	t->right = newNode;
 
      	// Fix RED RED violation if exists
      	fixRedRed(newNode);
    }
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


// Fix RED RED violation at given node r
void fixRedRed(btNode *r) 
{
	// 1. If r is the root, color it BLACK and return
    if (r == root) 
	{
        r->color = 'B';
        return;
    }
 
    // Initialize parent, grandParent, uncle of r
    btNode *parent = r->parent, *grandParent = parent->parent, *un = uncle(r);
 
	if (parent->color == 'R') 
	{
       	if (un && un->color == 'R')
	    {
            // 2. Uncle RED, perform color flip and recurse for grandParent of r
            parent->color = 'B';
            un->color = 'B';
            grandParent->color = 'R';
            fixRedRed(grandParent);
        }     
		else 
		{          
            // 3. Uncle BLACK (color of NULL is considered as BLACK). There are 4 Cases for nodes r, parent and grandParent of r
            if (leftSon(parent)) 
		    {
				if (leftSon(r)) 
                {	// Left Left Case - Swap colors and Rotate right  
            		swapColors(parent, grandParent);
					rotateR(grandParent);
				}	
          		else 
		    	{
               		// Left Right Case - Swap colors and Double rotate (Rotate left and then Rotate right) 
					swapColors(r, grandParent);
					rotateL(parent);
               		rotateR(grandParent);
            	}        		
        	} 
			else 
			{
				if (leftSon(r)) 
				{
              		// Right Left Case - Swap colors and Double rotate (Rotate right and then Rotate left)
               		swapColors(r, grandParent);
					rotateR(parent);
					rotateL(grandParent);
            	} 
				else  
               	{	// Right Right Case - Swap colors and Rotate left
					swapColors(parent, grandParent);
					rotateL(grandParent);
			    }	
			}        	
        }
    }
}


// Inorder traversal of the tree rooted at r
void inorderBinTree(btNode *r)
{
    if (r)
    {
      	inorderBinTree(r->left);
        printbtNodeInfo(r);  
        inorderBinTree(r->right);
    }
}


// Print tree node info
void printbtNodeInfo(btNode *r)
{
	// If node r has one son or is a leaf, print also the number of BLACK nodes and the total number of nodes on the path from r to tree root
	if ((r->left == r->right) || (r->left && !r->right) || (!r->left && r->right))  
	{
		int blackNodes, totalNodes;
		btPathNodes(r, &blackNodes, &totalNodes);
		printf("\n%s   %d\t%c\t%d\t%d", r->data.Date, r->data.Volume, r->color, blackNodes, totalNodes);
	}
	else
	    printf("\n%s   %d\t%c\t-\t-", r->data.Date, r->data.Volume, r->color);
}


// Compute the number of BLACK nodes / total nodes on the path from node r to tree root
void btPathNodes(btNode *r, int *blackNodes, int *totalNodes)
{
	btNode *p = r;
	*blackNodes = *totalNodes = 0;
	
	while (p)
	{
		(*totalNodes)++;
		if (p->color == 'B')
		    (*blackNodes)++;
		p = p->parent;
	}
}


// Print tree structure 
void printBinTree(btNode *r, int k)
{
	if (r)
	{
		printBinTree(r->right, k+1);
		for (int i = 0; i < k; i++)
            printf("      ");
		printf("%d (%c)\n", r->data.Volume, r->color);
		printBinTree(r->left, k+1);
	}
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

