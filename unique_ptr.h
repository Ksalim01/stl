#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

#include <utility>

template <class T>
class UniquePtr {
	T* ptr_;

public:
	UniquePtr();
	UniquePtr(T* object);
	UniquePtr(const UniquePtr& other) = delete;
	UniquePtr(UniquePtr&& other) noexcept;
	UniquePtr& operator=(const UniquePtr& other) = delete;
	UniquePtr& operator=(UniquePtr&& other) noexcept;
	~UniquePtr();

	T* Release();
	void Reset(T* ptr = nullptr);
	void Swap(UniquePtr<T>& other);
	T* Get() const;
	T& operator*() const;
	T* operator->() const;
	explicit operator bool() const;
};

template <class T>
UniquePtr<T>::UniquePtr(): ptr_(nullptr) {
}

template <class T>
UniquePtr<T>::UniquePtr(T* object): ptr_(object) {
}

template <class T>
UniquePtr<T>::UniquePtr(UniquePtr&& other) noexcept: ptr_(other.ptr_) {
	other.ptr_ = nullptr;
}

template <class T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr&& other) noexcept {
	ptr_ = other.ptr_;
	other.ptr_ = nullptr;
	return *this;
}

template <class T>
UniquePtr<T>::~UniquePtr() {
	delete ptr_;
}

template <class T>
T* UniquePtr<T>::Release() {
	T* ptr = ptr_;
	ptr_ = nullptr;
	return ptr;
}

template <class T>
void UniquePtr<T>::Reset(T* ptr) {
	delete ptr_;
	ptr_ = ptr;
}

template <class T>
T* UniquePtr<T>::Get() const {
	return ptr_;
}

template <class T>
T& UniquePtr<T>::operator*() const {
	return *ptr_;
}

template <class T>
T* UniquePtr<T>::operator->() const {
	return ptr_;
}

template <class T>
void UniquePtr<T>::Swap(UniquePtr<T>& other) {
	std::swap(ptr_, other.ptr_);
}

template <class T>
UniquePtr<T>::operator bool() const {
	return ptr_ != nullptr;
}


#endif 