#include <assert.h>
#include <sstream>
#include "qna_tool.h"

using namespace std;


size_t find(vector<string>& v, string& s) {
    for (size_t i = 0;i < v.size();i++) {
        if (v[i] == s) return i;
    }
    return -1;
}

QNA_tool::QNA_tool(){
    // Implement your function here 

    ifstream word_counter("unigram_freq.csv");
    string file_content;
    getline(word_counter, file_content);
    while (getline(word_counter, file_content)) {
        stringstream line_info(file_content);
        string to_insert;
        string ct;
        getline(line_info, to_insert, ',');
        getline(line_info, ct, ' ');
        word_freq.insert_word(to_insert, stoll(ct));
    }
    word_counter.close();
}

QNA_tool::~QNA_tool(){
    // Implement your function here 

}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here  
    word_counts.insert_sentence(book_code, page, paragraph, sentence_no, sentence);
    paras.add(book_code, page, paragraph, 0);
    // alt method
    // searcher.insert_sentence(book_code, page, paragraph, sentence_no, sentence);
    return;
}

Node* QNA_tool::get_top_k_para(string question, int k) {
    // Implement your function here
    for (int i = 0;i < paras.capacity;i++) {
        Para* itr = (*paras.storage)[i];
        while (itr) {
            itr->ct = 0;
            itr = itr->next;
        }
        
    }

    question.push_back('.');
    string word = "";
    vector<string> words;
    Table content;
    for (char& c: question) {
        if (Info::ascii[(int)c] < 0) {
            if (word.length() > 0) {
                // if (find(words, word) == -1) 
                words.emplace_back(word);
            }
            word = "";
        } else 
            word.push_back(Info::value[Info::ascii[(int)c]]);
    }

    vector<Para*> heap;

    // Para _1 = Para(1, 0, 7);
    // Para _2 = Para(1, 1, 0);
    // Para _3 = Para(1, 1, 4);
    // Para _4 = Para(1, 3, 0);
    // Para _5 = Para(1, 5, 0);
    // vector<Para> test = {_1, _2, _3, _4, _5};

    for (string& w: words) {
        long double score = ((double)word_counts.get_word_count(w) + 1)/((double)word_freq.get_word_count(w) + 1);
        // increment score of each word using distrib of w in trie
        Table* distrib = word_counts.getDistrib(w);
        // cout << distrib << "\n";
        // cout << w << " " << distrib->size << "\n";
        // for (Para p: test) {
        //     cout << "(" << p.book_code << ", " << p.page_no << ", " << p.para << ") " << distrib->get(p.book_code, p.page_no, p.para) << "\n";
        // }
        if (!distrib) continue;
        for (int i = 0;i < distrib->capacity;i++) {
            Para* results = (*distrib->storage)[i];
            while (results) {
                bool pushIt = !paras.get(results->book_code, results->page_no, results->para);
                Para* tmp = paras.add(results->book_code, results->page_no, results->para, results->ct * score);
                if (pushIt) heap.push_back(tmp);
                results = results->next;
            }
        }

        // alt method
        // string tS = " " + w + " ";
        // int ct = 0;
        // Node* results = searcher.search(tS, ct);
        // cout << w << " " << score << " " << ct << "\n";
        // while (results) {
        //     bool pushIt = !paras.get(results->book_code, results->page, results->paragraph);
        //     Para* tmp = paras.add(results->book_code, results->page, results->paragraph, score);
        //     // cout << tmp << " " << results->book_code << " " << results->page << " " << results->paragraph << " " << paras.get(results->book_code, results->page, results->paragraph) << " " << pushIt << "\n";
        //     if (pushIt) heap.push_back(tmp);
        //     results = results->right;
        // }
    }

    // pad with score 0 paragraphs
    int n = heap.size();
    if (n < k) {
        for (int i = 0;i < paras.capacity;i++) {
            Para* itr = (*paras.storage)[i];
            while (itr) {
                if (n >= k || n == paras.size) break;
                if (itr->ct == 0) {
                    n++;
                    heap.push_back(itr);
                }
                itr = itr->next;
            }
        }
    }

    if (n < k) k = paras.size;
    // use min heap for topK
    topK myRank = topK(heap, k);

    return myRank.getTopK();
}

void QNA_tool::query(string question, string filename){
    // Implement your function here  
    std::cout << "Q: " << question << std::endl;
    std::cout << "A: " << "Studying COL106 :)" << std::endl;
    return;
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph){

    cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;
    
    std::string filename = "mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ',')) {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
            }
            
            // Check if the element is a number or a string
            if (token[0] == '\'') {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph)
        ){
            res += sentence;
        }
    }

    inputFile.close();
    return res;
}

void QNA_tool::query_llm(string filename, Node* root, int k, string API_KEY, string question){

    // first write the k paragraphs into different files

    Node* traverse = root;
    int num_paragraph = 0;

    while(num_paragraph < k){
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "These are the excerpts from Mahatma Gandhi's books.\nOn the basis of this, ";
    outfile << question;
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();
 
    // you do not need to necessarily provide k paragraphs - can configure yourself

    // python3 <filename> API_KEY num_paragraphs query.txt
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}

// int main() {
//     QNA_tool ques;
//     ques.insert_sentence(0, 0, 0, 345345, "Mahatma was the Gandhi of India.");
//     ques.insert_sentence(0, 0, 1, 1, "Mahatma hated Pakistan.");
//     ques.insert_sentence(0, 1, 0, 25, "What did Mahatma think of India? Mahatma loved India.");
//     ques.insert_sentence(0, 1, 0, 26, "What did Mahatma think of India? Mahatma loved India.");
//     ques.insert_sentence(0, 1, 1, 1, "The who to the what to the were to the when");
//     Table* t = ques.word_counts.getDistrib("the");
//     cout << t->size << "\n";
//     for (int i = 0;i < t->capacity; i++) {
//         Para* obj = (*t->storage)[i];
//         while (obj) {
//             obj->info();
//             obj = obj->next;
//         }
//     }
//     cout << "inserts done\n";
//     Node* tmp = ques.get_top_k_para("hated", 3);
//     while (tmp) {
//         cout << tmp->book_code << " " << tmp->page << " " << tmp->paragraph << "\n";
//         tmp = tmp->right;
//     }

//     cout << "num2\n";

//     Node* tmp2 = ques.get_top_k_para("gandhi", 3);
//     while (tmp2) {
//         cout << tmp2->book_code << " " << tmp2->page << " " << tmp2->paragraph << "\n";
//         tmp2 = tmp2->right;
//     }

//     return 0;
// }