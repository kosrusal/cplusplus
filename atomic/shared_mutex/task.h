#pragma once

#include <atomic>
#include <memory>

class SharedMutex {
public:
    void lock() {
        if (count_ == 1 && count_shared > 0) {
            if (state_shared.exchange(1))
                state_shared.wait(1);
        }
        count_.fetch_add(1);
        state_shared.store(1);
        while (state_.exchange(1)) {
            state_.wait(1);
        }
    }

    void unlock() {
        state_.store(0);
        auto i = count_.fetch_sub(1);
        if (i > 1) {
            state_.notify_one();
        }
        if (i == 1) {
            state_shared.store(0);
            state_shared.notify_all() ;
        }
    }

    void lock_shared() {
        if (count_shared == 0 && count_ > 0)
            if (state_shared.exchange(1))
                state_shared.wait(1);

        if (count_shared.fetch_add(1) == 0) {
            //state_.store(1);
            lock();
        }
    }

    void unlock_shared() {
        if (count_shared.fetch_sub(1) == 1) {
            unlock();

        }
    }
private:
    std::atomic<int> state_{0};
    std::atomic<int> state_shared{0};
    std::atomic<int> count_{0};
    std::atomic<int> count_shared{0};
};
