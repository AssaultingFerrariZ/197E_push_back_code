

#ifndef SUBSYSTEM_HPP
#define SUBSYSTEM_HPP
#include "fmt/core.h"
#include "main.h"
#include "pros/rtos.hpp"
#include <functional>
#include <memory>
#include <unordered_map>
#include <mutex>

class Subsystem {
private:
    std::vector<std::function<void()>> functionQueue;
    pros::Mutex queueMutex; // 🔐

public:
    virtual ~Subsystem() = default;

    void functionHandler();

    template <typename Func>
    void addFunction(Func&& f) {
        std::lock_guard<pros::Mutex> lock(queueMutex);
        functionQueue.emplace_back(std::function<void()>(std::forward<Func>(f)));
    }

    void clearQueue() {
        std::lock_guard<pros::Mutex> lock(queueMutex);
        functionQueue.clear();
    }
};


using SubsystemPtr = std::shared_ptr<Subsystem>;

class SubsystemHandler {
private:
    std::unordered_map<SubsystemPtr, pros::Task> subsystemTaskMap;

public:
    SubsystemHandler(const std::vector<SubsystemPtr>& subsystems);

    pros::Task getTask(const SubsystemPtr& subsys) const;
};

#endif // SUBSYSTEM_HPP
