#include "intake.hpp"
#include "definitions.hpp"
#include "pros/rtos.hpp"
#include "main.h"
#include <ostream>

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
        case LOAD_BOTTOM:
            pickupSgn = 1;
            backSgn = 0;
            topSgn = 0;
            break;
        case FILL_MIDDLE:
            pickupSgn = 1;
            backSgn = 0.3;
            topSgn = -0.3;
        case SCORE_NO_BOTTOM:
            pickupSgn = 0;
            backSgn = 1;
            topSgn = -1;
            
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
    this->loading = true;
    this->functionID = 1;
    doublePark.set_value(false);
    hood.set_value(true);
    intakeLift.set_value(false);
    move(velocity);
}

void Intake::loadBottom(int velocity) {
    this->loading = true;
    intakeScoreConfig = LOAD_BOTTOM;
    this->functionID = 10;
    doublePark.set_value(false);
    move(velocity);
}

std::vector<int> Intake::get_actual_velocity() {
    return {static_cast<int>(pickupStage->get_actual_velocity()), static_cast<int>(backStage->get_actual_velocity()), static_cast<int>(topStage->get_actual_velocity())};
}

std::vector<int> Intake::get_target_velocity() {
    return {static_cast<int>(pickupStage->get_target_velocity()), static_cast<int>(backStage->get_target_velocity()), static_cast<int>(topStage->get_target_velocity())};
}

bool Intake::isIntaking() {
    return abs(this->get_target_velocity()[0]) > 50; // or whatever threshold makes sense
}


void Intake::antiJam() {
    anti_jam_running = true;

    topStage->move(0);
    backStage->move(-127);
    pickupStage->move(-127);
    pros::delay(ANTI_JAM_DELAY_MS);

    // After reversing, stop briefly to reset velocities
    topStage->move(0);
    backStage->move(0);
    pickupStage->move(0);
    pros::delay(100);

    // Resume forward intake motion
    topStage->move(127);
    backStage->move(127);
    pickupStage->move(127*topSgn);

    anti_jam_running = false;
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
    this->loading = false;
    this->functionID = 2;
    doublePark.set_value(false);
    move(velocity);
}

void Intake::scoreTop(int velocity) {
    intakeScoreConfig = TOP;
    this->loading = false;
    this->functionID = 3;
    hood.set_value(false);
    intakeLift.set_value(false);
    doublePark.set_value(false);
    // pros::delay(250);
    move(velocity);
}


void Intake::scoreMiddle(int velocity) {
    intakeScoreConfig = MIDDLE;
    this->loading = false;
    this->functionID = 4;
    intakeLift.set_value(false);
    doublePark.set_value(false);
    move(velocity);
}

void Intake::fillMiddle(int velocity) {
    intakeScoreConfig = FILL_MIDDLE;
    this->loading = false;
    this->functionID = 11;
    move(velocity);
}

void Intake::outake(int velocity) {
    intakeScoreConfig = BOTTOM;
    this->functionID = 6;
    doublePark.set_value(false);
    move(velocity);
}

void Intake::scoreNoBottom(int velocity) {
    intakeScoreConfig = SCORE_NO_BOTTOM;
    this->functionID = 12;
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


    
    while ((doubleParkDist.get_distance() > 45 || doubleParkDist.get_distance() < 15) && timeout < 2000) {
        backStage->move(-127);
        pickupStage->move(-68);
        pros::delay(69);
        timeout += 69;
        std::cout<<doubleParkDist.get_distance()<<std::endl;
    }
    backStage->move(0);
    pickupStage->move(0);
    if (timeout < 2000) doublePark.set_value(true);
    //     this->clearQueue();
    // });
}
