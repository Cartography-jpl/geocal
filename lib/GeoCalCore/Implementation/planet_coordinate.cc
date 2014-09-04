#include "planet_coordinate.h"
using namespace GeoCal;

// Constants for mars
template<> SpicePlanetConstant MarsConstant::h(MarsConstant::NAIF_CODE);
template<> const char* MarsConstant::name = "Mars";


