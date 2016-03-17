#ifndef CLTEST_SESSIONSTRPARSINGUTILS_H
#define CLTEST_SESSIONSTRPARSINGUTILS_H

#include "CommonStructures.h"

namespace SessionStrParsingUtils {
    SectorLimits getSectorLimits(const char* str, const char* end);
    Car getCar(const char* str, int index);

};

#endif //CLTEST_SESSIONSTRPARSINGUTILS_H
