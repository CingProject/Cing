// Summary: Test for composite-all intersection and generate contacts.
// Copyright: 2007  Philip Rideout.  All rights reserved.
// License: see bsd-license.txt

#include <flatland/shapes.hpp>
#include <flatland/intersection.hpp>

using namespace Flatland;

bool TestCompositeGeometry(const Geometry &g1, const Geometry &g2)
{
    assert(g1.GetShape() == Shape::Composite);
    const Composite &c = static_cast<const Composite&>(g1);

    for (Composite::const_iterator g = c.begin(); g != c.end(); ++g)
        if (Intersection::Test(**g, g2))
            return true;

    return false;
}

void FindCompositeGeometry(const Geometry &g1, const Geometry &g2, ContactList &contacts)
{
    assert(g1.GetShape() == Shape::Composite);
    const Composite &c = static_cast<const Composite&>(g1);

    for (Composite::const_iterator g = c.begin(); g != c.end(); ++g)
    {
        if (Intersection::Test(**g, g2))
            Intersection::Find(**g, g2, contacts);
    }
}

void Intersection::FindCompositeQuad(const Geometry &g1, const Geometry &g2, ContactList &contacts)
{
    FindCompositeGeometry(g1, g2, contacts);
}

void Intersection::FindCompositeCircle(const Geometry &g1, const Geometry &g2, ContactList &contacts)
{
    FindCompositeGeometry(g1, g2, contacts);
}

void Intersection::FindCompositeTerrain(const Geometry &g1, const Geometry &g2, ContactList &contacts)
{
    FindCompositeGeometry(g1, g2, contacts);
}

void Intersection::FindCompositeComposite(const Geometry &g1, const Geometry &g2, ContactList &contacts)
{
    FindCompositeGeometry(g1, g2, contacts);
}

void Intersection::FindQuadComposite(const Geometry &g1, const Geometry &g2, ContactList &contacts)
{
    contacts.ToggleNormalInversion();
    FindCompositeGeometry(g2, g1, contacts);
    contacts.ToggleNormalInversion();
}

void Intersection::FindCircleComposite(const Geometry &g1, const Geometry &g2, ContactList &contacts)
{
    contacts.ToggleNormalInversion();
    FindCompositeGeometry(g2, g1, contacts);
    contacts.ToggleNormalInversion();
}

void Intersection::FindTerrainComposite(const Geometry &g1, const Geometry &g2, ContactList &contacts)
{
    contacts.ToggleNormalInversion();
    FindCompositeGeometry(g2, g1, contacts);
    contacts.ToggleNormalInversion();
}

bool Intersection::TestCompositeQuad(const Geometry &g1, const Geometry &g2)
{
    return TestCompositeGeometry(g1, g2);
}

bool Intersection::TestCompositeCircle(const Geometry &g1, const Geometry &g2)
{
    return TestCompositeGeometry(g1, g2);
}

bool Intersection::TestCompositeTerrain(const Geometry &g1, const Geometry &g2)
{
    return TestCompositeGeometry(g1, g2);
}

bool Intersection::TestCompositeComposite(const Geometry &g1, const Geometry &g2)
{
    return TestCompositeGeometry(g1, g2);
}

bool Intersection::TestQuadComposite(const Geometry &g1, const Geometry &g2)
{
    return TestCompositeGeometry(g2, g1);
}

bool Intersection::TestCircleComposite(const Geometry &g1, const Geometry &g2)
{
    return TestCompositeGeometry(g2, g1);
}

bool Intersection::TestTerrainComposite(const Geometry &g1, const Geometry &g2)
{
    return TestCompositeGeometry(g2, g1);
}
