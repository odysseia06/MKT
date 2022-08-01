#include "Commands.h"
#include "Calculations.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string.h>
#include "VISA/Win64/Include/visa.h"
#include "Windows.h"
#include <algorithm>
#include <iomanip>
#include <numeric>
#define CMD_SIZE 64
#define FREQ 100000
#define TSCAL 0.000005
double tscal_inc[] = { 0.000005, 0.00001, 0.00002, 0.00005, 0.0001 };
void print_commands(std::vector<std::string>& commands)
{

    for (int i = 0; i < commands.size(); i++)
        std::cout << commands[i] << std::endl;
}

void run_commands(std::vector<std::string>& commands, ViStatus& status, ViSession& scopeSession, ViSession& rmSession)
{
    char cmd[CMD_SIZE];
    ViChar buffer[1024];
    for (std::string n : commands) {
        strcpy_s(cmd, n.c_str());
        status = viWrite(scopeSession, (ViBuf)cmd, (ViUInt32)strlen(cmd), VI_NULL);
        if (status != VI_SUCCESS) {
            viStatusDesc(rmSession, status, buffer);
            std::cout << buffer << std::endl;
            exit(-1);
        }
    }
}
int readHeader(ViSession& scopeSession, ViUInt32& ioBytes, ViStatus& status)
{   //Assumes TMC Header has exactly 11 characters.
    static char readFirst[2];
    
    status = viRead(scopeSession, (ViPBuf)readFirst, (ViUInt32)2, &ioBytes);
    //std::cout << ioBytes << " --Read First" << std::endl;
    static char readHead[9];
    status = viRead(scopeSession, (ViPBuf)readHead, (ViUInt32)9, &ioBytes);
    //std::cout << ioBytes << " --Read Header" << std::endl;
    auto readByte = std::strtol(readHead, nullptr, 10);
    //std::cout << readByte << " bytes to be read" << std::endl;
    return readByte;
    
}   

void readTest(ViSession& scopeSession, ViUInt32& ioBytes, ViStatus& status)
{
    static char readByte[700];
    status = viRead(scopeSession, (ViPBuf)readByte, (ViUInt32)120011, &ioBytes);
    std::stringstream ss;
    ss << std::hex;
    for (int i = 10; i < 700; i++) {
        //ss << std::setw(2) << std::setfill('0') << (int)readByte[i] << ",";
        std::cout << (int)readByte[i] << std::endl;
    }
    //std::cout << ss.str()  << std::endl;
    
}

std::vector<double> readWave(ViSession& scopeSession, ViUInt32& ioBytes, ViStatus& status, int readByte)
{
    char* readWaveAsc = new char[readByte];
    status = viRead(scopeSession, (ViPBuf)readWaveAsc, (ViUInt32)readByte, &ioBytes);
    std::vector<double> wave;
    std::stringstream ss(readWaveAsc);

    
    while (ss.good())
    {
        std::string substr;
        std::getline(ss, substr, ',');

        double subdb = std::stod(substr);
        wave.push_back(subdb);

    }
    delete[] readWaveAsc;
    /*
    for (double i : wave)
    {
        std::cout << i << std::endl;
    } */
    return wave;
    
}

void replace(double& i)
{
    if (i == 200.0)
        i = 0;
    if (i == -200.0)
        i = 0;
}

double dutyCycle(ViSession& rmSession, ViSession& scopeSession, ViUInt32& ioBytes, ViStatus& status,
    std::vector<std::string> commands)
{
    status = viWrite(scopeSession, (ViConstBuf)":TIM:MAIN:SCAL 0.05", 
        (ViUInt32)strlen(":TIM:MAIN:SCAL 0.05"), VI_NULL);
    std::vector<double> dutycycles;
    Sleep(1000);
    for (int i = 0; i < 5; i++)
    {
        run_commands(commands, status, scopeSession, rmSession);
        
        int readByte = readHeader(scopeSession, ioBytes, status);

        std::vector<double> wave = readWave(scopeSession, ioBytes, status, readByte);
        std::for_each(wave.begin(), wave.end(), replace);

        double target = 0;
        double count = std::count(wave.begin(), wave.end(), target);
        double dutycycle = (wave.size() - count) / (wave.size());
        dutycycles.push_back(dutycycle);
    }

    double dutycycle_avg = std::accumulate(dutycycles.begin(), dutycycles.end(), 0.0) / dutycycles.size();
    return dutycycle_avg;
}

void showPower(ViSession& rmSession, ViSession& scopeSession, ViUInt32& ioBytes, ViStatus& status,
    std::vector<std::string> voltageCommands, std::vector<std::string> currentCommands) {
    status = viWrite(scopeSession, (ViConstBuf)":TIM:MAIN:SCAL 0.000005",
        (ViUInt32)strlen(":TIM:MAIN:SCAL 0.0000005"), VI_NULL);
    Sleep(1000);
    for (int i = 0; i < 5; i++) {
        run_commands(voltageCommands, status, scopeSession, rmSession);

        int readByteV = readHeader(scopeSession, ioBytes, status);

        std::vector<double> voltageWave = readWave(scopeSession, ioBytes, status, readByteV);

        Sleep(100);

        run_commands(currentCommands, status, scopeSession, rmSession);

        int readByteC = readHeader(scopeSession, ioBytes, status);

        std::vector<double> currentWave = readWave(scopeSession, ioBytes, status, readByteC);

        Sleep(100);
        
        double vRms = findRMS(voltageWave);
        double iRms = findRMS(currentWave);
        std::cout << "Voltage RMS: " << vRms << std::endl;
        std::cout << "Current RMS" << iRms << std::endl;
  
    }
}

void askPreamble(ViStatus& status, ViSession& scopeSession, ViSession& rmSession, ViUInt32& ioBytes)
{
    status = viWrite(scopeSession, (ViConstBuf)":WAV:PRE?",
        (ViUInt32)strlen(":WAV:PRE?"), VI_NULL);
    static char readData[64];
    status = viRead(scopeSession, (ViPBuf)readData, (ViUInt32)512, &ioBytes);
    std::cout << readData << std::endl;
}

int measurePoint(double& tscal)
{
    double period = 1 / (double)FREQ;
    //std::cout << period << std::endl;
    double xinc = tscal / 50;
    //std::cout << xinc << std::endl;
    int points = (int) (period / xinc);
    //std::cout << points << std::endl;
    int a = 600 % points;
    int totalPoint = 600 - a;
    return totalPoint;
}

void experimentMain(ViStatus& status, ViSession& scopeSession, ViSession& rmSession, ViUInt32& ioBytes)
{

}