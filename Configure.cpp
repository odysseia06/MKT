#include <iostream>
#include <vector>
#include <string.h>
#include "VISA/Win64/Include/visa.h"
#include "Commands.h"
#include "Windows.h"
#include "Configure.h"

void config(struct Trigger trig, struct Channel chan1, struct Channel chan2, struct Timebase tim,
	ViSession& scopeSession, ViUInt32& ioBytes, ViStatus& status)
{
	std::cout << trig.coupling << std::endl;

}

void defaultConfig(struct Trigger trig, struct Channel chan1, struct Channel chan2, struct Timebase tim,
	ViSession& scopeSession, ViUInt32& ioBytes, ViStatus& status)
{
	strcpy_s(trig.mode, "EDGE");
	strcpy_s(trig.edge_source, "CHAN1");
	strcpy_s(trig.edge_level, "3000");
	strcpy_s(trig.edge_slope, "POS");

	strcpy_s(chan1.scale, "5000");
	strcpy_s(chan1.offset, "0");
	strcpy_s(chan1.coupling, "1");
	strcpy_s(chan1.probe, "1000");
	strcpy_s(chan1.display, "ON");

	strcpy_s(chan2.scale, "5");
	strcpy_s(chan2.offset, "0");
	strcpy_s(chan2.coupling, "DC");
	strcpy_s(chan2.probe, "1");
	strcpy_s(chan2.display, "ON");

	strcpy_s(tim.scale, "0.000005");
	strcpy_s(tim.offset, "0");

	strcat_s(trig_mode, trig.mode);
	strcat_s(trig_edge_sour, trig.edge_source);
	strcat_s(trig_edge_lev, trig.edge_level);
	strcat_s(trig_edge_slope, trig.edge_slope);

	strcat_s(chan1_scal, chan1.scale);
	strcat_s(chan1_offs, chan1.offset);
	strcat_s(chan1_coup, chan1.coupling);
	strcat_s(chan1_prob, chan1.probe);
	strcat_s(chan1_disp, chan1.display);

	strcat_s(chan2_scal, chan2.scale);
	strcat_s(chan2_offs, chan2.offset);
	strcat_s(chan2_coup, chan2.coupling);
	strcat_s(chan2_prob, chan2.probe);
	strcat_s(chan2_disp, chan2.display);

	strcat_s(tim_scal, tim.scale);
	strcat_s(tim_offs, tim.offset);

	char *commands[16] = {trig_mode, trig_edge_sour, trig_edge_slope, trig_edge_lev,
	chan1_scal, chan1_offs, chan1_coup, chan1_prob, chan1_disp,
	chan2_scal, chan2_offs, chan2_coup, chan2_prob, chan2_disp,
	tim_scal, tim_offs };
	
	for (int i = 0; i < 16; i++) {
		status = viWrite(scopeSession, (ViConstBuf)commands[i], (ViUInt32)strlen(commands[i]), VI_NULL);
		if (status != VI_SUCCESS) {
			std::cout << "Error writing commands.";
			break;
		}
		Sleep(200);
	}
	Sleep(1000);
}