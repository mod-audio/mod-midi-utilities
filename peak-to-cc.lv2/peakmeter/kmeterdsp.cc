// ------------------------------------------------------------------------
//
//  Copyright (C) 2008-2015 Fons Adriaensen <fons@linuxaudio.org>
//  Copyright (C) 2016 Filipe Coelho <falktx@falktx.com>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// ------------------------------------------------------------------------


#include <math.h>
#include "kmeterdsp.h"

Kmeterdsp::Kmeterdsp ()
{
    int fsamp = 48000;
    int fsize = 128;
    init (fsamp, fsize, 0.25f, 30.0f);
}

void Kmeterdsp::init (int fsamp, int fsize, float hold, float fall)
{
    _z0 = 0;
    _z1 = 0;
    _z2 = 0;
    _dpk = 0;
    _cnt = 0;

    // Called by initialisation code.
    //
    // fsamp = sample frequency
    // fsize = period size
    // hold  = peak hold time, seconds
    // fall  = peak fallback rate, dB/s

    float t;

    _wdcf = 5 * 6.28f / fsamp;                 // dc filter coefficient
    _wrms = 9.72f / fsamp;                     // ballistic filter coefficient
    t = (float) fsize / fsamp;                 // period time in seconds
    _hold = (int)(hold / t + 0.5f);            // number of periods to hold peak
    _fall = powf (10.0f, -0.05f * fall * t);   // per period fallback multiplier
}

float Kmeterdsp::process (const float *p, int n)
{
    // Called by JACK's process callback.
    //
    // p : pointer to sample buffer
    // n : number of samples to process

    float  s, t, z0, z1, z2;

    // Get filter state.
    z0 = _z0;
    z1 = _z1;
    z2 = _z2;

    // Process n samples. Find digital peak value for this
    // period and perform filtering on squared signal.
    t = 0;
    while (n--)
    {
        s = *p++;

        if (s < -1.0f)
            s = -1.0f;
        else if (s > 1.0f)
            s = 1.0f;

        z0 += _wdcf * (s - z0);      // DC filter
        s -= z0;
        s *= s;
        if (t < s) t = s;            // Update digital peak.
        z1 += _wrms * (s - z1);      // Update first filter.
        z2 += _wrms * (z1 - z2);     // Update second filter.
    }
    t = sqrtf (t);

    // Save filter state.
    _z0 = z0;
    _z1 = z1;
    _z2 = z2;

    // Digital peak hold and fallback.
    if (t > _dpk)
    {
        // If higher than current value, update and set hold counter.
        _dpk = t;
        _cnt = _hold;
    }
    else if (_cnt) _cnt--; // else decrement counter if not zero,
    else
    {
        _dpk *= _fall;     // else let the peak value fall back,
    }

    return _dpk;
}
