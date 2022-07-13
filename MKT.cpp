#include <iostream>
#include <vector>
#include "VISA/Win64/Include/visa.h"
#include "Windows.h"
#include <string.h>
#include "Initialize.h"
#include "Commands.h"
#include "Calculations.h"
ViSession rmSession, scopeSession;
ViStatus status;
ViFindList resourceList;
ViUInt32 numResources;
ViUInt32 ioBytes;
ViChar buffer[1024];


std::vector<std::string> commands1 = {":WAV:SOUR CHAN1", ":WAV:MODE NORM", ":WAV:FORM ASC", ":WAV:DATA?"};
std::vector<std::string> commands2 = { ":AUT", ":STOP", ":WAV:SOUR CHAN1", ":WAV:MODE RAW", ":WAV:FORM BYTE", ":WAV:STAR1", ":WAV:STOP 120000", ":WAV:DATA?"};

int main()
{
    int a = init(status, rmSession, scopeSession, resourceList, numResources, ioBytes);
    if (a == 0)
        return 0;

    

    /*
    for (double i : wave)
        std::cout << i << " " << std::endl;  */
    while (true) {
        run_commands(commands1, status, scopeSession, rmSession);
        int readByte = readHeader(scopeSession, ioBytes, status);
        std::vector<double> wave = readWave(scopeSession, ioBytes, status, readByte);
        double rms = findRMS(wave);
        std::cout << "RMS: " << rms << std::endl;
        Sleep(500);
    }
    //Close the session to the resource
    viClose(scopeSession);
    viClose(rmSession);
    return 0;
}
