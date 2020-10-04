#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;
typedef pair<double, double> cut;

// *** BST CODE BELOW ***

struct Node {
  double key;
  int size;    // number of nodes in this node's subtree
  Node *left;
  Node *right;
  Node (double k) { key = k; size = 1; left = right = NULL; }
};

// insert key k into tree T, returning a pointer to the resulting tree
Node *insert(Node *T, double k)
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
Node *find(Node *T, double k)
{
  if (T == NULL) return NULL;
  if(k == T->key) return T;
  if (k < T->key) return find(T->left, k);
  else return find(T->right, k);
}

// Return pointer to the node with key k if it exists
// If not, return a pointer to the node with the largest key smaller than k (i.e., 
// k's predecessor) or NULL if there isn't any node with key smaller than k.
Node *predfind(Node *T, double k)
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
Node *remove(Node *T, double k)
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
pair<Node *, Node *> split(Node *T, double k) {
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
Node *insert_keep_balanced(Node *T, double k)
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
//returns the number of nodes in the tree with keys < x
int get_rank(Node* T, double k) {
  if(!T) return 0;
  if(k == T->key) return T->left ? T->left->size : 0;
  else if(k < T->key) return get_rank(T->left, k);
  else return (T->left ? T->left->size : 0) + 1 + get_rank(T->right, k);
}

// prints out the inorder traversal of T (i.e., the contents of T in sorted order)
void print_rank_inorder(Node* head, Node *T)
{
  if (T==NULL) return;
  print_rank_inorder(head, T->left);
  cout << "Key: " << T->key << " is rank: " << get_rank(head, T->key) << endl;
  print_rank_inorder(head, T->right);
}

// *** BST CODE ABOVE ***

// ** N^2 runtime solution below **

void nSquaredPancake() {
  vector<cut> V;
  double angle1 = 0, angle2 = 0;

  while(cin >> angle1 >> angle2)
    angle1 < angle2 ? V.push_back(make_pair(angle1, angle2)) : V.push_back(make_pair(angle2, angle1));

  int N = 0, P = 0;
  for(cut C1 : V){
    for(cut C2 : V) {
      if((C1 != C2) && ((C1.first < C2.first) && (C1.second < C2.second) && (C2.first < C1.second))) P++;
    }
    N++;
  }

  cout << "Cuts: " << N << " Intersections (Should be: 415199866): " << P << endl;
  cout << "Answer: " << 1 + N + P << endl;

  return;
}

// ** N^2 runtime solution above **

// ** NlogN runtime solution below **

void nLogNPancake() {
  vector<cut> V;
  Node* T = NULL;
  double angle1=0, angle2=0;
  int N = 0, P = 0;

  while(cin >> angle1 >> angle2) {
    V.push_back(make_pair(angle1, angle2));
    V.push_back(make_pair(angle2, angle1));
    N++;
  }
  sort(V.begin(), V.end());

  for(cut C : V){
    if (!find(T, C.second) && C.second > C.first)  T=insert_keep_balanced(T, C.second); 
    if ( find(T, C.first)  && C.first  > C.second) T=remove(T, C.first);
    P+=get_rank(T, C.second);
    
  }

  cout << "Cuts: " << N << " Intersections (Should be: 415199866): " << P << endl;
  cout << "Answer: " << 1 + N + P << endl;

  return;
}

// ** NlogN runtime solution below **

int main() {
  cout << "Finding parts in N^2 time..." << endl;
  nSquaredPancake();
  cout << "Done." << endl << endl;

  return 0;
}