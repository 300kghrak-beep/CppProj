#include <iostream>

template <class T>
class UniquePtr {
    T* ptr;
public:
    explicit UniquePtr(T* ptr = nullptr) : ptr(ptr) {};
    UniquePtr(UniquePtr&& other) noexcept: ptr(other.ptr) {
        other.ptr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this == &other) return *this;
        delete ptr;
        ptr = other.ptr;
        other.ptr = nullptr;
        return *this;
    }


    T& operator*() noexcept {
        return *ptr;
    }

    const T& operator*() const noexcept {
        return *ptr;
    }

    T* operator->() noexcept {
        return ptr;
    }
    const T* operator->() const noexcept  {
        return ptr;
    }

    explicit operator bool() const noexcept {
        return ptr != nullptr;
    }

    void swap(UniquePtr& other) noexcept {
        T* tmp = ptr;
        ptr = other.ptr;
        other.ptr = tmp;
    }

    T* release() noexcept {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    void reset(T* other = nullptr) noexcept {
        if (ptr != other) {
            delete ptr;
            ptr = other;
        }
    }

    T* get() const noexcept {
        return ptr;
    }


    UniquePtr(const UniquePtr& other) = delete;
    UniquePtr& operator=(const UniquePtr& other) = delete;

    ~UniquePtr() noexcept { delete ptr; }

};

