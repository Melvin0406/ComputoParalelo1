#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

class Counter {
private:
    int id;
    int total;

public:
    explicit Counter(int id) {
        this->total = 0;
        this->id = id;
    }

    int getTotal() const {
        return this->total;
    }

    void count() {
        for (int i = 0; i < 100; i++) {
            int randomNumber = 1 + (rand() % 1000);
            total+=randomNumber;
        }
        std::cout << "Counter " << id << ": " << total << std::endl;
    }

    void showTotal() {
        std::cout << "\nCounter " << id << " is the max with: " << total << std::endl;
    }
};

int main() {
    srand(time(0));

    std::vector<Counter> counters;
    for (int i = 1; i <= 10; i++) {
        counters.emplace_back(i);
    }

    std::vector<std::thread> threads;
    for (auto& counter : counters) {
        threads.emplace_back(&Counter::count, &counter);
    }

    for (auto &t : threads) {
        t.join();
    }

    std::sort(counters.begin(), counters.end(),
              [](const Counter &a, const Counter &b) {
                  return a.getTotal() > b.getTotal();
              });

    counters.front().showTotal();

    return 0;
}