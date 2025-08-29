#include "ActiveObject.h"
#include <iostream>

ActiveObject::ActiveObject(const std::string& name) 
    : running(false), processedTasks(0), objectName(name) {
}

ActiveObject::~ActiveObject() {
    stop();
}

void ActiveObject::start() {
    if (!running) {
        running = true;
        workerThread = std::thread(&ActiveObject::run, this);
    }
}

void ActiveObject::stop() {
    if (running) {
        running = false;
        condition.notify_all();
        if (workerThread.joinable()) {
            workerThread.join();
        }
    }
}

void ActiveObject::enqueue(std::function<void()> task) {
    std::lock_guard<std::mutex> lock(queueMutex);
    taskQueue.push(task);
    condition.notify_one();
}

int ActiveObject::getQueueSize() const {
    std::lock_guard<std::mutex> lock(queueMutex);
    return taskQueue.size();
}

void ActiveObject::run() {
    while (running) {
        std::unique_lock<std::mutex> lock(queueMutex);
        condition.wait(lock, [this] { return !taskQueue.empty() || !running; });
        
        if (!running) break;
        
        if (!taskQueue.empty()) {
            auto task = taskQueue.front();
            taskQueue.pop();
            lock.unlock();
            
            try {
                task();
                processedTasks++;
            } catch (const std::exception& e) {
                std::cerr << "[" << objectName << "] Error: " << e.what() << std::endl;
            }
        }
    }
}