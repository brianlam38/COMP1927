typedef int TreeItem;
#define TreeItemShow(i)     printf("%d",(i))

typedef struct treeNode * Treelink;

//Functions to modify the tree
Treelink createTree(void);
void destroyTree(Treelink tree);
Treelink insertRec (Treelink tree, TreeItem item);
Treelink insertIterative(Treelink tree, TreeItem key) ;
Treelink delete(Treelink tree,TreeItem);


//Functions to query the tree
TreeItem getItem(Treelink t);
int height(Treelink t);
int size(Treelink t);
Treelink search(Treelink t, TreeItem i);
Treelink searchIterative(Treelink t, TreeItem i);


//Functions to traverse the tree
void preorderTraversal (Treelink tree);
void inorderTraversal (Treelink tree);
void postorderTraversal (Treelink tree);


void showBSTree(Treelink t);

//PRACTICE QUESTIONS WILL PROVIDE MORE INFO LATER
int sumEven(Treelink tree); //easy
Treelink getSmallest(Treelink tree); //easy
int numInternalNodes(Treelink tree); //easy
int hasPathSum(Treelink tree, int sum);
void doubleTree(Treelink tree);
Treelink trimTree(Treelink tree, int min, int max);
