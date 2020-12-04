
struct Node {  
  double key[11];  
  Node *left, *right;  
  Node (double *k) { key = k; left = right = NULL; } // needs to be modified still...
};

Node *insert(Node *root, double *val, int dim) {  
  if (root == NULL) return new Node(val);  
  if (val[dim] < root->key[dim]) root->left = insert(root->left, val, (dim+1)%11);  
  else root->right = insert(root->right, val, (dim+1)%11);  return root;
}
  
  // double P[] = {1,2,3,4,5,...};
  // root = insert(root, P, 0);