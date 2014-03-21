// Summary: Test for quad-circle intersection and generate contacts.
// Copyright: 2007  Philip Rideout.  All rights reserved.
// License: see bsd-license.txt

#include <flatland/shapes.hpp>
#include <flatland/intersection.hpp>

bool Flatland::Intersection::TestCircleQuad(const Geometry &c, const Geometry &q)
{
    return TestQuadCircle(q, c);
}

bool Flatland::Intersection::TestQuadCircle(const Geometry &g1, const Geometry &g2)
{
    assert(g1.GetShape() == Shape::Quad);
    assert(g2.GetShape() == Shape::Circle);
    const Quad &q = static_cast<const Quad&>(g1);
    const Circle &c = static_cast<const Circle&>(g2);

    bool onborder = false;
    vec2 l, t;

    vec2 p = c.Center() - q.Center();
    l.x = q.Extent(0);
    t.x = dot(p, q.Axis(0));
    if (t.x < -l.x) { t.x = -l.x; onborder = true; }
    if (t.x >  l.x) { t.x =  l.x; onborder = true; }

    l.y = q.Extent(1);
    t.y = dot(p, q.Axis(1));
    if (t.y < -l.y) { t.y = -l.y; onborder = true; }
    if (t.y >  l.y) { t.y =  l.y; onborder = true; }

    if (!onborder)
        return true;

    vec2 qq, r;
    vec2 a(q.Axis(0).x, q.Axis(1).x);
    vec2 b(q.Axis(0).y, q.Axis(1).y);
    qq.x = dot(t, a);
    qq.y = dot(t, b);
    r = p - qq;
    float depth = c.Radius() - r.length();
    return depth >= 0;
}

void Flatland::Intersection::FindCircleQuad(const Geometry &c, const Geometry &q, ContactList &contacts)
{
    contacts.ToggleNormalInversion();
    FindQuadCircle(q, c, contacts);
    contacts.ToggleNormalInversion();
}

void Flatland::Intersection::FindQuadCircle(const Geometry &g1, const Geometry &g2, ContactList &contacts)
{
    assert(g1.GetShape() == Shape::Quad);
    assert(g2.GetShape() == Shape::Circle);
    const Quad &q = static_cast<const Quad&>(g1);
    const Circle &c = static_cast<const Circle&>(g2);

    bool onborder = false;
    vec2 l, t;

    vec2 p = c.Center() - q.Center();
    l.x = q.Extent(0);
    t.x = dot(p, q.Axis(0));
    if (t.x < -l.x) { t.x = -l.x; onborder = true; }
    if (t.x >  l.x) { t.x =  l.x; onborder = true; }

    l.y = q.Extent(1);
    t.y = dot(p, q.Axis(1));
    if (t.y < -l.y) { t.y = -l.y; onborder = true; }
    if (t.y >  l.y) { t.y =  l.y; onborder = true; }

    if (!onborder)
    {
        // circle center is inside quad. find closest face to `t'
        float min_distance = l.x - fabsf(t.x);
        vec2 normal = t.x < 0 ? -q.Axis(0) : q.Axis(0);

        float face_distance = l.y - fabsf(t.y);
        if (face_distance < min_distance)
        {
            min_distance = face_distance;
            normal = t.y < 0 ? q.Axis(1) : -q.Axis(1);
        }

        float depth = min_distance + c.Radius();
        contacts.AddContact(c.Center(), normal, depth);
    }

    vec2 qq, r;
    vec2 a(q.Axis(0).x, q.Axis(1).x);
    vec2 b(q.Axis(0).y, q.Axis(1).y);
    qq.x = dot(t, a);
    qq.y = dot(t, b);
    r = p - qq;

    float distance = r.length();
    float depth = c.Radius() - distance;
    if (depth < 0)
        return;

    if (distance)
        r.normalize();
    else
        r = vec2(1, 0);

    contacts.AddContact(qq + q.Center(), -r, depth);
}
