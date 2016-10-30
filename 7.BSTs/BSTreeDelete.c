/*  DELETE VALUE FROM BSTree */
//  Return new BSTree t without Key k

// PART 1: TRAVERSE TO NODE
BSTree BSTreeDelete(BSTree t, Key k)
{
	if (t == NULL)							// #1: Empty tree, return NULL
		return NULL;						//     (or non existent value)

	if (k < t->value)						// #2: k < curr value, go to LHS
		t->left = BSTreeDelete(t->left,k);

	if (k > t->value)						// #3: k > curr value, go to RHS
		t->right = BSTreeDelete(t->right,k);

	if (k == t->value)						// #4: node reached, call
		t = deleteRoot(t);					//     deleteRoot(t) fn

	return t;
}

// PART 2: DELETE NODE
// We delete by copying successor value
// into root node + deleting successor
BSTree deleteRoot(BSTree t)
{
	Link newRoot;

	// CASE #1: NO SUBTREES => simply free t
	if (t->left == NULL && t->right == NULL) {
		free(t);
		return NULL;
	}

	// CASE #2a: RIGHT SUBTREE => make it the new root
	if (t->left == NULL && t->right != NULL) {
		newRoot = t->right;
		free(t);
		return newRoot;
	}

	// CASE #2b: LEFT SUBTREE => make it the new root
	if (t->left != NULL && t->right == NULL) {
		newRoot = t->left;
		free(t);
		return newRoot;
	}

	// CASE #3: TWO SUBTREES => replace root by successor
	if (t->left != NULL && t->right != NULL) {
	{
		Link curr = t->right;		// Move to RHS Subtree
		while (curr->left != NULL) {// Find successor node
			curr = curr->left;			// (left-most node)
		}
		Key succ = curr->value;		// Take succ node val
		t = BSTreeDelete(t,succ);	// Go back to top of tree +
									// delete successor node
		t->value = succ;			// Copy succ val in root

		return t;
	}
}
