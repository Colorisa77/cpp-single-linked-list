#pragma once

#include <iterator>
#include <cassert>
#include <algorithm>

template<typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    template<typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;

        explicit BasicIterator(Node* node) {
            this->node_ = node;
        }

    public:
        using iterator_category = std::forward_iterator_tag;

        using value_type = Type;

        using difference_type = std::ptrdiff_t;

        using pointer = ValueType*;

        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept {
            this->node_ = other.node_;
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            if (rhs.node_ == this->node_) {
                return true;
            }
            return false;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            if (!(rhs == *this)) {
                return true;
            }
            return false;
        }

        BasicIterator& operator++() noexcept {
            assert(node_ != nullptr);
            this->node_ = this->node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            auto old_value(*this);
            ++(*this);
            return old_value;
        }

        [[nodiscard]] reference operator*() const {
            assert(node_ != nullptr);
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const {
            assert(node_ != nullptr);
            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };
public:

    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(&head_);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        Node& result = const_cast<Node&>(head_);
        return ConstIterator(&result);
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator(&head_);
    }

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator(nullptr);
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }


    SingleLinkedList() = default;
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        if (size_ == 0) {
            return true;
        }
        return false;
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_ != nullptr);
        pos.node_->next_node = new Node(value, pos.node_->next_node);
        ++size_;
        return Iterator(pos.node_->next_node);
    }

    void PopFront() noexcept {
        if (this->size_ != 0) {
            Node* removed_node = head_.next_node;
            head_.next_node = head_.next_node->next_node;
            --size_;
            delete removed_node;
        }
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        Node* removed_node = pos.node_->next_node;
        pos.node_->next_node = removed_node->next_node;
        --size_;
        delete removed_node;
        return Iterator(pos.node_->next_node);
    }

    void Clear() noexcept {
        while (size_ > 0) {
            auto temp = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = temp;
            --size_;
        }

    }

    SingleLinkedList(std::initializer_list<Type> values) {
        for (auto it = values.end(); it != values.begin(); --it) {
            if (it == values.end()) {
                continue;
            }
            this->PushFront(*(it));
        }
        auto it = values.begin();
        this->PushFront(*(it));
    }

    SingleLinkedList(const SingleLinkedList& other) {
        if (this != &other) {
            if (other.GetSize() == 0) {
                return;
            }
            ConstIterator other_node = other.begin();
            this->PushFront(*other_node);
            ++other_node;
            ConstIterator this_node = this->begin();
            for (; other_node != other.end(); ++other_node) {
                this->InsertAfter(this_node, *other_node);
                ++this_node;
            }
        }
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        SingleLinkedList tmp(rhs);
        swap(tmp);
        return *this;
    }

    void swap(SingleLinkedList& other) noexcept {
        std::swap(this->size_, other.size_);
        std::swap(this->head_.next_node, other.head_.next_node);
    }

    ~SingleLinkedList() {
        Clear();
    }

private:
    Node head_;
    size_t size_ = 0;

};

//Обмен содержимого списков
template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template<typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (lhs.GetSize() != rhs.GetSize()) {
        return false;
    }
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template<typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (!(lhs == rhs));
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if ((lhs < rhs) || (lhs == rhs)) {
        return true;
    }
    return false;
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (!(lhs < rhs));
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if ((lhs > rhs) || (lhs == rhs)) {
        return true;
    }
    return false;
}