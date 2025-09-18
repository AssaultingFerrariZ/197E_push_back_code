#include "autons.hpp"
#include "intake.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "definitions.hpp"
#include "pros/optical.h"
#include "pros/rtos.hpp"
#include <format>

float clamp(float output, float min, float max) {
	if (output < min) output = min;
	if (output > max) output = max;
	return output;
}



//Autons defined below
void pidTuning() {
	chassis->setPose(0, 0, 0);
	chassis->turnToHeading(90, 10000);
}

void soloAWP() {
	//set position and descore
	chassis->setPose(0, 0, -90);
	bunnyEars.set_value(true);

	//drive up to match loader
	chassis->moveToPoint(-29.52, 0.288, 1100, {.minSpeed = 20, .earlyExitRange = 0.5});
	chassis->turnToPoint(-29.52, -16.391, 700);
	matchLoader.set_value(true);

	//load blocks
	chassis->moveToPoint(-31.92, -16.391, 1500, {.maxSpeed = 70});
	intake->load(127);
	pros::delay(1250);
	chassis->moveToPoint(-31.92, 0.288, 500, {.forwards = false});
	chassis->moveToPoint(-31.92, -16.391, 1500, {.maxSpeed = 70});

	//back up and score
	chassis->moveToPoint(-32.42, 31.044, 1600, {.forwards = false, .maxSpeed = 67}, false);
	intake->stop();
	bunnyEars.set_value(false);
	chassis->tank(-60, -60);
	pros::delay(250);
	intake->scoreTop(127);
	matchLoader.set_value(false);
	pros::delay(2000);

	//load 3 stack
	chassis->moveToPoint(-34.365, 10.194,  500, {.minSpeed = 100, .earlyExitRange = 2});
	chassis->moveToPoint(-20.365, 10.194,  1000, {.minSpeed = 100, .earlyExitRange = 2});

	chassis->turnToPoint(-3.038, 27.422, 700, {});
	intake->load(127);

	chassis->moveToPoint(-3.038, 27.422, 2500, {.maxSpeed = 35}, false);
	pros::delay(250);
	chassis->turnToPoint(13.5, 45.422, 700, {.forwards = false});
	intake->stop();
	intake->scoreBottom(-127);
	pros::delay(500);
	intake->stop();
	chassis->moveToPoint(13.5, 45.422, 700, {.forwards = false, .maxSpeed = 50}, false);
	intake->scoreMiddle(127);
	// //back up and score in mid goal
	// chassis->moveToPoint(1.438, 43.422, 1000, {.forwards = false}, false);
	// intake->scoreMiddle(127);
	// pros::delay(500);
	// intake->stop();
	// //turn to second 3 stack
	// chassis->moveToPoint(31.632, 29.619, 750);
	// intake->load(127);

	// //score in low goal
	// chassis->turnToPoint(14.953, 43.997, 500);
	// chassis->moveToPoint(14.953, 43.997, 750, {}, false);
	// intake->scoreBottom(127);
	// pros::delay(500);
	// intake->stop();

	// //back up to other match loader
	// chassis->moveToPoint(56.937, -0.575, 1000, {.forwards = false, .minSpeed = 20, .earlyExitRange = 2});
	// chassis->turnToPoint(-55.787, -15.816, 600);

	// //drive up and load blocks
	// chassis->moveToPoint(-55.787, -15.816, 1000);
	// matchLoader.set_value(true);
	// intake->load(127);
	// chassis->waitUntilDone();
	// pros::delay(750);

	// //back up and score
	// chassis->moveToPoint(55.787, 27.893, 1000, {.forwards = false}, false);
	// intake->scoreTop(127);
}

void soloAWPRight() {
	//set position and descore
	chassis->setPose(0, 0, 90);
	bunnyEars.set_value(true);

	//drive up to match loader
	chassis->moveToPoint(28.52, 0.288, 1100, {.minSpeed = 20, .earlyExitRange = 0.5});
	chassis->turnToPoint(28.52, -16.391, 700);
	matchLoader.set_value(true);

	//load blocks
	chassis->moveToPoint(30.92, -16.391, 1500, {.maxSpeed = 70});
	intake->load(127);
	pros::delay(1250);
	chassis->moveToPoint(31.92, 0.288, 500, {.forwards = false});
	chassis->moveToPoint(31.92, -16.391, 1500, {.maxSpeed = 70});

	//back up and score
	chassis->moveToPoint(32.42, 31.044, 1600, {.forwards = false, .maxSpeed = 67}, false);
	intake->stop();
	bunnyEars.set_value(false);
	chassis->tank(-60, -60);
	pros::delay(250);
	intake->scoreTop(127);
	matchLoader.set_value(false);
	pros::delay(2000);

	//load 3 stack
	chassis->moveToPoint(34.365, 10.194,  500, {.minSpeed = 100, .earlyExitRange = 2});
	chassis->moveToPoint(20.365, 10.194,  1000, {.minSpeed = 100, .earlyExitRange = 2});

	chassis->turnToPoint(3.038, 27.422, 700, {});
	intake->load(127);

	chassis->moveToPoint(3.038, 27.422, 2500, {.maxSpeed = 35}, false);
	pros::delay(250);
	chassis->turnToPoint(-13.5, 45.422, 700, {.forwards = false});
	intake->stop();
	intake->scoreBottom(-127);
	pros::delay(500);
	intake->stop();
	chassis->moveToPoint(-15, 45.422, 700, {.forwards = false, .maxSpeed = 50}, false);
	intake->scoreBottom(127);

}

void elims9Ball() {
	//set position and descore
	chassis->setPose(0, 0, 149.87);
	bunnyEars.set_value(true);

	// //initial rush movements
	// chassis->moveToPoint(-10, 15, 2000, {.forwards = false, .minSpeed = 100, .earlyExitRange = 2});
	// chassis->moveToPoint(-10, 29, 2000, {.forwards = false, .minSpeed = 100, .earlyExitRange = 0.5});
	// chassis->moveToPoint(-11.35, 38.75, 1000, {.forwards = false, .minSpeed = 100, .earlyExitRange = 1});

	// //turn and grab blocks
	// chassis->swingToHeading(90, lemlib::DriveSide::RIGHT, 700, {}, false);
	// chassis->moveToPoint(-20, 47, 750, {.forwards = false}, false);
	// // chassis->waitUntilDone();
	// // pros::delay(200);
	// blockRush.set_value(true);

	//initial rush movements
	chassis->moveToPoint(-10, 15, 2000, {.forwards = false, .minSpeed = 100, .earlyExitRange = 2});
	chassis->moveToPoint(-10, 29, 2000, {.forwards = false, .minSpeed = 100, .earlyExitRange = 0.5});
	chassis->moveToPoint(-12.55, 34.85, 1000, {.forwards = false, .minSpeed = 50, .earlyExitRange = 1});

	//turn and grab blocks
	chassis->swingToHeading(90, lemlib::DriveSide::RIGHT, 700, {}, false);
	chassis->moveToPoint(-20, 46, 750, {.forwards = false}, false);
	// chassis->waitUntilDone();
	// pros::delay(200);
	blockRush.set_value(true);

	//grab 3 stack
	pros::delay(500);
	chassis->moveToPoint(8.5, 10, 1500, {.maxSpeed = 40, .minSpeed = 20, .earlyExitRange = 4});
	intake->load(127);
	chassis->moveToPoint(9.75, 10, 1500, {.maxSpeed = 40}, false);
	pros::delay(250);



	// //move to match load
	// chassis->turnToPoint(-21.606, -0.052, 750, {.forwards = false, .maxSpeed = 65, .minSpeed = 40, .earlyExitRange = 10}, false);
	// chassis->moveToPoint(-21.606, 3.052, 1950, {.forwards = false, .maxSpeed = 80, .minSpeed = 20, .earlyExitRange = 2}, false);
	// chassis->turnToHeading(90, 500, {.maxSpeed = 60}, false);
	// pros::delay(250);
	// blockRush.set_value(false);
	// pros::delay(250);
	chassis->turnToHeading(90, 1000, {.maxSpeed = 60}, false);
	intake->stop();
	blockRush.set_value(false);
	pros::delay(250);
	chassis->moveToPoint(15, 10.5, 700);
	chassis->turnToPoint(-30, 5, 500);
	chassis->moveToPoint(-27.5, 5, 1500, {.maxSpeed = 80});
	intake->load(127);



	// //setup rush
	// chassis->moveToPoint(-10, 0, 1500);
	// chassis->turnToPoint(-28, 16, 250, {.forwards = false});
	// intake->stop();
	

	// chassis->moveToPoint(-28, 16, 1000, {.forwards = false, .minSpeed = 30, .earlyExitRange = 4});
	chassis->turnToPoint(-32, -11.5, 500, {}, false);
	//grab match loads
	// chassis->moveToPoint(-30, 0.5, 1000, {.maxSpeed = 8});
	// intake->load(127);
	// chassis->waitUntilDone();
	matchLoader.set_value(true);
	pros::delay(250);
	chassis->moveToPoint(-32, -11.5, 1000, {.maxSpeed = 88});
	chassis->waitUntilDone();
	pros::delay(500);
	//score
	chassis->moveToPoint(-29, 26.593, 950, {.forwards = false, .maxSpeed = 70, .minSpeed = 40}, false);
	chassis->tank(-60, -60);
	intake->scoreTop(127);


	

	
}

void skills() {
	chassis->setPose(0, 0, -90);
	bunnyEars.set_value(true);

	//drive up to match loader
	chassis->moveToPoint(-29.52, 0.288, 1100, {.minSpeed = 20, .earlyExitRange = 0.5});
	chassis->turnToPoint(-29.52, -16.391, 700);
	matchLoader.set_value(true);

	//load blocks
	chassis->moveToPoint(-34.92, -16.391, 1000, {.maxSpeed = 70});
	intake->load(127);
	pros::delay(1250);
	chassis->moveToPoint(-32.92, 0.288, 500, {.forwards = false});
	chassis->moveToPoint(-32.92, -16.391, 1000, {.maxSpeed = 50});
	chassis->moveToPoint(-32.92, 0.288, 500, {.forwards = false});
	chassis->moveToPoint(-32.92, -16.391, 1000, {.maxSpeed = 50});
	chassis->moveToPoint(-32.92, 0.288, 500, {.forwards = false});
	chassis->moveToPoint(-32.92, -16.391, 1000, {.maxSpeed = 50});
	chassis->waitUntilDone();
	pros::delay(500);
	chassis->moveToPoint(-32.53, 31.044, 1600, {.forwards = false, .maxSpeed = 47}, false);
	intake->stop();
	chassis->tank(-60, -60);
	pros::delay(250);
	intake->scoreTop(127);
	matchLoader.set_value(false);
	pros::delay(3250);

	chassis->moveToPoint(-20.365, 10.194,  1000, {.minSpeed = 70, .earlyExitRange = 2});

	chassis->turnToPoint(2.438, 32.422, 700, {});
	intake->load(127);

	chassis->moveToPoint(2.438, 32.422, 3500, {.maxSpeed = 30});

	chassis->turnToPoint(-24.893, 7.273, 700, {.forwards = false});
	intake->stop();
	chassis->moveToPoint(-24.893, 7.273, 1700, {.forwards = false});
	chassis->turnToPoint(-29.577, 27.51, 700, {.forwards = false});
	chassis->moveToPoint(-29.577, 27.51, 2000, {.forwards = false, .maxSpeed = 55}, false);
	chassis->tank(-60, -60);
	intake->scoreTop(127);
	pros::delay(2500);

	chassis->moveToPoint(-34, 10, 1000);
	intake->stop();
	chassis->turnToPoint(63.019, 3.795, 500);
	chassis->moveToPoint(63.019, 3.795, 2000, {});
	chassis->turnToPoint(63.387, -27.075, 500);
	matchLoader.set_value(true);
	chassis->moveToPoint(63.387, -27.075, 1500, {.maxSpeed = 70});
	intake->load(127);
	chassis->moveToPoint(63.92, 0.288, 500, {.forwards = false});
	chassis->moveToPoint(63.92, -26.391, 1000, {.maxSpeed = 50});
	chassis->moveToPoint(63.92, 0.288, 500, {.forwards = false});
	chassis->moveToPoint(63.92, -26.391, 1000, {.maxSpeed = 50});
	chassis->moveToPoint(63.019, 29.091, 1500, {.forwards = false, .maxSpeed = 55}, false);
	intake->stop();
	chassis->tank(-60, -60);
	pros::delay(250);
	intake->scoreTop(127);
	matchLoader.set_value(false);
	pros::delay(2250);

	chassis->moveToPoint(63.703, 14.229, 1000);
	chassis->moveToPoint(26.877, -25.596, 1000);
	chassis->turnToPoint(7.273, -25.913, 600, {.forwards = false});
	chassis->moveToPoint(7.273, -25.913, 1000, {.forwards = false});
	chassis->waitUntil(15);
	blockRush.set_value(false);

}