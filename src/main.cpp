#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "definitions.hpp"
#include "autons.hpp"
#include "pros/llemu.hpp"
#include "subsystem.hpp"
#include <memory>


/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}



float wrap360(float angle) {
	while (!(angle >= 0 && angle < 360)) {
		if (angle < 0) angle += 360;
		if (angle >= 360) angle -= 360;
		pros::delay(10);
	}
	return angle;
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
// this runs at the start of the program





void initialize() {
    pros::lcd::initialize(); // initialize brain screen
    std::cout<<"Initialized Program" << std::endl;
    chassis->calibrate();
    pros::Task screenTask( [&] {
        while (1) { // infinite loop
            // print measurements from the horizontal sensor
            // pros::lcd::print(0, "Horizontal Sensor: %i", horizontal_sensor.get_position());
            // // print measurements from the vertical sensor
            // pros::lcd::print(1, "Veritcal Sensor: %i", veritcal_sensor.get_position());
            
            pros::lcd::print(0, "%.2f Heading", wrap360(chassis->getPose().theta));  // Prints status of the emulated screen LCDs
			pros::lcd::print(1, "%.2f X", chassis->getPose().x);  // Prints status of the emulated screen LCDs
			pros::lcd::print(2, "%.2f Y", chassis->getPose().y);  

            pros::delay(20); // delay to save resources. DO NOT REMOVE

        }
    });
	SubsystemHandler handler({intake});
    controller.clear();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	autoSelected = true;
	autoActive = true;
	auto selection = autonSelectorMap.find(currentAutoSelection);
	if (selection != autonSelectorMap.end()) {
		selection->second.second();
	}

	autoActive = false;
}


void opcontrol() {
    autoActive = false;
    chassis->cancelAllMotions();
    pros::delay(20);
    pros::lcd::print(3, "This is now opcontrol!");
    if (!autoSelected) controller.print(2, 1, autonSelectorMap[currentAutoSelection].first.c_str());
	while (!autoSelected && !pros::c::competition_is_connected()) {
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
			controller.clear_line(2);
			pros::delay(50);
			currentAutoSelection = (int)clamp(currentAutoSelection-1, 1, autonSelectorMap.size());
			controller.print(2, 1, autonSelectorMap[currentAutoSelection].first.c_str());
		}
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
			controller.clear_line(2);
			pros::delay(50);
			currentAutoSelection = (int)clamp(currentAutoSelection+1, 1, autonSelectorMap.size());
			controller.print(2, 1, autonSelectorMap[currentAutoSelection].first.c_str());
		}
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
			controller.clear();
			autoSelected = true;
		}
	}
    // loop forever
    while (true) {
        // get left y and right x positions
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        // move the robot
        chassis->arcade(leftY, rightX);

		if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
			intake->load(127);
		} else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
			intake->scoreBottom(127);
		} else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
			intake->scoreTop(127);
		} else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
			intake->scoreMiddle(127);
		} else {
			intake->stop();
		}

		// delay to save resources
        pros::delay(20);
    }
}