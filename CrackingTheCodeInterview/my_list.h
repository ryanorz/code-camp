#ifndef my_list_h
#define my_list_h

struct Node {
    int value;
    Node* next;

    Node(int v)
        : value(v)
        , next(nullptr)
    {
    }
};

#include <iostream>
#include <sys/types.h>
#include <vector>
using std::vector;

Node* createList(vector<int> array)
{
    Node* head = nullptr;
    Node* tail = nullptr;
    for (auto v : array) {
        Node* node = new Node(v);
        if (head == nullptr) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = tail->next;
        }
    }
    return head;
}

void destroyList(Node* head)
{
    if (head == nullptr) {
        return;
    }
    while (head) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

bool listEquals(Node* head, vector<int> array)
{
    size_t index = 0;
    while (head && index < array.size()) {
        if (head->value != array[index]) {
            return false;
        }
        index++;
        head = head->next;
    }
    return (array.size() - index == 0) && (head == nullptr);
}

void listDisplay(Node* head)
{
    std::cout << "[ ";
    while (head) {
        std::cout << head->value << " ";
        head = head->next;
    }
    std::cout << "]\n";
}

#endif