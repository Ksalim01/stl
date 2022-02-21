#ifndef ANY_H
#define ANY_H
#include <algorithm>
#include <memory>

class BadAnyCast : public std::exception {
    public:
	const char* what() const noexcept override {
		return "BadAnyCast";
	}
};

class Base {
public:
	virtual std::unique_ptr<Base> Clone() const = 0;
	virtual ~Base() = default;
};

template<class T>
class Derived : public Base {
	T value_;

public:
	Derived() = default;
	Derived(const T& other);
	Derived(T&& other);
	std::unique_ptr<Base> Clone() const override;
	T Get() const;
};

template <class T>
Derived<T>::Derived(const T& other) : value_(other) {
}

template <class T>
Derived<T>::Derived(T&& other) : value_(std::move(other)) {
}

template <class T>
T Derived<T>::Get() const {
	return value_;
}

template <class T>
std::unique_ptr<Base> Derived<T>::Clone() const {
	return std::make_unique<Derived<T>>(value_);
}

class Any {
	std::unique_ptr<Base> ptr_;

public:
	Any();
	Any(const Any& other);
	Any(Any&& other) noexcept = default;	
	template<class T>
	Any(const T& value);
	template<class T>
	Any& operator=(const T& value);
	Any& operator=(const Any& other);
	Any& operator=(Any&& other) = default;
	~Any() = default;

	void Swap(Any& other);
	void Reset();
	bool HasValue() const;
	template<class T>
	friend T any_cast(const Any& value);
};

inline Any::Any() : ptr_(nullptr) {
}

inline Any::Any(const Any& other) : ptr_(other.ptr_->Clone()) {
}

template <class T>
Any::Any(const T& value) : ptr_(std::make_unique<Derived<T>>(value)) {
}

template <class T>
Any& Any::operator=(const T& value) {
	ptr_ = std::make_unique<Derived<T>>(value);
	return *this;
}

inline Any& Any::operator=(const Any& other) {
	if (this == &other) {
		return *this;
	}
	ptr_ = other.ptr_->Clone();
	return *this;
}

inline void Any::Swap(Any& other) {
	std::swap(ptr_, other.ptr_);
}

inline bool Any::HasValue() const {
	return ptr_ != nullptr;
}

inline void Any::Reset() {
	ptr_ = nullptr;
}

template <class T>
T any_cast(const Any& value) {
	if (dynamic_cast<Derived<T>*>(value.ptr_.get()) != nullptr) {
		return dynamic_cast<Derived<T>*>(value.ptr_.get())->Get();
	}
	else {
		throw BadAnyCast();
	}
}

#endif // ANY_H
