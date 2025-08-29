#ifndef ACTIVE_OBJECT_H
#define ACTIVE_OBJECT_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <atomic>

class ActiveObject {
private:
    std::thread workerThread;
    std::queue<std::function<void()>> taskQueue;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> running;
    std::atomic<int> processedTasks;
    std::string objectName;
    
public:
    ActiveObject(const std::string& name);
    ~ActiveObject();
    
    void enqueue(std::function<void()> task);
    void start();
    void stop();
    int getProcessedTaskCount() const { return processedTasks; }
    int getQueueSize() const;
    const std::string& getName() const { return objectName; }
    
private:
    void run();
};

#endif