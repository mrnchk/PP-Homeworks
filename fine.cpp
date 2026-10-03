#include <iostream>
#include <mutex>

class IntSet {
private:
    struct Node {
        int value;
        Node* next;
        std::mutex mtx;

        Node(int value) : value(value), next(nullptr) {}
    };

    Node* head;

public:
    IntSet()
    {
        head = new Node(0);
    }

    bool contains(int value)
    {
        Node* prev = head;
        prev->mtx.lock();

        Node* curr = prev->next;

        if (curr != nullptr) {
            curr->mtx.lock();
        }

        while (curr != nullptr && curr->value < value) {
            prev->mtx.unlock();

            prev = curr;
            curr = curr->next;
            if (curr != nullptr) {
                curr->mtx.lock();
            }
        }

        if (curr != nullptr) {
            curr->mtx.unlock();
        }

        prev->mtx.unlock();

        return curr != nullptr && curr->value == value;
    }

    bool add(int value)
    {
        Node* prev = head;
        prev->mtx.lock();

        Node* curr = prev->next;

        if (curr != nullptr) {
            curr->mtx.lock();
        }

        while (curr != nullptr && curr->value < value) {
            prev->mtx.unlock();

            prev = curr;
            curr = curr->next;
            if (curr != nullptr) {
                curr->mtx.lock();
            }
        }

        if (curr != nullptr && curr->value == value) {
            curr->mtx.unlock();
            prev->mtx.unlock();
            return false;
        }

        Node* node = new Node(value);
        node->next = curr;
        prev->next = node;

        if (curr != nullptr) {
            curr->mtx.unlock();
        }

        prev->mtx.unlock();

        return true;
    }

    bool remove(int value)
    {
        Node* prev = head;
        prev->mtx.lock();

        Node* curr = prev->next;

        if (curr != nullptr) {
            curr->mtx.lock();
        }

        while (curr != nullptr && curr->value < value) {
            prev->mtx.unlock();

            prev = curr;
            curr = curr->next;
            if (curr != nullptr) {
                curr->mtx.lock();
            }
        }

        if (curr == nullptr || curr->value != value) {
            if (curr != nullptr) {
                curr->mtx.unlock();
            }

            prev->mtx.unlock();
            return false;
        }

        prev->next = curr->next;

        curr->mtx.unlock();
        delete curr;
        prev->mtx.unlock();

        return true;
    }
};
