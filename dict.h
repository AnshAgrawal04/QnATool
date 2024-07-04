// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include <fstream>
// #define ASCII_USABLE 54
#define ASCII_USABLE 87
#define MAX_LOAD 0.7
using namespace std;

class Info {
public:
    // static constexpr int ascii[128] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, -1, -1, -1, 4, 5, -1, -1, -1, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, -1, -1, 17, 18, 19, -1, -1, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, -1, 46, -1, 47, 48, 49, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 50, 51, 52, 53, -1}; 
    // static constexpr int value[ASCII_USABLE] = {35, 36, 37, 38, 42, 43, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 60, 61, 62, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 92, 94, 95, 96, 123, 124, 125, 126};
    static constexpr int ascii[128] = {54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, -1, -1, -1, 0, 1, 2, 3, -1, -1, -1, 4, 5, -1, -1, -1, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, -1, -1, 17, 18, 19, -1, -1, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, -1, 46, -1, 47, 48, 49, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 50, 51, 52, 53, 86}; 
    static constexpr int value[ASCII_USABLE] = {35, 36, 37, 38, 42, 43, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 60, 61, 62, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 92, 94, 95, 96, 123, 124, 125, 126, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 127};
};

class Para {
public:
    int book_code;
    int page_no;
    int para;
    long double ct = 0;
    Para* next = nullptr;
    Para(int bk, int pg, int par): book_code(bk), page_no(pg), para(par) {};
    void info() {
        cout << book_code << " " << page_no << " " << para << " " << ct << "\n"; 
    }
};

class Table {
public:
    int size = 0;
    int capacity = 1024;
    vector<Para*>* storage;

    Table(int c = 1024) {
        capacity = c;
        storage = new vector<Para*>(c, nullptr);
    }

    ~Table() {
        for (int i = 0;i < capacity;i++) {
            Para* obj = (*storage)[i];
            while (obj) {
                // cout << obj->book_code << "\n";
                Para* tmp = obj->next;
                delete obj;
                obj = tmp;
            }
            delete obj;
        }
        delete storage;
    }

    long long unsigned int cantor(long long unsigned int a, long long unsigned int b) {
        return ((a + b + 1) * (a + b)) / 2;
    }

    long long unsigned int hash(int i, int j, int k) {
        return cantor(j, cantor(i, k)) % capacity;
    }

    Para* add(int bk, int pg, int par, long double toAdd = 1) {
        if (size > MAX_LOAD * capacity) {
            /*--
            ofstream fout("debug.txt", ios::app);
            fout << "resize\n";
            fout.close();
            // --*/
            bool flag = false;
            capacity = 2 * capacity;
            vector<Para*>* newStorage = new vector<Para*>(capacity, nullptr);
            for (int i = 0;i < capacity/2;i++) {
                Para* itr = (*storage)[i];
                while (itr) {
                    Para* nxt = itr->next;
                    long long unsigned int newKey = hash(itr->book_code, itr->page_no, itr->para);
                    itr->next = (*newStorage)[newKey];
                    (*newStorage)[newKey] = itr;
                    itr = nxt;
                }
            }
            delete storage;
            storage = newStorage;
        }

        long long unsigned int hashKey = hash(bk, pg, par);
        Para* obj = (*storage)[hashKey];
        // cout << bk << " " << pg  << " " << par << " " << hashKey << " ";
        while (obj) {
            if ((obj->book_code == bk) && (obj->page_no == pg) && (obj->para == par)) {
                obj->ct = obj->ct + toAdd;
                return obj;
            }
            obj = obj->next;
        }
        size++;
        Para* newPara = new Para(bk, pg, par);
        newPara->ct = toAdd;
        newPara->next = (*storage)[hashKey];
        (*storage)[hashKey] = newPara;
        return newPara;
    }

    long double get(int bk, int pg, int par) {
        long long unsigned int hashKey = hash(bk, pg, par);
        Para* obj = (*storage)[hashKey];
        while (obj) {
            if ((obj->book_code == bk) && (obj->page_no == pg) && (obj->para == par)) {
                return obj->ct;
            }
            obj = obj->next;
        }
        return 0;
    }
};

class TrieNode {
public:
    TrieNode* children[ASCII_USABLE];
    Table* distrib;
    long long int count = 0;

    TrieNode();
    ~TrieNode();

    void distribAdd(int bk, int pg, int par);
    TrieNode* insert(const string& word);
    TrieNode* insertChar(const char& c);
    long long int search(const string& word);
    TrieNode* assignCount(const string& word, const long long int& cnt) ;
    Table* getDistrib(const string& word);
};

class Dict {
private:
    // You can add attributes/helper functions here
    // string separators = " .,-:!\"\'()?—[]“”‘’˙;@";
    TrieNode* trieRoot;

public: 
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict();

    ~Dict();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    long long int get_word_count(string word);

    /* -----------------------------------------*/
    void insert_word(const string word, const long long int cnt);
    Table* getDistrib(const string& word);
};
