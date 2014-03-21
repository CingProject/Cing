// Summary: Defines vec2 methods.
// Copyright: 2007  Philip Rideout.  All rights reserved.
// License: see bsd-license.txt

#include <flatland/vector.hpp>
#include <math.h>

using namespace Flatland;

const float d2r = pi / 180;

vec2 vec2::rotate(float d) const { return rotate(vec2(cosf(d2r * d), sinf(d2r * d))); }
float vec2::length() const { return sqrtf(dot(*this, *this)); }

// returns true for IEEE floats that are infinity or a not-a-number
bool Flatland::is_nan(float f)
{
    unsigned long bits = *((unsigned long*) &f);

    // quiet -NaN
    if (bits >= 0xffc00001 && bits <= 0xffffffff) return true;
    
    // indeterminate
    if (bits == 0xffc00000) return true;

    // signaling -NaN
    if (bits >= 0xff800001 && bits <= 0xffbfffff) return true;

    // minus infinity
    if (bits == 0xff800000) return true;

    // positive infinity
    if (bits == 0x7f800000) return true;

    // signaling +NaN
    if (bits >= 0x7f800001 && bits <= 0x7fbfffff) return true;

    // signaling +NaN
    if (bits >= 0x7fc00000 && bits <= 0x7fffffff) return true;

    return false;
}

float Flatland::round(float f)
{
    return (float) ((int) (f + 0.5f));
}
