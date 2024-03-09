/**********************************************************************
* PART II.A: Binary Search Tree by Date implemented as RED-BLACK tree *
**********************************************************************/

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

void binaryTreeByDateMenu(int argc, char *argv[]);
void readFileToBinTree(int argc, char *argv[]);

btNode *createbtNode(dataItem x);
btNode *uncle(btNode *r);
int leftSon(btNode *r);
btNode *sibling(btNode *r);   
int hasRedSon(btNode *r); 
btNode *minValuebtNode(btNode *r);
btNode *replacebtNode(btNode *r); 

void rotateL(btNode *r);
void rotateR(btNode *r);

void swapColors(btNode *n1, btNode *n2) ;
void swapDataValues(btNode *n1, btNode *n2);

void fixRedRed(btNode *r); 
void fixDoubleBlack(btNode *r);

btNode *searchBinTree(btNode *r, char x[11]);
void insertToBinTree(dataItem x);
void deleteFromBinTree(btNode *r);

void inorderBinTree(btNode *r);
void printbtNodeInfo(btNode *r);
void printBinTree(btNode *r, int k);
void btPathNodes(btNode *r, int *blackNodes, int *totalNodes);



int main(int argc, char *argv[])
{
    binaryTreeByDateMenu(argc, argv);
    return 0;  
}


// Binary tree by Date menu: Read file by Date and display user menu
void binaryTreeByDateMenu(int argc, char *argv[])
{
    readFileToBinTree(argc, argv);

    int selection;
    char x[11];
    dataItem d;
    btNode *r;

    while (1)
    {
        printf("1. Inorder traversal of BST");
        printf("\n2. Search volume for a given date");
        printf("\n3. Modify volume for a given date");
        printf("\n4. Delete BST node of a given date");
        printf("\n5. Exit\n");

        printf("\nEnter your choice (1 - 5): ");
        scanf("%d",&selection);
          
        switch (selection)
        {
            case 1 :
                    printf("\n\nDate         Volume\tColor\tBNodes\tTNodes\n");
                    printf("----------------------------------------------");
                    inorderBinTree(root);

                    printf("\n\n");system("pause");
		    printf("\n\n\nTree structure:\n");
                    printf("---------------\n\n");
                    printBinTree(root, 1);
                    printf("\n\n");
                    break;

            case 2 :
                    printf("\n\nGive the date (yyyy-mm-dd): ");
                    scanf("%s", x);

                    if (!root)
                        printf("\nTree is empty\n\n");	
                    else
		    {
			r = searchBinTree(root, x);
			if (strcmp(x, r->data.Date) != 0) 
			    printf("\nThis date does not exist in the tree\n\n\n");	 
			else  
                            printf("\nVolume for the given date is: %d\n\n\n", r->data.Volume);
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
			if (strcmp(x, r->data.Date) != 0) 
			    printf("\nThis date does not exist in the tree\n\n\n");	 
			else
                        {       
			    printf("\nCurrent record: %s | %d", r->data.Date, r->data.Volume);
                            printf("\n\nGive the new volume (>= 0): ");
                            scanf("%d", &r->data.Volume);
                            printf("\nVolume modified\n\n\n");
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
			if (strcmp(x, r->data.Date) != 0) 
			    printf("\nThis date does not exist in the tree\n\n\n");	 
			else
			{
			    deleteFromBinTree(r);
                            printf("\n\nDate found and deleted\n\n\n");
                        }
                    } 	
                    break;
                        
            case 5 :        
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
    if (!r->parent || !r->parent->parent)
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
 

// Returns pointer to sibling of node r
btNode *sibling(btNode *r) 
{
    // If r has no parent then sibling = NULL 
    if (!r->parent)
        return NULL;
 
    if (leftSon(r))
        // Sibling on the right
	return r->parent->right;
    else
    	// Sibling on the left 
	return r->parent->left;
}


// Return 1 if node r has RED son or 0 otherwise
int hasRedSon(btNode *r) 
{
    if (r->left && r->left->color == 'R')
	return 1;  
	      
    if (r->right && r->right->color == 'R')
	return 1;  
	
    return 0;
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
 

// Utility function
void swapDataValues(btNode *n1, btNode *n2) 
{
    dataItem t;
    
    t = n1->data;
    n1->data = n2->data;
    n2->data = t;
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
   
  
// Locate tree node that replaces the deleted node r
btNode *replacebtNode(btNode *r) 
{
    // Node with two sons: Return the inorder successor (node with minimum value in the right subtree)
    if (r->left && r->right)  
        return minValuebtNode(r->right);
 
    // Leaf case
    if (!r->left && !r->right)
        return NULL;
 
    // One son case
    if (r->left)
        return r->left;
    else
        return r->right;
} 
  

// Search for a given date x in a non-empty tree rooted at r
// If found return the node (used in delete) else return the last node while traversing (used in insert)
btNode *searchBinTree(btNode *r, char  x[11]) 
{
    if (strcmp(r->data.Date, x) == 0) 
	return r;
	
    if (strcmp(r->data.Date, x) > 0) 
    {
        if (!r->left)
            return r;
       	else 
	    return searchBinTree(r->left, x);
    } 
	
    if (strcmp(r->data.Date, x) < 0) 
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
        btNode *t = searchBinTree(root, x.Date);
 
        if (strcmp(t->data.Date, x.Date) == 0)  
            return; // Duplicates are not allowed in a RED-BLACK tree. Actually, as the Date value of each data record is unique, we have no duplicates 
 
      	// Connect new node to the right node
        newNode->parent = t;
 
        if (strcmp(t->data.Date, x.Date) > 0)
            t->left = newNode;
      	else
            t->right = newNode;
 
      	// Fix RED RED violation if exists
      	fixRedRed(newNode);
    }
}


// Delete node r from the tree
void deleteFromBinTree(btNode *r) 
{
    btNode *p = replacebtNode(r); // p is the son that replaces the deleted node r
    btNode *parent = r->parent;
 
    if (!p) 
    {
        // p is NULL therefore r is a leaf
        if (r == root) // After deletion the tree becomes empty
            root = NULL;
        else 
	{
            if (r->color == 'B') 
		// p and r are both BLACK (color of NULL is considered as BLACK), fix DOUBLE BLACK at r
		fixDoubleBlack(r);
        	
		// Delete r from the tree
        	if (leftSon(r))  
          	    parent->left = NULL;
         	else  
          	    parent->right = NULL;
        }
        free(r);
        return;
    }
 
    if (!r->left || !r->right) // r has only one son, p is the son of r and it is a RED leaf
    {
	if (r == root)  
	{
            // r is the root, assign the data record of p to r and delete leaf p
            r->data = p->data;
            r->left = r->right = NULL;
            free(p);
        } 
	else 
	{
            // Detach r from the tree and move p up
            if (leftSon(r)) 
          	parent->left = p;
            else  
          	parent->right = p;
        	
	    free(r);
        	
	    p->parent = parent;
            // Color p BLACK
	    p->color = 'B';
        }
    	return;
    }
 
    // r has 2 sons, swap values with successor and recurse
    swapDataValues(p, r);
    deleteFromBinTree(p);
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
                {   
		    // Left Left Case - Swap colors and Rotate right  
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
               	{	
		    // Right Right Case - Swap colors and Rotate left
		    swapColors(parent, grandParent);
		    rotateL(grandParent);
		}	
	    }        	
        }
    }
}


// Fix DOUBLE BLACK violation at given node r
void fixDoubleBlack(btNode *r) 
{
    if (r == root)
       // Reached root
       return;
 
    btNode *sibl = sibling(r), *parent = r->parent;
    if (!sibl) 
      	// 1. No sibling, DOUBLE BLACK pushed up
      	fixDoubleBlack(parent);
    else
    {
        // 2. Sibling RED
	if (sibl->color == 'R') // There are 2 Cases when sibling of r is RED
	{
            // Swap colors and Rotate
            swapColors(sibl, parent);
            if (leftSon(sibl))  
             	// Left Case - Rotate right  
             	rotateR(parent);
            else  
             	// Right Case - Rotate left 
             	rotateL(parent);
            fixDoubleBlack(r);
       	} 
	else 
	{
	    // 3. Sibling BLACK with at least one RED son
            if (hasRedSon(sibl)) // There are 4 Cases when BLACK sibling has at least one RED son
	    {
		if (sibl->left && sibl->left->color == 'R') 
              	{
            	    if (leftSon(sibl)) 
		    {
              		// Left Left Case - Update colors and Rotate right
              		sibl->left->color = sibl->color;
              		sibl->color = parent->color;
              	 	rotateR(parent);
            	    } 
		    else 
		    {
              		// Right Left Case - Update color and Double rotate (Rotate right and then Rotate left)
              		sibl->left->color = parent->color;
              		rotateR(sibl);
              		rotateL(parent);
            	    }
              	} 
		else 
		{
                    if (leftSon(sibl)) 
		    {
              		// Left Right Case - Update color and Double rotate (Rotate left and then Rotate right)
              		sibl->right->color = parent->color;
              		rotateL(sibl);
              		rotateR(parent);
                     } 
		     else 
		     {
              		 // Right Right Case - Update colors and Rotate left
              		 sibl->right->color = sibl->color;
              		 sibl->color = parent->color;
                      	 rotateL(parent);
                     }
              	}
              	parent->color = 'B';
           } 
	   else 
	   { 
             	// 4. BLACK sibling has two BLACK sons: Perform color flip and recurse for parent of r if it is BLACK
             	sibl->color = 'R';
             	if (parent->color == 'B')
                    fixDoubleBlack(parent);
             	else
                    parent->color = 'B';
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



