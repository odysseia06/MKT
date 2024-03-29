#pragma once
#include <iostream>
#include <vector>
#include <string.h>
#include "VISA/Win64/Include/visa.h"
#include "Windows.h"
#include <algorithm>
#include <functional>

void print_commands(std::vector<std::string>& commands);
void run_commands(std::vector<std::string>& commands, ViStatus& status, ViSession& scopeSession, ViSession& rmSession);
int readHeader(ViSession& scopeSession, ViUInt32& ioBytes, ViStatus& status);
std::vector<double> readWave(ViSession& scopeSession, ViUInt32& ioBytes, ViStatus& status, int readByte);
void replace(double& i);
double dutyCycle(ViSession& rmSession, ViSession& scopeSession, ViUInt32& ioBytes, ViStatus& status,
	std::vector<std::string> commands);
void readTest(ViSession& scopeSession, ViUInt32& ioBytes, ViStatus& status);
void showPower(ViSession& rmSession, ViSession& scopeSession, ViUInt32& ioBytes, ViStatus& status,
	std::vector<std::string> voltageCommands, std::vector<std::string> currentCommands);
void askPreamble(ViStatus& status, ViSession& scopeSession, ViSession& rmSession, ViUInt32& ioBytes);
int measurePoint(double& tscal);
struct waves { std::vector<double> voltage; std::vector<double> current; };
waves experimentMain1(ViStatus& status, ViSession& scopeSession, ViSession& rmSession, ViUInt32& ioBytes,
	std::vector<std::string> voltageCommands, std::vector<std::string> currentCommands);
std::vector<double> timeAxis();



struct Trigger {
	char mode[16];
	char coupling[16];
	char sweep[16];
	char edge_source[16];
	char edge_slope[16];
	char edge_level[16];
};

struct Channel {
	char chan[16];
	char bwLimit[16];
	char coupling[16];
	char display[16];
	char invert[16];
	char offset[16];
	char range[16];
	char scale[16];
	char probe[16];
};

struct Timebase {
	char scale[16];
	char offset[16];
};
