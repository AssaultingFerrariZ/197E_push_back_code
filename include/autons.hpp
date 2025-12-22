#pragma once
#include "main.h"


void pidTuning();
void soloAWP();
void elims7BallRedRight();
void elims7BallRedLeft();
void elims7BallBlueLeft();
void elims7BallBlueRight();
void halfAWPRedLeft();
void halfAWPBlueLeft();
void soloAWPRedLeft();
void soloAWPBlueLeft();
void skills();


float wrap360(float angle);
float clamp(float output, float min, float max);

pros::Task screenTask();

