#include <stdio.h>
#include <stdlib.h>
#include "BTreeInterface.h"

/* This code is from the book "Data Structures and Program Design in C" by
R. Kruse, C.L. Tondo and B. Leung
Prentice Hall, 1997. */

/* This is the file BTreeImplementation.c */



/* Successor: replaces an entry by its immediate successor.
   Pre: current points to a node in a B-tree with an entry in index pos.
   Post: This entry is replaced by its immediate successor under order of keys. */
void Successor(Treenode *current, int pos)
{
  if (current->branch[pos] != NULL) {  /* Successor is the first key in the following branch */
    /* Replace the entry by its succesor */
    current->entry[pos].key = current->branch[pos]->entry[1].key;
    current->entry[pos].value = current->branch[pos]->entry[1].value;
  }
  else if (pos < current->count) {  /* Successor is the the next key in the same node */
    /* Replace the entry by its succesor */
    current->entry[pos].key = current->entry[pos+1].key;
    current->entry[pos].value = current->entry[pos+1].value;
  }
  return;
}


/* Removekth: Remove kth smallest key.
   Pre: The B-tree to which root points has been created.
        Index k has a value between 0 and (number of entries - 1). Otherwise the return value is undefined.
   Post: The kth smallest key has been removed.
   Uses: Inorder, DeleteTree */
void Removekth(Treenode *root, unsigned int k)
{
  int Keycounter = STARTCOUNT;
  Key kthKey = Inorder(root, &Keycounter, k);
  DeleteTree(kthKey, root);
  return;
}


/* Inorder: Find the kth smallest key using inorder traversal.
   Pre: The B-tree to which root points has been created.
        Keycounter has been initialized as STARTCOUNT.
        Index k has a value between 0 and (number of entries - 1). Otherwise the return value is undefined.
   Post: The return value is the kth smallest key.
   Uses: Inorder recursively */
Key Inorder(Treenode *root, int *Keycounter, unsigned int k)
{
  int i, kthKey;
  if (root) {
    for (i=0; i<=root->count; i++) {
      kthKey = Inorder(root->branch[i], Keycounter, k);
      if ((*Keycounter) == k) {
        break;
      }
      if (i+1 <= root->count) {
        (*Keycounter) += 1;
        if ((*Keycounter) == k) {
          kthKey = root->entry[i+1].key;
          break;
        }
      }
    }
    return kthKey;
  }
  else if ((*Keycounter) == STARTCOUNT) {
    (*Keycounter) += 1;
  }
}



/* CreateTree: create a B-tree.
   Pre: None.
   Post: An empty B-tree has been created to which root points. */
void CreateTree(Treenode **root)
{
*root=NULL;
}


/* Preorder: print each node of the B-tree in preorder.
   Pre: The B-tree to which root points has been created.
   Post: Each node of the B-tree has been printed in preorder. */
void Preorder(Treenode *root)
{
   int i;

   if (root){
      
      for (i=1; i<=root->count; i++)
         printf("%d\n", root->entry[i].key);

      printf("end of node\n");

      for (i=0; i<=root->count; i++)
        Preorder(root->branch[i]);

   }
}


/* SearchNode: searches keys in node for target.
   Pre: target is a key and current points to a node of a B-tree.
   Post: Searches keys in node for target; returns location pos of target, or
   branch on which to continue search.*/
Boolean SearchNode(Key target, Treenode *current, int *pos)
{
  if (target < current->entry[1].key){  /* Take the leftmost branch */
    *pos=0;
    return FALSE;
  } else {                              /* Start a sequential search through the keys */
    for (*pos=current->count; target < current->entry[*pos].key && *pos > 1; (*pos)--)
      ;
    return (target == current->entry[*pos].key);
  }
}


/* SearchTree: traverse the B-tree looking for target.
   Pre: If the key target is present in the B-tree, then the return value points to
   the node containing target in position targetpos. Otherwise, the return value is
   NULL and targetpos is undefined.
   Uses: SearchTree recursively, SearchNode. */
Treenode *SearchTree(Key target, Treenode *root, int *targetpos)
{
   if (!root)
     return NULL;
   else if (SearchNode(target, root, targetpos))
     return root;
   else
     return SearchTree(target, root->branch[*targetpos], targetpos);
}


/* PushIn: inserts a key into a node.
   Pre: medentry belongs at index pos in node *current, which has room for it.
   Post: Inserts key medentry and pointer medright into *current at index pos. */
void PushIn(Treeentry medentry, Treenode *medright, Treenode *current, int pos)
{
  int i;        /* index to move keys to make room for medentry */

  for (i=current->count; i>pos; i--){
    /* Shift all keys and branches to the right */
    current->entry[i+1]=current->entry[i];
    current->branch[i+1]=current->branch[i];
  }
  current->entry[pos+1]=medentry;
  current->branch[pos+1]=medright;
  current->count++;
}


/* Split: splits a full node.
   Pre: medentry belongs at index pos of node *current which is full.
   Post: Splits node *current with entry medentry and pointer medright at index pos
         into nodes *current and *newright with median entry newmedian.
   Uses: PushIn */
void Split(Treeentry medentry, Treenode *medright, Treenode *current, int pos, Treeentry *newmedian, Treenode **newright)
{
  int i;             /* used for copying from *current to new node */
  int median;        /* median position in the combined, overfull node */

  if (pos<=MIN)     /* Find splitting point. Determine if new key goes to left or right half */
    median=MIN;
  else
    median=MIN+1;

  /* Get a new node and put it on the right */
  *newright=(Treenode *)malloc(sizeof(Treenode));
  for (i=median+1; i<=MAX; i++){    /* Move half the keys to the right node */
    (*newright)->entry[i-median]=current->entry[i];
    (*newright)->branch[i-median]=current->branch[i];
  }
  (*newright)->count=MAX-median;
  current->count=median;

  if (pos <= MIN)          /* Push in the new key */
    PushIn(medentry, medright, current, pos);
  else
    PushIn(medentry, medright, *newright, pos-median);
  *newmedian=current->entry[current->count];
  (*newright)->branch[0]=current->branch[current->count];
  current->count--;
}


/* PushDown: recursively move down tree searching for newentry.
   Pre: newentry belongs in the subtree to which current points.
   Post: newentry has been inserted into the subtree to which current points; if TRUE
   is returned, then the height of the subtree has grown, and medentry needs
   to be reinserted higher in the tree, with subtree medright on its right.
   Uses: PushDown recursively, SearchNode, Split, PushIn. */
Boolean PushDown(Treeentry newentry, Treenode *current, Treeentry *medentry, Treenode **medright)
{
  int pos;              /*branch on which to continues the search */

  if (current==NULL){  /* cannot insert into empty tree; terminates */
    *medentry=newentry;
    *medright=NULL;
    return TRUE;
  } else {             /* Search the current node */
    if (SearchNode(newentry.key, current, &pos))
      printf("Inserting duplicate key into B-tree");
    if (PushDown(newentry, current->branch[pos], medentry, medright))
      if (current->count < MAX){    /*Reinsert median key.  */
        PushIn(*medentry, *medright, current, pos);
        return FALSE;
      } else {
        Split(*medentry, *medright, current, pos, medentry, medright);
        return TRUE;
      }
    return FALSE;
  }
}


/* InsertTree: Inserts entry into the B-tree.
   Pre: The B-tree to which root points has been created, and no entry in the B-tree
   has key equal to newentry key.
   Post: newentry has been inserted into the B-tree, the root is returned.
   Uses: PushDown */
Treenode *InsertTree(Treeentry newentry, Treenode *root)
{
  Treeentry medentry;   /* node to be reinserted as new root */
  Treenode *medright;   /* subtree on right of medentry     */
  Treenode *newroot;   /* used when the height of the tree increases */

  if (PushDown(newentry, root, &medentry, &medright)){
    /* Tree grows in height. Make a new root */
    newroot=(Treenode *)malloc(sizeof(Treenode));
    newroot->count=1;
    newroot->entry[1]=medentry;
    newroot->branch[0]=root;
    newroot->branch[1]=medright;
    return newroot;
  }
  return root;
}


/* Combine: combine adjacent nodes.
   Pre: current points to a node in a B-tree with entries in the branches pos and
   pos-1, with too few to move entries.
   Post: The nodes at branches pos-1 and pos have been combined into one node,
   which also includes the entry formerly in *current at index pos. */
void Combine(Treenode *current, int pos)
{
  int c;
  Treenode *right;
  Treenode *left;
  right=current->branch[pos];
  left=current->branch[pos-1];    /* work with the left node */
  left->count++;                  /* insert the key from the parent */
  left->entry[left->count]=current->entry[pos];
  left->branch[left->count]=right->branch[0];
  for (c=1; c<=right->count; c++){ /* insert all keys from right node */
    left->count++;
    left->entry[left->count]=right->entry[c];
    left->branch[left->count]=right->branch[c];
  }
  for (c=pos; c< current->count; c++){  /* delete key from parent node */
    current->entry[c]=current->entry[c+1];
    current->branch[c]=current->branch[c+1];
  }
  current->count--;
  free(right);           /* dispose of the empty right node */
}


/* MoveLeft: move a key to the left.
Pre: current points to a node in a B-tree with entries in the branches pos and
pos-1, with too few in branch pos-1.
Post: The leftmost entry from branch pos has moved into *current, which has sent
an entry into the branch pos-1 */
void MoveLeft(Treenode *current, int pos)
{
  int c;
  Treenode *t;
  t=current->branch[pos-1];   /* move key from parent into left node */
  t->count++;
  t->entry[t->count]=current->entry[pos];
  t->branch[t->count]=current->branch[pos]->branch[0];
  t=current->branch[pos];  /* Move key from right node into parent */
  current->entry[pos]=t->entry[1];
  t->branch[0]=t->branch[1];
  t->count--;
  for (c=1; c<=t->count; c++){
    /* shift all keys in the right node one position leftward */
    t->entry[c]=t->entry[c+1];
    t->branch[c]=t->branch[c+1];
  }
}


/* MoveRight: move a key to the right.
   Pre: current points to a node in a B-tree with entries in the branches pos and
   pos-1, with too few entries in branch pos.
   Post: The rightmost entry from branch pos-1 has moved into *current, which has
   sent an entry into the branch pos */
void MoveRight(Treenode *current, int pos)
{
  int c;
  Treenode *t;
  t=current->branch[pos];
  for (c=t->count; c>0; c--){
  /* shift all keys in the right node one position */
    t->entry[c+1]=t->entry[c];
    t->branch[c+1]=t->branch[c];
  }
  t->branch[1]=t->branch[0];   /* move key from parent to right node */
  t->count++;
  t->entry[1]=current->entry[pos];
  t=current->branch[pos-1];   /* move last key of left node into parent */
  current->entry[pos]=t->entry[t->count];
  current->branch[pos]->branch[0]=t->branch[t->count];
  t->count--;
}


/* Restore: restore the minimum number of entries.
Pre: current points to a node in a B-tree with an entry in index pos; the branch to
the right of pos has one too few entries.
Post: An entry taken from elsewhere is used to restore the minimum number of entries by
entering it at current->branch[pos].
Uses: MoveLeft, MoveRight, Combine */
void Restore(Treenode *current, int pos)
{
if (pos==0)       /* case: leftmost key */
  if (current->branch[1]->count > MIN)
    MoveLeft(current, 1);
  else
    Combine(current, 1);
  else if (pos == current->count) /*case: rightmost key */
    if (current->branch[pos-1]->count > MIN)
      MoveRight(current, pos);
    else
      Combine(current, pos);
  else if (current->branch[pos-1]->count > MIN)
    MoveRight(current, pos);
  else if (current->branch[pos+1]->count > MIN)
    MoveLeft(current, pos+1);
  else
    Combine(current, pos);
}


/* Remove: delete an entry and the branch to its right.
   Pre: current points to a node in a B-tree with an entry in index pos.
   Post: This entry and the branch to its right are removed from *current */
void Remove(Treenode *current, int pos)
{
  int i;        /* index for moving entries */
  for (i=pos+1; i<=current->count; i++){
    current->entry[i-1]=current->entry[i];
    current->branch[i-1]=current->branch[i];
  }
  current->count--;
}


/* RecDeleteTree: look for target to delete.
   Pre: target is the key of some entry in the subtree of a B-tree to which current points.
   Post: This entry has been deleted from the B-tree.
   Uses: RecDeleteTree recursively, SearchNode, Successor, Remove, Restore */
void RecDeleteTree(Key target, Treenode *current)
{
  int pos;          /* location of target or of branch on which to search */
  if (!current){
    printf("Target was not in the B-tree.\n");
    return;         /* Hitting an empty tree is an error */
  } else {
    if (SearchNode(target, current, &pos))
      if (current->branch[pos-1]){
        Successor(current, pos);  /* replaces entry[pos] by its successor */
        RecDeleteTree(current->entry[pos].key, current->branch[pos]);
      } else
        Remove(current, pos);  /* removes key from pos of *current */
    else                    /* Target was not found in the current node */
      RecDeleteTree(target, current->branch[pos]);
    if (current->branch[pos])
      if (current->branch[pos]->count < MIN)
        Restore(current, pos);
  }
}


/* DeleteTree: deletes target from the B-tree.
   Pre: target is the key of some entry in the B-tree to which root points.
   Post: This entry has been deleted from the B-tree.
   Uses: RecDeleteTree */
Treenode *DeleteTree(Key target, Treenode *root)
{
  Treenode *oldroot;      /* used to dispose of an empty root */

  RecDeleteTree(target, root);
  if (root->count==0){    /* root is empty */
    oldroot=root;
    root=root->branch[0];
    free(oldroot);
  }
  return root;
}
