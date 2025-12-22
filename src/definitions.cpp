#include "definitions.hpp"
#include "pros/adi.hpp"
#include "pros/distance.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/motor_group.hpp"
#include "autons.hpp"
#include "main.h"
#include "pros/optical.hpp"
#include "subsystem.hpp"
#include <memory>

// Controller definition
pros::Controller controller(pros::E_CONTROLLER_MASTER);
pros::Controller liveScorer(pros::E_CONTROLLER_PARTNER);

// Sensor and motor initialization

pros::MotorGroup leftSide({-8, -12, -4 }, pros::MotorGearset::blue);
pros::MotorGroup rightSide({1, 5, 21}, pros::MotorGearset::blue);

pros::Motor pickupStage(2, pros::v5::MotorGears::blue, pros::v5::MotorEncoderUnits::deg);
pros::Motor backStage(7, pros::v5::MotorGears::green, pros::v5::MotorEncoderUnits::deg);
pros::Motor topStage(-17, pros::v5::MotorGears::green, pros::v5::MotorEncoderUnits::deg);

pros::Optical colorSensor(14);
pros::Distance doubleParkDist(18);

std::shared_ptr<Intake> intake = std::make_shared<Intake>(std::vector<pros::Motor*>{&pickupStage, &backStage, &topStage});

pros::Imu imu(11); 
pros::Rotation horizontal_sensor(-19);
pros::Rotation veritcal_sensor(3);

// Chassis setup
lemlib::Drivetrain drivetrain(&leftSide, &rightSide, 10.395, 3.25, 450, 12);

//pistons
pros::adi::DigitalOut odomLift('A');
pros::adi::DigitalOut matchLoader('B');
pros::adi::DigitalOut hood('F');
pros::adi::DigitalOut descore('G');
pros::adi::DigitalOut doublePark('H');
pros::adi::DigitalOut intakeLift('C');
   



// Tracking wheel setup
lemlib::TrackingWheel horizontal_tracking_wheel
    (&horizontal_sensor, 2, .75);
lemlib::TrackingWheel veritcal_tracking_wheel
    (&veritcal_sensor, 2, -.25);

// Odometer setup
lemlib::OdomSensors odom(&veritcal_tracking_wheel, nullptr, 
    &horizontal_tracking_wheel, nullptr,&imu);

// Angular PID controller
// Angular PID controller configuration
lemlib::ControllerSettings angular_controller(
    4, 
    0.25, 
    30.5, 
    3, 
    0.1,
    50,
    0.5, 
    150, 
    0);

// Lateral PID controller
lemlib::ControllerSettings lateral_controller(5.4, // proportional gain (kP)
                                              0.125, // integral gain (kI)
                                              37.75, // derivative gain (kD)
                                              2.5, // anti windup
                                              1, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              2.5, // large error range, in degrees
                                              250, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// Input Scaling Setup

    // input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttle_curve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

    // input curve for steer input during driver control
lemlib::ExpoDriveCurve steer_curve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);
   

// Chassis setup

std::shared_ptr<lemlib::Chassis> chassis(new lemlib::Chassis(drivetrain, lateral_controller, angular_controller, odom));

// Auton Selector setup
std::map<int, std::pair<std::string, std::function<void()>>> autonSelectorMap = {
    {1, {"PID Tuning", pidTuning}},
    {2, {"Elims 7 Red Left", elims7BallRedLeft}},
    {3, {"Elims 7 Red Right", elims7BallRedRight}},
    {4, {"Elims 7 Blue Right", elims7BallBlueRight}},
    {5, {"Elims 7 Blue Left", elims7BallBlueLeft}},
    {6, {"Half AWP Red", halfAWPRedLeft}},
    {7, {"Half AWP Blue", halfAWPBlueLeft}},
    {8, {"Solo AWP Red", soloAWPRedLeft}},
    {9, {"Solo AWP Blue", soloAWPBlueLeft}},
    {10, {"Skills", skills}}
};

int currentAutoSelection = 6;
bool autoSelected = false;
bool autoActive = false;

