#ifndef VECTOR_H
#define VECTOR_H
#include <cstddef>

template<class T>
class Vector {
	size_t size_;
	size_t capacity_;
	T* buf_;
	const static size_t kIncreaseFactor = 2;

	void Reallocate(size_t cap);
	size_t CalculateCapacity(size_t cap) const;

public:
	Vector();
	explicit Vector(size_t size);
	Vector(size_t size, const T& value);
	Vector(const Vector& other);
	Vector& operator=(const Vector& other);
	~Vector();

	size_t Size() const;
	size_t Capacity() const;
	void PushBack(const T& value);
	void PopBack();
	void Resize(size_t new_size, const T& value);
	void Resize(size_t new_size);
	bool Empty() const;
	void Clear();
	void Reserve(size_t new_cap);
	void ShrinkToFit();
	T& Front();
	T& Back();
	const T* Data() const;
	const T Front() const;
	const T Back() const;
	void Swap(Vector& other);
	const T operator[](size_t idx) const;
	T& operator[](size_t idx);
};


template<class T>
void Copy(const T* from, size_t size, T* to);
template<class T>
void Fill(T* buf, size_t size, const T& value);
template<class T>
void Swap(T& lhs, T& rhs);

template<class T>
bool operator<(const Vector<T>& lhs, const Vector<T>& rhs);
template<class T>
bool operator<=(const Vector<T>& lhs, const Vector<T>& rhs);
template<class T>
bool operator>(const Vector<T>& lhs, const Vector<T>& rhs);
template<class T>
bool operator>=(const Vector<T>& lhs, const Vector<T>& rhs);
template<class T>
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs);
template<class T>
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs);


template <class T>
void Vector<T>::Reallocate(size_t cap) {
	capacity_ = cap;
	T* new_buf = new T[capacity_];
	Copy(buf_, size_, new_buf);
	delete[] buf_;
	buf_ = new_buf;
}


template <class T>
size_t Vector<T>::CalculateCapacity(size_t cap) const {
	size_t new_cap = capacity_ == 0 ? 1 : capacity_;
	while (new_cap < cap) {
		new_cap *= kIncreaseFactor;
	}
	return new_cap;
}


template <class T>
Vector<T>::Vector() : size_(0), capacity_(0), buf_(nullptr) {}


template <class T>
Vector<T>::Vector(size_t size) : size_(size), capacity_(size) {
	buf_ = new T[capacity_];
}


template <class T>
Vector<T>::Vector(size_t size, const T& value) : Vector(size) {
	Fill(buf_, size, value);
}


template <class T>
Vector<T>::Vector(const Vector& other) {
	size_ = other.size_;
	capacity_ = other.capacity_;
	buf_ = new T[capacity_];
	Copy(other.buf_, size_, buf_);
}


template <class T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
	if (this == &other) {
		return *this;
	}
	if (other.size_ > capacity_) {
		Reallocate(other.size_);
	}
	size_ = other.size_;
	Copy(other.buf_, size_, buf_);
	return *this;
}

template <class T>
Vector<T>::~Vector() {
	delete[] buf_;
}

template <class T>
size_t Vector<T>::Size() const {
	return size_;
}

template <class T>
size_t Vector<T>::Capacity() const {
	return capacity_;
}

template <class T>
void Vector<T>::PushBack(const T& value) {
	size_t cap = CalculateCapacity(size_ + 1);
	if (cap > capacity_) {
		Reallocate(cap);
	}
	buf_[size_] = value;
	++size_;
}

template <class T>
void Vector<T>::PopBack() {
	--size_;
}

template <class T>
void Vector<T>::Resize(size_t new_size) {
	if (new_size > capacity_) {
		Reallocate(new_size);
	}
	size_ = new_size;
}

template <class T>
void Vector<T>::Resize(size_t new_size, const T& value) {
	const size_t before_size = size_;
	Resize(new_size);
	if (size_ > before_size) {
		Fill(buf_ + before_size, size_ - before_size, value);
	}
}

template <class T>
bool Vector<T>::Empty() const {
	return size_ == 0;
}

template <class T>
void Vector<T>::Clear() {
	size_ = 0;
}

template <class T>
void Vector<T>::Reserve(size_t new_cap) {
	if (capacity_ < new_cap) {
		Reallocate(new_cap);
	}
}

template <class T>
void Vector<T>::ShrinkToFit() {
	Reallocate(size_);
}


template <class T>
T& Vector<T>::Front() {
	return buf_[0];
}

template <class T>
T& Vector<T>::Back() {
	return buf_[size_ - 1];
}

template <class T>
const T* Vector<T>::Data() const {
	return buf_;
}


template <class T>
const T Vector<T>::Front() const {
	return buf_[0];
}


template <class T>
const T Vector<T>::Back() const {
	return buf_[size_ - 1];
}


template <class T>
void Vector<T>::Swap(Vector& other) {
	::Swap(buf_, other.buf_);
	::Swap(capacity_, other.capacity_);
	::Swap(size_, other.size_);
}


template <class T>
T& Vector<T>::operator[](size_t idx) {
	return buf_[idx];
}


template <class T>
const T Vector<T>::operator[](size_t idx) const {
	return buf_[idx];
}


template <class T>
bool operator<(const Vector<T>& lhs, const Vector<T>& rhs) {
	const size_t min_size = (lhs.Size() < rhs.Size()) ? lhs.Size() : rhs.Size();
	for (size_t i = 0; i < min_size; ++i) {
		if (lhs[i] < rhs[i]) {
			return true;
		} else if (lhs[i] > rhs[i]) {
			return false;
		}
	}
	return lhs.Size() < rhs.Size();
}


template <class T>
bool operator<=(const Vector<T>& lhs, const Vector<T>& rhs) {
	return !(rhs < lhs);
}


template <class T>
bool operator>(const Vector<T>& lhs, const Vector<T>& rhs) {
	return rhs < lhs;
}


template <class T>
bool operator>=(const Vector<T>& lhs, const Vector<T>& rhs) {
	return !(lhs < rhs);
}


template <class T>
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs) {
	const size_t size = lhs.Size();
	if (size != rhs.Size()) {
		return false;
	}
	for (size_t i = 0; i < size; ++i) {
		if (lhs[i] != rhs[i]) {
			return false;
		}
	}
	return true;
}

template <class T>
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs) {
	return !(rhs == lhs);
}


template<class T>
void Copy(const T* from, size_t size, T* to) {
	for (size_t i = 0; i < size; ++i) {
		to[i] = *from;
		++from;
	}
}

template<class T>
void Fill(T* buf, size_t size, const T& value) {
	for (size_t i = 0; i < size; ++i) {
		buf[i] = value;
	}
}

template <class T>
void Swap(T& lhs, T& rhs) {
	T temp = lhs;
	lhs = rhs;
	rhs = temp;
}

#endif
