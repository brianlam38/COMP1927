/* Traversal with visit operation
	(*visit)(item) = Do something when we visit an item
	Example: Print something out, compute something etc.

	Visit function: Takes item, does something to it, no returns.
*/
void Traverse(Tree t, void (*visit)(Item)) {
	if (t != NULL) {
		// visit data in root node FIRST = NLR (prefix)
		Traverse(t->left, visit);
		// visit data in root node MIDDLE = LNR (infix)
		Traverse(t->right,visit);
		// visit data in root node LAST = LRN (postfix)
 	}
 	// Level-order cannot be implemented recursively
}

/* Passing ptr into function

   void(*visit)(item)

   *visit = ptr called "visit" to a function with parameter of type (Item)
   (*visit) = *visit as a parameter to the void() function.

*/

