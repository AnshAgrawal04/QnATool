// Do NOT add any other includes
#include "dict.h"

const int Info::ascii[128];
const int Info::value[ASCII_USABLE];

TrieNode::TrieNode(): distrib(nullptr) {
    for (int i = 0;i < ASCII_USABLE;i++)
        children[i] = nullptr;
}

TrieNode::~TrieNode() {
    // cout << "kek\n";
    if (distrib) delete distrib;
}

void TrieNode::distribAdd(int bk, int pg, int par) {
    if (!distrib) distrib = new Table();
    distrib->add(bk, pg, par);
    /*--
    ofstream fout("debug.txt", ios::app);
    fout << " " << distrib.size << "\n";
    fout.close();
    // --*/
    // if (bk == 1 && pg == 0 && par == 7) { 
    //     cout << distrib->get(1, 0, 7) << "\n";
    //     cout << distrib  << "\n";
    // }
}

TrieNode* TrieNode::insert(const string& word) {
    TrieNode* curr = this;
    for (char c: word) {
        int ind = Info::ascii[(int)c];
        if (!curr->children[ind]) curr->children[ind] = new TrieNode();
        curr = curr->children[ind];
    }
    curr->count = curr->count + 1;
    return curr;
}
TrieNode* TrieNode::insertChar(const char& c) {
    TrieNode* curr = this;
    int ind = Info::ascii[(int)c];
    if (!curr->children[ind]) curr->children[ind] = new TrieNode();
    curr = curr->children[ind];
    /*--
    ofstream fout("debug.txt", ios::app);
    fout << (char)Info::value[ind];
    fout.close();
    // --*/
    return curr;
}
long long int TrieNode::search(const string& word) {
    TrieNode* curr = this;
    for (char c: word) {
        int ind = Info::ascii[(int)c];
        if (!curr->children[ind]) return 0;
        curr = curr->children[ind];
    }
    return curr->count;
}
TrieNode* TrieNode::assignCount(const string& word, const long long int& cnt) {
    TrieNode* curr = this;
    for (char c: word) {
        int ind = Info::ascii[(int)c];
        if (!curr->children[ind]) curr->children[ind] = new TrieNode();
        curr = curr->children[ind];
    }
    curr->count = cnt;
    return curr;
}
Table* TrieNode::getDistrib(const string& word) {
    TrieNode* curr = this;
    for (char c: word) {
        int ind = Info::ascii[(int)c];
        if (!curr->children[ind]) return nullptr;
        curr = curr->children[ind];
    }
    return curr->distrib;
}

Dict::Dict() {
    trieRoot = new TrieNode();
}

void helperTrieDelete(TrieNode* root) {
    for (int i = 0;i < ASCII_USABLE;i++) {
        if (root->children[i]) {
            // cout << "deleting ";
            helperTrieDelete(root->children[i]);
        }
    }
    delete root;
}

Dict::~Dict() {
    helperTrieDelete(trieRoot);
}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    sentence.push_back('.');
    string word = "";
    TrieNode* curr = trieRoot;
    for (auto c: sentence) {
        if (Info::ascii[(int)c] < 0) {
            if (curr != trieRoot) {
                // if (book_code == 1 && page == 0 && paragraph == 7) cout << word << " " << "\n";
                curr->count = curr->count + 1;
                curr->distribAdd(book_code, page, paragraph);
            }
            word = "";
            curr = trieRoot;
        } else {
            curr = curr->insertChar(c);
            word.push_back(Info::value[Info::ascii[(int)c]]);
        }
    }
    return;
}

void Dict::insert_word(const string word, long long int cnt) {
    trieRoot->assignCount(word, cnt);
}

Table* Dict::getDistrib(const string& word) {
    return trieRoot->getDistrib(word);
}

long long int Dict::get_word_count(string word) {
    return trieRoot->search(word);
}

// int main() {
//     Dict d;
//     string s = "I am Bat-man.";
//     string p = "You?aren't going to win. I am Bat man.";
//     d.insert_sentence(0, 0, 0, 0, s);
//     d.insert_sentence(0, 0, 0, 0, p);
//     cout << d.get_word_count("you") << "\n";
//     return 0;
// }