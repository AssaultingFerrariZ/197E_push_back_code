#include "definitions.hpp"
#include "pros/motor_group.hpp"
#include "autons.hpp"
#include "main.h"
#include "subsystem.hpp"
#include <memory>

// Controller definition
pros::Controller controller(pros::E_CONTROLLER_MASTER);


// Sensor and motor initialization

pros::MotorGroup leftSide({1, 2, 3}, pros::MotorGearset::blue);
pros::MotorGroup rightSide({4, 5, 6}, pros::MotorGearset::blue);

pros::Motor pickupStage(18, pros::v5::MotorGears::blue, pros::v5::MotorEncoderUnits::deg);
pros::Motor backStage(9, pros::v5::MotorGears::green, pros::v5::MotorEncoderUnits::deg);
pros::Motor topStage(16, pros::v5::MotorGears::green, pros::v5::MotorEncoderUnits::deg);

std::shared_ptr<Intake> intake = std::make_shared<Intake>(std::vector<pros::Motor*>{&pickupStage, &backStage, &topStage});

pros::Imu imu(7); 
pros::Rotation horizontal_sensor(15);
pros::Rotation veritcal_sensor(17);

// Chassis setup
lemlib::Drivetrain drivertrain(&leftSide, &rightSide, 10.395, 3.25, 450, 2);

// Tracking wheel setup
lemlib::TrackingWheel horizontal_tracking_wheel
    (&horizontal_sensor, lemlib::Omniwheel::NEW_2, 0);
lemlib::TrackingWheel veritcal_tracking_wheel
    (&veritcal_sensor, lemlib::Omniwheel::NEW_2, 0);

// Odometer setup
lemlib::OdomSensors odom(&veritcal_tracking_wheel, nullptr, 
    &horizontal_tracking_wheel, nullptr, &imu);

// Lateral PID controller
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// Angular PID controller
lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in degrees
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
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

std::shared_ptr<lemlib::Chassis> chassis(new lemlib::Chassis(drivertrain, lateral_controller, angular_controller, odom, &throttle_curve, &steer_curve));

// Auton Selector setup

std::map<int, std::pair<std::string, std::function<void()>>> autonSelectorMap = {
    {1, {"PID Tuning", pidTuning}}
};

int currentAutoSelection = 1;
bool autoSelected = false;
bool autoActive = false;

