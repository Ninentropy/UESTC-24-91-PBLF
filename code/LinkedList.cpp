#include "LinkedList.h"

// 构造函数
LinkedList::LinkedList() : head(nullptr) {}

// 析构函数
LinkedList::~LinkedList() {
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}

// 在链表末尾添加节点
void LinkedList::append(std::string value) {
    Node* newNode = new Node(value);
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
void LinkedList::insert(std::string value, int index) {
    if (index < 0) return;

    Node* newNode = new Node(value);
    if (index == 0) {
        newNode->next = head;
        head = newNode;
        return;
    }

    Node* current = head;
    for (int i = 0; i < index - 1 && current != nullptr; i++) {
        current = current->next;
    }

    if (current != nullptr) {
        newNode->next = current->next;
        current->next = newNode;
    }
}

// 删除指定位置的节点
void LinkedList::remove(int index) {
    if (index < 0 || head == nullptr) return;

    if (index == 0) {
        Node* temp = head;
        head = head->next;
        delete temp;
        return;
    }

    Node* current = head;
    for (int i = 0; i < index - 1 && current->next != nullptr; i++) {
        current = current->next;
    }

    if (current->next != nullptr) {
        Node* temp = current->next;
        current->next = temp->next;
        delete temp;
    }
}

// 打印链表
void LinkedList::display() const {
    Node* current = head;
    while (current != nullptr) {
        std::cout << current->data << " -> ";
        current = current->next;
    }
    std::cout << "nullptr" << std::endl;
}

// 获取链表长度
int LinkedList::size() const {
    int count = 0;
    Node* current = head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

// 判断链表是否为空
bool LinkedList::isEmpty() const {
    return head == nullptr;
}
void LinkedList::clear() {
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current; // 保存当前节点
        current = current->next; // 移动到下一个节点
        delete temp; // 删除当前节点
    }
    head = nullptr; // 将头指针置为 nullptr
}
std::string LinkedList::findAt(int index) {
    if (index < 0) {
        throw std::out_of_range("Index must be non-negative");
    }

    Node* current = head;
    int currentIndex = 0;

    while (current) {
        if (currentIndex == index) {
            return current->data;
        }
        current = current->next;
        currentIndex++;
    }

    throw std::out_of_range("Index out of range");
}