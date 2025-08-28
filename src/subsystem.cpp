#include "subsystem.hpp"

void Subsystem::functionHandler() {
    std::lock_guard<pros::Mutex> lock(queueMutex);
    while (!functionQueue.empty()) {
        auto func = functionQueue.front();
        if (functionID != prevFunctionID) {
            func(); 
            prevFunctionID = functionID; 
        }
        functionQueue.erase(functionQueue.begin()); 
    }
}


SubsystemHandler::SubsystemHandler(const std::vector<SubsystemPtr>& subsystems) {
    for (const auto& subsystem : subsystems) {
        subsystemTaskMap.insert({
            subsystem,
            pros::Task([subsystem] {
                while (true) {
                    subsystem->functionHandler();
                    pros::delay(10);
                }
            })
        });
    }
}

pros::Task SubsystemHandler::getTask(const SubsystemPtr& subsys) const {
    return subsystemTaskMap.at(subsys);
}
