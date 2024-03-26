#include <iostream>

class Array {
public:
    Array(std::ostream& ostream)
        : Ostream_(ostream)
        , size_(0)
        , capacity_(2) 
        , data_(new int[2])
    {
        Ostream_ << "Constructed. " << *this << std::endl;
    }

    Array(const Array& array)
        : Ostream_(array.Ostream_)
        , size_(array.size_)
        , capacity_(array.capacity_) 
        , data_(new int[array.capacity_])
    {
        CopyData(array.data_, data_);
        Ostream_ << "Constructed from another Array. " << *this << std::endl;
    }

    Array(size_t size, std::ostream& ostream = std::cout, int defaultValue = 0)
        : Ostream_(ostream)
        , size_(size)
        , capacity_(size_*2) 
        , data_(new int[size_*2])
    {
        for (size_t i = 0; i < size_; i++)
            data_[i] = defaultValue;
        Ostream_ << "Constructed with default. " << *this << std::endl;
    }
    ~Array() {
        delete[] data_;
        Ostream_ << "Destructed " << size_ << std::endl;
    }

    size_t Size() const {
        return size_;
    }
    size_t Capacity() const {
        return capacity_;
    }
    void Reserve(size_t newCapacity) {
        if (newCapacity > capacity_) {
            capacity_ = newCapacity;
        }
    }
    
    void Resize(size_t newSize) {
        if (newSize <= size_) {
            size_ = newSize;
            int *newdata = new int[capacity_];
            CopyData(data_, newdata);
            delete[] data_;
            data_ = newdata;
            return;
        }
        capacity_ = newSize;
        int *newdata = new int[capacity_];
        CopyData(data_, newdata);
        for (size_t i = size_; i < capacity_; i++)
            newdata[i] = 0;
        delete[] data_;
        data_ = newdata;
        size_ = newSize;
    }

    void PushBack(int value = 0) {
        if (size_ == capacity_) {
            capacity_ *= 2;
            int *newData = new int[capacity_];
            CopyData(data_, newData);
            delete[] data_;
            data_ = newData;
        }
        data_[size_] = value;
        size_++;
        

    }

    void PopBack() {
        if (size_ > 0)
            size_--;
    }

    const int& operator [](const size_t i) const {
        return data_[i];
    }

    int& operator [](const size_t i) {
        return data_[i];
    }

    explicit operator bool() const {
        if (size_ == 0) return false;
        return true;
    }

    bool operator <(const Array& it) const {
        for (size_t i = 0; i < std::min(size_, it.size_); i++) {
            if (data_[i] < it.data_[i])
                return true;
            else if (data_[i] > it.data_[i])
                return false;
        }
        return false;
    }

    bool operator >(const Array& it) const {
        return !(*this < it);
    }

    bool operator !=(const Array& it) const {
        return (*this < it) || !(*this < it);
    }
    
    bool operator ==(const Array& it) const {
        return !((*this < it) && !(*this < it));
    }

    bool operator <=(const Array& it) const {
        return (*this < it) || !((*this < it) && !(*this < it));
    }

    bool operator >=(const Array& it) const {
        return !(*this < it) || !((*this < it) && !(*this < it));
    }

    Array& operator <<(const int value) {
        PushBack(value);
        return *this;
    }
    Array& operator <<(const Array& it) {
        size_t newsize = size_ + it.size_;
        size_t newcapacity = newsize*2;
        int *newdata = new int[newcapacity];
        CopyData(data_, newdata);
        for (size_t i = size_; i < newsize; i++) {
            newdata[i] = it.data_[i-size_];
        }
        
        delete[] data_;
        size_ = newsize;
        capacity_ = newcapacity;
        this->data_ = newdata;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& ostream, const Array& array);

private:
    std::ostream& Ostream_;
    size_t size_;
    size_t capacity_;
    int* data_;
    
    void CopyData(int* source, int* destination) {
        for (size_t i = 0; i < size_; i++)
            destination[i] = source[i];
    }
};

std::ostream& operator <<(std::ostream& ostream, const Array& array) {
    ostream << "Result Array's capacity is " << array.capacity_ << " and size is " << array.size_;
    if (array.size_ > 0) {
        ostream << ", elements are: ";
        for (size_t i = 0; i < array.size_; i++) {
            if (i == array.size_ - 1)
                ostream << array.data_[i];
            else
                ostream << array.data_[i] << ", ";
        }
    }
    //ostream << "\n";
    return ostream;
}
