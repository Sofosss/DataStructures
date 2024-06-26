/****************************************************************
* PART II.A: Binary Search Tree by Date implemented as AVL tree *
****************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

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

void binaryTreeByDateMenu(int argc, char *argv[]);
void readFileToBinTree(int argc, char *argv[]);

btNode *createbtNode(dataItem x);
int btNodeHeight(btNode *r);
int btNodeBalance(btNode *r);
btNode *rotateL(btNode *r);
btNode *rotateR(btNode *r);

btNode *searchBinTree(btNode *r, char x[11]);
btNode *insertToBinTree(btNode *r, dataItem x);
btNode *deleteFromBinTree(btNode *r, char x[11]);
btNode *minValuebtNode(btNode *r);

int max(int a, int b);
void inorderBinTree(btNode *r);
void printBinTree(btNode *r, int k);



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
                    printf("\n\nDate         Volume\tHeight\tBalance\n");
                    printf("---------------------------------------");
                    inorderBinTree(root);

		    printf("\n\n");system("pause");
                    printf("\n\nTree structure:\n\n");
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
                    	if (!r)
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
                    	if (!r)
                             printf("\nThis date does not exist in the tree\n\n");
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
                    	if (!r)
                            printf("\nThis date does not exist in the tree\n\n");
                    	else
                    	{
			    root = deleteFromBinTree(root, x);
                            r->height = btNodeHeight(r);
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

	while (fgets(line, 80, fp))
    {
        sscanf(line, "%10s,%f,%f,%f,%f,%d,%d", dt.Date, &a, &b, &c, &d, &dt.Volume, &e);  // a, b, c, d and e are dummy variables
        root = insertToBinTree(root, dt);
        root->height = btNodeHeight(root); // Update height of root node
    }

    free(fileName);
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
 
    if (strcmp(x.Date, r->data.Date) < 0) // The key of each data record is the Date field
        r->left = insertToBinTree(r->left, x); // Insert x to r's left subtree
    else if (strcmp(x.Date, r->data.Date) > 0) 
             r->right = insertToBinTree(r->right, x); // Inser tx to r's right subtree
         else
             return r; // Duplicates are not allowed in an AVL tree. Actually, as the Date value of each data record is unique, we have no duplicates 
				 
    // 2. Update height of ancestor node r
    r->height = btNodeHeight(r);
 
    // 3. Get balance of ancestor node r to check whether the node became unbalanced  
    int balance = btNodeBalance(r);
 
    // If the node becomes unbalanced, then there are 4 Cases
 
    // Left Left Case - Right rotation
    if (balance > 1 && strcmp(x.Date, r->left->data.Date) < 0)
        return rotateR(r);
 
    // Right Left Case - Double rotation: Rotate right and then Rotate left
    if (balance < -1 && strcmp(x.Date, r->right->data.Date) < 0)    
    {
        r->right = rotateR(r->right);
        return rotateL(r);
    }
	
    // Left Right Case - Double rotation: Rotate left and then Rotate right
    if (balance > 1 && strcmp(x.Date, r->left->data.Date) > 0) 
    {
        r->left = rotateL(r->left);
        return rotateR(r);
    }
	
    // Right Right Case - Left rotation
    if (balance < -1 && strcmp(x.Date, r->right->data.Date) > 0) 
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
  
    // 2. Update height of the current node r  
    r->height = btNodeHeight(r);
  
    // 3. Get balance of node r in order to check whether the node became unbalanced 
    int balance = btNodeBalance(r);
  
    // If the node becomes unbalanced, then there are 4 Cases
  
    // Left Left Case - Right rotation
    if (balance > 1 && btNodeBalance(r->left) >= 0)
        return rotateR(r);
  
    // Left Right Case - Double rotation: Rotate left and then Rotate right 
    if (balance > 1 && btNodeBalance(r->left) < 0)
    {
        r->left =  rotateL(r->left);
        return rotateR(r);
    }
  
    // Right Left Case - Double rotation: Rotate right and then Rotate left
    if (balance < -1 && btNodeBalance(r->right) > 0)
    {
        r->right = rotateR(r->right);
        return rotateL(r);
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

