#include <iostream>
#include <algorithm>

template<typename T>
class Array {
public:
    Array(std::ostream& ostream) 
        : Ostream_(ostream)
        , size_(0)
        , capacity_(2) 
        , data_(new T[2])
    {}

    Array(const Array<T>& array) 
        : Ostream_(array.Ostream_)
        , size_(array.size_)
        , capacity_(array.capacity_) 
        , data_(new T[array.capacity_])
    {
        CopyData(array.data_, data_);
    }

    Array(size_t size, std::ostream& ostream = std::cout, T defaultValue = T()) 
        : Ostream_(ostream)
        , size_(size)
        , capacity_(size_*2) 
        , data_(new T[size_*2])
    {
        for (size_t i = 0; i < size_; i++)
            data_[i] = defaultValue;
    }

    ~Array() {
        delete[]  data_;
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
            T *newdata = new T[capacity_];
            CopyData(data_, newdata);
            delete[] data_;
            data_ = newdata;
            return;
        }
        capacity_ = newSize;
        T *newdata = new T[capacity_];
        CopyData(data_, newdata);
        for (size_t i = size_; i < capacity_; i++)
            newdata[i] = 0;
        delete[] data_;
        data_ = newdata;
        size_ = newSize;
    }

    void PushBack(T value = 0) {
        if (size_ == capacity_) {
            capacity_ *= 2;
            T *newData = new T[capacity_];
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

    const T& operator [](const size_t i) const {
        return data_[i];
    }

    T& operator [](const size_t i) {
        return data_[i];
    }

    explicit operator bool() const {
        if (size_ == 0) return false;
        return true;
    }

    bool operator <(const Array<T>& it) const {
        for (size_t i = 0; i < std::min(size_, it.size_); i++) {
            if (data_[i] < it.data_[i])
                return true;
            else if (data_[i] > it.data_[i])
                return false;
        }
        return false;
    }

    bool operator >(const Array<T>& it) const {
        return !(*this < it);
    }

    bool operator !=(const Array<T>& it) const {
        return (*this < it) || !(*this < it);
    }

    bool operator ==(const Array<T>& it) const {
        return !((*this < it) && !(*this < it));
    }

    bool operator <=(const Array<T>& it) const {
        return (*this < it) || !((*this < it) && !(*this < it));
    }

    bool operator >=(const Array<T>& it) const {
        return !(*this < it) || !((*this < it) && !(*this < it));
    }

    Array<T>& operator <<(const T& value) {
        PushBack(value);
        return *this;
    }

    Array<T>& operator << (const Array<T>& it)  {
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

    bool Insert(size_t pos, const T& value) {
        if (pos > size_)
            return false;
        if (size_ == capacity_) {
            capacity_ *= 2;
            T *newData = new T[capacity_];
            CopyData(data_, newData);
            delete[] data_;
            data_ = newData;
        }
        //data_[size_] = data_[0];
        for (size_t i = pos; i < size_; i++) {
                data_[i+1] = data_[i];
        }
        data_[pos] = value;
        size_++;
        return true;
    }

    bool Erase(size_t pos) {
        if (pos >= size_)
            return false;
        for (size_t i = pos+1; i < size_; i++) {
            data_[i-1] = data_[i];
        }
        size_--;
        return true;
    }


private:
    // ToDo
    std::ostream& Ostream_;
    size_t size_;
    size_t capacity_;
    T* data_;
    
    void CopyData(T* source, T* destination) {
        for (size_t i = 0; i < size_; i++)
            destination[i] = source[i];
    }
};

template<typename T>
std::ostream& operator <<(std::ostream& ostream, const Array<T>& array) {
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

