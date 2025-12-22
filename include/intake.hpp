#pragma once

#include "subsystem.hpp"
#include "pros/motors.hpp"
#include <vector>

class Intake : public Subsystem {
private:
    std::vector<pros::Motor*> motors;

    const int ANTI_JAM_DELAY_MS = 200;
    const int COLOR_SORT_DELAY_MS = 850;

    enum scoreConfig {STOP, LOAD, TOP, MIDDLE, FILL_MIDDLE, BOTTOM, LOAD_BOTTOM, SCORE_NO_BOTTOM };
    scoreConfig intakeScoreConfig = TOP;


    pros::Motor* pickupStage;
    float pickupSgn = 1;
    pros::Motor* backStage;
    float backSgn = 1;
    pros::Motor* topStage;
    float topSgn = 1;

    void setSigns();
    void move(int velocity);


public:
    bool anti_jam_running = false;
    bool isIntaking();
    bool loading = false;
    
    enum blockColor { RED, BLUE, UNKNOWN };
    blockColor allianceColor = UNKNOWN;

    Intake(std::vector<pros::Motor*> _motors);

    std::vector<pros::Motor*> getMotors() const;

    bool anti_jam_enabled = true;
    bool color_sorting_enabled = true;
    bool sort_out_bottom = false;
    bool color_stop_enabled = false;

    void load(int velocity);
    std::vector<int> get_actual_velocity();
    std::vector<int> get_target_velocity();
    void antiJam();
    void colorSort();
    void loadBottom(int velocity);
    void scoreBottom(int velocity);
    void scoreTop(int velocity);
    void scoreMiddle(int velocity);
    void fillMiddle(int velocity);
    void scoreNoBottom(int velocity);
    void outake(int velocity);
    void stop();
    void doubleParkMacro();

};

