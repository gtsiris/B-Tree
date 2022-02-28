#include "BTreeTypes.h"

/* This code is from the book "Data Structures and Program Design in C" by
R. Kruse, C.L. Tondo and B. Leung
Prentice Hall, 1997. */

/* This is the file BTreeInterface.h */



void CreateTree(Treenode **);
void PreOrder(Treenode *);
Treenode *SearchTree(Key, Treenode *, int *);
Treenode *InsertTree(Treeentry, Treenode *);
Treenode *DeleteTree(Key, Treenode *);
void Successor(Treenode *, int);
void Removekth(Treenode *, unsigned int);
Key Inorder(Treenode *, int *, unsigned int);
