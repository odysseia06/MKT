#pragma once
#include <iostream>
#include <vector>
#include <string.h>
#include "VISA/Win64/Include/visa.h"

void print_commands(std::vector<std::string>& commands);
void run_commands(std::vector<std::string>& commands, ViStatus& status, ViSession& scopeSession, ViSession& rmSession);
int readHeader(ViSession& scopeSession, ViUInt32& ioBytes, ViStatus& status);
std::vector<double> readWave(ViSession& scopeSession, ViUInt32& ioBytes, ViStatus& status, int readByte);