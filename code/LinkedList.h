#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>


// 单向链表的节点结构
struct Node {
    std::string data;        // 数据域
    Node* next;      // 指针域
    Node(std::string value) : data(value), next(nullptr) {}
};

// 单向链表类
class LinkedList {
private:
    Node* head;      // 链表头指针

public:
    LinkedList();                      // 构造函数
    ~LinkedList();                     // 析构函数

    void append(std::string value);            // 在链表末尾添加节点
    void insert(std::string value, int index); // 在指定位置插入节点
    void remove(int index);            // 删除指定位置的节点
    void display() const;              // 打印链表
    void clear();
    int size() const;                  // 获取链表长度
    std::string findAt(int index);      //查找指定元素
    bool isEmpty() const;              // 判断链表是否为空
};

#endif // LINKEDLIST_H
