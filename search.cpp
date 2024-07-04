// Do NOT add any other includes
#include "search.h"
#define ASCII_CHARS 128
using namespace std;

SearchEngine::SearchEngine(){
    // Implement your function here  
}

SearchEngine::~SearchEngine(){
    sentences.clear();
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence) {
    string to_ins = "";
    for (char& c: sentence) {
        if (c <= 'Z' && 'A' <= c)
            to_ins.push_back(c + 32);
        else
            to_ins.push_back(c);
    }
    sentences.emplace_back(Sentence{book_code, page, paragraph, sentence_no, to_ins});
    return;
}

void find_last_occurrence(const string& pattern, const int& len, int arr[ASCII_CHARS]) {
    for (int i = 0;i < ASCII_CHARS;i++)
        arr[i] = -1;
    for (int i = 0;i < len;i++)
        arr[pattern[i]] = i;
}

Node* SearchEngine::search(string pattern, int& n_matches) {
    Node* curr = NULL;
    Node* head = NULL;
    n_matches = 0;

    string ts = "";
    for (char& c: pattern) {
        if (c <= 'Z' && 'A' <= c)
            ts.push_back(c + 32);
        else
            ts.push_back(c);
    }

    int p = ts.length();
    if (p == 0)
        return NULL;
    int last_occurrence[ASCII_CHARS];
    find_last_occurrence(ts, p, last_occurrence);

    for (auto& sentence: sentences) {
        int n = sentence.text.length();
        int loc = 0;

        while (loc <= (n-p)) {
            int match = p-1;
            while (match >= 0 && ts[match] == sentence.text[loc + match])
                match--;
            
            if (match < 0) {
                n_matches++;
                Node* currtmp = new Node(sentence.bk, sentence.pg, sentence.para, sentence.sent, loc);
                currtmp->left = NULL;
                currtmp->right = NULL;
                if (curr) {
                    currtmp->left = curr;
                    curr->right = currtmp;
                }
                if (!currtmp->left)
                    head = currtmp;
                curr = currtmp;
                loc += (loc + p < n) ? (p - last_occurrence[sentence.text[loc + p]]): 1;
            } else {
                loc += max(1, match - last_occurrence[sentence.text[loc + match]]);
            }
        }
    }
    
    return head;
}

// int main() {
//     SearchEngine s;
//     string p = "An Autobiography, Pt. I, Ch. VIII";
//     string q = "I am 45 years old.";
//     s.insert_sentence(1, 0, 3, 6, p);
//     s.insert_sentence(1, 1, 1, 2, q);
//     int n = 0;
//     Node* res = s.search("graphy, Pt. I, Ch. V", n);
//     cout << res->book_code << " " << res->page << " " << res->paragraph << " " << res->sentence_no << " " << res->offset << "\n";
//     // res = res->right;
//     // cout << res->book_code << " " << res->page << " " << res->paragraph << " " << res->sentence_no << " " << res->offset << "\n";
//     cout << n << "\n";
//     return 0;
// }