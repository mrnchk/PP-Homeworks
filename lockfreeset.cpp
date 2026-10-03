#include <atomic>
#include <cstdint>
#include <optional>
#include <utility>

template<typename T>
class LockFreeSet {
private:
    struct Node {
        std::optional<T> value;
        std::atomic<std::uintptr_t> next;
        Node* allocatedNext;

        Node()
            : value(std::nullopt),
              next(0),
              allocatedNext(nullptr) {}

        Node(T v, Node* n)
            : value(std::move(v)),
              next(reinterpret_cast<std::uintptr_t>(n)),
              allocatedNext(nullptr) {}
    };

    static constexpr std::uintptr_t MARK = 1;

    Node* head;
    Node* tail;

    std::atomic<Node*> allocated{nullptr};

    void registerNode(Node* node) {
        Node* old = allocated.load();
        while (!allocated.compare_exchange_weak(old, node)) {
            node->allocatedNext = old;
        }
    }

    std::pair<Node*, Node*> find(const T& value) {
        while (true) {
            Node* pred = head;
            Node* curr = reinterpret_cast<Node*>(
                pred->next.load() & ~MARK
            );

            while (true) {
                if (curr == tail) {
                    return {pred, curr};
                }

                std::uintptr_t next = curr->next.load();

                Node* succ = reinterpret_cast<Node*>(
                    next & ~MARK
                );

                if ((next & MARK) != 0) {
                    std::uintptr_t expected =
                        reinterpret_cast<std::uintptr_t>(curr);

                    if (!pred->next.compare_exchange_strong(
                            expected,
                            reinterpret_cast<std::uintptr_t>(succ))) {
                        break;
                    }

                    curr = succ;
                    continue;
                }

                if (!(*curr->value < value)) {
                    return {pred, curr};
                }

                pred = curr;
                curr = succ;
            }
        }
    }

public:
    LockFreeSet() {
        tail = new Node();
        head = new Node();

        head->next.store(
            reinterpret_cast<std::uintptr_t>(tail)
        );

        registerNode(tail);
        registerNode(head);
    }

    ~LockFreeSet() {
        Node* curr = allocated.load();

        while (curr != nullptr) {
            Node* next = curr->allocatedNext;
            delete curr;
            curr = next;
        }
    }

    bool add(T value) {
        Node* node = new Node(value, nullptr);
        registerNode(node);

        while (true) {
            auto [pred, curr] = find(value);

            if (curr != tail && *curr->value == value) {
                return false;
            }

            node->next.store(
                reinterpret_cast<std::uintptr_t>(curr)
            );

            std::uintptr_t expected =
                reinterpret_cast<std::uintptr_t>(curr);

            if (pred->next.compare_exchange_strong(
                    expected,
                    reinterpret_cast<std::uintptr_t>(node))) {
                return true;
            }
        }
    }

    bool remove(T value) {
        while (true) {
            auto [pred, curr] = find(value);

            if (curr == tail || *curr->value != value) {
                return false;
            }

            std::uintptr_t next = curr->next.load();

            if ((next & MARK) != 0) {
                continue;
            }

            std::uintptr_t expected = next;

            if (!curr->next.compare_exchange_strong(
                    expected,
                    next | MARK)) {
                continue;
            }

            expected = reinterpret_cast<std::uintptr_t>(curr);

            pred->next.compare_exchange_strong(
                expected,
                next
            );

            return true;
        }
    }

    bool contains(T value) const {
        Node* curr = reinterpret_cast<Node*>(
            head->next.load() & ~MARK
        );

        while (curr != tail) {
            std::uintptr_t next = curr->next.load();

            if (!(*curr->value < value)) {
                return *curr->value == value &&
                       (next & MARK) == 0;
            }

            curr = reinterpret_cast<Node*>(
                next & ~MARK
            );
        }

        return false;
    }

    bool isEmpty() const {
        while (true) {
            Node* first = reinterpret_cast<Node*>(
                head->next.load() & ~MARK
            );

            if (first == tail) {
                return true;
            }

            std::uintptr_t next = first->next.load();

            if ((next & MARK) == 0) {
                return false;
            }

            std::uintptr_t expected =
                reinterpret_cast<std::uintptr_t>(first);

            head->next.compare_exchange_strong(
                expected,
                next & ~MARK
            );
        }
    }
};
