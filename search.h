// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include "Node.h"
using namespace std;

class Sentence {
public:
    int bk;
    int pg;
    int para;
    int sent;
    string text;
    Sentence(int book_code, int page, int paragraph, int sentence, string txt) {
        bk = book_code;
        pg = page;
        para = paragraph;
        sent = sentence;
        text =  txt;
    }
};

class SearchEngine {
private:
    // You can add attributes/helper functions here
    vector<Sentence> sentences;

public: 
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    SearchEngine();

    ~SearchEngine();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    Node* search(string pattern, int& n_matches);

    /* -----------------------------------------*/
};