#include <iostream>
#include <fstream>
#include <map>
using namespace std;

const int N = 1000;
bool ans[N+1];
bool Q[N+1];
bool ambig[N+1];
map<int, bool> helper; 

/*

As an example for part 1, if question 335 is answered with 'F', 
then you'd need to answer question 1 with 'T' to make question 1 correct

*/

int main(void)
{
  ifstream tf_file("tf.txt");
  ofstream answer_file("answers.txt");;
  int num = 0;
  string TF, qnum, the, answer, to, question, is, s;

  for (int i=1; i<=N; i++) {
    tf_file >> qnum >> TF >> the >> answer >> to >> question >> num >> is >> s; ans[num] = (s=="T");
  } //Read in answers to questions
  num = 0;
  tf_file.clear();
  tf_file.seekg(0, std::ios::beg);
  for (int i=1; i<=N; i++) { //If the line is true then the line and question should agree
    tf_file >> qnum >> TF >> the >> answer >> to >> question >> num >> is >> s;
    if (s == "T") Q[i]=ans[num];    
    else          Q[i]=!ans[num];      
  }
  for (int i=1; i<=N; i++) {answer_file << (Q[i] ? "T" : "F") << endl;}
}