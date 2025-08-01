#include "autons.hpp"
#include "main.h"
#include "definitions.hpp"

float clamp(float output, float min, float max) {
	if (output < min) output = min;
	if (output > max) output = max;
	return output;
}



//Autons defined below
void pidTuning() {
    pros::lcd::print(3, "Yay the auto works! gfldkjsgfdjgkfljdsgklf;jdsglkf");
}

