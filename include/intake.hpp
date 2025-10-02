#pragma once

#include "subsystem.hpp"
#include "pros/motors.hpp"
#include <vector>

class Intake : public Subsystem {
private:
    std::vector<pros::Motor*> motors;

    const int ANTI_JAM_DELAY_MS = 500;
    const int COLOR_SORT_DELAY_MS = 250;

    enum scoreConfig {STOP, LOAD, TOP, MIDDLE, BOTTOM };
    scoreConfig intakeScoreConfig = TOP;


    pros::Motor* pickupStage;
    int pickupSgn = 1;
    pros::Motor* backStage;
    int backSgn = 1;
    pros::Motor* topStage;
    int topSgn = 1;

    void setSigns();
    void move(int velocity);


public:
    enum blockColor { RED, BLUE, UNKNOWN };
    blockColor allianceColor = UNKNOWN;

    Intake(std::vector<pros::Motor*> _motors);

    std::vector<pros::Motor*> getMotors() const;

    bool anti_jam_enabled = true;
    bool color_sorting_enabled = true;
    
    void load(int velocity);
    std::vector<int> get_actual_velocity();
    std::vector<int> get_target_velocity();
    void antiJam();
    void colorSort();
    void scoreBottom(int velocity);
    void scoreTop(int velocity);
    void scoreMiddle(int velocity);
    void stop();

};

