#include "Initialize.h"
#include "VISA/Win64/Include/visa.h"
int init(ViStatus& status, ViSession& rmSession, ViSession& scopeSession, ViFindList& resourceList, ViUInt32& numResources,
	ViUInt32& ioBytes)
{
    ViChar buffer[1024];
    char usbResource[VI_FIND_BUFLEN];
    /*---------Open Default RM--------------*/
    status = viOpenDefaultRM(&rmSession);
    if (status == VI_SUCCESS) {
        std::cout << "VISA resource manager is successfully opened.\n";
    }
    else {
        std::cout << "Could not open VISA resource manager.\n";
        viStatusDesc(rmSession, status, buffer);
        std::cout << buffer << std::endl;
        return 0;
    }
    /*--------Find Resource----------------*/
    status = viFindRsrc(rmSession, (ViConstString)"USB?*INSTR", &resourceList, &numResources, usbResource);
    if (status == VI_SUCCESS) {
        std::cout << "Resource " << usbResource << " is found.\n";
    }
    else {
        viStatusDesc(rmSession, status, buffer);
        std::cout << buffer << std::endl;
        return 0;
    }
    /*--------Open a Session------------*/
    status = viOpen(rmSession, usbResource, VI_NULL, VI_NULL, &scopeSession);
    if (status == VI_SUCCESS) {
        std::cout << "Session opened successfully.\n";
    }
    else {
        std::cout << "Error connecting to the instrument." << std::endl;
        viStatusDesc(rmSession, status, buffer);
        std::cout << buffer << std::endl;
        return 0;
    }
    viSetAttribute(scopeSession, VI_ATTR_TMO_VALUE, 50000);
    return 1;
}