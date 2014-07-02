// Summary: Test for circle-circle intersection and generate contacts.
// Copyright: 2007  Philip Rideout.  All rights reserved.
// License: see bsd-license.txt

#include <flatland/shapes.hpp>
#include <flatland/intersection.hpp>

bool Flatland::Intersection::TestCircleCircle(const Geometry &g1, const Geometry &g2)
{
    assert(g1.GetShape() == Shape::Circle);
    assert(g2.GetShape() == Shape::Circle);
    const Circle &c1 = static_cast<const Circle&>(g1);
    const Circle &c2 = static_cast<const Circle&>(g2);

    float distance = (c2.Center() - c1.Center()).length();
    return distance <= c1.Radius() + c2.Radius();
}

void Flatland::Intersection::FindCircleCircle(const Geometry &g1, const Geometry &g2, ContactList &contacts)
{
    assert(g1.GetShape() == Shape::Circle);
    assert(g2.GetShape() == Shape::Circle);
    const Circle &c1 = static_cast<const Circle&>(g1);
    const Circle &c2 = static_cast<const Circle&>(g2);

    vec2 delta = c2.Center() - c1.Center();
    float radiusSum = c1.Radius() + c2.Radius();
    float distance = delta.length();
    if (distance > radiusSum)
        return;

    vec2 pos;
    vec2 normal;
    float depth;

    if (distance <= 0)
    {
        pos = c1.Center();
        normal = vec2(1, 0);
        depth = radiusSum;
    }
    else
    {
        normal = delta / distance;
        float k = 0.5 * (c2.Radius() - c1.Radius() - distance);
        pos = c1.Center() + normal * k;
        depth = radiusSum - distance;
    }

    contacts.AddContact(pos, -normal, depth);
}
