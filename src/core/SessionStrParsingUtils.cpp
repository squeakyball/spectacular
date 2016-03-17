#include "SessionStrParsingUtils.h"

namespace SessionStrParsingUtils {

SectorLimits getSectorLimits(const char* str, const char* end)
{
    const char* ptr = nullptr;
    int len = 0;
    //parseYaml(str, "SplitTimeInfo:Sectors:", &ptr, &len);

    SectorLimits limits;
    const char* val = nullptr;
    //parseYaml(ptr, "SectorStartPct:", &val, &len);
    while(val < end && val != nullptr)
    {
        double pct = strtod(val, nullptr);
        if(errno != ERANGE)
            limits.push_back(pct);

        ptr = val;
        //parseYaml(ptr, "SectorStartPct:", &val, &len);
    }

    return limits;
}

Car getCar(const char* str, int index)
{

    return Car();
}

}
