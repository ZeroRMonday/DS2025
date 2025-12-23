#include <bits/stdc++.h>
using namespace std;

/*====================================================
    Bitmap 位图类（按实验给出的接口进行简化实现）
====================================================*/
class Bitmap {
private:
    vector<unsigned char> M;
    int _sz;   // 已使用 bit 数
public:
    Bitmap(int n = 8) {
        M.assign((n + 7) / 8, 0);
        _sz = 0;
    }

    void expand(int k) {
        if (k < 8 * M.size()) return;
        int newSize = (2 * k + 7) / 8;
        M.resize(newSize, 0);
    }

    void set(int k) {
        expand(k);
        M[k >> 3] |= (0x80 >> (k & 0x07));
        _sz = max(_sz, k + 1);
    }

    bool test(int k) {
        expand(k);
        return (M[k >> 3] & (0x80 >> (k & 0x07)));
    }

    string bits2string(int n) {
        string s;
        for (int i = 0; i < n; i++)
            s.push_back(test(i) ? '1' : '0');
        return s;
    }
};

/*====================================================
   Huffman Node（树节点）
====================================================*/
struct Node {
    char ch;               // 字母（非叶子节点为 0）
    int weight;            // 权值（频率）
    Node *l, *r;           // 左右孩子

    Node(char c, int w) : ch(c), weight(w), l(NULL), r(NULL) {}
};

/* 优先队列比较器：权值小的优先 */
struct cmp {
    bool operator()(Node* a, Node* b) {
        return a->weight > b->weight;
    }
};

/*====================================================
    统计 26 个字母频率（I HAVE A DREAM）
====================================================*/
vector<int> countFreq(const string &text) {
    vector<int> freq(26, 0);
    for (char c : text) {
        if (isalpha(c)) {
            c = tolower(c);
            freq[c - 'a']++;
        }
    }
    return freq;
}

/*====================================================
    构造 Huffman 树
====================================================*/
Node* buildHuffTree(const vector<int>& freq) {
    priority_queue<Node*, vector<Node*>, cmp> pq;
    for (int i = 0; i < 26; i++)
        if (freq[i] > 0)
            pq.push(new Node('a' + i, freq[i]));

    // 只有一个字母也要特殊处理
    if (pq.size() == 1) {
        Node* only = pq.top(); pq.pop();
        Node* root = new Node(0, only->weight);
        root->l = only;
        return root;
    }

    while (pq.size() > 1) {
        Node *a = pq.top(); pq.pop();
        Node *b = pq.top(); pq.pop();
        Node *p = new Node(0, a->weight + b->weight);
        p->l = a;
        p->r = b;
        pq.push(p);
    }
    return pq.top();
}

/*====================================================
    DFS 根据 Huffman 树生成编码
====================================================*/
unordered_map<char, string> HuffCode; // 字符 → 编码

void dfs(Node* u, string path) {
    if (!u) return;
    // 叶子节点：存编码
    if (!u->l && !u->r) {
        HuffCode[u->ch] = path;
        return;                    
    }
    dfs(u->l, path + "0");
    dfs(u->r, path + "1");
}

/*====================================================
    对单词进行编码（输出 Bitmap）
====================================================*/
Bitmap encodeWord(const string &w) {
    Bitmap bm;
    int bitIndex = 0;

    for (char c : w) {
        char ch = tolower(c);
        if (!isalpha(ch)) continue;

        string code = HuffCode[ch];
        for (char b : code) {
            if (b == '1') bm.set(bitIndex);
            bitIndex++;
        }
    }
    return bm;
}

/*====================================================
    主程序：构建 Huffman 树 + 编码 dream
====================================================*/
int main() {
    string text =
        "I have a dream that one day this nation will rise up and live out "
        "the true meaning of its creed: 'We hold these truths to be self-evident; "
        "that all men are created equal.'"; // 你也可以读 txt 文件

    // 1. 统计字母频率
    vector<int> freq = countFreq(text);

    // 2. 构建 Huffman 树
    Node* root = buildHuffTree(freq);

    // 3. 生成 26 字母编码
    dfs(root, "");

    cout << "===== Huffman 编码表 =====\n";
    for (auto &kv : HuffCode)
        cout << kv.first << " : " << kv.second << "\n";

    cout << "\n===== 单词 dream 的 Huffman 编码 =====\n";
    string word = "dream";
    Bitmap bm = encodeWord(word);

    // dream 的总编码长度
    int totalBits = 0;
    for (char c : word) totalBits += HuffCode[c].size();

    cout << "dream → " << bm.bits2string(totalBits) << "\n\n";

    // 你可以再选其他单词
    string w2 = "huffman";
    Bitmap bm2 = encodeWord(w2);
    int len2 = 0;
    for (char c : w2) len2 += HuffCode[c].size();
    cout << "huffman → " << bm2.bits2string(len2) << "\n";

    return 0;
}

