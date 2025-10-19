#include "main.h"
#include "intake.hpp"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "definitions.hpp"
#include "autons.hpp"
#include "liblvgl/llemu.hpp"
#include "pros/colors.hpp"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/optical.h"
#include "pros/rtos.hpp"
#include "subsystem.hpp"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <ostream>
#include <vector>


/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */

bool stopIntakeControl = false;


template<typename T> std::vector<int> vectorCompare(std::vector<T> v1, std::vector<T> v2) {
	std::vector<int> error_vals = {};
	if (v1.size() != v2.size()) return {-1};
	for (int i = 0; i < v1.size(); i++) error_vals.push_back(std::abs(v2[i]-v1[i]));
	return error_vals;
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



SubsystemHandler* handler = NULL;
// SubsystemPtr screen(new Subsystem());
void initialize() {
    pros::lcd::initialize(); // initialize brain screen
    std::cout<<"Initialized Program" << std::endl;
	chassis->calibrate();          // starts odom task
	colorSensor.set_integration_time(20);
	// std::cout<<"Hello Prabhas it is i" <<std::endl;
	// std::cout<<"basically what you want to do is press the down button and immediately start spinning the bot using the stick, after you configure odom"<<std::endl;
	// std::cout<<"the down button should automate the recording process of the data"<<std::endl;
	// std::cout<<"alternatively if the automated process does not work, don't worry about it and just spam the up button like we did the first time"<<std::endl;
	// std::cout<<"\n\n\n"<<std::endl;

	colorSensor.set_led_pwm(100);
	intake->allianceColor = Intake::RED;
	intake->color_sorting_enabled = true;
	handler = new SubsystemHandler({intake});


	pros::Task antiJam([&] {
		std::cout << "anti jam running" << std::endl;
		std::vector<int> prevVelocity = intake->get_actual_velocity();
		bool in_threshold = false;
		while (1) {
			if (intake->anti_jam_enabled) {
				for (int i = 0; i < 3; i++) {
					if (intake->get_target_velocity()[i]-intake->get_actual_velocity()[i] > 550) in_threshold = true;
				}
				// std::cout<<"prev velo:  " << prevVelocity<<std::endl;

				auto errors = vectorCompare(intake->get_target_velocity(), intake->get_actual_velocity());
				auto nearZero = vectorCompare(prevVelocity, {0, 0, 0});

				if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
					std::cout<<"Timestamp: "<<pros::millis()<<"\n\tin_threshold: "<<in_threshold<<"\n\terrors: "<<*std::max_element(errors.begin(), errors.end())<<"\n\tnearZero: "<<*std::max_element(nearZero.begin(), nearZero.end())<<std::endl;
					std::cout<<"\tPrev Velocity 1, 2, 3: "<<prevVelocity[0]<<", "<<prevVelocity[1]<<", "<<prevVelocity[2]<<std::endl;
					std::cout<<"\tCurrent Velocity 1, 2, 3: "<<intake->get_actual_velocity()[0]<<", "<<intake->get_actual_velocity()[1]<<", "<<intake->get_actual_velocity()[2]<<std::endl;
					std::cout<<"\t\tTRIGGER ANTI JAM: "<<(in_threshold && *std::max_element(errors.begin(), errors.end())>100 && *std::max_element(nearZero.begin(), nearZero.end())<100)<<std::endl;
				}
				if (in_threshold && *std::max_element(errors.begin(), errors.end())>100 && *std::max_element(nearZero.begin(), nearZero.end())<100) {
					intake->antiJam();
					in_threshold = false;
				} 
				prevVelocity = intake->get_actual_velocity();
				pros::delay(20);
			}
		}
	});

	pros::Task colorSorter([&] {
		while (!autoSelected) {
			pros::delay(20);
		}
		bool toSort = false;
		using Intake::RED, Intake::BLUE, Intake::UNKNOWN;
		while (1) { //infinitely loop the task for the program's lifetime
			//determine if the ring is red or blue by comparing RGB values
			Intake::blockColor currentBlock = UNKNOWN;
			double hue = colorSensor.get_hue();
			if (colorSensor.get_proximity() > 90 && intake->color_sorting_enabled) {
				static int j = 0;
				if (hue < 65 && hue > 0 && intake->allianceColor == BLUE) {
					currentBlock = RED;
					controller.print(0, 0, "red");
					// std::cout<<"registered red"<<std::endl;
				} 
				
				if (hue < 300 && hue > 75 && intake->allianceColor == RED) {
					currentBlock = BLUE;
					controller.print(0, 0, "blu");
					// std::cout<<"registered blue"<<std::endl;

				}
				j++;
				// controller.print(0, 5, "%d", j);
				// std::cout<<"toSort: "<<toSort<<std::endl;

				// std::cout<<"colorSortEnabled: "<<intake->color_sorting_enabled<<std::endl;


				if (intake->color_sorting_enabled && intake->allianceColor != currentBlock && currentBlock != UNKNOWN) toSort = true;
				
				pros::lcd::print(1, "toSort: %i", toSort);

				if (toSort) {
					static int i = 0;
					i++;
					controller.print(1, 3 , "%d", i);
					// stopIntakeControl = true;
					
					intake->colorSort();
					stopIntakeControl = false;
					toSort = false; 
				}
			}
			pros::delay(10);
		}
	});	

	

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
	intake->color_sorting_enabled = false;
	intake->anti_jam_enabled = true;
    chassis->cancelAllMotions();
	colorSensor.set_led_pwm(100);
    pros::delay(20);
    // pros::lcd::print(3, "This is now opcontrol!")

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
			while (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {}
		}
		
	}
    // loop forever
    while (true) {
        // get left y and right x positions
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        // move the robot
        chassis->arcade(leftY, rightX);
		// if (!stopIntakeControl) {
			if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
				intake->load(127);
			} else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
				intake->doubleParkMacro();
			} else if (controller.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_A)) {
				intakeLift.set_value(true);
			} else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
				intake->scoreBottom(127);
			} else if (controller.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_B)) {
				intakeLift.set_value(false);	
			} else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
				intake->outake(127);	
			} else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
				intake->scoreTop(127);
			} else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
				intake->scoreMiddle(127);
			} else {   
				intake->stop();
			}
		// }
		// if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
		// 	std::cout<<"X: "<<chassis->getPose().x<<", Y: "<<chassis->getPose().y<<", Heading: "<<wrap360(chassis->getPose().theta)<<std::endl;
		// }

		// if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
		// 	pros::Task recordData([] {
		// 	for (int i = 0; i < 150; i++) {
		// 		std::cout<<"X: "<<chassis->getPose().x<<", Y: "<<chassis->getPose().y<<", Heading: "<<wrap360(chassis->getPose().theta)<<std::endl;
		// 		pros::delay(20);
		// 	}});
			
		// }
		
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) intake->clearQueue();

		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
			static bool scraperState = false;
			scraperState = !scraperState;
			matchLoader.set_value(scraperState);
		}


		if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
			static bool bunnyEarsState = false;
			bunnyEarsState = !bunnyEarsState;
			bunnyEars.set_value(bunnyEarsState);
		}

		// if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
		lemlib::Pose pose = chassis->getPose();

		pros::lcd::print(1, "X: %.2f in\n Y: %.2f in\n Heading: %.2f deg\n",
				pose.x, pose.y, pose.theta);

        // }
		//delay to save resources
        pros::delay(20);
    }
}