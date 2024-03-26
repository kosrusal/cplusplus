#pragma once

#include <mutex>
#include <condition_variable>
#include <optional>
#include <queue>

class TimeOut : public std::exception {
    const char* what() const noexcept override {
            return "Timeout";
    }
};

template<typename T>
class UnbufferedChannel {
public:
    void Put(const T& data) {
        std::unique_lock<std::mutex> lock(mtx);
        cv_for_write.wait(lock, [this](){return q.empty();});
        q.push(data);
        cv_for_read.notify_one();
        cv_thread.wait(lock);
        cv_for_write.notify_one();
    }

    T Get(std::chrono::milliseconds timeout = std::chrono::milliseconds(0)) {
        std::unique_lock<std::mutex> lock(mtx);
        if (cv_for_read.wait_until(lock, std::chrono::high_resolution_clock::now()+timeout, [this](){return !q.empty();})) {
            auto res = q.front();
            q.pop();
            cv_thread.notify_one();
            return res;
        } else throw TimeOut();
    }
private:
    std::queue<T> q;
    std::mutex mtx;
    std::condition_variable cv_for_read, cv_for_write, cv_thread;
};
