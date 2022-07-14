#pragma once
#include <iostream>
#include <vector>
#include <string.h>
#include "VISA/Win64/Include/visa.h"
#include "Commands.h"
#include "Windows.h"
static char trig_mode[32] = ":TRIG:MODE ";
static char trig_edge_sour[32] = ":TRIG:EDGE:SOUR ";
static char trig_edge_lev[32] = ":TRIG:EDGE:LEV ";
static char trig_edge_slope[32] = ":TRIG:EDGE:SLOP ";

static char chan1_scal[32] = ":CHAN1:SCAL ";
static char chan1_offs[32] = ":CHAN1:OFFS ";
static char chan1_prob[32] = ":CHAN1:PROB ";
static char chan1_coup[32] = "CHAN1:COUP ";
static char chan1_disp[32] = "CHAN1:DISP ";
static char chan2_scal[32] = ":CHAN2:SCAL ";
static char chan2_offs[32] = ":CHAN2:OFFS ";
static char chan2_prob[32] = ":CHAN2:PROB ";
static char chan2_coup[32] = "CHAN2:COUP ";
static char chan2_disp[32] = "CHAN2:DISP ";

static char tim_scal[32] = ":TIM:MAIN:SCAL ";
static char tim_offs[32] = ":TIM:MAIN:OFFS ";

void config(struct Trigger trig, struct Channel chan1, struct Channel chan2, struct Timebase tim,
	ViSession& scopeSession, ViUInt32& ioBytes, ViStatus& status);

void defaultConfig(struct Trigger trig, struct Channel chan1, struct Channel chan2, struct Timebase tim,
	ViSession& scopeSession, ViUInt32& ioBytes, ViStatus& status);