#pragma once
#include <algorithm>
#include <typeinfo>
#include <iostream>


class Any
{
public:
    Any () : held_(new holder<int>(-1000)) {
        ++held_->col;
    }

    Any(const Any& t) : held_(t.held_) {
        ++held_->col;
    }

    Any(Any&& t) : held_(t.held_) {
        t.held_ = new holder<int>(-1000);
        ++t.held_->col;
    }

	template<typename T>
	Any(const T& t) : held_(new holder<T>(t)){
        ++held_->col;
    }

    template<typename T>
    Any(const T&& t) : held_(new holder<T>(t)) {
        ++held_->col;
    }

	~Any(){ 
        --held_->col;
        if (held_->col == 0)
            delete held_;
        held_ = NULL;
    }

	template<typename U>
	U Value() const
	{
		if(typeid(U) != held_->type_info())
			throw std::bad_cast();
		return static_cast<holder<U>* >(held_)->t_;
	}

    bool Empty() {
        try {
            return (this->Value<int>() == -1000);
        } catch (std::exception&) {
            return 0;
        }
    }

    void Reset() {
        // if (held_->col == 1)
        //     delete held_;
        --held_->col;
        if (held_->col == 0)
            delete held_;
        held_ = new holder<int>(-1000);
        ++held_->col;
    }

    void Swap(Any& other) {
        std::swap(held_, other.held_);
    }

    Any* operator =(Any&& other) {
        if (held_->col == 1)
            delete held_;
        else
            --held_->col;
        held_ = new holder<int>(-1000);
        ++held_->col;
        std::swap(held_, other.held_);
        //if (other.held_->col == 1)
        return this;
    }

    Any& operator =(const Any& other) {
        if (this == &other)
            return *this;
        --held_->col;
        if (held_->col == 0)
            delete held_;
        held_ = other.held_;
        ++held_->col;
        return *this;
    }

    template<typename T>
    Any* operator =(const T&& t) {
        
        delete held_;
        
        held_ = new holder<T>(t);
        ++held_->col;
        return this;
    }

    template<typename T>
    Any& operator =(const T& t) {
        delete held_;

        held_ = new holder<T>(t);
        ++held_->col;
        //++held_->col;
        return *this;
    }



private:
	struct base_holder
	{
		virtual ~base_holder(){}
		virtual const std::type_info& type_info() const = 0;
        size_t col = 0;
	};
	
	template<typename T> struct holder : base_holder
	{
		explicit holder(const T& t) : t_(t){}
		const std::type_info& type_info() const
		{
			return typeid(t_);
		}
		T t_;
	};
private:
	base_holder* held_ = NULL;
};
