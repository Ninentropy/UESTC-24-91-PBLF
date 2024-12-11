#include "NoteList.h"

// 构造函数初始化链表为空
NoteList::NoteList() : head(nullptr) {}

// 析构函数释放所有节点内存
NoteList::~NoteList() {
    clear();
}

// 在链表末尾添加节点
void NoteList::append(Note note) {
    Node* newNode = new Node(note);
    if (head == nullptr) {
        head = newNode;
    } else {
        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// 在指定位置插入节点
void NoteList::insert(Note note, int index) {
    if (index < 0) return;

    Node* newNode = new Node(note);

    if (index == 0 || head == nullptr) {
        newNode->next = head;
        head = newNode;
        return;
    }

    Node* current = head;
    for (int i = 0; current != nullptr && i < index - 1; ++i) {
        current = current->next;
    }

    if (current != nullptr) {
        newNode->next = current->next;
        current->next = newNode;
    } else {
        delete newNode; // 如果插入位置超出链表长度，则删除新节点
    }
}

// 删除指定位置的节点
void NoteList::remove(int index) {
    if (index < 0 || head == nullptr) return;

    Node* toDelete;

    if (index == 0) {
        toDelete = head;
        head = head->next;
    } else {
        Node* current = head;
        for (int i = 0; current != nullptr && i < index - 1; ++i) {
            current = current->next;
        }

        if (current == nullptr || current->next == nullptr) return;

        toDelete = current->next;
        current->next = current->next->next;
    }

    delete toDelete;
}

// 打印链表
void NoteList::display() const {
    Node* current = head;
    while (current != nullptr) {
        std::cout << "Pitch: " << current->note.pitch 
                  << ", Duration: " << current->note.duration 
                  << ", Volume: " << current->note.volume << std::endl;
        current = current->next;
    }
}

// 清空链表
void NoteList::clear() {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

// 获取链表长度
int NoteList::size() const {
    int count = 0;
    Node* current = head;
    while (current != nullptr) {
        ++count;
        current = current->next;
    }
    return count;
}

// 查找指定元素
Note NoteList::findAt(int index) {
    if (index < 0 || head == nullptr) throw std::out_of_range("Index out of range");

    Node* current = head;
    for (int i = 0; current != nullptr && i < index; ++i) {
        current = current->next;
    }

    if (current == nullptr) throw std::out_of_range("Index out of range");
    
    return current->note;
}

// 判断链表是否为空
bool NoteList::isEmpty() const {
    return head == nullptr;
}