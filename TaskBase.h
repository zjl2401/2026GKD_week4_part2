#pragma once
#include <iostream>
#include <thread>
#include <atomic>
using namespace std;

class TaskBase {
protected:
    int key;
    atomic<int>* p1;
    atomic<int>* p2;
    atomic<bool> running;
    thread th;

public:
    TaskBase(int k, atomic<int>* _p1, atomic<int>* _p2);
    virtual ~TaskBase();

    void start();
    void stop();

    virtual void run() = 0;
    virtual void callback(int msg) = 0;
    int getKey() const { return key; }
};

// Task1
class Task1 : public TaskBase {
public:
    Task1(int k, atomic<int>* _p1, atomic<int>* _p2);
    void run() override;
    void callback(int msg) override;
};

// Task2
class Task2 : public TaskBase {
    atomic<int> kFactor;
public:
    Task2(int k, atomic<int>* _p1, atomic<int>* _p2);
    void run() override;
    void callback(int msg) override;
};

// Task3
class Task3 : public TaskBase {
public:
    Task3(int k, atomic<int>* _p1, atomic<int>* _p2);
    void run() override;
    void callback(int msg) override;
};
