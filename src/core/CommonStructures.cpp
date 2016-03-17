#include "CommonStructures.h"

void InfoHub::clear() {
    weather = Weather();
    sectors.clear();
    classes.clear();
    drivers.clear();
    cars.fill(Car());
}