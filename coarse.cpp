#include <iostream>
#include <mutex>

class IntSet {
private:
    struct Node {
        int value;
        Node* next;

        Node(int value) : value(value), next(nullptr) {}
    };

    Node* head;
    std::mutex mtx;

public:
    IntSet() : head(nullptr) {}

    bool contains(int value)
    {
        std::lock_guard<std::mutex> lock(mtx);

        Node* curr = head;

        while (curr != nullptr && curr->value < value)
            curr = curr->next;

        return curr != nullptr && curr->value == value;
    }

    bool add(int value)
    {
        std::lock_guard<std::mutex> lock(mtx);

        Node* prev = nullptr;
        Node* curr = head;

        while (curr != nullptr && curr->value < value) {
            prev = curr;
            curr = curr->next;
        }

        if (curr != nullptr && curr->value == value)
            return false;

        Node* node = new Node(value);
        node->next = curr;

        if (prev == nullptr)
            head = node;
        else
            prev->next = node;

        return true;
    }

    bool remove(int value)
    {
        std::lock_guard<std::mutex> lock(mtx);

        Node* prev = nullptr;
        Node* curr = head;

        while (curr != nullptr && curr->value < value) {
            prev = curr;
            curr = curr->next;
        }

        if (curr == nullptr || curr->value != value)
            return false;

        if (prev == nullptr)
            head = curr->next;
        else
            prev->next = curr->next;

        delete curr;

        return true;
    }
};
