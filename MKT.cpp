#include <iostream>
#include <vector>
#include "VISA/Win64/Include/visa.h"
#include "Windows.h"
#include <string.h>
#include "Initialize.h"
#include "Commands.h"
#include "Calculations.h"
#include "Configure.h"

ViSession rmSession, scopeSession;
ViStatus status;
ViFindList resourceList;
ViUInt32 numResources;
ViUInt32 ioBytes;
ViChar buffer[1024];
Trigger defaultTrig;
Channel defaultChan1;
Channel defaultChan2;
Timebase defaultTim;

std::vector<std::string> commands1 = {":WAV:SOUR CHAN1", ":WAV:MODE NORM", ":WAV:FORM ASC", ":WAV:DATA?"};
std::vector<std::string> commands2 = { ":WAV:SOUR CHAN2", ":WAV:MODE NORM", ":WAV:FORM ASC", ":WAV:DATA?" };
std::vector<std::string> commands3 = { ":STOP", ":WAV:SOUR CHAN1", ":WAV:FORM RAW", ":WAV:FORM BYTE",
":WAV:STAR 1", ":WAV:STOP 120000", ":WAV:DATA?"};
//commit deneme
int main()
{
    int a = init(status, rmSession, scopeSession, resourceList, numResources, ioBytes);
    if (a == 0)
        return 0;

    //askPreamble(status, scopeSession, rmSession, ioBytes);
    double tscal = 0.000005;
    int b = measurePoint(tscal);
    std::cout << b << std::endl;
    
    //defaultConfig(defaultTrig, defaultChan1, defaultChan2, defaultTim, scopeSession, ioBytes, status);




    
    //double dutyc = dutyCycle(rmSession, scopeSession, ioBytes, status, commands1);
    //std::cout << "Dutycycle is: " << (double)dutyc * 100 << "%" << std::endl;
    //Sleep(1000);
    //showPower(rmSession, scopeSession, ioBytes, status, commands1, commands2);
    //defaultConfig(defaultTrig, defaultChan1, defaultChan2, defaultTim, scopeSession, ioBytes, status);
    





    //Close the session to the resource
    viClose(scopeSession);
    viClose(rmSession);
    return 0;
}
