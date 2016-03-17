/*
Copyright (c) 2013, iRacing.com Motorsport Simulations, LLC.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of iRacing.com Motorsport Simulations nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// Simple demo that reads iracing telemetry data and writes it to a file

//------

// Uncomment to log data to display
#define LOG_TO_DISPLAY

// Uncomment to log to ascii CSV format instead of binary IBT format
//#define LOG_TO_CSV

// Uncomment to log only when driver in car
//#define LOG_IN_CAR_ONLY

//------
#define MIN_WIN_VER 0x0501

#ifndef WINVER
#	define WINVER			MIN_WIN_VER
#endif

#ifndef _WIN32_WINNT
#	define _WIN32_WINNT		MIN_WIN_VER
#endif

#pragma warning(disable:4996) //_CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <signal.h>

#include "irsdk_defines.h"
#include "APIDataReceiver.h"
#include "SessionStrParsingUtils.h"

// for timeBeginPeriod
#pragma comment(lib, "Winmm")

// 16 ms timeout
#define TIMEOUT 16

// exited with ctrl-c
/*void ex_program(int sig)
{
    (void)sig;

    printf("recieved ctrl-c, exiting\n\n");

   // end_session(true);

    signal(SIGINT, SIG_DFL);
    exit(0);
}*/

int main()
{
    // trap ctrl-c
   // signal(SIGINT, ex_program);

    // bump priority up so we get time from the sim
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

   /* InfoHub info;
    APIDataReceiver receiver(info);
    receiver.run();*/

    FILE* file;
    file = fopen("sessionstr.txt", "r");
    fseek(file, 0, SEEK_END);
    int sz = ftell(file);
    rewind(file);
    char buffer[sz];
    memset(buffer, 0, sz);
    fread(buffer, 1, sz, file);

    auto res = SessionStrParsingUtils::getSectorLimits(buffer, buffer + sz);

    fclose(file);
    return 0;
}

