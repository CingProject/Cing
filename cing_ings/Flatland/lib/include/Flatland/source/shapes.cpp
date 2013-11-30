// Summary: Defines methods for Quad, Wall, Block, Circle, Line, Terrain.
// Copyright: 2007  Philip Rideout.  All rights reserved.
// License: see bsd-license.txt

#include <ode/ode.h>
#include <flatland/shapes.hpp>
#include <flatland/intersection.hpp>
#include <cassert>
#include <algorithm>

using namespace Flatland;

Quad::Quad(float width, float height)
{
    extent[0] = width / 2;
	extent[1] = height / 2;
    UpdateBounds();
}

Block::Block(vec2 topleft, vec2 bottomright) : Quad(bottomright.x - topleft.x, bottomright.y - topleft.y)
{
    SetCenter(vec2(topleft.x + bottomright.x, topleft.y + bottomright.y) / 2);
    SetAxis(vec2(1, 0));
    UpdateBounds();
}

Block::Block(vec2 center, float width, float height) : Quad(width, height)
{
    SetCenter(center);
    SetAxis(vec2(1, 0));
    UpdateBounds();
}

Block::Block(float left, float top, float right, float bottom) : Quad(right - left, bottom - top)
{
    SetCenter(vec2(right + left, top + bottom) / 2);
    SetAxis(vec2(1, 0));
    UpdateBounds();
}

Line::Line(vec2 a, vec2 b) : Quad((b - a).length(), 0)
{
    SetCenter((a + b) / 2);
    SetAxis((b - a).hat());
    UpdateBounds();
}

Terrain::Terrain(const vec2 &start) : previous(start)
{
    vertices.push_back(start);
    spans[start.x] = 0;
}

Circle::Circle(const vec2 &center, float radius) : radius(radius)
{
    SetCenter(center);
    UpdateBounds();
}

void Quad::UpdateBounds()
{
    vec2 eaxis[2] = {EAxis(0), EAxis(1)};

    corners[0] = center - eaxis[0] - eaxis[1];
    corners[1] = center + eaxis[0] - eaxis[1];
    corners[2] = center + eaxis[0] + eaxis[1];
    corners[3] = center - eaxis[0] + eaxis[1];

    bounds.left = std::min(std::min(std::min(corners[0].x, corners[1].x), corners[2].x), corners[3].x);
    bounds.top = std::min(std::min(std::min(corners[0].y, corners[1].y), corners[2].y), corners[3].y);
    bounds.right = std::max(std::max(std::max(corners[0].x, corners[1].x), corners[2].x), corners[3].x);
    bounds.bottom = std::max(std::max(std::max(corners[0].y, corners[1].y), corners[2].y), corners[3].y);
}

bool Quad::Contains(const vec2 &v) const
{
    vec2 p = v - center;
    p = p.rotate(axis);
    return fabsf(p.x) < extent[0] && fabsf(p.y) < extent[1];
}

/// add a vertex to the terrain shape.
void Terrain::push_back(const vec2 &v)
{
    assert(v.x > previous.x && "terrains must be built in left-to-right order");
    lines.push_back(Line(previous, v));
    spans[v.x] = lines.size();
    previous = v;
    vertices.push_back(v);
}

bool Terrain::GetIndexRange(float left, float right, int &lower, int &upper) const
{
    SpanMap::const_iterator lowerBound = spans.lower_bound(left);
    if (lowerBound == spans.end())
        return false;

    lower = lowerBound->second - 1;
    if (lower < 0)
        lower = 0;
    assert(lower < (int) lines.size());

    SpanMap::const_iterator upperBound = spans.upper_bound(right);
    if (upperBound == spans.end())
        upper = lines.size() - 1;
    else
        upper = upperBound->second - 1;
    assert(upper >= 0 && upper < (int) lines.size());

    return true;
}

void Terrain::UpdateBounds()
{
    bounds.left = HUGE_VAL;
    bounds.top = HUGE_VAL;
    bounds.right = -HUGE_VAL;
    bounds.bottom = -HUGE_VAL;
    if (lines.empty())
        return;

    const Line &front = lines.front();
    bounds.left = std::min(bounds.left, front.Origin().x);

    for (LineStrip::const_iterator l = lines.begin(); l != lines.end(); ++l)
    {
        bounds.top = std::min(bounds.top, l->Origin().y);
        bounds.bottom = std::max(bounds.bottom, l->Origin().y);
    }

    const Line &back = lines.back();
    bounds.top = std::min(bounds.top, back.End().y);
    bounds.bottom = std::max(bounds.bottom, back.End().y);
    bounds.right = std::max(bounds.right, back.End().x);
}

void Circle::UpdateBounds()
{
    bounds.left = center.x - radius;
    bounds.top = center.y - radius;
    bounds.right = center.x + radius;
    bounds.bottom = center.y + radius;
}


Composite::Composite(vec2 centroid)
{
    SetCenter(centroid);
    UpdateBounds();
}

Composite::~Composite() // TODO make sure this gets called
{
    for (iterator g = geometries.begin(); g != geometries.end(); ++g)
        delete &(*g);
}

void Composite::UpdateBounds()
{
    bounds.left = HUGE_VAL;
    bounds.top = HUGE_VAL;
    bounds.right = -HUGE_VAL;
    bounds.bottom = -HUGE_VAL;
    if (geometries.empty())
        return;

    for (iterator g = geometries.begin(); g != geometries.end(); ++g)
    {
        (*g)->UpdateBounds();
        const aabb &b = (*g)->GetBounds();
        if (b.left < bounds.left)
            bounds.left = b.left;
        if (b.right > bounds.right)
            bounds.right = b.right;
        if (b.top < bounds.top)
            bounds.top = b.top;
        if (b.bottom > bounds.bottom)
            bounds.bottom = b.bottom;
    }
}

void Composite::push_back(Geometry *geometry)
{
    geometries.push_back(geometry);
}

void Composite::SetCenter(const vec2 &center)
{
    vec2 delta = center - this->center;
    Geometry::SetCenter(center);
    for (const_iterator g = begin(); g != end(); ++g)
        (*g)->SetCenter((*g)->Center() + delta);
}

void Composite::SetAxis(const vec2 &axis)
{
    vec2 rotation = this->axis.rotate(axis);

    for (const_iterator g = begin(); g != end(); ++g)
    {
        vec2 oldAxis = (*g)->Axis();
        vec2 oldCenter = (*g)->Center();

        vec2 newAxis = oldAxis.rotate(rotation);
        (*g)->SetAxis(newAxis);

        vec2 delta = oldCenter - center;
        if (!delta.x && !delta.y)
            continue;

        vec2 newCenter = center + delta.rotate(rotation);
        (*g)->SetCenter(newCenter);
    }
    Geometry::SetAxis(axis);
}

StaticComposite::StaticComposite(vec2 centroid)
{
	SetCenter(centroid);
	UpdateBounds();
}

StaticComposite::~StaticComposite() // TODO make sure this gets called
{
	//for (iterator g = geometries.begin(); g != geometries.end(); ++g)
	//	delete &(*g);
}

void StaticComposite::UpdateBounds()
{
	bounds.left = HUGE_VAL;
	bounds.top = HUGE_VAL;
	bounds.right = -HUGE_VAL;
	bounds.bottom = -HUGE_VAL;
	if (geometries.empty())
		return;

	for (iterator g = geometries.begin(); g != geometries.end(); ++g)
	{
		(*g)->UpdateBounds();
		const aabb &b = (*g)->GetBounds();
		if (b.left < bounds.left)
			bounds.left = b.left;
		if (b.right > bounds.right)
			bounds.right = b.right;
		if (b.top < bounds.top)
			bounds.top = b.top;
		if (b.bottom > bounds.bottom)
			bounds.bottom = b.bottom;
	}
}

void StaticComposite::push_back(Geometry *geometry)
{
	geometries.push_back(geometry);
}

void StaticComposite::SetCenter(const vec2 &center)
{
	vec2 delta = center - this->center;
	Geometry::SetCenter(center);
	for (const_iterator g = begin(); g != end(); ++g)
		(*g)->SetCenter((*g)->Center() + delta);
}

void StaticComposite::SetAxis(const vec2 &axis)
{
	vec2 rotation = this->axis.rotate(axis);

	for (const_iterator g = begin(); g != end(); ++g)
	{
		vec2 oldAxis = (*g)->Axis();
		vec2 oldCenter = (*g)->Center();

		vec2 newAxis = oldAxis.rotate(rotation);
		(*g)->SetAxis(newAxis);

		vec2 delta = oldCenter - center;
		if (!delta.x && !delta.y)
			continue;

		vec2 newCenter = center + delta.rotate(rotation);
		(*g)->SetCenter(newCenter);
	}
	Geometry::SetAxis(axis);
}

void Quad::SetMass(Body body, float density) const
{
    dMass m;
    if (extent[1])
        dMassSetBox(&m, density, 2 * extent[0], 2 * extent[1], 1);
    else
        dMassSetBox(&m, density, 2 * extent[0], 1, 1);
    dBodySetMass(body, &m);
}

void Circle::SetMass(Body body, float density) const
{
    dMass m;
    dMassSetBox(&m, density, radius, radius, 1); // TODO
    dBodySetMass(body, &m);
}

void Composite::SetMass(Body body, float density) const
{
    dMass m;
    dMassSetBox(&m, density, 100, 100, 1); // TODO
    dBodySetMass(body, &m);
}


void StaticComposite::SetMass(Body body, float density) const
{
	dMass m;
	dMassSetBox(&m, density, 100, 100, 1); // TODO
	dBodySetMass(body, &m);
}