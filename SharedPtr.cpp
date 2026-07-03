#include <iostream>

template <typename T>
class SharedPtr {
private:
    T* ptr;
    int* counter;

    void remove() {
        if (counter) {
            if (*counter == 1) {
                delete ptr;
                delete counter;

                ptr = nullptr;
                counter = nullptr;
            }
            else {
                (*counter) -= 1;
                ptr = nullptr;
                counter = nullptr;
            }
        }
    }
public:
    SharedPtr(T* ptr): ptr(ptr) {
        if (ptr) {
            counter = new int(1);
        }
        else {
            counter = nullptr;
        }
    };

    SharedPtr(const SharedPtr& other) {
        this->ptr = other.ptr;
        counter = other.counter;
        if (counter) {
            (*counter) += 1;
        }
        else {
            counter = new int(0);
        }
    }

    SharedPtr(SharedPtr&& other) noexcept {
        ptr = other.ptr;
        counter = other.counter;

        other.ptr = nullptr;
        other.counter = nullptr;
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (this == &other) return *this;

        remove();

        ptr = other.ptr;
        counter = other.counter;
        if (counter) (*counter)++;
        else counter = new int(0);
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this == &other) return *this;

        remove();

        ptr = other.ptr;
        counter = other.counter;

        other.ptr = nullptr;
        other.counter = nullptr;
        return *this;
    }


    T& operator*() {
        return *ptr;
    }

    const T& operator*() const {
        return *ptr;
    }

    T* operator->() {
        return ptr;
    }

    const T* operator->() const {
        return ptr;
    }

    T* getPointer() {
        if (ptr) return ptr;
        return nullptr;
    }

    int use_count() const {
        if (counter) {
            return *counter;
        }
        return 0;
    }


    ~SharedPtr() {
        remove();
    }
};



int main() {
    SharedPtr<int> a(new int(1));
    SharedPtr<int> b(new int(2));

    b = a;

    std::cout << b.use_count() << std::endl;
    std::cout << a.use_count();

    return 0;
}