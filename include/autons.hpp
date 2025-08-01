#pragma once
#include "main.h"


void pidTuning();

float wrap360(float angle);
float clamp(float output, float min, float max);

pros::Task screenTask();

