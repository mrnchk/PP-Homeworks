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

    bool validate(Node* prev, Node* curr)
    {
        Node* node = head;

        while (node != nullptr && node != prev) {
            node = node->next;
        }

        return node == prev && prev->next == curr;
    }

public:
    IntSet()
    {
        head = new Node(0);
    }

   bool contains(int value)
   {
        Node* curr = head->next;

        while (curr != nullptr && curr->value < value) {
            curr = curr->next;
        }

        return curr != nullptr && curr->value == value;
    }

    bool add(int value)
    {
        while (true) {
            Node* prev = head;
            Node* curr = prev->next;

            while (curr != nullptr && curr->value < value) {
                prev = curr;
                curr = curr->next;
            }

            prev->mtx.lock();

            if (curr != nullptr) {
                curr->mtx.lock();
            }

            if (!validate(prev, curr)) {
                if (curr != nullptr) {
                    curr->mtx.unlock();
                }

                prev->mtx.unlock();
                continue;
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
    }

    bool remove(int value)
    {
        while (true) {
            Node* prev = head;
            Node* curr = prev->next;

            while (curr != nullptr && curr->value < value) {
                prev = curr;
                curr = curr->next;
            }

            prev->mtx.lock();

            if (curr != nullptr) {
                curr->mtx.lock();
            }

            if (!validate(prev, curr)) {
                if (curr != nullptr) {
                    curr->mtx.unlock();
                }

                prev->mtx.unlock();
                continue;
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
            prev->mtx.unlock();

            return true;
        }
    }
};
