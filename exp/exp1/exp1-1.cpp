#include <iostream>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <stdexcept>  // 添加这一行
using namespace std;

// 简单的复数类
class Complex {
private:
    double real;
    double imag;

public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}
    
    double getReal() const { return real; }
    double getImag() const { return imag; }
    
    double getModulus() const {
        return sqrt(real * real + imag * imag);
    }
    
    void print() const {
        cout << real;
        if (imag >= 0)
            cout << " + " << imag << "i";
        else
            cout << " - " << -imag << "i";
    }
    
    // 重载相等运算符（实部和虚部都相同）
    bool operator==(const Complex& other) const {
        return real == other.real && imag == other.imag;
    }
    
    // 重载比较运算符（先比较模，模相同比较实部）
    bool operator<(const Complex& other) const {
        double mod1 = getModulus();
        double mod2 = other.getModulus();
        if (fabs(mod1 - mod2) < 1e-10) { // 模相等
            return real < other.real;     // 比较实部
        }
        return mod1 < mod2;
    }
};

// 简单的向量类
class SimpleVector {
private:
    Complex* data;
    int size;
    int capacity;

public:
    SimpleVector() : data(nullptr), size(0), capacity(10) {
        data = new Complex[capacity];
    }
    
    // 拷贝构造函数（重要！防止内存错误）
    SimpleVector(const SimpleVector& other) : size(other.size), capacity(other.capacity) {
        data = new Complex[capacity];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }
    
    // 赋值运算符
    SimpleVector& operator=(const SimpleVector& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            capacity = other.capacity;
            data = new Complex[capacity];
            for (int i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    
    ~SimpleVector() {
        if (data) delete[] data;
    }
    
    // 插入元素到末尾
    void insert(const Complex& c) {
        if (size >= capacity) {
            capacity *= 2;
            Complex* newData = new Complex[capacity];
            for (int i = 0; i < size; i++) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
        }
        data[size++] = c;
    }
    
    // 在指定位置插入
    void insert(int index, const Complex& c) {
        if (index < 0 || index > size) return;
        
        if (size >= capacity) {
            capacity *= 2;
            Complex* newData = new Complex[capacity];
            for (int i = 0; i < size; i++) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
        }
        
        for (int i = size; i > index; i--) {
            data[i] = data[i-1];
        }
        data[index] = c;
        size++;
    }
    
    // 删除指定位置元素
    void remove(int index) {
        if (index < 0 || index >= size) return;
        
        for (int i = index; i < size - 1; i++) {
            data[i] = data[i+1];
        }
        size--;
    }
    
    // 查找元素（实部和虚部都相同）
    int find(const Complex& c) const {
        for (int i = 0; i < size; i++) {
            if (data[i] == c) {
                return i;
            }
        }
        return -1;
    }
    
    // 唯一化（去重）
    void uniquify() {
        for (int i = 0; i < size; i++) {
            for (int j = i + 1; j < size; ) {
                if (data[i] == data[j]) {
                    remove(j);
                } else {
                    j++;
                }
            }
        }
    }
    
    int getSize() const { return size; }
    
    Complex& operator[](int index) {
        if (index < 0 || index >= size) {
            throw out_of_range("索引越界");
        }
        return data[index];
    }
    
    const Complex& operator[](int index) const {
        if (index < 0 || index >= size) {
            throw out_of_range("索引越界");
        }
        return data[index];
    }
    
    void print(const string& label = "") const {
        if (!label.empty()) {
            cout << label << ": ";
        }
        cout << "[ ";
        for (int i = 0; i < size; i++) {
            data[i].print();
            cout << " (模=" << data[i].getModulus() << ")";
            if (i < size - 1) cout << ", ";
        }
        cout << " ]" << endl;
    }
    
    // 冒泡排序
    void bubbleSort() {
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (data[j+1] < data[j]) {
                    swap(data[j], data[j+1]);
                }
            }
        }
    }
    
    // 归并排序
    void mergeSort() {
        if (size <= 1) return;
        
        Complex* temp = new Complex[size];
        mergeSortHelper(0, size - 1, temp);
        delete[] temp;
    }
    
private:
    void mergeSortHelper(int left, int right, Complex* temp) {
        if (left >= right) return;
        
        int mid = left + (right - left) / 2;
        mergeSortHelper(left, mid, temp);
        mergeSortHelper(mid + 1, right, temp);
        merge(left, mid, right, temp);
    }
    
    void merge(int left, int mid, int right, Complex* temp) {
        int i = left, j = mid + 1, k = 0;
        
        while (i <= mid && j <= right) {
            if (data[i] < data[j]) {
                temp[k++] = data[i++];
            } else {
                temp[k++] = data[j++];
            }
        }
        
        while (i <= mid) temp[k++] = data[i++];
        while (j <= right) temp[k++] = data[j++];
        
        for (i = 0; i < k; i++) {
            data[left + i] = temp[i];
        }
    }
};

// 生成随机复数
Complex generateRandomComplex() {
    double real = rand() % 20 - 10;
    double imag = rand() % 20 - 10;
    return Complex(real, imag);
}

// 打乱向量
void shuffleVector(SimpleVector& vec) {
    int n = vec.getSize();
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(vec[i], vec[j]);
    }
}

// 反转向量（用于创建逆序）
void reverseVector(SimpleVector& vec) {
    int n = vec.getSize();
    for (int i = 0; i < n / 2; i++) {
        swap(vec[i], vec[n - 1 - i]);
    }
}

// 区间查找：查找模在[m1, m2)范围内的元素
SimpleVector findInRange(const SimpleVector& vec, double m1, double m2) {
    SimpleVector result;
    for (int i = 0; i < vec.getSize(); i++) {
        double mod = vec[i].getModulus();
        if (mod >= m1 && mod < m2) {
            result.insert(vec[i]);
        }
    }
    return result;
}

// 测试排序效率（使用更大的数据量）
void testSortEfficiency(SimpleVector& vec, const string& condition) {
    cout << "\n=== " << condition << "情况下的排序效率 ===" << endl;
    
    // 使用更大的数据量（1000个元素）
    SimpleVector testVec;
    for (int i = 0; i < 1000; i++) {
        testVec.insert(generateRandomComplex());
    }
    
    // 根据条件设置顺序
    if (condition == "顺序") {
        testVec.mergeSort();
    } else if (condition == "逆序") {
        testVec.mergeSort();
        reverseVector(testVec);
    }
    // 乱序：保持原样
    
    SimpleVector vec1 = testVec; // 复制一份用于冒泡排序
    SimpleVector vec2 = testVec; // 复制一份用于归并排序
    
    // 测试冒泡排序
    clock_t start1 = clock();
    vec1.bubbleSort();
    clock_t end1 = clock();
    double time1 = (double)(end1 - start1) / CLOCKS_PER_SEC;
    
    // 测试归并排序
    clock_t start2 = clock();
    vec2.mergeSort();
    clock_t end2 = clock();
    double time2 = (double)(end2 - start2) / CLOCKS_PER_SEC;
    
    cout << "冒泡排序耗时: " << time1 << " 秒" << endl;
    cout << "归并排序耗时: " << time2 << " 秒" << endl;
    
    if (time2 > 0) {
        cout << "效率比较: 归并排序比冒泡排序快 " << time1/time2 << " 倍" << endl;
    } else {
        cout << "效率比较: 归并排序时间太短，无法比较" << endl;
    }
}

int main() {
    // 设置随机种子
    srand(time(0));
    
    cout << "=== 复数向量完整测试 ===" << endl;
    
    try {
        // 1. 生成随机向量（有重复项）
        SimpleVector vec;
        for (int i = 0; i < 10; i++) {
            vec.insert(generateRandomComplex());
            // 故意插入重复项
            if (i % 3 == 0 && i > 0) {
                vec.insert(vec[i-1]);
            }
        }
        vec.print("1. 原始向量（有重复项）");
        
        // 2. 测试置乱
        shuffleVector(vec);
        vec.print("2. 置乱后");
        
        // 3. 测试查找
        Complex target = vec[2]; // 用第三个元素作为查找目标
        int pos = vec.find(target);
        cout << "3. 查找复数: ";
        target.print();
        cout << " -> 位置: " << pos << endl;
        
        // 4. 测试插入和删除
        Complex newComplex(99, 99);
        vec.insert(3, newComplex);
        vec.print("4. 在位置3插入新元素后");
        
        vec.remove(5);
        vec.print("5. 删除位置5元素后");
        
        // 5. 测试唯一化
        vec.uniquify();
        vec.print("6. 唯一化后");
        
        // 6. 测试排序效率（使用更大的数据量）
        SimpleVector testVec;
        for (int i = 0; i < 1000; i++) {
            testVec.insert(generateRandomComplex());
        }
        
        // 顺序情况
        testVec.mergeSort();
        testSortEfficiency(testVec, "顺序");
        
        // 乱序情况
        shuffleVector(testVec);
        testSortEfficiency(testVec, "乱序");
        
        // 逆序情况
        testVec.mergeSort();
        reverseVector(testVec);
        testSortEfficiency(testVec, "逆序");
        
        // 7. 区间查找
        SimpleVector sortedVec;
        for (int i = 0; i < 20; i++) {
            sortedVec.insert(generateRandomComplex());
        }
        sortedVec.mergeSort();
        sortedVec.print("\n7. 排序后的向量（用于区间查找）");
        
        SimpleVector rangeResult = findInRange(sortedVec, 5.0, 10.0);
        rangeResult.print("模在[5.0, 10.0)范围内的复数");
        
        cout << "\n=== 所有测试完成! ===" << endl;
        
    } catch (const exception& e) {
        cout << "发生错误: " << e.what() << endl;
    }
    
    // 防止窗口闪退
    cout << "按任意键继续...";
    cin.get();
    
    return 0;
}
