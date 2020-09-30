#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "stringintmap.h"

using namespace std;

struct Webpage {
  string url;
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
  vector<int> afterLookup;
  StringIntMap after;
  Word() : text(""), bestNext("") {}
  Word(string txt) : text(txt), bestNext("") {}
};

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

string findNextBest(Word* word, vector<Word*> words, StringIntMap* wordsLookup)
{
  string bestWordYet = "";
  int freqCount = 0;
  for(int id : word->afterLookup) {
    if(word->after[words.at(id)->text] > freqCount) {
      freqCount = word->after[words.at(id)->text];
      bestWordYet = words.at(id)->text;
    }
  }
  word->bestNext = bestWordYet;
  return bestWordYet;
}

void predict(string query, vector<Word*> words, StringIntMap* wordsLookup)
{
  cout << color_green << "Here is where query results for '"
       << color_white << query
       << color_green << "' should go\n"
       << color_white << "Next best: " << words.at(wordsLookup->operator[](query))->bestNext << endl;
}

void process_keystrokes(vector<Word*> words, StringIntMap* wordsLookup)
{
  int ch = 0;
  string query;

  while (ch != '\n') {
    cout << clear_screen << color_green << "Search keyword: "
	 << color_white << query
	 << color_green << "-\n\n";

    predict(query, words, wordsLookup);
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

Webpage* pagesArray;
StringIntMap pagesLookup;
Word* wordsArray;
StringIntMap wordsLookup;

// This shows how to use some of the starter code above
int main(void)
{
  cout << color_green << "Reading input..." << endl;
  const char *filename = "webpages.txt";
  istringstream webfile (read_webpages_fast (filename));

  
  vector<Webpage*> pages; //TODO - Depricate
  vector<Word*> words;

  cout << "L1" << endl;

  string s; //Current string read in
  int P = 0, W = 0; //Page, Word count
  while (webfile >> s) {
    if (s == "PAGE") {
      webfile >> s; //url of page
      pagesLookup[s] = 1;
      P++; //inc page count
    } else if(s == "LINK") {
      webfile >> s; //Skip links for now
    } else {
      if(wordsLookup[s] < 1) { //No dupe words
        wordsLookup[s] = 1;
        W++; //Inc word count only on new words
      }
    }
  }

  pagesArray = new Webpage[P];
  wordsArray = new Word[W];

  P = 0;
  W = 0;
  s = "";
  webfile.clear();
  webfile.seekg(0);

  cout << "L2" << endl;

  while (webfile >> s) {
    if (s == "PAGE") {
      webfile >> s; //url of page
      //pages.push_back(new Webpage(s)); //Push back actual page obj
      pagesArray[pagesLookup[s]].url = s;
      //pagesLookup[s] = P; //Put a reference to the page obj in the map
      P++; //inc page count
    } else if(s == "LINK") {
      webfile >> s; //Skip links for now
    } else {
      if(wordsLookup[s] < 1) { //No dupe words
        //words.push_back(new Word(s)); //Push back word obj
        wordsArray[wordsLookup[s]].text = s; //Place a reference to the word obj
        W++; //Inc word count only on new words
      }
    }
  }

  P = 0;
  W = 0;
  s = "";
  webfile.clear();
  webfile.seekg(0);

  cout << "L3" << endl;
 
  string p;
  string preWord = "";
  while (webfile >> s) {
    if (s == "PAGE") {
      webfile >> s; //url
      p = s; //working page
      P++; //inc page count. TODO : Do I need this?
    } else if(s == "LINK") {
      webfile >> s; //LINK URL
      if(pagesLookup.find(s) && !(pages[pagesLookup[p]]->linksLookup.find(s))) { //Validate non-dead link TODO : Dupe check?
        pages[pagesLookup[p]]->links.push_back(pagesLookup[s]);
        pages[pagesLookup[p]]->linksLookup[s] = 1;
        pages[pagesLookup[p]]->numLinks++;
      }
    } else {
      //Implace word references into pages with order and duplicate preserved
      pages[pagesLookup[p]]->words.push_back(wordsLookup[s]);
      pages[pagesLookup[p]]->numWords++;

      //Increment the word before with this new word as it comes after
      //TODO : Basically this is wrong... need to account for ALL words that come after
      if(preWord != "") {
        words[wordsLookup[preWord]]->after[s]++; //TODO : Validate that this works even when word didn't appear...
        words[wordsLookup[preWord]]->afterLookup.push_back(wordsLookup[s]);
      }

      W++; //Simple word count 
      preWord = s;
    }
  }

  cout << "Loops End" << endl;

  for(Word* w : words) {
    findNextBest(w, words, &wordsLookup);
  }

  // Enter loop to ask for query
  process_keystrokes(words, &wordsLookup);

  //TODO Free

  for(Webpage* wp : pages){
    delete wp;
  }
  for(Word* w : words) {
    delete w;
  }

  return 0;
}
