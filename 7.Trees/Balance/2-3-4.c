// 2-3-4 Tree Implementation

// Node structure
typedef struct node Node;
typedef struct node *Tree;

struct node {
	int order;		// 2,3,4
	Item data[3];	// Array of items
	Tree child[4];	// Array of childs / subtrees
}

// Make a new 2-3-4 node (always order 2)
Node *newNode(Item it)
{
	Node *new = malloc(sizeof(Node));	  // Allocate mem
	assert(new != NULL);

	new->order = 2;						  // Init values
	new->data = it;
	new->child[0] = new->child[1] = NULL; // Set child = NULL

	return new;
}

// Searching 2-3-4 Trees (recursive)
// Returning ptr to an item
Item *search(Tree t, Key k)
{
	if (t == NULL) return NULL;			// Empty tree
	int i; int diff;
	int nitems = t->order-1;			// Set #items depending on order
	// find relevant slot in items
	for (i = 0; i < nitems; i++) {		// Scan along nodes to find a value that
		diff = cmp(k, key(t->data[i]));	// is larger than one we're searching for.
		if (diff <= 0) break;			// Stop when larger key is found
	}
	if (diff == 0)						// Item is found, return ptr to item
		return &(t->data[i]);
	else
		return search(t->child[i], k);	// Keep searching for key
}






