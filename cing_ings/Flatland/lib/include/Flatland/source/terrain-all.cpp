// Summary: Test for terrain-all intersection and generate contacts.
// Copyright: 2007  Philip Rideout.  All rights reserved.
// License: see bsd-license.txt

#include <flatland/shapes.hpp>
#include <flatland/intersection.hpp>

// define an anonymous namespace as a place for storing off
// computed information shared by Test and Find
namespace
{
    int lower, upper;
}

using namespace Flatland;

bool TestTerrainGeometry(const Geometry &g1, const Geometry &g2)
{
    assert(g1.GetShape() == Shape::Terrain);
    const Terrain &t = static_cast<const Terrain&>(g1);
    const Geometry &g = g2;

    if (!t.GetIndexRange(g.GetBounds().left, g.GetBounds().right, lower, upper))
        return false;

    for (int index = lower; index <= upper; ++index)
    {
        assert(index >= 0 && index < (int) t.size());
        const Line &line = t[index];
        if (Intersection::Test(line, g))
            return true;
    }

    return false;
}

void FindTerrainGeometry(const Geometry &g1, const Geometry &g2, ContactList &contacts)
{
    assert(g1.GetShape() == Shape::Terrain);
    const Terrain &t = static_cast<const Terrain&>(g1);
    const Geometry &g = g2;

    for (int index = lower; index <= upper; ++index)
    {
        const Line &line = t[index];
        if (Intersection::Test(line, g)) // TODO inefficient because it's already been called
            Intersection::Find(line, g, contacts);
    }
}

void Intersection::FindTerrainQuad(const Geometry &g1, const Geometry &g2, ContactList &contacts)
{
    FindTerrainGeometry(g1, g2, contacts);
}

void Intersection::FindTerrainCircle(const Geometry &g1, const Geometry &g2, ContactList &contacts)
{
    FindTerrainGeometry(g1, g2, contacts);
}

void Intersection::FindTerrainTerrain(const Geometry &g1, const Geometry &g2, ContactList &contacts)
{
    FindTerrainGeometry(g1, g2, contacts);
}

void Intersection::FindQuadTerrain(const Geometry &g1, const Geometry &g2, ContactList &contacts)
{
    contacts.ToggleNormalInversion();
    FindTerrainGeometry(g2, g1, contacts);
    contacts.ToggleNormalInversion();
}

void Intersection::FindCircleTerrain(const Geometry &g1, const Geometry &g2, ContactList &contacts)
{
    contacts.ToggleNormalInversion();
    FindTerrainGeometry(g2, g1, contacts);
    contacts.ToggleNormalInversion();
}

bool Intersection::TestTerrainQuad(const Geometry &g1, const Geometry &g2)
{
    return TestTerrainGeometry(g1, g2);
}

bool Intersection::TestTerrainCircle(const Geometry &g1, const Geometry &g2)
{
    return TestTerrainGeometry(g1, g2);
}

bool Intersection::TestTerrainTerrain(const Geometry &g1, const Geometry &g2)
{
    return TestTerrainGeometry(g1, g2);
}

bool Intersection::TestQuadTerrain(const Geometry &g1, const Geometry &g2)
{
    return TestTerrainGeometry(g2, g1);
}

bool Intersection::TestCircleTerrain(const Geometry &g1, const Geometry &g2)
{
    return TestTerrainGeometry(g2, g1);
}
