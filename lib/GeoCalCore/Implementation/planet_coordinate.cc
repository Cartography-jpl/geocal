#include "planet_coordinate.h"
using namespace GeoCal;

// Constants for Mars
template<> SpicePlanetConstant MarsConstant::h(MarsConstant::NAIF_CODE);
template<> const char* MarsConstant::name = "Mars";

// Constants for Europa
template<> SpicePlanetConstant EuropaConstant::h(EuropaConstant::NAIF_CODE);
template<> const char* EuropaConstant::name = "Europa";


