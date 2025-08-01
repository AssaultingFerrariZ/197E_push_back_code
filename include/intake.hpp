#pragma once

#include "subsystem.hpp"
#include "pros/motors.hpp"
#include <vector>

class Intake : public Subsystem {
private:
    std::vector<pros::Motor*> motors;

    enum scoreConfig {STOP, LOAD, TOP, MIDDLE, BOTTOM };
    scoreConfig intakeScoreConfig = TOP;

    pros::Motor* pickupStage;
    int pickupSgn = 1;
    pros::Motor* backStage;
    int backSgn = 1;
    pros::Motor* topStage;
    int topSgn = 1;

    void setSigns();

public:
    Intake(std::vector<pros::Motor*> _motors);

    std::vector<pros::Motor*> getMotors() const;

    void move(int velocity);
    void load(int velocity);
    void scoreBottom(int velocity);
    void scoreTop(int velocity);
    void scoreMiddle(int velocity);
    void stop();
};

