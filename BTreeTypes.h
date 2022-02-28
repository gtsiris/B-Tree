#define MAX 4                /* maximum number of keys in node. */
#define MIN 2                 /* minimum number of keys in node */
#define STARTCOUNT -2            /* Initial value of Keycounter */



typedef int Key;

typedef struct {
   Key key;
   int value;                        /* values can be arbitrary */
   } Treeentry;

typedef enum {FALSE, TRUE} Boolean;

typedef struct treenode Treenode;

struct treenode {
  int count;     /* denotes how many keys there are in the node */
  Treeentry entry[MAX+1];
  Treenode *branch[MAX+1];
};
