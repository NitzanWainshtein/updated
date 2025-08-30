#include "ActiveObject.h"
#include <iostream>

ActiveObject::ActiveObject(const std::string& name) 
    : running(false), processedTasks(0), objectName(name) {
}

ActiveObject::~ActiveObject() {
    stop();
}

void ActiveObject::start() {
    if (!running.load()) {
        running = true;
        workerThread = std::thread(&ActiveObject::run, this);
    }
}

void ActiveObject::stop() {
    if (running.load()) {
        running = false;
        condition.notify_all();
        if (workerThread.joinable()) {
            workerThread.join();
        }
    }
}

void ActiveObject::enqueue(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        taskQueue.push(std::move(task));
    }
    condition.notify_one();
}

int ActiveObject::getQueueSize() const {
    std::lock_guard<std::mutex> lock(queueMutex);
    return static_cast<int>(taskQueue.size());
}

void ActiveObject::run() {
    while (running.load()) {
        std::unique_lock<std::mutex> lock(queueMutex);
        condition.wait(lock, [this] { 
            return !taskQueue.empty() || !running.load(); 
        });
        
        if (!running.load()) break;
        
        if (!taskQueue.empty()) {
            auto task = std::move(taskQueue.front());
            taskQueue.pop();
            lock.unlock();
            
            try {
                task();
                processedTasks.fetch_add(1);
            } catch (const std::exception& e) {
                std::cerr << "[" << objectName << "] Error: " << e.what() << std::endl;
            } catch (...) {
                std::cerr << "[" << objectName << "] Unknown error occurred" << std::endl;
            }
        }
    }
}