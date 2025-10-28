#ifndef MYSTL_VECTOR_H
#define MYSTL_VECTOR_H

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

namespace MySTL {

template<typename T>
class Vector {
private:
    T* _data;           // 数据数组
    int _size;          // 当前元素个数
    int _capacity;      // 当前容量
    
    // 扩容函数
    void expand() {
        if (_size < _capacity) return;
        
        _capacity = (_capacity == 0) ? 1 : _capacity * 2;
        T* oldData = _data;
        _data = new T[_capacity];
        
        for (int i = 0; i < _size; i++) {
            _data[i] = oldData[i];
        }
        
        delete[] oldData;
    }
    
public:
    // 构造函数
    Vector() : _data(nullptr), _size(0), _capacity(0) {}
    
    Vector(int capacity) : _size(0), _capacity(capacity) {
        _data = new T[capacity];
    }
    
    // 析构函数
    ~Vector() {
        if (_data) {
            delete[] _data;
        }
    }
    
    // 拷贝构造函数
    Vector(const Vector& other) : _size(other._size), _capacity(other._capacity) {
        _data = new T[_capacity];
        for (int i = 0; i < _size; i++) {
            _data[i] = other._data[i];
        }
    }
    
    // 赋值运算符
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            if (_data) delete[] _data;
            
            _size = other._size;
            _capacity = other._capacity;
            _data = new T[_capacity];
            
            for (int i = 0; i < _size; i++) {
                _data[i] = other._data[i];
            }
        }
        return *this;
    }
    
    // 获取大小
    int size() const {
        return _size;
    }
    
    // 判断是否为空
    bool empty() const {
        return _size == 0;
    }
    
    // 获取容量
    int capacity() const {
        return _capacity;
    }
    
    // 下标运算符
    T& operator[](int index) {
        return _data[index];
    }
    
    const T& operator[](int index) const {
        return _data[index];
    }
    
    // 插入元素
    void insert(const T& value) {
        expand();
        _data[_size++] = value;
    }
    
    void insert(int index, const T& value) {
        if (index < 0 || index > _size) return;
        
        expand();
        for (int i = _size; i > index; i--) {
            _data[i] = _data[i-1];
        }
        _data[index] = value;
        _size++;
    }
    
    // 删除元素
    void remove(int index) {
        if (index < 0 || index >= _size) return;
        
        for (int i = index; i < _size - 1; i++) {
            _data[i] = _data[i+1];
        }
        _size--;
    }
    
    // 查找元素
    int find(const T& value) const {
        for (int i = 0; i < _size; i++) {
            if (_data[i] == value) {
                return i;
            }
        }
        return -1;
    }
    
    // 置乱向量
    void unsort() {
        srand(time(0));
        for (int i = _size - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            std::swap(_data[i], _data[j]);
        }
    }
    
    // 去重
    int deduplicate() {
        int oldSize = _size;
        int i = 1;
        while (i < _size) {
            if (find(_data[i], 0, i) < 0) {
                i++;
            } else {
                remove(i);
            }
        }
        return oldSize - _size;
    }
    
    // 在指定范围内查找
    int find(const T& value, int low, int high) const {
        for (int i = low; i < high; i++) {
            if (_data[i] == value) {
                return i;
            }
        }
        return -1;
    }
    
    // 遍历函数
    template<typename VST>
    void traverse(VST& visit) {
        for (int i = 0; i < _size; i++) {
            visit(_data[i]);
        }
    }
};

} // namespace MySTL

#endif
