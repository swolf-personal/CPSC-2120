#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>

#include <vector>

#include "stringintmap.h"
#include "pagemap.h"

using namespace std;

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

void predict(string query)
{
  cout << color_green << "Here is where query results for '"
       << color_white << query
       << color_green << "' should go\n";
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

struct Webpage {
  string url;
  int numLinks;
  int numWords;
  vector<int> links;
  vector<int> words;
  Webpage() : numLinks(0), numWords(0) {}
  Webpage(string u) : url(u), numLinks(0), numWords(0) {}
};

struct Word {
  string text;
  string bestNext;
  vector<int> pages;
  StringIntMap after;
  Word() : text(""), bestNext("") {}
  Word(string txt) : text(txt), bestNext("") {}
};

// This shows how to use some of the starter code above
int main(void)
{
  cout << color_green << "Reading input..." << endl;
  const char *filename = "webpages.txt";
  istringstream webfile (read_webpages_fast (filename));

  //PageMap pages;
  StringIntMap pagesLookup;
  vector<Webpage> pages;
  StringIntMap wordsLoopup;
  vector<Word> words;

  // For now, this just counts the number of web pages in the input file...
  string s;
  int P = 0, W = 0;
  while (webfile >> s) {
    if (s == "PAGE") {
      webfile >> s; // s is the url of the webpage currently being processed
      pages.push_back(Webpage(s));
      pagesLookup[s] = P;
      P++;
    } else if(s == "LINK") {
      webfile >> s; //LINK URL
    } else {
      words.push_back(Word(s));
      wordsLoopup[s] = W;
      W++;
    }
  }
  
  P = 0;
  W = 0;
  s = "";
  webfile.clear();
  webfile.seekg(0);
 
  string p;
  while (webfile >> s) {
    if (s == "PAGE") {
      webfile >> s; // s is the url of the webpage currently being processed
      p = s;
      P++;
    } else if(s == "LINK") {
      webfile >> s; //LINK URL
      if(pagesLookup.find(s)) {
        pages[pagesLookup[p]].links.push_back(pagesLookup[s]);
        pages[pagesLookup[p]].numLinks++;
      }
    } else {
      pages[pagesLookup[p]].words.push_back(wordsLoopup[s]);
      pages[pagesLookup[p]].numWords++;
      W++;
    }
  }

  // Enter loop to ask for query
  process_keystrokes();

  return 0;
}
