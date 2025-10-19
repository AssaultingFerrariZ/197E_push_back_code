#include "intake.hpp"
#include "definitions.hpp"
#include "pros/rtos.hpp"
#include "main.h"

Intake::Intake(std::vector<pros::Motor*> _motors)
    : motors(_motors),
      pickupStage(_motors.at(0)),
      backStage(_motors.at(1)),
      topStage(_motors.at(2)) {}

std::vector<pros::Motor*> Intake::getMotors() const {
    return motors;
}

void Intake::setSigns() {
    switch (intakeScoreConfig) {
        case STOP:
            pickupSgn = 0;
            backSgn = 0;
            topSgn = 0;
            break;
        case LOAD:
            pickupSgn = 1;
            backSgn = 1;
            topSgn = 1;
            break;
        case TOP:
            pickupSgn = 1;
            backSgn = 1;
            topSgn = 1;
            break;
        case MIDDLE:
            pickupSgn = 1;
            backSgn = 1;
            topSgn = -1;
            break;
        case BOTTOM:
            pickupSgn = -1;
            backSgn = -1;
            topSgn = -1;
            break;
    }
}

void Intake::move(int velocity) {
    this->addFunction([this, velocity] {
        setSigns();
        pickupStage->move(pickupSgn * velocity);
        backStage->move(backSgn * velocity);
        topStage->move(topSgn * velocity);
    });
}

void Intake::load(int velocity) {
    intakeScoreConfig = LOAD;
    this->functionID = 1;
    doublePark.set_value(false);
    bunnyEars.set_value(true);
    move(velocity);
}

std::vector<int> Intake::get_actual_velocity() {
    return {static_cast<int>(pickupStage->get_actual_velocity()), static_cast<int>(backStage->get_actual_velocity()*3), static_cast<int>(topStage->get_actual_velocity()*3)};
}

std::vector<int> Intake::get_target_velocity() {
    return {static_cast<int>(pickupStage->get_target_velocity()), static_cast<int>(backStage->get_target_velocity()*3), static_cast<int>(topStage->get_target_velocity()*3)};
}

void Intake::antiJam() {
    // this->addFunction([this] {
        // pickupStage->move(-127 * pickupSgn);
        // backStage->move(-127 * backSgn);
        auto PREV_SCORE_CONFIG = intakeScoreConfig;
        intakeScoreConfig = BOTTOM;
        this->setSigns();
        pros::delay(ANTI_JAM_DELAY_MS);
        intakeScoreConfig = PREV_SCORE_CONFIG;
        this->setSigns();
        // pickupStage->move(127 * pickupSgn);
        // backStage->move(127 * backSgn);
    // });
}

void Intake::colorSort() {
    // this->addFunction([this] {

        topStage->move(-127*topSgn);
        // this->addFunction([this] {
        pros::delay(COLOR_SORT_DELAY_MS);
        // });
        topStage->move(127*topSgn);
        std::cout<<"triggered sort function"<<std::endl;
    // });
}

void Intake::scoreBottom(int velocity) {
    intakeScoreConfig = BOTTOM;
    this->functionID = 2;
    doublePark.set_value(false);
    move(velocity);
}

void Intake::scoreTop(int velocity) {
    intakeScoreConfig = TOP;
    this->functionID = 3;
    bunnyEars.set_value(false);
    intakeLift.set_value(false);
    doublePark.set_value(false);
    // pros::delay(250);
    move(velocity);
}


void Intake::scoreMiddle(int velocity) {
    intakeScoreConfig = MIDDLE;
    this->functionID = 4;
    bunnyEars.set_value(false);
    intakeLift.set_value(false);
    doublePark.set_value(false);
    move(velocity);
}

void Intake::outake(int velocity) {
    intakeScoreConfig = BOTTOM;
    this->functionID = 6;
    doublePark.set_value(false);
    move(velocity);
}


void Intake::stop() {
    intakeScoreConfig = STOP;
    this->functionID = 5;
    move(0);
}

void Intake::doubleParkMacro() {
    // this->functionID = 7;
    // this->addFunction([this] {
    int timeout = 0;
    while ((doubleParkDist.get_distance() > 65 || doubleParkDist.get_distance() < 30) && timeout < 2000) {
        backStage->move(-127);
        pickupStage->move(-68);
        pros::delay(10);
        timeout += 10;
    }
    backStage->move(0);
    pickupStage->move(0);
    if (timeout < 2000) doublePark.set_value(true);
    //     this->clearQueue();
    // });
}
