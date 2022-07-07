#include "Calculations.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string.h>
#include "VISA/Win64/Include/visa.h"
#include <math.h>
double findRMS(std::vector<double>& wave)
{
	double sum = 0;
	double rms;
	for (double i : wave)
		sum += pow(i, 2.0);
	rms = sqrt((sum / wave.size()));
	return rms;
}