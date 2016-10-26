/*  DELETE VALUE FROM BSTree */
//  Return new BSTree t without Key k

// PART 1: TRAVERSE TO NODE
BSTree BSTreeDelete(BSTree t, Key k)
{
	// CASE #1: empty tree => nothing to delete
	if (t == NULL) return NULL;

	// CASE #2: k smaller than root =>
	//         new left subtree has k deleted
	if (k < t->value)
		t->left = BSTreeDelete(t->left,k);

	// CASE #3: k larger than root =>
	//         new right subtree has k deleted
	if (k > t->value)
		t->right = BSTreeDelete(t->right,k);

	// case #4: k is value at root =>
	//         new tree has root deleted
	// KEY VALUE IS FOUND. DELETE CURRENT NODE.
	if (k == t->value)
		t = deleteRoot(t);

	return t;
}

// PART 2: DELETE NODE
// We delete by copying successor value into root node + deleting successor
BSTree deleteRoot(BSTree t)
{
	Link newRoot;

	// CASE #1: no subtrees => tree empty after delete
	if (t->left == NULL && t->right == NULL) {	// LHS = NULL and RHS = NULL
		free(t);
		return NULL;
	}

	// CASE #2a: RIGHT SUBTREE => make it the new root
	if (t->left == NULL && t->right != NULL) {
		newRoot = t->right; // make child node -> new root node
		free(t);			// remove old root node
		return newRoot;			// NOTE: Linking back to parent is handled
	}							//       by recursion at the higher level

	// CASE #2b: LEFT SUBTREE => make it the new root
	if (t->left != NULL && t->right == NULL) {
		newRoot = t->left;
		free(t);
		return newRoot;
	}

	// CASE #3: TWO SUBTREES => replace root by successor
	if (t->left != NULL && t->right != NULL) {
	{
		Link curr = t->right;		// Move to RHS Subree
		while (curr->left != NULL) {// Find successor node
			curr = curr->left;			// (left-most node)
		}
		Key succ = curr->value;		// Take succ node val
		t = BSTreeDelete(t,succ);	// Delete succ node
		t->value = succ;			// Copy succ node val to root val

		return t;
	}
}
