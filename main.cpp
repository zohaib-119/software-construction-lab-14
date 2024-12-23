#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <map>
#include <shared_mutex>
#include <random>

// Task 1: Introduction to Multithreading
void printNumbers() {
    for (int i = 1; i <= 10; ++i) {
        std::cout << "Thread 1: " << i << std::endl;
    }
}

void printSquares() {
    for (int i = 1; i <= 10; ++i) {
        std::cout << "Thread 2: " << i * i << std::endl;
    }
}

// Task 2: Thread Synchronization
std::mutex counterMutex;
int counter = 0;

void incrementCounter() {
    for (int i = 0; i < 100; ++i) {
        std::lock_guard<std::mutex> lock(counterMutex);
        ++counter;
    }
}

// Task 3: Concurrent Data Structures
std::shared_mutex sharedDataMutex;
std::map<int, int> sharedData;

void writeData(int threadID) {
    for (int i = 0; i < 5; ++i) {
        std::unique_lock<std::shared_mutex> lock(sharedDataMutex);
        sharedData[threadID * 10 + i] = threadID;
        std::cout << "Thread " << threadID << " wrote: " << threadID * 10 + i << std::endl;
    }
}

void readData() {
    std::shared_lock<std::shared_mutex> lock(sharedDataMutex);
    for (const auto &entry : sharedData) {
        std::cout << "Read: Key = " << entry.first << ", Value = " << entry.second << std::endl;
    }
}

// Task 4: Simulation of Bank Transaction System
std::mutex accountMutex;
int bankAccountBalance = 1000;

void performTransactions(int clientID) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 200);

    for (int i = 0; i < 10; ++i) {
        int transaction = dist(gen);
        {
            std::lock_guard<std::mutex> lock(accountMutex);
            if (dist(gen) % 2 == 0) {
                bankAccountBalance += transaction;
                std::cout << "Client " << clientID << " deposited: " << transaction << " | New Balance: " << bankAccountBalance << std::endl;
            } else if (bankAccountBalance >= transaction) {
                bankAccountBalance -= transaction;
                std::cout << "Client " << clientID << " withdrew: " << transaction << " | New Balance: " << bankAccountBalance << std::endl;
            }
        }
    }
}

int main() {
    // Task 1: Introduction to Multithreading
    std::thread t1(printNumbers);
    std::thread t2(printSquares);
    t1.join();
    t2.join();

    // Task 2: Thread Synchronization
    std::thread t3(incrementCounter);
    std::thread t4(incrementCounter);
    std::thread t5(incrementCounter);
    t3.join();
    t4.join();
    t5.join();
    std::cout << "Final counter value: " << counter << std::endl;

    // Task 3: Concurrent Data Structures
    std::thread t6(writeData, 1);
    std::thread t7(writeData, 2);
    t6.join();
    t7.join();

    std::thread t8(readData);
    t8.join();

    // Task 4: Simulation of Bank Transaction System
    std::thread clients[5];
    for (int i = 0; i < 5; ++i) {
        clients[i] = std::thread(performTransactions, i + 1);
    }
    for (int i = 0; i < 5; ++i) {
        clients[i].join();
    }

    std::cout << "Final bank account balance: " << bankAccountBalance << std::endl;

    return 0;
}
