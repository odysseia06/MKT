#include <iostream>
#include <vector>
#include "VISA/Win64/Include/visa.h"
#include "Windows.h"
#include <string.h>
#include "Initialize.h"
#include "Commands.h"
#include "Calculations.h"
#include "Configure.h"
#include <numeric>

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

static volatile bool g_exit = false;

static BOOL WINAPI console_ctrl_handler(DWORD dwCtrlType)
{
    g_exit = true;
    return TRUE;
}

int main()
{
    SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
    int a = init(status, rmSession, scopeSession, resourceList, numResources, ioBytes);
    if (a == 0)
        return 0;

    double dutycycle = dutyCycle(rmSession, scopeSession, ioBytes, status, commands1);
    //std::cout << dutycycle << std::endl;

    std::vector<double> x_axis = timeAxis();
    std::vector<double> period_power;
    std::vector<double> voltageRMS;
    std::vector<double> currentRMS;
    

    while (!g_exit)
    {
        auto waves = experimentMain1(status, scopeSession, rmSession, ioBytes,
            commands1, commands2);
        double ppower = simpsonIntegration(waves.voltage, waves.current, x_axis);
        period_power.push_back(ppower);
        voltageRMS.push_back(findRMS(waves.voltage));
        currentRMS.push_back(findRMS(waves.current));
        double avg_ppower = std::accumulate(period_power.begin(), period_power.end(), 0.0) /
            period_power.size();
        double avg_power = avg_ppower * dutycycle;
        double vrms = std::accumulate(voltageRMS.begin(), voltageRMS.end(), 0.0) /
            voltageRMS.size();
        double irms = std::accumulate(currentRMS.begin(), currentRMS.end(), 0.0) /
            currentRMS.size();
        
        //std::cout << "voltage rms: " << findRMS(waves.voltage) << std::endl;
        //std::cout << "current rms: " << findRMS(waves.current) << std::endl;
        system("cls");
        
        std::cout << "period power: " << avg_ppower << std::endl;
        std::cout << "average power: " << avg_power << std::endl;
        std::cout << "voltage rms: " << vrms << std::endl;
        std::cout << "current rms: " << irms << std::endl;
        std::cout << "dutycycle: " << dutycycle << std::endl;

        
    }
    



    //Close the session to the resource
    viClose(scopeSession);
    viClose(rmSession);
    std::cout << "Program closed gracefully." << std::endl;
    return 0;
}
