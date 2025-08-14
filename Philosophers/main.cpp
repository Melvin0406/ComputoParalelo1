#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <print>

class Fork {
    std::mutex mtx;

public:
    void pickUp() {
        mtx.lock();
    }

    void putDown() {
        mtx.unlock();
    }
};

class Philosopher {
    std::shared_ptr<Fork> leftFork;
    std::shared_ptr<Fork> rightFork;
    std::string name;

public:
    explicit Philosopher(std::string name) : name(std::move(name)) {}

    void eat() {
        while (true) {
            leftFork->pickUp();
            // std::println("{} picked up left fork", name);
            // std::this_thread::sleep_for(std::chrono::seconds(1));
            rightFork->pickUp();
            // std::println("{} picked up right fork", name);

            std::println("{} started eating", name);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::println("{} finished eating", name);

            leftFork->putDown();
            rightFork->putDown();

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    std::shared_ptr<Fork> &LeftFork() { return leftFork; }
    std::shared_ptr<Fork> &RightFork() { return rightFork; }
};

int main() {
    const int numPhilosophers = 5;
    std::vector<std::shared_ptr<Fork>> forks(numPhilosophers);
    std::vector<Philosopher> philosophers;

    for (int i = 0; i < numPhilosophers; i++) {
        forks[i] = std::make_shared<Fork>();
    }

    for (int i = 0; i < numPhilosophers; i++) {
        philosophers.emplace_back("Shrekk " + std::to_string(i + 1));
        philosophers[i].LeftFork() = forks[i];
        philosophers[i].RightFork() = forks[(i + 1) % numPhilosophers];
    }

    std::vector<std::thread> threads;
    for (auto &p : philosophers) {
        threads.emplace_back(&Philosopher::eat, &p);
    }

    for (auto &t : threads) {
        t.join();
    }

    return 0;
}
