#include "NoteList.h"
#include <vector>
// 构造函数初始化链表为空
NoteList::NoteList() : head(nullptr),duration(500) {}
// 拷贝构造函数
NoteList::NoteList(const NoteList& other) {
    head = copyList(other.head);
    duration = other.duration;
}
// 赋值操作符
NoteList& NoteList::operator=(const NoteList& other) {
    if (this != &other) { // 防止自赋值
        clear();          // 清空现有数据
        head = copyList(other.head);
        duration = other.duration;
    }
    return *this;
}
// 析构函数释放所有节点内存
NoteList::~NoteList() {
    clear();
}

// 在链表末尾添加节点
void NoteList::append(Note note) {
    NoteNode* newNode = new NoteNode(note);
    if (head == nullptr) {
        head = newNode;
    } else {
        NoteNode* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// 在指定位置插入节点
void NoteList::insert(Note note, int index) {
    if (index < 0) return;

    NoteNode* newNode = new NoteNode(note);

    if (index == 0 || head == nullptr) {
        newNode->next = head;
        head = newNode;
        return;
    }

    NoteNode* current = head;
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

    NoteNode* toDelete;

    if (index == 0) {
        toDelete = head;
        head = head->next;
    } else {
        NoteNode* current = head;
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
    NoteNode* current = head;
    while (current != nullptr) {
        std::cout << "Pitch: " << current->note.pitch 
                  << ", Duration: " << this->duration 
                  << ", Volume: " << current->note.volume << std::endl;
        current = current->next;
    }
}

// 清空链表
void NoteList::clear() {
    while (head != nullptr) {
        NoteNode* temp = head;
        head = head->next;
        delete temp;
        // std::cout << "delete\n" <<std::endl;
        // display();
    }
}

// 获取链表长度
int NoteList::size() const {
    int count = 0;
    NoteNode* current = head;
    while (current != nullptr) {
        ++count;
        current = current->next;
    }
    return count;
}

// 查找指定元素
Note NoteList::findAt(int index) {
    if (index < 0 || head == nullptr) throw std::out_of_range("Index out of range");

    NoteNode* current = head;
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

double NoteList::getDuration() const {
    return duration;
}

void NoteList::setDuration(double d) {
    duration = d;
}

std::vector<int> NoteList::transformNoteBuffer(){
    NoteNode* current = head;
    std::vector<int> noteBuffer;
    while (current != nullptr) {
        Note note = current->note;
        if (note.pitch == 0)
        {
            noteBuffer.push_back(0);
            current = current->next;
            continue;
        }
        
        int noteInfo = (note.volume<<16) + (note.pitch<<8) + 0x90;
        noteBuffer.push_back(noteInfo);
        current = current->next;
    }
    return noteBuffer;
}