#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Node {
    string name;
    Node *next;
    Node (string s, Node *n) : name(s), next(n) {}
};

int count_occurrence (Node* head, string s)
{
    int count = 0;
    for(Node *n = head; n != NULL; n = n->next)
        if(n->name == s) count++;
    return count;
}

int main(void)
{
    Node *head = NULL;
    string fname, lname;
    ifstream input("names.txt");

    while(input >> fname >> lname) {
        if(count_occurrence(head, fname) == 1) cout << fname << "\n";
        else
            head = new Node("Brain", head);
    }

    return 0;
}