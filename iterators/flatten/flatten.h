#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

template <class T>
class FlattenedVector {
    // Your code here
private:
    vector<vector<T>>& it_;
    vector<int> sizes_;
public:
    class iterator {
    private:
        size_t t_id_;
        size_t vect_id_;
        const FlattenedVector<T>* q_;
    public:
        using iterator_category = random_access_iterator_tag;
        using difference_type = int;
        using value_type = int ;
        using pointer = int*;
        using reference = int&;

        iterator(int t_id, int vect_id, const FlattenedVector<T>* q) : t_id_(t_id), vect_id_(vect_id), q_(q) {
            while (vect_id_ < q_->it_.size() && q_->it_[vect_id_].size() == 0)
                ++vect_id_; 
        }

        iterator(const iterator& other) : t_id_(other.t_id_), vect_id_(other.vect_id_), q_(other.q_) {
            while (vect_id_ < q_->it_.size() && q_->it_[vect_id_].size() == 0)
                ++vect_id_; 
        }

        T& operator *() const {
            return (q_->it_)[vect_id_][t_id_];
        }

        T& operator [](size_t v) const {
            value_type n = (q_->sizes_[vect_id_] + t_id_);
            size_t l = upper_bound(q_->sizes_.begin(), q_->sizes_.end(), v+n)-(q_->sizes_.begin())-1;
            while ( l > 0 && q_->it_[l].size() == 0)
                --l;
            return (q_->it_)[l][v+n-(q_->sizes_[l])];
        }

        iterator operator ++() {
            if (t_id_ == q_->it_[vect_id_].size()-1 && vect_id_ != q_->it_.size()) {
                t_id_ = 0;
                ++vect_id_;
                if (vect_id_ < q_->it_.size() && q_->it_[vect_id_].size()==0) {
                    size_t l = upper_bound(q_->sizes_.begin(), q_->sizes_.end(), q_->sizes_[vect_id_]) - q_->sizes_.begin()-1;
                    vect_id_ = l;
                }
            }
            else    
                ++t_id_;
            return *this;
        }

        iterator operator ++(int) {

        }

        difference_type operator -(const iterator& other) const{

            return (q_->sizes_[vect_id_]+t_id_) - (q_->sizes_[other.vect_id_]+other.t_id_);
        }

        //Возможен случай уменьшения из нулевого подвектора
        iterator operator --() {
            if (t_id_ == 0 && vect_id_ != 0) {
                --vect_id_;
                if (vect_id_ > 0 && q_->it_[vect_id_].size() == 0) {
                    size_t l = upper_bound(q_->sizes_.begin(), q_->sizes_.end(), q_->sizes_[vect_id_]-1) - q_->sizes_.begin()-1;
                    vect_id_ = l;
                }
                t_id_ = q_->it_[vect_id_].size()-1;
                
            }
            else    
                --t_id_;
            return *this;
        }

        iterator operator --(int) {

        }

        iterator operator +(value_type v) const {
            value_type n = (q_->sizes_[vect_id_] + t_id_);
            if (v+n <= q_->sizes_[q_->sizes_.size()-1]) {
                size_t l = upper_bound(q_->sizes_.begin(), q_->sizes_.end(), n+v) - q_->sizes_.begin()-1;
                while (l > 0 && q_->it_[l].size()==0)
                    --l;
                return iterator(n+v-q_->sizes_[l], l, q_);
            }
            else {
                return iterator(0, q_->it_.size(), q_);
            }

        }

        iterator operator -(difference_type v) const {
            value_type n = (q_->sizes_[vect_id_] + t_id_);
            if (v <= n) {
                size_t l = upper_bound(q_->sizes_.begin(), q_->sizes_.end(), n-v) - q_->sizes_.begin()-1;
                while (l > 0 && q_->it_[l].size() == 0)
                    ++l;
                return iterator(n-v-q_->sizes_[l], l, q_);
            }
            else {
                return iterator(0, 0, q_);
            }
        }

        iterator& operator +=(difference_type v) {
            value_type n = (q_->sizes_[vect_id_] + t_id_);
            if (v+n < q_->sizes_[q_->sizes_.size()-1]) {
                size_t l = upper_bound(q_->sizes_.begin(), q_->sizes_.end(), n+v) - q_->sizes_.begin()-1;
                while ( l < q_->it_.size() && q_->it_[l].size()==0)
                    --l;
                vect_id_ = l;
                t_id_ = n+v-q_->sizes_[l];
            }
            else {
                vect_id_ = q_->it_.size();
                t_id_ = 0;
            }
            return *this;
        }

        iterator& operator -=(difference_type v) {
            value_type n = (q_->sizes_[vect_id_] + t_id_);
            if (v <= n) {
                size_t l = upper_bound(q_->sizes_.begin(), q_->sizes_.end(), n-v) - q_->sizes_.begin()-1;
                while (l > 0 && q_->it_[l].size() == 0)
                    ++l;
                vect_id_ = l;
                t_id_ = n-v-q_->sizes_[l];
            }
            else {
                vect_id_ = 0;
                t_id_ = 0;
            }
            return *this;
        }


        bool operator ==(const iterator other) const {
            return (q_->sizes_[vect_id_] + t_id_) == (other.q_->sizes_[other.vect_id_] + other.t_id_);
        }
        bool operator !=(const iterator other) const {
            return (q_->sizes_[vect_id_] + t_id_) != (other.q_->sizes_[other.vect_id_] + other.t_id_);
        }
        bool operator >(const iterator other) const {
            return (q_->sizes_[vect_id_] + t_id_) > (other.q_->sizes_[other.vect_id_] + other.t_id_);
        }
        bool operator <(const iterator other) const {
            return (q_->sizes_[vect_id_] + t_id_) < (other.q_->sizes_[other.vect_id_] + other.t_id_);
        }
        bool operator <=(const iterator other) const {
            return (q_->sizes_[vect_id_] + t_id_) <= (other.q_->sizes_[other.vect_id_] + other.t_id_);
        }
        
    };

    friend FlattenedVector<T>::iterator operator +(int v, const FlattenedVector<T>::iterator& other) {
        return other+v;
    }

    FlattenedVector(vector<vector<T>>& q) 
        : it_(q)
    {
        (sizes_).push_back(0);
        
        for (size_t i = 0; i < q.size(); i++) {
            sizes_.push_back(sizes_[i] + q[i].size());
        }
    }




    iterator begin() const {
        return iterator(0, 0, this);
    }

    iterator end() const {
        return iterator (0, it_.size(), this);
    }

};

