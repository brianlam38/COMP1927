// delete a value from a BSTree
// - return new BSTree t without Key k

BSTree BSTreeDelete(BSTree t, Key k)
{
	// case 1: empty tree => nothing to delete
	if (t == NULL) return NULL;

	// case 2: k smaller than root =>
	//         new left subtree has k deleted
	if (k < t->value)
		t->left = BSTreeDelete(t->left,k);

	// case 3: k larger than root =>
	//         new right subtree has k deleted
	if (k > t->value)
		t->right = BSTreeDelete(t->right,k);

	// case 4: k is value at root =>
	//         new tree has root deleted
	if (k == t->value)
		t = deleteRoot(t);

	return t;
}

// delete root of tree

BSTree deleteRoot(BSTree t)
{
	Link newRoot;

	// case 1: no subtrees => tree empty after delete
	if (t->left == NULL && t->right == NULL) {
		free(t);
		return NULL;
	}

	// case 2a: just right subtree => make it the new root
	if (t->left == NULL && t->right != NULL) {
		newRoot = t->right;
		free(t);
		return newRoot;
	}

	// case 2b: just left subtree => make it the new root
	if (t->left != NULL && t->right == NULL) {
		newRoot = t->left;
		free(t);
		return newRoot;
	}

	// case 3: two subtrees => replace root by successor
	if (t->left != NULL && t->right != NULL) {
	{
		// - find successor  (min value > root value)
		Link curr = t->right;
		while (curr->left != NULL)
			curr = curr->left;
		Key succ = curr->value;
		// - delete successor node
		t = BSTreeDelete(t,succ);
		// - put its value in root node
		t->value = succ;
		return t;
	}
}
