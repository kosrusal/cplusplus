#pragma once
#include <iterator>

namespace ranges {

// Определяет тип данных, на которые ссылается итератор
template <typename Iterator>
using IteratorValueType = typename std::iterator_traits<Iterator>::value_type;

// Определяет тип возвращаемого функцией значения F(Args...) -> Result
template <typename F, typename... Args>
using FunctionResultType = std::invoke_result_t<F, Args...>;

// Your code here
template <typename T>
class View {
public: 
    using iterator = forward_iterator_tag;
    using difference_type = int;
    using value_type = int ;
    using pointer = int*;
    using reference = int&;

    View (const T& t) : mbegin(t.begin()), mend(t.end()) {}

    View () : mbegin(nullptr), mend(nullptr) {}

    iterator begin(
        return mbeg;
    );
    
    iterator end(
        return mend;
    );

    View operator |(adaptor);
private:
    iterator mbeg, mend;
};

template<typename T>
View Empty() {
    return View<T>();
}

}  // namespace ranges
