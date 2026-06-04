#ifndef SMART_POINTERS_H
#define SMART_POINTERS_H

template<typename T>
class UniquePtr {
public:
	UniquePtr(T* ptr)
		: internal_ptr(ptr) {}

	// забраняваме копирането и присвояването от референция
	UniquePtr(const UniquePtr<T>& other) = delete;
	UniquePtr& operator=(const UniquePtr<T>& other) = delete;

	// имплементираме копиране и присвояване от RHS (right-hand side)
	// референция, т.е. обект, който може да стои само в дясната част
	// на присвояване
	UniquePtr(const UniquePtr<T>&& other);
	UniquePtr& operator=(const UniquePtr<T>&& other);

	void Reset(T* other_ptr = nullptr) {
		delete internal_ptr;
		internal_ptr = other_ptr;
	}
	T* Get() const {
		return internal_ptr;
	}

	// TODO: operator*, operator->

private:
	T* internal_ptr;
};

template <typename T>
UniquePtr<T>::UniquePtr(const UniquePtr<T>&& other) {
	Reset(other.Get());
}

template <typename T>
UniquePtr<T>& UniquePtr<T>::operator=(const UniquePtr<T>&& other) {
	Reset(other.Get());
	return *this;
}


#endif
