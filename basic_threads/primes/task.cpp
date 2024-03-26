#include "task.h"
#include <stdexcept>
#include <unordered_set>
#include <thread>

PrimeNumbersSet::PrimeNumbersSet()
        : nanoseconds_under_mutex_(0)
        , nanoseconds_waiting_mutex_(0)
{}

bool PrimeNumbersSet::IsPrime(uint64_t number) const {
    std::lock_guard<std::mutex> ul(set_mutex_);
    if (primes_.find((number)) !=  primes_.end())
        return true;
    return false;
}

uint64_t PrimeNumbersSet::GetNextPrime(uint64_t number) const {
    std::lock_guard<std::mutex> ul( set_mutex_);
    auto x = primes_.upper_bound(number);
    if (x == primes_.end()) throw std::invalid_argument("No");
    return *x;
}

size_t PrimeNumbersSet::GetPrimesCountInRange(uint64_t from, uint64_t to) const {
    std::lock_guard<std::mutex> ul(set_mutex_);
    return std::distance(primes_.lower_bound(from), primes_.lower_bound(to));
}

uint64_t PrimeNumbersSet::GetMaxPrimeNumber() const {
    std::lock_guard<std::mutex> ul(set_mutex_);
    if (primes_.size() == 0) return 0;
    return *(--primes_.end());
}

std::chrono::nanoseconds PrimeNumbersSet::GetTotalTimeWaitingForMutex() const {
    return std::chrono::nanoseconds(nanoseconds_waiting_mutex_);
}

std::chrono::nanoseconds PrimeNumbersSet::GetTotalTimeUnderMutex() const {
    return std::chrono::nanoseconds(nanoseconds_under_mutex_);
}

bool check(int v) {
    if (v < 2) return false;
    for (int i = 2; i*i <= v; ++i)
        if (v % i == 0) return false;
    return true;
}


void PrimeNumbersSet::AddPrimesInRange(uint64_t from, uint64_t to) {
    std::unordered_set<uint64_t> k;
    for (uint64_t i = from; i <  to; ++i) {
        if (check(i)) {
            k.insert(i);
        }
    }
    std::chrono::time_point<std::chrono::high_resolution_clock> waitstartTime = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::seconds (1));
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> finishTime;
    {
        std::lock_guard<std::mutex> ul(set_mutex_);
        startTime = std::chrono::high_resolution_clock::now();
        for (auto& c : k)
            primes_.insert(c);

        finishTime = std::chrono::high_resolution_clock::now();
    }

    const std::chrono::duration waitDuration = startTime - waitstartTime;
    std::chrono::nanoseconds multithreadDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(waitDuration);
    nanoseconds_waiting_mutex_ += multithreadDuration.count();

    const std::chrono::duration duration = finishTime - startTime;
    multithreadDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
    nanoseconds_under_mutex_ += multithreadDuration.count();



}