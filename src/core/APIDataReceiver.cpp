#include <windows.h>
#include <mmsystem.h>
#include <time.h>
#include <c++/iostream>
#include <c++/chrono>
#include "APIDataReceiver.h"
#include "SessionStrParsingUtils.h"
#include "irsdk_defines.h"

#pragma warning(disable:4996) //_CRT_SECURE_NO_WARNINGS

#define TIMEOUT 16

APIDataReceiver::APIDataReceiver(InfoHub& infoHub)
:info(infoHub){
}

APIDataReceiver::~APIDataReceiver() {

}

void APIDataReceiver::run()
{
    timeBeginPeriod(1);
    int lastUpdate = -1;

    while(!stop)
    {
        // wait for new data and copy it into the data buffer, if data is not null
        if(waitForDataReady(TIMEOUT, data))
        {
            if(header)
            {

                // if header changes size, assume a new connection
                if(!data || dataSize != header->bufLen)
                {
                    reset();
                    dataSize = header->bufLen;
                    data = new char[dataSize];
                }
                else if(data)
                {
                    if(lastUpdate < header->sessionInfoUpdate)
                    {
                        updateStaticInfo();
                        //TODO: update semi-static data
                        lastUpdate = header->sessionInfoUpdate;
                    }

                    //TODO: update dynamic data
                }
            }
        }
        else if(!isConnected()) { // session ended
            reset();
        }
    }

    reset();
    shutdown();
    timeEndPeriod(1);
}

void APIDataReceiver::reset() {
    if(data) {
        info.clear();

        delete[] data;
        data = nullptr;
        header = nullptr;
    }
}

bool APIDataReceiver::waitForDataReady(int timeout, char *data) {
    if(initialized || startup())
    {
        // just to be sure, check before we sleep
        if(getNewData(data))
            return true;

        // sleep till signaled
        WaitForSingleObject(dataValidEventHandle, static_cast<DWORD>(timeout));

        // we woke up, so check for data
        return getNewData(data);
    }

    // sleep if error
    if(timeout > 0)
        Sleep(static_cast<DWORD>(timeout));

    return false;
}

bool APIDataReceiver::startup() {
    if(!memMapFileHandle)
    {
        memMapFileHandle = OpenFileMapping( FILE_MAP_READ, FALSE, IRSDK_MEMMAPFILENAME);
        lastTickCount = std::numeric_limits<int>::max();
    }

    if(memMapFileHandle)
    {
        if(!sharedMem)
        {
            sharedMem = static_cast<const char*>(MapViewOfFile(memMapFileHandle, FILE_MAP_READ, 0, 0, 0));
            header = reinterpret_cast<const irsdk_header*>(sharedMem);
            lastTickCount = std::numeric_limits<int>::max();
        }

        if(sharedMem)
        {
            if(!dataValidEventHandle)
            {
                dataValidEventHandle = OpenEvent(SYNCHRONIZE, false, IRSDK_DATAVALIDEVENTNAME);
                lastTickCount = std::numeric_limits<int>::max();
            }

            if(dataValidEventHandle)
            {
                initialized = true;
                return true;
            }
        }
    }

    initialized = false;
    return false;
}


void APIDataReceiver::shutdown() {
    if(dataValidEventHandle)
    {
        CloseHandle(dataValidEventHandle);
        dataValidEventHandle = nullptr;
    }

    if(sharedMem)
    {
        UnmapViewOfFile(sharedMem);
        sharedMem = nullptr;
        header = nullptr;
    }

    if(memMapFileHandle)
    {
        CloseHandle(memMapFileHandle);
        memMapFileHandle = nullptr;
    }

    initialized = false;
    lastTickCount = std::numeric_limits<int>::max();
}

bool APIDataReceiver::getNewData(char *data) {
    if(initialized || irsdk_startup())
    {
        // if sim is not active, then no new data
        if(!(header->status & irsdk_stConnected))
        {
            lastTickCount = std::numeric_limits<int>::max();
            return false;
        }

        int latest = 0;
        for(int i = 1; i < header->numBuf; i++)
            if(header->varBuf[latest].tickCount < header->varBuf[i].tickCount)
                latest = i;

        // if newer than last recieved, than report new data
        if(lastTickCount < header->varBuf[latest].tickCount)
        {
            // if asked to retrieve the data
            if(data)
            {
                // try twice to get the data out
                for(int count = 0; count < 2; count++)
                {
                    int curTickCount =  header->varBuf[latest].tickCount;
                    memcpy(data, sharedMem + header->varBuf[latest].bufOffset, static_cast<size_t >(header->bufLen));
                    if(curTickCount ==  header->varBuf[latest].tickCount)
                    {
                        lastTickCount = curTickCount;
                        lastValidTime = time(nullptr);
                        return true;
                    }
                }
                // if here, the data changed out from under us.
                return false;
            }
            else
            {
                lastTickCount =  header->varBuf[latest].tickCount;
                lastValidTime = time(nullptr);
                return true;
            }
        }
            // if older than last recieved, than reset, we probably disconnected
        else if(lastTickCount >  header->varBuf[latest].tickCount)
        {
            lastTickCount =  header->varBuf[latest].tickCount;
            return false;
        }
        // else the same, and nothing changed this tick
    }

    return false;
}

const char* APIDataReceiver::getSessionInfoString() {
    if(initialized)
    {
        return sharedMem + header->sessionInfoOffset;
    }
    else
        return nullptr;
}

bool APIDataReceiver::isConnected() {
    if(initialized)
    {
        int elapsed = static_cast<int>(difftime(time(nullptr), lastValidTime));
        return (header->status & irsdk_stConnected) > 0 && elapsed < timeout;
    }

    return false;
}

void APIDataReceiver::updateStaticInfo() {
    std::cout << "Session info update " << std::chrono::system_clock::now().time_since_epoch().count() << std::endl;
   /* const char* str = getSessionInfoString();

    if(info.sectors.empty())
    {
        info.sectors = SessionStrParsingUtils::getSectorLimits(str, str + header->sessionInfoLen);
    }*/
}
