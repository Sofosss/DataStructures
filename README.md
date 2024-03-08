<h1 align="center">
<br>
<img src="./media/datastrct_logo.png" alt="logo" style="width:180px;margin-bottom:4vh">
<br>
<br>
<strong>Data Structures Project 2021</strong>
</h1>
<br>

# Table of Contents
* [About the project](#about-the-project)
* [PART I: Sorting and Searching Algorithms](#part-i-sorting-and-searching-algorithms)
* [PART II: Binary Search Trees and Hashing](#part-ii-binary-search-trees-and-hashing)
* [Contact](#contact)
* [License](#license)


# About the Project
The project was implemented as part of the Data Structures course during my second year of studies at the Department of Computer Engineering and Informatics (CEID) at the University of Patras. 

In the first part of the project, algorithms for sorting and searching are implemented on a dataset (provided in this repository) containing historical data with daily values for three stocks traded on the NYSE and NASDAQ stock exchanges from 2005 to 2017. Specifically, the implemented sorting algorithms include **MergeSort**, **QuickSort**, **HeapSort** and **CountingSort**. The searching algorithms encompass **Binary Search**, **Interpolation Search** and **Binary Interpolation Search (BIS)**.

In the second part, binary search trees are developed, specifically **AVL** and **Red-Black (RB)** trees. Each node of these trees maintains the Date and Volume fields of a stock record from the corresponding input file. The binary search trees are ordered based on a specific field (initially Date, with a later implementation for the Volume field), and they are implemented with dynamic memory management. In case of modification or deletion of a record (e.g., a tree node), the tree is appropriately readjusted, adhering strictly to the specifications of its structure. Additionally, a **Hash Chain** structure is provided as an alternative implementation.


# PART I: Sorting and Searching Algorithms
1. ***MergeSort and QuickSort***: In [PROJECT_PART_I_1][project-part-I-1-link], the **MergeSort** and **QuickSort** algorithms are implemented with the purpose of sorting dates (``Date`` field) in ascending order based on the opening values (``Open`` field). For QuickSort, three different implementations (variations) are provided. The first implementation corresponds to the **Standard** QuickSort, the second to the **Randomized** QuickSort and the third to the **Median of Three** logic.

2. ***HeapSort and CountingSort***: In [PROJECT_PART_I_2][project-part-I-2-link], the **HeapSort** and **CountingSort** algorithms are implemented with the objective of sorting dates (``Date`` field) in ascending order based on the opening values (``Open`` field). For CountingSort, two different implementations (variations) are provided. The first implementation corresponds to the **Standard** CountingSort, and the second to the **Extended** CountingSort.
The Standard CountingSort operates on the integer values of the Close field (Close: a field of a stock record). However, it does not produce a correctly sorted array (e.g., data records with Close values of 35,510, 35,243, 35,210 appear in the sorted array in that order). The Extended CountingSort operates on the values of 1000\*Close, considering them as integers since the Close values of data records have three decimal places. Thus, it produces a correctly sorted array at the expense of requiring a larger space for the C array, which is 1000 times larger than the C array of the standard CountingSort. [ More precisely, it holds that 1.000*[(int(max)–int(min)-1]+1 ≤ size of array C ≤ 1.000*[(int(max)–int(min)+1]-1, where max and min are the maximum and minimum decimal values and (int)max, (int)min are the maximum and minimum integer values (integer part of the decimal value) of the Close field, respectively.]

3. ***Binary Search and Interpolation Search***: 
In [PROJECT_PART_I_3][project-part-I-3-link], the algorithms **Binary Search** and **Interpolation Search** are implemented with the purpose of finding the total transaction volume (``Volume`` field) for the stocks in the corresponding file, based on a specific date (``Date`` field).

4. ***Binary Interpolation Search and Binary Interpolation Search Improved***:
In [PROJECT_PART_I_4][project-part-I-4-link], the algorithms **Binary Interpolation Search** and **Binary Interpolation Search Improved** are implemented with the purpose of finding the total transaction volume (``Volume`` field) for the stocks in the corresponding file, based on a specific date (``Date`` field). In both variations, the average-case time complexity remains **O(lognlogn)**. The Binary Interpolation Search variation performs jumps of size √n until it locates the block of size √n that potentially contains the search element x. In the next step, the jumps have a size of √√n and so on. This approach improves the **worst-case time complexity from O(n) to O(√n)**. The Improved Binary Interpolation Search variation executes exponentially large jumps (of size √n, 2\*√n, 2^2\*√n, etc.) until it identifies the block that may contain x. Then, it calls the Binary Search algorithm for that block with elements at positions, for example, m+2^(i-1)\*√n to m+2^i\*√n in array S. This helps locate the block of size √n that potentially contains x. In one step, at most logn jumps are performed and the Binary Search algorithm requires at most log√n comparisons. Consequently, the total time complexity is **O(logn)** and in the next step it requires **O(log√n)** and so on, resulting in a **worst-case time complexity of O(logn)**.


# PART II: Binary Search Trees and Hashing
### ***Binary Search Tree implemented as AVL and RED-BLACK  based on the ``Date`` field***

In [PROJECT_PART_II_A_AVL][project-part-I-A-AVL-link] | [PROJECT_PART_II_A_RB][project-part-I-A-RB-link], the Binary Search Tree (BST) is implemented as an AVL | RED-BLACK node-oriented tree. Each node of the tree maintains the Date and Volume fields of a stock record from the corresponding input file. 
 
 We should support the following: 
 1. Displaying the Binary Search Tree (BST) with an in-order traversal. Each representation should include a header with the titles of the elements of the records being depicted.
 2. Searching for the volume trading price (``Volume``) based on the date (``Date``) provided by the user. 
 3. Modifying the volume trading price corresponding to a specific date, which will be provided by the user. 
 4. Deleting a record corresponding to a specific date, which will be provided by the user.

The structure of a node in AVL:
```c
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
```

The structure of a node in Red-Black(RB):
```c
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
```

The requested operations are implemented through the following functions:

```c
void inorderBinTree(btNode *r)
btNode *searchBinTree(btNode *r, char x[11])
deleteFromBinTree(btNode *r, char x[11])
```

(For ``deleteFromBinTree()``, in the case of an AVL tree, it returns a pointer to a node in the tree, while in the case of a RED-BLACK tree, it does not return any value).

The ``modifyBinTree()`` function has not been implemented separately.

For each different structure (AVL, RED-BLACK), a set of auxiliary functions is used (e.g., ``rotateL()``, ``rotateR()``, ``btNodeHeight()``, ``btNodeBalance()``, ``uncle()``, ``sibling()``, ``swapColors()``, ``fixRedRed()``, ``fixDoubleBlack()``, etc.) to implement the basic operations.

📌 The creation of the tree is done with the help of the corresponding function ``insertToBinaryTree(dataItem x)``, which is called by ``readFileToBinaryTree()`` every time a data record is read. In the case of the AVL tree, the function returns a pointer to a tree node, while in the case of the RED-BLACK tree, it does not return any value. The insertion of duplicate records (data records with the same value in the ``Date`` field) is not allowed. Practically, since all values in the ``Date`` field are unique, there are no duplicate records (the precaution against inserting duplicates is taken for completeness reasons).

📌 The rebalancing of the subtree with the root being the node T1 is performed using the functions btNode ``*rotateL(btNode *T1)`` and btNode ``*rotateR(btNode *T1)``, which implement the single left and right rotations. Double rotation involves either a single left and a single right rotation or a single right and a single left rotation.

⚠️ In the case of the **AVL** tree, the insertion of a new node is concluded with at most a single or double terminal rotation, while in the deletion of a node, rotations may propagate up to the root of the tree.

⚠️ In the case of the **RED-BLACK** tree, for each insertion or deletion operation of a node, at most one single and one double terminal rotation are executed.

❗When a violation of the properties of the **RED-BLACK** tree occurs in the node 'r', the functions ``void fixRedRed(btNode *r)`` and ``void fixDoubleBlack(btNode *r)`` are called to correct the problem.
- ``fixRedRed()`` is called during the insertion of a new node into the tree when two consecutive red nodes are created (red parent and red child).
- ``fixDoubleBlack()`` is called during the deletion of a black node that leads to a reduction by 1 in the number of black nodes on the paths from the root of the tree that, before the deletion, they(the paths) passed through the deleted node. 

Both functions are recursive, operate level-wise and perform color changes on the nodes, which may propagate up to the root of the tree, along with O(1) rotations.

For the representation of the nodes in each tree, the functions ``inorderBinTree()`` and ``printBinTree()`` are utilized:

In ``inorderBinTree()``, the fundamental information (``Date, Volume``) of each data record stored in the tree nodes is displayed, along with a set of auxiliary information primarily for testing purposes.
In the AVL tree implementation, additional information such as the height and balance factor of each node is displayed.
In the RED-BLACK tree implementation, for each node with one or zero children (leaf), the color of the node, the count of black nodes in the path from the node to the tree's root, as well as the total count of red and black nodes in the path are displayed.

```printBinTree()``` prints the structure of the tree from left to right with the ``Volume`` information of each node.
In the AVL tree implementation, the balance factor of each node is additionally printed.
In the RED-BLACK tree implementation, the color of each node is printed.


### ***Binary Search Tree implemented as AVL and RED-BLACK  based on the ``Volume`` field***
As in the previous case tree is constructed with the function **insertToBinTree()**. The insertion of duplicate records (data records with identical values in the ``Date`` and ``Volume`` fields) is not allowed. As mentioned earlier, since all values in the ``Date`` field are unique, there are no duplicate records (the precaution against inserting duplicates is taken for completeness reasons).

 We should support the following: 
1. Finding the day/days with the **MINIMUM trading volume**.
2. Finding the day/days with the **MAXIMUM trading volume**.


The requested operations are implemented through the following functions:

``btNode *minValuebtNode(btNode *r)``: Returns a pointer to the node in the tree with the smallest ``Volume`` value (leftmost node of the tree with only a right child or is a leaf) or NULL if the tree is empty.

``btNode *maxValuebtNode(btNode *r)``: Returns a pointer to the node in the tree with the largest ``Volume`` value (rightmost node of the tree with only a left child or is a leaf) or NULL if the tree is empty.

``void reportBinTree(btNode *r, int x)``: Prints the ``Date`` values of data records that have a ``Volume`` value equal to x. [ ``reportBinTree()`` is quite general: it can easily be modified to print the ``Date`` values of data records with ``Volume`` values between x1 and x2, where x1 < x2. The time complexity for a tree with n data records is O(logn + k), where k is the size of the answer (the number of data records with ``Date`` values within the given range). ]
# Contact

### Authors:
- Sofotasios Argiris | <a href="mailto:a.sofotasios@ac.upatras.gr">a.sofotasios@ac.upatras.gr</a>


# License

Distributed under the [MIT] License. See `LICENSE.md` for more details.


<!-- MARKDOWN LINKS & IMAGES -->
[project-part-I-1-link]: ./src/PartI/PROJECT_PART_I_1.cpp
[project-part-I-2-link]: ./src/PartI/PROJECT_PART_I_2.cpp
[project-part-I-3-link]: ./src/PartI/PROJECT_PART_I_3.cpp
[project-part-I-4-link]: ./src/PartI/PROJECT_PART_I_4.cpp
[project-part-I-A-AVL-link]: ./src/PartII/PROJECT_PART_II_Α_AVL.cpp
[project-part-I-A-RB-link]: ./src/PartII/PROJECT_PART_II_Α_RB.cpp