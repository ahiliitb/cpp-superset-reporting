#include <thread>
#include <mutex>
#include <bits/stdc++.h>
using namespace std;


std::mutex mtx;  // Mutex to protect shared resource
int counter = 0;

void increment_counter(int id) {
    mtx.lock();  // Lock the mutex before modifying shared data
    ++counter;
    std::cout << "Thread " << id << " incremented counter to " << counter << std::endl;
    mtx.unlock();  // Unlock the mutex after modifying shared data
}

int main() {
    // std::thread t1(increment_counter, 1);
    // std::thread t2(increment_counter, 2);

    // t1.join();
    // t2.join();
    float y = 1.5;
    int x = (int)(y*2);
    

    return 0;
}
