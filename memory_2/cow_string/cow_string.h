#include <cstddef>

using namespace std;

struct State{
    int ref_count = 0;
    size_t size_;
    size_t capacity_;
    char* data_ = nullptr;
};

class CowString {
private:
    State* uk;

public:
    // constrcutors
    CowString () : uk(new State{.ref_count = 1, .size_ = 0, .capacity_ = 2, .data_ = new char[2]}) {}

    CowString(const CowString& other) : uk(other.uk) {
        ++uk->ref_count;
    }

    ~CowString() {
        --uk->ref_count;
        if (uk->ref_count == 0 && uk->data_ != nullptr) {
            delete[] uk->data_;
            uk->data_ = nullptr;
            delete uk;
        }  
    }

    // copy operator=
    CowString operator =(const CowString& other) {
        if (this == &other) {
            return *this;
        }
        if (uk != nullptr) {
            --uk->ref_count;
            if (uk->ref_count == 0) {
                delete[] uk->data_;
                uk->data_ = nullptr;
            }
        }
        uk = other.uk;
        ++uk->ref_count;
        return *this;
    }

    const char& At(size_t index) const {
        return (uk->data_)[index];
    }

    char& operator[](size_t index) {
        if (uk->ref_count > 1) {
            State* ns = new State{.ref_count = 1, .size_ = uk->size_, .capacity_ = uk->capacity_, .data_ = new char[uk->capacity_]};
            --uk->ref_count;
            for (size_t i = 0; i < uk->size_; i++)
                ns->data_[i] = uk->data_[i];
            uk = ns;
        }
        return (uk->data_)[index];
    }

    const char& Back() const {
        return (uk->data_)[uk->size_ - 1];
    }

    void PushBack(char c) {
        if (uk->ref_count > 1) {
            State* ns = new State{.ref_count = 1, .size_ = uk->size_, .capacity_ = uk->capacity_, .data_ = new char[uk->capacity_]};
            --uk->ref_count;
            for (size_t i = 0; i < uk->size_; i++)
                ns->data_[i] = uk->data_[i];
            uk = ns;
        }
        if (uk->size_ >= uk->capacity_) 
            Resize(uk->size_);
        uk->data_[uk->size_] = c;
        ++uk->size_;
    }

    size_t Size() const {
        return uk->size_;
    }
    size_t Capacity() const {
        return uk->capacity_;
    }

    void Reserve(size_t capacity) {
        uk->capacity_ = capacity;
        char* new_data = new char[uk->capacity_];
        //copy(uk->data_, uk->data_+uk->size_, new_data);
        for (size_t i = 0; i < uk->size_; i++) {
            new_data[i] = uk->data_[i];
        }
        delete[] uk->data_;
        uk->data_ = new_data;
    }

    void Resize(size_t size) {
        if (size < uk->size_) {
            uk->size_ = size;
        }
        else {
            uk->capacity_ = size*2;
            char* new_data = new char[uk->capacity_];
            //copy(uk->data_, uk->data_+uk->size_, new_data);
            for (size_t i = 0; i < uk->size_; i++) {
                new_data[i] = uk->data_[i];
            }
            uk->size_ = size;
            delete[] uk->data_;
            uk->data_ = new_data;
        }
    }
};
