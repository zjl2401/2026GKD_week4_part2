#include "TaskManager.h"
#include <iostream>
#include <sstream>
#include <chrono>
using namespace std;

TaskManager::TaskManager() : out(0), running(true) {}

TaskManager::~TaskManager() {
    running = false;
    if (t1.joinable()) t1.join();
    if (t2.joinable()) t2.join();
    for (auto& task : tasks) task->stop();
}

void TaskManager::addTask(int key, int kind) {
    lock_guard<mutex> lock(mtx);
    a.emplace_back(0);
    atomic<int>* p1 = &a.back();
    atomic<int>* p2 = (tasks.empty()) ? &out : &a[tasks.size() - 1];
    unique_ptr<TaskBase> task;
    if (kind == 1) task = make_unique<Task1>(key, p1, p2);
    else if (kind == 2) task = make_unique<Task2>(key, p1, p2);
    else task = make_unique<Task3>(key, p1, p2);
    task->start();
    tasks.push_back(move(task));
}

void TaskManager::popTask() {
    lock_guard<mutex> lock(mtx);
    if (!tasks.empty()) {
        tasks.back()->stop();
        tasks.pop_back();
        a.pop_back();
    }
}

void TaskManager::callback(int key, int msg) {
    lock_guard<mutex> lock(mtx);
    for (auto& t : tasks) {
        if (t->getKey() == key) {
            t->callback(msg);
            break;
        }
    }
}

void TaskManager::run() {
    // 线程1：监控 out
    t1 = thread([this]() {
        while (running) {
            int val = out.exchange(0);
            if (val != 0) {
                cout << "out: " << val << endl;
            }
            this_thread::sleep_for(chrono::milliseconds(1));
        }
    });

    // 线程2：cin输入
    t2 = thread([this]() {
        string line;
        while (running) {
            if (!getline(cin, line)) break;
            string cmd;
            int key, kind, msg;
            istringstream ss(line);
            ss >> cmd;
            if (cmd == "add") {
                ss >> key >> kind;
                addTask(key, kind);
            } else if (cmd == "pop") {
                popTask();
            } else if (cmd == "callback") {
                ss >> key >> msg;
                callback(key, msg);
            } else if (cmd == "exit") {
                running = false;
            }
        }
    });
}
