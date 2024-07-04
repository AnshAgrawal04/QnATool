#pragma once
#include <iostream>
#include <fstream>
#include "Node.h"
#include "dict.h"
#include "search.h"

using namespace std;

class topK {
public:
    vector<Para*> heap;
    int size = 0;
    int k = 0;

    topK(vector<Para*> v, int num) {
        k = num;
        for (int i = 0;i < k;i++) heap.push_back(v[i]);
        size = k;
        for (int i = size/2 - 1;i >= 0;i--) heapifyDown(i);
        for (size_t i = k;i < v.size();i++) push(v[i]);
    }

    void heapifyDown(int i) {
        int l = 2*i + 1;
        if (l >= size) return;
        int m = i;
        if (heap[l]->ct < heap[m]->ct) m = l;
        int r = 2*i + 2;
        if (r < size) {
            if (heap[r]->ct < heap[m]->ct) m = r;
        }
        if (i != m) {
            swap(heap[m], heap[i]);
            heapifyDown(m);
        }
    }

    void heapifyUp(int i) {
        if (i <= 0) return;
        int p = (i - 1) / 2;
        if (heap[i]->ct < heap[p]->ct) {
            swap(heap[i], heap[p]);
            heapifyUp(p);
        }
    }
    
    void push(Para* p) {
        if (size < k) {
            heap.push_back(p);
            size++;
            heapifyUp(size-1);
        } else if (heap[0]->ct < p->ct) {
            // cout << heap[0]->ct  << " " << p->ct << "\n";
            heap[0] = p;
            heapifyDown(0);
        }
    }
    Node* getTopK() {
        Node* curr = nullptr;
        for (int i = 0;i < k;i++) {
            Node* tmp = new Node(heap[0]->book_code, heap[0]->page_no, heap[0]->para, -1, 100000*heap[0]->ct);
            tmp->right = curr;
            if (curr) curr->left = tmp;
            curr = tmp;
            size--;
            heap[0] = heap[size];
            heapifyDown(0);
        }
        return curr;
    }
};

class QNA_tool {

private:

    // You are free to change the implementation of this function
    void query_llm(string filename, Node* root, int k, string API_KEY, string question);
    // filename is the python file which will call ChatGPT API
    // root is the head of the linked list of paragraphs
    // k is the number of paragraphs (or the number of nodes in linked list)
    // API_KEY is the API key for ChatGPT
    // question is the question asked by the user

    // You can add attributes/helper functions here

public:

    Dict word_counts;
    Dict word_freq;
    Table paras = Table(1024);

    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    
    QNA_tool(); // Constructor
    ~QNA_tool(); // Destructor

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);
    // This function is similar to the functions in dict and search 
    // The corpus will be provided to you via this function
    // It will be called for each sentence in the corpus

    Node* get_top_k_para(string question, int k);
    // This function takes in a question, preprocess it
    // And returns a list of paragraphs which contain the question
    // In each Node, you must set: book_code, page, paragraph (other parameters won't be checked)

    std::string get_paragraph(int book_code, int page, int paragraph);
    // Given the book_code, page number, and the paragraph number, returns the string paragraph.
    // Searches through the corpus.

    void query(string question, string filename);
    // This function takes in a question and a filename.
    // It should write the final answer to the specified filename.

    /* -----------------------------------------*/
    /* Please do not touch the code above this line */

    // You can add attributes/helper functions here
};