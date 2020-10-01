#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "stringintmap.h"

using namespace std;

//A foreword:
// I'm very sorry this is messy...

struct Webpage {
  string url;
  double weight;
  double newWeight;
  int numLinks;
  int numWords;
  vector<int> links;
  StringIntMap linksLookup;
  vector<int> words;
  Webpage() : numLinks(0), numWords(0) {}
  Webpage(string u) : url(u), numLinks(0), numWords(0) {}
};

struct Word {
  string text;
  string bestNext;
  vector<int> pages;
  vector<int> afterLookup; //TODO : Depricate
  StringIntMap after;
  Word() : text(""), bestNext("") {}
  Word(string txt) : text(txt), bestNext("") {}
};

Webpage* pagesArray;
StringIntMap pagesLookup;
Word* wordsArray;
StringIntMap wordsLookup;

int wordsAmt = 0;
int pagesAmt = 0;

// Returns entire file as one big string, quickly
string read_webpages_fast(const char *filename)
{
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf (stderr, "Error: can't open %s", filename);
    exit(1);
  }
  fseek (fp, 0, SEEK_END);
  int size = ftell(fp);
  fseek (fp, 0, SEEK_SET);
  char *buff = new char[size+1];
  buff[size] = 0;
  int dummy_result = fread (buff, size, 1, fp);
  fclose (fp);
  string result(buff);
  delete [] buff;
  return result;
}

// Some ANSI terminal commands you can print to clear screen and change colors
string clear_screen = "\e[2J\e[H";
string color_red = "\e[31;40m";
string color_green = "\e[32;40m";
string color_yellow = "\e[33;40m";
string color_blue = "\e[34;40m";
string color_magenta = "\e[35;40m";
string color_cyan = "\e[36;40m";
string color_white = "\e[37;40m";
string color_whiteblue = "\e[37;44m";

void findNextBest(Word& word)
{
  int freqCount = 0;
  string* afterWordsArray = word.after.get_keys();

  for(int i = 0; i < word.after.get_num_elems(); i++) {
    if(word.after[afterWordsArray[i]] > freqCount) {
      freqCount = word.after[afterWordsArray[i]];
      word.bestNext = afterWordsArray[i];
    }
  }
  return;
}

void googlePageRank(){
  for(int i = 0; i < 50; i++) {
    //cout << i << endl;
    for(int pDex = 0; pDex < pagesAmt; pDex++) {
      pagesArray[pDex].newWeight = 0.1;
    }

    for(int pDex = 0; pDex < pagesAmt; pDex++) {
      for(int lDex : pagesArray[pDex].links) {
        //cout << "This happends" << endl;
        pagesArray[lDex].newWeight += (0.9 * pagesArray[pDex].weight / pagesArray[pDex].links.size());
        //if(pagesArray[pDex].numLinks < 1) cout << "you're a dingus" << endl;
      }
      if(pagesArray[pDex].links.size() < 1) pagesArray[pDex].newWeight += 0.9*pagesArray[pDex].weight;
    }

    for(int pDex = 0; pDex < pagesAmt; pDex++) {
      pagesArray[pDex].weight = pagesArray[pDex].newWeight;
      pagesArray[pDex].newWeight = 0.1;
    }
  }
}

bool comparePages(Webpage p1, Webpage p2) {
  return (p1.weight > p2.weight);
}

bool comparePagesSpecial(int p1, int p2) {
  return (pagesArray[p1].weight > pagesArray[p2].weight);
}

void predict(string query)
{
  if(wordsLookup.find(query) && query != "") {
  cout << color_green << "The next best word based on your search is: '"
       << color_white << wordsArray[wordsLookup[query]].bestNext
       << color_green << "'\n"
       << "There are " << wordsArray[wordsLookup[query]].pages.size() << " pages that your word is in." << endl
       << "Hopefully this just works: " << endl << endl
       << "\n\n";
      if((wordsArray[wordsLookup[query]].pages.size()) > 4) {
        sort(wordsArray[wordsLookup[query]].pages.begin(), wordsArray[wordsLookup[query]].pages.end(), comparePagesSpecial); //TODO hey man... this might kill realtime...
        cout << pagesArray[wordsArray[wordsLookup[query]].pages.at(0)].url << endl << "Weight: " << pagesArray[wordsArray[wordsLookup[query]].pages.at(0)].weight  << endl;
        cout << pagesArray[wordsArray[wordsLookup[query]].pages.at(1)].url  << endl << "Weight: " << pagesArray[wordsArray[wordsLookup[query]].pages.at(1)].weight  << endl;
        cout << pagesArray[wordsArray[wordsLookup[query]].pages.at(2)].url  << endl << "Weight: " << pagesArray[wordsArray[wordsLookup[query]].pages.at(2)].weight  << endl;
        cout << pagesArray[wordsArray[wordsLookup[query]].pages.at(3)].url  << endl << "Weight: " << pagesArray[wordsArray[wordsLookup[query]].pages.at(3)].weight  << endl;
      }
  }
}

void process_keystrokes()
{
  int ch = 0;
  string query;

  while (ch != '\n') {
    cout << clear_screen << color_green << "Search keyword: "
	 << color_white << query
	 << color_green << "-\n\n";

    predict(query);
    cout << flush;

    struct termios oldt, newt;
    tcgetattr (STDIN_FILENO, &oldt);
    newt = oldt; newt.c_lflag &= ~(ICANON|ECHO);
    tcsetattr (STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr (STDIN_FILENO, TCSANOW, &oldt);

    if (ch == 8 || ch == 127) { // backspace
      if (query.length() > 0) query = query.substr(0, query.length()-1);
    } else if (ch != ' ' && ch != '\n') query += ch;
  }
  cout << color_white;
}

// This shows how to use some of the starter code above
int main(void)
{
  cout << color_green << "Reading input..." << endl;
  const char *filename = "webpages.txt";
  istringstream webfile (read_webpages_fast (filename));

  cout << "L1" << endl;

  string s; //Current string read in
  int P = -1, W = 0; //Page, Word count
  while (webfile >> s) {
    if (s == "PAGE") {
      P++;
      webfile >> s; //url of page
      pagesLookup[s] = P;
      //P++; //inc page count
    } else if(s == "LINK") {
      webfile >> s; //Skip links for now
    } else {
      if(!wordsLookup.find(s)) { //No dupe words
        wordsLookup[s] = W;
        W++; //Inc word count only on new words
      }
    }
  }

  pagesArray = new Webpage[P];
  pagesAmt = P;
  wordsArray = new Word[W];
  wordsAmt = W;

  P = -1;
  W = 0;
  s = "";
  webfile.clear();
  webfile.seekg(0);

  cout << "L2" << endl;

  string preWord = "";
  while (webfile >> s) {
    if (s == "PAGE") {
      P++;
      webfile >> s; //url of page
      pagesArray[P].url = s;
      pagesArray[P].weight = 1;
      //P++; //inc page count
    } else if(s == "LINK") {
      webfile >> s; //Skip links for now
      if(pagesLookup.find(s)) pagesArray[P].links.push_back(pagesLookup[s]);
    } else {
      pagesArray[P].numWords++;
      wordsArray[wordsLookup[s]].text = s;
      pagesArray[P].words.push_back(wordsLookup[s]);
      //if(pagesArray[P].linksLookup.find(pagesArray[P].url)) {
      //  pagesArray[P].words.push_back(wordsLookup[s]);
      //  pagesArray[P].linksLookup.insert(pagesArray[P].url, P);
      //}
      wordsArray[wordsLookup[s]].pages.push_back(P);
      if(preWord != "") wordsArray[wordsLookup[preWord]].after[s]++;
      preWord = s;
    }
  }

  cout << "Loops End" << endl;

  cout << "Finding all next best words" << endl;

  for(int i = 0; i < wordsAmt; i++) {
    findNextBest(wordsArray[i]);
  }

  cout << "Ranking Pages" << endl;

  googlePageRank();

  /*
  cout << "Printing Tops.." << endl;

  int specialNumber = sizeof(pagesArray)/sizeof(pagesArray[0]);
  //int specialerNumber = 
  sort(pagesArray, pagesArray+pagesAmt, comparePages);

  cout << "Top Page: " << pagesArray[0].url << " Weight: " << pagesArray[0].weight << endl;
  cout << "2nd Page: " << pagesArray[1].url << " Weight: " << pagesArray[0].weight << endl;
  cout << "3rd Page: " << pagesArray[2].url << " Weight: " << pagesArray[0].weight << endl;
  cout << "4th Page: " << pagesArray[3].url << " Weight: " << pagesArray[0].weight << endl;
  */

  cout << "Starting Search" << endl;
  // Enter loop to ask for query
  process_keystrokes();

  //TODO Free

  return 0;
}
