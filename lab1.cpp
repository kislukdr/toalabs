#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>


using std::chrono::time_point;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;


class Time {
    time_point <high_resolution_clock> begin;
    time_point <high_resolution_clock> end;
    unsigned long long time_ns;
public:
    void clear() { time_ns = 0; }
    void start() { begin = high_resolution_clock::now(); }
    void finish() {
        end = high_resolution_clock::now();
        nanoseconds duration = duration_cast <nanoseconds> (end - begin);
        time_ns = duration.count();
    }
    void print();
    Time() { time_ns = 0; }
};


class Primes {
    int last_n;
    std::vector <int> values;
    std::vector <Time> times;
    bool is_prime(int value);
    void add(int n, int additive);
public:
    int get(int i) { return values[i]; }
    void clear_time() {
        for (int i = 0; i < times.size(); i++)
            times[i].clear();
    }
    void print_time() {
        std::cout << std::endl;
        for (int i = 0; i < times.size(); i++) {
            std::cout << std::setw(4) << values[i] << " | ";
            times[i].print();
            std::cout << std::endl;
        }
    }
    void expand_to(int value);
    bool print(int* i, int limit); // true - limit reached
    Primes() {
        last_n = 0;
        values = { 2, 3 };
        times = { Time(), Time() };
    }
};


int main() {
    Time total_time;
    total_time.start();

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
        int i = 1;
        std::cout << "[2";
        bool limit_reached = primes.print(&i, limit);
        std::cout << "]";
        if (!limit_reached) {
            primes.expand_to(limit);
            if (primes.get(i) <= limit)
                primes.print(&i, limit);
        }
        std::cout << std::endl;
        primes.print_time();
    }

    total_time.finish();
    std::cout << "Total time: ";
    total_time.print();
    std::cout << std::endl;
    return 0;
}


bool Primes::is_prime(int value) {
    // no %2 checking: (6n+-1)%2 = 1
    int root = (int)sqrt(value);
    for (int i = 3; i <= root; i += 2)
        if (value % i == 0)
            return false;
    return true;
}


void Primes::add(int n, int additive) {
    Time time;
    time.start();
    int value = 6*n + additive;
    bool value_is_prime = is_prime(value);
    time.finish();
    if (value_is_prime) {
        values.push_back(value);
        times.push_back(time);
    }
}


void Primes::expand_to(int value) {
    while (values.back() < value) {
        int n = ++last_n;
        add(n, -1);
        add(n, +1);
    }
}


bool Primes::print(int* i, int limit) {
    while (*i < values.size()) {
        int prime = values[*i];
        if (prime > limit)
            return true;
        std::cout << ", " << prime;
        if (prime == limit)
            return true;
        (*i)++;
    }
    return false;
}


void Time::print() {
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