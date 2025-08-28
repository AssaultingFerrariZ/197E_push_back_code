#include "definitions.hpp"
#include "pros/adi.hpp"
#include "pros/motor_group.hpp"
#include "autons.hpp"
#include "main.h"
#include "pros/optical.hpp"
#include "subsystem.hpp"
#include <memory>

// Controller definition
pros::Controller controller(pros::E_CONTROLLER_MASTER);


// Sensor and motor initialization

pros::MotorGroup leftSide({-7, -8, -18}, pros::MotorGearset::blue);
pros::MotorGroup rightSide({9, 10, 17}, pros::MotorGearset::blue);

pros::Motor pickupStage(4, pros::v5::MotorGears::blue, pros::v5::MotorEncoderUnits::deg);
pros::Motor backStage(-6, pros::v5::MotorGears::green, pros::v5::MotorEncoderUnits::deg);
pros::Motor topStage(20, pros::v5::MotorGears::green, pros::v5::MotorEncoderUnits::deg);

pros::Optical colorSensor(16);

std::shared_ptr<Intake> intake = std::make_shared<Intake>(std::vector<pros::Motor*>{&pickupStage, &backStage, &topStage});

pros::Imu imu(11); 
pros::Rotation horizontal_sensor(-1);
pros::Rotation veritcal_sensor(-5);

// Chassis setup
lemlib::Drivetrain drivetrain(&leftSide, &rightSide, 10.395, 3.25, 450, 2);

pros::adi::DigitalOut matchLoader('B');
pros::adi::DigitalOut blockRush('A');
pros::adi::DigitalOut bunnyEars('H');

// Tracking wheel setup
lemlib::TrackingWheel horizontal_tracking_wheel
    (&horizontal_sensor, 2, -6.528225);
lemlib::TrackingWheel veritcal_tracking_wheel
    (&veritcal_sensor, 2, 0.13161);

// Odometer setup
lemlib::OdomSensors odom(&veritcal_tracking_wheel, nullptr, 
    &horizontal_tracking_wheel, nullptr,&imu);

// Angular PID controller
// Angular PID controller configuration
lemlib::ControllerSettings angular_controller(
    1.6, 
    0.25, 
    9, 
    3, 
    0.1,
    50,
    0.5, 
    150, 
    0);

// Lateral PID controller
lemlib::ControllerSettings lateral_controller(6.75, // proportional gain (kP)
                                              0.125, // integral gain (kI)
                                              18.75, // derivative gain (kD)
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
    {2, {"Elims Block Rush", elims9Ball}},
    {3, {"Solo AWP", soloAWP}}

};

int currentAutoSelection = 2;
bool autoSelected = false;
bool autoActive = false;

