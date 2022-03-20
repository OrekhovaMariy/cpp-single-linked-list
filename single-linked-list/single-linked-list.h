#pragma once

template <typename Type>
class SingleLinkedList {
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

private:
    template<typename T>
    void copy_elem(const T& values) {
        for (const auto& v : values) {
            PushFront(v);
        }
    }
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    size_t size_ = 0;

public:
    SingleLinkedList() { size_ = 0; }

    [[nodiscard]] size_t GetSize() const noexcept { return size_; }

    [[nodiscard]] bool IsEmpty() const noexcept;

    void PushFront(const Type& value);

    ~SingleLinkedList() {
        Clear();
    }

    void Clear() noexcept;

    void PopFront() noexcept;

    SingleLinkedList(std::initializer_list<Type> values);

    SingleLinkedList(const SingleLinkedList& other);

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this != &rhs) {
            SingleLinkedList rhs_copy(rhs);
            swap(rhs_copy);
        }
        return *this;
    }

    void swap(SingleLinkedList& other) noexcept;

    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;

        explicit BasicIterator(Node* node) : node_(node) {}

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept : BasicIterator(other.node_) {}

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        BasicIterator(const BasicIterator<const Type>& other) noexcept : BasicIterator(other.node_) {}

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        BasicIterator& operator++() noexcept {
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            BasicIterator current(node_);
            node_ = node_->next_node;
            return current;
        }

        [[nodiscard]] reference operator*() const noexcept {
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            if (node_ != nullptr) { return &(node_->value); }
            return{};
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

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(&head_);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_));
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_));
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        if (pos == cbefore_begin()) {
            PushFront(value);
            return Iterator(head_.next_node);
        }
        else {
            Node* elem = new Node(value, pos.node_->next_node);
            pos.node_->next_node = elem;
            ++size_;
            return Iterator(elem);
        }
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        if (pos == cbefore_begin()) {
            PopFront();
            return Iterator(head_.next_node);
        }
        else {
            Node* del_elem = pos.node_->next_node;
            pos.node_->next_node = pos.node_->next_node->next_node;
            delete del_elem;
            --size_;
            return Iterator(pos.node_->next_node);
        }
    }
};

template<typename Type>
SingleLinkedList<Type>::SingleLinkedList(std::initializer_list<Type> values) {
    SingleLinkedList tmp_invert;
    for (auto it : values) {
        tmp_invert.PushFront(it);
    }
    copy_elem(tmp_invert);
    size_ = values.size();
}

template<typename Type>
SingleLinkedList<Type>::SingleLinkedList(const SingleLinkedList& other) {
    SingleLinkedList tmp_invert;
    for (auto it : other) {
        tmp_invert.PushFront(it);
    }
    copy_elem(tmp_invert);
}

template<typename Type>
[[nodiscard]] bool SingleLinkedList<Type>::IsEmpty() const noexcept {
    if (GetSize() == 0) {
        return true;
    }
    return false;
}

template<typename Type>
void SingleLinkedList<Type>::PushFront(const Type& value) {
    head_.next_node = new Node(value, head_.next_node);
    ++size_;
}

template<typename Type>
void SingleLinkedList<Type>::Clear() noexcept {
    while (size_)
    {
        Node* del = head_.next_node;
        head_.next_node = head_.next_node->next_node;
        delete del;
        --size_;
    }
}

template<typename Type>
void SingleLinkedList<Type>::PopFront() noexcept {
    if (size_) {
        Node* del = head_.next_node;
        head_.next_node = head_.next_node->next_node;
        delete del;
        --size_;
    }
    else { before_begin() = end(); }
}

template<typename Type>
void SingleLinkedList<Type>::swap(SingleLinkedList& other) noexcept {
    //if (this != &other) {
    std::swap(head_.next_node, other.head_.next_node);
    std::swap(size_, other.size_);
}

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(std::equal(lhs.begin(), lhs.end(), rhs.begin()));
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
        rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(std::lexicographical_compare(rhs.begin(), rhs.end(),
        lhs.begin(), lhs.end()));
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(rhs.begin(), rhs.end(),
        lhs.begin(), lhs.end());
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(std::lexicographical_compare(lhs.begin(), lhs.end(),
        rhs.begin(), rhs.end()));
}
