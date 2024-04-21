#ifndef SINGLY_LINKED_LIST_HPP
#define SINGLY_LINKED_LIST_HPP

#include <memory>
#include <cassert>
#include <stdexcept>
#include <utility>
#include <iostream>
#include <limits>


template<typename T, typename Alloc = std::allocator<T>>
struct singly_linked_list {
    struct Node {
        T data;
        std::unique_ptr<Node> next_node;

        explicit Node(const T &value) : data(value), next_node(nullptr) {}

        explicit Node(T &&value) : data(std::move(value)), next_node(nullptr) {}
    };

    std::unique_ptr<Node> head_ = nullptr;
    Node *tail_ = nullptr;
    std::size_t size_ = 0;

    singly_linked_list() = default;

    explicit singly_linked_list(std::size_t n) {
        try {
            for (std::size_t i = 0; i < n; ++i) {
                push_back(T());
            }
        } catch (...) {
            clear();
            throw;
        }
    }

    singly_linked_list(std::size_t n, const T &value) {
        try {
            for (std::size_t i = 0; i < n; ++i) {
                push_back(value);
            }
        } catch (...) {
            clear();
            throw;
        }
    }

    ~singly_linked_list() noexcept {
        clear();
    }

    void clear() noexcept {
        while (head_ != nullptr) {
            auto temp = std::move(head_->next_node);
            head_->next_node.reset();
            head_.reset();
            head_ = std::move(temp);
            --size_;
        }
    }

    void push_back(const T &value) {
        try {
            assert(size_ < std::numeric_limits<std::size_t>::max() && "Size limit reached");
            if (!head_) {
                head_ = std::make_unique<Node>(value);
                tail_ = head_.get();
            } else {
                tail_->next_node = std::make_unique<Node>(value);
                tail_ = tail_->next_node.get();
            }
            ++size_;
        } catch (const std::exception &e) {
            std::cerr << "Exception caught: " << e.what() << std::endl;
            throw;
        }
    }

    void push_front(const T &value) {
        try {
            assert(size_ < std::numeric_limits<std::size_t>::max() && "Size limit reached");
            std::unique_ptr<Node> new_node = std::make_unique<Node>(value);
            new_node->next_node = std::move(head_);
            head_ = std::move(new_node);
            ++size_;
        } catch (const std::exception &e) {
            std::cerr << "Exception caught: " << e.what() << std::endl;
            throw;
        }
    }

    void pop_front() {
        try {
            assert(head_ != nullptr && "Pop from empty list");
            if (head_ != nullptr) {
                auto temp = std::move(head_->next_node);
                head_->next_node.reset();
                head_ = std::move(temp);
                --size_;
            }
        } catch (const std::exception &e) {
            std::cerr << "Exception caught: " << e.what() << std::endl;
            throw;
        }
    }

    void pop_back() {
        try {
            assert(head_ != nullptr && "Pop from empty list");
            if (head_->next_node == nullptr) {
                head_.reset();
                tail_ = nullptr;
            } else {
                Node *current = head_.get();
                while (current->next_node->next_node) {
                    current = current->next_node.get();
                }
                current->next_node.reset();
                tail_ = current;
            }
            --size_;
        } catch (const std::exception &e) {
            std::cerr << "Exception caught: " << e.what() << std::endl;
            throw;
        }
    }

    void insert(std::size_t pos, std::size_t count, const T &value) {
        try {
            assert(pos <= size_ && "Index out of range");

            if (count == 0)
                return;

            if (pos == 0) {
                for (std::size_t i = 0; i < count; ++i) {
                    push_front(value);
                }
            } else if (pos == size_) {
                for (std::size_t i = 0; i < count; ++i) {
                    push_back(value);
                }
            } else {
                Node *current = head_.get();
                for (std::size_t i = 0; i < pos - 1; ++i) {
                    current = current->next_node.get();
                }
                for (std::size_t i = 0; i < count; ++i) {
                    std::unique_ptr<Node> new_node = std::make_unique<Node>(value);
                    new_node->next_node = std::move(current->next_node);
                    current->next_node = std::move(new_node);
                    ++size_;
                    current = current->next_node.get();
                }
            }
        } catch (const std::exception &e) {
            std::cerr << "Exception caught: " << e.what() << std::endl;
            throw;
        }
    }

    [[nodiscard]] std::size_t size() const noexcept {
        return size_;
    }

    T &operator[](std::size_t index) {
        try {
            assert(index < size_ && "Index out of range");
            Node *current = head_.get();
            for (std::size_t i = 0; i < index; ++i) {
                current = current->next_node.get();
            }
            return current->data;
        } catch (const std::exception &e) {
            std::cerr << "Exception caught: " << e.what() << std::endl;
            throw;
        }
    }

    const T &operator[](std::size_t index) const {
        try {
            assert(index < size_ && "Index out of range");
            Node *current = head_.get();
            for (std::size_t i = 0; i < index; ++i) {
                current = current->next_node.get();
            }
            return current->data;
        } catch (const std::exception &e) {
            std::cerr << "Exception caught: " << e.what() << std::endl;
            throw;
        }
    }

    template<typename U>
    int find(const U &value) const {
        int index = 0;
        try {
            for (Node *current = head_.get(); current != nullptr; current = current->next_node.get()) {
                if (current->data == value) {
                    return index;
                }
                ++index;
            }
        } catch (const std::exception &e) {
            std::cerr << "Exception caught: " << e.what() << std::endl;
            throw;
        }
        return -1;
    }
};

#endif // SINGLY_LINKED_LIST_HPP
