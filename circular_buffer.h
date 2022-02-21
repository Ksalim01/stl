#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H
#include <cstddef>

template <class T>
class CircularBuffer {
	size_t capacity_;
	size_t size_;
	size_t front_;
	size_t back_;
	T* buf_;
	const static size_t kIncreaseFactor = 2;

	void Reallocate(size_t new_cap);
	size_t CalculateCapacity(size_t cap) const;

public:
	CircularBuffer();
	explicit CircularBuffer(size_t count);
	CircularBuffer(const CircularBuffer& other);
	CircularBuffer& operator=(const CircularBuffer& other);
	~CircularBuffer();

	const T operator[](size_t idx) const;
	T& operator[](size_t idx);
	const T Front() const;
	T& Front();
	const T Back() const;
	T& Back();
	bool Empty() const;
	size_t Size() const;
	size_t Capacity() const;
	void PushBack(const T& value);
	void PushFront(const T& value);
	void PopBack();
	void PopFront();
	void Clear();
	void Reserve(size_t new_cap);
	void Swap(CircularBuffer& other);
};

template<class T>
void Copy(const T* from, size_t size, T* to);
template<class T>
void Swap(T& lhs, T& rhs);

template <class T>
void CircularBuffer<T>::Reallocate(size_t new_cap) {
	T* new_buf = new T[new_cap];
	for (size_t i = 0; i < size_; ++i) {
		new_buf[i] = (*this)[i];
	}
	delete[] buf_;
	capacity_ = new_cap;
	buf_ = new_buf;
	front_ = 0;
	back_ = size_ == 0 ? 0 : size_ - 1;
}

template <class T>
size_t CircularBuffer<T>::CalculateCapacity(size_t cap) const {
	size_t new_cap = capacity_ == 0 ? 1 : capacity_;
	while (new_cap < cap) {
		new_cap *= kIncreaseFactor;
	}
	return new_cap;
}

template <class T>
CircularBuffer<T>::CircularBuffer() : capacity_(0), size_(0), front_(0), back_(0), buf_(nullptr) {
}

template <class T>
CircularBuffer<T>::CircularBuffer(size_t count) : capacity_(count), size_(0), front_(0), back_(0) {
	buf_ = new T[capacity_];
}

template <class T>
CircularBuffer<T>::CircularBuffer(const CircularBuffer& other) :
capacity_(other.capacity_), size_(other.size_), front_(other.front_), back_(other.back_) {
	buf_ = new T[capacity_];
	Copy(other.buf_, size_, buf_);
}

template <class T>
CircularBuffer<T>& CircularBuffer<T>::operator=(const CircularBuffer& other) {
	if (this == &other) {
		return *this;
	}
	else if (other.size_ > capacity_) {
		Reallocate(other.capacity_);
	}
	size_ = other.size_;
	front_ = 0;
	back_ = size_ - 1;
	for (size_t i = 0; i < size_; ++i) {
		(*this)[i] = other[i];
	}
	return *this;
}

template <class T>
CircularBuffer<T>::~CircularBuffer() {
	delete[] buf_;
}

template <class T>
const T CircularBuffer<T>::operator[](size_t idx) const {
	return buf_[(idx + front_) % capacity_];
}

template <class T>
T& CircularBuffer<T>::operator[](size_t idx) {
	return buf_[(idx + front_) % capacity_];
}

template <class T>
const T CircularBuffer<T>::Front() const {
	return buf_[front_];
}

template <class T>
const T CircularBuffer<T>::Back() const {
	return buf_[back_];
}

template <class T>
T& CircularBuffer<T>::Front() {
	return buf_[front_];
}

template <class T>
T& CircularBuffer<T>::Back() {
	return buf_[back_];
}

template <class T>
bool CircularBuffer<T>::Empty() const {
	return size_ == 0;
}

template <class T>
size_t CircularBuffer<T>::Size() const {
	return size_;
}

template <class T>
size_t CircularBuffer<T>::Capacity() const {
	return capacity_;
}

template <class T>
void CircularBuffer<T>::PushBack(const T& value) {
	const size_t cap = CalculateCapacity(size_ + 1);
	if (cap > capacity_) {
		Reallocate(cap);
	}
	back_ = (back_ + 1) % capacity_;
	++size_;
	buf_[back_] = value;
}

template <class T>
void CircularBuffer<T>::PushFront(const T& value) {
	const size_t cap = CalculateCapacity(size_ + 1);
	if (cap > capacity_) {
		Reallocate(cap);
	}
	front_ = (front_ + capacity_ - 1) % capacity_;
	++size_;
	buf_[front_] = value;
}

template <class T>
void CircularBuffer<T>::PopBack() {
	--size_;
	back_ = (back_ + capacity_ - 1) % capacity_;
}


template <class T>
void CircularBuffer<T>::PopFront() {
	--size_;
	front_ = (front_ + 1) % capacity_;
}

template <class T>
void CircularBuffer<T>::Clear() {
	size_ = 0;
	front_ = back_ = 0;
}

template <class T>
void CircularBuffer<T>::Reserve(size_t new_cap) {
	if (capacity_ < new_cap) {
		Reallocate(new_cap);
	}
}

template <class T>
void CircularBuffer<T>::Swap(CircularBuffer& other) {
	::Swap(buf_, other.buf_);
	::Swap(capacity_, other.capacity_);
	::Swap(size_, other.size_);
	::Swap(front_, other.front_);
	::Swap(back_, other.back_);
}


template<class T>
void Copy(const T* from, size_t size, T* to) {
	for (size_t i = 0; i < size; ++i) {
		to[i] = from[i];
	}
}

template<class T>
void Swap(T& lhs, T& rhs) {
	T temp = lhs;
	lhs = rhs;
	rhs = temp;
}

#endif
