#ifndef NOTELIST_H
#define NOTELIST_H

#include <iostream>

struct Note
{
    int pitch;
    double duration;
    int volume;
    Note(int p = 0, double d = 500, int v = 0x7f) : pitch(p), duration(d), volume(v) {}
};

// 音符链表的节点结构
struct Node {
    Note note;        // 数据域
    Node* next;      // 指针域
     Node(const Note& n, Node* nxt = nullptr) : note(n), next(nxt) {}
};

// 单向链表类
class NoteList {
private:
    Node* head;      // 链表头指针

public:
    NoteList();                      // 构造函数
    ~NoteList();                     // 析构函数

    void append(Note note);            // 在链表末尾添加节点
    void insert(Note note, int index); // 在指定位置插入节点
    void remove(int index);            // 删除指定位置的节点
    void display() const;              // 打印链表
    void clear();
    int size() const;                  // 获取链表长度
    Note findAt(int index);      //查找指定元素
    bool isEmpty() const;              // 判断链表是否为空
};

#endif // NOTELIST_H