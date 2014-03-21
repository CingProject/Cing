// Summary: Test for quad-quad intersection and generate contacts.
// Copyright: 2007  Philip Rideout.  All rights reserved.
// License: see bsd-license.txt

#include <flatland/shapes.hpp>
#include <flatland/intersection.hpp>

// define an anonymous namespace as a place for storing off
// computed information shared by Test and Find
namespace
{
    Flatland::vec2 delta;
    float dotA0B0, dotA0B1, dotA1B0, dotA1B1;
    float absDotA0B0, absDotA0B1, absDotA1B0, absDotA1B1;
    float extAA0B0, extAA0B1, extAA1B0, extAA1B1, extBA0B0, extBA0B1, extBA1B0, extBA1B1;
    float depth;
    enum ECode {NA0, NA1, NB0, NB1, PA0, PA1, PB0, PB1} code;
}

bool Flatland::Intersection::TestQuadQuad(const Geometry &g1, const Geometry &g2)
{
    assert(g1.GetShape() == Shape::Quad);
    assert(g2.GetShape() == Shape::Quad);
    const Quad &a = static_cast<const Quad&>(g1);
    const Quad &b = static_cast<const Quad&>(g2);

    delta = a.Center() - b.Center();
    float minDepth = HUGE_VAL;

    // test the seperating axis of a.Axis(0)
    dotA0B0 = dot(a.Axis(0), b.Axis(0));
    absDotA0B0 = fabsf(dotA0B0);
    dotA0B1 = dot(a.Axis(0), b.Axis(1));
    absDotA0B1 = fabsf(dotA0B1);
    extBA0B0 = b.Extent(0)*absDotA0B0;
    extBA0B1 = b.Extent(1)*absDotA0B1;
    float dist1 = dot(a.Axis(0), delta);
    float dist2 = a.Extent(0) + extBA0B0 + extBA0B1;
    depth = dist2 - fabsf(dist1);
    if (depth < 0)
        return false;
    if (depth < minDepth)
    {
        minDepth = depth;
        code = dist1 < 0 ? NA0 : PA0;
    }

    // test the seperating axis of a.Axis(1)
    dotA1B0 = dot(a.Axis(1), b.Axis(0));
    absDotA1B0 = fabsf(dotA1B0);
    dotA1B1 = dot(a.Axis(1), b.Axis(1));
    absDotA1B1 = fabsf(dotA1B1);
    extBA1B0 = b.Extent(0)*absDotA1B0;
    extBA1B1 = b.Extent(1)*absDotA1B1;
    dist1 = dot(a.Axis(1), delta);
    dist2 = a.Extent(1) + extBA1B0 + extBA1B1;
    depth = dist2 - fabsf(dist1);
    if (depth < 0)
        return false;
    if (depth < minDepth)
    {
        minDepth = depth;
        code = dist1 < 0 ? NA1 : PA1;
    }

    // test the seperating axis of b.Axis(0)
    extAA0B0 = a.Extent(0)*absDotA0B0;
    extAA1B0 = a.Extent(1)*absDotA1B0;
    dist1 = dot(b.Axis(0), delta);
    dist2 = b.Extent(0) + extAA0B0 + extAA1B0;
    depth = dist2 - fabsf(dist1);
    if (depth < 0)
        return false;
    if (depth < minDepth)
    {
        minDepth = depth;
        code = dist1 < 0 ? NB0 : PB0;
    }

    // test the seperating axis of b.Axis(1)
    extAA0B1 = a.Extent(0)*absDotA0B1;
    extAA1B1 = a.Extent(1)*absDotA1B1;
    dist1 = dot(b.Axis(1), delta);
    dist2 = b.Extent(1) + extAA0B1 + extAA1B1;
    depth = dist2 - fabsf(dist1);
    if (depth < 0)
        return false;
    if (depth < minDepth)
    {
        minDepth = depth;
        code = dist1 < 0 ? NB1 : PB1;
    }

    return true;
}

void Flatland::Intersection::FindQuadQuad(const Geometry &g1, const Geometry &g2, ContactList &contacts)
{
    assert(g1.GetShape() == Shape::Quad);
    assert(g2.GetShape() == Shape::Quad);
    const Quad &a = static_cast<const Quad&>(g1);
    const Quad &b = static_cast<const Quad&>(g2);

    vec2 normal;
    vec2 normal2;
    vec2 center;
    int axis0, axis1;
    int a0, a1;
    const Quad *reference;
    const Quad *incident;
    float m11;
    float k1;
    bool negate;

    switch (code)
    {
        case PA0:
            normal = a.Axis(0);
            axis0 = 0; axis1 = 1;
            normal2 = -normal;
            reference = &a; incident = &b;
            a0 = absDotA0B1 > absDotA0B0 ? 1 : 0; a1 = 1 - a0;
            if (a1)
            {
                m11 = dotA1B1;
                k1 = extBA1B1;
                negate = -dotA0B0 < 0;
            }
            else
            {
                m11 = dotA1B0;
                k1 = extBA1B0;
                negate = -dotA0B1 < 0;
            }
            center = -delta;
            break;

        case PA1:
            normal = a.Axis(1);
            axis0 = 1; axis1 = 0;
            normal2 = -normal;
            reference = &a; incident = &b;
            a0 = absDotA1B1 > absDotA1B0 ? 1 : 0; a1 = 1 - a0;
            if (a1)
            {
                m11 = dotA0B1;
                k1 = extBA0B1;
                negate = -dotA1B0 < 0;
            }
            else
            {
                m11 = dotA0B0;
                k1 = extBA0B0;
                negate = -dotA1B1 < 0;
            }
            center = -delta;
            break;

        case PB0:
            normal = b.Axis(0);
            axis0 = 0; axis1 = 1;
            normal2 = normal;
            reference = &b; incident = &a;
            a0 = absDotA1B0 > absDotA0B0 ? 1 : 0; a1 = 1 - a0;
            if (a1)
            {
                m11 = dotA1B1;
                k1 = extAA1B1;
                negate = dotA0B0 < 0;
            }
            else
            {
                m11 = dotA0B1;
                k1 = extAA0B1;
                negate = dotA1B0 < 0;
            }
            center = delta;
            break;

        case PB1:
            normal = b.Axis(1);
            axis0 = 1; axis1 = 0;
            normal2 = normal;
            reference = &b; incident = &a;
            a0 = absDotA1B1 > absDotA0B1 ? 1 : 0; a1 = 1 - a0;
            if (a1)
            {
                m11 = dotA1B0;
                k1 = extAA1B0;
                negate = dotA0B1 < 0;
            }
            else
            {
                m11 = dotA0B0;
                k1 = extAA0B0;
                negate = dotA1B1 < 0;
            }
            center = delta;
            break;

        case NA0:
            normal = -a.Axis(0);
            axis0 = 0; axis1 = 1;
            normal2 = -normal;
            reference = &a; incident = &b;
            a0 = absDotA0B1 > absDotA0B0 ? 1 : 0; a1 = 1 - a0;
            if (a1)
            {
                m11 = dotA1B1;
                k1 = extBA1B1;
                negate = dotA0B0 < 0;
            }
            else
            {
                m11 = dotA1B0;
                k1 = extBA1B0;
                negate = dotA0B1 < 0;
            }
            center = -delta;
            break;

        case NA1:
            normal = -a.Axis(1);
            axis0 = 1; axis1 = 0;
            normal2 = -normal;
            reference = &a; incident = &b;
            a0 = absDotA1B1 > absDotA1B0 ? 1 : 0; a1 = 1 - a0;
            if (a1)
            {
                m11 = dotA0B1;
                k1 = extBA0B1;
                negate = dotA1B0 < 0;
            }
            else
            {
                m11 = dotA0B0;
                k1 = extBA0B0;
                negate = dotA1B1 < 0;
            }
            center = -delta;
            break;

        case NB0:
            normal = -b.Axis(0);
            axis0 = 0; axis1 = 1;
            normal2 = normal;
            reference = &b; incident = &a;
            a0 = absDotA1B0 > absDotA0B0 ? 1 : 0; a1 = 1 - a0;
            if (a1)
            {
                m11 = dotA1B1;
                k1 = extAA1B1;
                negate = -dotA0B0 < 0;
            }
            else
            {
                m11 = dotA0B1;
                k1 = extAA0B1;
                negate = -dotA1B0 < 0;
            }
            center = delta;
            break;

        case NB1:
            normal = -b.Axis(1);
            axis0 = 1; axis1 = 0;
            normal2 = normal;
            reference = &b; incident = &a;
            a0 = absDotA1B1 > absDotA0B1 ? 1 : 0; a1 = 1 - a0;
            if (a1)
            {
                m11 = dotA1B0;
                k1 = extAA1B0;
                negate = -dotA0B1 < 0;
            }
            else
            {
                m11 = dotA0B0;
                k1 = extAA0B0;
                negate = -dotA1B1 < 0;
            }
            center = delta;
            break;
    }

    // compute center point of incident face, in reference-face coordinates
    if (negate)
        center += incident->EAxis(a0);
    else
        center -= incident->EAxis(a0);

    // find the intersection points
    float collisions[2];
    int count = 0;
    {
        float c1 = dot(center, reference->Axis(axis1));
        float a = c1 - k1;
        float b = c1 + k1;
        float w = reference->Extent(axis1);

        if (w > a && w < b)
            collisions[count++] = w - c1;
        if (a < w && a > -w)
            collisions[count++] = -k1;
        if (-w > a && -w < b)
            collisions[count++] = -w - c1;
        if (b < w && b > -w)
            collisions[count++] = k1;
    }

    // Count should be 2 or less.  It is almost always equal to 2.
    assert(count < 3);

    for (int j = 0; j < count; ++j)
    {
        float k1 = collisions[j] / m11;
        vec2 point = center + incident->Axis(a1) * k1;
        float depth = reference->Extent(axis0) - dot(normal2, point);
        if (depth >= 0)
            contacts.AddContact(point + reference->Center(), normal, depth);
    }
}
