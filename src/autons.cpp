#include "autons.hpp"
#include "intake.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "definitions.hpp"
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
	chassis->moveToPoint(-36.52, 0.288, 1000, {.minSpeed = 20, .earlyExitRange = 2});
	chassis->turnToPoint(-36.52, -16.391, 600);

	//load blocks
	chassis->moveToPoint(-36.52, -16.391, 1000);
	matchLoader.set_value(true);
	chassis->waitUntilDone();
	intake->load(127);
	pros::delay(1000);

	//back up and score
	chassis->moveToPoint(-38.533, 29.044, 1000, {.forwards = false});
	intake->stop();
	chassis->waitUntilDone();
	intake->scoreTop(127);
	pros::delay(750);

	//load 3 stack
	chassis->moveToPoint(-12.365,30.194,  1000, {.minSpeed = 20, .earlyExitRange = 2});
	intake->load(127);
	chassis->turnToPoint(1.438, 43.422, 600, {.forwards = false});

	//back up and score in mid goal
	chassis->moveToPoint(1.438, 43.422, 1000, {.forwards = false}, false);
	intake->scoreMiddle(127);
	pros::delay(500);
	intake->stop();
	//turn to second 3 stack
	chassis->moveToPoint(31.632, 29.619, 750);
	intake->load(127);

	//score in low goal
	chassis->turnToPoint(14.953, 43.997, 500);
	chassis->moveToPoint(14.953, 43.997, 750, {}, false);
	intake->scoreBottom(127);
	pros::delay(500);
	intake->stop();

	//back up to other match loader
	chassis->moveToPoint(56.937, -0.575, 1000, {.forwards = false, .minSpeed = 20, .earlyExitRange = 2});
	chassis->turnToPoint(-55.787, -15.816, 600);

	//drive up and load blocks
	chassis->moveToPoint(-55.787, -15.816, 1000);
	matchLoader.set_value(true);
	intake->load(127);
	chassis->waitUntilDone();
	pros::delay(750);

	//back up and score
	chassis->moveToPoint(55.787, 27.893, 1000, {.forwards = false}, false);
	intake->scoreTop(127);
}

void elims9Ball() {
	// //set position and descore
	// chassis->setPose(0, 0, 180); 
	// descore.set_value(true);

	// //initial rush movements
	// chassis->moveToPoint(-18.404, 25.881, 750, {.forwards = false, .minSpeed = 20, .earlyExitRange = 2});
	// chassis->moveToPoint(-20.704, 37.383, 625, {.forwards = false});

	// //turn and grab blocks
	// chassis->turnToPoint(-28.181, 45.435, 350, {.forwards = false}, false);
	// blockRush.set_value(true);

	// //grab 3 stack
	// chassis->turnToPoint(-4.026, 26.456, 450);
	// chassis->moveToPoint(-4.026, 26.456, 650);
	// intake->load(127);

	// //move to match loader
	// chassis->turnToPoint(-27.606, -8.052, 450, {.forwards = false});
	// blockRush.set_value(false);
	// chassis->turnToPoint(-27.606, -8.052, 450);
	// chassis->moveToPoint(-27.606, -8.052, 750);

	// //grab match loads
	// chassis->turnToPoint(-27.031, -19.267, 300, {}, false);
	// matchLoader.set_value(true);
	// chassis->moveToPoint(-27.031, -19.267, 450, {.maxSpeed = 30}, false);
	// pros::delay(550);

	// //score
	// chassis->moveToPoint(-29.331, 25.593, 750, {}, false);
	// intake->scoreTop(127);

	chassis->setPose(0, 0, 149.55);
	bunnyEars.set_value(true);
	chassis->moveToPoint(-18.5, 39, 1500, {.forwards = false});
	chassis->turnToHeading(120, 800);
// 	blockRush.set_value(true);
}
