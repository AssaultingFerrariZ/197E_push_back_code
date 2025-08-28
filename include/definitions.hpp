#pragma once    

#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "pros/motor_group.hpp"
#include "pros/motors.hpp"
#include "intake.hpp"
#include "pros/optical.hpp"
#include "subsystem.hpp"
#include <map>
#include <memory>
#include <utility>

extern pros::MotorGroup leftSide;
extern pros::MotorGroup rightSide;
extern lemlib::Drivetrain drivetrain;

extern pros::Imu imu;
extern pros::Rotation horizontal_sensor;
extern pros::Rotation veritcal_sensor;

extern lemlib::TrackingWheel horitzontal_tracking_wheel;
extern lemlib::TrackingWheel veritcal_tracking_wheel;
extern lemlib::OdomSensors odom;

extern pros::Motor pickupStage;
extern pros::Motor backStage;
extern pros::Motor topStage;

extern pros::Optical colorSensor;

extern std::shared_ptr<Intake> intake;

extern lemlib::ControllerSettings lateral_controller;
extern lemlib::ControllerSettings angular_controller;
extern lemlib::ExpoDriveCurve throttle_curve;
extern lemlib::ExpoDriveCurve steer_curve;

extern pros::adi::DigitalOut matchLoader;
extern pros::adi::DigitalOut blockRush;
extern pros::adi::DigitalOut bunnyEars;



extern std::shared_ptr<lemlib::Chassis> chassis;

extern pros::Controller controller;

extern std::map<int, std::pair<std::string, std::function<void()>>> autonSelectorMap;
extern int currentAutoSelection;

extern bool autoSelected;
extern bool autoActive;





