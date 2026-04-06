#include <iostream>
#include <algorithm>
#include <queue>
#include <utility>
#include <fstream>
#include <string>
#include <cctype>
#include <chrono>
#include <vector>
#include <cmath>

using namespace std;

template <typename T>
struct Node {
    T data;
    int height;
    Node* left;
    Node* right;
    Node(const T& value) : data(value), height(1), left(nullptr), right(nullptr) {}
};

struct BalanceCheck {
    int height;
    int max_balance_factor;
    bool is_balanced;
};

// AVL Tree Template Class
template <typename T>
class AVLTree {
private:
    Node<T>* root;

    int height(Node<T>* N) const {
        return (N == nullptr) ? 0 : N->height;
    }

    void updateHeight(Node<T>* N) {
        if (N != nullptr) {
            N->height = 1 + max(height(N->left), height(N->right));
        }
    }

    int getBalance(Node<T>* N) const {
        return (N == nullptr) ? 0 : height(N->left) - height(N->right);
    }

    Node<T>* minValueNode(Node<T>* node) {
        Node<T>* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }
    // Rotations
    Node<T>* rotateRight(Node<T>* y) {
        Node<T>* x = y->left;
        Node<T>* T2 = x->right;
        x->right = y;
        y->left = T2;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    Node<T>* rotateLeft(Node<T>* x) {
        Node<T>* y = x->right;
        Node<T>* T2 = y->left;
        y->left = x;
        x->right = T2;
        updateHeight(x);
        updateHeight(y);
        return y;
    }
    // Recursive Insert
    Node<T>* insertRec(Node<T>* node, const T& data) {
        if (node == nullptr)
            return new Node<T>(data);
        if (data < node->data)
            node->left = insertRec(node->left, data);
        else if (data > node->data)
            node->right = insertRec(node->right, data);
        else return node;

        updateHeight(node);
        int balance = getBalance(node);

        // LL Case
        if (balance > 1 && data < node->left->data) return rotateRight(node);
        // RR Case
        if (balance < -1 && data > node->right->data) return rotateLeft(node);

        // LR Case
        if (balance > 1 && data > node->left->data) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        // RL Case
        if (balance < -1 && data < node->right->data) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }
    // Recursive Remove
    Node<T>* removeRec(Node<T>* rootNode, const T& data) {
        if (rootNode == nullptr) return rootNode;

        if (data < rootNode->data)
            rootNode->left = removeRec(rootNode->left, data);
        else if (data > rootNode->data)
            rootNode->right = removeRec(rootNode->right, data);
        else {
            // 0 or 1 child
            if (rootNode->left == nullptr || rootNode->right == nullptr) {
                Node<T>* temp = rootNode->left ? rootNode->left : rootNode->right;
                delete rootNode;
                return temp;
            }
            // 2 children (inorder replace)
            else {
                Node<T>* temp = minValueNode(rootNode->right);
                rootNode->data = temp->data;
                rootNode->right = removeRec(rootNode->right, temp->data);
            }
        }

        if (rootNode == nullptr)
            return rootNode;
        updateHeight(rootNode);
        int balance = getBalance(rootNode);

        // LL or LR
        if (balance > 1) {
            if (getBalance(rootNode->left) < 0) {
                rootNode->left = rotateLeft(rootNode->left);
            }
            return rotateRight(rootNode);
        }

        // RR or RL
        if (balance < -1) {
            if (getBalance(rootNode->right) > 0) {
                rootNode->right = rotateRight(rootNode->right);
            }
            return rotateLeft(rootNode);
        }

        return rootNode;
    }

    BalanceCheck check_balance_rec(Node<T>* node) const {
        if (node == nullptr) {
            return { 0, 0, true };
        }
        
        BalanceCheck left = check_balance_rec(node->left);
        BalanceCheck right = check_balance_rec(node->right);
    
        int current_height = 1 + std::max(left.height, right.height);
        int balance = left.height - right.height;
        int abs_balance = std::abs(balance);

        bool current_is_balanced = left.is_balanced && right.is_balanced && (abs_balance <= 1);
        
        int max_bal = std::max({abs_balance, left.max_balance_factor, right.max_balance_factor });

        return {current_height, max_bal, current_is_balanced};

    }

    void deleteTree(Node<T>* node) {
        if (node != nullptr) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

public:
    // Constructor + Destructor
    AVLTree() : root(nullptr) {}
    ~AVLTree() {
        deleteTree(root);
    }

    // Methods
    void insert(const T& data) {
        root = insertRec(root, data);
    }
    void remove(const T& data) {
        root = removeRec(root, data);
    }
    bool contains(const T& data) const {
        Node<T>* current = root;
        while (current != nullptr) {
            if (data == current->data) return true;
            else if (data < current->data) current = current->left;
            else current = current->right;
        }
        return false;
    }

    void displayBalance() const {
        if (root == nullptr) {
            std::cout << "Tree is empty\n";
            return;
        }

        BalanceCheck result = check_balance_rec(root);

        std::cout << "Balance Summary:\n";
        std::cout << "Max Height:" << result.height << "\n";
        std::cout << "Max Balance Factor: " << result.max_balance_factor << "\n";

        if (result.is_balanced) {
            std::cout << "Balance Status: Balanced!\n";
        }
        else {
            std::cout << "Balance Status: Out of Balance!\n";
        }
        std::cout << "\n";

    }
};

void load_dictionary_avl(AVLTree<std::string>& dict, const std::string& filename, int& word_count) {
    std::ifstream file(filename);
    std::string word;
    word_count = 0;

    if (!file.is_open()) {
        std::cerr << "Error: Could not Open Dictionary File: " << filename << "\n";
        return;
    }

    while (file >> word) {
        std::string clean_word;
        for (char c : word) {
            if (std::isalpha(static_cast<unsigned char>(c))) {
                clean_word += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            }
        }

        if (!clean_word.empty()) {
            dict.insert(clean_word);
            word_count++;
        }
    }
    file.close();
}

void timed_spell_check(const AVLTree<std::string>& dict, const std::string& filename, int& words_checked) {
    std::ifstream file(filename);
    std::string word;
    words_checked = 0;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open dictionary file: " << filename << "\n";
        return;
    }
    while (file >> word) {
        std::string clean_word;
        for (char c : word) {
            int c_val = static_cast<unsigned char>(c);
            if (std::isalpha(c_val)) {
                clean_word += static_cast<char>(std::tolower(c_val));
            }
        }

        if (!clean_word.empty()) {
            dict.contains(clean_word);
            words_checked++;
        }
    }
    file.close();
}

int main() {
    AVLTree<std::string> avl_dictionary;
    int total_words = 0;
    std::string dictionary_path = "dictionary.txt";
    std::string user_word;

    std::cout << "Loading + Balancing Dictionary..." << endl;
    
    // Timing for loading dictionary into tree
    auto start_load = std::chrono::high_resolution_clock::now();
    load_dictionary_avl(avl_dictionary, dictionary_path, total_words);
    auto end_load = std::chrono::high_resolution_clock::now();

    auto duration_load = std::chrono::duration_cast<std::chrono::duration<double>>(end_load - start_load);

    std::cout << "Dictionary Loaded + Balanced. Words load: " << total_words << endl;
    std::cout << "Time taken: " << duration_load.count() << " seconds" << endl;

    // Displays balance info on tree
    avl_dictionary.displayBalance();

    // Timing of spellchecking tree
    int check_count = 0;

    std::cout << "Full file spellcheck on tree\n";
    auto start_check = std::chrono::high_resolution_clock::now();
    timed_spell_check(avl_dictionary, dictionary_path, check_count);
    auto end_check = std::chrono::high_resolution_clock::now();
    auto duration_check = std::chrono::duration_cast<std::chrono::milliseconds>(end_check - start_check);

    std::cout << "Words checked in file: " << check_count << "\n";
    std::cout << "Total spell check time: " << duration_check.count() << " ms\n";
    std::cout << "Spellcheck loop below \n\n";

    std::cout << "Enter a word to spell check (type 'quit' to exit):" << endl;

    while (std::cin >> user_word && user_word != "quit") {
        std::string clean_word;
        for (char c : user_word) {
            if (std::isalpha(c)) {
                clean_word += std::tolower(c);
            }
        }
        if (!clean_word.empty()) {
            auto start_search = std::chrono::high_resolution_clock::now();
            bool found = avl_dictionary.contains(clean_word);
            auto end_search = std::chrono::high_resolution_clock::now();
            auto duration_search = std::chrono::duration_cast<std::chrono::milliseconds>(end_search - start_search);

            if (found) {
                std::cout << "Word: " << user_word << " is spelled correctly. ";
            }
            else {
                std::cout << "Word: " << user_word << " is NOT spelled correctly. ";
            }
            std::cout << "Search Time: " << duration_search.count() << " ms" << endl;
        }
        std::cout << "\nEnter a word to spell check (type 'quit' to exit):" << endl;
    }

    std::cout << "Exititing spell checker." << std::endl;

    return 0;
}
