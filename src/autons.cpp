#include "autons.hpp"
#include "intake.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "definitions.hpp"
#include "pros/llemu.hpp"
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
	// intake->scoreTop(127);
	odomLift.set_value(true);
	// chassis->setPose(0, 0, 0);
	// // chassis->moveToPoint(0, 30, 2000);
	// chassis->turnToHeading(90, 1000);
}


void elims7BallRedLeft() {
	
	chassis->setPose(0, 0, 0);
	intake->allianceColor = Intake::RED;
	intake->color_sorting_enabled = false;
	intake->anti_jam_enabled = true;
	hood.set_value(true);
	intake->color_stop_enabled = true;
	
	//intake 3 group
	chassis->moveToPoint(-8.25, 25, 1500);
	intake->load(127);
	chassis->waitUntil(13);
	matchLoader.set_value(true);

	//go to match loader
	chassis->turnToPoint(-34, 3.451, 750, {});
	chassis->moveToPoint(-34, 3.451, 1250, {});
	chassis->turnToPoint(-34, -16.215, 700);

	//match load
	chassis->moveToPoint(-34, -16.215, 1000, {.maxSpeed = 70});
	chassis->waitUntilDone();
	chassis->tank(7, 7);
	pros::delay(200);
	chassis->cancelAllMotions();


	//score in long goal
	chassis->moveToPoint(-34, 27.031, 1500, {.forwards = false, .maxSpeed = 70}, false);
	chassis->tank(-60, -60);
	topStage.move(127);
	backStage.move(127);
	pickupStage.move(-127);
	pros::delay(250);
	intake->stop();
	pros::delay(250);
	hood.set_value(false);
	pros::delay(250);
	chassis->waitUntilDone();
	intake->scoreTop(127);
	pros::delay(2050);


	//ram w hood
	chassis->tank(60, 60);
	pros::delay(400);
	chassis->tank(0, 0);
	pros::delay(100);
	chassis->tank(-90, -90);
	hood.set_value(true);
	pros::delay(700);
	chassis->moveToPoint(-34, 27.031, 700, {.forwards = false, .minSpeed = 110});


}


void elims7BallRedRight() {
	chassis->setPose(0, 0, 0);
	intake->allianceColor = Intake::RED;
	intake->color_sorting_enabled = false;
	intake->anti_jam_enabled = true;
	hood.set_value(true);
	intake->color_stop_enabled = true;
	
	//intake 3 group
	chassis->moveToPoint(7.25, 25, 1500);
	intake->load(127);
	chassis->waitUntil(13.5);
	matchLoader.set_value(true);

	//go to match loader
	chassis->turnToPoint(30.25, 3.451, 750, {});
	chassis->moveToPoint(30.25, 3.451, 1250, {});
	chassis->turnToPoint(30.25, -16.215, 700);

	//match load
	chassis->moveToPoint(30.25, -16.215, 1000, {.maxSpeed = 70});
	chassis->waitUntilDone();
	chassis->tank(7, 7);
	pros::delay(400);
	chassis->cancelAllMotions();


	//score in long goal
	chassis->moveToPoint(30, 27.031, 1500, {.forwards = false, .maxSpeed = 70}, false);
	chassis->tank(-60, -60);
	topStage.move(127);
	backStage.move(127);
	pickupStage.move(-127);
	pros::delay(250);
	intake->stop();
	pros::delay(250);
	hood.set_value(false);
	pros::delay(250);
	chassis->waitUntilDone();
	intake->scoreTop(127);
	pros::delay(1900);


	//ram w hood
	chassis->tank(60, 60);
	pros::delay(400);
	chassis->tank(0, 0);
	pros::delay(100);
	chassis->tank(-90, -90);
	hood.set_value(true);
	pros::delay(700);
	chassis->moveToPoint(30, 27.031, 700, {.forwards = false, .minSpeed = 110});


}

void elims7BallBlueRight() {
	//set position and descore
	chassis->setPose(0, 0, 0);
	intake->allianceColor = Intake::BLUE;
	intake->color_sorting_enabled = true;
	intake->anti_jam_enabled = true;
	hood.set_value(true);
	
	//intake 3 group
	chassis->moveToPoint(6.313, 22.992, 1500);
	intake->load(127);
	chassis->waitUntil(16);
	matchLoader.set_value(true);

	//go to match loader
	chassis->turnToPoint(32.594, 3.451, 750, {});
	chassis->moveToPoint(32.594, 3.451, 1250, {});
	chassis->turnToPoint(31.5, -12.215, 700);

	//match load
	chassis->moveToPoint(31.5, -12.215, 1000, {});
	chassis->waitUntilDone();
	chassis->tank(30, 30);
	pros::delay(925);
	chassis->cancelAllMotions();

	//score
	chassis->moveToPoint(32.5, 15.031, 1000, {.forwards = false, .minSpeed = 40, .earlyExitRange = 2});
	chassis->moveToPoint(32.5, 27.031, 1500, {.forwards = false, .maxSpeed = 60}, false);
	chassis->tank(-20, -20);
	topStage.move(127);
	backStage.move(127);
	pickupStage.move(-127);
	pros::delay(250);
	intake->stop();
	pros::delay(250);
	hood.set_value(false);
	chassis->waitUntilDone();
	intake->scoreTop(127);
	pros::delay(4000);

	//ram w hood
	chassis->tank(100, 100);
	pros::delay(200);
	chassis->tank(0, 0);
	hood.set_value(true);
	pros::delay(500);
	chassis->moveToPoint(32.5, 27.031, 700, {.forwards = false, .minSpeed = 110});
	chassis->tank(-60, -60);
}

void elims7BallBlueLeft() {

	//set position and descore
	chassis->setPose(0, 0, 0);
	intake->allianceColor = Intake::BLUE;
	intake->color_sorting_enabled = false;
	intake->anti_jam_enabled = true;
	hood.set_value(true);
	intake->color_stop_enabled = true;
	
	//intake 3 group
	chassis->moveToPoint(-8.25, 25, 1500);
	intake->load(127);
	chassis->waitUntil(13);
	matchLoader.set_value(true);

	//go to match loader
	chassis->turnToPoint(-34, 3.451, 750, {});
	chassis->moveToPoint(-34, 3.451, 1250, {});
	chassis->turnToPoint(-34, -16.215, 700);

	//match load
	chassis->moveToPoint(-34, -16.215, 1000, {.maxSpeed = 70});
	chassis->waitUntilDone();
	chassis->tank(7, 7);
	pros::delay(200);
	chassis->cancelAllMotions();


	//score in long goal
	chassis->moveToPoint(-34, 27.031, 1500, {.forwards = false, .maxSpeed = 70}, false);
	chassis->tank(-60, -60);
	topStage.move(127);
	backStage.move(127);
	pickupStage.move(-127);
	pros::delay(250);
	intake->stop();
	pros::delay(250);
	hood.set_value(false);
	pros::delay(250);
	chassis->waitUntilDone();
	intake->scoreTop(127);
	pros::delay(2050);


	//ram w hood
	chassis->tank(60, 60);
	pros::delay(400);
	chassis->tank(0, 0);
	pros::delay(100);
	chassis->tank(-90, -90);
	hood.set_value(true);
	pros::delay(700);
	chassis->moveToPoint(-34, 27.031, 700, {.forwards = false, .minSpeed = 110});
}

void halfAWPRedLeft() {
	//set pose
	chassis->setPose(0, 0, 0);

	//set up variables and pistons
	intake->allianceColor = Intake::RED;
	intake->color_sorting_enabled = false;
	intake->anti_jam_enabled = true;
	hood.set_value(false);
	intake->color_stop_enabled = true;

	//intake first 3 from pile near mid goal
	intake->load(127);
	chassis->moveToPoint(-7.889, 23.955, 700);
	chassis->waitUntil(14.75);
	matchLoader.set_value(true);

	//intake 2 under long goal
	chassis->waitUntilDone();
	pros::delay(200);

	chassis->moveToPose(-33.431, 42.689, -80, 1475, {});
	pros::delay(200);
	matchLoader.set_value(false);
	intake->loadBottom(127);
	chassis->waitUntilDone();
	pros::delay(400);

	//drive back and align to mid goal
	// chassis->turnToPoint(-12.85, 22.245, 550, {.forwards = false});
	chassis->moveToPoint(-12.85, 22.245, 1050, {.forwards = false, .maxSpeed = 80}, false);
	chassis->cancelAllMotions();
	chassis->turnToPoint(7.464, 40.421, 500, {.forwards = false});
	chassis->moveToPoint(7.464, 40.421,1200, {.forwards = false, .maxSpeed = 60}, false);
	chassis->setPose(7.464, 38.421, chassis->getPose().theta);

	// //score 4 in mid goal (optimal amount)
	intake->scoreTop(-127);
	pros::delay(100);
	intake->scoreNoBottom(127);
	pros::delay(1225);
	intake->stop();

	//drive to match loader
	intake->stop();
	chassis->moveToPoint(-30.682, 6.989, 900);
	chassis->turnToPoint(-30.694, -12.389, 450);
	

	//match load
	intake->load(127);
	matchLoader.set_value(true);
	chassis->moveToPoint(-30.994, -12.389, 900, {.maxSpeed = 80}, false);
	chassis->tank(7, 7);
	pros::delay(575);
	chassis->tank(0, 0);
	chassis->cancelAllMotions();

	//score in long goal
	chassis->moveToPoint(-31.057, 24.243, 950, {.forwards = false, .maxSpeed = 80}, false);
	intake->scoreTop(127);
	pros::delay(1450);
	chassis->setPose(-32.057, 24.243, chassis->getPose().theta);

	//turn and align wing
	matchLoader.set_value(false);
	chassis->moveToPoint(-43.946, 15.328, 900);
	chassis->turnToPoint(-43.946, 43.157, 500, {.forwards = false});

	//push wing
	chassis->moveToPoint(-43.946, 43.157, 1300, {.forwards = false, .maxSpeed = 80});

}


void halfAWPBlueLeft() {
	chassis->setPose(0, 0, -90);
	intake->allianceColor = Intake::BLUE;
	intake->color_sorting_enabled = false;
	intake->anti_jam_enabled = true;
	hood.set_value(true);
	intake->color_stop_enabled = false;

	//go to first match loader and score in goal
	chassis->moveToPoint(-32.733, 0, 1200);
	chassis->turnToPoint(-33.5, -19.116, 500);
	intake->load(127);
	matchLoader.set_value(true);
	chassis->moveToPoint(-33.5, -19.116, 1000, {.maxSpeed = 60});
	chassis->waitUntilDone();
	chassis->tank(20, 20);
	pros::delay(1500); 
	chassis->tank(0, 0);
	chassis->moveToPoint(-34, 15.893, 700, {.forwards = false, .minSpeed = 40, .earlyExitRange = 1});
	chassis->moveToPoint(-34, 27.893, 700, {.forwards = false, .maxSpeed = 60});
	intake->scoreTop(127);
	chassis->tank(-50, -50);
	pros::delay(100);
	intake->scoreTop(-127);
	pros::delay(100);
	intake->scoreTop(127);
	pros::delay(2000);
	hood.set_value(true);
	chassis->moveToPoint(-34, 8.031, 600, { .minSpeed = 110});
	chassis->tank(0, 0);
	pros::delay(500);
	chassis->moveToPoint(-34, 27.031, 800, {.forwards = false, .minSpeed = 110});
	pros::delay(200);

	//go to 3 pile in score in mid goal
	chassis->moveToPoint(-34, 6.49, 600, {.minSpeed = 100});
	chassis->turnToPoint(-2.09, 28.756, 600);
	matchLoader.set_value(false);
	intake->load(127);
	chassis->moveToPoint(-2.09, 28.756, 1300);
	chassis->waitUntil(18);
	matchLoader.set_value(true);

	//go score in mid goal
	chassis->turnToPoint(8, 42, 600, {.forwards = false, .direction = lemlib::AngularDirection::CW_CLOCKWISE});
	chassis->moveToPoint(8, 42, 800, {.forwards = false, .maxSpeed = 40, .minSpeed = 20, .earlyExitRange = 2});
	chassis->turnToHeading(-135, 300);
	matchLoader.set_value(false);
	chassis->waitUntilDone();
	// chassis->tank(-20, -20);
	intake->scoreMiddle(127);
	pros::delay(1500);

	//go intake other 3 pile and go to match loader
	// chassis->moveToPoint(-13, 41.559-28.438, 600);
	
	// chassis->turnToPoint(36.344, 18, 500);
	// chassis->moveToPoint(36.344, 18, 1200);
	// chassis->waitUntil(22); 
	// matchLoader.set_value(true);
	// chassis->turnToPoint(15.061, 40.739, 600);
	// matchLoader.set_value(false);
	// chassis->moveToPoint(22.061, 44.739-8.251, 200, {.minSpeed = 40, .earlyExitRange = 2});
	// chassis->moveToPoint(15.061, 44.739, 1100, {.maxSpeed = 70});
	// chassis->waitUntil(3);
	// intakeLift.set_value(true);
	// intake->scoreBottom(127);

	// //intake from match load and score
	// chassis->turnToPoint(54.637, -16.679, 600);
	// chassis->moveToPoint(54.637, -16.679, 1000);
	// chassis->waitUntilDone();
	// pros::delay(750);
	// chassis->moveToPoint(55.212, 2.893, 1500, {.forwards = false, .maxSpeed = 70}, false);
	// intake->scoreTop(127);
}

void soloAWPRedLeft() {
	chassis->setPose(-2, 0, -90);
	intake->allianceColor = Intake::RED;
	intake->color_sorting_enabled = false;
	intake->anti_jam_enabled = false;
	hood.set_value(true);
	intake->color_stop_enabled = true;

	//go to first match loader and match load
	chassis->moveToPoint(-33.5, 0, 1200, {.minSpeed = 30, .earlyExitRange = 1});
	chassis->turnToPoint(-34, -19.116, 500);
	intake->load(127);
	chassis->waitUntilDone();
	matchLoader.set_value(true);
	pros::delay(125);
	chassis->moveToPoint(-34, -19.116, 1000, {.maxSpeed = 65});
	chassis->waitUntilDone();
	// chassis->tank(20, 20);
	pros::delay(245); 
	// chassis->tank(0, 0);

	//score at long goal
	chassis->moveToPoint(-35, 15.893, 700, {.forwards = false, .minSpeed = 40, .earlyExitRange = 1});
	chassis->moveToPoint(-35, 27.893, 700, {.forwards = false, .maxSpeed = 60});
	pros::delay(200);
	chassis->tank(-50, -50);
	intake->scoreTop(127);
	pros::delay(1200);
	// pros::lcd::print(3, "X: %.2f, Y: %.2f", chassis->getPose().x, chassis->getPose().y);
	chassis->setPose(-32.53, 14.81, chassis->getPose().theta);
	intake->color_stop_enabled = false;
	

	//intake 3 pile near mid goal
	chassis->moveToPoint(-34, 6.49, 600, {.minSpeed = 100, .earlyExitRange = 1});
	chassis->turnToPoint(-1.59, 28.756, 600);
	hood.set_value(true);
	matchLoader.set_value(false);
	intake->load(127);
	chassis->moveToPoint(-1.59, 28.756, 1300);
	chassis->waitUntil(17);
	matchLoader.set_value(true);

	//score in mid goal
	chassis->turnToPoint(7.5, 41.5, 750, {.forwards = false, .direction = lemlib::AngularDirection::CW_CLOCKWISE, .maxSpeed = 95});
	chassis->moveToPoint(7.5, 41.5, 800, {.forwards = false, .maxSpeed = 40, .minSpeed = 20, .earlyExitRange = 2});
	chassis->turnToHeading(-135, 300);
	matchLoader.set_value(false);
	chassis->waitUntilDone();
	intake->scoreTop(-127);
	pros::delay(50);
	intake->scoreMiddle(127);
	pros::delay(800);

	//go intake other 3 pile 
	chassis->moveToPoint(13, 25.121, 600);
	intake->loadBottom(127);
	chassis->turnToPoint(40.344, 23.5, 500);
	chassis->moveToPoint(40.344, 23.5, 1200);
	chassis->waitUntil(25); 
	matchLoader.set_value(true);
	chassis->waitUntilDone();
	pros::delay(125);

	//score in bottom goal
	chassis->turnToPoint(14.561, 48.539, 600, {}, false);
	matchLoader.set_value(false);
	// chassis->moveToPoint(20.061, 49.739-8.251, 200, {.minSpeed = 40, .earlyExitRange = 2});
	chassis->moveToPoint(14.561, 48.539, 600, {.maxSpeed = 60, .minSpeed = 20, .earlyExitRange = 0.5});
	chassis->waitUntil(3);
	intakeLift.set_value(true);
	chassis->turnToHeading(-45, 300);
	pros::delay(100);
	intake->scoreBottom(127);
}

void soloAWPBlueLeft() {
	chassis->setPose(0, 0, -90);
	intake->allianceColor = Intake::BLUE;
	intake->color_sorting_enabled = false;
	intake->anti_jam_enabled = true;
	hood.set_value(true);
	intake->color_stop_enabled = false;

	//go to first match loader and match load
	chassis->moveToPoint(-32.733, 0, 1200, {.minSpeed = 30, .earlyExitRange = 1});
	chassis->turnToPoint(-33.5, -19.116, 500);
	intake->load(127);
	matchLoader.set_value(true);
	chassis->moveToPoint(-33.5, -19.116, 1000, {.maxSpeed = 70});
	chassis->waitUntilDone();
	// chassis->tank(20, 20);
	pros::delay(375); 
	// chassis->tank(0, 0);

	//score at long goal
	chassis->moveToPoint(-34, 15.893, 700, {.forwards = false, .minSpeed = 40, .earlyExitRange = 1});
	chassis->moveToPoint(-34, 27.893, 700, {.forwards = false, .maxSpeed = 60});
	pros::delay(200);
	intake->scoreTop(127);
	chassis->tank(-50, -50);
	pros::delay(100);
	intake->scoreTop(-127);
	pros::delay(100);
	intake->scoreTop(127);
	pros::delay(1650);
	// chassis->setPose(-34, 27.893, chassis->getPose().theta);
	hood.set_value(true);
	

	//intake 3 pile near mid goal
	chassis->moveToPoint(-34, 6.49, 600, {.minSpeed = 100, .earlyExitRange = 1});
	chassis->turnToPoint(-2.09, 28.756, 600);
	matchLoader.set_value(false);
	intake->load(127);
	chassis->moveToPoint(-2.09, 28.756, 1300);
	chassis->waitUntil(18);
	matchLoader.set_value(true);

	//score in mid goal
	chassis->turnToPoint(7.5, 43.5, 600, {.forwards = false, .direction = lemlib::AngularDirection::CW_CLOCKWISE});
	chassis->moveToPoint(7.5, 43.5, 800, {.forwards = false, .maxSpeed = 40, .minSpeed = 20, .earlyExitRange = 2});
	chassis->turnToHeading(-135, 300);
	matchLoader.set_value(false);
	chassis->waitUntilDone();
	intake->scoreMiddle(127);
	pros::delay(1250);

	//go intake other 3 pile 
	chassis->moveToPoint(13, 25.121, 600);
	intake->loadBottom(127);
	chassis->turnToPoint(40.344, 24.25, 500);
	chassis->moveToPoint(40.344, 24.25, 1200);
	chassis->waitUntil(21); 
	matchLoader.set_value(true);

	//score in bottom goal
	chassis->turnToPoint(15.061, 40.739, 600, {}, false);
	matchLoader.set_value(false);
	// chassis->moveToPoint(20.061, 49.739-8.251, 200, {.minSpeed = 40, .earlyExitRange = 2});
	chassis->moveToPoint(15.061, 49.739, 600, {.maxSpeed = 60});
	chassis->waitUntil(3);
	intakeLift.set_value(true);
	intake->scoreBottom(127);
}

void skills() {
	chassis->setPose(6.039, 11.502, 0); 
	intake->color_sorting_enabled = false;
	intake->anti_jam_enabled = true;
	hood.set_value(true);

	//grab one red block
	chassis->turnToHeading(65, 400);
	chassis->moveToPoint(17.5, 29.469, 1500, {.maxSpeed = 80});
	intake->load(127);

	//go to and match load #1
	chassis->turnToPoint(38.233-3.5, 13.327, 600);
	chassis->moveToPoint(38.233-3.5, 13.327, 1000);
	chassis->turnToPoint(38.233-3.5, -8, 700);
	matchLoader.set_value(true);
	intake->load(127);
	chassis->moveToPoint(38.233-3.5, -8, 1200, {.maxSpeed = 60});
	chassis->waitUntilDone();
	chassis->tank(7, 7);
	pros::delay(2100);

	//1st scoring
	chassis->moveToPoint(37.933-4.1, 35.945, 1500, {.forwards = false, .maxSpeed = 60}, false);
	chassis->tank(-20, -20);
	pros::delay(300);
	intake->scoreTop(127);
	pros::delay(200);
	intake->scoreTop(-127);
	pros::delay(100);
	intake->scoreTop(127);
	pros::delay(2500);
	hood.set_value(false);
	chassis->tank(0, 0);
	chassis->cancelAllMotions();

	//ram with hood
	chassis->tank(90, 90);
	pros::delay(200);
	chassis->tank(0, 0);
	pros::delay(500);
	chassis->tank(-50, -50);
	hood.set_value(true);
	pros::delay(1350);
	chassis->cancelAllMotions();
	// chassis->moveToPoint(38-4.1, 36, 1500, {.forwards = false, .maxSpeed = 50, .minSpeed = 50}, false);

	//go to and match load #2
	// pros::delay(500);
	chassis->setPose(33.5, 36, chassis->getPose().theta);
	pros::delay(100);
	chassis->moveToPoint(48, 18.94, 1000);
	matchLoader.set_value(false);
	chassis->turnToPoint(46, 98.217, 500, {.forwards = false});
	chassis->moveToPoint(46, 98.217, 2200, {.forwards = false});
	chassis->moveToPoint(31.85, 110.136, 2200, {.forwards = false, .maxSpeed = 90});
	chassis->turnToPoint(31.85, 132.115, 800);
	matchLoader.set_value(true);
	intake->load(127);
	chassis->moveToPoint(33, 132.115, 800, {.maxSpeed = 60}, false);
	chassis->tank(7, 7);
	pros::delay(2500);


	//2nd scoring (enable color stop to only score red blocks)
	intake->allianceColor = Intake::BLUE;
	

	chassis->moveToPoint(33, 84.543, 1500, {.forwards = false, .maxSpeed = 60}, false);
	intake->scoreTop(127);
	chassis->tank(-20, -20);
	pros::delay(750);
	intake->color_stop_enabled = true;
	pros::delay(750);
	chassis->cancelAllMotions();
	chassis->tank(0, 0);
	intake->stop();

	//disable color stop
	intake->color_stop_enabled = false;
	intake->scoreTop(-127);
	pros::delay(200);
	intake->stop();
	hood.set_value(true);
	matchLoader.set_value(false);


	//go to and match load #3
	chassis->setPose(0, 0, chassis->getPose().theta);
	pros::delay(200);
	chassis->moveToPoint(0, 12, 1000, {});
	chassis->turnToPoint(-94, 12, 700, {});
	chassis->moveToPoint(-94, 12, 2750, {.maxSpeed = 95});
	chassis->turnToPoint(-93, 41, 900, {});
	matchLoader.set_value(true);
	intake->load(127);
	chassis->moveToPoint(-93, 41, 1000, {.maxSpeed = 60}, false);
	chassis->tank(7, 7);
	pros::delay(2200);
	

	//3rd scoring
	chassis->moveToPoint(-94.5, -6, 1750, {.forwards = false, .maxSpeed = 60}, false);
	chassis->tank(-20, -20);
	intake->scoreTop(127);
	pros::delay(4100);
	chassis->cancelAllMotions();
	chassis->tank(0, 0);
	intake->stop();
	matchLoader.set_value(false);


	//go to and match load #4
	chassis->setPose(-94, 0, chassis->getPose().theta);
	chassis->moveToPoint(-94, 12, 400, {});
	chassis->moveToPoint(-106.5, 21, 900, {});
	matchLoader.set_value(false);
	hood.set_value(true);
	chassis->turnToPoint(-109.5, -63, 800, {.forwards = false});
	chassis->moveToPoint(-109.5, -63, 2000, {.forwards = false, .maxSpeed = 100});
	chassis->moveToPoint(-98, -73, 1750, {.forwards = false});
	chassis->turnToPoint(-98, -98, 1000, {});
	matchLoader.set_value(true);
	intake->load(127);
	chassis->moveToPoint(-97, -98, 1000, {.maxSpeed = 60}, false);
	chassis->tank(7, 7);
	pros::delay(2500);

	//scoring 4th (color stop enable to only score blue blocks)
	// intake->allianceColor = Intake::BLUE;
	// intake->color_stop_enabled = true;

	chassis->moveToPoint(-97, -56, 1750, {.forwards = false, .maxSpeed = 60}, false);
	chassis->tank(-20, -20);
	intake->scoreTop(127);
	pros::delay(2500);
	chassis->cancelAllMotions();
	chassis->tank(0, 0);
	intake->stop();
	matchLoader.set_value(false);

	//clear blocks from parking zone and park
	chassis->setPose(-94, -56, chassis->getPose().theta);
	chassis->moveToPoint(-94, -67, 700, {.minSpeed = 80, .earlyExitRange = 5});
	chassis->turnToPoint(-82, -87, 700, {.minSpeed = 60, .earlyExitRange = 5});
	chassis->moveToPoint(-82, -87, 1000, {.minSpeed = 60, .earlyExitRange = 5});
	chassis->turnToPoint(-64, -92.5, 700, {.minSpeed = 60, .earlyExitRange = 5});
	chassis->moveToPoint(-64, -92.5, 700, {}, false);
	hood.set_value(true);
	chassis->turnToHeading(90, 300, {}, false);
	chassis->tank(0, 0);
	odomLift.set_value(true);
	// matchLoader.set_value(true);
	pros::delay(500);
	chassis->tank(100, 100);
	chassis->cancelAllMotions();
	intake->load(127);
	pros::delay(625);
	chassis->tank(0, 0);
	matchLoader.set_value(false);
}