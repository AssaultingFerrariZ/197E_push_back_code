#include "autons.hpp"
#include "intake.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "definitions.hpp"
#include "pros/optical.h"
#include "pros/rtos.hpp"
#include <algorithm>
#include <format>

float clamp(float output, float min, float max) {
	if (output < min) output = min;
	if (output > max) output = max;
	return output;
}



//Autons defined below
void pidTuning() {
	intake->allianceColor =  Intake::RED;
	intake->color_sorting_enabled = true;
	intake->anti_jam_enabled = true;
	intake->scoreTop(127);

	// chassis->setPose(0, 0, 0);
	// // chassis->moveToPoint(0, 30, 2000);
	// chassis->turnToHeading(90, 1000);
}


void elims7BallRedLeft() {
	//set position and descore
	chassis->setPose(0, 0, 0);
	intake->allianceColor = Intake::RED;
	intake->color_sorting_enabled = true;
	intake->anti_jam_enabled = true;
	bunnyEars.set_value(true);
	
	//intake 3 group
	chassis->moveToPoint(-4.313, 20.992, 1500);
	intake->load(127);
	chassis->waitUntil(15);
	matchLoader.set_value(true);

	//go to match loader
	chassis->turnToPoint(-34.194, 3.451, 750, {});
	chassis->moveToPoint(-34.194, 3.451, 1250, {});
	chassis->turnToPoint(-33, -11.215, 700);

	//match load
	chassis->moveToPoint(-33, -11.215, 1000, {});
	chassis->waitUntilDone();
	chassis->tank(30, 30);
	pros::delay(1425);
	chassis->cancelAllMotions();

	//score
	chassis->turnToPoint(-32, 27.031, 300, {.forwards = false, .minSpeed = 60, .earlyExitRange = 1});
	chassis->moveToPoint(-32, 27.031, 1500, {.forwards = false, .maxSpeed = 60}, false);
	intake->scoreTop(-127);
	pros::delay(250);
	intake->stop();
	pros::delay(250);
	bunnyEars.set_value(false);
	chassis->waitUntilDone();
	chassis->tank(-60, -60);
	intake->scoreTop(127);
	pros::delay(3500);

	//ram w hood
	chassis->tank(100, 100);
	pros::delay(200);
	chassis->tank(0, 0);
	bunnyEars.set_value(true);
	pros::delay(500);
	chassis->moveToPoint(-32, 27.031, 700, {.forwards = false, .minSpeed = 127});
	chassis->tank(-60, -60);
}

void elims7BallRedRight() {

}

void elims7BallBlueRight() {

}

void elims7BallBlueLeft() {

}

void soloAWPRedLeft() {
	chassis->setPose(0, 0, -90);
	intake->allianceColor = Intake::RED;
	intake->color_sorting_enabled = true;
	intake->anti_jam_enabled = true;
	bunnyEars.set_value(true);

	//go to first match loader and score in goal
	chassis->moveToPoint(-32.733, 0, 1200);
	chassis->turnToPoint(-33.5, -13.116, 500);
	intake->load(127);
	matchLoader.set_value(true);
	chassis->moveToPoint(-33.5, -13.116, 700);
	chassis->waitUntilDone();
	chassis->tank(50, 50);
	pros::delay(1450); 
	chassis->moveToPoint(-33.5, 15.893, 700, {.forwards = false, .minSpeed = 40, .earlyExitRange = 1});
	chassis->moveToPoint(-33.5, 27.893, 700, {.forwards = false, .maxSpeed = 60});
	chassis->tank(-50, -50);
	intake->scoreTop(127);
	pros::delay(1000);
	intake->stop();
	chassis->tank(0, 0);

	//go to 3 pile in score in mid goal
	chassis->moveToPoint(-32.8, 9.49, 400, {.minSpeed = 100});
	chassis->turnToPoint(0.09, 28.756, 400);
	matchLoader.set_value(false);
	intake->load(127);
	chassis->moveToPoint(0.09, 28.756, 1300);
	chassis->waitUntil(18);
	matchLoader.set_value(true);
	//part 
	chassis->turnToPoint(10.438, 42.559, 500, {.forwards = false});
	chassis->moveToPoint(10.438, 42.559, 800, {.forwards = false, .maxSpeed = 70});
	matchLoader.set_value(false);
	chassis->waitUntilDone();
	intake->scoreMiddle(127);
	pros::delay(2000);

	//go intake other 3 pile and go to match loader
	chassis->moveToPoint(0, 42.559-14.438, 600);
	chassis->turnToPoint(44.344, 25, 500);
	chassis->moveToPoint(44.344, 25, 800);
	chassis->waitUntil(20); 
	matchLoader.set_value(true);
	chassis->turnToPoint(10.061, 50.739, 600);
	chassis->moveToPoint(10.061, 50.739, 900, {}, false);
	matchLoader.set_value(false);
	intake->scoreBottom(127);

	// //intake from match load and score
	// chassis->turnToPoint(54.637, -16.679, 600);
	// chassis->moveToPoint(54.637, -16.679, 1000);
	// chassis->waitUntilDone();
	// pros::delay(750);
	// chassis->moveToPoint(55.212, 27.893, 1500, {.forwards = false, .maxSpeed = 70}, false);
	// intake->scoreTop(127);
}

void soloAWPBlueLeft() {
	
}

void skills() {
	chassis->setPose(6.039, 11.502, 0); //don't ask questions.
	intake->color_sorting_enabled = false;
	bunnyEars.set_value(true);

	//grab one red block
	chassis->turnToHeading(65, 700);
	chassis->moveToPoint(8.641, 29.469, 1500, {.maxSpeed = 80});
	intake->load(127);

	//go to and match load
	chassis->turnToPoint(30.233, 13.327, 600);
	chassis->moveToPoint(30.233, 13.327, 1000);
	chassis->turnToPoint(30.233, -3, 600);
	matchLoader.set_value(true);
	intake->load(127);
	chassis->moveToPoint(30.233, -3, 800);
	chassis->waitUntilDone();
	chassis->tank(20, 20);
	pros::delay(1750);

	//score
	chassis->moveToPoint(30.033, 35.945, 1500, {.forwards = false, .maxSpeed = 70});
	intake->stop();
	chassis->waitUntilDone();
	chassis->tank(-60, -60);
	intake->scoreTop(127);
	pros::delay(3500);
	chassis->tank(0, 0);
	chassis->cancelAllMotions();
	chassis->moveToPoint(30.033, 23, 2500, {.maxSpeed = 50});
	chassis->moveToPoint(30.033, 35.945, 1500, {.forwards = false, .maxSpeed = 50});

	//go to other match loader
	chassis->moveToPoint(40.147, 12.94, 1000);
	matchLoader.set_value(false);
	chassis->turnToPoint(47.048, 78.217, 500, {.forwards = false});
	chassis->moveToPoint(47.048, 78.217, 1500, {.forwards = false});
	chassis->moveToPoint(30.933, 110.136, 2500, {.forwards = false});
	chassis->turnToPoint(30.933, 129.115, 1000);
	matchLoader.set_value(true);
	intake->load(127);
	chassis->moveToPoint(30.933, 129.115, 1000, {.maxSpeed = 80}, false);
	chassis->tank(20, 20);
	pros::delay(1500);

	//score
	chassis->moveToPoint(30.733, 84.543, 1500, {.forwards = false, .maxSpeed = 70}, false);
	chassis->tank(-60, -60);
	intake->scoreTop(127);
	pros::delay(1050);
	chassis->cancelAllMotions();
	chassis->tank(0, 0);
	intake->stop();
	bunnyEars.set_value(true);

	//go to third match loader
	chassis->moveToPoint(30.733, 103.784, 1000);
	matchLoader.set_value(false);
	chassis->turnToPoint(-64.662, 103.784, 700);
	chassis->moveToPoint(-64.662, 103.784, 2500);

	//unload match loader
	chassis->turnToPoint(-63.662, 129.115, 850);
	intake->load(127);
	matchLoader.set_value(true);
	chassis->moveToPoint(-63.662, 129.115, 1000);
	chassis->waitUntilDone();
	chassis->tank(20, 20);
	pros::delay(1750);
	chassis->tank(0, 0);
	intake->stop();

	//score
	chassis->moveToPoint(-63.662, 82.817, 1400, {.forwards = false, .maxSpeed = 80}, false);
	chassis->tank(-60, -60);
	intake->scoreTop(127);
	pros::delay(2500);
	intake->stop();
	chassis->tank(0, 0);






}