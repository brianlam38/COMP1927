// a Tree is represented by a ptr to its ROOT NODE
typedef struct Node *Tree;

// a Node contains its data(key), LHS and RHS subtrees
typedef struct Node {
	int data;
	Tree left;
	Tree right;
} Node;

// macros that we might occasionally use
#define data(tree) ((tree)->data)
#define left(tree) ((tree)->left)
#define right(tree) ((tree)->right)


