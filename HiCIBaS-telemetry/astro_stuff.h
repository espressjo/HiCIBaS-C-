#ifndef ASTRO_STUFF_H
#define ASTRO_STUFF_H

#include <string>

using namespace std;

string from_hour_to_hms(float ra);
string from_hour_to_dms(float ra);
string from_degree_to_dms(float dec);

#endif

