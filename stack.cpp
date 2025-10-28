  #include <iostream>
  using namespace std;

  #include "stack.h"
  
  // 析构函数：释放所有节点
  Stack::~Stack() {
      while (!empty()) {
          pop();
      }
  }

  // 入栈：在栈顶添加新节点
  void Stack::push(int x) {
      ListNode* newNode = new ListNode(x);
      newNode->next = topNode;
      topNode = newNode;
  }

  // 出栈：移除栈顶节点
  void Stack::pop() {
      if (empty()) {
          cerr << "Error: Stack is empty!" << endl;
          return;
      }
      ListNode* temp = topNode;
      topNode = topNode->next;
      delete temp;  // 释放内存，避免泄漏
  }

  // 获取栈顶元素（不修改栈）
  int Stack::top() const {
      if (empty()) {
          cerr << "Error: Stack is empty!" << endl;
          return -1;  // 用-1表示错误（可优化为抛异常）
      }
      return topNode->val;
  }

  // 判断栈是否为空
  bool Stack::empty() const {
      return topNode == nullptr;
  }
  // 这是“main函数”，必须有！
int main() {
    // 这里写你的测试代码（比如打印“Hello World”）
    cout << "测试程序开始！" << endl;
    return 0; // 程序结束
}
