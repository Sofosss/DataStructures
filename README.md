<h1 align="center">
<br>
<img src="./media/datastrct_logo.png" alt="logo" style="width:200px;margin-bottom:4vh">
<br>
<strong>Data Structures Project 2021</strong>
</h1>
<br>

# Table of Contents
* [About the project](#about-the-project)
* [PART I: Sorting and Searching Algorithms](#part-i-sorting-and-searching-algorithms)
* [PART II: Binary Search Trees and Hashing](#part-ii-binary-search-trees-and-hashing)
* [Contact](#contact)


# About the Project
The project was implemented as part of the Data Structures course during my second year of studies at the Department of Computer Engineering and Informatics (CEID) at the University of Patras. 

In the first part of the project, algorithms for sorting and searching are implemented on a dataset (provided in this repository) containing historical data with daily values for three stocks traded on the NYSE and NASDAQ stock exchanges from 2005 to 2017. Specifically, the implemented sorting algorithms include **MergeSort**, **QuickSort**, **HeapSort** and **CountingSort**. The searching algorithms encompass **Binary Search**, **Interpolation Search** and **Binary Interpolation Search (BIS)**.

In the second part, binary search trees are developed, specifically **AVL** and **Red-Black (RB)** trees. Each node of these trees maintains the Date and Volume fields of a stock record from the corresponding input file. The binary search trees are ordered based on a specific field (initially Date, with a later implementation for the Volume field), and they are implemented with dynamic memory management. In case of modification or deletion of a record (e.g., a tree node), the tree is appropriately readjusted, adhering strictly to the specifications of its structure. Additionally, a **Hash Chain** structure is provided as an alternative implementation.


# PART I: Sorting and Searching Algorithms
1. ***MergeSort and QuickSort***: In [PROJECT_PART_I_1][project-part-I-1-link], the MergeSort and QuickSort algorithms are implemented with the purpose of sorting dates (``Date`` field) in ascending order based on the opening values (``Open`` field). For QuickSort, three different implementations (variations) are provided. The first implementation corresponds to the **Standard** QuickSort, the second to the **Randomized** QuickSort and the third to the **Median of Three** logic.

2. ***HeapSort and CountingSort***: In [PROJECT_PART_I_2][project-part-I-2-link], the **HeapSort** and **CountingSort** algorithms are implemented with the objective of sorting dates (``Date`` field) in ascending order based on the opening values (``Open`` field). For CountingSort, two different implementations (variations) are provided. The first implementation corresponds to the **Standard** CountingSort, and the second to the **Extended** CountingSort.
The Standard CountingSort operates on the integer values of the Close field (Close: a field of a stock record). However, it does not produce a correctly sorted array (e.g., data records with Close values of 35,510, 35,243, 35,210 appear in the sorted array in that order). The Extended CountingSort operates on the values of 1000\*Close, considering them as integers since the Close values of data records have three decimal places. Thus, it produces a correctly sorted array at the expense of requiring a larger space for the C array, which is 1000 times larger than the C array of the standard CountingSort. [ More precisely, it holds that 1.000*[(int(max)–int(min)-1]+1 ≤ size of array C ≤ 1.000*[(int(max)–int(min)+1]-1, where max and min are the maximum and minimum decimal values and (int)max, (int)min are the maximum and minimum integer values (integer part of the decimal value) of the Close field, respectively.]

3. ***Binary Search and Interpolation Search***: 





# PART II: Binary Search Trees and Hashing

# Contact

### Authors:
- Sofotasios Argiris | <a href="mailto:a.sofotasios@ac.upatras.gr">a.sofotasios@ac.upatras.gr</a>


# License

Distributed under the [MIT] License. See `LICENSE.md` for more details.


<!-- MARKDOWN LINKS & IMAGES -->
[project-part-I-1-link]: ./src/PartI/PROJECT_PART_I_1.cpp
[project-part-I-2-link]: ./src/PartI/PROJECT_PART_I_2.cpp