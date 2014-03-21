// Summary: Declares vec2 and related math and graphics methods.
// Copyright: 2007  Philip Rideout.  All rights reserved.
// License: see source/bsd-license.txt

#pragma once

namespace Flatland
{
    const float pi = 3.1415926535897932384626433832795f;
    bool is_nan(float f);
    float round(float f);

    struct vec2;
    float dot(const vec2 &a, const vec2 &b);
    float cross(vec2 a, vec2 b);

    /// 2D vector
    struct vec2
    {
        vec2() {}
        vec2(float x, float y) : x(x), y(y) {}
        vec2(const float *f) : x(f[0]), y(f[1]) {}
        float length() const;
        vec2 rotate(float d) const;
        float squared_length() const { return dot(*this, *this); }
        void normalize() { *this /= length(); }
        vec2 rotate(vec2 v) const { return vec2(x * v.x + y * v.y, v.x * y - x * v.y); }
        vec2 hat() const { return *this / length(); }
        vec2 perp() const { return vec2(-y, x); }
        vec2 flip() const { return vec2(y, x); }
        void snap(float epsilon);
        vec2 operator-() const { return vec2(-x, -y); }
        vec2 &operator-=(vec2 v) { x -= v.x; y -= v.y; return *this; }
        vec2 &operator+=(vec2 v) { x += v.x; y += v.y; return *this; }
        vec2 &operator/=(float f) { x /= f; y /= f; return *this; }
        vec2 operator+(vec2 v) const { return vec2(x + v.x, y + v.y); }
        vec2 operator-(vec2 v) const { return vec2(x - v.x, y - v.y); }
        vec2 operator*(float f) const { return vec2(x * f, y * f); }
        vec2 operator/(float f) const { return vec2(x / f, y / f); }
        float x, y;
    };

    inline float dot(const vec2 &a, const vec2 &b) { return a.x * b.x + a.y * b.y; }
    inline float cross(vec2 a, vec2 b) { return a.x * b.y - a.y * b.x; }
}
