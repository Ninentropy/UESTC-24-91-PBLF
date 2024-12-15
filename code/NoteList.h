#ifndef NOTELIST_H
#define NOTELIST_H

#include <iostream>
#include <vector>
using namespace std;
struct Note
{
    int pitch;
    int volume;
    Note(int p = 0, int v = 0x7f) : pitch(p), volume(v) {}
};

// 音符链表的节点结构
class NoteNode {
public:
    Note note;        // 数据域
    NoteNode* next;      // 指针域
    NoteNode(const Note& n, NoteNode* nxt = nullptr) : note(n), next(nxt) {}
};

// 单向链表类
class NoteList {
    
private:
    NoteNode* head;      // 链表头指针
    double duration;
    // 辅助函数：用于递归复制链表节点
    NoteNode* copyList(NoteNode* node) {
        if (node == nullptr) return nullptr;
        NoteNode* newNode = new NoteNode(node->note);
        newNode->next = copyList(node->next);
        return newNode;
    }

public:
    NoteList();                      // 构造函数
    NoteList(const NoteList& other); // 拷贝构造函数
    NoteList& operator=(const NoteList& other); // 赋值操作符
    ~NoteList();                     // 析构函数

    void append(Note note);            // 在链表末尾添加节点
    void insert(Note note, int index); // 在指定位置插入节点
    void remove(int index);            // 删除指定位置的节点
    void display() const;              // 打印链表
    void clear();
    int size() const;                  // 获取链表长度
    Note findAt(int index);      //查找指定元素
    bool isEmpty() const;              // 判断链表是否为空
    double getDuration() const;        // 添加此行，获取持续时间
    void setDuration(double d);        // 添加此行，设置持续时间
    std::vector<int> transformNoteBuffer();
};

#endif // NOTELIST_H