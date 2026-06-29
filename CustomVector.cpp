#include <iostream>
#define ushort unsigned short
#define uint unsigned int


enum class FreeMemoryType {Division, Minus, Once, Fibonaci, Base, Full};
enum class AllocState { Multivision, Addition, everyTime, Fibonaci, Base };


std::ostream& operator<<(std::ostream& os, const FreeMemoryType type) {
    switch (type) {
        case FreeMemoryType::Division: return os << "Division";
        case FreeMemoryType::Minus: return os << "Minus";
        case FreeMemoryType::Once: return os << "Once";
        case FreeMemoryType::Fibonaci: return os << "Fibonaci";
        case FreeMemoryType::Base: return os << "Base";
        case FreeMemoryType::Full: return os << "Full";
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const AllocState state) {
    switch (state) {
        case AllocState::Multivision: return os << "Multivision";
        case AllocState::Addition: return os << "Addition";
        case AllocState::everyTime: return os << "EveryTime";
        case AllocState::Fibonaci: return os << "Fibonaci";
        case AllocState::Base: return os << "Base";
    }
    return os;
}

class AllocatedMemory {
protected:
    uint multivision;
    uint plus;
    AllocState allocState;
public:
    AllocatedMemory(): multivision(2), plus(0), allocState(AllocState::Base) {}
    AllocatedMemory(const uint mult, const uint plus_) {
        multivision = mult;
        plus = plus_;
        allocState = AllocState::Base;
    }

    [[nodiscard]] uint getMultivision() const {
        return multivision;
    }
    [[nodiscard]] uint getPlus() const {
        return plus;
    }

    void setMultivision(const uint mult) {
        if (this->multivision != mult && mult >= 2) this->multivision = mult;
    }

    void setPlus(const uint plus) {
        if (this->plus != plus && plus >= 2) this->plus = plus;
    }

    void addCapacity(uint& capacity, uint& oldCapacity) {
        switch (allocState) {
            case AllocState::Multivision:
                if (capacity == 0) {
                    capacity = 1;
                    break;
                }
                if (multivision <= 0) {
                    allocState = AllocState::Base;
                    break;
                }
                capacity *= multivision;
                break;
            case AllocState::Addition:
                capacity += plus;
                break;
            case AllocState::everyTime:
                capacity++;
                break;
            case AllocState::Fibonaci:
                if (capacity == 0) {
                    capacity = 1;
                    oldCapacity = 1;
                }
                else {
                    capacity += oldCapacity;
                }
                break;
            case AllocState::Base:
                if (capacity > 0) {
                    capacity *= 2;
                    break;
                }
                capacity = 1;
                break;
        }
    }

    [[nodiscard]] AllocState getAllocState() const {
        return allocState;
    }

    void setAllocState(const AllocState& allocState) {
        this->allocState = allocState;
    }
};


class FreeMemory {
    uint division;
    uint minus;
    FreeMemoryType freeType;
public:
    FreeMemory(): division(2), minus(0), freeType(FreeMemoryType::Base){}
    FreeMemory(const uint division, const uint minus): division(division), minus(minus), freeType(FreeMemoryType::Base) {}

    [[nodiscard]] uint getDivision() const { return division; }
    [[nodiscard]] uint getMinus() const { return minus; }

    void setDivision(const uint division) {  if   (this->division != division && division >= 2)   this->division = division; }
    void setMinus(const uint minus) {  if   (this->minus != minus && minus >= 2)   this->minus = minus; }

    void removeCapacity(uint& capacity, uint& oldcapacity, const uint& length) const {
        if (capacity == length) return;
        const uint temp = capacity;
        switch (freeType) {
            case FreeMemoryType::Division:
                if (capacity / division >= length) {
                    capacity /= division;
                    break;
                }
                break;
            case FreeMemoryType::Minus:
                if (capacity - minus >= length) {
                    capacity -= minus;
                    break;
                }
                break;
            case FreeMemoryType::Once:
                if (capacity - 1 >= length) {
                    capacity--;
                    break;
                }
                break;
            case FreeMemoryType::Fibonaci:
                if (capacity - oldcapacity >= length) {
                    capacity -= oldcapacity;
                    break;
                }
                break;
            case FreeMemoryType::Full:
                capacity -= capacity - length;
                break;
            case FreeMemoryType::Base:
                if (capacity / 2 >= length) {
                    capacity /= 2;
                    break;
                }
                break;
            default:
                break;
        }
        oldcapacity = temp;
    }

    [[nodiscard]] FreeMemoryType getFreeMemoryType() const {
        return freeType;
    }

    void setFreeMemoryType(const FreeMemoryType& freeType) {
        this->freeType = freeType;
    }
};


enum class RemoveMode { First, All, Last };
template <typename T>
class AdvancedVector {
    void moveData(const ushort& idx) {
        for (int i = idx; i < length - 1; i++) {
            data[i] = data[i + 1];
        }
    }

    void find(const T& value, ushort& index) {
        for (int i = 0; i < length; i++) {
            if (data[i] == value) {
                index = i;
                break;
            }
        }
    }

    void realloc() {
        T* newVector = new T[capacity];

        for (int i = 0; i < length; i++) {
            newVector[i] = data[i];
        }

        delete[] data;
        data = newVector;
    }

    T *data;
    AllocatedMemory& allocator;
    FreeMemory& free;
protected:
        uint oldcapacity;
        uint capacity;
        uint length;
public:
    AdvancedVector(AllocatedMemory& alloc, FreeMemory& fre)
        : data(nullptr),
          allocator(alloc),
          free(fre),
          oldcapacity(0),
          capacity(0),
          length(0)
    {}

    explicit AdvancedVector(const int StartCapacity, AllocatedMemory& alloc, FreeMemory& fre): allocator(alloc), free(fre) {
        oldcapacity = 0;
        capacity = StartCapacity;
        length = 0;
        data = new T[capacity];
    }

    AdvancedVector(const AdvancedVector& vector, AllocatedMemory& alloc, FreeMemory& fre):  allocator(alloc), free(fre) {
        capacity = vector.capacity;
        length = vector.length;
        oldcapacity = vector.oldcapacity;
        data = new T[capacity];
        for (int i = 0; i < length; i++) {
            data[i] = vector.data[i];
        }
        allocator.setAllocState(vector.allocator.getAllocState());
        free.setFreeMemoryType(vector.free.getFreeMemoryType());
    }

    AdvancedVector(AdvancedVector&& vector,  AllocatedMemory& alloc, FreeMemory& fre) noexcept: allocator(alloc), free(fre)
    {
        capacity = vector.capacity;
        length = vector.length;
        data = vector.data;
        oldcapacity = vector.oldcapacity;
        vector.data = nullptr;
        vector.capacity = 0;
        vector.length = 0;
        vector.oldcapacity = 0;
    }

    AdvancedVector& operator=(AdvancedVector&& vector) noexcept {
        if (this == &vector) return *this;
        delete[] data;
        capacity = vector.capacity;
        length = vector.length;
        data = vector.data;
        oldcapacity = vector.oldcapacity;
        allocator.setAllocState(vector.allocator.getAllocState());
        free.setFreeMemoryType(vector.free.getFreeMemoryType());

        vector.data = nullptr;
        vector.capacity = 0;
        vector.length = 0;
        vector.oldcapacity = 0;

        return *this;
    }

    AdvancedVector& operator=(const AdvancedVector& vector) {
        if (this == &vector) return *this;
        capacity = vector.capacity;
        length = vector.length;
        oldcapacity = vector.oldcapacity;


        T* newData = new T[capacity];
        for (int i = 0; i < length; i++) {
            newData[i] = vector.data[i];
        }

        delete[] data;
        data = newData;
        return *this;
    }

    void push_back(const T& value) {
        const uint temp = capacity;
        if (capacity == length) {
            allocator.addCapacity(capacity, oldcapacity);
            realloc();
        }
        oldcapacity = temp;
        data[length] = value;
        length++;
    }

    void remove(const T& value, const RemoveMode& removeMode) {
        bool found = false;
        for (int i = 0; i < length; i++) {
            if (data[i] == value) {
                found = true;
                break;
            }
        }
        if (!found) return;
        switch (removeMode) {
            case RemoveMode::First: {
                ushort index = 0;
                find(value, index);
                moveData(index);
                length--;
                break;
            }
            case RemoveMode::All: {
                ushort count = 0;
                for (int i = 0; i < length; i++) {
                    if (data[i] == value) {
                        count++;
                    }
                }
                for (ushort i = 0; i < count; i++) {
                    ushort index = 0;
                    find(value, index);
                    moveData(index);
                    length--;
                }
                break;
            }
            case RemoveMode::Last: {
                int index = -1;
                for (int i = length - 1; i >= 0; i--) {
                    if (data[i] == value) {
                        index = i;
                        break;
                    }
                }
                if (index != -1) moveData(index);
                length--;
                break;
            }
        }
    }

    void pop() {
        if (length > 0) length--;
    }

    void erase(const uint index) {
        if (index >= length) return;
        moveData(index);
        length--;
    }

    void append(const AdvancedVector& vector) {
        while (length + vector.length > capacity) {
            const uint temp = capacity;
            allocator.addCapacity(capacity, oldcapacity);
            realloc();
            oldcapacity = temp;
        }
        for (int i = 0; i < vector.length; i++) {
            data[length + i] = vector.data[i];
        }
        length += vector.length;
    }

    void insert(const uint index, const T& value) {
        if (index >= length) throw std::out_of_range("index out of range");

        if (capacity == length) {
            const uint temp = capacity;
            allocator.addCapacity(capacity, oldcapacity);
            realloc();
            oldcapacity = temp;
        }

        for (int i = length; i >= index; i--) {
            if (i == 0) continue;
            data[i] = data[i - 1];
        }

        data[index] = value;
        length++;
    }

    T& at(uint index) {
        if (index >= length) throw std::out_of_range("index out of range");
        return data[index];
    }

    [[nodiscard]] bool indexOf(const T& value, uint& index) const {
        if (length <= 0) return false;
        for (int i = 0; i < length; i++) {
            if (data[i] == value) {
                index = i;
                return true;
            }
        }
        return false;
    }


    T& operator[](const uint index) {
        return data[index];
    }

    const T& operator[](const uint index) const {
        return data[index];
    }

    bool operator==(const AdvancedVector& vector) const {
        if (capacity != vector.capacity) return false;
        if (length != vector.length) return false;
        for (int i = 0; i < length; i++) {
            if (data[i] != vector.data[i]) return false;
        }
        return true;
    }

    bool operator!=(const AdvancedVector& vector) const {
        if (capacity == vector.capacity) return true;
        if (length == vector.length) return true;
        for (int i = 0; i < length; i++) {
            if (data[i] == vector.data[i]) return true;
        }
        return false;
    }

    bool operator<(const AdvancedVector& vector) const {
        if (capacity < vector.capacity) return true;
        if (length < vector.length) return true;
        return false;
    }

    bool operator>(const AdvancedVector& vector) const {
        if (capacity > vector.capacity) return true;
        if (length > vector.length) return true;
        return false;
    }

    AdvancedVector& operator+=(const AdvancedVector& vector) {
        while (length + vector.length > capacity) {
            const uint temp = capacity;
            allocator.addCapacity(capacity, oldcapacity);
            realloc();
            oldcapacity = temp;
        }
        for (int i = 0; i < length; i++) {
            data[length + i] += vector.data[i];
        }
        length += vector.length;
        return *this;
    }

    [[nodiscard]] int Length() const {return length;}
    [[nodiscard]] int Capacity() const {return capacity;}
    [[nodiscard]] int oldCapacity() const {return oldcapacity;}
    [[nodiscard]] bool empty() const {return length == 0;}
    [[nodiscard]] bool full() const {
        if (capacity == 0) return true;
        return capacity == length;
    };

    void clear() {
        delete[] data;
        data = nullptr;
        capacity = 0;
        oldcapacity = 0;
        length = 0;
    }

    void setMemoryFreeState(const FreeMemoryType& type) const { free.setFreeMemoryType(type); }
    void setFreeDivision(const uint division) const { free.setDivision(division); }
    void setFreeMinus(const uint minus) const { free.setMinus(minus); }

    [[nodiscard]] uint getFreeDivision() const { return free.getDivision(); }
    [[nodiscard]] uint getFreeMinus() const { return free.getMinus(); }
    [[nodiscard]] FreeMemoryType getMemoryFreeState() const { return free.getFreeMemoryType();}

    void setMemoryAddState(const AllocState state) const { allocator.setAllocState(state); }
    void setMemoryAddMultivision(const uint mult) const {allocator.setMultivision(mult); }
    void setMemoryAddPlus(const uint plus) const {allocator.setPlus(plus); }

    [[nodiscard]] uint getFreeMultivision() const { return allocator.getMultivision(); }
    [[nodiscard]] uint getFreePlus() const { return allocator.getPlus(); }
    [[nodiscard]] AllocState getMemoryAddState() const { return allocator.getAllocState();}


    void freeMemory() {
        free.removeCapacity(capacity, oldcapacity, length);
        if (capacity < length) capacity = length;
        realloc();
    }

    void addMemory() {
        allocator.addCapacity(capacity, oldcapacity);
        realloc();
    }

    ~AdvancedVector() {
        delete[] data;
    }
};
