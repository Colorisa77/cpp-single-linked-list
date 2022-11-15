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

        // Конвертирующий конструктор итератора из указателя на узел списка
        explicit BasicIterator(Node* node) {
            this->node_ = node;
        }

    public:
        //Объявление ниже типы сообщают стандартной библиотеке о свойствах этого итератора
        //итератор, который поддерживает операции инкремента и многократного разымнование
        using iterator_category = std::forward_iterator_tag;

        //тип элементов, по которым перемещаются итератор
        using value_type = Type;

        //Тип, используемый для хранения смещения между итераторами
        using difference_type = std::ptrdiff_t;

        //Тип указателя на итерируемое значение
        using pointer = ValueType*;

        //Тип ссылки на итерируемое значение
        using reference = ValueType&;

        BasicIterator() = default;

        //Конвертирующий конструктор
        BasicIterator(const BasicIterator<Type>& other) noexcept {
            this->node_ = other.node_;
        }

        //Что компилятор не выдавал предупреждение об отсутствии оператора = при наличии
        // пользовательского конструктора копирования, явно объявим оператор =
        // попросим компилятор сгенерировать его за нас
        BasicIterator& operator=(const BasicIterator& rhs) = default;

        //Оператора сравниявания итераторов (в роли второго аргумента выступает конст итератор//
        //Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            if (rhs.node_ == this->node_) {
                return true;
            }
            return false;
        }

        //Оператор неравенства
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            if (!(rhs == *this)) {
                return true;
            }
            return false;
        }

        //Оператор предикамента. После вызова итератор указывает на след элемент списка
        //Возвращает ссылку на самого себя
        //Инкремент итератора, не указывающего на существующий элемент списка, приводик к неопределенному поведению
        BasicIterator& operator++() noexcept {
            this->node_ = this->node_->next_node;
            return *this;
        }

        //Оператор постинкримента. После вызова итератор указывает на следующий элемент списка
        //Возвращает прежнее значение итератора
        //Инкремент итератора, не указывающего на существующий элемент списка
        //приводи к неопределенному поведению
        BasicIterator operator++(int) noexcept {
            auto old_value(*this);
            ++(*this);
            return old_value;
        }

        //Оператор разыменовывания. Возвращает ссылку на текущий элемент
        //Вызов этого оператора у итератора, не указывающего на существующий элемент списка, приводи к неопределенному поведению
        [[nodiscard]] reference operator*() const {
            return node_->value;
        }

        //Операция доступа к члену класса. Возвращает указатель на текущий элемент списка
        [[nodiscard]] pointer operator->() const {
            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };
public:

    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    //Итератор, допускающий изменение элементов списка
    using Iterator = BasicIterator<Type>;
    //Константный итератор, предствляющий доступ для чтения к элементам списка
    using ConstIterator = BasicIterator<const Type>;

    // Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator before_begin() noexcept {
        // Реализуйте самостоятельно
        return Iterator(&head_);
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        Node& result = const_cast<Node&>(head_);
        return ConstIterator(&result);
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator(&head_);
    }

    // Возвращает итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }

    // Возвращает итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    // Результат вызова эквивалентен вызову метода cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    // Результат вызова эквивалентен вызову метода cend()
    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator(nullptr);
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }


    SingleLinkedList() = default;
    // Возвращает количество элементов в списке за время O(1)
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    // Сообщает, пустой ли список за время O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
        // Заглушка. Реализуйте метод самостоятельно
        if (size_ == 0) {
            return true;
        }
        return false;
    }

    //Вставляет элемент в начало списка
    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }


    //Вставляет элемент после указанной позиции и возвращает итератор на элемент, следующий за удалённым
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        pos.node_->next_node = new Node(value, pos.node_->next_node);
        ++size_;
        return Iterator(pos.node_->next_node);
    }

    // Удаляет первый элемент в списке
    void PopFront() noexcept {
        if (this->size_ != 0) {
            Node* removed_node = head_.next_node;
            head_.next_node = head_.next_node->next_node;
            --size_;
            delete removed_node;
        }
    }


    // Удаляет элемент, следующий за pos. Возвращает итератор на элемент, следующий за удалённым
    Iterator EraseAfter(ConstIterator pos) noexcept {
        Node* removed_node = pos.node_->next_node;
        pos.node_->next_node = removed_node->next_node;
        --size_;
        delete removed_node;
        return Iterator(pos.node_->next_node);
    }

    // Очищает список за время O(N)
    void Clear() noexcept {
        while (size_ > 0) {
            auto temp = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = temp;
            --size_;
        }

    }

    //Конструирование списка на основе initializer_list. Последовательно должна быть одинаковой
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

    //Конструктор копирования для копирования другого списка
    SingleLinkedList(const SingleLinkedList& other) {
        if (this != &other) {
            SingleLinkedList temp;
            if (other.GetSize() == 0) {
                return;
            }
            for (auto it = other.begin(); it != other.end(); ++it) {
                temp.PushFront((*it));
            }
            Iterator curr_node = temp.begin();
            Iterator next_node = ++temp.begin();
            Node* prev_node = nullptr;
            size_t size = temp.GetSize();
            while (size != 1) {
                curr_node.node_->next_node = prev_node;
                prev_node = curr_node.node_;
                curr_node = next_node;
                ++next_node;
                --size;
            }
            curr_node.node_->next_node = prev_node;
            temp.head_.next_node = curr_node.node_;
            prev_node = nullptr;
            delete prev_node;
            swap(temp);
        }
    }

    //Оператора =
    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        SingleLinkedList tmp(rhs);
        swap(tmp);
        return *this;
    }

    //Обмен содержимого списков
    void swap(SingleLinkedList& other) noexcept {
        std::swap(this->size_, other.size_);
        std::swap(this->head_.next_node, other.head_.next_node);
    }

    ~SingleLinkedList() {
        Clear();
    }

private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
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