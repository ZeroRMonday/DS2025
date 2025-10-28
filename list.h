  #ifndef LISTNODE_H
  #define LISTNODE_H

  struct ListNode {
      int val;             // 节点值
      ListNode* next;      // 下一个节点指针
      ListNode(int x) : val(x), next(nullptr) {}  // 构造函数
  };

  #endif // LISTNODE_H
