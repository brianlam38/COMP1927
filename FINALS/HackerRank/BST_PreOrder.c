// ########################
// TREE: Preorder Traversal
// DIFFICULTY: Easy
// ########################

// Complete the preOrder function in your editor below, which has  parameter:
// a pointer to the root of a binary tree. It must print the values in the tree's
// preorder traversal as a single line of space-separated values.

// Output Format
// Print the tree's preorder traversal as a single line of space-separated values.

/* Complete the function below:
Node is defined as  

struct node
{
    int data;
    node* left;
    node* right;
};

*/

void preOrder(node *root) {
    
    node *curr = root;
    
    if (curr != NULL) {
        printf("%d ",curr->data);
        preOrder(curr->left);
        preOrder(curr->right);
    }
}