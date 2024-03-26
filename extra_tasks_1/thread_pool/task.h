#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <functional>
#include <condition_variable>
#include <atomic>
#include <stdexcept>
#include <cassert>
#include <iostream>
#include <chrono>

/*
 * Требуется написать класс ThreadPool, реализующий пул потоков, которые выполняют задачи из общей очереди.
 * С помощью метода PushTask можно положить новую задачу в очередь
 * С помощью метода Terminate можно завершить работу пула потоков.
 * Если в метод Terminate передать флаг wait = true,
 *  то пул подождет, пока потоки разберут все оставшиеся задачи в очереди, и только после этого завершит работу потоков.
 * Если передать wait = false, то все невыполненные на момент вызова Terminate задачи, которые остались в очереди,
 *  никогда не будут выполнены.
 * После вызова Terminate в поток нельзя добавить новые задачи.
 * Метод IsActive позволяет узнать, работает ли пул потоков. Т.е. можно ли подать ему на выполнение новые задачи.
 * Метод GetQueueSize позволяет узнать, сколько задач на данный момент ожидают своей очереди на выполнение.
 * При создании нового объекта ThreadPool в аргументах конструктора указывается количество потоков в пуле. Эти потоки
 *  сразу создаются конструктором.
 * Задачей может являться любой callable-объект, обернутый в std::function<void()>.
 */

class ThreadPool {
public:
    ThreadPool(size_t threadCount)
            : threads(), active(true), wait_(false), cnt(0)
    {
        for (size_t i = 0; i < threadCount; ++i) {
            threads.emplace_back(&ThreadPool::f, this);
        }
    }


    void f() {
        while (true) {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]() -> bool { return !q.empty() || !active; });
            if (active == 0 && (wait_ == 0 || q.empty())) {
                ++cnt;
                if (cnt == threads.size()) term_cv.notify_one();
                return;
            }
            auto func = std::move(q.front());
            q.pop();
            lock.unlock();
            func();
        }

    }

    void PushTask(const std::function<void()>& task) {
        std::unique_lock<std::mutex> lock(mtx);
        if (active) {
            q.push(task);
            cv.notify_one();
        } else {
            throw std::exception();
        }
    }

    void Terminate(bool wait) {
        {
            std::unique_lock<std::mutex> lock(mtx);
            active = false;
            wait_ = wait;
            cv.notify_all();
            if (wait)
                term_cv.wait(lock, [this]() { return q.empty(); });
        }
        for (auto& thread : threads) {
            if (thread.joinable())
                thread.join();
        }
    }

    bool IsActive() const {
        std::unique_lock<std::mutex> lock(mtx);
        return active;
    }

    size_t QueueSize() const {
        std::unique_lock<std::mutex> lock(mtx);
        return q.size();
    }
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> q;
    mutable std::mutex mtx;
    std::condition_variable cv, term_cv;
    bool active;
    bool wait_;
    size_t cnt;
};

