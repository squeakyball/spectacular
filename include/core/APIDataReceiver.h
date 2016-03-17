#ifndef CLTEST_APIDATARECEIVER_H
#define CLTEST_APIDATARECEIVER_H

#include "CommonStructures.h"
#include "irsdk_defines.h"

class APIDataReceiver {
public:
    APIDataReceiver(InfoHub& infoHub);
    ~APIDataReceiver();

    void run();

private:
    static constexpr double timeout = 30.0; // timeout after 30 seconds with no communication
    time_t lastValidTime = 0;
    HANDLE memMapFileHandle = nullptr;
    HANDLE dataValidEventHandle = nullptr;
    const char* sharedMem = nullptr;
    int lastTickCount = std::numeric_limits<int>::max();
    const irsdk_header* header = nullptr;
    char* data = nullptr;
    bool initialized = false;
    InfoHub& info;
    int dataSize = 0;
    bool stop = false;

    void reset();
    void updateStaticInfo();

    bool startup();
    void shutdown();
    bool getNewData(char *data);
    bool waitForDataReady(int timeout, char* data);
    const char* getSessionInfoString();
    bool isConnected();
};


#endif //CLTEST_APIDATARECEIVER_H
