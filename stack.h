  #ifndef STACK_H
  #define STACK_H
  #include "list.h"

  class Stack {
  private:
      ListNode* topNode;  // 栈顶节点指针
  public:
      Stack() : topNode(nullptr) {}  // 构造函数（空栈）
      ~Stack();                       // 析构函数（释放内存）
      void push(int x);               // 入栈
      void pop();                     // 出栈
      int top() const;                // 获取栈顶元素
      bool empty() const;             // 判断栈是否为空
  };

  #endif // STACK_H
