// Red Black Trees

// assume t is never NULL
#define L(t)   ((t) == NULL ? NULL : (t)->left)
#define R(t)   ((t) == NULL ? NULL : (t)->right)
#define red(t) ((t) != NULL && (t)->colour == RED)

// ################################
// RB Search: Standard BST approach
// ################################
Item *search(Tree t, Key k) {
	if (t == NULL) return NULL;		 // #1 empty tree
	int diff = cmp(k, key(t->data));
	if (diff < 0)					 // #2 LHS subtree
		return search(t->left, k);
	else if (diff > 0)				 // #3 RHS subtree
		return search(t->right, k);
	else							 // #4 found item, return ptr to item
		return &(t->data);
}

// ############
// RB Insertion
// ############

typedef enum {RED,BLACK} Colr;
typedef enum {LEFT,RIGHT} Dirn;

// Insertion Wrapper Function
void insertRedBlack(Tree t, Item it)
{
	t->root = insertRB(t->root, it, 0);	// start at root, insert item.
										// paramter #3 -> Go LHS or RHS
											// inRight = 0 -> Go LHS
											// inRight = 1 -> Go RHS
	t->root->colour = BLACK;
}

// Main insertion function (recursive)
Tree insertRB(Link t, Item it, Dirn dir)
{
   Key v = key(it);
   if (t == NULL) return newNode(it);
   if (red(L(t)) && red(R(t))) {
      t->colour = RED;
      t->left->colour = BLACK;
      t->right->colour = BLACK;
   }
   if (less(v, key(t->value))) {
      t->left = insertRB(t->left, it, LEFT);
      if (red(t) && red(L(t)) && dir==RIGHT)
         t = rotateR(t);
      if (red(L(t)) && red(L(L(t)))) {
         t = rotateR(t);
         t->colour = BLACK;
         t->right->colour = RED;
      }
   }
   else {
      t->right = insertRB(t->right, it, RIGHT);
      if (red(t) && red(R(t)) && dir==LEFT)
         t = rotateL(t);
      if (red(R(t)) && red(R(R(t)))) { 
         t = rotateL(t);
         t->colour = BLACK;
         t->left->colour = RED;
      }
    }
    return t;
}




