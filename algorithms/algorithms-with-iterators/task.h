#include <cstdlib>
#include <iterator>
#include <stdint.h>

/*
 * Нужно написать функцию, которая принимает на вход диапазон, применяет к каждому элементу данную операцию и затем складывает результат применения операции в новый диапазон
 * Входной диапазон задан итераторами [firstIn; lastIn)
 * Выходной диапазон начинается с firstOut и имеет такую же длину как входной диапазон
 * Операция является функцией с одним аргументом (унарная функция), возвращающая результат такого типа, который можно положить в OutputIt
 */

template<class InputIt, class OutputIt, class UnaryOperation>
void Transform(InputIt firstIn, InputIt lastIn, OutputIt firstOut, UnaryOperation op) {
    for (;firstIn < lastIn; ++firstIn) {
        *(firstOut) = op(*firstIn);
        ++firstOut;
    }
}

/*
 * Нужно написать функцию, которая принимает на вход диапазон и переставляет элементы в нем таким образом, чтобы элементы,
 * которые удовлетворяют условию p, находились ближе к началу диапазона, чем остальные элементы.
 * Входной диапазон задан итераторами [first; last)
 * p является функцией с одним аргументом (унарная функция), возвращающая результат типа bool
 */

template<class BidirIt, class UnaryPredicate>
void Partition(BidirIt first, BidirIt last, UnaryPredicate p) {
    //first = std::find_if_not(first, last, p);
    for (; first < last; ++first) {
        if (!p(*first))
            break;
    }
    if (first == last)
        return ;
    
    for (auto i = std::next(first); i != last; ++i) {
        if (p(*i)) {
            std::swap(*i, *first);
            ++first;
        }
    }
    
}

/*
 * Нужно написать функцию, которая принимает на вход два отстотированных диапазона и объединяет их в новый отсортированный диапазон, содержащий все элементы обоих входных диапазонов.
 */
template<class InputIt1, class InputIt2, class OutputIt>
void Merge(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt firstOut) {
    
    while (first1 < last1 || first2 < last2) {
        //std::cout << *first1 << " " << *first2 << std::endl;
        if (first2 == last2 || (first1 != last1 && *first1 < *first2)) {
            *firstOut = *first1;
            ++firstOut;
            ++first1;
        }
        else if (first1 == last1 || (first2 != last2 && *first1 > *first2)) {
            *firstOut = *first2;
            ++firstOut;
            ++first2;
        }
        else if (*first1 == *first2) {
            *firstOut = *first1;
            ++firstOut;
            ++first1;
            *firstOut = *first2;
            ++firstOut;
            ++first2;
        }
    }

}


/*
 * Напишите класс "диапазон чисел Фибоначчи"
 * Экземпляр класса представляет из себя диапазон от первого до N-го числа Фибоначчи (1, 2, 3, 5, 8, 13 и т.д.)
 * С помощью функций begin и end можно получить итераторы и пробежать по диапазону или передать их в STL-алгоритмы
 */
class FibonacciRange {
private:
    size_t n_;

public:

    class Iterator {
        friend class FibonacciRange;
    private:
        uint64_t prev_;
        uint64_t cur_;
        size_t n_;
    
    public:
        using value_type = uint64_t;
        using difference_type = ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::input_iterator_tag;

        Iterator (size_t amount) : prev_(1), cur_(1), n_(amount) 
        {
            //Посчитать prev_ и cur_
            size_t c = n_;
            while (c > 1) {
                std::swap(prev_, cur_);
                cur_ += prev_;
                c--;
            }
        }

        Iterator (uint64_t int1, uint64_t int2, size_t amount) : prev_(int1), cur_(int2), n_(amount) {}

        Iterator (const Iterator& q) : prev_(q.prev_), cur_(q.cur_), n_(q.n_) {}

        value_type operator *() const {
            // разыменование итератора -- доступ к значению
            return cur_;
        }

        Iterator& operator ++() {
            // prefix increment
            std::swap(prev_, cur_);
            cur_ += prev_;
            n_++;
            return *this;
        }

        Iterator operator ++(int) {
            // postfix incrementfirst1 == last1
            // if (n_ == 0 || n_ == 1) {
            //     return Iterator(prev_, cur_, n_++);
            // }
            uint64_t pr = prev_ + cur_;
            std::swap(prev_, cur_);
            std::swap(cur_, pr);
            ++n_;
            return Iterator(pr, prev_, n_-1);
        }

        bool operator ==(const Iterator& rhs) const {
            return n_ == rhs.n_;
        }

        bool operator !=(const Iterator& rhs) const {
            if (n_ != rhs.n_)
                return 1;
            return 0;
        }

        bool operator <(const Iterator& rhs) const {
            if (n_ < rhs.n_)
                return 1;
            return 0;
        }
    };

    FibonacciRange(size_t amount) : n_(amount) {}

    Iterator begin() const {
        return Iterator(0);
    }

    Iterator end() const {
        return Iterator(n_);
    }

    size_t size() const {
        return n_;
    }
};
