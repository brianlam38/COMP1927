// 2-3-4 Tree Implementation

// Node structure
typedef struct node Node;
typedef struct node *Tree;

struct node {
	int order;		// 2,3,4
	Item data[3];	// Array of items (in order to be searched)
	Tree child[4];	// Array of childs / subtrees (links to subtrees)
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
	int nitems = t->order-1;			// #1 Set no.items depending on order
	// find relevant slot in items
	for (i = 0; i < nitems; i++) {		// #2 Scan along nodes to find a value that
		diff = cmp(k, key(t->data[i]));	//    is larger than one we're searching for.
		if (diff <= 0) break;			//    Stop when larger key is found
	}
	if (diff == 0)						// #3 Item is found, return ptr to item
		return &(t->data[i]);
	else
		return search(t->child[i], k);	// #4 Keep searching for key
}

// Insertion Algorithm (pseudocode)
// This applies to a 4-node tree
void insert(Tree, Item) {
	Node = search(Tree, key(Item));
	Parent = parent of Node;
	if (order(Node) < 4) {					// #1 order < 3 --> simple insert
		insert Item in Node;
		order++;
	} else {								// #2 order > 4 --> needs promotion
		promote = Node.data[1];				//    Set mid Item for promote
		NodeL = newNode containing data[0]; //	  Set NodeL
		NodeR = newNode containing data[2]; //	  Set NodeR
		
		if (key(Item) < key(data[1])) {
			insert Item in NodeL;
		else
			insert Item in NodeR;
		
		insert promote to Parent
		
		while (order(Parent) == 4) {
			keep promoting / split upwards
		if (isRoot(Parent) && order(Parent) == 4)
			split root, making new root;
	}
}





