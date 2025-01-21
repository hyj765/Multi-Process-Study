#pragma once
#include<iostream>
#include<future>
#include<thread>
#include<functional>
#include<map>
#include<tuple>
#include<mutex>
#include<queue>
#include<atomic>

class taskmanager {
public:

    static taskmanager& getInstance() noexcept
    {
        static taskmanager instance;
        return instance;
    }
    

    void initThread(int workersNum)
    {
        for (int i = 0; i < workersNum; ++i) {
            workerThreads.emplace_back([this]() {workerThread(); });
        }
    }

    template<typename Func, typename... Args>
    inline std::future<typename std::invoke_result<Func,Args...>::type> insertTask(Func function, Args... parameter)
    {
        using ResultType = typename std::invoke_result<Func, Args...>::type;
        std::lock_guard<std::mutex> lock(taskMtx);
        
        auto task = std::make_shared<std::packaged_task<ResultType()>>(
            std::bind(std::forward<Func>(function), std::forward<Args>(parameter)...));

        auto res = task->get_future();
   
        taskQueue.push_front([task]() { (*task)(); });
        cv.notify_one();

        return res;
    }
    
private:



    void workerThread()
    {
        while (taskManagerisRun)
        {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(taskMtx);
                cv.wait(lock, [&]() {return taskQueue.empty() == false || !taskManagerisRun; });
   
                if (taskManagerisRun == false && taskQueue.empty()) return;
                
                task = taskQueue.front();
                taskQueue.pop_front();
            }

            task();
        }
    }

    taskmanager() { taskManagerisRun = true; };
    taskmanager(const taskmanager& other) = delete;
    taskmanager(taskmanager& other) = delete;
    taskmanager& operator=(const taskmanager& task) = delete;
    ~taskmanager()
    {
        taskManagerisRun = false;
        cv.notify_all();
    }
    std::vector<std::jthread> workerThreads;
    std::mutex taskMtx;
    std::condition_variable cv;
    bool taskManagerisRun = false;
    std::deque <std::function<void()>> taskQueue;
   
};
