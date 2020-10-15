#include <iostream>
using namespace std;

struct Node {
  string key;
  int size;    // number of nodes in this node's subtree
  Node *left;
  Node *right;
  Node (string k) { key = k; size = 1; left = right = NULL; }
};

int myhash(string s, int table_size)
{
  unsigned int i, h = 0;
  for (i=0; i<s.length(); i++)
    h = (h * 2917 + (unsigned int)s[i]) % table_size;
  return h;
}

// insert key k into tree T, returning a pointer to the resulting tree
Node *insert(Node *T, string k)
{
  if (T == NULL) return new Node(k);
  if (k < T->key) T->left = insert(T->left, k);
  else T->right = insert(T->right, k);
  T->size++;
  return T;
}

// prints out the inorder traversal of T (i.e., the contents of T in sorted order)
void print_inorder(Node *T)
{
  if (T==NULL) return;
  print_inorder(T->left);
  cout << T->key << endl;
  print_inorder(T->right);
}

// return a pointer to the node with key k in tree T, or NULL if it doesn't exist
Node *find(Node *T, string k)
{
  if (T == NULL) return NULL;
  if(k == T->key) return T;
  if (k < T->key) return find(T->left, k);
  else return find(T->right, k);
}

// Return pointer to the node with key k if it exists
// If not, return a pointer to the node with the largest key smaller than k (i.e., 
// k's predecessor) or NULL if there isn't any node with key smaller than k.
Node *predfind(Node *T, string k)
{ //Hopefully this just works... don't think too hard
  if (T==NULL) return NULL;
  if (T->key == k) return T;
  else if(k < T->key) return predfind(T->left, k);
  else if(predfind(T->right, k) == NULL) return T;
  else return predfind(T->right, k);
}

// Join trees L and R (with L containing keys all <= the keys in R)
// Return a pointer to the joined tree.  
Node *join(Node *L, Node *R)
{
  // choose either the root of L or the root of R to be the root of the joined tree
  // (where we choose with probabilities proportional to the sizes of L and R)
  if(L == NULL && R == NULL) return NULL;
  else if(L == NULL) return R;
  else if(R == NULL) return L;

  // Probably need some base cases here or else the next line will crash...
  int total = L->size + R->size;
  int randn = rand() % total;  // Pick random number in {0, 1, ..., total-1}
  if (randn < L->size) { // Happens with probability L->size / total
    // Make L the root of the joined tree in this case
    if (L->key < R->key) L->right = join(L->right, R);
    else L->left = join(L->left, R);
    L->size = total;
    return L;
  } else {               // Happens with probability R->size / total
    // Make R the root of the joined tree in this case
    if (R->key < L->key) R->right = join(R->right, L);
    else R->left = join(R->left, L);
    R->size = total;
    return R;
  }
}

// remove key k from T, returning a pointer to the resulting tree.
// if k isn't present in T, then return the pointer to T's root, with T unchanged
Node *remove(Node *T, string k)
{
  if (T==NULL) return NULL;
  if(k == T->key) {
    Node *newTree = join(T->left, T->right);
    delete T;
    return newTree;
  }
  if (k < T->key) T->left = remove(T->left, k);
  else            T->right = remove(T->right, k);
  //Very cool feature - if you don't make sure the element is in the tree it breaks the whole prog :)
  T->size--; 
  return T;
}

// Split tree T on key k into tree L (containing keys <= k) and a tree R (containing keys > k)
pair<Node *, Node *> split(Node *T, string k) {
  Node *leftRoot = NULL;
  Node *rightRoot = NULL;

  if(T==NULL) return make_pair(T,T);
  else if (k < T->key) {
    rightRoot = T;
    pair<Node*, Node*> result = split(T->left,k);
    T->left = result.second;
    leftRoot = result.first;
    if(leftRoot != NULL) rightRoot->size -= leftRoot->size;
    return make_pair(leftRoot, rightRoot);
  } else {
    leftRoot = T;
    pair<Node*, Node*> result = split(T->right,k);
    T->right = result.first;
    rightRoot = result.second;
    if(rightRoot != NULL) leftRoot->size -= rightRoot->size;
    return make_pair(leftRoot, rightRoot);
  }
}


// insert key k into the tree T, returning a pointer to the resulting tree
// keep the tree balanced by using randomized balancing.
// We'll discuss the algorithm used here in lecture:
// if N represents the size of our tree after the insert, then
// ... with probability 1/N, insert k at the root of T (by splitting on k)
// ... otherwise recursively call insert_keep_balanced on the left or right
Node *insert_keep_balanced(Node *T, string k)
{
  if (T == NULL) return new Node(k);
  if (rand() % (1 + T->size) == 0) {
    // With probability 1/N, insert at root...
    pair<Node *, Node *> result = split (T, k);
    T = new Node(k); // ok to re-use T for new root, since result keeps pointers to remnants of original tree after splitting it
    T->left = result.first;
    T->right = result.second;
  } else {
    // Otherwise, recurseively insert on left or right side...
    if (k < T->key) T->left  = insert_keep_balanced(T->left,  k);
    else            T->right = insert_keep_balanced(T->right, k);
  }
  // Make sure T's size is correct, since its subtrees may have changed
  T->size = 1;
  if (T->left != NULL) T->size += T->left->size;
  if (T->right != NULL) T->size += T->right->size;
  return T;
}

//Yoink ^^

int main(void)
{
  Node* trunk = NULL;
  string s1 = "", s2 = "";
  cout << "Getting input... " << endl;
  while(cin >> s1 >> s2){
    Node* tinyTree = insert(NULL, s1);
    tinyTree = insert(tinyTree, s2);
    trunk = join(trunk, tinyTree);
  }
  cout << "Input done." << endl;

  cout << "Printing tree... " << endl;
  print_inorder(trunk);

}