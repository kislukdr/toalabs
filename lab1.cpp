#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <cstdint>


using std::chrono::time_point;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;


class Timer {
    time_point <high_resolution_clock> begin;
    time_point <high_resolution_clock> end;
public:
    void start() { begin = high_resolution_clock::now(); }
    uint64_t finish() {
        end = high_resolution_clock::now();
        nanoseconds duration = duration_cast <nanoseconds> (end - begin);
        return duration.count();
    }
    static void print(uint64_t time_ns);
};


class Primes {
    std::vector <int> values;
    std::vector <uint64_t> times;
    bool is_prime(int value);
public:
    int get(int i) { return values[i]; }
    void clear_time() {
        for (int i = 0; i < times.size(); i++)
            times[i] = 0;
    }
    void print_time();
    void expand_to(int value);
    bool print(int* i, int limit); // true - limit reached
};


int main() {
    Timer total_timer;
    total_timer.start();

    Primes primes;
    int limit;

    std::cout << "Enter 0 to exit" << std::endl;
    while (true) {
        std::cout << "\nEnter limit: ";
        std::cin >> limit;
        if (limit == 1)
            continue;
        if (limit == 0)
            break;
        if (limit < 0) {
            std::cout << "Error: negative limit" << std::endl;
            continue;
        }

        primes.clear_time();
        int i = 0;
        std::cout << "in memory: ";
        bool limit_reached = primes.print(&i, limit);
        std::cout << std::endl << "calculated: ";
        if (!limit_reached) {
            primes.expand_to(limit);
            primes.print(&i, limit);
        }
        std::cout << std::endl;
        primes.print_time();
    }

    std::cout << "Total time: ";
    Timer::print(total_timer.finish());
    std::cout << std::endl;
    return 0;
}


bool Primes::is_prime(int value) {
    if (value <= 1)
        return false;
    int root = (int)sqrt(value);
    if (value == 2)
        return true;
    if (value % 2 == 0)
        return false;
    for (int i = 3; i <= root; i += 2)
        if (value % i == 0)
            return false;
    return true;
}


void Primes::expand_to(int value) {
    int i = 0;
    Timer timer;
    timer.start();
    if (values.size())
        i = values.back() + 1;
    for (; i <= value; i++) {
        if (!is_prime(i))
            continue;
        values.push_back(i);
        uint64_t time_ns = timer.finish();
        times.push_back(time_ns);
    }
}


void Primes::print_time() {
    std::cout << std::endl;
    for (int i = 0; i < times.size(); i++) {
        if (!times[i])
            continue;
        std::cout << std::setw(4) << values[i] << " : ";
        Timer::print(times[i]);
        std::cout << std::endl;
    }
}


bool Primes::print(int* i, int limit) {
    while (*i < values.size()) {
        int prime = values[*i];
        if (prime > limit)
            return true;
        std::cout << prime << " ";
        if (prime == limit)
            return true;
        (*i)++;
    }
    return false;
}


void Timer::print(uint64_t time_ns) {
    int ns = time_ns % 1000;
    int us = (time_ns / 1000) % 1000;
    int ms = (time_ns / (int)1e6) % 1000;
    int s = (time_ns / (int)1e9) % 60;
    int m = time_ns / (unsigned long long)60e9;
    if (m)
        std::cout << m << " m ";
    if (s)
        std::cout << s << " s ";
    if (ms && !m && !s)
        std::cout << ms << " ms ";
    if (us && !m && !s)
        std::cout << us << " us ";
    if (ns && !m && !s)
        std::cout << ns << " ns";
}