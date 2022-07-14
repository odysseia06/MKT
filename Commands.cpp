#include "Commands.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string.h>
#include "VISA/Win64/Include/visa.h"
#define CMD_SIZE 64

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
    return wave;
}

