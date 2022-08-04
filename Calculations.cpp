#include "Calculations.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string.h>
#include "VISA/Win64/Include/visa.h"
#include <math.h>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <functional>
double findRMS(std::vector<double>& wave)
{
	double sum = 0;
	double rms;
	for (double i : wave)
		sum += pow(i, 2.0);
	rms = sqrt((sum / wave.size()));
	return rms;
}

double simpsonIntegration(std::vector<double> voltageWave, std::vector<double> currentWave,
	std::vector<double> x_axis)
{
	std::vector<double> powerWave;
	
	double result = 0;
	std::transform(voltageWave.begin(), voltageWave.end(), currentWave.begin(), std::back_inserter(powerWave),
		std::multiplies<double>());
	
	
	for (int i = 1; i < 599; i = i + 2)
	{
		result += powerWave[i - 1];
		result += powerWave[i] * (double)4;
		result += powerWave[i + 1];
	}
	
	result *= (x_axis[1] - x_axis[0]) / (double)3;
	
	result /= (x_axis.back() - x_axis.front());
	
	return result;

}