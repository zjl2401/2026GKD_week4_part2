#include "TaskBase.h"
#include <chrono>
using namespace std;

// TaskBase
TaskBase::TaskBase(int k, atomic<int>* _p1, atomic<int>* _p2)
    : key(k), p1(_p1), p2(_p2), running(false) {}

TaskBase::~TaskBase() {
    stop();
}

void TaskBase::start() {
    running.store(true);
    th = thread(&TaskBase::run, this);
}

void TaskBase::stop() {
    running.store(false);
    if (th.joinable()) th.join();
}

// Task1
Task1::Task1(int k, atomic<int>* _p1, atomic<int>* _p2)
    : TaskBase(k, _p1, _p2) {}

void Task1::run() {
    while (running) {
        int val = p1->exchange(0);
        if (val != 0) {
            *p2 = val + 1;
            cout << "write Task1-" << key << ": " << *p2 << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}

void Task1::callback(int msg) {
    p1->store(msg);
}

// Task2
Task2::Task2(int k, atomic<int>* _p1, atomic<int>* _p2)
    : TaskBase(k, _p1, _p2), kFactor(1) {}

void Task2::run() {
    while (running) {
        int val = p1->exchange(0);
        if (val != 0) {
            *p2 = val * kFactor.load();
            cout << "write Task2-" << key << ": " << *p2 << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}

void Task2::callback(int msg) {
    kFactor.store(msg);
    p1->store(1);
}

// Task3
Task3::Task3(int k, atomic<int>* _p1, atomic<int>* _p2)
    : TaskBase(k, _p1, _p2) {}

void Task3::run() {
    while (running) {
        int val = p1->exchange(0);
        if (val != 0) {
            *p2 = val;
            cout << "write Task3-" << key << ": " << *p2 << endl;
            this_thread::sleep_for(chrono::milliseconds(1));
            *p2 = val + 1;
            cout << "write Task3-" << key << ": " << *p2 << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}

void Task3::callback(int msg) {
    p1->store(msg);
}
