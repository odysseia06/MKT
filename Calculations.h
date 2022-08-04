#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <string.h>
#include "VISA/Win64/Include/visa.h"
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <functional>
double findRMS(std::vector<double>& wave);
double simpsonIntegration(std::vector<double> voltageWave, std::vector<double> currentWave,
	std::vector<double> x_axis);