#include "intake.hpp"
#include "definitions.hpp"

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
            topSgn = 0;
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
    bunnyEars.set_value(true);
    move(velocity);
}

void Intake::scoreBottom(int velocity) {
    intakeScoreConfig = BOTTOM;
    this->functionID = 2;
    bunnyEars.set_value(false);
    move(velocity);
}

void Intake::scoreTop(int velocity) {
    intakeScoreConfig = TOP;
    this->functionID = 3;
    bunnyEars.set_value(false);
    move(velocity);
}

void Intake::scoreMiddle(int velocity) {
    intakeScoreConfig = MIDDLE;
    this->functionID = 4;
    bunnyEars.set_value(false);
    move(velocity);
}

void Intake::stop() {
    intakeScoreConfig = STOP;
    this->functionID = 5;
    bunnyEars.set_value(false);
    move(0);
}
