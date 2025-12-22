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


template<typename T> std::vector<T> vectorCompare(std::vector<T> v1, std::vector<T> v2) {
	std::vector<T> error_vals = {};
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

		int jamCounter[2] = {0, 0};  // per-stage persistence
		bool in_threshold = false;

		while (true) {
			if (!intake->anti_jam_enabled) {
				pros::delay(50);
				continue;
			}

			if (!intake->isIntaking() || intake->loading) {
				in_threshold = false;
				jamCounter[0] = jamCounter[1];
				pros::delay(20);
				continue;
			}

			auto target = intake->get_target_velocity();
			auto actual = intake->get_actual_velocity();

			// --- thresholds chosen from your data ---
			int stageThresholds[2] = {250, 100};   // pickup, back
			int persistCycles[2]  = {5, 3};          // require jam for ~100 ms on back/top

			for (int i = 0; i < 2; i++) {
				if ((std::abs(target[i] - actual[i]) > stageThresholds[i]) &&
					(std::abs(actual[i]) < 50) && (std::abs(target[i]) > 100)) {
					jamCounter[i]++;
				} else {
					jamCounter[i] = 0;
				}

			}

			// detect if *any* stage jammed long enough
			bool jamDetected = false;
			for (int i = 0; i < 2; i++) {
				if (jamCounter[i] >= persistCycles[i])
					jamDetected = true;
			}

			if (jamDetected && intake->isIntaking()) {
				std::cout << "JAM DETECTED — activating anti-jam\n";
				intake->antiJam();
				jamCounter[0] = jamCounter[1];
			}

			pros::delay(70);
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
				controller.print(0, 5, "%d", j);
				// std::cout<<"toSort: "<<toSort<<std::endl;

				// std::cout<<"colorSortEnabled: "<<intake->color_sorting_enabled<<std::endl;


				if (intake->color_sorting_enabled && intake->allianceColor != currentBlock && currentBlock != UNKNOWN) toSort = true;
				
				// pros::lcd::print(1, "toSort: %i", toSort);

				if (toSort) {
					static int i = 0;
					i++;
					controller.print(1, 3 , "%d", i);
					// stopIntakeControl = true;
					
					if (!intake->sort_out_bottom) intake->colorSort();
					else {
						intake->outake(127);
						pros::delay(1050);
						intake->scoreTop(127);
					}
					stopIntakeControl = false;
					toSort = false; 
				}
			}
			pros::delay(10);
		}
	});	

	pros::Task colorStop([&] {
		while (!autoSelected) {
			pros::delay(20);
		}
		while(1) {
			double hue = colorSensor.get_hue();
			if(intake->color_stop_enabled && intake->allianceColor == Intake::RED) {
				if(colorSensor.get_proximity() > 90 && (hue < 300 && hue > 75)) {
					intake->stop();
				}
			}

			else if(intake->color_stop_enabled && intake->allianceColor == Intake::BLUE) {
				if(colorSensor.get_proximity() > 90 && (hue < 65 && hue > 0)) {
					intake->stop();
				}
			}
		pros::delay(10);
	} 

	});
	
	pros::Task liveScoring([&] {
		//INIT
		//clear controller screen of default behavior
		liveScorer.clear();

		//set up score variables
		int netScore = 0; 
		//use this to know when to update screen. if net != prev, screen updates
		int prevScore = netScore;

		//set up enum for bonus given for ease of use
		enum bonusGiven {ALLIANCE = 1, OPPONENT = -1, NEUTRAL = 0};

		//initial print
		liveScorer.print(1, 1, "Net Score: %d", netScore);

		//while loop:
		while (1) {

			//set up bonus variables
			static bonusGiven longGoal1 = NEUTRAL;
			static bonusGiven longGoal2 = NEUTRAL;
			static bonusGiven midGoal = NEUTRAL;
			static bonusGiven lowGoal = NEUTRAL;
			static bonusGiven autoBonus = NEUTRAL;


			//long goal control buttons
			//behavior: press button to select state
			//if button pressed is already in correct state, set to neutral
			if (liveScorer.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
				if (longGoal1 == ALLIANCE) longGoal1 = NEUTRAL;
				else longGoal1 = ALLIANCE;
			}

			if (liveScorer.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
				if (longGoal1 == OPPONENT) longGoal1 = NEUTRAL;
				else longGoal1 = OPPONENT;
			}

			if (liveScorer.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
				if (longGoal2 == ALLIANCE) longGoal2 = NEUTRAL;
				else longGoal2 = ALLIANCE;
			}

			if (liveScorer.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
				if (longGoal2 == OPPONENT) longGoal2 = NEUTRAL;
				else longGoal2 = OPPONENT;
			}

			//middle goal buttons
			if (liveScorer.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
				if (midGoal == ALLIANCE) midGoal = NEUTRAL;
				else midGoal = ALLIANCE;
			}

			if (liveScorer.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
				if (midGoal == OPPONENT) midGoal = NEUTRAL;
				else midGoal = OPPONENT;
			}

			//low goal buttons
			if (liveScorer.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
				if (lowGoal == ALLIANCE) lowGoal = NEUTRAL;
				else lowGoal = ALLIANCE;
			}

			if (liveScorer.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
				if (lowGoal == OPPONENT) lowGoal = NEUTRAL;
				else lowGoal = OPPONENT;
			}

			//auto toggle
			//behavior: cycle between neutral, alliance, opponent
			if (liveScorer.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
				switch (autoBonus) {
					case NEUTRAL:
						autoBonus = ALLIANCE;
						break;
					case ALLIANCE:
						autoBonus = OPPONENT;
						break;
					case OPPONENT:
						autoBonus = NEUTRAL;
						break;
					default:
						autoBonus = NEUTRAL;
						break;
				}
			}

			//calculate and score
			netScore = (10*longGoal1) + (10*longGoal2) + (8*midGoal) + (6*lowGoal) + (10*autoBonus);

			//print if there is a score change as to not overload controller
			if (netScore != prevScore) liveScorer.print(1, 1, "Net Score: %d", netScore);

			//set prevScore to netScore for next loop
			prevScore = netScore;
			pros::delay(20);
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
	intake->anti_jam_enabled = false;
	intake->allianceColor = Intake::RED;
    chassis->cancelAllMotions();
	colorSensor.set_led_pwm(100);
    pros::delay(20);
	intake->color_stop_enabled = false;
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
		if (!intake->anti_jam_running) {
			if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
				intake->load(127);
			// } else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
			// 	intake->doubleParkMacro();
			} else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
				intake->fillMiddle(127);
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
		}
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
			std::cout<<"X: "<<chassis->getPose().x<<", Y: "<<chassis->getPose().y<<", Heading: "<<wrap360(chassis->getPose().theta)<<std::endl;
		}

		// if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
		// 	pros::Task recordData([] {
		// 	for (int i = 0; i < 150; i++) {
		// 		std::cout<<"X: "<<chassis->getPose().x<<", Y: "<<chassis->getPose().y<<", Heading: "<<wrap360(chassis->getPose().theta)<<std::endl;
		// 		pros::delay(20);
		// 	}});
			
		// }
		
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
			static bool scraperState = false;
			scraperState = !scraperState;
			matchLoader.set_value(scraperState);
		}


		if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
			static bool descoreState = false;
			descoreState = !descoreState;
			descore.set_value(descoreState);
			static bool hoodState = false;
			hoodState = true;
			hood.set_value(hoodState);
		}

		if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
			static bool odomLiftState = false;
			odomLiftState = !odomLiftState;
			odomLift.set_value(odomLiftState);
		}

		if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
			static bool distSensorState = false;
			distSensorState = !distSensorState ;
			doublePark.set_value(distSensorState);
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
