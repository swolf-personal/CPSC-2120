#include <iostream>
using namespace std;

struct Node {
  int key;
  Node *next;
  Node (int k, Node *n) { key = k; next = n; }
};

void print_list(Node *head)
{
  for (Node *n = head; n != NULL; n = n->next) cout << n->key << " ";
  cout << "\n";
}

// Restore list to sorted order.  Return the head pointer of the resulting list.
Node *fix_list(Node *head)
{
  if(head == NULL) return head;
  if (head->next == NULL) return head;

  if (head->key < head->next->key) {
    head->next = fix_list(head->next);
    return head;
  }

  else { //Head is greater than next. Send it down the line
    Node *temp1 = head;
    Node *temp2 = head->next;
    Node *temp3 = head->next->next;
    temp1->next = temp3;
    temp2->next = temp1;
    temp2->next = fix_list(temp1);
    return temp2; //Dear god I wish I could refactor this... it's pretty cool though. Hi Dr. Dean
  }
  
  return head;
}

int main(void)
{
  Node *head = NULL;

  // Make list with one out-of-place node
  for (int x=100; x>=50; x-=9) head = new Node(x, head);
  head = new Node(87, head);
  for (int x=45; x>=0; x-=13) head = new Node(x, head);

  cout << "Original list:  "; print_list(head);
  cout << "Corrected list: "; print_list(fix_list(head));
}