#include <chrono>
#include <sstream>

class Timer {
private:
    std::chrono::seconds q_;
    std::chrono::time_point<std::chrono::steady_clock> now;



public:
    Timer (std::chrono::seconds q) : q_(q), now(std::chrono::steady_clock::now()) {}

    bool Expired() const {
        return std::chrono::steady_clock::now() -now >= q_;
    }
};

class TimeMeasurer {
private:
    std::ostream& q_;
    std::chrono::time_point<std::chrono::steady_clock> now;
public:
    TimeMeasurer(std::ostream& q) : q_(q), now(std::chrono::steady_clock::now()) {}

    ~TimeMeasurer() {
        std::chrono::milliseconds  ans = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - now);
        q_ << "Elapsed time: " << ans.count() << "ms" << "\n";
    }
};
