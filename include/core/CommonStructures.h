#ifndef CLTEST_COMMONSTRUCTURES_H
#define CLTEST_COMMONSTRUCTURES_H

#include <string>
#include <array>
#include <map>
#include <QtCore/QString>

static int INVALID_VALUE = -1;

struct Car {
    int ID = INVALID_VALUE;
    bool isSpectator = false;
    bool isSafetyCar = false;

    std::string number;
    std::string name;

    int classID = INVALID_VALUE;

    int userID = INVALID_VALUE;
    int teamID = INVALID_VALUE;
    std::string teamName;

    int lap = INVALID_VALUE;
    int lapsComplete = 0;
    double fastestTime = 0;
    int incidents = 0;
    int reasonOutID = 0;
};

struct Driver {
    std::string name;
    int iRating = INVALID_VALUE;
    int licenseLevel = INVALID_VALUE;
    int licenseSubLevel = INVALID_VALUE;
    std::string clubName;
};

struct Weather {
    QString type;
    double temperature = 0.0;
    double humidity = 0.75;
    double pressure = 101.0;
    double wind = 2.0;
    double windDirection = 0.0;
};

using SectorLimits = std::vector<double>;

struct InfoHub {
    std::array<Car, 64> cars;
    std::map<int, Driver> drivers;
    std::map<int, QString> classes;
    SectorLimits sectors;
    Weather weather;

    void clear();
};

#endif //CLTEST_COMMONSTRUCTURES_H
