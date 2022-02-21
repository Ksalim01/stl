#ifndef SHARED_PTR_H
#define SHARED_PTR_H

#include<algorithm>

class BadWeakPtr : public std::exception {
public:
	const char* what() const noexcept override {
		return "BadWeakPtr";
	}
};


template<class T>
class SharedPtr;

struct Counter {
	size_t shared_cnt_;
	size_t weak_cnt_;

	Counter() : shared_cnt_(0), weak_cnt_(0) {
	}
	Counter(size_t shared, size_t weak) : shared_cnt_(shared), weak_cnt_(weak) {
	}
};

template<class T>
class WeakPtr {
	T* ptr_;
	Counter* cnt_;

public:
	WeakPtr();
	WeakPtr(const WeakPtr& other);
	WeakPtr(WeakPtr&& other) noexcept;
	WeakPtr(const SharedPtr<T>& other);
	WeakPtr& operator=(const WeakPtr& other);
	WeakPtr& operator=(WeakPtr&& other) noexcept;
	~WeakPtr();

	void Swap(WeakPtr<T>& other);
	void Reset();
	size_t UseCount() const;
	bool Expired() const;
	SharedPtr<T> Lock() const;
	T* Get() const;
	friend class SharedPtr<T>;
};


template<class T>
class SharedPtr {
	T* ptr_;
	Counter* cnt_;

public:
	SharedPtr();
	SharedPtr(T* object);
	SharedPtr(const SharedPtr& other);
	SharedPtr(SharedPtr&& other) noexcept;
	SharedPtr(const WeakPtr<T>& other);
	SharedPtr& operator=(const SharedPtr& other);
	SharedPtr& operator=(SharedPtr&& other) noexcept;
	~SharedPtr();

	void Reset(T* ptr = nullptr);
	void Swap(SharedPtr<T>& other);
	T* Get() const;
	size_t UseCount() const;
	T& operator*() const;
	T* operator->() const;
	explicit operator bool() const;
	friend class WeakPtr<T>;
};

template <class T>
SharedPtr<T>::SharedPtr() : ptr_(nullptr), cnt_(nullptr) {
}


template <class T>
SharedPtr<T>::SharedPtr(T* object) : ptr_(object), cnt_(object == nullptr ? nullptr : new Counter(1, 0)) {
}

template <class T>
SharedPtr<T>::SharedPtr(const SharedPtr& other) : ptr_(other.ptr_), cnt_(other.cnt_) {
	++cnt_->shared_cnt_;
}

template <class T>
SharedPtr<T>::SharedPtr(SharedPtr&& other) noexcept : ptr_(other.ptr_), cnt_(other.cnt_) {
	other.cnt_ = nullptr;
	other.ptr_ = nullptr;
}

template <class T>
SharedPtr<T>::SharedPtr(const WeakPtr<T>& other) : ptr_(other.Get()), cnt_(other.cnt_) {
	if (other.Expired()) {
		throw BadWeakPtr();
	}
	if (cnt_ != nullptr && ptr_ != nullptr) {
		++cnt_->shared_cnt_;
	}
}

template <class T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& other) {
	if (this == &other) {
		return *this;
	}
	Reset();
	ptr_ = other.ptr_;
	cnt_ = other.cnt_;
	++cnt_->shared_cnt_;
	return *this;
}

template <class T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr&& other) noexcept {
	Reset();
	ptr_ = other.ptr_;
	cnt_ = other.cnt_;
	other.ptr_ = nullptr;
	other.cnt_ = nullptr;
	return *this;
}

template <class T>
SharedPtr<T>::~SharedPtr() {
	Reset();
}

template <class T>
void SharedPtr<T>::Reset(T* ptr) {
	if (cnt_ != nullptr && cnt_->shared_cnt_ == 1 && cnt_->weak_cnt_ == 0) {
		delete ptr_;
		delete cnt_;
	} else if (cnt_ != nullptr && cnt_->shared_cnt_ == 1) {
		delete ptr_;
		--cnt_->shared_cnt_;
	} else if (cnt_ != nullptr && cnt_->shared_cnt_ != 0) {
		--cnt_->shared_cnt_;
	}
	ptr_ = ptr;
	cnt_ = ptr == nullptr ? nullptr : new Counter(1, 0);
}


template <class T>
T* SharedPtr<T>::Get() const {
	return ptr_;
}

template <class T>
size_t SharedPtr<T>::UseCount() const {
	if (cnt_ == nullptr) {
		return 0;
	}
	return cnt_->shared_cnt_;
}

template <class T>
T& SharedPtr<T>::operator*() const {
	return *ptr_;
}

template <class T>
T* SharedPtr<T>::operator->() const {
	return ptr_;
}

template <class T>
SharedPtr<T>::operator bool() const {
	return Get() != nullptr;
}

template <class T>
void SharedPtr<T>::Swap(SharedPtr<T>& other) {
	std::swap(ptr_, other.ptr_);
	std::swap(cnt_, other.cnt_);
}


template <class T>
WeakPtr<T>::WeakPtr() : ptr_(nullptr), cnt_(nullptr) {
}

template <class T>
WeakPtr<T>::WeakPtr(const WeakPtr& other) : ptr_(other.ptr_), cnt_(other.cnt_) {
	if (cnt_ != nullptr && ptr_ != nullptr) {
		++cnt_->weak_cnt_;
	}
}

template <class T>
WeakPtr<T>::WeakPtr(WeakPtr&& other) noexcept : ptr_(other.ptr_), cnt_(other.cnt_) {
    other.ptr_ = nullptr;
	other.cnt_ = nullptr;
}

template <class T>
WeakPtr<T>::WeakPtr(const SharedPtr<T>& other) : ptr_(other.Get()), cnt_(other.cnt_) {
	if (ptr_ != nullptr && cnt_ != nullptr) {
		++cnt_->weak_cnt_;
	}
}

template <class T>
WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr& other) {
	if (this == &other) {
		return *this;
	}
	Reset();
	ptr_ = other.ptr_;
	cnt_ = other.cnt_;
	++cnt_->weak_cnt_;
	return *this;
}

template <class T>
WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr&& other) noexcept {
	Reset();
	ptr_ = other.ptr_;
	cnt_ = other.cnt_;
	other.cnt_ = nullptr;
	other.ptr_ = nullptr;
	return *this;
}

template <class T>
WeakPtr<T>::~WeakPtr() {
	Reset();
}

template <class T>
bool WeakPtr<T>::Expired() const {
	return cnt_ == nullptr ? true : cnt_->shared_cnt_ == 0;
}

template <class T>
size_t WeakPtr<T>::UseCount() const {
	return cnt_ == nullptr ? 0 : cnt_->shared_cnt_;
}

template <class T>
void WeakPtr<T>::Swap(WeakPtr<T>& other) {
	std::swap(ptr_, other.ptr_);
	std::swap(cnt_, other.cnt_);
}

template <class T>
void WeakPtr<T>::Reset() {
	if (cnt_ != nullptr && cnt_->weak_cnt_ == 1 && cnt_->shared_cnt_ == 0) {
		delete cnt_;
	} else if (cnt_ != nullptr && cnt_->weak_cnt_ != 0) {
		--cnt_->weak_cnt_;
	}
	ptr_ = nullptr;
	cnt_ = nullptr;
}

template <class T>
SharedPtr<T> WeakPtr<T>::Lock() const {
	return Expired() ? SharedPtr<T>() : SharedPtr<T>(*this);
}

template <class T>
T* WeakPtr<T>::Get() const {
	return ptr_;
}

#endif
