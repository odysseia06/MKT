#pragma once
#include "VISA/Win64/Include/visa.h"
#include <iostream>

int init(ViStatus& status, ViSession& rmSession, ViSession& scopeSession, ViFindList& resourceList, ViUInt32& numResources,
	ViUInt32& ioBytes);