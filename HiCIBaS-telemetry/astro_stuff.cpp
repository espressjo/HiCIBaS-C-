#include "astro_stuff.h"

string from_hour_to_hms(float ra)
{
    
int h,m;
float s;
float buff;
    h = static_cast<int>(ra);
    buff = ra-h;
    buff = buff*60.0;
    m = static_cast<int>(buff);
    s = 60*(buff-m);
    return to_string(h)+":"+to_string(m)+":"+to_string(s);
}
string from_hour_to_dms(float ra)
{
    
int h,m;
float s;
float buff;
ra = ra/24.0 *360.0;
    h = static_cast<int>(ra);
    buff = ra-h;
    buff = buff*60.0;
    m = static_cast<int>(buff);
    s = 60*(buff-m);
    return to_string(h)+":"+to_string(m)+":"+to_string(s);
}

string from_degree_to_dms(float dec)
{
    
int h,m;
float s;
float buff;
    h = static_cast<int>(dec);
    buff = dec-h;
    buff = buff*60.0;
    m = static_cast<int>(buff);
    s = 60*(buff-m);
    return to_string(h)+":"+to_string(m)+":"+to_string(s);
}