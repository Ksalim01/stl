#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#include <ostream>
#include <utility>


class StringView {
    const char* str_;
    size_t size_;

public:
    typedef const char* const_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	
    StringView();
    StringView(const char* str);
    StringView(const char* str, size_t size);
    char operator[](size_t index) const;
    char At(int index) const;
    char Front() const;
    char Back() const;
    size_t Size() const;
    size_t Length() const;
    bool Empty() const;
    const char* Data() const;
    void Swap(StringView& other);
    void RemovePrefix(size_t prefix_size);
    void RemoveSuffix(size_t suffix_size);
    const StringView Substr(size_t pos, size_t count = -1) const;
    constexpr const_iterator begin() const {
        return const_iterator(str_);
    }
    constexpr const_iterator end() const {
        return const_iterator(str_ + size_);
    }
    constexpr const_iterator cbegin() const {
        return begin();
    }
    constexpr const_iterator cend() const {
        return end();
    }
    constexpr const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }
    constexpr const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }
    constexpr const_reverse_iterator crbegin() const {
        return rbegin();
    }
    constexpr const_reverse_iterator crend() const {
        return rend();
    }
};

inline StringView::StringView() : str_(nullptr), size_(0) {
}

inline StringView::StringView(const char* str) : str_(str), size_(strlen(str)) {
}

inline StringView::StringView(const char* str, size_t size) : str_(str), size_(size) {
}

inline char StringView::operator[](size_t index) const {
    return str_[index];
}

inline char StringView::At(int index) const {
    if (index < 0 || index > static_cast<int>(size_)) {
        throw std::out_of_range("");
    }
    return str_[index];
}

inline char StringView::Front() const {
    return str_[0];
}

inline char StringView::Back() const {
    return str_[size_ - 1];
}

inline size_t StringView::Size() const {
    return size_;
}

inline size_t StringView::Length() const {
    return size_;
}

inline void StringView::Swap(StringView& other) {
    std::swap(str_, other.str_);
    std::swap(size_, other.size_);
}

inline bool StringView::Empty() const {
    return size_ == 0;
}

inline void StringView::RemovePrefix(size_t prefix_size) {
    str_ += prefix_size;
    size_ -= prefix_size;
}

inline void StringView::RemoveSuffix(size_t suffix_size) {
    size_ -= suffix_size;
}

inline const char* StringView::Data() const {
    return str_;
}

inline const StringView StringView::Substr(size_t pos, size_t count) const {
    StringView substr(str_);
    substr.RemovePrefix(pos);
    size_t size = count + pos > substr.size_ ? substr.size_ - pos : count;
    substr.size_ = size;
    return substr;
}

#endif
