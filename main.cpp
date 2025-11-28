#include "TaskManager.h"

int main() {
    TaskManager manager;
    manager.run();

    // 主线程等待子线程退出
    while (true) {
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    return 0;
}
