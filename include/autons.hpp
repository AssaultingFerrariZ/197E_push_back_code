#pragma once
#include "main.h"


void pidTuning();
void soloAWP();
void elims9Ball();
void skills();
void soloAWPRight();

float wrap360(float angle);
float clamp(float output, float min, float max);

pros::Task screenTask();

