#pragma once

#include <iostream>
using namespace std;

class Range {
    // Your code here
public:

    class iterator {
    private:
        int64_t cur_;
        int64_t step_;

    public: 
        using iterator_category = forward_iterator_tag;
        using difference_type = int;
        using value_type = int;
        using pointer = int*;
        using reference = int&;

        iterator(int64_t start, int64_t step) 
            : cur_(start)
            , step_(step)
        {}

        iterator operator +() {
            cur_ += step_;
            return *this;
        }

        iterator operator ++() {
            cur_+=step_;
            return *this;
        }

        iterator operator ++(int) {
            iterator cur = iterator(cur_, step_);
            cur_ += step_;
            return cur;
        }

        int64_t operator *() {
            return cur_;
        }

        bool operator !=(const iterator& other) const {
            return cur_ != other.cur_;
        }

        bool operator ==(const iterator& other) const {
            return cur_ == other.cur_;
        }
    };

    Range(const int64_t end)
        : start_(0)
        , end_(end)
        , step_(1)
    {}

    Range(const int64_t start, const int64_t end)
        : start_(start)
        , end_(end)
        , step_(1)
    {}

    Range(const int64_t start, const int64_t end, const int64_t step)
        : start_(start)
        , end_(end)
        , step_(step)
    {
        int64_t a = (end_-start_)/step_;
        if ((end_-start_)%step_ != 0) a++; 
        end_ = start_ + step_*a;
    }

    int64_t Size() const {
        return (end_-start_)/step_;
    }

    iterator begin() const {
        return iterator(start_, step_);
    }

    iterator end() const {
        return iterator(end_, step_);
    }




private:
    int64_t start_;
    int64_t end_;
    int64_t step_;
};
