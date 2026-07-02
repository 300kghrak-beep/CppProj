#include <iostream>
using ushort = unsigned short;
using uint = unsigned int;


template <class T>
class vector {
    T* data;
    uint size;
    uint capacity;

    void resize() {
        if (capacity == 0) {
            capacity = 1;
            data = static_cast<T*>(operator new(sizeof(T) * capacity));
            return;
        }

        T* temp = static_cast<T*>(operator new(sizeof(T) * (capacity * 2)));

        for (int i = 0; i < size; i++) {
            new (temp + i) T(std::move(data[i]));
            (data + i)->~T();
        }
        operator delete(data);

        data = temp;
        capacity *= 2;
    }
public:
    template <class U>
    class Iterator {
        U* ptr;
    public:
        explicit Iterator(U* ptr = nullptr) : ptr(ptr) {}

        U& operator*() {
            return *ptr;
        }

        const U& operator*() const {
            return *ptr;
        }

        Iterator& operator++() {
            ++ptr;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp(*this);
            ++ptr;
            return temp;
        }

        Iterator& operator--() {
            --ptr;
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp(*this);
            --ptr;
            return temp;
        }

        U* operator->() const {
            return ptr;
        }

        bool operator==(const Iterator& other) const {
            return ptr == other.ptr;
        }

        bool operator!=(const Iterator& other) const {
            return ptr != other.ptr;
        }


        Iterator& operator+=(std::ptrdiff_t n) {
            ptr += n;
            return *this;
        }

        Iterator& operator-=(std::ptrdiff_t n) {
            ptr -= n;
            return *this;
        }

        Iterator operator-(std::ptrdiff_t n) const {
            return Iterator(ptr - n);
        }

        Iterator operator+(std::ptrdiff_t n) const {
            return Iterator(ptr + n);
        }

        std::ptrdiff_t operator-(const Iterator& other) const {
            return ptr - other.ptr;
        }

        friend Iterator operator+(std::ptrdiff_t n, const Iterator& it) {
            return Iterator(it.ptr + n);
        }

        bool operator<(const Iterator& other) const {
            return ptr < other.ptr;
        }

        bool operator>(const Iterator& other) const {
            return ptr > other.ptr;
        }

        bool operator<=(const Iterator& other) const {
            return ptr <= other.ptr;
        }

        bool operator>=(const Iterator& other) const {
            return ptr >= other.ptr;
        }

        U& operator[](uint index) {
            return *(ptr + index);
        }

        const U& operator[](uint index) const {
            return *(ptr + index);
        }
    };

    using iterator = Iterator<T>;
    using const_iterator = Iterator<const T>;


    iterator begin() {
        return iterator(data);
    }

    iterator end() {
        return iterator(data + size);
    }

    const_iterator begin() const {
        return const_iterator(data);
    }

    const_iterator end() const {
        return const_iterator(data + size);
    }


    vector() {
        size = 0;
        capacity = 16;
        data = static_cast<T*>(operator new(sizeof(T) * capacity));
    }

    vector(std::initializer_list<T> list) {
        capacity = list.size();
        size = list.size();
        auto it = list.begin();

        data = static_cast<T*>(operator new(sizeof(T) * capacity));
        for (int i = 0; i < size; i++) {
            new (data + i) T(*(it++));
        }
    }

    explicit vector(uint capacity) {
        size = 0;
        this->capacity = capacity;
        data = static_cast<T*>(operator new(sizeof(T) * capacity));
    }

    vector(const vector& other) {
        size = other.size;
        capacity = other.capacity;
        data = static_cast<T*>(operator new(sizeof(T) * capacity));

        for (int i = 0; i < size; i++) {
            new (data + i) T(other.data[i]);
        }
    }

    vector(vector&& other) noexcept {
        size = other.size;
        capacity = other.capacity;
        data = other.data;

        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    vector& operator=(const vector& other) {
        if (this == &other) return *this;

        for (int i = 0; i < size; i++) {
            (data + i)->~T();
        }
        operator delete(data);

        size = other.size;
        capacity = other.capacity;
        data = static_cast<T*>(operator new(sizeof(T) * capacity));

        for (int i = 0; i < size; i++) {
            new (data + i) T(other.data[i]);
        }

        return *this;
    }

    vector& operator=(vector&& other) noexcept {
        if (this == &other) return *this;

        for (int i = 0; i < size; i++) {
            (data + i)->~T();
        }
        operator delete(data);

        size = other.size;
        capacity = other.capacity;
        data = other.data;

        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;

        return *this;
    }

    void push_back(const T& value) {
        if (size == capacity) resize();
        new (data + size) T(value);
        size++;
    }

    void push_back(T&& value) {
        if (size == capacity) resize();
        new (data + size) T(std::move(value));
        size++;
    }


    void pop_back() {
        if (size > 0) {
            size--;
            (data + size)->~T();
        }
    }

    void pop_index(uint index) {
        if (size > 0 && index < size) {
            for (uint i = index; i < size - 1; i++) {
                data[i] = std::move(data[i + 1]);
            }
            size--;
            (data + size)->~T();
        }
    }

    void remove(const T& value) {
        int idx = -1;
        for (int i = 0; i < size; i++) {
            if (data[i] == value) {
                idx = i;
                break;
            }
        }

        if (idx == -1) return;

        for (int i = idx; i < size - 1; i++) {
            data[i] = std::move(data[i + 1]);
        }

        size--;
        (data + size)->~T();
    }

    void insert(uint index, const T& value) {
        if (index > size) throw std::out_of_range("Index is out of range!");
        if (size == capacity) resize();
        if (size == 0) {
            new (data) T(value);
            size++;
            return;
        }

        new (data + size) T(std::move(*(data + size - 1)));

        for (int i = size - 1; i > index; i--) {
            data[i] = std::move(data[i - 1]);
        }

        data[index] = value;
        size++;
    }

    void erase(iterator pos_1) {
        uint index = pos_1 - begin();
        for (uint i = index; i < size - 1; i++) {
            data[i] = std::move(*(data + i + 1));
        }
        size--;
        (data + size)->~T();
    }

    void erase(iterator pos_1, iterator pos_2) {
        uint start = pos_1 - begin();
        uint end = pos_2 - begin();
        uint difference = end - start;

        if (start == end) return;
        if (start > end) throw std::out_of_range("Wrong placement of diaposone variables!");
        if (start > size || end > size) throw std::out_of_range("Wrong variables of diaposone!");

        if (difference == size) clear();

        for (int i = 0; i < size - difference; i++) {
            *(data + start + i) = std::move(data[i + end]);
        }

        for (int i = size - difference; i < size; i++) (data + i)->~T();
        size -= difference;
    }


    void cut(iterator pos_1, iterator pos_2) {
        if ((pos_1 - begin()) >= size || (pos_2 - begin()) > size || (pos_1 - begin()) > pos_2 - begin()) throw std::out_of_range("Wrong values set to iterators!");

        uint start = pos_1 - begin();
        uint end = pos_2 - begin();
        uint difference = end - start;

        for (int i = 0; i < size - difference - start; i++) {
            *(data + start + i) = std::move(*(data + end + i));
        }

        for (int i = size - difference; i < size; i++) (data + i)->~T();
        size -= difference;
    }

    void cutFromStart(iterator pos) {
        if (pos - begin() >= size) return;
        if ((pos - begin()) == size - 1) {
            clear();
            return;
        }

        uint end = pos - begin();

        for (int i = 0; i < size - end; i++) {
            *(data + i) = std::move(*(data + end + i));
        }

        for (uint i = end; i < size; i++) (data + i)->~T();
        size -= end;
    }

    void cutToEnd(iterator pos) {
        if (pos - begin() >= size - 1) return;

        uint start = pos - begin();

        for (uint i = start; i < size; i++) (data + i)->~T();
        size -= start;
    }


    void keepRange(iterator pos_1, iterator pos_2) {
        uint start = pos_1 - begin();
        uint end = pos_2 - begin();
        uint difference = end - start;

        if (start == end) return;
        if (start > end) throw std::out_of_range("Wrong placement of diaposone variables!");
        if (start > size || end > size) throw std::out_of_range("Wrong variables of diaposone!");


        for (int i = 0; i < difference; i++) {
            *(data + i) = std::move(*(data + start + i));
        }

        for (uint i = end; i < size; i++) (data + i)->~T();

        size = difference;
    }


    void keepRangeFromStart(iterator pos) {
        if ((pos - begin()) > size) return;

        uint end = pos - begin();

        for (uint i = end; i < size; i++) (data + i)->~T();

        size = end;
    }

    void keepRangeToEnd(iterator pos) {
        if ((pos - begin()) >= size) return;

        uint start = pos - begin();

        for (int i = 0; i < size - start; i++) {
            *(data + i) = std::move(*(data + start + i));
        }

        for (int i = size - start; i < size; i++) (data + i)->~T();
        size = size - start;
    }


    void append(const vector& other) {
        if (this == &other) return;

        while (size + other.size > capacity) {
            resize();
        }

        for (int i = 0; i < other.size; i++) {
            new (data + size + i) T(other.data[i]);
        }

        size += other.size;
    }


    void expand(uint toAdd) noexcept {
        T* temp = static_cast<T*>(operator new(sizeof(T) * (capacity + toAdd)));

        for (int i = 0; i < size; i++) {
            new (temp + i) T(std::move(*(data + i)));
            (data + i)->~T();
        }
        operator delete(data);

        data = temp;
        capacity += toAdd;
    }


    void reserve(uint newCapacity) noexcept {
        if (newCapacity <= capacity) return;

        T* newData = static_cast<T*>(operator new(sizeof(T) * newCapacity));
        for (uint i = 0; i < size; i++) {
            new (newData + i) T(std::move(data[i]));
            (data + i)->~T();
        }
        operator delete(data);

        data = newData;
        capacity = newCapacity;
    }

    void shrink(uint toRemove) noexcept {
        if (capacity - toRemove < size) return;

        T* temp = static_cast<T*>(operator new(sizeof(T) * (capacity - toRemove)));

        for (int i = 0; i < size; i++) {
            new (temp + i) T(std::move(*(data + i)));
            (data + i)->~T();
        }
        operator delete(data);

        data = temp;
        capacity -= toRemove;
    }


    void shrink_to_fit() noexcept {
        if (size == capacity) return;

        T* newData = static_cast<T*>(operator new(sizeof(T) * size));

        for (int i = 0; i < size; i++) {
            new (newData + i) T(std::move(*(data + i)));
            (data + i)->~T();
        }
        operator delete(data);

        data = newData;
        capacity = size;
    }

    void swap(vector& other) noexcept {
        T* tempData = data;
        uint capacityTemp = capacity;
        uint sizeTemp = size;

        data = other.data;
        size = other.size;
        capacity = other.capacity;

        other.data = tempData;
        other.capacity = capacityTemp;
        other.size = sizeTemp;
    }


    template <class... Args>
    void emplace_back(Args&&... args) {
        if (size == capacity) resize();
        new (data + size) T(std::forward<Args>(args)...);
        size++;
    }

    template <class... Args>
    void emplace(iterator pos, Args&&... args) {
        if (pos - begin() >= size) throw std::out_of_range("Wrong placement of diaposone!");
        uint index = pos - begin();
        if (size == capacity) resize();

        new (data + size) T(std::move(*(data + size - 1)));

        for (int i = size - 1; i > index; i--) {
            *(data + i) = std::move(*(data + i - 1));
        }

        (data +  index)->~T();

        new (data + index) T(std::forward<Args>(args)...);
        size++;
    }


    T& operator[](const uint index) noexcept {
        return data[index];
    }

    const T& operator[](const uint index) const noexcept {
        return data[index];
    }

    T& at(uint index) {
        if (index >= size) throw std::out_of_range("Index is out of range!");
        return data[index];
    }

    const T& at(uint index) const {
        if (index >= size) throw std::out_of_range("Index is out of range!");
        return data[index];
    }

    void clear() {
        for (int i = 0; i < size; i++) {
            (data + i)->~T();
        }
        size = 0;
    }

    ~vector() {
        for (int i = 0; i < size; i++) {
            (data + i)->~T();
        }
        operator delete(data);
    }
};