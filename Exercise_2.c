#include <stdio.h>
#include <stdlib.h>
#include "BTreeInterface.h"

/* This main program is for testing the code
from the book "Data Structures and Program Design in C" by
R. Kruse, C.L. Tondo and B. Leung
Prentice Hall, 1997. */



int main(void)
{
   Treenode *r, *s;
   Treeentry e;
   int i, v;
   int pos;
   unsigned int k;
 
   CreateTree(&r);

   /* Populate the B-tree with the integer keys given as input */
   while (scanf("%d", &v) != EOF){
     e.key=v;
     e.value=100;        /* This is a default value; values are not used anywhere in the program */
     r=InsertTree(e, r);
   }
   printf("\n");
   
   /* Test whether the B-tree was constructed correctly by printing its nodes in preorder */
   printf("Nodes of tree in preorder:\n");
   Preorder(r);
   printf("\n");
   
   
   /* Remove a given key; if it is necessary replace it by its successor */
   printf("Choose an existing key to be removed: ");
   scanf("%d", &v);
   printf("\n");
   s = SearchTree(v, r, &pos);
   if (s == NULL)
     printf("Given key does not exist in this tree.\n\n");
   else {
     DeleteTree(v, r);        /* This fuction uses RecDeleteTree, which uses Successor operation */
   }
   
   /* Test whether the entry was removed correctly by printing the nodes in preorder */
   printf("Nodes of tree in preorder after the removal of given key:\n");
   Preorder(r);
   printf("\n");
   
   
   /* Remove the entry that has the kth smallest key */
   printf("Choose the number k between 0 and (number of keys - 1) and the kth smallest key will be removed: ");
   scanf("%d", &k);                     /* Index k must be between 0 and (number of keys - 1) */
   printf("\n");
   Removekth(r, k);
   
   /* Test whether the entry was removed correctly by printing the nodes in preorder */
   printf("Nodes of tree in preorder after the removal of kth smallest key:\n");
   Preorder(r);
   printf("\n");
   
   return 0;
}
