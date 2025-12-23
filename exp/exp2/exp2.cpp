#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <unordered_map>
#include <string>
#include <bitset>
#include <cstring>  // 引入 memset 函数

using namespace std;

// 二叉树数据结构
template <typename T>
class BinTree {
public:
    struct Node {
        T data;
        Node* left;
        Node* right;
        Node(T val) : data(val), left(nullptr), right(nullptr) {}
    };

    BinTree() : root(nullptr) {}

    void insert(T val) {
        root = insert(root, val);
    }

    Node* getRoot() {
        return root;
    }

private:
    Node* root;

    Node* insert(Node* node, T val) {
        if (node == nullptr)
            return new Node(val);

        if (val < node->data)
            node->left = insert(node->left, val);
        else
            node->right = insert(node->right, val);
        return node;
    }
};

// 哈夫曼编码树节点
struct HuffmanNode {
    char data;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char d, int f) : data(d), freq(f), left(nullptr), right(nullptr) {}

    // 重载运算符，优先队列中小的频率优先
    bool operator>(const HuffmanNode& other) const {
        return freq > other.freq;
    }
};

// 哈夫曼树
class HuffmanTree {
public:
    HuffmanTree(const string& str) {
        buildTree(str);
    }

    void buildTree(const string& str) {
        unordered_map<char, int> freq;
        for (char c : str) {
            if (isalpha(c)) {
                freq[tolower(c)]++;
            }
        }

        // 使用优先队列，构建哈夫曼树
        priority_queue<HuffmanNode*, vector<HuffmanNode*>, greater<HuffmanNode*>> pq;
        for (auto& pair : freq) {
            pq.push(new HuffmanNode(pair.first, pair.second));
        }

        while (pq.size() > 1) {
            HuffmanNode* left = pq.top();
            pq.pop();
            HuffmanNode* right = pq.top();
            pq.pop();

            HuffmanNode* parent = new HuffmanNode('\0', left->freq + right->freq);
            parent->left = left;
            parent->right = right;

            pq.push(parent);
        }

        root = pq.top();
    }

    // 递归生成哈夫曼编码
    void generateHuffmanCode(HuffmanNode* node, string str, unordered_map<char, string>& huffmanCode) {
        if (node == nullptr) {
            return;
        }

        if (node->data != '\0') {
            huffmanCode[node->data] = str;
        }

        generateHuffmanCode(node->left, str + "0", huffmanCode);
        generateHuffmanCode(node->right, str + "1", huffmanCode);
    }

    // 获取哈夫曼树根节点
    HuffmanNode* getRoot() {
        return root;
    }

private:
    HuffmanNode* root;
};

// 位图类
class Bitmap {
private:
    unsigned char* M;
    int N, _sz;

public:
    Bitmap(int n = 8) {
        init(n);
    }

    ~Bitmap() {
        delete[] M;
        M = nullptr;
    }

    void init(int n) {
        if (n <= 0) n = 8;  // 设置最小尺寸
        M = new unsigned char[(n + 7) / 8];  // 计算需要多少字节
        memset(M, 0, (n + 7) / 8);  // 用 0 填充位图
        N = (n + 7) / 8;
        _sz = 0;
    }

    void set(int k) {
        if (k < 0) return;
        int byteIndex = k / 8;
        int bitIndex = k % 8;
        M[byteIndex] |= (1 << (7 - bitIndex));
        _sz++;
    }

    void clear(int k) {
        if (k < 0) return;
        int byteIndex = k / 8;
        int bitIndex = k % 8;
        M[byteIndex] &= ~(1 << (7 - bitIndex));
        _sz--;
    }

    bool test(int k) {
        if (k < 0) return false;
        int byteIndex = k / 8;
        int bitIndex = k % 8;
        return M[byteIndex] & (1 << (7 - bitIndex));
    }

    void printBitmap(int n) {
        for (int i = 0; i < n; i++) {
            cout << (test(i) ? "1" : "0");
        }
        cout << endl;
    }
};

// 在主程序中，注释或删除位图相关代码，保留哈夫曼编码部分
int main() {
    string text = "I have a dream that one day this nation will rise up, live out the true meaning of its creed: 'We hold these truths to be self-evident, that all men are created equal.'";
    
    // 创建哈夫曼树
    HuffmanTree huffTree(text);
    
    // 生成哈夫曼编码
    unordered_map<char, string> huffmanCode;
    huffTree.generateHuffmanCode(huffTree.getRoot(), "", huffmanCode);

    cout << "Huffman Codes: " << endl;
    for (auto& pair : huffmanCode) {
        cout << pair.first << ": " << pair.second << endl;
    }

    // 如果不需要位图输出，可以注释掉
    // Bitmap bitmap(32);
    // bitmap.set(0);
    // bitmap.set(2);
    // bitmap.set(4);
    // bitmap.printBitmap(32);

    return 0;
}

